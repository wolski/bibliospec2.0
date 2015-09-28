/*
##############################################################################
# file: ms_mascotresproteinsum.cpp                                           #
# 'msparser' toolkit                                                         #
# Encapsulates the protein summary report from the mascot results file       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2002 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /MowseBranches/ms_mascotresfile_1.2/include/ms_mascotrespr $ #
#     $Author: davidc $ #
#       $Date: 2007-02-12 10:55:44 $ #
#   $Revision: 1.17 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESPROTEINSUM_HPP
#define MS_MASCOTRESPROTEINSUM_HPP

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
#include <ctype.h>
#endif

// Includes from the standard template library
#include <string>


namespace matrix_science {
    /** @addtogroup resfile_group
     *  
     *  @{
     */
    //! Use this class to get protein summary results
    /*! This class inherits from ms_mascotresults, and all the functions 
     * except for getPeptide() are documented there. Create an ms_peptidesummary
     * or ms_proteinsummary object rather than an ms_mascotresults object
     */
    class MS_MASCOTRESFILE_API ms_proteinsummary : public ms_mascotresults
    {
        public:
            //! Call this constructor once to create peptide summary results
            ms_proteinsummary(ms_mascotresfile  &resfile,
                const unsigned int flags = ms_mascotresults::MSRES_GROUP_PROTEINS 
                                         | ms_mascotresults::MSRES_SHOW_SUBSETS,
                              double             minProbability = 0.0,
                              int                maxHitsToReport = 50,
                              const char *       unigeneIndexFile = 0,
                              const char *       singleHit = 0);
            virtual ~ms_proteinsummary();

            //! Return a peptide object for the specified query / hit
            virtual ms_peptide getPeptide(const int q, const int p) const;

            //! Return a peptide object for the specified query / hit
            virtual bool getPeptide(const int q, const int h, ms_peptide * & pep) const;

            //! Return a list of proteins that matched the same peptide
            virtual std::string getProteinsWithThisPepMatch(const int q, const int p, const bool quotes=false);

            //! Return the complete error tolerant mod string from h1_q2_et_mods, or q1_p1_et_mods
            virtual std::string getErrTolModString(const int q, const int p) const;

            //! Return the complete error tolerant mod primary neutral loss string from h1_q1_et_mods_primary
            virtual std::string getErrTolModMasterString(const int q, const int p) const;

            //! Return the complete error tolerant mod slave neutral loss string from h1_q1_et_mods_slave
            virtual std::string getErrTolModSlaveString(const int q, const int p) const;

            //! Return the complete error tolerant mod peptide neutral loss string from h1_q1_et_mods_pep
            virtual std::string getErrTolModPepString(const int q, const int p) const;

            //! Return the complete error tolerant mod required peptide neutral loss string from h1_q1_et_mods_reqpep
            virtual std::string getErrTolModReqPepString(const int q, const int p) const;

            //! Return the complete tag string from h1_q2_tag or q1_p1_tag
            virtual std::string getTagString(const int q, const int p) const;

            //! Return the first number from h1_q2_drange=0,256
            virtual int getTagDeltaRangeStart(const int q, const int p) const;

            //! Return the second number from h1_q2_drange=0,256
            virtual int getTagDeltaRangeEnd(const int q, const int p) const;

            //! Return the complete terminal residue string from h1_q1_terms
            virtual std::string getTerminalResiduesString(const int q, const int h) const;

            //! Return an empty string
            virtual std::string getComponentString(const int q, const int h) const;

            //! Returns the maximum 'rank' or 'hit' or 'p' value 
            virtual int getMaxRankValue() const;

            //! Return the 'protein' score value for cutting off results. Different for peptide and protein summary
            virtual int getProteinScoreCutoff(double OneInXprobRnd) const;

            //! Returns number of queries (masses) used when calculating PMF protein scores
            int    getNumPmfQueriesUsed() const;

            //! Indicates whether a mass value was used when calculating PMF protein scores
            bool   isPmfQueryUsed(const int queryIdx) const;

        protected:  
            // Not safe to copy or assign this object.
            ms_proteinsummary(const ms_proteinsummary & rhs);
            ms_proteinsummary & operator=(const ms_proteinsummary & rhs);
            void calculateDecoyStats( double dOneInXprobRnd);
            virtual bool isValidQandP(const int q, const int h) const;

        private:
            bool parseProtein(const int hit);
            void loadPepRes(const int hit, 
                            std::string & accession, 
                            const int frame);
            void addMixtureHits();
            int singleHitAsInt_;
            ms_peptide emptyPeptide_;

            bool         isPmfRanks_;
            int          numPmfQueriesUsed_;
            std::vector<int> pmfQueriesUsed_;
            int          numPmfHitsPreserved_;


    };
    /** @} */ // end of resfile_group
}   // matrix_science namespace

#endif // MS_MASCOTRESPROTEINSUM_HPP

/*------------------------------- End of File -------------------------------*/
