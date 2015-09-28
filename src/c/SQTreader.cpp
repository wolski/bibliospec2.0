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
 * Class definition for SQTreader, a class for parsing sqt files
 */

#include "SQTreader.h"
#include "BlibMaker.h"

using namespace std;

namespace BiblioSpec {

SQTreader::SQTreader(BlibBuilder& maker, 
                     const char* sqtfile, 
                     const ProgressIndicator* parent_progress)
: BuildParser(maker, sqtfile, parent_progress)
{
    // initialize mod arrays
    for(int i=0; i<MAX_MODS; i++) {
        staticMods[i]=0;
        diffMods[i] = 0;
    }
}

SQTreader::~SQTreader()
{
    if(file.is_open())
        file.close();
}


/**
 * Open sqt file for reading.  Read in header and leave filepointer at
 * beginning of first record.
 */
void SQTreader::openRead()
{
    if(file.is_open())
        file.close();

    file.open(getFileName().c_str());

    if(!file.is_open()) {
        throw BlibException(true, "Couldn't open '%s'.", getFileName().c_str());
    }

    //read through header to get the modifications
    string buffer;
    char thisLine[1024];
    char mods[512];
    bool percolated = false;
    while(file.peek() == 'H') {

        getline(file,buffer);
        if(buffer.find("StaticMod") != string::npos) {
            strcpy(thisLine, buffer.c_str());
            sscanf(thisLine, "%*c %*s %s", mods);
            float modValue;
            char modLetter;
            sscanf(mods, "%c%*c%f", &modLetter, &modValue);
      
            staticMods[(int)modLetter]=modValue;
        }
    
        if( buffer.find(" DiffMod") != string::npos || 
            buffer.find("	DiffMod") != string::npos) {
            size_t posEquals = buffer.find("=");
            if( posEquals == string::npos ){ cerr << "Blah" << endl; }
            char modSymbol = buffer[posEquals - 1];
            double modValue = atof(buffer.c_str() + posEquals +1);

            diffMods[(int)modSymbol] = modValue;
        }

        if(buffer.find("Percolator") != string::npos) {
            percolated = true;
        }
    }// next line
  
    if( percolated == false ){
        file.close();
        throw BlibException(true, 
                            "SQT file '%s' must be processed by Percolator.", 
                            getFileName().c_str());
    }

}

/**
 * \brief Implementation of BuildParser virtual function.  Reads the
 * SQT file and populates the vector of PSMs with the information.
 */
bool SQTreader::parseFile() {
    openRead();

    vector<const char*> extensions;
    extensions.push_back(".ms2");
    extensions.push_back(".cms2");
    extensions.push_back(".bms2");
    extensions.push_back(".pms2");
    string fileroot = getFileRoot(getFileName());
    setSpecFileName(fileroot.c_str(), extensions);

    //  parseFile_mine();
    extractPSMs();

    // close file
    if( file.is_open() ) {
        file.close();
    }
    return true;
}

/**
 * Read an sqt file beginning with the first S line. collect a
 * list of PSMs that pass score cutoff, populate the library tables
 * with spectrum information.
 */
void SQTreader::extractPSMs()
{
    string buffer;
    double scoreThreshold = getScoreThreshold(SQT);
    Verbosity::debug("Using Percolator q-value threshold %f",
                     scoreThreshold);
  
    while(!file.eof()) {
        if(!buffer.empty())
            buffer.clear();
        
        curPSM_ = new PSM();
        
        getline(file,buffer); //the S line
        
        wholePepSeq[0]='\0';
        // read the S line to get spectrum scan number, charge, mass
        if(buffer[0] == 'S') {
            sscanf(buffer.c_str(), "%*c %d %*d %d %*d %*s %lf %*f %*f %*d", 
                   &scanNumber,&charge, &precursorMH);
            curPSM_->charge = charge;
            curPSM_->specKey = scanNumber;
        }
        
        buffer.clear();
        getline(file,buffer); //the first M line
        // read the first M line to get score and sequence
        if(buffer[0] == 'M') {
            sscanf(buffer.c_str(), "%*c %*d %*d %*f %*f %*f %lf %*d %*d %s %*c",
                   &qvalue, wholePepSeq);
            curPSM_->score = -1 * qvalue; // q-values negatated by percolator
        }
        
        if( curPSM_->score > scoreThreshold ) {// good matches score 0 to threshold
            delete curPSM_;
            curPSM_ = NULL;
        } else {
            // get the unmodified seq and mods from the file's version of seq
            parseModifiedSeq(wholePepSeq, curPSM_->unmodSeq, curPSM_->mods);
            psms_.push_back(curPSM_);
            Verbosity::comment(V_DETAIL, "Saving PSM: scan %i, charge %i, "
                               "qvalue %.3g, seq %s.", curPSM_->specKey,
                               curPSM_->charge, curPSM_->score, 
                               curPSM_->unmodSeq.c_str());
            curPSM_ = NULL;
        }
        
        while(file.peek() != 'S') {
            if(!file.eof())
                getline(file,buffer);
            else
                break;
        }
        
    }
    
    buildTables(PERCOLATOR_QVALUE);
    
}


/**
 * \brief convert the sequence as presented in the sqt file into an
 * unmodified sequence and a vector of mods.
 *
 * The sqt file format of sequence is flankingAA.SEQ.flankingAA where
 * any modified residue in SEQ is followed by a key character (@%^)
 * whose mass shift is defined in the header.  Any static mods are
 * also defined in the header.  If hasFlankingAA is false the given
 * modSeq does not include the flanking sequences.
 */
void SQTreader::parseModifiedSeq(const char* modSeq, 
                                 string& unmodSeq, 
                                 vector<SeqMod>& mods,
                                 bool hasFlankingAA)
{

    string modSymbols = "*^$@#%"; // all the symbols we may encounter
    int start_idx = 2;            // skip the 'X.' of X.SEQ.X
    if( hasFlankingAA == false ){
        start_idx = 0;
    }

    // scan sequence for mod symbols, stopping before the flanking aa
    for(int i = start_idx; modSeq[i] != '.' && modSeq[i] != '\0'; i++){

        if(modSymbols.find(modSeq[i]) < string::npos) { //char is a mod symbol

            SeqMod newmod;
            newmod.position = i - 1 - start_idx;
            newmod.deltaMass = diffMods[(int)modSeq[i]];// check for numDiffMods
            mods.push_back(newmod);
        } else {
            unmodSeq += modSeq[i];
        }
    }

    // scan all residues for those with static mods
    for(unsigned int i=0; i < unmodSeq.size(); i++) {
        // look up residue in static mod mass array
        double modMass = staticMods[(int)unmodSeq[i]];
        if( modMass > 0 ) {
            SeqMod newmod;
            newmod.position = i + 1; // mods are 1-based in index
            newmod.deltaMass = modMass;
            mods.push_back(newmod);
        }
    }
}

} // namespace

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
