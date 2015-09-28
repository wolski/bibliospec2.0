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
//class definition of Library

#include "original-Library.h"

using namespace std;

library_header::library_header() {
    numSpec = 0;
    filtered = false;
    specVersion = 0;
    annotVersion = 0;
    nextId = 1;
}

const float Library::MAX_MZ = 1000000.0f;

Library::Library() {
    verbosity = DEFAULT_VERBOSITY;
}

Library::Library(const char* filename, int v) {
    verbosity = v;
    readFromFile(filename);
}

Library::~Library() {
    for(int i=0; i<(int)refSpecv.size(); i++) {
        if(refSpecv.at(i))
            delete refSpecv.at(i);
    }
}

void Library::init() {
    header.numSpec = 0;
    verbosity = DEFAULT_VERBOSITY;
    header.filtered = false;
    header.specVersion = 0;
    header.annotVersion = 0;
}

void Library::init(int v) {
    init();
    verbosity = v;
}

void Library::readFromFile(const char* filename)
{
    ifstream infile (filename, ios::binary);
    if( ! infile.is_open() ) {
        cout << "Could not open library file " << filename << endl;
        throw exception();
    }
    //should these files have some sort of tag as the first line to confirm that they are of the expected type?
    (verbosity > 1) ? cout << "Reading in library from file" << endl, true : true; 
    infile.read( (char*)&header, sizeof(LIBHEAD_T) );

    for(int i=0; i<header.numSpec; i++ ) {
        RefSpectrum* tmpRefSpec = new RefSpectrum();
        tmpRefSpec->readFromFile(infile);
        refSpecv.push_back( tmpRefSpec );
    }
    procPeaksv.assign( refSpecv.size(), NULL);  //empty vectory of processedPeak ptrs

}

void Library::writeToFile(const char* filename) {
    //check if sorted by m/z and if not, sort

    ofstream outfile (filename, ios::binary);
    if( ! outfile.is_open() ) {
        cout << "Could not open library file " << filename << endl;
        throw exception();
    }

    (verbosity > 1) ? cout << "Saving library" << endl, true : true;
    outfile.write( (char*)&header, sizeof(LIBHEAD_T) );

    assert( header.numSpec == refSpecv.size());
    for( int i=0; i<(int)refSpecv.size(); i++ ) {
        (refSpecv.at(i))->writeToFile(outfile);
    }
}

//returns the index number of the first spectrum with mz >= to given
int Library::findLowMz(float mz)
{
    RefSpectrum* lowSpec = new RefSpectrum(mz);
    vector<RefSpectrum*>::iterator low = 
        lower_bound( refSpecv.begin(), refSpecv.end(), lowSpec, compSpecPtrMz());
    int first = low - refSpecv.begin();
  
    return first;
}

//returns the index number of the first spectrum with mz >= to given
int Library::findHiMz(float mz)
{
    RefSpectrum* hiSpec = new RefSpectrum(mz);
    vector<RefSpectrum*>::iterator hi = 
        upper_bound(refSpecv.begin(), refSpecv.end(), hiSpec, compSpecPtrMz());
    int last = hi - refSpecv.begin();
    return last;
}

LibIterator Library::getSpecInRange(float lowMz, float hiMz)
{
    int first = findLowMz(lowMz);
    int last = findHiMz(hiMz);
    return LibIterator( refSpecv, procPeaksv, first, last );
}

LibIterator Library::getAllSpec()
{
    int first = 0;
    int last = refSpecv.size();

    return LibIterator( refSpecv, procPeaksv, first, last );
}


//returns an iterator that points to all spectra of a given ion type
//if no more ions or not sorted by ion, return empty iterator
LibIterator Library::getNextIon(int max ) {
    if( !sortedByIon ) 
        return LibIterator( refSpecv, procPeaksv, 0, 0 );  //exception?
    if( nextIonIndex == refSpecv.size() ) 
        return LibIterator( refSpecv, procPeaksv, 0, 0);

    int first = nextIonIndex;
    int last = nextIonIndex;
    int charge = refSpecv.at(nextIonIndex)->getCharge();
    string seq = refSpecv.at(nextIonIndex)->getSeq();
    string mods = refSpecv.at(nextIonIndex)->getMods();
 
    //scan the list of spectra until one has a different charge, seq, and mods
    while( last < (int)refSpecv.size() && 
           charge == refSpecv.at(last)->getCharge() &&
           seq == refSpecv.at(last)->getSeq() && 
           mods == refSpecv.at(last)->getMods() ) {

        last++;
    }
  
    nextIonIndex = last;

    //add check for max
    if( last - first > max ) {
        //randomize the order of those spec
        shuffle(first, last-1);
        last = first + max;
    }
    return LibIterator( refSpecv, procPeaksv, first, last);
  
}

//randomly re-order the spec between first and last
//used for filtering
void Library::shuffle(int firstIndex, int lastIndex) {
    srand(static_cast<unsigned>(time(0)));

    //for each element between firstIndex (inclusive) and last (not inclusive)
    //swap the ith with a randomly chosen index greater than i
    int swapLast = lastIndex - 1;

    for(int i=firstIndex; i < lastIndex -1; i++) {
        int swapFirst = i + 1;
    
        int newRand = getRandInt( i+1, lastIndex);
        int swapIndex = swapFirst + newRand;

        RefSpectrum* tmpSpec = refSpecv.at(i);
        refSpecv.at(i) = refSpecv.at(swapIndex);
        refSpecv.at(swapIndex) = tmpSpec;
    }

}

int Library::getRandInt(int lowest, int highest) {
    int range = highest - lowest +1;
    int newRand = rand();
    double offset = range * (newRand / (RAND_MAX + 1.0));
    return (int)offset;
}

void Library::sortByIon()
{
    sort(refSpecv.begin(), refSpecv.end(), compRefSpecPtrIon());
    sortedByIon = true;
    nextIonIndex = 0;
}

void Library::sortByID()
{
    (verbosity > 1) ? cout << "sorting by id" << endl, true : true;
    sort(refSpecv.begin(), refSpecv.end(), compRefSpecPtrId()); 
}

int Library::getNumSpec() {
    return header.numSpec;
}

string Library::getVersion_str() {
    ostringstream version_ss;
    version_ss << header.specVersion << "." << header.annotVersion;
    return version_ss.str();
}

bool Library::filtered() {
    return header.filtered;
}

LIBHEAD_T Library::getHeader() {
    return header;
}

vector<RefSpectrum*>::iterator Library::getFirstSpec() {
    return refSpecv.begin();
}

vector<RefSpectrum*>::iterator Library::getLastSpec() {
    return refSpecv.end();
}

/*
  Given a list of ids and annotations, change the annotations
  and change the version number
*/
void Library::annotate(const char* file) {
    const char* header_str = "id\tannotation";
    (verbosity > 1) ? cout << "Annotating spectra in library" << endl, true : true;

    ifstream annot_file ( file );
    if( !annot_file.is_open() ) {
        cerr << "Could not open annotation file " << file << endl;
        throw exception();
    }

    string line;
    getline( annot_file, line );
    if( line != header_str ) {
        cerr << "Header should read: " << header_str << endl;
        exit(1);
    }

    //get all id/annotation pairs
    vector<annot_pair> annots_v;
    while( annot_file.peek() != EOF ) {
        annot_pair pair;
        annot_file >> pair.id >> pair.annot;
        getline( annot_file, line); // so peek works
        annots_v.push_back(pair);
    }

    annot_file.close();

    //sort the pairs
    sort( annots_v.begin(), annots_v.end(), comp_pair_id );

    makeAnnotations( annots_v );

    header.annotVersion++;
}

void Library::makeAnnotations(vector<annot_pair>& annots_v) {
    //sort spec by id
    (verbosity>1) ? cout << "Sorting library spectra" << endl, true : true;
    sort(refSpecv.begin(), refSpecv.end(), compRefSpecPtrId());

    //look at all spec in lib
    int i_update = 0;
    int i_lib = 0;
    annot_pair update; 
    int curSpecId = 0; 

    while( i_lib<(int)refSpecv.size() && i_update<(int)annots_v.size() ) {

        update = annots_v.at(i_update);
        curSpecId = refSpecv.at(i_lib)->getID();

        if( update.id == curSpecId ) {
            refSpecv.at(i_lib)->setAnnot( update.annot );
            i_lib++; 
            i_update++; 

        } else if(update.id > curSpecId) {
            i_lib++; 
        } else { //update.id < curSpecId
            (verbosity > 1) ? cerr << "WARNING: Spectrum " << update.id << " was not found in the library" << endl, true : true;
            i_update++; 
        }
    }

    //warn about missed annotations
    while( i_update < (int)annots_v.size() ) {
        update = annots_v.at(i_update);
        (verbosity > 0) ?cerr << "WARNING: Spectrum " << update.id << " was not found in the library" << endl, true : true;
        i_update++; 
    }

    //sort spec by id
    (verbosity>1) ? cout << "Sorting library spectra" << endl, true : true;
    sort(refSpecv.begin(), refSpecv.end(), compSpecPtrMz());

}

void Library::deleteSpec(const char* filename) {
    const char* header_str = "delete";
    (verbosity > 1) ? cout << "Deleting spectra" << endl, true : true;

    //open list of ids
    ifstream delFile( filename );
    if( ! delFile.is_open() ) {
        cout << "Could not open " << filename << endl;
        throw exception();
    }

    string line;
    getline( delFile, line );
    if( line != header_str ) {
        cerr << "Header should read " << header_str << endl;
        exit(1);
    }

    //get all ids
    vector<int> ids_v;
    while( delFile.peek() != EOF ) {
        int id;
        delFile >> id;
        ids_v.push_back( id );
        getline( delFile, line);
    }
    delFile.close();

    //sort list
    sort( ids_v.begin(), ids_v.end() );
    //sort spec
    sort( refSpecv.begin(), refSpecv.end(), compRefSpecPtrId());

    //go through both lists, for found spec, change mz to a large constant
    markDeletedSpec( ids_v );

    //sort by m/z
    sort( refSpecv.begin(), refSpecv.end(), compSpecPtrMz());

    //remove spec with mz of large constant
    while( refSpecv.back()->getMz() == MAX_MZ ) {
        refSpecv.pop_back();
        header.numSpec--;
    }

    //change version number
    header.specVersion++;
}

void Library::markDeletedSpec(vector<int>& ids_v) {
    int i_delete = 0;
    int i_lib = 0;
    int delId = 0; 
    int curSpecId = 0; 

    //look through all but last
    while( i_delete<(int)ids_v.size() && i_lib<(int)refSpecv.size() ) {
        delId = ids_v.at(i_delete);
        curSpecId = refSpecv.at(i_lib)->getID();

        if( delId == curSpecId ) {
            refSpecv.at(i_lib)->setMz( MAX_MZ );
            i_lib++;
            i_delete++;
        } else if( delId > curSpecId ) {
            i_lib++;
        } else { //delID < curSpecId
            (verbosity > 0) ? cerr << "WARNING: Spectrum " << delId << " was not found in the library" << endl, true : true;
            i_delete++;
        }
    }

    //warn about any missed delids
    while( i_delete < (int)ids_v.size()) {
        delId = ids_v.at(i_delete);
        cerr << "WARNING: Spectrum " << delId << " was not found in the library" << endl;
        i_delete++;
    }

}

void Library::addSpec(vector<RefSpectrum*>::iterator first,
                      vector<RefSpectrum*>::iterator last) {

    //insert spec into vector
    assert( header.numSpec == refSpecv.size() );
    refSpecv.insert( refSpecv.end(), first, last);

    //renumber new spec (saving new and old id numbers)
    for(int i=header.numSpec; i<(int)refSpecv.size(); i++) {
        refSpecv.at(i)->setID( header.nextId++ );
    }
    //sort by m/z
    sort( refSpecv.begin(), refSpecv.end(), compSpecPtrMz() );
    //update numSpec
    header.numSpec = refSpecv.size();
    //update version
    header.specVersion++;
}


bool comp_pair_id( annot_pair p1, annot_pair p2 ) {
    return p1.id < p2.id;
};

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
