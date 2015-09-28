/*
##############################################################################
# file: ms_mascotresfile.hpp                                                 #
# 'msparser' toolkit                                                         #
# Encapsulates a  mascot results file                                        #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2002 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /MowseBranches/ms_mascotresfile_1.2/include/ms_mascotresfi $ #
#     $Author: davidc $ #
#       $Date: 2008-03-06 13:56:39 $ #
#   $Revision: 1.21 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESFILE_HPP
#define MS_MASCOTRESFILE_HPP


#ifdef _WIN32
#pragma warning(disable:4251)   // Don't want all classes to be exported
#pragma warning(disable:4786)   // Debug symbols too long
#   ifndef _MATRIX_USE_STATIC_LIB
#       ifdef MS_MASCOTRESFILE_EXPORTS
#           define MS_MASCOTRESFILE_API __declspec(dllexport)
#       else
#           define MS_MASCOTRESFILE_API __declspec(dllimport)
#       endif
#   else
#       define MS_MASCOTRESFILE_API
#   endif
#else
#   define MS_MASCOTRESFILE_API
#endif

// for the sake of #include <string>
#ifdef __ALPHA_UNIX__
#include <varargs.h>
#else
#include <stdarg.h>
#endif


// Includes from the standard template library
#include <time.h>
#include <string>
#include <list>
#include <vector>
#include <set>

namespace matrix_science {
    class ms_searchparams;

    /** @addtogroup resfile_group
     *  
     *  @{
     */

#ifndef SWIG
    class multiBuf {
    public:
        multiBuf()  {pMem_ = 0; len_ = 0; pEnd_ = 0;}
    public :
        char *pMem_;  // pointer to first block of memory
        int  len_;    // excluding any null terminator
        char *pEnd_;  // Pointer to last byte (not the null terminator)
    private:
    };
    typedef std::vector<multiBuf> multiBuf_v;  

    class multiBufMemPtr {
    public:
        multiBufMemPtr(int bufNum, char *pMem)
            : bufNum_(bufNum), pMem_(pMem) {};
        multiBufMemPtr() : bufNum_(-1), pMem_(0) {};
        void decrement(const multiBuf_v & buffers);
        void decrementUntil(const multiBuf_v & buffers, const char * chars);
        void increment(const multiBuf_v & buffers);
        bool isValid() const {return bufNum_ != -1;}
        bool operator<(const multiBufMemPtr & rhs);
            
    public:
        int    bufNum_;
        char * pMem_;
    };

    // ms_sortByKeyCriterion class is used internally in the library
    // to give fast access to the keys. Don't use it from outside the DLL
    class ms_sortByKeyCriterion
    {
        public:
            bool operator() (const char * p1, const char * p2) const;

        private:
            // Unfortunately, when called from Perl in Windows, 
            // the toupper function ends up calling extra functions
            // to convert to wide char and back again. We don't
            // want language independent conversion here - the
            // key names are all single byte.
            inline char my_toupper(char ch) const
            {
                if (ch >= 'a' && ch <= 'z')
                    return 'A' + (ch-'a');
                else
                    return ch;
            }

    };
#endif

    //! The first requirement before using any other functions or classes is to create a ms_mascotresfile object 
    /*!
     * You must create an object of this class before anything else. 
     * The constructor takes the file name as a parameter. Note that all
     * key names are case insensitive.
     */
    class MS_MASCOTRESFILE_API ms_mascotresfile
    {
        public:
            //! Section names in the standard mascot results files
            /*!
             * To access from perl, use, for example: <P><tt>
             * $x = $msparser::ms_mascotresfile::SEC_MASSES
             * </tt></P>
             */
            // Developer note: Add new sections to this enum just before
            // SEC_INDEX just in case a perl programmer has used constant
            // numbers instead of SEC_ values in their code.
            enum section 
            { 
                SEC_PARAMETERS,       //!< parameters section 
                SEC_HEADER,           //!< header section 
                SEC_MASSES,           //!< masses section
                SEC_SUMMARY,          //!< summary section
                SEC_MIXTURE,          //!< mixture section (pmf mixture)
                SEC_PEPTIDES,         //!< peptides section
                SEC_PROTEINS,         //!< proteins section
                SEC_QUERY1,           //!< query1 section. Don't use, see getQuerySectionValueStr() etc.
                SEC_QUANTITATION,     //!< quantitation section
                SEC_UNIMOD,           //!< unimod section
                SEC_ENZYME,           //!< enzyme section
                SEC_TAXONOMY,         //!< taxonomy section
                SEC_DECOYSUMMARY,     //!< decoy_summary section.  See also: \ref decoySearchPage
                SEC_DECOYMIXTURE,     //!< decoy_mixture section.  See also: \ref decoySearchPage
                SEC_DECOYPEPTIDES,    //!< decoy_peptides section. See also: \ref decoySearchPage
                SEC_DECOYPROTEINS,    //!< decoy_proteins section. See also: \ref decoySearchPage
                SEC_ERRTOLSUMMARY,    //!< error tolerant summary section.  See also: \ref errorTolerantPage
                SEC_ERRTOLPEPTIDES,   //!< error tolerant peptides section. See also: \ref errorTolerantPage
                SEC_INDEX,            //!< index section
                SEC_NUMSECTIONS       //!< !!don't use - place holder
            };

            //! Definitions for error numbers.
            /*!
             * To access from perl, use, for example: <P>
             *  <tt>$x = $msparser::ms_mascotresfile::ERR_NO_ERROR</tt>
             *  </P><BR>
             * Messages are classified as fatal errors [F] or warnings [W]. A warning
             * will not cause ms_mascotresfile::isValid() to return false
             */
            enum err
            {
                ERR_NO_ERROR          = 0x0000, //!< [W] Success
                ERR_NOMEM             = 0x0001, //!< [F] Failed to allocate memory to load the file
                ERR_NOSUCHFILE        = 0x0002, //!< [F] The file passed in the constructor does not exist
                ERR_READINGFILE       = 0x0003, //!< [F] Opened the file successfully, but failed to read from it
                ERR_QUERYOUTOFRANGE   = 0x0004, //!< [F] Set if query < 1 or query > getNumQueries
                ERR_MISSINGENTRY      = 0x0005, //!< [F] Set if there is no qexp value in the file
                ERR_PEPSUMMPEPGET     = 0x0006, //!< [F] Value of q, p or h out of range, so cannot get peptide info
                ERR_PEPTIDESTR        = 0x0007, //!< [F] The string in the peptides block is not valid
                ERR_ACCINPEPTIDESTR   = 0x0008, //!< [F] Could not parse an item for a given accession in the peptide section
                ERR_PROTSUMM          = 0x0009, //!< [F] Error parsing a line in the protein summary
                ERR_PROTSUMMPEP       = 0x000A, //!< [F] Couldn't parse peptide information from the protein summary section
                ERR_ADDPEPTIDES       = 0x000B, //!< [F] Failed to add peptides when creating the peptide summary
                ERR_MISSINGHIT        = 0x000C, //!< [F] Missing hit in the summary section
                ERR_MISSINGSECTION    = 0x000D, //!< [F] Complete missing section in the file
                ERR_MISSINGSECTIONEND = 0x000E, //!< [F] Missing end of section in the file
                ERR_MALFORMED_ERR_TOL = 0x000F, //!< [W] Expecting a line of format: q1_p2_et_mods=0.984020,0.000000,Citrullination
                ERR_NO_ERR_TOL_PARENT = 0x0010, //!< [F] No parent search file. See \ref errorTolerantPage
                ERR_NULL_ACC_PEP_SUM  = 0x0011, //!< [W] An empty accession string has been found. Possible problem in database
                ERR_NULL_ACC_PROT_SUM = 0x0012, //!< [W] An empty accession string has been found. Possible problem in database
                ERR_DUPE_ACCESSION    = 0x0013, //!< [W] A possible duplicate accession string has been found. Possible problem in database.
                ERR_UNASSIGNED_PROG   = 0x0014, //!< [F] Programming error! Calling getNumberOfUnassigned() or getUnassigned() before createUnassignedList()
                ERR_UNASSIGNED_RANGE  = 0x0015, //!< [F] Calling ms_mascotresults::getUnassigned() with out of range number
                ERR_UNASSIGNED_UNK    = 0x0016, //!< [F] Calling ms_mascotresults::getUnassigned() - unable to retrieve value
                ERR_NO_UNIGENE_FILE   = 0x0017, //!< [F] Failed to open the unigene file specified
                ERR_DUPLICATE_KEY     = 0x0018, //!< [W] Duplicate entries with the same key in the named section.
                ERR_OLDRESULTSFILE    = 0x0019, //!< [F] Very old results file (last century!). Parser requires 1.02 or later
                ERR_MALFORMED_TAG     = 0x001A, //!< [W] Expecting a line in format: q1_p2_tag=1:3:5:6,2:4:12:6,...
                ERR_MALFORMED_DRANGE  = 0x001B, //!< [W] Expecting a line in format: q1_p2_drange=0,256
                ERR_INVALID_NUMQUERIES= 0x001C, //!< [W] Invalid number of queries in results file has been corrected.
                ERR_MALFORMED_TERMS   = 0x001D, //!< [W] Expecting a line in format: q1_p2_terms=A,B:-,I:...
                ERR_INVALID_RESFILE   = 0x001E, //!< [F] Invalid results file format - missing or corrupt headers
                ERR_LASTONE           = 0x001F
            };
            // this list has to be included into ms_errors.hpp file too !!!

            //! Always need to create an object using this constructor
            ms_mascotresfile(const char * szFileName,
                             const int    keepAliveInterval = 0,
                             const char * keepAliveText = "<!-- %d seconds -->\n" );

            virtual ~ms_mascotresfile();

            // ------------------- Basic generic functions -------------------
            
            //! Returns the version number of the Mascot Parser library
            std::string getMSParserVersion() const;
            
            //! Returns true if there is an entry for the passed section 
            bool doesSectionExist(const section sec) const;

            //! Returns true if there is a peptides section, and if there are <I>any</I> results in it
            bool anyPeptideSummaryMatches(const section sec=SEC_PEPTIDES) const;

            // When calling 'getSectionValue' from outside the dll, be careful
            // that enough space is 'reserved' in the string.
            // Return value is length of the actual string that it wanted to
            // to return, so if this is larger than maxLen, then you are 
            // missing some of the string. Best to call getSectionValueStr
            //! Return the string value from any line in the results file
            int  getSectionValue(const section sec, const char * key, char * str, int maxLen) const;

            //! Return the integer value from any line in the results file
            int  getSectionValueInt(const section sec, const char * key) const;

            //! Return the floating point value from any line in the results file
            double getSectionValueDouble(const section sec, const char * key) const;

            //! Return the string value from any line in the results file
            std::string getSectionValueStr(const section sec, const char * key) const;

            //! Return the string value from a query in the results file
            int  getQuerySectionValue(const int query, const char * key, char * str, int maxLen) const;

            //! Return the integer value from a query in the results file
            int  getQuerySectionValueInt(const int query, const char * key) const;

            //! Return the floating point value from a query in the results file
            double getQuerySectionValueDouble(const int query, const char * key) const;

            //! Return the string value from a query in the results file
            std::string getQuerySectionValueStr(const int query, const char * key) const;

            //! Return the job number for this file - obtained from the file name
            int getJobNumber() const;

            //! Get the key name for each item in a section.
            std::string enumerateSectionKeys(const section sec, 
                                             const int num) const;


            //! Has the file loaded properly?
            bool isValid() const;

#ifndef SWIG
            // Not used from the outside world...
            void setError(int error, ...);
#endif

            //! Return the number of errors since the last call to clearAllErrors
            int  getNumberOfErrors() const;

            //! Remove all errors from the current list of errors
            void clearAllErrors();

            //! Return a specific error number - or ms_mascotresfile::ERR_NO_ERROR
            int  getErrorNumber(const int num = -1) const;

            //! Return the last error number - or ms_mascotresfile::ERR_NO_ERROR
            int  getLastError() const;

            //! Return a specific error as a string. 
            std::string getErrorString(const int num) const;

            //! Return the last error number - or an empty string
            std::string getLastErrorString() const;

            // ------------------- Basic helper  functions -------------------
            // In a long string, comma separated, this function can be
            // used to retrieve the sub strings. It also tries to retrieve an
            // accession string properly - assuming that is has quotes around 
            // it. 
            //! Helper function - mainly for internal library use
            bool getNextSubStr(const std::string & input, 
                               std::string::size_type & idx,
                               std::string & output,
                               const char * separator = ",",
                               const bool removeQuotes = false) const;


            // ----------------- Specific results functions ------------------
            //! Returns the number of queries (peptide masses or ms-ms spectra)
            int    getNumQueries()      const { return numQueries_;           }

            //! Returns the maximum number of hits possible for a protein summary
            int    getNumHits(const section sec=SEC_SUMMARY) const;

            //! Returns the number of sequences in the fasta file searched
            int    getNumSeqs()         const { return numSequences_;         }

            //! Returns the number of sequences that passed the taxonomy filter in the fasta file searched
            int    getNumSeqsAfterTax() const { return numSequencesAfterTax_; }

            //! Returns the number of residues in the fasta file searched
            double getNumResidues()     const { return numResidues_;          }

            //! Returns the execution time in seconds
            int    getExecTime()        const { return execTime_;             }

            //! Returns the date and time of the search in seconds since midnight January 1st 1970
            int    getDate()            const { return searchDate_;           }

            //! Returns the version of Mascot used to perform the search 
            std::string getMascotVer()  const { return version_;              }

            //! Returns the fasta file version
            std::string getFastaVer()   const { return fastaVersion_;         }

            //! Returns the unique task ID used by Mascot Daemon
            std::string getUniqueTaskID() const;

            //! Returns true if the search was a PMF search (SEARCH=PMF). 
            bool isPMF()            const;

            //! Returns true if the search was an MSMS search (SEARCH=MIS)
            bool isMSMS()           const;

            //! Returns true if the search was a sequence query search (SEARCH=SQ)
            bool isSQ()             const;

            //! Returns true if the search was an error tolerant search
            bool isErrorTolerant()  const;

            //! Returns true if any of the queries in the search just contain a single peptide mass
            bool anyPMF();

            //! Returns true if any of the queries in the search contain ions data
            bool anyMSMS();

            //! Returns true if any of the queries in the search contain seq or comp commands
            bool anySQ();

            //! Returns true if any of the queries in the search contain tag or etag commands
            bool anyTag();

            //! Returns the experimental mass value as entered by the user 
            double getObservedMass(const int query);

            //! The 'charge' returned will be 0 for Mr, otherwise it will be 1, -1, 2, -2, 3, -3 etc. and -100 for an error
            int    getObservedCharge(const int query);
            
            //! Returns the experimental mass value (as a relative mass) as entered by the user 
            double getObservedMrValue(const int query);

            //! Returns the experimental intensity for the peptide
            double getObservedIntensity(const int query);

            //! To perform a repeat search need to build up appropriate string
            std::string getRepeatSearchString(const int query);

            //! Returns the name of the results file passed into the constructor
            std::string getFileName() const  { return fileName_;             }

            //! Returns a reference to the search parameters class
            ms_searchparams & params() const { return *params_;              }

            //! Returns an object that represents quantitation-section as a reduced quantitation.xml file
            bool getQuantitation(ms_quant_configfile *qfile) const;

            //! Returns an object that represents unimod-section as a reduced unimod_2.xml file
            bool getUnimod(ms_umod_configfile *ufile) const;

            //! Returns an object that represents enzyme-section as a reduced 'enzymes' file
            bool getEnzyme(ms_enzymefile *efile) const;

            //! Returns an object that represents taxonomy-section as a reduced 'taxonomy' file
            bool getTaxonomy(ms_taxonomyfile *tfile) const;

            //! Outputs, for example: < !-- 23 seconds -->
            bool outputKeepAlive();

        protected:  
            // Not safe to copy or assign this object.
            ms_mascotresfile(const ms_mascotresfile & rhs);
            ms_mascotresfile & operator=(const ms_mascotresfile & rhs);
    
        private:
            std::string  fileName_;
            int          numQueries_;
            int          protSummaryHits_;
            int          numSequences_;        
            int          numSequencesAfterTax_;
            double       numResidues_;         
            int          execTime_;            
            int          searchDate_;          
            std::string  version_;             
            std::string  fastaVersion_; 
            int          keepAliveInterval_;
            std::string  keepAliveText_;
            time_t       keepAliveStartTime_;
            time_t       lastKeepAliveTime_;

            bool         anyMSMS_;
            bool         cachedAnyMSMS_;
            bool         anyPMF_;
            bool         cachedAnyPMF_;
            bool         anySQ_;
            bool         cachedAnySQ_;
            bool         anyTag_;
            bool         cachedAnyTag_;
            mutable bool isErrorTolerant_;
            mutable bool cachedIsErrorTolerant_;

            std::vector<int> errorNumbers_;
            std::vector<std::string>errorStrings_; // Will include 'user' data
            std::vector<int> errorRepeats_;        // So that we don't get strings of data

            ms_searchparams *params_;
            
            const char * sectionTitles_[SEC_NUMSECTIONS];

            multiBufMemPtr sectionStart_[SEC_NUMSECTIONS];
            multiBufMemPtr sectionEnd_  [SEC_NUMSECTIONS];

            multiBuf_v buffers_;

            std::string endSectionKey_;
            bool   isWinIniFormat_;
            multiBufMemPtr findSectionStart(const char * szSectionName,
                                           const multiBufMemPtr * startLookingAt = 0);
            multiBufMemPtr findSectionEnd(const multiBufMemPtr sectionStart);

            // Array of maps for each section
            typedef std::set<const char *,ms_sortByKeyCriterion> sortedKeys;
            sortedKeys sorted_[SEC_NUMSECTIONS];
            bool fillUpSortedList(const int section,
                                  const multiBufMemPtr sectionStart,
                                  const multiBufMemPtr sectionEnd,
                                  sortedKeys & sorted_keys);

            // There are an 'unknown' number of query sections
            bool hasQuerySectionBeenIndexed;
            std::vector<multiBufMemPtr> querySectionStart_;
            std::vector<multiBufMemPtr> querySectionEnd_;
            std::vector<sortedKeys> sortedQueries_;

            // Private function to get string
            bool inDLLgetSectVal(const multiBufMemPtr sectionStart,
                                 const multiBufMemPtr sectionEnd,
                                 sortedKeys & sorted_keys,
                                 const char * key,
                                 std::string & result) const;

            // Private function to get string
            bool inDLLgetSectionAsString(const multiBufMemPtr sectionStart,
                                        const multiBufMemPtr sectionEnd,
                                        std::string & result) const;

            bool readFile(const char * szFileName);
            void debugCheckReadFileOK();

    };
    /** @} */ // end of resfile_group
}   // matrix_science namespace
#endif // MS_MASCOTRESFILE_HPP

/*------------------------------- End of File -------------------------------*/
