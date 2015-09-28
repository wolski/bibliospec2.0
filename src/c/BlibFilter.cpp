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
 * This programs takes a redundant library and
 * compute all_vs_all, select one final spec into
 * the non_redundant library.
 *
 * $ LibFilter.cpp, v 1.0 2009/01/27 15:53:52 Ning Zhang Exp $
 */

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sqlite3.h>
#include <map>
#include <vector>
#include <time.h>
#include "zlib.h"
#include "RefSpectrum.h"
#include "PeakProcess.h"
#include "DotProduct.h"
#include "Match.h"
#include "BlibMaker.h"
#include "ProgressIndicator.h"
#include "BlibUtils.h"
#include "Verbosity.h"
#include "CommandLine.h"
#include "boost/program_options.hpp"

using namespace std;
namespace ops = boost::program_options;

namespace BiblioSpec {

class BlibFilter : public BlibMaker
{
 public:
    BlibFilter();
    ~BlibFilter();
    void parseCommandLine(const int argc, char** const argv,
                          ops::variables_map& options_table);
    void init();
    virtual void commit();
    virtual void attachAll();
    void buildNonRedundantLib();

 protected:
    virtual string getLSID();
    virtual void getNextRevision(int* major, int* minor);
    vector<PEAK_T> getUncompressedPeaks(int& numPeaks,
                                        int& mzLen, Byte* comprM, 
                                        int& intensityLen, Byte* comprI);
    void compAndInsert(vector<RefSpectrum*>& oneIon);

 private:
    string redundantFileName_;  // The name of the file being filtered
    // filtered lib name stored by BlibMaker
    const char* redundantDbName_; // The name it's given as an attached db
    int minPeaks_;        // Spectrum must have this many peaks to be included
    double minAverageScore_; // don't include best spec if average dotp is lower

    bool redundantLibHasAdditionalColumns_;
    char zSql[2048];

    void getCommandLineValues(ops::variables_map& options_table);
};
} // namespace

using namespace BiblioSpec;

/**
 * The starting point for BlibFilter, program to select one spectrum
 * for each peptide ion as the best representative of a redundant
 * library and store it in a new library.
 */
int main(int argc, char* argv[])
{
    // declare storage for options values
    ops::variables_map options_table;

    BlibFilter filter;
    filter.parseCommandLine(argc, argv, options_table);

    filter.init();

    filter.beginTransaction();
    Verbosity::debug("About to begin filtering.");
    filter.buildNonRedundantLib();
    Verbosity::debug("Finished filtering.");
    filter.endTransaction();
    filter.commit();
}

BlibFilter::BlibFilter()
{
    redundantDbName_ = "redundant";
    minPeaks_ = 20; 
    minAverageScore_ = 0;
    redundantLibHasAdditionalColumns_ = true;
    // Never append to a non-redundant library
    setOverwrite(true);
    setRedundant(false);
}

BlibFilter::~BlibFilter(){
}

/**
 * Read the given command line and store option and argument values in
 * the given variables_map.  Also reads parameter file with command
 * line values overriding file values.  Sets appropriate values for
 * the BlibFilter object (filenames, options).
 *
 * Exits on error if unexpected option found, if option is missing its
 * argument, if required argument is missing.
 */
void BlibFilter::parseCommandLine(const int argc, 
                                  char** const argv,
                                  ops::variables_map& options_table){

    // define the optional command line args 
    options_description optionsDescription("Options");
    try{

        optionsDescription.add_options()
            ("memory-cache,m",
             value<int>()->default_value(250),
             "SQLite memory cache size in Megs.  Default 250M.")

            ("min-peaks,n",
             value<int>()->default_value(20),
             "Only include spectra with at least this many peaks.  Default 20.")

            ("min-score,s",
             value<double>()->default_value(0),
             "Best spectrum must have at least this average score to be included.  Default 0.")

            ;

        // define the required command line args
        vector<const char*> argNames;
        argNames.push_back("redundant-library");
        argNames.push_back("filtered-library");

        // create a CommandLine object to do the dirty work
        BiblioSpec::CommandLine parser("BlibFilter", 
                                       optionsDescription,
                                       argNames, 
                                       false);// no multiple values
                                              // for last arg

        // read command line and param file (if given)
        parser.parse(argc, argv, options_table);

    } catch(std::exception& e) {
        cerr << "ERROR: " << e.what() << "." << endl << endl;
        exit(1);
    } catch(...) {
        BiblioSpec::Verbosity::error("Encountered exception of unknown type "
                                     "while parsing command line.");
    }

    getCommandLineValues(options_table);
}

/**
 * Save the needed values parsed from the command line.
 */
void BlibFilter::getCommandLineValues(ops::variables_map& options_table){
    redundantFileName_ = options_table["redundant-library"].as<string>();
    minPeaks_ = options_table["min-peaks"].as<int>();
    minAverageScore_ = options_table["min-score"].as<double>();
    setLibName(options_table["filtered-library"].as<string>());
}

void BlibFilter::attachAll()
{
    Verbosity::status("Filtering redundant library '%s'.",
                      redundantFileName_.c_str());
    sprintf(zSql, "ATTACH DATABASE '%s' as %s", redundantFileName_.c_str(), 
            redundantDbName_);
    sql_stmt(zSql);
}

void BlibFilter::commit()
{
    BlibMaker::commit();
    
    string detachCmd = "DETACH DATABASE ";
    detachCmd += redundantDbName_;
    sql_stmt(detachCmd.c_str());
}

string BlibFilter::getLSID()
{
    // Use the same LSID as the redundant version, but replace
    // 'redundant' with 'nr'.
    sprintf(zSql, "SELECT libLSID FROM %s.LibInfo", redundantDbName_);
    
    int iRow, iCol;
    char** result;
    int rc = sqlite3_get_table(getDb(), zSql, &result, &iRow, &iCol, 0);
    
    check_rc(rc, zSql);
    
    string libLSID = result[1];
    string redundant = ":redundant:";
    libLSID.replace(libLSID.find(redundant), redundant.length(), ":nr:");
    
    sqlite3_free_table(result);
    return libLSID;
}

void BlibFilter::getNextRevision(int* major, int* minor)
{
    // Use same revision as the redundant version
    sprintf(zSql, "SELECT majorVersion, minorVersion FROM %s.LibInfo", 
            redundantDbName_);
    
    int iRow, iCol;
    char** result;
    int rc = sqlite3_get_table(getDb(), zSql, &result, &iRow, &iCol, 0);
    
    check_rc(rc, zSql);  // does this also check that there is at least one row returned?
    
    *major = atoi(result[2]);
    *minor = atoi(result[3]);
    
    sqlite3_free_table(result);
}

/**
 *  Initialize a new filtered library.  First call the parent classes
 *  init method which opens a new file (taking care of overwriting) and
 *  creates standard tables.  Here add additional tables required for
 *  filtered libraries: PeptideIons and RetentionTimes.
 */
void BlibFilter::init(){
    BlibMaker::init();
    strcpy(zSql,
           "CREATE TABLE RetentionTimes (PeptideID INTEGER, "
           "retentionTime REAL, "
           "FOREIGN KEY(PeptideID) REFERENCES PeptideIons(peptideID) )" );
    sql_stmt(zSql);

    strcpy(zSql,
           "CREATE TABLE PeptideIons (id INTEGER PRIMARY KEY autoincrement, "
           "spectrumID INTEGER, "
           "bareSequence VARCHAR(150), "
           "modifiedSequence VARCHAR(200), "
           "charge TINYINT, "
           "FOREIGN KEY(spectrumID) REFERENCES RefSpectrum(id) )"
           );
    sql_stmt(zSql);

}

void BlibFilter::buildNonRedundantLib()
{
    Verbosity::debug("Starting buildNonRedundant.");

    string msg = "ERROR: Failed building library ";
    msg += getLibName();
    setMessage(msg.c_str());

    // first copy over all of the spectrum source files
    transferSpectrumFiles(redundantDbName_);

    // find out if we have retention times and other additional columns
    const char* optional_cols = "";
    if( tableColumnExists(redundantDbName_, "RefSpectra", "retentionTime") ){
        optional_cols = ", retentionTime ";
        redundantLibHasAdditionalColumns_ = true;
    } else {
        redundantLibHasAdditionalColumns_ = false;
    }

    vector<RefSpectrum*> oneIon;
    char lastPepModSeq[1024], pepModSeq[1024];
    lastPepModSeq[0]='\0';
    pepModSeq[0]='\0';

    int lastCharge=0, charge=0;

    Verbosity::debug("Counting Spectra.");
    ProgressIndicator progress(getSpectrumCount(redundantDbName_));

    Verbosity::debug("Sorting spectra by sequence and charge.");
    //first Order by peptideModSeq and charge, filter by num peaks
    sprintf(zSql,
            "SELECT id,peptideSeq,precursorMZ,precursorCharge,peptideModSeq,"
            "prevAA, nextAA, numPeaks, peakMZ, peakIntensity %s "
            "FROM %s.RefSpectra, %s.RefSpectraPeaks "
            "WHERE %s.RefSpectra.id=%s.RefSpectraPeaks.RefSpectraID "
            " and %s.RefSpectra.numPeaks >= %i "
            "ORDER BY peptideModSeq, precursorCharge", optional_cols,
            redundantDbName_, redundantDbName_, redundantDbName_,
            redundantDbName_, redundantDbName_, minPeaks_);

    smart_stmt pStmt;
    int rc = sqlite3_prepare(getDb(), zSql, -1, &pStmt, 0);

    check_rc(rc, zSql, 
             "Failed selecting redundant spectra for comparison.");
    Verbosity::debug("Successfully sorted.");

    rc = sqlite3_step(pStmt);

    // for each spectrum entry in table
    while( rc==SQLITE_ROW ) {
        progress.increment();
        
        // create a RefSpectrum object and populate all fields
        // then get charge and seq
        // TODO: RefSpectrum* tmpRef = nextRefSpec(pStmt);

        RefSpectrum* tmpRef = new RefSpectrum();

        strcpy(pepModSeq,
               reinterpret_cast<const char*>(sqlite3_column_text(pStmt,4)));
        charge = sqlite3_column_int(pStmt,3);
        
        tmpRef->setLibSpecID(sqlite3_column_int(pStmt,0));
        tmpRef->setSeq(reinterpret_cast<const char*>(sqlite3_column_text(pStmt,
                                                                         1)));
        tmpRef->setMz(sqlite3_column_double(pStmt,2));
        tmpRef->setCharge(charge);
        // if not selected, value == 0
        tmpRef->setRetentionTime(sqlite3_column_double(pStmt, 10));
        tmpRef->setMods(pepModSeq);
        tmpRef->setPrevAA("-");
        tmpRef->setNextAA("-");
        
        int numPeaks = sqlite3_column_int(pStmt,7);

        int numBytes1=sqlite3_column_bytes(pStmt,8);
        Byte* comprM = (Byte*)sqlite3_column_blob(pStmt,8);
        int numBytes2=sqlite3_column_bytes(pStmt,9);
        Byte* comprI = (Byte*)sqlite3_column_blob(pStmt,9);
        
        
        // is this slow for copying the peak vector? better to return a ptr?
        vector<PEAK_T> peaks = getUncompressedPeaks(numPeaks, numBytes1,
                                                    comprM, numBytes2, comprI);
        if (peaks.size() == 0) {
            Verbosity::error("Unable to read peaks for redundant library "
                             "spectrum %i, sequence %s, charge %i.",
                             tmpRef->getLibSpecID(), (tmpRef->getSeq()).c_str(),
                             tmpRef->getCharge());
        }
        tmpRef->setRawPeaks(peaks);
        // TODO end nextRefSpec

        // if this spec has same seq and charge, add to the collection
        if(strcmp(pepModSeq,lastPepModSeq) == 0 && lastCharge == charge) {
            oneIon.push_back(tmpRef);
        } else {// filter & start new collection for a different seq and charge
            
            if(!oneIon.empty()) {
                Verbosity::comment(V_DETAIL, "Selecting spec for %s, charge %i"
                                   " from %i spectra.", lastPepModSeq,
                                   lastCharge, oneIon.size());
                compAndInsert(oneIon);
                clearVector(oneIon);
            }
            
            oneIon.push_back(tmpRef);
            strcpy(lastPepModSeq, pepModSeq);
            lastCharge = charge;
            Verbosity::comment(V_DETAIL, "Collecting spec for %s, charge %i,",
                               pepModSeq, charge);
        }
        
        rc = sqlite3_step(pStmt);
    }// next table entry
    
    // Insert the last spectrum
    if (!oneIon.empty()) {
        progress.increment();
        Verbosity::comment(V_DETAIL, "Selecting spec for %s, charge %i"
                           " from %i spectra.", lastPepModSeq,
                           lastCharge, oneIon.size());
        compAndInsert(oneIon);
        clearVector(oneIon);
    }
    // we may have selected fewer spectra than were in the library
    // update the progress indicator
    progress.finish();
}

vector<PEAK_T> BlibFilter::getUncompressedPeaks(int& numPeaks,
                                                int& mzLen, Byte* comprM, 
                                                int& intensityLen, Byte* comprI)
{
    //variables for compressed files
    uLong uncomprLen;
    double *mz;
    float *intensity;

    uncomprLen=numPeaks*sizeof(double);
    if ((int)uncomprLen == mzLen)
        mz = (double*) comprM;
    else {
        mz = new double[numPeaks];
        uncompress((Bytef*)mz, &uncomprLen, comprM, mzLen);
    }

    uncomprLen=numPeaks*sizeof(float);
    if ((int)uncomprLen == intensityLen)
        intensity = (float*) comprI;
    else {
        intensity = new float[numPeaks];
        uncompress((Bytef*)intensity, &uncomprLen, comprI, intensityLen);
    }
    
    vector<PEAK_T> peaks(numPeaks);
    PEAK_T p;
    for(int i=0;i<numPeaks;i++) {
        p.mz = mz[i];
        p.intensity = intensity[i];
        // Check for corruption
        if (p.mz != p.mz || p.mz < 0 || p.mz > 100000 ||
            p.intensity != p.intensity || p.intensity < 0) {
            // Corrupted peaks
            peaks.clear();
            return peaks;
        }
        peaks[i] = p;
    }

    if (mz != (double*)comprM)
        delete [] mz;
    if (intensity != (float*)comprI)
        delete [] intensity;

    return peaks;
}

/**
 * Given a vector containing RefSpectrum for the same sequence and
 * charge, find the best representative and insert into the current table.
 * The "best representative" is currently defined as the spectrum that
 * has the highest average dot product when compared to all other
 * spectra.  
 *
 * When the collection contains exactly one spectrum, add it.  When the
 * spectrum contains exactly two spectra, the average dot product will
 * be the same for both so use a different criterion to choose.
 * Eventually, when a quailty-of-match score (e.g. p-value) is stored,
 * use the spec with the higher score.  For now, use the one with more
 * peaks. 
 */
void BlibFilter::compAndInsert(vector<RefSpectrum*>& oneIon)
{
    int num_spec = oneIon.size();
    RefSpectrum* bestSpec = NULL;
    int specID = 0; // id RefSpec table in filtered index

    if(num_spec == 1){ // add that one spectrum
        specID = transferSpectrum(redundantDbName_, 
                                  oneIon.at(0)->getLibSpecID(), 
                                  num_spec,
                                  redundantLibHasAdditionalColumns_);
        bestSpec = oneIon.front();
    } else if(num_spec == 2) { // choose the one with more peaks
        // in the future, pick the one with the best search score
        int best_idx = 0;
        if( oneIon.at(0)->getNumRawPeaks() < oneIon.at(1)->getNumRawPeaks() ) {
            best_idx = 1;
        }

        specID = transferSpectrum(redundantDbName_, 
                                  oneIon.at(0)->getLibSpecID(), 
                                  num_spec,
                                  redundantLibHasAdditionalColumns_);
        bestSpec = oneIon.at(0);
    } else { // compute all-by-all dot-products
        
        // preprocess all RefSpectrum in oneIon
        PeakProcessor proc;
        proc.setClearPrecursor(true);
        proc.setNumTopPeaksToUse(100);
        // TODO (BF Aug-12-09): all processing should be controlled by
        // parameters available to the user
        
        RefSpectrum* tmpRef;
        for(int i=0; i<(int)oneIon.size(); i++) {
            tmpRef=oneIon.at(i);
            proc.processPeaks(tmpRef);
        }
        
        // create an array where we'll sum scores for each spectrum
        // initialize to 0
        vector<double> scores(oneIon.size(), 0);

        // for each spectrum
        for(int i=0; i<(int)oneIon.size(); i++) {
            RefSpectrum* tmpRef1 = oneIon.at(i);

            // compare to all subsequent spectrum
            for(int j=i+1; j<(int)oneIon.size(); j++) {

                RefSpectrum* tmpRef2 = oneIon.at(j);
                Match thisMatch(tmpRef1, tmpRef2);
                DotProduct::compare(thisMatch);
                double dotProduct = thisMatch.getScore(DOTP);

                // add the score to the running total for both spec
                scores[i] += dotProduct;
                scores[j] += dotProduct;
            }
        } // next spectrum

        // find the best score and keep the spectrum associated with it
        size_t maxScoreIndex = getMaxElementIndex(scores);
        double bestScore = scores[maxScoreIndex];
        double bestAverageScore = bestScore / (double)oneIon.size() ;

        // If best average score is too low, don't include it 
        if ( bestAverageScore >= minAverageScore_ ){
            specID = transferSpectrum(redundantDbName_, 
                                      oneIon.at(maxScoreIndex)->getLibSpecID(), 
                                      oneIon.size(),
                                      redundantLibHasAdditionalColumns_);
            bestSpec = oneIon.at(maxScoreIndex);
        } else {
            Verbosity::warn("Best score is %f for %s, charge %d after "
                            "comparing %i spectra.  This sequence will not be "
                            "included in the filtered library.", 
                            bestAverageScore, (oneIon.at(0)->getSeq()).c_str(),
                            oneIon.at(0)->getCharge(), oneIon.size());
            return;
        }
    }

    // add the sequence, charge, representative spec into PeptideIons
    sprintf(zSql,
            "INSERT INTO PeptideIons (spectrumID, bareSequence, "
            "modifiedSequence, charge) "
            "VALUES (%d, '%s', '%s', %d)",
            specID, bestSpec->getSeq().c_str(), bestSpec->getMods().c_str(),
            bestSpec->getCharge());
    sql_stmt(zSql);

    // add rt, peptideIonId for all refspec
    int peptideIonID = (int)sqlite3_last_insert_rowid(getDb());
    for(int i = 0; i < num_spec; i++){
        // if( oneIon.at(i)->getRetentionTime() == 0){ continue; }
        sprintf(zSql,
                "INSERT INTO RetentionTimes (peptideID, retentionTime) "
                "VALUES (%d, %f)", peptideIonID, 
                oneIon.at(i)->getRetentionTime());
        sql_stmt(zSql);
    }
}
/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
