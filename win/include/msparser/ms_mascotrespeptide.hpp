/*
##############################################################################
# file: ms_mascotrespeptide.hpp                                              #
# 'msparser' toolkit                                                         #
# Encapsulates a peptide from the summary section or peptides section of the #
# mascot results file                                                        #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2002 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /MowseBranches/ms_mascotresfile_1.2/include/ms_mascotrespe $ #
#     $Author: davidc $ #
#       $Date: 2007-11-08 15:01:28 $ #
#   $Revision: 1.19 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESPEPTIDE_HPP
#define MS_MASCOTRESPEPTIDE_HPP

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
#include <vector>

namespace matrix_science {
    class ms_protein;

    /** @addtogroup resfile_group
     *  
     *  @{
     */

    //! This class encapsulates a peptide from the mascot results file.
    /*!
     * This class is used for protein summary and peptide summary results. 
     * There is generally no need to create an object of this class - simply 
     * read the results from it. See ms_peptidesummary::getPeptide() and
     * ms_proteinsummary::getPeptide()
     *
     */
    class MS_MASCOTRESFILE_API ms_peptide
    {
        public:
            //! Default constructor
            ms_peptide();

            //! Constructor to initialise all values
            ms_peptide( int         query,
                        int         rank,
                        int         missedCleavages,
                        double      mrCalc,
                        double      delta,
                        double      observed,
                        int         charge,
                        int         numIonsMatched,
                        std::string peptideStr,
                        int         peaksUsedFromIons1,
                        std::string varModsStr,
                        double      ionsScore,
                        std::string seriesUsedStr,
                        int         peaksUsedFromIons2,
                        int         peaksUsedFromIons3);

            //! Copying constructor
            ms_peptide(const ms_peptide& src);

            //! Destructor
            ~ms_peptide();

            //! C++ assignment operator
            ms_peptide& operator=(const ms_peptide& right);

            //! Copies all content from another instance of the class
            void copyFrom(const ms_peptide* src);

            //! Each peptide is associate with a query. 
            /*! 
             * \return The query number in the range 1..ms_mascotresfile::getNumQueries()
             */
            int         getQuery()              const { return query_;                }

            //! Return the 'rank' of the peptide match.
            /*! Each spectrum will match to a number of different peptides.
             *  The best (highest scoring) match will have a rank value of 1
             *  In a peptide summary, this will be in the range 1..10. 
             *  For a protein summary, this will be the hit number (1..50).
             *  \sa ms_mascotresults::getMaxRankValue())
             * \return The rank
             */
            int         getRank()               const { return rank_;                 }

            void        setRank(int rank)             { rank_ = rank;                 }
            
            //! Similar to getRank() except that equivalent scores get the same rank.
            int         getPrettyRank()         const;

            //! Should only be used from within the library
            /*!
             * See also #getPrettyRank()
             *  \param rank Is the the new pretty rank
             */
            void        setPrettyRank(int rank)       { prettyRank_ = rank;           }

            //! Returns true if there was a peptide match to this spectrum. 
            /*!
             * Internally (and in the results file) this is signified
             * by the first value on the line, which is the number of
             * missed cleavages. See also #getMissedCleavages()
             * \return True if there was a peptide match to this spectrum
             */
            bool        getAnyMatch()           const { return missedCleavages_ != -1;}

            //! Returns the number of missed cleavages. 
            /*!
             * See also #getAnyMatch()
             * \return Will be -1 if there was no match. 
             * Will always be 0 for a no enzyme search
             */
            int         getMissedCleavages()    const { return missedCleavages_;      }

            //! Returns the calculated relative mass for this peptide 
            /*!
             * Takes into account any modifications
             * \return The relative mass.
             */
            double      getMrCalc()             const { return mrCalc_;               }

            //! Returns the difference between the calculated and experimental relative masses.
            /*!
             * \return The delta in daltons
             */
            double      getDelta()              const { return delta_;                }

            //! Returns the observed mass / charge value. 
            /*!
             *
             * \return The observed mass / charge value. 
             */
            double      getObserved()           const { return observed_;             }

            //! Returns the observed mz value as a relative mass. 
            /*!
             * This is equal to getMrCalc() + getDelta(), so note that this
             * will be zero if there was no match because there is no calculated 
             * value and no delta. It is generally recommended
             * that you call ms_mascotresfile::getObservedMrValue() since this
             * will always return the relative mass, even for no match.
             * \return The observed mz value as a relative mass
             */
            double      getMrExperimental()     const { return mrExp_;                }

            //!Returns the charge state for the parent mass. 
            /*!
             * This will be 0 for an Mr value, or 1,2,3,4 etc. If an error 
             * has occurred, then the charge will be -1
             * \return The charge state for the parent mass
             */
            int         getCharge()             const { return charge_;               }

            //! Returns the number of ions matched.
            /*!
             * Mascot begins by selecting a small number of experimental peaks 
             * on the basis of normalised intensity. It calculates a probability 
             * based score according to the number of matches. It then increases 
             * the number of selected peaks and recalculates the score. It 
             * continues to iterate until it is clear that the score can only get 
             * worse. It then reports the best score it found, which should 
             * correspond to an optimum selection, taking mostly real peaks and 
             * leaving behind mostly noise <br>
             * Mascot is not trying to find all possible matches in the spectrum. 
             * Many spectra have "peak at every mass" noise, and can match any 
             * ion series from any sequence if there is no intensity discrimination. 
             * So, you may look at a peptide view report and see obvious matches 
             * that are unlabelled. However, if the peak selection was to be 
             * extended to include these additional matches, it would also have to 
             * include a number of additional noise peaks, and the score 
             * would decrease.
             * \return The number of ions matched in order to get the highest score
             */
            int         getNumIonsMatched()     const { return numIonsMatched_;       }

            //! Returns the sequence found for the peptide.
            std::string getPeptideStr(bool substituteAmbiguous = true) const;
            
            //! Returns number of peaks used from ions1.
            /*!
             * It is possible, but unusual, to specify which ions series
             * particular ions come from:
             * \sa http://www.matrixscience.com/help/sq_help.html#IONS
             * If a search specifies that some ions are from the b series,
             * some are from the y series and that some are from any series
             * then these will be stored separately in Ions1, Ions2 and Ions3.
             * The number of matches to each set of ions is avaible using
             * getPeaksUsedFromIons1(), getPeaksUsedFromIons2() and
             * getPeaksUsedFromIons3(). <br>
             * For most searches, getPeaksUsedFromIons1() is the only
             * function that needs to be used.
             * See also #getNumIonsMatched()
             * \return The number of peaks used from ions1
             */
            int         getPeaksUsedFromIons1() const { return peaksUsedFromIons1_;   }

            //! Returns number of peaks used from ions2.
            /*!
             * It is possible, but unusual, to specify which ions series
             * particular ions come from:
             * \sa http://www.matrixscience.com/help/sq_help.html#IONS
             * If a search specifies that some ions are from the b series,
             * some are from the y series and that some are from any series
             * then these will be stored separately in Ions1, Ions2 and Ions3.
             * The number of matches to each set of ions is avaible using
             * getPeaksUsedFromIons1(), getPeaksUsedFromIons2() and
             * getPeaksUsedFromIons3(). <br>
             * For most searches, getPeaksUsedFromIons1() is the only
             * function that needs to be used.
             * See also #getNumIonsMatched()
             * \return The number of peaks used from ions2
             */
            int         getPeaksUsedFromIons2() const { return peaksUsedFromIons2_;   }

            //! Returns number of peaks used from ions3.
            /*!
             * It is possible, but unusual, to specify which ions series
             * particular ions come from:
             * \sa http://www.matrixscience.com/help/sq_help.html#IONS
             * If a search specifies that some ions are from the b series,
             * some are from the y series and that some are from any series
             * then these will be stored separately in Ions1, Ions2 and Ions3.
             * The number of matches to each set of ions is avaible using
             * getPeaksUsedFromIons1(), getPeaksUsedFromIons2() and
             * getPeaksUsedFromIons3(). <br>
             * For most searches, getPeaksUsedFromIons1() is the only
             * function that needs to be used.
             * See also #getNumIonsMatched()
             * \return The number of peaks used from ions3
             */
            int         getPeaksUsedFromIons3() const { return peaksUsedFromIons3_;   }

            //! Variable modifications as a string of digits.
            /*!
             * One digit is used for the N terminus, one for each residue 
             * and one for the C terminus. Each digit specifies the modification 
             * used to obtain the match: 0 indicates no modification, 
             * 1 indicates delta1, 2 indicates delta2 etc., in the 
             * masses section. An 'X' is used to indicate an error tolerant 
             * modification that can be retrieved using 
             * matrix_science::ms_peptidesummary::getErrTolModName. <br>
             * To support numbers greater than 9, the letters A..W are 
             * permitted, with A being 10 and W being 32.<br>
             * See also \ref BackwardCompatibility_9varmods. <br>
             * See also 
             * ms_mascotresults::getReadableVarMods()
             * \return Variable modifications as a string of digits
             */
            std::string getVarModsStr()         const { return varModsStr_;           }

            //! Variable modifications as a string of digits.
            /*!
             * One digit is used for the N terminus, one for each residue 
             * and one for the C terminus. Each digit specifies the modification 
             * used to obtain the match: 0 indicates no modification, 
             * 1 indicates delta1, 2 indicates delta2 etc., in the 
             * masses section. An 'X' is used to indicate an error tolerant 
             * modification that can be retrieved using 
             * matrix_science::ms_peptidesummary::getErrTolModName. <br>
             * To support numbers greater than 9, the letters A..W are 
             * permitted, with A being 10 and W being 32.<br>
             * See also \ref BackwardCompatibility_9varmods. <br>
             * See also 
             * ms_mascotresults::getReadableVarMods() <br>
             * Setting the string does not cause a re-calculation of the 
             * peptide mass - this needs to be done manually.
             * \param str is the new variable modifications as a string of digits
             */
            void setVarModsStr(const std::string str) { varModsStr_ = str; }


            //! Returns the ions score.
            /*!
             * Note that ms_protein::getPeptideIonsScore() returns the ions
             * score in the context of the <I>protein</I> match and will 
             * generally be slightly lower than the return value from this
             * function. The Mascot results pages display the score returned
             * from this function because results from similar proteins are
             * displayed together.
             * \return The ions score
             */
            double      getIonsScore()          const { return ionsScore_;            }

            //! Returns the series used as a string.
            /*! 
             * The string is a set of 0s and 1s and 2s. \par
             * \li 0 - no significant match 
             * \li 1 - significant match, but not used in score
             * \li 2 - significant match, used for scoring
             *
             * The 'position' in the string is used to indicate which series: \par
             * \li 01 - A series
             * \li 02 - placeholder (was A - NH3 series in older versions of Mascot)
             * \li 03 - A++ series
             * \li 04 - B series
             * \li 05 - placeholder (was B - NH3 series in older versions of Mascot)
             * \li 06 - B++ series
             * \li 07 - Y series
             * \li 08 - placeholder (was Y - NH3 series in older versions of Mascot)
             * \li 09 - Y++ series
             * \li 10 - C series
             * \li 11 - C++ series
             * \li 12 - X series
             * \li 13 - X++ series
             * \li 14 - Z series
             * \li 15 - Z++ series
             * \li 16 - Z+H series
             * \li 17 - Z+H++ series
             *
             * so, for example: 00020010000000000 would indicate b series used
             * for scoring, and y series significant, but not used for scoring.
             * Note that earlier versions of Mascot did not look for all these
             * ions series, so the string will not necessarily be 17 characters
             * long. 
             */
            std::string getSeriesUsedStr()      const { return seriesUsedStr_;        }

            //! returns true if the two peptides are identical
            bool        isSamePeptideStr(ms_peptide * peptide, 
                                         bool substituteAmbiguous = true) const;

            //! returns true if the two variable modifications are identical
            bool        isSameVarModsStr(ms_peptide * peptide) const;

            //! This function used internally in the library
            void        setFirstProtAppearedIn(int n) { firstProtAppearedIn_ = n;     }

            //! Returns the hit numberof the first protein that contains this peptide
            /*!
             * \return The hit number (1..n) 
             */
            int         getFirstProtAppearedIn() const{ return firstProtAppearedIn_;  }

            //! This function is used internally in the library
            /*!
             * \param n Is the new intensity. See getIonsIntensity() for more details
             */
            void        setIonsIntensity(double n)    { ionsIntensity_       = n;     }

            //! Returns the total intensity of all of the ions in the spectrum
            /*!
             * This value is only valid for ms-peptidesummary when sorting unnassigned by 
             * intensity. It is the same value as would be returned by
             * ms_inputquery::getTotalIonsIntensity()
             * \return The total intensity of all of the ions in the spectrum
             */
            double      getIonsIntensity() const      { return ionsIntensity_;        }

            //! Returns a pointer to a protein that contains this peptide
            const ms_protein * getProtein(int num) const;

            //! Returns the number of proteins that contains this peptide
            int   getNumProteins() const;

            //! Called internally
            bool setAmbiguityString(const std::string val);

            //! Used for X, B and Z residues in source databases where Mascot then substitutes for a residue
            std::string getAmbiguityString() const;

            //! This function used internally in the library
            void addProtein(const ms_protein * protein);

            //! Called internally
            void        setPrimaryNlStr(const std::string value);

            //! Returns neutral loss information associated with any modification for the peptide
            std::string getPrimaryNlStr() const;

            //! Returns true if this peptide came from the error tolerant search.
            bool getIsFromErrorTolerant() const;

            // Called internally
            void setIsFromErrorTolerant(const bool isFromErrorTolerant);

            //! Called internally
            void        setComponentStr(const std::string value);

            //! Returns quantitation method component name used for the peptide match
            std::string getComponentStr() const;

            //! Returns the number of 13C peaks offset required to get a match with the supplied tolerance
            int getNum13C(const double tol, const std::string tolu, const std::string mass_type) const;

        private:
            int         query_;
            int         rank_;
            int         prettyRank_;
            int         missedCleavages_;
            double      mrCalc_;
            double      delta_;
            double      observed_;
            double      mrExp_;
            int         charge_;
            int         numIonsMatched_;
            std::string peptideStr_;
            int         peaksUsedFromIons1_;
            std::string varModsStr_;
            double      ionsScore_;
            std::string seriesUsedStr_;
            int         peaksUsedFromIons2_;
            int         peaksUsedFromIons3_;
            int         firstProtAppearedIn_;
            double      ionsIntensity_;
            std::vector <const ms_protein *> proteins_;
            bool        isFromErrorTolerant_;
            char      * primaryNlStr_;
            char      * componentStr_;
            char      * ambiguousStr_;    // The subst string
            char      * unambiguousStr_;  // The string with all ambiguous residues substituted

            std::string getUnambiguousStr(const std::string val) const;

    };
    /** @} */ // end of resfile_group
}   // matrix_science namespace

#endif // MS_MASCOTRESPEPTIDE_HPP

/*------------------------------- End of File -------------------------------*/
