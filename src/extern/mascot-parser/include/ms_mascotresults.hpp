/*
##############################################################################
# file: ms_mascotresults.hpp                                                 #
# 'msparser' toolkit                                                         #
# Abstract class for either ms_peptidesummary or ms_proteinsummary           #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2002 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /MowseBranches/ms_mascotresfile_1.2/include/ms_mascotresul $ #
#     $Author: davidc $ #
#       $Date: 2008-03-14 16:02:36 $ #
#   $Revision: 1.33 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESULTS_HPP
#define MS_MASCOTRESULTS_HPP

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
#include <list>
#include <vector>
#include <set>
#include <map>

#ifdef __AIX__
#undef SCORE
#endif

namespace matrix_science {
    /** @addtogroup resfile_group
     *  
     *  @{
     */

    class ms_unigene;

#ifndef SWIG
    // Helper class - don't use from outside library
    class ms_proteinSortByAccessionCriterion
    {
        public:
            bool operator() (const ms_protein &p1, const ms_protein &p2) const
            {
                // see parser bug 493: we only need to change the behaviour
                // for protein summary (where hit != 0)
                if ( p1.getProteinSummaryHit() == 0 ) {
                    return p1.getAccession() < p2.getAccession();
                }

                if ( p1.getAccession() < p2.getAccession() )
                    return true;
                else if ( p1.getAccession() > p2.getAccession() )
                    return false;
                else
                    return (p1.getFrame() < p2.getFrame());
            }
    };

    class ms_mascotresults;

    // Helper class - don't use from outside library
    class ms_proteinSortByScore
    {
        public:
            enum SORTBY
            {
                SORTBY_SCORE_THEN_ACC,
                SORTBY_SCORE_THEN_HIT,
                SORTBY_SCORE_THEN_DELTA
            };
            ms_proteinSortByScore(SORTBY s, const ms_mascotresults * r = 0): s_(s), r_(r) {}

            bool operator() (const ms_protein * p1, const ms_protein * p2) const;

        private:
            SORTBY s_;
            const ms_mascotresults * r_;
    };

    // Helper class - don't use from outside library
    class ms_unassignedSortBy
    {
        public:
            enum sortBy {QUERY, SCORE, INTENSITY};

        private:
            sortBy mode_;

        public:
            ms_unassignedSortBy(sortBy mode=QUERY) : mode_(mode) {}

            bool operator() (const ms_peptide *p1, const ms_peptide *p2) const
            {
                switch (mode_)
                {
                case QUERY:
                    return (p1->getQuery() < p2->getQuery());

                case SCORE:
                    return (p1->getIonsScore() > p2->getIonsScore());

                case INTENSITY:
                    return (p1->getIonsIntensity() > p2->getIonsIntensity());

                default:  // Keep some compilers quiet....
                    return (p1->getQuery() < p2->getQuery());
                }
            }
    };
#endif // SWIG

    //! Abstract class for either ms_peptidesummary or ms_proteinsummary
    /*!
     * Also see the documentation for ms_peptidesummary or ms_proteinsummary.
     * The following functions provide threshold values: <BR>
     * <UL>
     * <li>getPeptideIdentityThreshold() </li>
     * <li>getAvePeptideIdentityThreshold() </li>
     * <li>getMaxPeptideIdentityThreshold() </li>
     * <li>getProteinThreshold() </li>
     * <li>getHomologyThreshold() </li>
     * <li>getHomologyThresholdForHistogram() </li>
     * <li>getProbFromScore() </li>
     * <li>getProbOfPepBeingRandomMatch() </li>
     * <li>double getProbOfProteinBeingRandomMatch() </li>
     * </UL>
     * 
     */
    class MS_MASCOTRESFILE_API ms_mascotresults
    {
        public:

            //! flags for the type of results
            /*!
             * To access from perl, use, for example: <tt> <BR>
             * $x = $msparser::ms_mascotresults::MSRES_GROUP_PROTEINS <BR>
             * </tt>
             * Not all are applicable for protein summary (e.g. 
             * MSRES_REQUIRE_BOLD_RED)
             *
             */
            enum FLAGS 
            { 

                MSRES_NOFLAG                    = 0x00000000, //!< Does nothing
                MSRES_GROUP_PROTEINS            = 0x00000001, //!< Group proteins with same peptide matches. See \ref subsetsPage
                MSRES_SHOW_SUBSETS              = 0x00000002, //!< Show proteins that only match a subset of peptides. See \ref subsetsPage
                MSRES_SUBSETS_DIFF_PROT         = 0x00000004, //!< Proteins that contain a subset of peptides are treated as a unique protein. See \ref subsetsPage
                MSRES_REQUIRE_BOLD_RED          = 0x00000008, //!< Only proteins that have a top scoring peptide not seen before will be returned
                MSRES_SHOW_ALL_FROM_ERR_TOL     = 0x00000010, //!< If this flag is set, then all hits from error tolerant search are shown. See \ref errorTolerantPage
                MSRES_IGNORE_PMF_MIXTURE        = 0x00000020, //!< If this flag is set, then PMF mixtures are ignored. See \ref pmfmixturePage
                MSRES_MUDPIT_PROTEIN_SCORE      = 0x00000040, //!< Protein scoring for the peptide summary was changed at Mascot v2.0 for large (MudPIT) searches. See ms_protein::getScore()
                MSRES_DECOY                     = 0x00000080, //!< If this flag is set, then use the results from searching against the decoy database. See \ref decoySearchPage
                MSRES_INTEGRATED_ERR_TOL        = 0x00000100, //!< If this flag is set, then create a ms_peptidesummary object that contains results from the summary and et_summary section. See \ref errorTolerantPage
                MSRES_ERR_TOL                   = 0x00000200, //!< If this flag is set, then create a ms_peptidesummary object that contains results from the et_summary section. See \ref errorTolerantPage
                MSRES_MAXHITS_OVERRIDES_MINPROB = 0x00000400, //!< If minProbability and maxHitsToReport are both non zero, then minProbability is ignored when determining the number of proteins to be displayed. See ms_mascotresults::ms_mascotresults
                MSRES_DUPE_INCL_IN_SCORE_NONE   = 0x00000000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_A      = 0x00002000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_B      = 0x00004000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_C      = 0x00008000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_D      = 0x00010000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_E      = 0x00020000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_F      = 0x00040000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_G      = 0x00080000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_H      = 0x00100000, //!< See \ref duplicatesPage
                MSRES_DUPE_INCL_IN_SCORE_I      = 0x00200000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_NONE          = 0x00400000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_A             = 0x00800000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_B             = 0x01000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_C             = 0x02000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_D             = 0x04000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_E             = 0x08000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_F             = 0x10000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_G             = 0x20000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_H             = 0x40000000, //!< See \ref duplicatesPage
                MSRES_DUPE_REMOVE_I             = 0x80000000, //!< See \ref duplicatesPage
                MSRES_DUPE_DEFAULT              = 0x04800000  //!< Default parameter for treatment of duplicates. See \ref duplicatesPage
            };

            enum sortBy 
            {
                QUERY, 
                SCORE, 
                INTENSITY
            };

            enum dupeStatus
            {
                dupe_query_same = 0x0001,  //!< See \ref duplicatesPage
                dupe_seq_same   = 0x0002,  //!< See \ref duplicatesPage
                dupe_mods_same  = 0x0004,  //!< See \ref duplicatesPage
                dupe_pos_same   = 0x0008   //!< See \ref duplicatesPage
            };


            //! Use ms_peptidesummary::ms_peptidesummary or ms_proteinsummary::ms_proteinsummary
            ms_mascotresults(ms_mascotresfile  &resfile,
                             const unsigned int flags,
                             double             minProbability,
                             int                maxHitsToReport,
                             const char *       unigeneIndexFile,
                             const char *       singleHit = 0);
            virtual ~ms_mascotresults();
            
            //! Return the ms_protein hit - returns null if hit > number of hits
            virtual ms_protein * getHit(const int hit) const;

            //! Returns the number of hits in the results
            virtual int getNumberOfHits() const;

            //! Return protein description if available.
            std::string getProteinDescription(const char * accession) const;

            //! Return protein mass if available.
            double getProteinMass(const char * accession) const;

            //! Return the mass of a sequence (protein or peptide)
            double getSequenceMass(const char * seq) const;

            //! Return a pointer to the protein entry given an accession
            const ms_protein * getProtein(const char * accession) const;

            //! Return a pointer to the protein entry given an accession
            const ms_protein * getComponentProtein(const char * accession) const;

            //! Return the next protein that contains all the peptides in the 'master' protein
            virtual ms_protein * getNextSimilarProtein(const int masterHit, const int subset) const;

            //! Return the next protein that contains some of the peptides in the 'master' protein
            virtual ms_protein * getNextSubsetProtein(const int masterHit, const int subset) const;

            //! Return the ms_peptide object given the query and either the rank (ms_peptidesummary) or the hit (ms_proteinsummary)
            virtual ms_peptide getPeptide(const int q, const int p) const = 0;

            //! Return the ms_peptide object given the query and either the rank (ms_peptidesummary) or the hit (ms_proteinsummary)
            virtual bool getPeptide(const int q, const int p, ms_peptide * & pep) const = 0;

            //! Return the threshold value for this ms-ms data being a random match
	        virtual int getPeptideIdentityThreshold(const int query, double OneInXprobRnd) const;

            //! Return the average threshold value for all ms-ms data sets
            virtual int getAvePeptideIdentityThreshold(double OneInXprobRnd) const;

            //! Return the max threshold value for all ms-ms data sets
            virtual int getMaxPeptideIdentityThreshold(double OneInXprobRnd) const;

            //! Return the 'protein' score value for cutting off results. Different for peptide and protein summary
            virtual int getProteinScoreCutoff(double OneInXprobRnd) const = 0;

            //! Return a threshold value for the protein summary report
            virtual int getProteinThreshold(double OneInXprobRnd) const;

            //! Returns the 'homology' threshold
            virtual int getHomologyThreshold(const int query,
                                             double OneInXprobRnd,
                                             const int rank=1) const;

            //! Returns the value for the 'yellow section' in the histogram
            virtual int getHomologyThresholdForHistogram(double OneInXprobRnd) const;

            //! Returns a probability value given a score
            virtual int getProbFromScore(const double score) const;

            //! Returns the expectation value for the given peptide score and query
            virtual double getPeptideExpectationValue(const double score, 
                                                      const int query) const;

            //! Deprecated - use getPeptideExpectationValue()
            virtual double getProbOfPepBeingRandomMatch(const double score, 
                                                        const int query) const;

            //! Returns the expectation value for the given protein score 
            virtual double getProteinExpectationValue(const double score) const;

            //! Deprecated - use getProteinExpectationValue()
            virtual double getProbOfProteinBeingRandomMatch(const double score) const;

            //! Return a list of proteins that matched the same peptide
            virtual std::string getProteinsWithThisPepMatch(const int q, const int p, const bool quotes=false) = 0;

            //! Return the complete error tolerant mod string from h1_q2_et_mods, or q1_p1_et_mods
            virtual std::string getErrTolModString(const int q, const int p) const = 0;

            //! Return the error tolerant mod primary neutral loss string from h1_q2_et_mods_master, or q1_p1_et_mods_master
            virtual std::string getErrTolModMasterString(const int q, const int p) const = 0;

            //! Return the error tolerant mod slave neutral loss string from h1_q2_et_mods_slave, or q1_p1_et_mods_slave
            virtual std::string getErrTolModSlaveString(const int q, const int p) const = 0;

            //! Return the error tolerant mod peptide neutral loss string from h1_q2_et_mods_pep, or q1_p1_et_mods_pep
            virtual std::string getErrTolModPepString(const int q, const int p) const = 0;

            //! Return the error tolerant mod required peptide neutral loss string from h1_q2_et_mods_reqpep, or q1_p1_et_mods_reqpep
            virtual std::string getErrTolModReqPepString(const int q, const int p) const = 0;

            //! Return the error tolerant mod name from h1_q2_et_mods, or q1_p1_et_mods
            virtual std::string getErrTolModName(const int q, const int p) const;

            //! Return the error tolerant mod delta from h1_q2_et_mods, or q1_p1_et_mods
            virtual double getErrTolModDelta(const int q, const int p) const;

            //! Return the error tolerant mod neutral loss from h1_q2_et_mods, or q1_p1_et_mods
            virtual double getErrTolModNeutralLoss(const int q, const int p) const;

            //! Return the error tolerant mod additional primary neutral losses from h1_q2_et_mods_master, or q1_p1_et_mods_master
            virtual std::vector<double> getErrTolModMasterNeutralLoss(const int q, const int p) const;

            //! Return the error tolerant mod slave neutral losses from h1_q2_et_mods_slave, or q1_p1_et_mods_slave
            virtual std::vector<double> getErrTolModSlaveNeutralLoss(const int q, const int p) const;

            //! Return the error tolerant mod peptide neutral losses from h1_q2_et_mods_pep, or q1_p1_et_mods_pep
            virtual std::vector<double> getErrTolModPepNeutralLoss(const int q, const int p) const;

            //! Return the error tolerant mod peptide neutral losses from h1_q2_et_mods_reqpep, or q1_p1_et_mods_reqpep
            virtual std::vector<double> getErrTolModReqPepNeutralLoss(const int q, const int p) const;

            //! Return a 'human readable' string with the variable and error tolerant mods 
            virtual std::string getReadableVarMods(const int q, const int p,
                                                   const int numDecimalPlaces=2) const;

            //! Return the complete tag string from h1_q2_tag or q1_p1_tag
            virtual std::string getTagString(const int q, const int p) const = 0;

            //! Return the start position for the tag-match from h1_q2_tag or q1_p1_tag
            virtual int getTagStart(const int q, const int p, const int tagNumber) const;

            //! Return the end position for the tag-match from h1_q2_tag or q1_p1_tag
            virtual int getTagEnd(const int q, const int p, const int tagNumber) const;

            //! Return the series ID for the tag-match from h1_q2_tag or q1_p1_tag
            virtual int getTagSeries(const int q, const int p, const int tagNumber) const;

            //! Return the first number from h1_q2_drange=0,256
            virtual int getTagDeltaRangeStart(const int q, const int p) const = 0;

            //! Return the second number from h1_q2_drange=0,256
            virtual int getTagDeltaRangeEnd(const int q, const int p) const = 0;

            //! Return the complete terminal residue string from h1_q1_terms or q1_p1_terms
            virtual std::string getTerminalResiduesString(const int q, const int p) const = 0;

            //! Return q1_p2_comp string value, for h1_q2 this string is always empty
            virtual std::string getComponentString(const int q, const int p) const = 0;

            //! Returns the maximum 'rank' or 'hit' or 'p' value 
            virtual int getMaxRankValue() const = 0;

            //! To have a list of unassigned peptides, need to call this first
            bool createUnassignedList(sortBy s  = QUERY);

            //! Need to call createUnassignedList() before calling this
            int getNumberOfUnassigned() const;

            //! Need to call createUnassignedList() before calling this
            ms_peptide getUnassigned(const int num) const;

            //! Returns true if the item indexed by num in the assigned list should be bold 
            bool getUnassignedIsBold(const int num) const;

            //! Returns true if the item indexed by num in the assigned list should have a check box next to it
            bool getUnassignedShowCheckbox(const int num) const;

            void addProtein(const std::string & accession, 
                            const int frame,
                            const long start, const long end, 
                            const long multiplicity,
                            const int q, const int p,
                            const double score,
                            const double uncorrectedScore,
                            const char residueBefore,
                            const char residueAfter,
                            const ms_protein * component = 0,
                            const bool integratedET = false);

            const ms_protein * addComponentProtein(const std::string & accession, 
                                                   const int frame,
                                                   const long start, 
                                                   const long end, 
                                                   const long multiplicity,
                                                   const int q, const int p,
                                                   const double score,
                                                   const double uncorrectedScore,
                                                   const char residueBefore,
                                                   const char residueAfter,
                                                   const ms_protein * component = 0,
                                                   const bool integratedET = false);

            //! Returns scores for top 50 proteins, even if less in the peptidesummary or proteinsummary
            virtual double getProteinScoreForHistogram(const int num) const;

            //! Returns TRUE for a search against a nucelic acid database
            bool isNA() const;

            //! Returns the flags as specified in the constructor. See ms_mascotresults::FLAGS 
            unsigned int getFlags() const { return flags_; }

            //! Peptides shorter than this are ignored when putting proteins into groups
            int getMinPepLenInPepSummary() const;

            //! Return the number of hits with a score at or above the identity threshold
            virtual long getNumHitsAboveIdentity(double OneInXprobRnd);

            //! Return the number of hits from the decoy search with a score at or above the identity threshold
            virtual long getNumDecoyHitsAboveIdentity(double OneInXprobRnd);

            //! Return the number of hits with a score at or above the homology threshold
            virtual long getNumHitsAboveHomology(double OneInXprobRnd);

            //! Return the number of hits from the decoy search with a score at or above the homology threshold
            virtual long getNumDecoyHitsAboveHomology(double OneInXprobRnd);

            //! Specifies which subset proteins should be reported
            virtual void setSubsetsThreshold(const double scoreFraction);

            //! Return the minProbability value passed to the ms_mascotresults::ms_mascotresults constructor 
            virtual double getProbabilityThreshold() const;

            //! Used internally in the library. See \ref duplicatesPage
            bool queryRemoveThisPeptide(const unsigned short dupeFlags) const;

            //! Used internally in the library. See \ref duplicatesPage
            bool queryScoreThisPeptide(const unsigned short dupeFlags) const;

        protected:
            // Not safe to copy or assign this object.
            ms_mascotresults(const ms_mascotresults & rhs);
            ms_mascotresults & operator=(const ms_mascotresults & rhs);
            virtual void calculateDecoyStats(double dOneInXprobRnd) = 0;
            virtual bool isValidQandP(const int q, const int p) const = 0;

            ms_mascotresfile &resfile_;
            int   numQueries_;
            double tolFactor_;

            typedef std::set<ms_protein,ms_proteinSortByAccessionCriterion> proteinSet;
            proteinSet proteins_;

            // For unigene, the original proteins are not saved in proteins_
            proteinSet componentProteins_;

            ms_proteinSortByScore * proteinSorter_;

            typedef std::multiset<ms_protein *,ms_proteinSortByScore> sortedSet;
            sortedSet * sorted_;

            // - Not documented - Get the 'corrected' ions score given multiplicity
            double getIonsScoreCorrected(const double ionsScore, 
                                         const long   multiplicity) const;

            unsigned int      flags_;
            double            minProbability_;
            int               maxHitsToReport_;
            bool              extraProtAfterMinProtScore_;
            bool              groupByQueryNumber_;
            int               numProteins_;
            std::string       unigeneIndexFile_;
            ms_unigene      * unigene_;
            bool              tooOld_;
            int               minPepLenInPepSummary_;
            std::string       singleHit_;


            // The elements in the vector of peptides are accessed by
            // q + (p * num queries)
            std::vector<ms_peptide *> peptides_;
            void sortByScore();
            bool setBoldAndCheckBoxItems(ms_protein * prot,
                                         const int proteinNumber);
            //! Return the threshold value for this ms-ms data being a random match
	        virtual int getPepIdentThreshProtected(const int query, 
                                                   double OneInXprobRnd,
                                                   ms_mascotresfile::section sec,
                                                   double * pQmatch = 0) const;
            virtual int getHomologyThreshProtected(const int query,
                                                   double OneInXprobRnd,
                                                   ms_mascotresfile::section sec,
                                                   const int rank=1) const;


            typedef std::multiset<ms_peptide *, ms_unassignedSortBy> unassignedSet;
            unassignedSet * unassignedList_;
            double top50Scores[50];
            std::vector<unsigned short>dupeRemoveIDs_;
            std::vector<unsigned short>dupeIncludeInScoreIDs_;

            bool bDecoyStatsCalculated_;
            double dOneInXprobRndForDecoy_;
            long numHitsAboveIdentity_;
            long numDecoyHitsAboveIdentity_;
            long numHitsAboveHomology_;
            long numDecoyHitsAboveHomology_;
            ms_mascotresfile::section secSummary_;
            ms_mascotresfile::section secMixture_;
            ms_mascotresfile::section secPeptides_;
            ms_mascotresfile::section secProteins_;
            double subsetsScoreFraction_;

        private:
            mutable bool   cachedHomology_[2];
            mutable double cachedHomologyProb_[2];
            mutable std::vector<double> cachedHomologyValues_[2];
    };
    /** @} */ // end of resfile_group
}   // matrix_science namespace

#endif // MS_MASCOTRESULTS_HPP

/*------------------------------- End of File -------------------------------*/
