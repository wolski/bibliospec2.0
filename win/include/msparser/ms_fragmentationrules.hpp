/*
##############################################################################
# file: ms_fragmentationrules.hpp                                            #
# 'msparser' toolkit                                                         #
# Encapsulates a fragmentation_rules file (mascot configuration files)       #
#                                                                            #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
# @(#)$Source: /export/cvsroot/parser/inc/ms_fragmentationrules.hpp,v $
# @(#)$Revision: 1.18 $
# @(#)$Date: 2007-03-08 12:11:39 $ 
# @(#)$Author: davidc $ 
##############################################################################
*/

#ifndef MS_FRAGMENTATIONRULES_HPP
#define MS_FRAGMENTATIONRULES_HPP

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
#include <vector>
#include <string>


namespace matrix_science {
    /** @addtogroup config_group
     *  
     *  @{
     */

    //! This class encapsulates a single entry (an instrument) from a "fragmentation_rules" file
    /*!
     * <p>Objects of this class are normally created by loading a file using the
     * ms_fragrulesfile class.</p>
     */
    class MS_MASCOTRESFILE_API ms_fragmentationrules : public ms_customproperty
    {
        friend class ms_fragrulesfile;
    public:

        //! Definitions for ID-constants for series used
        /*!
        * To access from perl, use, for example: <P>
        *  <tt>$x = $msparser::ms_fragmentationrules::FRAG_A_SERIES</tt>
        *  </P><BR>
        * See http://www.matrixscience.com/help/fragmentation_help.html for 
        * details of ions types
        */
        enum  FRAG_SERIES_TYPE
        {
            FRAG_IMMONIUM        = 4,  //!< Immonium ions
            FRAG_A_SERIES        = 5,  //!< A series ions
            FRAG_A_MINUS_NH3     = 6,  //!< Referred to as a* in Mascot reports
            FRAG_A_MINUS_H2O     = 7,  //!< Referred to as a° in Mascot reports
            FRAG_B_SERIES        = 8,  //!< B series ions
            FRAG_B_MINUS_NH3     = 9,  //!< Referred to as b* in Mascot reports
            FRAG_B_MINUS_H2O     = 10, //!< Referred to as a° in Mascot reports
            FRAG_C_SERIES        = 11, //!< C series ions
            FRAG_X_SERIES        = 12, //!< X series ions
            FRAG_Y_SERIES        = 13, //!< Y series ions
            FRAG_Y_MINUS_NH3     = 14, //!< Referred to as y* in Mascot reports
            FRAG_Y_MINUS_H2O     = 15, //!< Referred to as y° in Mascot reports
            FRAG_Z_SERIES        = 16, //!< Z series ions
            FRAG_INTERNAL_YB     = 17, //!< Internal series, caused by double backbone cleavage. Combination of b type and y type cleavage 
            FRAG_INTERNAL_YA     = 18, //!< Internal series, caused by double backbone cleavage. Combination of a type and y type cleavage 
            FRAG_Z_PLUS_1        = 21, //!< Z+1 series
            FRAG_D_SERIES        = 22, //!< D Satellite Ions. A series partial side chain. Collision induced dissociation of ions at keV energies.
            FRAG_V_SERIES        = 23, //!< V Satellite Ions. Y series partial side chain. Collision induced dissociation of ions at keV energies.
            FRAG_W_SERIES        = 24, //!< W Satellite Ions. Z series partial side chain. Collision induced dissociation of ions at keV energies.
            FRAG_Z_PLUS_2        = 25  //!< Z+2 series
        };

        //! Default constructor is used to create an empty object
        ms_fragmentationrules();

        //! Copying constructor
        ms_fragmentationrules(const ms_fragmentationrules& src);

        //! Destructor
        virtual ~ms_fragmentationrules();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Copies all information from another instance of the class
        void copyFrom(const ms_fragmentationrules* right);

        //! Assignment operator
        ms_fragmentationrules& operator=(const ms_fragmentationrules& right);

        //! Returns a title of the instrument
        std::string getTitle() const;

        //! Assigns a new title to the rule
        void setTitle(const char * title);

        //! The lowest series number that can be used
        static int getFirstSeries();

        //! The lowest series number that can be used
        static int getFirstSerie();

        //! The highest series number that can be used
        static int getLastSeries();

        //! The highest series number that can be used
        static int getLastSerie();

        //! Returns <b>true</b> if the 'singly charged' fragmentation rule is specified
        bool isSinglyCharged() const;

        //! Allows to change availability of 'singly charged' fragmentation rule
        void setSinglyCharged(const bool value);

        //! Returns <b>true</b> if the 'doubly charged 2+' fragmentation rule is specified
        bool isCharged2Plus() const;

        //! Allows to change availability of 'doubly charged 2+' fragmentation rule
        void setCharged2Plus(const bool value);

        //! Returns <b>true</b> if the 'doubly charged 3+' fragmentation rule is specified
        bool isCharged3Plus() const;

        //! Allows to change availability of 'doubly charged 3+' fragmentation rule
        void setCharged3Plus(const bool value);

        //! Returns true if a given serie is specified for the instrument
        bool isSeriesUsed(const int series) const;

        //! Deletes all series usage information
        void clearSeries();

        //! Sets availability on a given series
        void setSeriesUsed(const int series, const bool val);

        //! Use this method to do a basic check on a newly created object
        bool isValid() const;

        //! Tries to detect any inconsistencies in the instrument definition
        bool verifyRule(ms_errs* err_, const bool tryToFix);

        //! Returns a description string for a given series
        static std::string getSeriesDescription(const int series);

        //! Returns the minimum mass to be considered for internal fragements
        double getMinInternalMass() const;

        //! Set the minimum mass to be considered for internal fragements
        void setMinInternalMass(const double mass);

        //! Returns the maximum mass to be considered for internal fragements
        double getMaxInternalMass() const;

        //! Sets the maximum mass to be considered for internal fragements
        void setMaxInternalMass(const double mass);

        //! Returns a short series name
        static std::string getSeriesName(const int series);

        //! Returns the length of the series string output in a Mascot results file
        static int getTotalSeriesOutput();

    private:

        

        std::string title_;
        bool* which_series_;
        double minInternalMass_;
        double maxInternalMass_;
    }; // class ms_fragmentationrules

    //! This class encapsulates a complete "fragmentation_rules" file
    /*!
     *  Usage of this class is simple. Just create an instance, set an explicit file name
     *  if necessary, call #read_file()-member, check for errors (#isValid()) and retrieve
     *  an instrument by its name or number.
     */
    class MS_MASCOTRESFILE_API ms_fragrulesfile: public ms_errors
    {
    public:
        //! Default constructor
        ms_fragrulesfile();
        
        //! Immediate-action constructor that reads the given file
        ms_fragrulesfile(const char* filename, const matrix_science::ms_connection_settings * cs = 0);

        //! Copying constructor
        ms_fragrulesfile(const ms_fragrulesfile& src);

        //! Destructor
        virtual ~ms_fragrulesfile();

        //! Re-initialises the list of fragmentation rules, so the instance can be re-used
        void defaultValues();

        //! Can be used to make a copy of another instance
        void copyFrom(const ms_fragrulesfile* right);

        //! C++ style assignment operator
        ms_fragrulesfile& operator=(const ms_fragrulesfile& right);
        
        //! Use this member to set a non-default file name
        void setFileName(const char* filename);

        //! Get the currently used file name
        std::string getFileName() const;

        //! Sets the sessionID and proxy server for use with an http transfer
        void setConnectionSettings(const matrix_science::ms_connection_settings & cs);

        //! Returns the sessionID and proxy server for use with an http transfer
        matrix_science::ms_connection_settings getConnectionSettings() const;

        //! Reads and parses the file, populates the list of instruments
        void read_file();

        //! Stores instrument definitions in a file
        void save_file();

        //! Returns a number of instruments configured
        int  getNumberOfInstruments() const;

        //! Deletes all instruments definitions
        void clearInstruments();

        //! Adds a new instrument definition at the end of the list
        void appendInstrument(const ms_fragmentationrules* item);

        //! Returns an instrument name by its number from 0 to (#getNumberOfRules()-1)
        std::string getInstrumentName(const int instrumentNum) const;

        //! Returns a configuration for an instrument by name
        const ms_fragmentationrules* getInstrumentByName(const char * name) const;

        //! Returns a configuration for an instrument by its number from 0 to (#getNumberOfRules()-1)
        const ms_fragmentationrules* getInstrumentByNumber(const int instrumentNum) const;

        //! Update the information for a specific instrument
        bool updateInstrumentByNumber(const int num, const ms_fragmentationrules item);

        //! Update the information for a specific instrument
        bool updateInstrumentByName(const char* name, const ms_fragmentationrules item);

        //! Remove an instrument from the list in memory
        bool deleteInstrumentByNumber(const int num);

        //! Remove an instrument from the list in memory
        bool deleteInstrumentByName(const char* name);


    private:
        std::string filename_;
        std::vector< ms_fragmentationrules* > rules_;
        std::vector< std::string > comments_;
        ms_connection_settings cs_;
    }; // ms_fragrulesfile
    /** @} */ // end of config_group
}   // matrix_science namespace

#endif // MS_FRAGMENTATIONRULES_HPP

/*------------------------------- End of File -------------------------------*/

