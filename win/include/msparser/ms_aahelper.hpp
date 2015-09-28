/*
##############################################################################
# File: ms_aahelper.hpp                                                      #
# Mascot Parser toolkit                                                      #
# Utility functions for generating peptides, peptide and fragment masses     #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Source: /export/cvsroot/parser/inc/ms_aahelper.hpp,v $
#    $Author: yuryr $ 
#      $Date: 2006-02-03 17:02:05 $ 
#  $Revision: 1.19 $
##############################################################################
*/

#ifndef MS_AAHELPER_HPP
#define MS_AAHELPER_HPP

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
#include <string>
#include <vector>


namespace matrix_science {
    /** @addtogroup tools_group
     *  
     *  @{
     */


    //! The class facilitates creation of peptides and calculation of MS-fragment masses
    class MS_MASCOTRESFILE_API ms_aahelper: public ms_errors
    {
    public:
        //! Default constructor
        ms_aahelper();

        //! Copying constructor
        ms_aahelper(const ms_aahelper& right);

        //! Destructor
        ~ms_aahelper();

        //! Assignment operator
        ms_aahelper& operator=(const ms_aahelper& right);

        //! Copies all data from another instance of the class
        void copyFrom(const ms_aahelper* right);

        //! Initialises the instance
        void defaultValues();

        //! Sets an enzyme to use
        void setEnzyme(const ms_enzyme* enzyme);

        //! Returns a pointer to the current enzyme
        const ms_enzyme* getEnzyme() const;

        //! Copies all residue and chemical element masses from the mass-file instance
        void setMasses(const ms_masses *masses);

        //! Returns a pointer to the internal copy of mass-file
        const ms_masses* getMasses() const;

        //! Applies fixed modifications to residue masses and stores variable modifications for later use
        void setAvailableModifications(const ms_modvector *fixedModVector, 
                                       const ms_modvector *varModVector);

        //! Returns a list of fixed modifications that have been successfully applied i.e. don't have conflicts
        ms_modvector getFixedMods() const;

        //! Returns a list of variable modifications that have no conflicts with fixed modifications
        ms_modvector getVarMods() const;

        //! Initialises internal members in preparation for generating peptides
        void startIteratePeptides(const char* sequenceProtein,
                                  const int sizeProtein,
                                  const int pfa);

        //! Has to be called each time before to get a new peptide until it returns false
        bool getNextPeptide();

        //! Returns the first position of the current peptide
        int getPepStart() const;

        //! Returns the last position of the current peptide
        int getPepEnd() const;

        //! Returns number of missed cleavage sites for the current peptide
        int getMissed() const;

        //! Has to be called after when no more peptides needed in order to prevent from giving more peptides
        void stopIteratePeptides();

        //! Calculates and returns a peptide m/z (or Mr) value without specifying exact positions of modified residues
        double calcPeptideMZ(const char* proteinStr,
                             const int proteinLen,
                             const int startPos,
                             const int endPos,
                             const std::vector< int > numThatMustBeModded, 
                             const int charge, 
                             const MASS_TYPE massType,
                             ms_errs *err) const;

        //! Creates and returns a peptide object
        ms_peptide createPeptide(const char* proteinStr,
                                 const int proteinLen,
                                 const int startPos,
                                 const int endPos,
                                 const std::vector< int > modded, 
                                 const std::vector< int > whichNl,
                                 const int charge,
                                 const MASS_TYPE massType,
                                 ms_errs *err) const;

        //! Returns a vector of ion-masses for the given series and peptide
        std::vector< double > calcFragments(const ms_peptide* peptide, 
                                            const int seriesID, 
                                            const bool doubleCharged,
                                            const double minMass, 
                                            const double maxMass,
                                            const MASS_TYPE massType,
                                            ms_fragmentvector* fragments,
                                            ms_errs* err) const;

        //! Returns a vector of ion-masses for the given series and peptide
        bool calcFragmentsEx(const ms_peptide* peptide, 
                             const int seriesID, 
                             const int fragCharge,
                             const double minMass, 
                             const double maxMass,
                             const MASS_TYPE massType,
                             ms_fragmentvector* fragments,
                             ms_errs* err) const;

#ifndef SWIG
        //! Returns a vector of ion-masses for the given series and peptide
        std::vector< double > calcFragments(const ms_peptide* peptide, 
                                            const int seriesID, 
                                            const bool doubleCharged,
                                            const double minMass, 
                                            const double maxMass,
                                            const MASS_TYPE massType,
                                            std::vector< ms_fragment > *fragments,
                                            ms_errs* err) const;

        //! Returns a vector of ion-masses for the given series and peptide
        bool calcFragmentsEx(const ms_peptide* peptide, 
                             const int seriesID, 
                             const int fragCharge,
                             const double minMass, 
                             const double maxMass,
                             const MASS_TYPE massType,
                             std::vector< ms_fragment > *fragments,
                             ms_errs* err) const;
#endif

                             
        std::string convertPrimaryNlVectorToString(const std::vector< int > modded,
                                                   const std::vector< int > whichNl, 
                                                   ms_errs* err) const;

        std::vector< int > convertPrimaryNlStringToVector(const std::string varModStr,
                                                          const std::string primaryNlStr, 
                                                          ms_errs* err) const;

    private:
        bool iscleavagepoint(const char left, const char right) const;
        bool switchIndependent();
        void updatemissed(const int leftPos);
        void dropmissed();
        void clearMods();
        bool cfr(const ms_peptide* peptide, 
                const int seriesID, 
                const int fragCharge,
                const double minMass, 
                const double maxMass,
                const MASS_TYPE massType,
                std::vector< ms_fragment > *fragments,
                ms_errs* err,
                std::vector< double > *vi) const;

        void getSeriesParams(const char* pepStr,
                             const int seriesID,
                             const MASS_TYPE massType,
                             double& delta,
                             int& start,
                             int& end,
                             bool& forward) const;

        ms_masses   massesUnmod_;
        ms_masses   masses_;

        ms_modvector fixedMods_;
        ms_modvector varMods_;
        std::vector< int > varModsSrc_;
        ms_modvector varMods_used;

        ms_enzyme   enzyme_;
        bool        bEnzymeSet_;
        std::string protStr_;
        int         protLen_;
        int         pfa_; // missed cleavages limit

        // internal iteration parameters
        int         numInd_; // current independent enzyme ID
        int         l_, r_; // the last peptide ends
        int         missed_; // number of missed cleavages in the current peptide
        int         enzymesPassed_;

        // for the sake of semi-specific enzymes
        bool        forward_;
    }; // ms_aahelper
    /** @} */ // end of tools_group
} // matrix_science

#endif // MS_AAHELPER_HPP

/*------------------------------- End of File -------------------------------*/
