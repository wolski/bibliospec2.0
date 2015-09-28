/*
##############################################################################
# file: ms_wwwoptions.hpp                                                    #
# 'msparser' toolkit                                                         #
# Represents "WWW" section of "mascot.dat" file                              #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_wwwoptions.hpp          $ #
#     $Author: davidc $ #
#       $Date: 2007-01-16 13:16:11 $ #
#   $Revision: 1.7 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_WWWOPTIONS_HPP
#define MS_WWWOPTIONS_HPP

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

    //! Lists all supported types of entries in <tt>WWW</tt> section
    /*!
     * Sequence string and full text reports are supported
     * To access from perl, use, for example: <P><tt>
     * $x = $msparser::WWW_REP
     * </tt></P>
     */
    enum WWW_TYPE
    {
        WWW_SEQ = 0,    //!< Sequence string source
        WWW_REP = 1     //!< Full text report source
    };

    //! Represent a single entry in <tt>WWW</tt> section of <tt>mascot.dat</tt> file
    /*!
     *  A number of instances of this class are held in ms_wwwoptions class 
     *  in order to represent the whole section. Consult Mascot manual if you are unsure
     *  of how to configure an entry. It defines an information source where CGI scripts 
     *  look for the information needed to compile a results report.
     */
    class MS_MASCOTRESFILE_API ms_wwwentry
    {
        friend class ms_datfile;
        friend class ms_wwwoptions;

    public:
        //! Default constructor
        ms_wwwentry();

        //! Copying constructor
        ms_wwwentry(const ms_wwwentry& src);
        
        //! Destructor
        virtual ~ms_wwwentry();

        //! Initialises the instance with default values
        void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_wwwentry* right);

        //! Assignment operator for C++ client applications
        ms_wwwentry& operator=(const ms_wwwentry& right);

        //! Returns name of the database the entry refers to
        /*!
         *  Please note, that database name is case sensitive.
         *  The name should be one of the specified in <tt>Databases</tt> section names.
         *  Every name can be associated with more than one entry in <tt>WWW</tt> section
         *  as different entry types are supported. In order to retrieve entry type use 
         *  #getType().<BR>
         *  By default it is an empty string.
         */
        std::string getName() const;

        //! Allows to set database name for the entry
        /*!
         *  See #getName() for more information.
         */
        void setName(const char* value);

        //! Returns name of the database the entry refers to
        /*!
         *  Several entry types are supported. For the complete list of types see
         *  enumeration values of WWW_TYPE type.<BR>
         *  There is no default value for this parameter.
         */
        WWW_TYPE getType() const;

        //! Allows to change entry type
        /*!
         *  See #getType() for more information.
         */
        void setType(const WWW_TYPE value);

        //! Returns a parse rule number for the entry that corresponds to one of the parse rules specified in <tt>PARSE</tt> section of <tt>mascot.dat</tt> file
        /*!
         *  The index of a rule in the <tt>PARSE</tt> section that can be used to 
         *  extract the information required.<BR>
         *  Default value is <b>-1</b> which is an invalid rule number.
         */
        int getParseRule() const;

        //! Allows to change parse rule number for the entry
        /*!
         *  See #getParseRule() for more information.
         */
        void setParseRule(const int value);

        //! Returns a host name used to retrieve reports from
        /*!
         *  The information source. For <tt>ms-getseq.exe</tt> or a similar local executable, 
         *  this column should contain localhost. 
         *  For a remote source, or a local source that will be queried as 
         *  a CGI application, enter the hostname. 
         *  (The word <b>localhost</b> is used to determine whether 
         *  the application is a command line executable or a CGI application. 
         *  If you want to specify a CGI application on the local server, 
         *  just specify the hostname in some other way, for example <b>127.0.0.1</b>).<BR>
         *  \sa #getPortNumber()
         *  Default value is <b>localhost</b>.
         */
        std::string getHostName() const;

        //! Allows to change host name parameter
        /*!
         *  See #getHostName() for more information.
         */
        void setHostName(const char* value);

        //! Returns a port number for the host used as the information source
        /*!
         *  In most cases it should be left as <b>80</b> which is a default value.
         *  \sa #getHostName()
         */
        int getPortNumber() const;

        //! Allows to change port number
        /*!
         *  See #getPortNumber() for more information.
         */
        void setPortNumber(const int value);

        //! Returns path to the executable and parameters
        /*!
         *  A string containing the path to the executable and parameters, 
         *  some of which are variables. By default it is empty.
         */
        std::string getPath() const;

        //! Allows to change path
        /*!
         *  See #getPath() for more information.
         */
        void setPath(const char* value);

    private:
        std::string name_;
        WWW_TYPE type_;
        int parseRuleNum_;
        std::string hostName_;
        int portNumber_;
        std::string path_;

        std::string getStringName() const;
        std::string getStringValue() const;
    }; // class MS_MASCOTRESFILE_API ms_wwwentry

    //! Represents the whole <tt>WWW</tt> section
    /*!
     *  <tt>WWW</tt> section defines where CGI scripts look for 
     *  the information needed to compile a results report.
     *  At least one entry is required for each database, 
     *  to define the source from which the sequence string of 
     *  a database entry can be obtained. A second line can 
     *  optionally define the source from which the full text 
     *  report of an entry can be obtained. 
     *  The syntax is very similar in both cases, independent of 
     *  whether the information originates locally or on a remote system.<BR>
     *  An instance of this class holds a list of entries (See class ms_wwwentry) 
     *  that can be obtained and dealt with individually. By default the list is empty.
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
    class MS_MASCOTRESFILE_API ms_wwwoptions: public ms_customproperty
    {
        friend class ms_datfile;
    public:

        //! Default constructor
        ms_wwwoptions();

        //! Copying constructor
        ms_wwwoptions(const ms_wwwoptions& src);

        //! Destructor
        virtual ~ms_wwwoptions();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_wwwoptions* right);

        //! Assignment operator for C++ client applications
        ms_wwwoptions& operator=(const ms_wwwoptions& right);

        //! Tells whether the section has been actually read from the file
        /*!
         *  By default a <tt>WWW</tt> section is unavailable until it has 
         *  been set to a different state.
         */
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. should it be saved in a file
        void setSectionAvailable(const bool value);

        //! Returns total number of entries in the section
        /*!
         *  Note that there might be several entries associated with each database. 
         *  Therefore, total number of entries differs with the number of databases
         *  in <tt>Databases</tt> section. All entries are not ordered and 
         *  represented as is. Use specific functions like #getSeqEntryByName() and
         *  #getRepEntryByName() to retrieve the entry you know a name of
         *  of simply #getEntry() by the entry index.
         */
        int getNumberOfEntries() const;

        //! Returns an entry by its index
        /*!
         *  See #getNumberOfEntries() for more information.
         */
        const ms_wwwentry* getEntry(const int index) const;

        //! Searches the entries list for the given name and type <tt>SEQ</tt>
        /*!
         *  If an entry is not found <b>NULL</b> value is returned back.
         *  Note that database names are case sensitive.
         */
        const ms_wwwentry* getSeqEntryByName(const char* dbName) const;
        
        //! Searches the entries list for the given name and type <tt>REP</tt>
        /*!
         *  If an entry is not found <b>NULL</b> value is returned back.
         *  Note that database names are case sensitive.
         */
        const ms_wwwentry* getRepEntryByName(const char* dbName) const;

        //! Deletes all entries from the list
        void clearEntries();
        
        //! Append a new entry which is a copy of the only parameter
        void appendEntry(const ms_wwwentry* item);

    private:
        bool sectionAvailable_;
        std::vector< ms_wwwentry* > entries_;
    }; // class ms_wwwoptions
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_WWWOPTIONS_HPP
