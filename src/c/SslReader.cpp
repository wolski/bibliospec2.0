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
 * The SslReader parses the scan number, charge, sequence and spectrum
 * file name from the ssl file and stores each record.  Records are
 * sorted and grouped by file.  Spectra are then retrieved from the
 * spectrum files. 
 */

#include "SslReader.h"

namespace BiblioSpec {

  SslReader::SslReader(BlibBuilder& maker,
                       const char* sslname,
                       const ProgressIndicator* parent_progress)
    : BuildParser(maker, sslname, parent_progress)
  {
    Verbosity::debug("Creating SslReader.");
    sslName_ = sslname;
  }

  SslReader::~SslReader()
  {
    if( sslFile_.is_open() ){
      sslFile_.close();
    }
  };

  bool SslReader::parseFile(){
    Verbosity::debug("Parsing File.");
    if( ! openSsl() ){
      return false;
    }

    Verbosity::debug("Collecting Psms.");
    collectPsms();

    // for each ms2 file
    if( fileMap_.size() > 1 ){
      initSpecFileProgress((int)fileMap_.size());
    }

    map<string, vector<PSM*> >::iterator fileIterator = fileMap_.begin();
    for(; fileIterator != fileMap_.end(); ++fileIterator) {
      setSpecFileName((fileIterator->first).c_str());

      // move from map to psms_
      psms_ = fileIterator->second;

      // look at first psm for scanKey vs scanName
      if( psms_.front()->specKey == -1 ){ // default value
        lookUpBy_ = NAME_ID;
      } else {
        lookUpBy_ = SCAN_NUM_ID;
      }

      buildTables(UNKNOWN_SCORE_TYPE);
    }

    
    return true;
  }

  /**
   * Try to open .ssl file and read the header line.  Return false if
   * the file cannot be opened or if the header is incorrect.
   */
  bool SslReader::openSsl(){

    Verbosity::debug("Opening ssl File.");
    sslFile_.open(sslName_.c_str());
    if( !sslFile_.is_open() ){
      throw BlibException(true, "Could not open ssl file '%s'.", 
                          sslName_.c_str());
    }
    // confirm that header looks correct
    string line;
    getline(sslFile_, line);
    const char* header = "file\tscan\tcharge\tsequence\tmodifications";
    if( line.compare(0, strlen(header), header) ){
      throw BlibException(false,
                          "SSL header is not correct.  Should be '%s'.", 
                          header);
    }
    return true;
  }

  /**
   * Read the ssl file and parse all psms.  Store them by ms2 file
   * name.  Assumes .ssl file has been opened and is pointing to first
   * record.
   */
  void SslReader::collectPsms(){
    // for each line, get ms2 name, scan, charge, seq, mods

    int lineNum = 1;
    string ms2;
    string scanStr;
    string chargeStr;
    string modSeq;
    string line;
    map<string, vector<PSM*> >::iterator mapAccess;


    while( !sslFile_.eof() ){

      lineNum++;

      curPSM_ = new PSM();
      // read all values into temp strings
      sslFile_ >> ms2 >> scanStr >> chargeStr >> curPSM_->unmodSeq >> modSeq;
      // discard rest of line
      getline(sslFile_, line);

      // parse the scan id, either as an int or as a string
      try{
        curPSM_->specKey = boost::lexical_cast<int>(scanStr);
      } catch(bad_lexical_cast) {
        curPSM_->specName = scanStr;
      }

      // skip this if we didn't get a valid id
      if( curPSM_->specKey == -1 && curPSM_->specName.empty() ){//default values
        delete curPSM_;
        continue;
      }

      try{
        curPSM_->charge = boost::lexical_cast<int>(chargeStr);
      } catch(bad_lexical_cast) {
        Verbosity::error("The charge '%s' on line %d is not numeric.",
                         chargeStr.c_str(), lineNum);
      }

      // parse the mod seq into a vector of mods
      parseModSeq(curPSM_->mods, modSeq);

      // add the psm to the map
      mapAccess = fileMap_.find(ms2);
      if( mapAccess == fileMap_.end() ){ // add this file
        vector<PSM*> tmpPsms(1, curPSM_);
        fileMap_[ms2] = tmpPsms;
      } else { // add curPSM to this file
        (mapAccess->second).push_back(curPSM_);
      }

      ms2.clear();
      scanStr.clear();
      chargeStr.clear();
      modSeq.clear();
    } // next psm
  }

  void SslReader::parseModSeq(vector<SeqMod>& mods, 
                              string& modSeq){
    // find next [
    size_t nonAaChars = 0;// for finding aa position
    size_t openBracket = modSeq.find_first_of("[");
    while( openBracket != string::npos ){
      SeqMod mod;
      // get mass diff
      size_t closeBracket = modSeq.find_first_of("]", openBracket);
      string mass = modSeq.substr(openBracket + 1, 
                                  closeBracket - openBracket -1);
      mod.deltaMass = atof( mass.c_str() );
      // get position
      mod.position = openBracket - nonAaChars;
      nonAaChars += (closeBracket - openBracket + 1);
      // add to mods
      mods.push_back(mod);
      // find next
      openBracket = modSeq.find_first_of("[", openBracket + 1);
    }
  }

} // namespace



/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
