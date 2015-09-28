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
 * The MascotResultsReader collects a list of psms that should be
 * included in the library.  It passes the file object it was using to
 * the MascotSpecReader so the file only has to be opened and parsed once.
 */

#include "MascotResultsReader.h"

namespace BiblioSpec {

MascotResultsReader::MascotResultsReader(BlibBuilder& maker, 
                    const char* datFileName, 
                    const ProgressIndicator* parent_progress)
: BuildParser(maker, datFileName, parent_progress)
{
    Verbosity::comment(V_DETAIL, "Creating a MascotResultsReader.");

    // Create the ms_file
    ms_file_ = new ms_mascotresfile(datFileName);
    if( !ms_file_->isValid() ) {
        throw BlibException(true, "'%s' is not a valid .dat file.", 
                            datFileName);
    }

    // set score threshold
    scoreThreshold_ = getScoreThreshold(MASCOT);

    // create the results objects
    ms_results_ = 
        new ms_peptidesummary(*ms_file_,
                              ms_mascotresults::MSRES_DUPE_REMOVE_NONE,
                              0, // get all results and filter later
                              0, // get all ranks
                              0, // ignoreIonsScoreBelow
                              0);// min len

    // create the spec reader, sharing the file and results objects
    delete specReader_;  
    specReader_ = new MascotSpecReader(datFileName, ms_file_, ms_results_);
    // register the name with BuildParser, but don't try to open it
    this->setSpecFileName(datFileName, false);

    // get modifications information
    ms_params_ = new ms_searchparams(*ms_file_);

    for(int i=1; 0 != ms_params_->getFixedModsDelta(i); i++){
        double deltaMass = ms_params_->getFixedModsDelta(i);
        string residues = ms_params_->getFixedModsResidues(i);

        // add each residue with a static mod to the collection
        for(size_t c=0; c < residues.length(); c++){
            staticMods_.insert(pair<char, double>(residues.at(c), deltaMass));
        }
        Verbosity::comment(V_DEBUG, 
                           "Static mod of delta mass %f for residues %s.",
                           deltaMass, residues.c_str());

    }

    // check for isotope labeling
    getIsotopeMasses();

}


MascotResultsReader::~MascotResultsReader()
{
    delete ms_params_;
    // the spec reader will delete the results and file
    //delete ms_results_; 
    //delete ms_file_;

    // Delete all the mod tables
    map<string, ModTable* >::iterator it = methodModsMaps_.begin();
    while (it != methodModsMaps_.end())
    {
        delete it->second;
        it++;
    }
}

bool MascotResultsReader::parseFile(){

    // look at every spectrum
    for(int specId = 1; specId <=ms_file_->getNumQueries(); specId++) {
        Verbosity::comment(V_DETAIL, "Parsing spec %d.", specId);

        // note: the peptides should not be deleted, as this will corrupt the result
        ms_peptide* pep;
        int rank = 1;

        ms_results_->getPeptide(specId, rank, pep);
        if(!pep->getAnyMatch()) {// skip this spec if no matches
            continue;
        }

        // find out if it passes the score threshold
        double ionScore = pep->getIonsScore();
        double expectationValue = 
            ms_results_->getPeptideExpectationValue(ionScore, specId);

        if( expectationValue > scoreThreshold_ ) {
            continue;
        }

        // see if the higer ranking peptides have the same score as
        // the first-ranked 
        // return the highest ranking peptide to include: 0 for none
        int maxRank = findMaxRankingPeptideToAdd(specId);

        while( rank <= maxRank ){

            PSM* cur_psm = new PSM();
            cur_psm->charge = pep->getCharge();
            cur_psm->unmodSeq = pep->getPeptideStr();
            cur_psm->specKey = specId;
            cur_psm->score = expectationValue;

           // get any labeling
            string quant = pep->getComponentStr();
            applyIsotopeDiffs(cur_psm, quant);

            // get any modifications
            string readVarMods =  ms_results_->getReadableVarMods(specId, rank);
            parseMods(cur_psm, pep->getVarModsStr(), readVarMods);
            psms_.push_back(cur_psm);

            Verbosity::comment(V_ALL, "Adding spec %d, charge %d, score %f, "
                               "seq %s.", cur_psm->specKey, cur_psm->charge, 
                               cur_psm->score, cur_psm->unmodSeq.c_str());

            ms_results_->getPeptide(specId, ++rank, pep);
        
        }// next psm with same score and I/L equivalent peptide
    }// next spectrum

    // put all the collected spectra in the library
    buildTables(MASCOT_IONS_SCORE);

    return true;
}

/**
 * Examine the higher ranking peptides for the current spectrum to see
 * which, if any, PSMs should be included in the library.  If the
 * top-ranking PSM has a unique score (or if it is the only candidate
 * for this spectrum, include it only.  If the top two or more PSMs
 * have the same score, but different sequences (that are not I/L
 * equivilent), do not include any PSMs.  If the top n psms have I/L
 * equivalent sequences, include n psms.  Return the number of psms to include.
 */
int MascotResultsReader::findMaxRankingPeptideToAdd(int specId){
    int rank = 1;
    ms_peptide* prevPep; 
    ms_results_->getPeptide(specId, rank, prevPep);
    string prevSeq = prevPep->getPeptideStr();

    while( rank < ms_results_->getMaxRankValue() ){ // should break before this
        ms_peptide* curPep;  
        ms_results_->getPeptide(specId, rank+1, curPep);
        string curSeq = curPep->getPeptideStr();

        // if the sequences of the two are the same, then there was not a
        // higher ranking peptide, just add the first
        if( prevSeq == curSeq ){
            return rank;
        }
        
        // if the "pretty rank" of the current peptide is not 1, include
        // just the top-ranked peptide
        if( curPep->getPrettyRank() > 1 ){
            return rank;
        } 

        // else pretty rank is 1, compare the seqs
        if( ! seqsILEquivalent(prevSeq, curSeq) ){
            Verbosity::debug("Not including spectrum %d, charge %d. "
                             "Two sequences with the same score: %s, %s.",
                             specId, prevPep->getCharge(), 
                             prevSeq.c_str(), curSeq.c_str());
            return 0;
        } else { // they are the same, see if there are more
            rank++;
            prevSeq = curSeq;
        }
    
    }// next peptide

    // should return before this
    return rank;
}

/**
 * Translate the modstr into a set of SeqMod's and add them to the
 * psm-mods vector.  Also add any static mods which can be looked up
 * by residue from psm->unmodSeq.
 *
 * The modstr is a string of ints, one for each residue in the
 * peptide plus one n and c terminus.  A zero (0) indicates no
 * modification.  A non-zero can be used to look up the mod delta mass
 * from the ms_params_.  The readableModStr is a semi-colon-separated
 * list of text descriptions of the modifications to the sequence.
 * For mods generated from an error tolerant search, the text is the
 * only record of the mod mass shift.
 * Position is 1-based. 
 * 
 */
void MascotResultsReader::parseMods(PSM* psm, string modstr, 
                                    string readableModStr){
    int first_mod_pos = 1;
    int last_mod_pos = modstr.length() - 2;

    // first parse the terminal character
    if( modstr.at(0) == 'X' ){
        addErrorTolerantMod(psm, readableModStr, first_mod_pos);
    } else {
        addVarMod(psm, modstr.at(0), first_mod_pos);
    }

    // for characters first to last in modstr, 
    // use the int value of the char to look up delta mass
    for(int i = first_mod_pos; i <= last_mod_pos; i++){
        if( modstr.at(i) == 'X' ){
            addErrorTolerantMod(psm, readableModStr, i);
        } else {
            addVarMod(psm, modstr.at(i), i);
        }
    }

    // now get terminal character at the other end
    if( modstr.at(last_mod_pos+1) == 'X' ){
        addErrorTolerantMod(psm, readableModStr, last_mod_pos);
    } else {
        addVarMod(psm, modstr.at(last_mod_pos+1), last_mod_pos);
    }

    // for static mods look up each residue in the staticMods collection
    for(size_t i=0; i < psm->unmodSeq.length(); i++){
        ModTable::iterator found = staticMods_.find(psm->unmodSeq.at(i));
        if( found != staticMods_.end()) {
            double deltaMass = found->second;
            SeqMod mod;
            mod.position = i + 1;
            mod.deltaMass = deltaMass;
            psm->mods.push_back(mod);
        }
    }
}

/**
 * Add a modification to the psm.  The varLookUpChar is a character
 * 0-9 or A-W that can be used to look up the mass of a particular
 * mod.  aaPosition is the location of this mod in the peptide.
 */
void MascotResultsReader::addVarMod(PSM* psm, 
                                    char varLookUpChar, 
                                    int aaPosition){
    
  int idx = getVarModIndex(varLookUpChar);
  if( idx != 0 ) {
    SeqMod mod;
    mod.deltaMass = ms_params_->getVarModsDelta(idx);
    mod.position = aaPosition;
    psm->mods.push_back(mod);
  }
}

/**
 * Add a modification to the psm.  Use the readableModStr
 * representation of the modifications to find the mass difference.
 */
void MascotResultsReader::addErrorTolerantMod(PSM* psm, 
                                              string readableModStr, 
                                              int aaPosition){

    // mods in readableModStr are separated by ;
    // if there is an error tolerant mod, it will be last and can be
    // identified by the presence of square braces enclosing a mass
    size_t lastSemiColon = readableModStr.find_last_of(';');
    if( lastSemiColon == string::npos ){
        lastSemiColon = 0;
    }

    // first check to see if this mod is a base change in which case
    // there is no post-translational modification
    if( readableModStr.find("NA_INSERTION", lastSemiColon) != string::npos ||
        readableModStr.find("NA_DELETION", lastSemiColon) != string::npos || 
        readableModStr.find("NA_SUBSTITUTION", lastSemiColon) != string::npos ){
        Verbosity::comment(V_DETAIL, "No change for an indel/substitution mod "
                           "in '%s'", readableModStr.c_str());
        return;
    }

    // find the open brace
    size_t brace = readableModStr.find('[', lastSemiColon);
    if( brace == string::npos ){
        throw BlibException(true, 
                            "Error tolerant modification is missing a mass "
                            "shift. Not found in '%s' from file '%s'.", 
                            readableModStr.c_str(), getFileName().c_str());
    }
    // parse the mass shift
    double mass = atof(readableModStr.c_str() + brace + 1);

    Verbosity::comment(V_DETAIL, "Adding ET mod with mass shift %f from "
                       "'%s'", mass, readableModStr.c_str());
    // create new mod and add to the psm
    SeqMod mod;
    mod.deltaMass = mass;
    mod.position = aaPosition;
    psm->mods.push_back(mod);
}


/**
 * The characters of a var mod string may include 0-9 and A-W where A
 * = 10 and W = 32.  Convert the character into its integer value
 */
int MascotResultsReader::getVarModIndex(const char c){
    char cBuf[2];
    cBuf[0] = c;
    cBuf[1] = '\0';
    int intVal = atoi(cBuf);

    if( intVal == 0 && cBuf[0] != '0' ) { 
        intVal = static_cast<int>(c) - static_cast<int>('A') + 10;
    }

    if( intVal > 32 || intVal < 0 ) {
        throw BlibException(true, 
                            "'%c' is not a legal modification character in %s.",
                            c, getFileName().c_str());
    }
    return intVal;
}

/**
 * \brief Find out if there was any isotope labeling for this run and
 * if so generate a table of mass differences for each residue.  These
 * differences will be reported as diff mods.  Create one table for
 * each method component name (e.g. heavy and light for 15N) and store
 * with the name as the key in methodModsMaps_.
 */
void MascotResultsReader::getIsotopeMasses(){

    // find out if there is labeling in this run
    string quantName = ms_params_->getQUANTITATION();
    Verbosity::debug("Quantitation method is %s.", 
                     (quantName.empty()) ? "not specified" : quantName.c_str());

    if( quantName.empty() || quantName == "None") {
        return;
    }

    // get the quantification parameters
    ms_quant_configfile quantConfig;
    if( !ms_file_->getQuantitation(&quantConfig) ) {
        throw BlibException(true, "Cannot get quantitation information "
                            "from file '%s'.", getFileName().c_str());
    }

    // e.g. method is 15N, components are light/heavy
    const ms_quant_method* method = 
        quantConfig.getMethodByName(quantName.c_str());
    if( method == NULL ) {
        throw BlibException(true, "Cannot get quantitation method %s "
                            "in file '%s'.", quantName.c_str(), 
                            getFileName().c_str());
    }

    // get the parameters that include isotope masses
    ms_umod_configfile massConfig;
    ms_file_->getUnimod(&massConfig);
    
    // get a set of default masses to compare isotopes to
    ms_masses defaultMasses;

    // for each component in the method , create a table of mass diffs
    for(int comp_idx=0; comp_idx < method->getNumberOfComponents(); comp_idx++){
        string name = method->getComponentByNumber(comp_idx)->getName();
        ModTable* mods = new ModTable();
        methodModsMaps_[name] = mods;

        Verbosity::debug("Creating mods table for component %s.", name.c_str());

        ms_masses heavyMasses;
        heavyMasses.applyIsotopes(&massConfig, 
                                  method->getComponentByNumber(comp_idx));
        // for each residue
        for(char aa = 'A'; aa < 'Z'; aa++){
            double heavy = heavyMasses.getResidueMass(MASS_TYPE_MONO, aa);
            double light = defaultMasses.getResidueMass(MASS_TYPE_MONO, aa);
            if( (heavy - light) > 0.000005 ) { // don't add 0 mass diffs
                mods->insert(pair<char,double>(aa, (heavy - light)));
            }
        }

    }

}

/**
 * \brief Add a diff mod to each residue if there is isotope labeling
 * for this peptide.  Assumes that methodModMaps_ has already been
 * initialized.
 */
void MascotResultsReader::applyIsotopeDiffs(PSM* psm, string quantName){

    if( quantName.empty()) {
        return;
    }

    // find table of mods for this name
    ModTable* mods = methodModsMaps_.find(quantName)->second;
    if( mods == NULL ) {// do I need to test iterator != map.end() instead
        throw BlibException(true, "Labeling method %s was not found in file "
                            "'%s'.", quantName.c_str(), getFileName().c_str());
    }

    // find mod for each residue
    for(size_t i=0; i < psm->unmodSeq.length(); i++){
        ModTable::iterator found = 
            mods->find(psm->unmodSeq.at(i));

        if( found != mods->end() ) {
            double deltaMass = found->second;
            SeqMod mod;
            mod.position = i + 1;
            mod.deltaMass = deltaMass;
            psm->mods.push_back(mod);
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
