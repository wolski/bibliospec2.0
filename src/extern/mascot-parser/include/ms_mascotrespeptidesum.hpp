/*
##############################################################################
# file: ms_mascotrespeptidesum.hpp                                           #
# 'msparser' toolkit                                                         #
# Encapsulates the peptide summary report from the mascot results file       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2002 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /MowseBranches/ms_mascotresfile_1.2/include/ms_mascotrespe $ #
#     $Author: davidc $ #
#       $Date: 2006-12-19 15:13:03 $ #
#   $Revision: 1.20 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESPEPTIDESUM_HPP
#define MS_MASCOTRESPEPTIDESUM_HPP

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

#ifdef __ALPHA_UNIX__
#include <ctype.h>
#endif

// Includes from the standard template library
#include <string>
#include <list>
#include <vector>
#include <set>


namespace matrix_science {

    /** @addtogroup resfile_group
     *  
     *  @{
     */
/*!
 * @copydoc ms_mascotresults
 */

    //! Use this class to get peptide summary results
    /*! This class inherits from ms_mascotresults, and all the functions 
     * except for getPeptide() are documented there. Create an ms_peptidesummary
     * or ms_proteinsummary object rather than an ms_mascotresults object
     */
    class MS_MASCOTRESFILE_API ms_peptidesummary : public matrix_science::ms_mascotresults
    {
        public:
            //! Call this constructor once to create peptide summary results
            ms_peptidesummary(ms_mascotresfile  &resfile,
                              const unsigned int flags = MSRES_GROUP_PROTEINS,
                              double             minProbability = 0.0,
                              int                maxHits = 50,
                              const char *       unigeneIndexFile = 0,
                              double             ignoreIonsScoreBelow = 0.0,
                              int                minPepLenInPepSummary = 0,
                              const char *       singleHit = 0);
            virtual ~ms_peptidesummary();
            
            // This is 'hard-coded' to 10 in Mascot
            enum { PEPS_PER_QUERY = 10 };

            //! Return a peptide object for the specified query / rank
            virtual ms_peptide getPeptide(const int q, const int p) const;

            //! Return a peptide object for the specified query / rank
            virtual bool getPeptide(const int q, const int p, ms_peptide * & pep) const;

            //! Return a list of proteins that matched the same peptide
            virtual std::string getProteinsWithThisPepMatch(const int q, const int p, const bool quotes=false);

            //! Return the complete error tolerant mod string from h1_q2_et_mods, or q1_p1_et_mods
            virtual std::string getErrTolModString(const int q, const int p) const;

            //! Return the complete error tolerant mod master neutral loss string from q1_p1_et_mods_master
            virtual std::string getErrTolModMasterString(const int q, const int p) const;

            //! Return the complete error tolerant mod slave neutral loss string from q1_p1_et_mods_slave
            virtual std::string getErrTolModSlaveString(const int q, const int p) const;

            //! Return the complete error tolerant mod peptide neutral loss string from q1_p1_et_mods_pep
            virtual std::string getErrTolModPepString(const int q, const int p) const;

            //! Return the complete error tolerant mod required peptide neutral loss string from q1_p1_et_mods_reqpep
            virtual std::string getErrTolModReqPepString(const int q, const int p) const;

            //! Return the complete tag string from q1_p1_tag
            virtual std::string getTagString(const int q, const int p) const;

            //! Return the first number from q1_p2_drange=0,256
            virtual int getTagDeltaRangeStart(const int q, const int p) const;

            //! Return the second number from q1_p2_drange=0,256
            virtual int getTagDeltaRangeEnd(const int q, const int p) const;

            //! Return the complete terminal residue string from q1_p1_terms
            virtual std::string getTerminalResiduesString(const int q, const int p) const;

            //! Return q1_p2_comp string value
            virtual std::string getComponentString(const int q, const int p) const;

            //! Returns the maximum 'rank' or 'hit' or 'p' value 
            virtual int getMaxRankValue() const;

            //! Return the 'protein' score value for cutting off results. Different for peptide and protein summary
            virtual int getProteinScoreCutoff(double OneInXprobRnd) const;

            //! Returns the 'source' rank for a given peptide match
            int getSrcRank(int q, int p) const;

            //! Returns the 'source' section for a given peptide match
            ms_mascotresfile::section getSrcSection(int q, int p)const;

        protected:  
            // Not safe to copy or assign this object.
            ms_peptidesummary(const ms_peptidesummary & rhs);
            ms_peptidesummary & operator=(const ms_peptidesummary & rhs);
            void calculateDecoyStats(double dOneInXprobRnd);
            virtual bool isValidQandP(const int q, const int p) const;

        private:
            ms_peptide * loadPepRes(const ms_mascotresfile::section sec,
                                    int q, int p, double minIonsScore,
                                    std::string::size_type & idx,
                                    bool loadAccessions);
            void loadIntoProteins(const ms_mascotresfile::section sec,
                                  std::string str,
                                  std::string::size_type idx,
                                  int q, int p, int rank,
                                  double ionsScore);

            double            ignoreIonsScoreBelow_;

            // Some private variables and functions for error tolerant search
            bool checkErrorTolerantStatus(int q, int p, double ionsScore, 
                                          bool fromET,
                                          const std::string & pepStr);
            ms_mascotresfile  * errTolSource_;
            ms_peptidesummary * errTolPepSummary_;
            unsigned int        errTolType_;
            bool                missingErrTolParent_;
            std::vector<unsigned char> srcRank_;
            bool srcRankInitialised_;

            void setSrcRank(int q, int p, int srcRank, ms_mascotresfile::section peptideSec);
            int getSrcRank(int q, int p, ms_mascotresfile::section & peptideSec) const;
            enum PEP_SECTIONS { PEP_SEC_INVALID, PEP_SEC_PEPTIDES, PEP_SEC_DECOYPEPTIDES, PEP_SEC_ERRTOLPEPTIDES, PEP_SEC_LAST};
            unsigned char secLookupFwd_[ms_mascotresfile::SEC_NUMSECTIONS];
            int secLookupRev_[PEP_SEC_LAST];
            double avePepIdentThresh_;
    };
    /** @} */ // end of resfile_group
}   // matrix_science namespace

#endif // MS_MASCOTRESPEPTIDESUM_HPP

/*------------------------------- End of File -------------------------------*/
