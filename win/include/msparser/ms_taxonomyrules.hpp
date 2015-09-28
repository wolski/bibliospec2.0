/*
##############################################################################
# file: ms_taxonomyrules.hpp                                                 #
# 'msparser' toolkit                                                         #
# Encapsulates "mascot.dat"-file that describes most important parameters    #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_taxonomyrules.hpp       $ #
#     $Author: davidc $ #
#       $Date: 2007-01-16 13:16:11 $ #
#   $Revision: 1.8 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_TAXONOMYRULES_HPP
#define MS_TAXONOMYRULES_HPP

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

    //! Lists all possible taxonomy species file formats
    /*!
     * Several types of file format are supported
     * To access from perl, use, for example: <P><tt>
     * $x = $msparser::TAX_SPECIES_SWISSPROT
     * </tt></P>
     */
    enum TAX_SPECIES_FORMAT
    {
        TAX_SPECIES_NCBI,       //!< NCBI web-site source for taxonomy species files
        TAX_SPECIES_SWISSPROT,  //!< SwissProt
        TAX_SPECIES_PDB,        //!< PDB
        TAX_SPECIES_GI2TAXID,   //!< GI2TAXID
        TAX_SPECIES_FORMAT_COUNT        /* Always leave this one last */
    };

    //! An instance of this class describes one entry of taxonomy species files
    class MS_MASCOTRESFILE_API ms_taxspeciesfiles
    {
        friend class ms_datfile;
        friend class ms_taxonomyrules;

    public:
        //! Default constructor
        ms_taxspeciesfiles();

        //! Copying constructor
        ms_taxspeciesfiles(const ms_taxspeciesfiles& src);

        //! Destructor
        virtual ~ms_taxspeciesfiles();

        //! Initialises the instance
        void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_taxspeciesfiles* right);

        //! Assignment operator for C++ client applications
        ms_taxspeciesfiles& operator=(const ms_taxspeciesfiles& right);

        //! Returns a format identifier for the entry
        TAX_SPECIES_FORMAT getFormat() const;

        //! Allows to change format for the entry
        void setFormat(const TAX_SPECIES_FORMAT value);

        //! Returns a file name
        std::string getFileName() const;

        //! Allows to change a file name
        void setFileName(const char* name);

    private:
        TAX_SPECIES_FORMAT  format_;
        std::string         filename_;

        std::string getStringValue() const;
    }; // class ms_taxspeciesfiles

    //! Lists possible formats for <tt>nodes.dmp</tt> file
    /*!
     * Currently only one format is supported. To access from perl, 
     * use, for example: <P><tt>
     * $x = $msparser::TAX_NODE_NCBI
     * </tt></P>
     */
    enum TAX_NODE_FORMAT
    {
        TAX_NODE_NCBI //!< NCBI web-site source
    };

    //! Describes one taxonomy nodes file
    class MS_MASCOTRESFILE_API ms_taxnodesfiles
    {
        friend class ms_datfile;
        friend class ms_taxonomyrules;
    public:

        //! Default constructor
        ms_taxnodesfiles();

        //! Copying constructor
        ms_taxnodesfiles(const ms_taxnodesfiles& src);

        //! Destructor
        virtual ~ms_taxnodesfiles();

        //! Initialises the instance
        void defaultValues();

        //! Can be used to create a copy of another instance
        void copyFrom(const ms_taxnodesfiles* right);

        //! Assignment operator for C++ client applications
        ms_taxnodesfiles& operator=(const ms_taxnodesfiles& right);

        //! Returns format identifier of the file
        TAX_NODE_FORMAT getFormat() const;

        //! Allows to change the format property
        void setFormat(const TAX_NODE_FORMAT value);

        //! Returns the file name
        std::string getFileName() const;

        //! Allows to change the file name
        void setFileName(const char* name);

    private:
        TAX_NODE_FORMAT format_;
        std::string     filename_;
        std::string getStringValue() const;
    }; // class ms_taxnodesfiles

    //! Lists some constants used for combining TAX_CHOP_SRC values
    /*!
     * The following are currently supported. 
     * To access from perl, use, for example: <P><tt>
     * $x = $msparser::TAX_CHOP_PREFIX
     * </tt></P>
     */
    enum TAX_CHOP_TYPES
    {
        TAX_CHOP_PREFIX = 0x0001, //!< Remove all words at the start of the text specified in the PrefixRemoves section. See ms_taxonomyrules::getPrefixRemove()
        TAX_CHOP_SUFFIX = 0x0002, //!< Remove all words at the end of the text   specified in the SuffixRemoves section. See ms_taxonomyrules::getSuffixRemove()
        TAX_CHOP_WORDS  = 0x0004  //!< Remove one word at a time from the end of the text and try to get a taxonomy match again.
    };

    //! Data type used for the parameter specifying how to chop a source line
    typedef unsigned int TAX_CHOP_SRC;

    //! Represents regular expression parse rule plus some additional parameters
    class MS_MASCOTRESFILE_API ms_parserule_plus
    {
        friend class ms_datfile;
        friend class ms_taxonomyrules;

    public:
        //! Default constructor
        ms_parserule_plus();

        //! Copying constructor
        ms_parserule_plus(const ms_parserule_plus& src);

        //! Destructor
        virtual ~ms_parserule_plus();

        //! Initialises the instance
        void defaultValues();

        //! Can be used to create a copy of another instance
        void copyFrom(const ms_parserule_plus* right);

        //! Assignment operator for C++ client applications
        ms_parserule_plus& operator=(const ms_parserule_plus& right);

        //! Returns file format
        TAX_SPECIES_FORMAT getFileTypeToSearch() const;

        //! Allows to change file format
        void setFileTypeToSearch(const TAX_SPECIES_FORMAT value);

        //! Returns regular expression-based parse rule
        const ms_parserule* getRule() const;

        //! Can be used to set a new parse rule
        void setRule(const ms_parserule* src);

        //! Returns additional parameter specifying how to chop source line
        TAX_CHOP_SRC getChopSource() const;

        //! Allows to change the parameter specifying how to chop source line
        void setChopSource(const TAX_CHOP_SRC value);

        //! Returns database name
        std::string getNameOfDB() const;

        //! Can be used to change database name
        void setNameOfDB(const char* name);

    private:
        TAX_SPECIES_FORMAT  fileTypeToSearch_;
        ms_parserule        rule_;
        TAX_CHOP_SRC        chopSrc_;
        std::string         nameOfDb_;
        std::string getStringValue() const;
    }; // ms_parserule_plus

    //! The class represents a single <b>Taxonomy_XXX</b> section in <tt>mascot.dat</tt> file
    /*!
     *  Taxonomy section defines a set of taxonomy rules that can be selected for a database.
     *  Usage of taxonomy rules can be turned off by setting <b>Enabled</b> property to <b>0</b>.
     *  See #isEnabled() for more information.
     *  Instances of this class are created in ms_datfile class.<BR>
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
    class MS_MASCOTRESFILE_API ms_taxonomyrules: public ms_customproperty
    {
        friend class ms_datfile;

    public:
        //! Default constructor
        ms_taxonomyrules();

        //! Copying constructor
        ms_taxonomyrules(const ms_taxonomyrules& src);

        //! Destructor
        virtual ~ms_taxonomyrules();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_taxonomyrules* right);

        //! Assignment operator for C++ client applications
        ms_taxonomyrules& operator=(const ms_taxonomyrules& right);

        //! Tells whether the section has been actually read from the file
        /*!
         *  By default a <tt>Taxonomy</tt> section is unavailable until it has 
         *  been set to a different state.
         */
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. should it be saved in a file
        void setSectionAvailable(const bool value);

        //! Returns <b>TRUE</b> if <b>Enabled</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  In order to turn off the taxonomy set <b>Enabled</b> parameter to <b>0</b>.
         *  Most of the other parameters will be ignored although the taxonomy 
         *  will be available for a database to select in database maintenance utility.
         *  Default value is <b>0</b>.
         */
        bool isEnabled() const;

        //! Allows to change <b>Enabled</b> parameter value
        /*!
         *  See #isEnabled() for more information.
         */
        void setEnabled(const bool flag);

        //! Returns value of <b>Identifier</b> parameter
        /*!
         *  This parameter contains a symbolic name for the taxonomy specification
         *  as seen, for instance, in database GUI utility.
         *  By default it is empty.
         */
        std::string getIdentifier() const;

        //! Allows to change <b>Identifier</b> parameter value
        /*!
         *  For more information see #getIdentifier().
         */
        void setIdentifier(const char* str);

        //! Returns value of <b>ErrorLevel</b> parameter
        /*!
         *  <b>ErrorLevel</b> indicates the type of warnings or errors that are found 
         *  when creating the taxonomy information. Possible values and their meanings:
         *  <ul>
         *  <li><b>0</b> - an entry is put into the ‘NoTaxonomyMatch.txt’ file for 
         *  every sequence where no taxonomy information is found</li>
         *  <li><b>1+</b>(severe) - an entry is put into the ‘NoTaxonomyMatch.txt’ file for
         *  every sequence that had any accession string without a match. 
         *  Since some sequences in NCBI will have up to 200 <tt>gi</tt> numbers (sources), 
         *  there is a reasonable chance that some of these entries will 
         *  not have species information, and this would cause the errors 
         *  files to become very large</li>
         *  </ul>
         *  Default value is <b>1</b>.
         */
        int getErrorLevel() const;

        //! Allows to change <b>ErrorLevel</b> parameter value
        /*!
         *  See #getErrorLevel() for more information.
         */
        void setErrorLevel(const int value);

        //! Returns <b>TRUE</b> if <b>FromRefFile</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  FromRefFile is set to <b>0</b> indicates that the taxonomy should be found 
         *  in the <tt>.fasta</tt> file rather than in a reference file.
         *  Default value is <b>0</b>.
         *  \sa #isConcatRefFileLines(), #getDescriptionLineSep().
         */
        bool isFromRefFile() const;

        //! Allows to change <b>FromRefFile</b> parameter value
        /*!
         *  See #isFromRefFile() for more information.
         */
        void setFromRefFile(const bool flag);

        //! Returns <b>TRUE</b> if <b>ConcatRefFileLines</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  Value <b>1</b> (default value) means that there might be multiple lines in reference file 
         *  describing the same entry of the database. 
         *  \sa #isFromRefFile(), #getDescriptionLineSep().
         */
        bool isConcatRefFileLines() const;

        //! Allows to change <b>ConcatRefFileLines</b> parameter value
        /*!
         *  See #isConcatRefFileLines() for more information.
         */
        void setConcatRefFileLines(const bool flag);

        //! Returns value of <b>DescriptionLineSep</b> parameter
        /*!
         *  The line that contains the species IDs has multiple IDs, 
         *  separated by a character, whose ASCII code is 
         *  specified by <b>DescriptionLineSep</b>.
         *  There is no default value for this parameter.
         */
        char getDescriptionLineSep() const;

        //! Allows to change <b>DescriptionLineSep</b> parameter value
        /*!
         *  See #getDescriptionLineSep() for more information.
         */
        void setDescriptionLineSep(const char value);

        //! Returns a number of <b>NoBreakDescLineIf</b> entries
        /*!
         *  See #getNoBreakDescLineIf() for explanations.
         */
        int getNumberOfNoBreakDescLineIf() const;

        //! Returns an entry <b>NoBreakDescLineIf</b> by its number
        /*!
         *  <b>NoBreakDescLineIf</b> specifies keywords that prevent description line
         *  from being broken into separate words. By default it is an empty list.
         *  \param index number of entry from 0 to (#getNumberOfNoBreakDescLineIf()-1)
         *  \return a string value
         */
        std::string getNoBreakDescLineIf(const int index) const;

        //! Deletes all entries for <b>NoBreakDescLineIf</b> parameter
        /*!
         *  See #getNoBreakDescLineIf() for more information.
         */
        void clearNoBreakDescLineIf();

        //! Adds an entry into the list for <b>NoBreakDescLineIf</b> parameter
        /*!
         *  See #getNoBreakDescLineIf() for more information.
         *  \param str an item to add a copy of into the list
         */
        void appendNoBreakDescLineIf(const char* str);

        //! Returns a number of file names specified in <b>SpeciesFiles</b> parameter
        /*!
         *  See #getSpeciesFiles() and documentation for ms_taxspeciesfiles class.
         */
        int getNumberOfSpeciesFiles() const;

        //! Returns an instance of ms_taxspeciesfiles describing one entry in <b>SpeciesFiles</b> parameter
        /*!
         *  The <b>SpeciesFiles</b> file is used to convert a species name to a taxonomy ID.
         *  For more detailed information on how to configure this parameter consult Mascot manual
         *  and documentation for ms_taxspeciesfiles class. By default the list is empty.
         *  \param index file number from 0 to (#getNumberOfSpeciesFiles()-1)
         *  \return instance of ms_taxspeciesfiles class describing one of the files 
         *  specified in <b>SpeciesFiles</b> parameter of the taxonomy-section.
         */
        const ms_taxspeciesfiles * getSpeciesFile(const int index) const;

        //! Deletes all entries for <b>SpeciesFiles</b> parameter
        /*!
         *  See #getSpeciesFiles() for more information.
         */
        void clearSpeciesFiles();

        //! Adds an entry into the list for <b>SpeciesFiles</b> parameter
        /*!
         *  See #getSpeciesFiles() for more information.
         *  \param item an item to add a copy of into the list
         */
        void appendSpeciesFile(const ms_taxspeciesfiles * item);

        // no longer in use
        int getNumberOfStrFiles() const;
        // no longer in use
        const ms_taxspeciesfiles * getStrFile(const int index) const;
        // no longer in use
        void clearStrFiles();
        // no longer in use
        void appendStrFile(const ms_taxspeciesfiles * item);

        // no longer in use
        const ms_parserule_plus* getStrRule() const;
        // no longer in use
        void setStrRule(const ms_parserule_plus* src);

        //! Returns a number of file names specified in <b>NodesFiles</b> parameter
        /*!
         *  See #getNodesFiles() and documentation for ms_taxnodesfiles class.
         */
        int getNumberOfNodesFiles() const;

        //! Returns an instance of ms_taxnodesfiles describing one entry in <b>NodesFiles</b> parameter
        /*!
         *  The <b>NodesFiles</b> file is used to traverse taxonomy hierarchy. 
         *  It contains taxonomy ID nodes together with their parents IDs.
         *  For more detailed information on how to configure this parameter 
         *  consult Mascot manual and documentation for ms_taxnodesfiles class. 
         *  By default the list of files is empty.
         *  \param index file number from 0 to (#getNumberOfNodesFiles()-1)
         *  \return instance of ms_taxnodesfiles class describing one of the files 
         *  specified in <b>NodesFiles</b> parameter of the taxonomy-section
         */
        const ms_taxnodesfiles * getNodesFile(const int index) const;

        //! Deletes all entries for <b>NodesFiles</b> parameter
        /*!
         *  See #getNodesFiles() for more information.
         */
        void clearNodesFiles();

        //! Adds an entry into the list for <b>NodesFiles</b> parameter
        /*!
         *  See #getNodesFiles() for more information.
         *  \param item an item to add a copy of into the list
         */
        void appendNodesFile(const ms_taxnodesfiles * item);

        //! Returns a number of file names specified in <b>GencodeFiles</b> parameter
        /*!
         *  See #getGencodeFiles() and documentation for ms_taxnodesfiles class.
         */
        int getNumberOfGencodeFiles() const;

        //! Returns an instance of ms_taxnodesfiles describing one entry in <b>GencodeFiles</b> parameter
        /*!
         *  The <b>GencodeFiles</b> file is used to find a proper NA translation table for a given taxonomy.
         *  For more detailed information on how to configure this parameter 
         *  consult Mascot manual and documentation for ms_taxnodesfiles class.
         *  By default the list of files is empty.
         *  \param index file number from 0 to (#getNumberOfGencodeFiles()-1)
         *  \return instance of ms_taxnodesfiles class describing one of the files 
         *  specified in <b>GencodeFiles</b> parameter of the taxonomy-section.
         */
        const ms_taxnodesfiles * getGencodeFile(const int index) const;

        //! Deletes all entries for <b>GencodeFiles</b> parameter
        /*!
         *  See #getGencodeFiles() for more information.
         */
        void clearGencodeFiles();

        //! Adds an entry into the list for <b>GencodeFiles</b> parameter
        /*!
         *  See #getGencodeFiles() for more information.
         *  \param item an item to add a copy of into the list
         */
        void appendGencodeFile(const ms_taxnodesfiles * item);

        //! Returns a value of <b>DefaultRule</b> parameter
        /*!
         *  The <b>DefaultRule</b> describes how to find the species name in the 
         *  line of text in the reference file. 
         *  The string in quotes is a regular expression.
         *  All words in the <b>PrefixRemoves</b> and 
         *  <b>SuffixRemoves</b> keywords should be removed before trying 
         *  to do a match. For more detailed information on how to specify this 
         *  parameter consult Mascot manual. If specified <b>DoThisRuleFirst</b> 
         *  is applied first, and the default rule would only be used if this failed.
         *  \sa #getQuickRefSearch(), #getDoThisRuleFirst() and #getAccFromSpeciesLine().
         */
        const ms_parserule_plus* getDefaultRule() const;

        //! Allows to change <b>DefaultRule</b> parameter value
        /*!
         *  See #getDefaultRule().
         */
        void setDefaultRule(const ms_parserule_plus* src);

        //! Returns a number of <b>PrefixRemoves</b> parameter strings
        /*!
         *  See #getPrefixRemove().
         */
        int getNumberOfPrefixRemoves() const;

        //! Returns <b>PrefixRemoves</b> string by number
        /*!
         *  See #getDefaultRule() for information on this parameter.
         *  By default the list of prefixes is empty.
         *  \param index number of a string specified in the parameter from 0 to (#getNumberOfPrefixRemoves()-1)
         *  \return one of the string specifed in the parameter
         */
        std::string getPrefixRemove(const int index) const;

        //! Deletes all entries for <b>PrefixRemoves</b> parameter
        /*!
         *  See #getPrefixRemove() for more information.
         */
        void clearPrefixRemoves();

        //! Adds an entry into the list for <b>PrefixRemoves</b> parameter
        /*!
         *  See #getPrefixRemove() for more information.
         *  \param item an item to add a copy of into the list
         */
        void appendPrefixRemove(const char * item);

        //! Returns a number of <b>SuffixRemoves</b> parameter strings
        /*!
         *  See #getSuffixRemove().
         */
        int getNumberOfSuffixRemoves() const;

        //! Returns <b>SuffixRemoves</b> string by number
        /*!
         *  See #getDefaultRule() for information on this parameter.
         *  By default the list of suffixes is empty.
         *  \param index number of a string specified in the parameter from 0 to (#getNumberOfSuffixRemoves()-1)
         *  \return one of the string specifed in the parameter
         */
        std::string getSuffixRemove(const int index) const;

        //! Deletes all entries for <b>SuffixRemoves</b> parameter
        /*!
         *  See #getSuffixRemove() for more information.
         */
        void clearSuffixRemoves();

        //! Adds an entry into the list for <b>SuffixRemoves</b> parameter
        /*!
         *  See #getSuffixRemove() for more information.
         *  \param item an item to add a copy of into the list
         */
        void appendSuffixRemove(const char * item);

        //! Returns value of <b>SrcDatabaseRule</b> parameter
        /*!
         *  The parameter is used for finding database source string with regular expression.
         *  \sa #getPerDbSrcRules().
         */
        const ms_parserule* getSrcDatabaseRule() const;

        //! Allows to change value of <b>SrcDatabaseRule</b> parameter
        /*!
         *  See #getSrcDatabaseRule().
         */
        void setSrcDatabaseRule(const ms_parserule* src);

        //! Returns a number of database source strings
        /*!
         *  See #getPerDbSrcRule() and documentation for ms_parserule_plus class.
         */
        int getNumberOfPerDbSrcRules() const;

        //! Returns database source string by its number
        /*!
         *  Database source strings contained in taxonomy section all look like
         *  RULE_XXX or OTHERRULE.
         *  By default the list of rules is empty.
         *  \param index database source string number from 0 to (#getNumberOfPerDbSrcRules()-1)
         */
        const ms_parserule_plus * getPerDbSrcRule(const int index) const;

        //! Deletes all database source strings
        /*!
         *  See #getPerDbSrcRule() for more information.
         */
        void clearPerDbSrcRules();

        //! Adds a new database source string into the list
        /*!
         *  See #getPerDbSrcRule() for more information.
         *  \param item an item to add a copy of into the list
         */
        void appendPerDbSrcRule(const ms_parserule_plus * item);

        //! Returns value of <b>DoThisRuleFirst</b> parameter
        /*!
         *  See #getDefaultRule().
         */
        const ms_parserule* getDoThisRuleFirst() const;

        //! Allows to change <b>DoThisRuleFirst</b> parameter value
        /*!
         *  See #getDoThisRuleFirst().
         */
        void setDoThisRuleFirst(const ms_parserule* src);

        //! Returns value of <b>AccFromSpeciesLine</b> parameter
        /*!
         *  MSDB database explicitly associates each species line 
         *  with the accession string of the primary database entry. 
         *  A further rule, <b>AccFromSpeciesLine</b>, is used to 
         *  extract this accession string.
         */
        const ms_parserule* getAccFromSpeciesLine() const;

        //! Allows to change <b>AccFromSpeciesLine</b> parameter
        /*!
         *  See #getAccFromSpeciesLine() for more information.
         */
        void setAccFromSpeciesLine(const ms_parserule* src);

        //! Returns value of <b>QuickRefSearch</b> parameter
        /*!
         *  The <b>QuickRefSearch</b> string is used to speed up the 
         *  compressing of the database. Rather than use the regular 
         *  expression for each line in the <tt>.ref</tt> file, 
         *  this text is used for a fast compare to the string <tt>C;Species</tt>. 
         *  Other lines are ignored.
         *  By default it is empty.
         */
        std::string getQuickRefSearch() const;

        //! Allows to change value of <b>QuickRefSearch</b> parameter
        /*!
         *  See #getQuickRefSearch().
         */
        void setQuickRefSearch(const char* str);

        //! Returns value of <b>DBLevelTaxId</b> parameter
        /*!
         *  DB-level taxonomy ID can be specified in order to find proper NA translation
         *  table without <tt>names.dmp</tt> and <tt>nodes.dmp</tt> files usage.
         *  Default value is <b>-1</b> which is not a valid taxonomy ID.
         */
        int getDBLevelTaxId() const;

        //! Allows to change <b>DBLevelTaxId</b> parameter value
        /*
         *  See #getDBLevelTaxId().
         */
        void setDBLevelTaxId(const int value);

        //! Returns <b>TRUE</b> if there is such a parameter in the file
        bool isDBLevelTaxId() const;

        //! Deletes <b>DBLevelTaxId</b> parameter and prevents it from storing in the file
        void clearDBLevelTaxId();

        //! Returns <b>TRUE</b> if <b>MitochondrialTranslation</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  Two types of NA translation tables can be used: nuclear and mitochondrial.
         *  Setting this parameter to <b>0</b> indicates the first type whereas <b>1</b> value
         *  indicates the second one.
         *  Default value is <b>0</b>.
         */
        bool isMitochondrialTranslation() const;

        //! Allows to set <b>MitochondrialTranslation</b> parameter value
        /*!
         *  See #isMitochondrialTranslation().
         */
        void setMitochondrialTranslation(const bool flag);

    private:
        bool                sectionAvailable_;
        bool                enabled_;
        std::string         identifier_;
        int                 errorLevel_;
        bool                fromRefFile_;
        bool                concatRefFileLines_;
        char                descriptionLineSep_;

        std::vector< std::string > noBreakDescLineIf_;
        std::vector< ms_taxspeciesfiles* > speciesFiles_;
        std::vector< ms_taxspeciesfiles* > strStrFiles_;
        std::vector< ms_taxnodesfiles* > nodesFiles_;
        std::vector< ms_taxnodesfiles* > gencodeFiles_;
        std::vector< ms_parserule_plus* > perDbSrcRules_;
        std::vector< std::string > prefixRemoves_;
        std::vector< std::string > suffixRemoves_;

        ms_parserule        srcDatabaseRule_;
        ms_parserule        doThisRuleFirst_;
        ms_parserule        accFromSpeciesLine_;
        ms_parserule_plus   defaultRule_;
        ms_parserule_plus   strStrRule_;

        std::string         quickRefSearch_;
        int                 dbLevelTaxId_;
        bool                mitochondrialTranslation_;
    }; // class ms_taxonomyrules
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_TAXONOMYRULES_HPP

/*------------------------------- End of File -------------------------------*/
