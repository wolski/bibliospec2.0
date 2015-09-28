/*
##############################################################################
# file: ms_mascotresprotein.hpp                                              #
# 'msparser' toolkit                                                         #
# Encapsulates a protein - either for protein summary or peptide summary     #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2002 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_mascotresprotein.hpp    $ #
#     $Author: davidc $ #
#       $Date: 2006-12-19 15:13:03 $ #
#   $Revision: 1.14 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESPROTEIN_HPP
#define MS_MASCOTRESPROTEIN_HPP

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


namespace matrix_science {

    class ms_mascotresults;
    class ms_pepinfoSortByScore;

    /** @addtogroup resfile_group
     *  
     *  @{
     */

    //! This class encapsulates a protein in the mascot results file
    /*!
     * A pointer to one of these is returned from 
     * ms_peptidesummary::getHit() or ms_proteinsummary::getHit()
     * so there should be no need to create one of these from outside the library.
     */
    class MS_MASCOTRESFILE_API ms_protein
    {
        public:
            //! enum to say if a protein is similar to another higher scoring protein
            /*!
             * To access from perl, use, for example: <P><tt>
             * $x = $msparser::ms_protein::GROUP_PARTIAL
             * </tt></P><BR>
             *
             * See \ref subsetsPage <BR>
             * Note that if there are say 3 proteins with the same 4 
             * peptide matches, then the highest scoring protein will have
             * GROUP_NO, and the other two will have GROUP_COMPLETE. Calling
             * getSimilarProteinName() on the highest scoring protein 
             * will return an empty string. Calling it for the other two proteins
             * will return the accession for the highest scoring protein.
             * See also ms_mascotresults::getNextSimilarProtein() and
             * ms_mascotresults::getNextSubsetProtein()
             */
            enum GROUP 
            { 
                GROUP_UNKNOWN, //!< No information about grouping
                GROUP_NO,      //!< Does not contain same set (or subset) of peptides as another proteins.
                GROUP_SUBSET,  //!< Contains a subset of peptides in one ore more other proteins.
                GROUP_COMPLETE //!< Contains an identical set of peptides to one or more other proteins.
            };
            // enum for the each peptide in the protein to indicate if it is a duplicate
            /*!
             * A protein match is made up of one or more peptides. Duplicate
             * peptides don't increase the coverage of the protein. They also
             * do not increase the score except for MudPIT scoring
             */
            enum DUPLICATE 
            { 
                DUPE_NotDuplicate,           //!< There are no other peptides with the same sequence in this protein - from this query or other queries
                DUPE_Duplicate,              //!< Another peptide from a different query with the same sequence as this got a higher score
                DUPE_DuplicateSameQuery,     //!< Another match for the same query with the same peptide string got a higher score (different mods)
                DUPE_HighestScoringDuplicate //!< There is at least one other peptide the same as this with a lower score
            };


            //! Constructors - used from ms_proteinsummary and ms_peptidesummary.
            ms_protein(const double score,
                       const std::string accession,
                       const bool updateScoreFromPepScores,
                       const int  proteinSummaryHit = 0);

            //! Destructor - called automatically - don't call explicitly from Perl or Java
            virtual ~ms_protein();

            //! Return the accession string for a protein
            std::string getAccession() const;

            //! Return the protein score for this protein.
            double getScore()          const;

            //! Will only return a different score from getScore() if the MSRES_MUDPIT_PROTEIN_SCORE flag has been specified.
            double getNonMudpitScore()     const;

            //! Return the number of peptides that had a match in this protein
            int    getNumPeptides()    const;

            //! Return the number of peptides excluding those that with duplicate matches to same query
            int    getNumDisplayPeptides()    const;

            //! Does this protein only contain the same peptides that as those in another protein?
            inline GROUP getGrouping()  const { return group_;          }

            //! Set grouping - called from within library - do not call from outside the library. 
            void setGrouping(GROUP g)         { group_ = g;       }

            //! Return the query number given the peptide 'number'
            int    getPeptideQuery         (const int   pepNumber) const;

            //! Return the 'rank' number given the peptide 'number'
            int    getPeptideP             (const int   pepNumber) const;

            //! Return the frame number given the peptide 'number'
            int    getPeptideFrame         (const int   pepNumber) const;

            //! Return the peptide start residue given the peptide 'number'
            long   getPeptideStart         (const int   pepNumber) const;

            //! Return the peptide end residue given the peptide 'number'
            long   getPeptideEnd           (const int   pepNumber) const;

            //! Return the number of precursor matches in this protein for the specified peptide 'number'
            long   getPeptideMultiplicity  (const int   pepNumber) const;

            //! Return the DUPLICATE status given the peptide 'number'
            DUPLICATE getPeptideDuplicate  (const int   pepNumber) const;

            //! Return the ions score within this protein context given the peptide 'number'
            double getPeptideIonsScore     (const int   pepNumber) const;
            
            //! Returns true if this peptide should be displayed in bold in a Mascot report
            bool   getPeptideIsBold        (const int   pepNumber) const;
            
            //! Should only be used internally in the library
            void   setPeptideIsBold        (const int   pepNumber);

            //! Returns true if a check box for repeat searches should be shown in a Mascot report
            bool   getPeptideShowCheckbox  (const int   pepNumber) const;
            
            //! Should only be used internally in the library
            void   setPeptideShowCheckbox  (const int   pepNumber);
            
            //! Returns 0 except for a unigene entry or a PMF mixture entry. 
            int    getPeptideComponentID   (const int   pepNumber) const;
            
            //! Returns the residue immediately before the peptide 
            char   getPeptideResidueBefore (const int   pepNumber) const;

            //! Returns the residue immediately after the peptide 
            char   getPeptideResidueAfter  (const int   pepNumber) const;

            //! Find a protein in the results
            bool isASimilarProtein(const ms_protein       * prot, 
                                   const ms_mascotresults * results,
                                   const bool groupByQueryNumber = false);

            //! Return the accession of a protein that contains the same set (or a subset of) of the peptides.
            /*!
             * See \ref subsetsPage and getGrouping() and ms_protein::GROUP
             */
            std::string getSimilarProteinName() const { return similarProtein_;  }

            //! Don't call from outside the library - Add a peptide from ms_peptidesummary or ms_proteinsummary
            void addOnePeptide(const ms_mascotresults & results,
                               const int frame,
                               const long start, const long end, 
                               const long multiplicity,
                               const int q, const int p,
                               const double correctedScore,
                               const double uncorrectedScore,
                               const char residueBefore,
                               const char residueAfter,
                               const double relativeMass,
                               const ms_protein * component,
                               const bool integratedET);


            //! Return the number of residues covered
            long getCoverage() const;

            //! See if any match to this query
            bool anyMatchToQuery(const int query) const;

            //! See if any match to this query and 'P' (rank / hit)
            bool anyMatchToQueryAndP(const int query, const int P) const;

            //! Return a list of comma separated experimental masses that don't match
            std::string getUnmatchedMasses(ms_mascotresfile & resfile,
                                           const int numDecimalPlaces = 2) const;

            //! Returns the frame number for the protein
            int getFrame() const;

            //! Returns true if any of the peptides in the match were top scoring and not seen before
            bool anyBoldRedPeptides(const ms_mascotresults & results) const;

            //! Returns true if the 'protein' is actually a Unigene entry
            bool isUnigene() const;

            //! Used internally to set the isUnigene() flag
            void setIsUnigeneEntry();

            //! Returns true if the 'protein' is actually a PMF mixture 
            bool isPMFMixture() const;

            //! Used internally to set the isPMFMixture() flag
            void setIsPMFMixture();

            //! Sorts the peptides into ascending query number
            void sortPeptides(const ms_mascotresults & results);

            //! For Unigene and PMF mixture, return number of 'component' proteins.
            int getNumComponents() const;

            //! For Unigene and PMF mixture return the 'component' protein
            const ms_protein * getComponent(const int componentNumber) const;

            //! For a protein from the protein summary _only_
            int getProteinSummaryHit() const;

            //! Return the RMS value of the deltas between the calculated and experimental value
            double getRMSDeltas(const ms_mascotresults & results) const;

            //! Returns the hit number in the results list. 
            int getHitNumber() const;

            //! Used internally to set the hit number. See also getHitNumber()
            void setHitNumber(const int hit) { hitNum_ = hit;}

            //! Return the length (in residues) of the longest peptide in the protein
            int getLongestPeptideLen() const;

        private:
            // For each peptide, we have frame, start, end multiplicity
            // and we want to just have a reference to the peptide
            // structures - using query and 'p', where p is 1..10
            typedef struct
            {
                double      ionsScore;
                double      uncorScore;
                double      mr;
                long        start;
                long        end;
                long        multiplicity;
                int         query;
                int         p;
                int         frame;
                int         componentID;
                DUPLICATE   duplicate;
                short       dupeStatus;
                bool        bold;
                bool        checkBox;
                bool        integratedET;
                DUPLICATE   nonETduplicate;
                short       nonETdupeStatus;
                char        residueBefore;
                char        residueAfter;
            } PEPINFO;


            std::vector<PEPINFO *> peptides_; // sort by query
            std::vector<PEPINFO> allPeptides_;

            typedef std::set<PEPINFO *, ms_pepinfoSortByScore> mapSortedByScore;
            mapSortedByScore * sortByScorePeptides_;

            std::string accession_;
            double score_;
            double nonMudPITScore_;
            GROUP group_;
            std::string similarProtein_; // Accession of the protein that has same peptides (or superset of peptides)

            // For protein summary, the protein score is calculated by
            // nph-mascot.exe, and is in the results file. For the
            // peptide summary, the score is calculated by adding the ions
            // scores
            bool updateScoreFromPepScores_;  

            // For unigene, we need to get the description line from the unigene file
            bool unigene_;

            // flag for if protein actually originates from a PMF mixture
            bool pmfMixture_;

            // For unignene and PMF mixture, the protein is really a 'pseudo'
            // protein, made up from a number of 'real' proteins
            typedef std::vector<const ms_protein *> componentVect;
            componentVect components_;

            int proteinSummaryHit_;

            int hitNum_;

            // Sorting the list of peptides is expensive - don't repeat...
            bool sorted_;

            int longestPeptideLen_;  // Useful with minPepLenInPepSummary

            friend class prot_sort;
            friend class ms_pepinfoSortByScore;
    };
#ifndef SWIG
    class prot_sort
    {
    public:
        static bool smallerQuery(const ms_protein::PEPINFO * p1, const ms_protein::PEPINFO * p2);
    };


    class ms_pepinfoSortByScore
    {
    public:
        ms_pepinfoSortByScore(bool removeDiffPos = false) { removeDiffPos_ = removeDiffPos; }
        bool operator() (const ms_protein::PEPINFO * p1, const ms_protein::PEPINFO * p2) const;

    private:
        bool removeDiffPos_;

    };

#endif
    /** @} */ // end of resfile_group
}   // matrix_science namespace

#endif // MS_MASCOTRESPROTEIN_HPP

/*------------------------------- End of File -------------------------------*/




