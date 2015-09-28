/*
##############################################################################
# file: ms_databaseoptions.hpp                                               #
# 'msparser' toolkit                                                         #
# Encapsulates "mascot.dat"-file that describes most important parameters    #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_databaseoptions.hpp     $ #
#     $Author: yuryr $ #
#       $Date: 2007-03-06 12:57:30 $ #
#   $Revision: 1.9 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_DATABASEOPTIONS_HPP
#define MS_DATABASEOPTIONS_HPP

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
    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Contains all possible database types
    /*!
     * The following are currently supported. 
     * To access from perl, use, for example: <P><tt>
     * $x = $msparser::AA
     * </tt></P>
     */
    enum DATABASE_TYPE
    {
        AA=0, //!< Amino acid database
        NA=1  //!< Nucleic acid database
    };

    //! An instance of this class represents a single database definition from <tt>Databases</tt>-section of <tt>mascot.dat</tt> file
    /*!
     *  An instance of this class can be normally created within ms_datfile class. 
     *  Before using a definition you must check whether it is active or not (see #isActive()).
     *  Inactive database definition is allowed to have invalid properties and, therefore,
     *  must be ignored for database searches/maintenance. An inactive definition can be made active
     *  with #setActive() method. Before doing this make sure all the properties are valid.<BR>
     *  One or more instances of this class are contained in the list of databases in ms_databases class.
     */
    class MS_MASCOTRESFILE_API ms_databaseoptions
    {
        friend class ms_datfile;
        friend class ms_databases;
    public:

        //! Default constructor
        ms_databaseoptions();

        //! Copying constructor
        ms_databaseoptions(const ms_databaseoptions& src);

        //! Destructor
        virtual ~ms_databaseoptions();

        //! Initialises the instance with default values
        void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_databaseoptions* right);

        //! Assignment operator for C++ client applications
        ms_databaseoptions& operator=(const ms_databaseoptions& right);

        //! Returns <b>TRUE</b> if the database definition is active and <b>FALSE</b> otherwise
        /*!
         *  Inactive definitions represent the entries that can be made active by
         *  the database maintenance utility. Normally, they are draft definitions with
         *  one or more invalid parameters. They are anyway supported in order to 
         *  allow storing/retrieving definitions from the file. By default, 
         *  the definition is inactive.
         */
        bool isActive() const;

        //! Set the definition as active or inactive
        /*!
         *  See #isActive() for more information.
         */
        void setActive(const bool value);

        //! Returns a value of <b>Name</b> parameter
        /*!
         *  Each database must have a unique case-sesitive name. 
         *  The name does not need to be the same as or even 
         *  similar to the filename of the actual <tt>FASTA</tt> file. 
         *  Do not use spaces, punctuation, or control characters in names.
         *  By default it is empty.
         */
        std::string getName() const;

        //! Allows to change <b>Name</b> parameter
        /*!
         *  See #getName() for more information.
         */
        void setName(const char* name);

        //! Returns a value of <b>Path</b> parameter
        /*!
         *  <tt>FASTA</tt> database files must be on a local disk. 
         *  Mascot creates its compressed files in the same directory 
         *  as the original <tt>FASTA</tt> file. 
         *  The location of the <tt>FASTA</tt> file is defined in the 
         *  <b>Path</b> field. This must be the fully qualified path 
         *  to the <tt>FASTA</tt> file, with a wild card in the filename.
         *  The delimiters between directories must always be forward slashes, 
         *  even if Mascot is running on a Windows system.
         *  By default it is empty.
         */
        std::string getPath() const;

        //! Allows to change <b>Path</b> parameter
        /*!
         *  See #getPath() for more information.
         */
        void setPath(const char* path);

        //! Returns one of DATABASE_TYPE enumeration values
        /*!
         *  AA for an amino acid (protein) database and NA for a nucleic acid (DNA) database.
         *  Default value is <b>0</b> (amino acid DB).
         */
        int getDBType() const;

        //! Allows to change database type
        /*!
         *  See #getDBType() for more information.
         */
        void setDBType(const int type);

        //! Returns value of <b>MemMap</b> flag
        /*!
         *  This flag is to indicate whether the database file should 
         *  be memory mapped (<b>1</b>) or not (<b>0</b>). 
         *  Database files should always be memory mapped. 
         *  Unlike memory locking, this does not consume physical RAM.
         *  Default values is <b>1</b>.
         *  \sa #isLockMemory()
         */
        bool isMemoryMapFiles() const;

        //! Allows to change <b>MemMap</b> flag
        /*!
         *  See #isMemoryMapFiles() for more information.
         */
        void setMemoryMapFiles(const bool flag);

        //! Returns value of <b>Threads</b> parameter
        /*!
         *  A Mascot search can use multiple threads. 
         *  If you are running in cluster mode, <b>Threads</b> must be set to <b>1</b>. 
         *  Otherwise, specify the same number of threads as processors in your system.
         *  Default value is <b>1</b>.
         */
        int getNumberOfThreads() const;

        //! Allows to change <b>Threads</b> parameter value
        /*!
         *  See #getNumberOfThreads() for more information.
         */
        void setNumberOfThreads(const int number);

        //! Returns <b>TRUE</b> if <b>MemLock</b> flag is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  Flag to indicate whether a memory mapped database file should be locked 
         *  in memory (<b>1</b>) or not (<b>0</b>). Default value is <b>0</b>.
         *  \sa #isMemoryMapFiles()
         */
        bool isLockMemory() const;

        //! Allows to change <b>MemLock</b> parameter value
        /*!
         *  See #isLockMemory() for more information.
         */
        void setLockMemory(const bool flag);

        //! Returns <b>TRUE</b> if <b>LocalRefFile</b> flag is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  Flag to indicate whether a local reference file is available (<b>1</b>) 
         *  or not (<b>0</b>). For certain databases (MSDB, OWL, and SwissProt), 
         *  it is possible to have a local reference file, 
         *  from which full text information can be taken for a <tt>Protein View</tt>
         *  report. Default value is <b>0</b>.
         */
        bool isLocalRefFile() const;

        //! Allows to change <b>LocalRefFile</b> parameter value
        /*!
         *  See #isLocalRefFile() for more information.
         */
        void setLocalRefFile(const bool flag);

        //! Returns <b>AccessionParseRuleIndex</b> parameter value
        /*!
         *  Index of the regular expression in the <tt>PARSE</tt> section that can be 
         *  used to parse an accession string from a <tt>FASTA</tt> file title line.
         *  Default value is <b>0</b>.
         */
        int getAccessionParseRuleIndex() const;

        //! Allows to change <b>AccessionParseRuleIndex</b> parameter value
        /*!
         *  See #getAccessionParseRuleIndex() for more information.
         */
        void setAccessionParseRuleIndex(const int index);

        //! Returns <b>DescriptionParseRuleIndex</b> parameter value
        /*!
         *  Index of the regular expression in the <tt>PARSE</tt> section that 
         *  can be used to parse a description string from a <tt>FASTA</tt> 
         *  file title line.
         *  Default value is <b>0</b>.
         */
        int getDescriptionParseRuleIndex() const;

        //! Allows to change <b>DescriptionParseRuleIndex</b> parameter value
        /*!
         *  See #getDescriptionParseRuleIndex() for more information.
         */
        void setDescriptionParseRuleIndex(const int index);

        //! Returns <b>AccessionRefParseRuleIndex</b> parameter value
        /*!
         *  Index of the regular expression in the <tt>PARSE</tt> section that 
         *  can be used to parse an accession string from a local full text 
         *  reference file. If there is no local reference file, this value is ignored.
         *  Default value is <b>0</b>.
         */
        int getAccessionRefParseRuleIndex() const;

        //! Allows to change <b>AccessionRefParseRuleIndex</b> parameter value
        /*!
         *  See #getAccessionRefParseRuleIndex() for more information.
         */
        void setAccessionRefParseRuleIndex(const int index);

        //! Returns <b>TaxonomyParseRuleIndex</b> parameter value
        /*!
         *  Index of the taxonomy rule block to be used to parse 
         *  taxonomy information. If taxonomy information is not 
         *  available, or is not to be used, this value should be set to <b>0</b>.
         *  Default value is <b>0</b>.
         */
        int getTaxonomyParseRuleIndex() const;

        //! Allows to change <b>TaxonomyParseRuleIndex</b> parameter value
        /*!
         *  See #getTaxonomyParseRuleIndex() for more information.
         */
        void setTaxonomyParseRuleIndex(const int index);

        // internal usage only
        bool isBlastIndexFiles() const;

        // internal usage only
        void setBlastIndexFiles(const bool flag);

        std::string getStringValue() const;

    private:
        void setCustomString();

        bool        active_;
        std::string name_;
        std::string path_;
        int         type_;
        int         __numEntries_;  // no longer used
        int         __uniqueID_;    // no longer used
        bool        mmapFiles_;
        bool        __useBlastIndexFiles_;    // no longer used - was also blocks
        int         numThreads_;
        bool        lockMemory_;
        bool        hasRefFile_;
        int         accessionParseRule_;
        int         descriptionParseRule_;
        int         accessionRefParseRule_;
        int         taxonomyParseRule_;
        std::string customString_;
    }; // class ms_databaseoptions

    //! An instance of this class represents <tt>Databases</tt>-section of <tt>mascot.dat</tt> file
    /*!
     *  An instance is created and populated with database entries within ms_datfile class.
     *  It contains active database definitions as well as inactive ones. When traversing
     *  the list of database with #getNumberOfDatabases() and #getDatabase() members don't
     *  forget to check each entry for being active. Otherwise one or all database parameters 
     *  might be invalid and, therefore, must be ignored. In text-based configuration file
     *  a database definition can be made inactive by commenting it out.<BR>
     *  For detailed information on any of the options please consult Mascot manual.<BR>
     *  Also get yourselves acquainted with the base class ms_customproperty. 
     *  It facilitates the following tasks:
     *  <ul>
     *  <li>retrieving an unsupported property</li>
     *  <li>retrieving raw/text/xml property representation</li>
     *  <li>checking for existence of a certain property rather than 
     *  dealing with its default value</li>
     *  <li>accessing commented lines in a section</li>
     *  </ul>
     *  More functionality is described in details in the documentation for the class.
     */
    class MS_MASCOTRESFILE_API ms_databases
    {
        friend class ms_datfile;
    public:
        //! Default constructor
        ms_databases();

        //! Copying constructor
        ms_databases(const ms_databases& src);

        //! Destructor
        virtual ~ms_databases();

        //! Initialises the instance with default values
        void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_databases* right);

        //! Assignment operator for C++ client applications
        ms_databases& operator=(const ms_databases& right);

        //! Tells if the section has been actually read form the file or have to be saved
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. whether it should be saved in the file
        void setSectionAvailable(const bool value);

        //! Returns total number of database definitions including inactive (commented out) databases
        /*!
         *  By default the list of database is empty unless explicitly populated.
         */
        int getNumberOfDatabases() const;

        //! Returns a database definition by its number
        /*!
         *  Use this member to retrieve a database definition with index 
         *  from 0 to (#getNumberOfDatabases()-1).
         */
        const ms_databaseoptions* getDatabase(const int index) const;

        //! Deletes all database definitions
        void clearDatabases();

        //! Allows to add one more database definition at the end of the list
        void appendDatabase(const ms_databaseoptions* item);

        //! Finds a database with the specified name and returns its number to access
        int findDatabase(const char* dbName, const bool searchInactive = false) const;

        //! Update the information for a specific database
        bool updateDatabaseByNumber(const int num, const ms_databaseoptions db);

        //! Update the information for a specific database
        bool updateDatabaseByName(const char* name, const ms_databaseoptions db);

        //! Remove a database from the list in memory
        bool deleteDatabaseByNumber(const int num);

        //! Remove a database from the list in memory
        bool deleteDatabaseByName(const char* name);

    private:
        bool sectionAvailable_;
        std::vector< ms_databaseoptions* > dbases_;
        std::vector< std::string > comments_;
    }; // class ms_databases
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_DATABASEOPTIONS_HPP

/*------------------------------- End of File -------------------------------*/
