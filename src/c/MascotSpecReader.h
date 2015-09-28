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
#pragma once

/**
 *  The MascotSpecReader uses the ms_parser library to read spectra
 *  from a .dat file.  MascotSpecReader can be instantiated with a
 *  filename or with a ms_mascotresfile object.
 */

#include "Verbosity.h"
#include "BlibUtils.h"
#include "SpecFileReader.h"
#include "msparser.hpp"

using namespace matrix_science;

namespace BiblioSpec {

class MascotSpecReader : public SpecFileReader {
 public:
    MascotSpecReader(){
        ms_file_ = NULL;
        ms_results_ = NULL;
    }
    MascotSpecReader(const char* filename,
                     ms_mascotresfile* ms_file, 
                     ms_mascotresults* ms_results = NULL){
        setFile(ms_file, ms_results);
        filename_ = filename;
    }
    virtual ~MascotSpecReader(){
        delete ms_results_;
        delete ms_file_;
    }


    /** 
     * This is the SpecFileReader function.  Can call with an empty 
     * filename if setFile(ms_mascotresfile) has already been called.
     * If filename is not empty, will replace any ms_mascotresfile it
     * already has.
     * Throw error if file not found, can't be opened, wrong type.
     */ 
    virtual void openFile(const char* filename, bool mzSort){

        if( strcmp(filename, "") == 0 || filename == NULL) {
            if( ms_file_ == NULL ) {
                throw BlibException(false,
                     //                Verbosity::comment(V_DETAIL, 
                      "MascotSpecReader::openFile can only accept an "
                       "empty filename if it already has an ms_file.");
            } else {  // we already have an ms_file, we'll read that
                Verbosity::comment(V_STATUS, "Importing spectra from %s.", 
                                   filename_.c_str());
                return;
            }
        } else { //we have a new file to open

        Verbosity::comment(V_STATUS, "Importing spectra from %s.", 
                           filename);
            delete ms_file_;
            ms_file_ = new ms_mascotresfile(filename);
            if( !ms_file_->isValid() ) {
                throw BlibException(true,
                                    //Verbosity::comment(V_ERROR, 
                                    "'%s' is not a valid .dat file.", 
                                   filename);
            }
            if( !ms_file_->isMSMS() ){
                throw BlibException(true,
                                    // Verbosity::comment(V_ERROR, 
                                    "'%s' does not have spectra.",
                                    filename);

            }
        }
    }
    
    void setFile(ms_mascotresfile* ms_file, 
                 ms_mascotresults* ms_results = NULL){
        Verbosity::comment(V_DETAIL, "MascotSpecReader attaching open file.");
        //delete ms_file_;
        ms_file_ = ms_file;

        if( !ms_file_->isMSMS() ){
            throw BlibException(false,
                                //Verbosity::comment(V_ERROR, 
                                "The given .dat file with search results does not also have spectra.");
            // in the future, we could then let the caller look for another input file type.
        }

        ms_results_ = ms_results;
    }
    
    /**
     * Read file to find the spectrum corresponding to identifier.
     * Fill in mz and numPeaks in returnData
     * Allocate arrays for mzs and intensities and fill with peak
     * values. 
     * Return true if spectrum found and successfully parsed, false if
     * spec not in file.
    */
    virtual bool getSpectrum(int specId, 
                             SpecData& returnData, 
                             SPEC_ID_TYPE findBy,
                             bool getPeaks = true)
    {
        Verbosity::comment(V_DETAIL, "MascotSpecReader looking for spec %d.",
                           specId);


        // I can only find a way to get the precursor m/z from the
        // peptide
        ms_peptide* pep;
        ms_results_->getPeptide(specId, 1, pep); // rank 1
        returnData.mz = pep->getObserved();
        returnData.id = specId;

        ms_inputquery spec(*ms_file_, specId);
        // retention time is optional in .dat files
        string rtStr = spec.getRetentionTimes();
        try{
            returnData.retentionTime = boost::lexical_cast<double>(rtStr);
        } catch (...){
            returnData.retentionTime = 0;
        }
        returnData.numPeaks = spec.getNumberOfPeaks(1);// first ion series

        if( getPeaks ){
            returnData.mzs = new double[returnData.numPeaks];
            returnData.intensities = new float[returnData.numPeaks];

            vector<pair<double,double> > peaks = spec.getPeakList(1);
            sort(peaks.begin(), peaks.end(), 
                 compare_first_pair_doubles_descending);
            for(size_t peak_i = 0; peak_i < peaks.size(); peak_i++) {
                returnData.mzs[peak_i] = peaks[peak_i].first;
                returnData.intensities[peak_i] = (float)peaks[peak_i].second;
            }
        }

        return true;
    }

    virtual void setIdType(SPEC_ID_TYPE type){}

    virtual bool getSpectrum(string identifier, 
                             SpecData& returnData, 
                             bool getPeaks)
    {
        Verbosity::warn("MascotSpecReader cannot fetch spectra by string" 
                        "identifier, only by scan number");
        return false;
    }

    /**
     * For now, only specific spectra can be accessed from the Mascot
     * files.
     */
    virtual bool getNextSpectrum(SpecData& returnData, bool getPeaks){
        Verbosity::warn("MascotSpecReader does not support sequential "
                        "file reading.");
        return false;
    }
    
 private:
    string filename_;   // keep around for reporting purposes
    ms_mascotresfile* ms_file_;    // get spec from here
    ms_mascotresults* ms_results_; // get pep from here (for m/z)
};

} // namespace

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
















