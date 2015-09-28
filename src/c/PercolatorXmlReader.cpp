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
/**
 * Class definition for PercolatorXmlReader, a class for parsing the
 * xml output from percolator, versions 1.15 and later.
 */

#include "PercolatorXmlReader.h"
#include "BlibMaker.h"

using namespace std;

namespace BiblioSpec {

PercolatorXmlReader::PercolatorXmlReader(BlibBuilder& maker, 
                                         const char* file, 
                                         const ProgressIndicator* progress)
: BuildParser(maker, file, progress), 
  currentState_(START_STATE), 
  qvalueThreshold_(getScoreThreshold(SQT))
{
    this->setFileName(file); // this is done for the saxhandler
    qvalueBuffer_ = new char[256];
    qvalueBuffer_[0] = '\0';
    qvalueBufferPosition_ = qvalueBuffer_;
}

PercolatorXmlReader::~PercolatorXmlReader()
{
    delete [] qvalueBuffer_;
}

/**
 * \brief Implementation of BuildParser virtual function.  Reads the
 * Percolator xml file and populates the vector of PSMs with the
 * information.
 */
bool PercolatorXmlReader::parseFile() {

    parse(); // the saxhandler will read the file and store the psms

    vector<const char*> extensions;
    extensions.push_back(".ms2");
    extensions.push_back(".cms2");
    extensions.push_back(".bms2");
    extensions.push_back(".pms2");

    BlibBuilder tmpBuilder;

    if( fileMap_.size() > 1 )
        initSpecFileProgress((int)fileMap_.size());

    // for each source file, read the .sqt for mods info, add spec to lib
    map<string, vector<PSM*> >::iterator mapAccess = fileMap_.begin();
    for(; mapAccess != fileMap_.end(); ++mapAccess){
        string filename = mapAccess->first;
        setSpecFileName(filename.c_str(), extensions);

        vector<PSM*>& psms = mapAccess->second;

        // create an SQTReader and get mods
        string fullFilename = getPath(getFileName());
        replaceExtension(filename, "sqt");
        fullFilename += filename;
        SQTreader modsReader(tmpBuilder, fullFilename.c_str(), NULL);
        try{
            modsReader.openRead();
        } catch(BlibException& e){ 
            const char* msg = e.what();
            if( strncmp(msg, "Couldn't open", strlen("Couldn't open")) == 0 ){
                e.addMessage(" SQT file required for reading modifications.");
                throw e;
            }// ignore warning that perc wasn't run on the sqt
        }

        applyModifciations(psms, modsReader);

        psms_ = psms; // transfer to BuildParser list

        buildTables(PERCOLATOR_QVALUE);
        clearVector(psms_);
    }

    return true;
}

/**
 * Called when each start tag is read from the file. 
 */
void PercolatorXmlReader::startElement(const XML_Char* name,
                                       const XML_Char** attributes){

    switch(currentState_){
    case START_STATE:
        if(isElement("percolator_output", name)){
            currentState_ = ROOT_STATE;
        } 
        break;

    case ROOT_STATE:
        if(isElement("psms", name)){
            currentState_ = PSMS_STATE;
        } else if(isElement("peptides", name)){
            currentState_ = PEPTIDES_STATE;
        }
        break;

    case PSMS_STATE:
        if(isElement("psm", name)){
            parseId(attributes);
        } else if(isElement("q_value", name)){
            currentState_ = QVALUE_STATE;
            qvalueBufferPosition_ = qvalueBuffer_;//reset ptr into buffer 
        } else if(isElement("peptide_seq", name)){
            parseSequence(attributes);
        }
        break;

    case IGNORE_PSM_STATE:
    case QVALUE_STATE: //shouldn't actually get here
    case PEPTIDES_STATE:
        return;
    }

}

/**
 * Called when each closing tag is read from the file.
 */
void PercolatorXmlReader::endElement(const XML_Char* name) {
    switch(currentState_){
    case PSMS_STATE:
        if(isElement("psms", name)){
            currentState_ = ROOT_STATE;
        } else if(isElement("psm", name)){
            addCurPSM();
        }
        break;

    case IGNORE_PSM_STATE:
        if(isElement("psm", name)){
            currentState_ = PSMS_STATE;
        }
        break;
    case QVALUE_STATE:
        if(isElement("q_value", name)){
            // add the q-value to the curPSM_
            curPSM_->score = atof(qvalueBuffer_);
            if( curPSM_->score > qvalueThreshold_ ){
                currentState_ = IGNORE_PSM_STATE;
            } else {
                currentState_ = PSMS_STATE;
            }
        }
        break;
    default:
        return;
    }
}

/**
 * Given the attributes of a 'psm' tag, start a new PSM in which to
 * store data. Extract the filename, scan number, and charge from the
 * id.
 */
void PercolatorXmlReader::parseId(const XML_Char** attributes){

    const char* isdecoy = getAttrValue("p:decoy", attributes);
    if( isdecoy == NULL || strcmp("true", isdecoy)  == 0 ){
        currentState_ = IGNORE_PSM_STATE;
        return;
    }

    // if we have a current PSM, overwrite it, if not create a new one
    if( curPSM_ == NULL ){
        curPSM_ = new PSM();
    }

    const char* idStr = getRequiredAttrValue("p:psm_id", attributes);
    char* buffer = new char[strlen(idStr) + 1];
    strcpy(buffer, idStr);

    char* token = strtok(buffer, "_");
    if( token == NULL ) 
        throw BlibException(false, "Error parsing psm_id '%s'", idStr);

    // hijack the specName field to store the filename
    curPSM_->specName = token;

    token = strtok(NULL, "_");
    if( token == NULL ) 
        throw BlibException(false, "Error parsing psm_id '%s'", idStr);

    curPSM_->specKey = atoi(token);

    token = strtok(NULL, "_");
    if( token == NULL ) 
        throw BlibException(false, "Error parsing psm_id '%s'", idStr);

    curPSM_->charge = atoi(token);

    delete [] buffer;
}

/**
 * Read the seq attribute and add it to the current PSM.
 */
void PercolatorXmlReader::parseSequence(const XML_Char** attributes){
    if(curPSM_ == NULL){
        throw BlibException(false, "Encountered a peptide sequence with no "
                            "spectrum to assign it to.");
    }

    // for now store the modified sequence in the unmod seq slot
    curPSM_->unmodSeq = getRequiredAttrValue("seq", attributes);
}

/**
 * Read all values between tags.  We only want the q-values, so only
 * read if we are in the QVALUES_STATE.
 */
void PercolatorXmlReader::characters(const XML_Char *s, int len){
    if( currentState_ != QVALUE_STATE ){
        return;
    }

    // copy len characters into current position in the qvalue buffer
    strncpy(qvalueBufferPosition_, s, len);
    qvalueBufferPosition_[len] = '\0';
    // move the current position to the end
    qvalueBufferPosition_ += len;
}

/**
 * Evaluate the current PSM and save it with the appropriate filename
 * if it passes the q-value threshold.
 */
void PercolatorXmlReader::addCurPSM(){
    if(curPSM_ == NULL)
        throw BlibException(false, "No PSM was read for this 'psm' tag.");

    if(curPSM_->score < qvalueThreshold_){ // add the psm to the map
        // retrieve and remove the filename from the psm
        string filename = curPSM_->specName;
        curPSM_->specName.clear();

        Verbosity::comment(V_DETAIL, "For file %s adding PSM: "
                           "scan %d, charge %d, sequence '%s'.",
                           filename.c_str(), curPSM_->specKey,
                           curPSM_->charge, curPSM_->unmodSeq.c_str());

        map<string, vector<PSM*> >::iterator mapAccess = 
            fileMap_.find(filename);
        if( mapAccess == fileMap_.end() ){ // not found, add the file
            vector<PSM*> tmpPsms(1, curPSM_);
            fileMap_[filename] = tmpPsms;
        } else {  // add this psm to existing file entry
            (mapAccess->second).push_back(curPSM_);
        }
        curPSM_ = NULL;
    }
}

/**
 * Use the SQTreader to translate the sequences with modification
 * symbols to a vector of mods, a seq with bracketed masses, and an
 * unmodified seq.
 */
void PercolatorXmlReader::applyModifciations(vector<PSM*>& psms, 
                                             SQTreader& modsReader){
    size_t numPsms = psms.size();
    for(size_t i=0; i < numPsms; i++){
        PSM* psm = psms[i];
        // we were temporarily storing the modified seq here
        string modSeq = psm->unmodSeq;
        psm->unmodSeq.clear();
        modsReader.parseModifiedSeq(modSeq.c_str(), psm->unmodSeq, 
                                    psm->mods, false);
    }
}



} // namespace

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
