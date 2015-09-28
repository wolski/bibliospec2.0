/*
  Copyright (c) 2011, University of Washington
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer. 
    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution. 
    * Neither the name of the <ORGANIZATION> nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
/*
 *class definition for PepXMLreader.h
 */

#include "TandemNativeParser.h"
#include "BlibMaker.h"

using namespace std;

namespace BiblioSpec {

TandemNativeParser::TandemNativeParser(BlibBuilder& maker,
                                       const char* xmlfilename,
                                       const ProgressIndicator* parentProgress)
: BuildParser(maker, xmlfilename, parentProgress),
    probCutOff_(getScoreThreshold(TANDEM)),
    curState_(ROOT_STATE),
    mass_(0),
    numMzs_(0),
    numIntensities_(0)
{
   this->setFileName(xmlfilename); // this is for the saxhandler
   setSpecFileName(xmlfilename, // this is for the BuildParser
                   false); // don't look for the file
   mzs_ = NULL;
   intensities_ = NULL;

   // point to self as spec reader
   delete specReader_;
   specReader_ = this;
}

TandemNativeParser::~TandemNativeParser() {
    specReader_ = NULL; // so the parent class doesn't try to delete itself
    // free spectra
    map<int,SpecData*>::iterator it;
    for(it = spectra_.begin(); it != spectra_.end(); ++it){
        delete it->second;
        it->second = NULL;
    }
}

/**
 * Called by saxhandler when a new xml start tag is reached.  Collect
 * information from each tag according to element type and the current state.
 */
void TandemNativeParser::startElement(const XML_Char* name, 
                                       const XML_Char** attr)
{
    if(isElement("group", name)){
        parseGroup(attr);
    } else if(isElement("domain", name)){
        newState(DOMAIN_STATE);
        parseDomain(attr);
    } else if(isElement("aa", name)){
        parseMod(attr);
    } else if(isElement("GAML:Xdata", name) && curState_ == PEAKS_STATE){
        newState(PEAKS_MZ_STATE);
    } else if(isElement("GAML:Ydata", name) && curState_ == PEAKS_STATE){
        newState(PEAKS_INTENSITY_STATE);
    } else if(isElement("GAML:values", name)){
        parseValues(attr);
    }

}

/**
 * Called by saxhandler when the closing tag of an xml element is
 * reached.  Change the state based on the tag and the current state.
 */
void TandemNativeParser::endElement(const XML_Char* name)
{
    if(isElement("group", name)){
        endGroup();
    } else if (isElement("domain", name)){
        endDomain();
    } else if (isElement("GAML:Xdata", name) && 
               curState_ == PEAKS_MZ_STATE){
        curState_ = getLastState();
    } else if (isElement("GAML:Ydata", name) && 
               curState_ == PEAKS_INTENSITY_STATE){
        curState_ = getLastState();
    }
}

bool TandemNativeParser::parseFile()
{
    bool success = parse();

    if( success ){
        buildTables(TANDEM_EXPECTATION_VALUE);
    } 

    return success;
}

/**
 * Extract data from a group element.  Groups of type = model at the
 * highest level are a single PSM.  Nested groups of type = support
 * contain the peaks.
 */
void TandemNativeParser::parseGroup(const XML_Char** attr){
    const char* type = getRequiredAttrValue("type", attr);
    const char* label = getAttrValue("label", attr);

    // three possible group types: model, support, parameter
    if( strcmp(type, "model") == 0 ){
        curState_ = PSM_GROUP_STATE;
        parsePSM(attr);

    } else if( strcmp(type, "support") == 0 ){
        // cur state now either generic nested group or peaks group
        if( strcmp(label, "fragment ion mass spectrum") == 0  ){
            newState(PEAKS_STATE);
        } else {
            newState(NESTED_GROUP_STATE);
        }
    } // type == something else, (e.g. parameter)


}

/**
 * Read the sequence for this PSM.
 */
void TandemNativeParser::parseDomain(const XML_Char** attr){

    if( curPSM_ == NULL ){
        throw BlibException(false, "TandemNativeParser encountered a domain "
                            "without an accompanying model group.");
    } else if( curPSM_->unmodSeq.empty() ){
        curPSM_->unmodSeq = getRequiredAttrValue("seq", attr);
        seqStart_ = getIntRequiredAttrValue("start", attr);
    } else {
        // can we assume that the sequences at other domains are the same?
        string seq = getRequiredAttrValue("seq", attr);
        if( seq != curPSM_->unmodSeq ){
            throw BlibException(false, "Two different sequences given for id "
                                "%d, %s and %s.", curPSM_->specKey, 
                                curPSM_->unmodSeq.c_str(), seq.c_str());
        }
    }
}

/**
 * Get modification for the current sequence.  Only collect them for
 * the first domain element encountered.
 */
void TandemNativeParser::parseMod(const XML_Char** attr){

    if( curPSM_ == NULL ){
        throw BlibException(false, "TandemNativeParser encountered a mod"
                            "ification without an accompanying model group.");
    }

    const char* aa = getRequiredAttrValue("type", attr);
    int protPosition = getIntRequiredAttrValue("at", attr);
    double deltaMass = getDoubleRequiredAttrValue("modified", attr);

    Verbosity::debug("Found modified %s at position %d with delta mass %f.",
                      aa, protPosition, deltaMass);

    // change the position to be relative to the seq start, not the
    // protein start
    int seqPosition = protPosition - seqStart_; // + 1?

    // confirm that the modified aa is present in that position in the seq
    if( curPSM_->unmodSeq.at(seqPosition) != *aa ){
        throw BlibException(false,
                            "Specified modification does not match sequence. "
                            "Given a modified %c at position %d which is a "
                            "%c in %s.", *aa, seqPosition, 
                            curPSM_->unmodSeq.at(seqPosition), 
                            curPSM_->unmodSeq.c_str());
    }
    
    // create a new mod
    SeqMod mod;
    mod.deltaMass = deltaMass;
    mod.position = seqPosition + 1; // mods are 1-based
    curPSM_->mods.push_back(mod);
}

/**
 * Create a new PSM and store the info for the current element.
 */
void TandemNativeParser::parsePSM(const XML_Char** attr){
        // get id, mass, charge, score
        curPSM_ = new PSM();
        curPSM_->charge = getIntRequiredAttrValue("z", attr);      
        curPSM_->specKey = getIntRequiredAttrValue("id", attr);            
        curPSM_->score = getDoubleRequiredAttrValue("expect", attr);      
        mass_ = getDoubleRequiredAttrValue("mh", attr);      
}

/**
 * Prepare to get the array of m/z values for the scan or the array of
 * intensity values.
 */
void TandemNativeParser::parseValues(const XML_Char** attr){

    int numValues = getIntRequiredAttrValue("numvalues", attr);
    if(curState_ == PEAKS_MZ_STATE){
        numMzs_ = numValues;
        mzs_ = new double[numMzs_]; 

    } else if(curState_ == PEAKS_INTENSITY_STATE ){
        numIntensities_ = numValues;
        intensities_ = new float[numIntensities_ ]; 

    } // else values for some other kind of data we don't care about

}

/**
 * Handler for all characters between tags.  We are only interested in
 * the peaks data in 
 * <group type="support" label="fragment ion mass spectrum">
 *    <GAML:Xdata><GAML:values>
 * and
 *    <GAML:Ydata><GAML:values>
 * Use curState to determine if we are there.
 */
void TandemNativeParser::characters(const XML_Char *s, int len){
    if( curState_ != PEAKS_MZ_STATE && curState_ != PEAKS_INTENSITY_STATE ){
        return;
    }

    // grab the data
    char* buf = new char[len + 1];
    strncpy(buf, s, len);
    buf[len] = '\0';  

    // concatinate it on to the appropriate string
    if( curState_ == PEAKS_MZ_STATE){
        mzStr_ += buf;
    } else if( curState_ == PEAKS_INTENSITY_STATE){
        intensityStr_ += buf;
    }

    // clean up
    delete [] buf;
}

void TandemNativeParser::getPeaks(
  istringstream& tokenizer, ///< string version of values
  double* array,            ///< store values here
  int maxSize)              ///< array size
{
    // store values
    int curIdx = 0;
    while( !tokenizer.eof() ){
        assert(curIdx < maxSize);
        tokenizer >> array[curIdx];
        curIdx++;
    }
}

void TandemNativeParser::getPeaks(
  istringstream& tokenizer, ///< string version of values
  float* array,            ///< store values here
  int maxSize)              ///< array size
{
    // store values
    int curIdx = 0;
    while( !tokenizer.eof() ){
        assert(curIdx < maxSize);
        tokenizer >> array[curIdx];
        curIdx++;
    }
}

/**
 * At the end of a group element, update the state.  If it is a PSM
 * group element, save the spectrum if the PSM passes the score
 * threshold and delete the PSM. The PSM should have been saved when
 * its domain element ended.
 */
void TandemNativeParser::endGroup(){

    if( curState_ == NESTED_GROUP_STATE ){
        curState_ = getLastState();
        
    } else if( curState_ == PEAKS_STATE ){
        curState_ = getLastState();

    } else if( curState_ == PSM_GROUP_STATE ){
        curState_ = ROOT_STATE;
        Verbosity::debug("Cur psm has id %d, charge %d, score %f, "
                         "mass %f, seq %s",
                         curPSM_->specKey, curPSM_->charge, 
                         curPSM_->score, mass_, curPSM_->unmodSeq.c_str());
        // keep psm if score passes threshold
        if( curPSM_->score <= probCutOff_ ){
            saveSpectrum();
        }
        delete curPSM_;
        curPSM_ = NULL;
        clearCurPeaks();
        
    }
}

/**
 * At the end of a domain element, update the state and save the PSM.
 * Create a copy of the PSM, clearing the sequence and modification
 * information in case there is another sequence (domain element)
 * associated with this spectrum.
 */
void TandemNativeParser::endDomain(){

    curState_ = getLastState();
    if( curPSM_->score <= probCutOff_ ){
        psms_.push_back(curPSM_);
        // create a copy of the current
        PSM* tmpPSM = new PSM();
        tmpPSM->charge = curPSM_->charge;
        tmpPSM->specKey = curPSM_->specKey;
        tmpPSM->score = curPSM_->score;
        curPSM_ = tmpPSM;
    } else { // or if we are not going to accept it, just clear the seq
        curPSM_->unmodSeq.clear();
        curPSM_->mods.clear();
    }
}

/**
 * Convert the string where we collected all the lines containing
 * peaks data into two arrays of m/z and intensity values.
 */
void TandemNativeParser::stringsToPeaks(){

    // replace newlines with space
    replaceAllChar(mzStr_, '\n', ' ');
    replaceAllChar(intensityStr_, '\n', ' ');

    // remove whitespace at the end
    deleteTrailingWhitespace(mzStr_);
    deleteTrailingWhitespace(intensityStr_);
    
    istringstream tokenizer(mzStr_);
    getPeaks(tokenizer, mzs_, numMzs_);

    istringstream tokenizer2(intensityStr_);
    getPeaks(tokenizer2, intensities_, numIntensities_);

}

/**
 * Transition to a new state (usually at the start of a new element)
 * by saveing the current in the history stack and setting the current
 * to the new.
 */
void TandemNativeParser::newState(STATE nextState){
    stateHistory_.push_back(curState_);
    curState_ = nextState;
}

/**
 * Return to the previous state (usually at the end of an element) by
 * popping the last state off the stack and setting the current to it.
 */
TandemNativeParser::STATE TandemNativeParser::getLastState(){
    STATE lastState = stateHistory_.at(stateHistory_.size() - 1);
    stateHistory_.pop_back();
    return lastState;
}

/**
 * Delete the arrays of mz's and intensities, set the size of those
 * two arrays to zero, and clear the peak strings.  
 */
void TandemNativeParser::clearCurPeaks(){
    delete [] mzs_;
    delete [] intensities_;

    numMzs_ = 0;
    numIntensities_ = 0;

    mzStr_.clear();
    intensityStr_.clear();
}

/**
 * Saves the current data as a new spectrum in the spectra map.
 * Computes precursor mz from mass and charge.  Clears the current
 * data in preparation for the next PSM to parse.
 */
void TandemNativeParser::saveSpectrum(){
    // parse the string list of peaks into arrays
    stringsToPeaks();

    // confirm that we have the same number of m/z's and intensities
    if( numIntensities_ != numMzs_ ){
        // TODO get line number
        throw BlibException(false, "Different numbers of peaks. Spectrum %d "
                            "has %d fragment m/z values and %d intensities.",
                            curPSM_->specKey, numMzs_, numIntensities_);
    }

    // compute the mz
    double mz = mass_ / curPSM_->charge; // TODO extra H+??

    // create a new SpecData
    SpecData* curSpec = new SpecData();
    curSpec->mz = mz;
    curSpec->retentionTime = retentionTime_;
    curSpec->numPeaks = numMzs_;
    curSpec->mzs = mzs_;
    curSpec->intensities = intensities_;
    mzs_ = NULL;
    intensities_ = NULL;

    // add to map
    // check to see if it is already there?
    spectra_[curPSM_->specKey] = curSpec;    

}

// SpecFileReader methods
/**
 * Implemented to satisfy SpecFileReader interface.  Since spec and
 * results files are the same, no need to open a new one.
 */
void TandemNativeParser::openFile(const char* filename, bool mzSort){
    return;
}

void TandemNativeParser::setIdType(SPEC_ID_TYPE type){}

/**
 * Return a spectrum via the returnData argument.  If not found in the
 * spectra map, return false and leave returnData unchanged.
 */
bool TandemNativeParser::getSpectrum(int identifier, 
                                     SpecData& returnData,
                                     SPEC_ID_TYPE findBy,
                                     bool getPeaks){
    map<int,SpecData*>::iterator found = spectra_.find(identifier);
    if( found == spectra_.end() ){
        return false;
    }

    SpecData* foundData = found->second;
    returnData = *foundData;
    return true;
}

bool TandemNativeParser::getSpectrum(string identifier, 
                                     SpecData& returnData, 
                                     bool getPeaks){
    Verbosity::warn("TandemNativeParser cannot fetch spectra by string "
                    "identifier, only by scan number.");
    return false;
}

/**
 *  For now, only specific spectra can be accessed from the
 *  TandemNativeParser.
 */
bool TandemNativeParser::getNextSpectrum(SpecData& returnData, bool getPeaks){
    Verbosity::warn("TandemNativeParser does not support sequential file "
                    "reading.");
    return false;

}

} // namespace


/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
