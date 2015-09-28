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

#include "PepXMLreader.h"
#include "BlibMaker.h"

using namespace std;

namespace BiblioSpec {

static const int STATE_ROOT = 0;
static const int STATE_PROPHET_SUMMARY = 1;
static const int STATE_SEARCH_HIT_BEST = 5;
static const int STATE_SEARCH_HIT_BEST_SEEN = 6;

PepXMLreader::PepXMLreader(BlibBuilder& maker,
                           const char* xmlfilename,
                           const ProgressIndicator* parentProgress)
: BuildParser(maker, xmlfilename, parentProgress),
  analysisType_(UNKNOWN_ANALYSIS),
  scoreType_(PEPTIDE_PROPHET_SOMETHING),
  state(STATE_ROOT)
{
    this->setFileName(xmlfilename); // this is for the saxhandler
    numFiles = 0;
    pepProb = 0;
    probCutOff = getScoreThreshold(PEPXML);
    dirs.push_back("../");   // look in parent dir in addition to cwd
    dirs.push_back("../../");  // look in grandparent dir in addition to cwd
    extensions.push_back(".mzML"); // look for spec in mzXML files
    extensions.push_back(".mzXML"); // look for spec in mzXML files
}

PepXMLreader::~PepXMLreader() {
}

void PepXMLreader::startElement(const XML_Char* name, const XML_Char** attr)
{
   if(isElement("peptideprophet_summary",name)) {
       analysisType_ = PEPTIDE_PROPHET_ANALYSIS;
       state = STATE_PROPHET_SUMMARY;
   } else if(state == STATE_PROPHET_SUMMARY && isElement("inputfile",name)) {
      // Count files for use in reporting percent complete
      numFiles++;
   } else if(isElement("msms_run_summary",name)) {
      string fileroot = getRequiredAttrValue("base_name",attr);
      // Because Mascot2XML uses the full path for the base_name,
      // only the part beyond the last "\" or "/" is taken.
      size_t slash = fileroot.rfind('/');
      size_t bslash = fileroot.rfind('\\');
      if (slash == string::npos || (bslash != string::npos && bslash > slash))
          slash = bslash;
      if (slash != string::npos)
          fileroot.erase(0, slash + 1);
          
      setSpecFileName(fileroot.c_str(), extensions, dirs); 
   }

   //get massType and search engine
   if(isElement("search_summary",name)) {
       const char* search_engine = getAttrValue("search_engine",attr);
       if(strncmp("Spectrum Mill", search_engine,
                  strlen("Spectrum Mill")) == 0 ) {
           Verbosity::comment(V_DEBUG, "Pepxml file is from Spectrum Mill.");
           analysisType_ = SPECTRUM_MILL_ANALYSIS;
           scoreType_ = SPECTRUM_MILL;
           probCutOff = 0; // accept all psms

           lookUpBy_ = INDEX_ID; 
           specReader_->setIdType(INDEX_ID);
       } else if(strncmp("OMSSA", search_engine, strlen("OMSAA")) == 0) {
           Verbosity::debug("Pepxml file is from OMSAA.");
           analysisType_ = OMSSA_ANALYSIS;
           scoreType_ = OMSSA_EXPECTATION_SCORE;
           probCutOff = getScoreThreshold(OMSSA);
       } // else assume peptide prophet 
       
       if(strcmp("monoisotopic",getAttrValue("fragment_mass_type",attr)) == 0)
           massType = 1;
       else
           massType = 0;
       
       AminoAcidMasses::initializeMass(aminoacidmass, massType);

   } else if(isElement("spectrum_query", name)) {
       // is it better to do this at the start of the element or the end?
       scanNumber=0;
       charge=0;
       precursorMZ=0;
       pepProb = 0;
       pepSeq[0]='\0';
       mods.clear();
       
       int minCharge = 1;
       
       // if prophet type
       if( analysisType_ == PEPTIDE_PROPHET_ANALYSIS ||
           analysisType_ == OMSSA_ANALYSIS ) {
           scanNumber = getIntRequiredAttrValue("start_scan",attr);
       }
       // if spectrum mill type
       if( analysisType_ == SPECTRUM_MILL_ANALYSIS ) {
           spectrumName = getRequiredAttrValue("spectrum",attr);
           scanNumber = -1;
           minCharge = 0;
           // In case this is necessary for calculating charge
           precursorMZ = atof(getAttrValue("precursor_m_over_z",attr));
       }
       charge = getIntRequiredAttrValue("assumed_charge",attr, minCharge, 20);
   } else if(isElement("search_hit", name)) {
       // Only use this search hit, if rank is 1 or missing (zero)
       if (atoi(getAttrValue("hit_rank",attr)) < 2 && state == STATE_ROOT) {
           strcpy(pepSeq,getRequiredAttrValue("peptide",attr));
           if(charge == 0) {
               if(precursorMZ == 0 || *getAttrValue("parentCharge",attr) != '\0')
                   charge = getIntRequiredAttrValue("parentCharge",attr, 1, 10);
               else {
                   // If all else fails with Spectrum Mill, use the pecursor m/z and
                   // neutral mass to calculate the charge.
                   double neutralMass = getDoubleRequiredAttrValue("calc_neutral_pep_mass",attr);
                   charge = (int)((neutralMass / precursorMZ) + 0.5);
               }
           }
           state = STATE_SEARCH_HIT_BEST;
       }
   } else if(isElement("mod_aminoacid_mass", name) && state == STATE_SEARCH_HIT_BEST) {
       // position is 1-based
       modPosition = getIntRequiredAttrValue("position",attr);
       modMass = getDoubleRequiredAttrValue("mass",attr);
       
       SeqMod curmod;
       curmod.position = modPosition;
       if( (int)strlen(pepSeq) < modPosition || modPosition < 1 ) {
           throw BlibException(false, "Cannot modify sequence %s at position "
                               "%i which is beyond its length (%i).", 
                               pepSeq, modPosition-1, strlen(pepSeq));
       }
       
       curmod.deltaMass = modMass - aminoacidmass[(int)pepSeq[modPosition-1]];
       
       mods.push_back(curmod);
   } else if(isElement("peptideprophet_result", name)) {
       pepProb = getDoubleRequiredAttrValue("probability",attr);      
   } else if(state == STATE_SEARCH_HIT_BEST && isElement("search_score", name)
             && strcmp(getAttrValue("name", attr), "expect") == 0){
       pepProb = getDoubleRequiredAttrValue("value", attr);
   }
   // no score for spectrum mill
   // mascot score is ??
}

void PepXMLreader::endElement(const XML_Char* name)
{
    if(isElement("peptideprophet_summary",name)) {
        state = STATE_ROOT;
        // now we know the number of files
        if( numFiles > 1 )
            initSpecFileProgress(numFiles);
    } else if(isElement("msms_run_summary", name)) {      
        if( analysisType_ == UNKNOWN_ANALYSIS ){
            throw BlibException(false, "The .pep.xml file is not from one of "
                                "the recognized sources (PeptideProphet, "
                                "SpectrumMill, OMSSA).");
        }
        // if we are using pep.xml from Spectrum mill, we still don't have
        // scan numbers/indexes, here's a hack to get them
        if( analysisType_ == SPECTRUM_MILL_ANALYSIS ) {
            findScanIndexFromName();
        }
        buildTables(scoreType_);
        
        // reset values for next 
        mzXMLFile[0]='\0';
        massType = 0;
        for(int i=0; i<128; i++) {
            aminoacidmass[i]=0;
        }
    } else if(isElement("spectrum_query", name)) {
        // check that we found all the values we need
        // charge and scanNumber were required attributes of spectrum_query
        // mods and spectrumName optional
        // no prob for spectrum mill
        // if prob not found for pep proph or mascot, default value is -1 and the psm will quietly be ignored
        // mascot has spectra with no peptides, but could report warning if spectrum mill or peptide prophet don't have a peptide sequence
        
        if( scorePasses(pepProb) && (int)strlen(pepSeq) > 0) {
            curPSM_ = new PSM();
            curPSM_->charge = charge;
            curPSM_->unmodSeq = pepSeq;
            curPSM_->specKey = scanNumber;
            curPSM_->score = pepProb;
            curPSM_->mods = mods;
            curPSM_->specName = spectrumName;
            
            Verbosity::comment(V_DETAIL, "Adding psm.  Scan %d, charge %d, "
                               "score %.2f, seq %s, name %s.",
                               scanNumber, charge, pepProb, pepSeq, 
                               spectrumName.c_str());
             psms_.push_back(curPSM_);
            curPSM_ = NULL;
        }
        
        // reset for next query
        // TODO (BF: Aug-13-09): would be faster/cleaner if we just kept an object at curPsm and filled values into that instead of using charge,pepSeq, etc.
        // this is reset at the beginning of the element, which is better?
        charge = 0;
        precursorMZ = 0;
        pepSeq[0] = '\0';
        scanNumber = -1;
        pepProb = 0;
        spectrumName.clear();
        
        mods.clear();
        state = STATE_ROOT;
    } else if(isElement("search_hit", name)) {
        if (state == STATE_SEARCH_HIT_BEST)
            state = STATE_SEARCH_HIT_BEST_SEEN;
    }
}

bool PepXMLreader::parseFile()
{
   
   return parse();
}

/**
 * Decide if the score passes the required threshold for the given
 * search analysis.
 * \returns True if score passes and PSM should be included in
 * library, else false.
 */
bool PepXMLreader::scorePasses(double score){
    switch(analysisType_){
    case PEPTIDE_PROPHET_ANALYSIS:
        if(score >= probCutOff){ 
            return true;
        }
        break;

    case SPECTRUM_MILL_ANALYSIS:
        return true;

    case OMSSA_ANALYSIS:
        if(score < probCutOff){
            return true;
        }
        break;

    case UNKNOWN_ANALYSIS:
        return false;
    }
    return false;
}


} // namespace

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
