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

#include "BlibUtils.h"


using namespace std;
namespace bfs = boost::filesystem;

namespace BiblioSpec {

/**
 * \brief Return a string from the root to the given filename.
 * For filenames with no path, prepends current working directory.
 * Does not resolve symbolic links. Does not check that the file
 * exists.
 */
string getAbsoluteFilePath(string filename){
    bfs::path fullPath = bfs::system_complete(filename.c_str());
    fullPath = fullPath.normalize();

    return fullPath.native_file_string();
}

/*
This version resolves symbolic links, but is POSIX specific, doesn't
work on Windows.  I couldn't find the Windows equivilent.
#include <stdlib.h> // for realpath
string getAbsoluteFilePath(const char* filename){

    char buff[4096];
    realpath(filename, buff);
    // fullpath() Windows??
    return string(buff);
}
*/
/**
 * \brief Return all of the string before the last / or \\. Returns an
 * empty string if neither found.
 */
string getPath(string fullFileName)
{
    string filepath;
    // find the beginning of the file name
    size_t lastSlash = fullFileName.find_last_of("/\\");
    if( lastSlash != string::npos ) {
        filepath = fullFileName.substr(0, lastSlash+1);
    }

    return filepath;
}

/**
 * \brief Return all of string after the last '.'.  Returns an empty
 * string if none found.
 */
string getExtension(string fullFileName);


/**
 * \brief Return the string between the last \ or // and the last
 * '.'.  Returns the whole string if neither found.
 */
string getFileRoot(string fullFileName)
{
    string fileroot;

    // find the beginning of the filename
    size_t lastSlash = fullFileName.find_last_of("/\\");
    if( lastSlash == string::npos ) {// if not found, set to beginning
        lastSlash = 0;
    } else {
        lastSlash += 1;
    }

    // find the beginning of the extension
    size_t lastDot = fullFileName.find_last_of(".");
    fileroot = fullFileName.substr(lastSlash, lastDot - lastSlash);

    //  cerr << "file root of "<< fullFileName << " is " << fileroot << endl;
    return fileroot;
}

/**
 * \brief Returns true if the end of the filename matches exactly the
 * ext string.  Assumes the ext string includes a dot.
 */
bool hasExtension(string filename, string ext);
bool hasExtension(string filename, const char* ext){
    const char* name_ptr = filename.c_str();
    return strcmp(name_ptr + filename.size() - strlen(ext), ext) == 0;
}

bool hasExtension(const char* name, const char* ext) {
    return strcmp(name + strlen(name) - strlen(ext), ext) == 0;
}

bool compare_first_pair_doubles_descending(const pair<double, double>& left,
                                           const pair<double, double>& right){
    return (left.first < right.first);
}

/**
 * Compare two doubles for sorting in descending order
 */
bool doublesDescending(const double left, const double right){
    return left > right;
}

/**
 * Replace all occurances of findChar and replace them with
 * replaceChar, returning the number of substitutions performed.
 */
size_t replaceAllChar(string& str, const char findChar, const char replaceChar){
    size_t count = 0;
    size_t position = str.find_first_of(findChar);
    while(position != string::npos){
        str.at(position) = replaceChar;
        count++;
        position = str.find_first_of(findChar);
    }
    return count;
}

/**
 *  Replace all characters after the last '.' with ext.  If no '.'
 *  found concatinate .ext onto the filename. 
 */
void replaceExtension(string& filename, const char* ext){
    size_t lastDot = filename.find_last_of(".");
    if( lastDot == string::npos) {
        filename += ".";
    } else {
        filename.erase(lastDot + 1);
    }

    filename += ext;

    return;
}

/**
 * Sum the masses of amino acids and modifications from the given
 * array of masses (as initialized by AminoAcidMasses).
 */
double getPeptideMass(string& modifiedSeq, double* masses){
    double mass = 0;
    for(int i = 0; i < (int)modifiedSeq.size(); i++){

        char aa = modifiedSeq[i];

        if( aa == '[' ){
            size_t end = modifiedSeq.find_first_of("]", i);
            string modStr = modifiedSeq.substr(i + 1, end - i);
            double modMass = atof(modStr.c_str());
            i = end + 1;

            mass += modMass;
        } else if( aa >= 'A' && aa <= 'Z' ){
            mass += masses[(int)aa];
        } else {
            Verbosity::error("Illegal character %c for computing mass of %s.",
                             aa, modifiedSeq.c_str());
        }
    }
    return mass;
}


/**
 * Delete any spaces or tabs at the end of the given string
 */
void deleteTrailingWhitespace(string& str){
    size_t last = str.size() - 1;
    while( str.at(last) == ' ' || str.at(last) == '\t' ){
        last--;
    }
    // last is now index position of last non-whitespace char
    str.erase(last + 1);
}

/**
 * Create a copy of the given string, converting all characters to
 * upper case.
 */
char* strcpy_all_caps(const char* original){
    if( original == NULL ){
        return NULL;
    }
    char* new_str = new char[strlen(original)];
    strcpy(new_str, original);

    for(int i = 0; new_str[i]; i++ ){
        new_str[i] = toupper(new_str[i]);
    }

    return new_str;
}

} // namespace

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
