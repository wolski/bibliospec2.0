/*
##############################################################################
# file: ms_mascotoptions.hpp                                                 #
# 'msparser' toolkit                                                         #
# Encapsulates "Options" section of "mascot.dat" file that describes most    #
# important parameters                                                       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_mascotoptions.hpp       $ #
#     $Author: yuryr $ #
#       $Date: 2007-01-18 18:24:44 $ #
#   $Revision: 1.23 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTOPTIONS_HPP
#define MS_MASCOTOPTIONS_HPP

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

    // class for internal usage only
    class MS_MASCOTRESFILE_API ms_blastaccession
    {
        friend class ms_datfile;

    public:
        ms_blastaccession();
        ms_blastaccession(const ms_blastaccession& src);
        virtual ~ms_blastaccession();

        void defaultValues();
        void copyFrom(const ms_blastaccession* right);
        ms_blastaccession& operator=(const ms_blastaccession& right);

        bool isUseRegex() const;
        void setUseRegex(const bool flag);

        int getMinLength() const;
        void setMinLength(const int value);

        const char* getStart() const;
        const char* getStop() const;

    private:

        bool useRegex_;
        int  minLen_;
        char start_[255];
        char stop_[255];
    }; // class ms_blastaccession

    //! An instance of this class represents all the parameters specified in <b>Option</b>-section of <tt>mascot.dat</tt>
    /*!
     *  An instance of this class is created and polulated in ms_datfile-class. 
     *  It can also be created separately and initialized with default values. One can create
     *  an instance of the class or copy from another instance in order to pass it then 
     *  as an options-containing object.
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
    class MS_MASCOTRESFILE_API ms_mascotoptions: public ms_customproperty
    {
        friend class ms_datfile;

    public:
        //! Definitions for columns in ms-review.exe for use with getReviewColWidth  ( 
        /*!
        * To access from perl, use, for example: <P>
        *  <tt>$x = $msparser::ms_mascotoptions::MS_REVIEW_COL_DURATION</tt>
        *  </P><BR>
        */
        enum MS_REVIEW_COL
        {
            MS_REVIEW_COL_JOB           = 0x0000, //!< The job number column
            MS_REVIEW_COL_PID           = 0x0001, //!< The process ID column
            MS_REVIEW_COL_DATABASE      = 0x0002, //!< The database name column
            MS_REVIEW_COL_USERNAME      = 0x0003, //!< The USERNAME column
            MS_REVIEW_COL_USEREMAIL     = 0x0004, //!< The USEREMAIL column
            MS_REVIEW_COL_TITLE         = 0x0005, //!< The search title (COM) column
            MS_REVIEW_COL_RESULTS_FILE  = 0x0006, //!< The results file name column 
            MS_REVIEW_COL_START_TIME    = 0x0007, //!< The job start time and date column
            MS_REVIEW_COL_DURATION      = 0x0008, //!< The job duration column (seconds)
            MS_REVIEW_COL_STATUS        = 0x0009, //!< The job status column - normally 'User read res'
            MS_REVIEW_COL_PRIORITY      = 0x000A, //!< The job priority column (0 = normal priority)
            MS_REVIEW_COL_SEARCHTYPE    = 0x000B, //!< The job search type column (PMF, MIS or SQ)
            MS_REVIEW_COL_ENZYMEUSED    = 0x000C, //!< The job enzyme used column (Yes or No)
            MS_REVIEW_COL_IPADDRESS     = 0x000D, //!< The IP address from where the search was submitted
            MS_REVIEW_COL_USERID        = 0x000E, //!< The USERID of the user who submitted the search
            MS_REVIEW_COL____LAST___    = 0x000F  //!< Place holder
        };

        //! Default constructor
        ms_mascotoptions();

        //! Copying constructor
        ms_mascotoptions(const ms_mascotoptions& src);

        //! Destructor
        virtual ~ms_mascotoptions();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_mascotoptions* right);

        //! C++ style assignment operator
        ms_mascotoptions& operator=(const ms_mascotoptions& right);

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Tells whether the section has been actually read from the file
        /*!
         *  By default the <tt>Options</tt> section is unavailable until it has 
         *  been set to a different state.
         */
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. should it be saved in a file
        void setSectionAvailable(const bool value);

        //! Return TRUE if <b>SaveLastQueryAsc</b> parameter is set to <b>1</b> in the file
        /*!
         *  <b>SaveLastQueryAsc</b> is a flag which controls whether the most recent input file 
         *  to Mascot (i.e. the MIME format file containing MS data and search parameters) 
         *  should be saved to disk (<b>1</b>) or not (<b>0</b>). This can be a useful debugging tool 
         *  when writing scripts or forms to submit searches to Mascot. 
         *  If <b>SaveLastQueryAsc</b> is set to <b>1</b>, the name of the file is determined by 
         *  <b>LastQueryAscFile</b>. Each new search overwrites this file. 
         *  Default value is <b>0</b>.
         *  \sa #isSaveEveryLastQueryAsc(), #getLastQueryAscFile().
         */
        bool isSaveLastQueryAsc() const;

        //! Sets <b>SaveLastQueryAsc</b> parameter to <b>1</b> or <b>0</b>. 
        /*!
         *  For more detailed information see #isSaveLastQueryAsc().
         */
        void setSaveLastQueryAsc(const bool flag);

        //! Returns TRUE if <b>SaveEveryLastQueryAsc</b> parameter is set to <b>1</b> in the file
        /*!
         *  If set to <b>1</b>, the Mascot input file will be saved for any search that fails 
         *  to complete because it generates a fatal error. The name of the output file 
         *  follows the same naming convention as a normal Mascot output file, 
         *  except for the additional suffix <tt>.inp</tt>. 
         *  If a search goes to completion, this file is deleted as soon as the 
         *  normal output file has been written to disk. 
         *  Default value is <b>1</b>.
         *  \sa #isSaveLastQueryAsc(), #getLastQueryAscFile().
         */
        bool isSaveEveryLastQueryAsc() const;

        //! Sets <b>SaveEveryLastQueryAsc</b> parameter to <b>1</b> or <b>0</b>
        /*!
         *  For more detailed information see #isSaveEveryLastQueryAsc().
         */
        void setSaveEveryLastQueryAsc(const bool flag);

        //! Returns value of <b>LastQueryAscFile</b> parameter
        /*!
         *  See #isSaveLastQueryAsc() for detailed explanations. 
         *  Default value for this parameter is <tt>../logs/lastquery.asc</tt>.
         */
        std::string getLastQueryAscFile() const;

        //! Sets <b>LastQueryAscFile</b> into the supplied value
        /*!
         *  See #isSaveLastQueryAsc() for detailed explanation.
         */
        void setLastQueryAscFile(const char* str);

        //! Returns <b>ErrorLogFile</b> parameter value
        /*!
         *  One of the critical file paths. 
         *  It defines where error messages should be stored.
         *  Default value is <tt>../logs/errorlog.txt</tt>
         */
        std::string getErrorLogFile() const;

        //! Allows to change <b>ErrorLogFile</b> property
        /*!
         *  See #getErrorLogFile() for more information.
         */
        void setErrorLogFile(const char* str);

        //! Returns <b>SearchLogFile</b> parameter value
        /*!
         *  One of the critical file paths. 
         *  It defines where search log file should be placed.
         *  Default value is <tt>../logs/searches.log</tt>
         */
        std::string getSearchLogFile() const;

        //! Allows to change <b>SearchLogFile</b> property
        /*!
         *  See #getSearchLogFile() for more information.
         */
        void setSearchLogFile(const char* str);

        //! Returns <b>MonitorLogFile</b> parameter value
        /*!
         *  One of the critical file paths. 
         *  It defines where monitor log file should be placed.
         *  Default value is <tt>../logs/monitor.log</tt>
         */
        std::string getMonitorLogFile() const;

        //! Allows to change <b>MonitorLogFile</b> property
        /*!
         *  See #getMonitorLogFile() for more information.
         */
        void setMonitorLogFile(const char* str);

        //! Returns value of <b>InterFileBasePath</b> parameter
        /*!
         *  One of the critical values that determine a location for storing 
         *  intermediate/output files. Mascot output files are written to a path given by:<BR>
         *  <tt>InterFileBasePath/InterFileRelPath/yyyymmdd/Fnnnnnn.dat</tt> <BR>
         *  Where <b>yyyymmdd</b> is the current ISO date, and <b>nnnnnn</b> is a 
         *  sequential job number. The path is split into a base path and a relative 
         *  path as seen by the CGI scripts so that the search engine can pass 
         *  a file path to (say) master_results.pl as:<BR>
         *  <tt>InterFileRelPath/yyyymmdd/Fnnnnnn.dat</tt><BR>
         *  Default value is <tt>C:/InetPub/Mascot/data</tt>.
         */
        std::string getInterFileBasePath() const;

        //! Allows to set <b>InterFileBasePath</b> parameter
        /*!
         *  See #getInterFileBasePath() for possible explanations.
         */
        void setInterFileBasePath(const char* str);

        //! Returns value of <b>InterFileRelPath</b> parameter
        /*!
         *  One of the critical values that determine a location for storing
         *  intermediate files. 
         *  Default value is <tt>../data</tt>. 
         *  See #getInterFileBasePath() for a more detailed explanation.
         */
        std::string getInterFileRelPath() const;

        //! Allows to set <b>InterFileRelPath</b> parameter
        /*!
         *  See #getInterFileRelPath() for possible explanations.
         */
        void setInterFileRelPath(const char* str);

        //! Returns <b>MascotCmdLine</b> parameter value
        /*!
         *  Filepath to the mascot executable. 
         *  Default value is <tt>../cgi/nph-mascot.exe</tt>.
         */
        std::string getMascotCmdLine() const;

        //! Allows to change <b>MascotCmdLine</b> parameter value
        /*!
         *  See #getMascotCmdLine() for more information.
         */
        void setMascotCmdLine(const char* str);

        //! Returns <b>TestDirectory</b> parameter value
        /*!
         *  <b>TestDirectory</b> contains the input files used by Monitor 
         *  to test new sequence databases. 
         *  Default value is <tt>../data/test</tt>.
         */
        std::string getTestDirectory() const;

        //! Allows to change <b>TestDirectory</b> parameter value
        /*!
         *  See #getTestDirectory() for more information.
         */
        void setTestDirectory(const char* str);

        //! Returns <b>MascotControlFile</b> parameter value
        /*!
         *  <b>MascotControlFile</b> contains critical internal parameters. 
         *  This file must be memory mapped and locked to provide interprocess 
         *  communication between different Mascot components. 
         *  Default value is <tt>../data/mascot.control</tt>.
         */
        std::string getMascotControlFile() const;

        //! Allows to change <b>MascotControlFile</b> parameter value
        /*!
         *  See #getMascotControlFile() for more information.
         */
        void setMascotControlFile(const char* str);

        //! Returns <b>MascNodeCtrlFile</b> parameter value
        /*!
         *  Default value is <tt>../data/mascotnode.control</tt>.
         */
        std::string getMascNodeCtrlFile() const;

        //! Allows to change <b>MascNodeCtrlFile</b> parameter value
        /*!
         *  See #getMascNodeCtrlFile() for more information.
         */
        void setMascNodeCtrlFile(const char* str);

        //! Returns <b>MascotJobIdFile</b> parameter value
        /*!
         *  <b>MascotJobIdFile</b> contains the next available job number. 
         *  If this file is deleted, the next job number will be initialised 
         *  to the value given by <b>UniqueJobStartNumber</b>, and a new jobId-file 
         *  created automatically. Default value is <tt>../data/mascot.job</tt>.
         */
        std::string getMascotJobIdFile() const;

        //! Allows to change <b>MascotJobIdFile</b> parameter value
        /*!
         *  See #getMascotJobIdFile() for more information.
         */
        void setMascotJobIdFile(const char* str);

        //! Returns <b>GetSeqJobIdFile</b> parameter value
        /*!
         *  <b>GetSeqJobIdFile</b> contains the next available job number for 
         *  the <tt>ms-getseq.exe</tt> utility. These numbers wrap around at <tt>999</tt> and do 
         *  not appear in the search logs. If this file is deleted, the next job 
         *  number will be reset to <b>1</b> and a new jobId-file created automatically.
         *  Default value is <tt>../data/getseq.job</tt>.
         */
        std::string getGetSeqJobIdFile() const;

        //! Allows to change <b>GetSeqJobIdFile</b> parameter value
        /*!
         *  See #getGetSeqJobIdFile() for more information.
         */
        void setGetSeqJobIdFile(const char* str);

        //! Returns <b>UniqueJobStartNumber</b> parameter value
        /*!
         *  See #getMascotJobIdFile() for explanations on usage of this parameter.
         *  Default value is <b>1</b>
         */
        int getUniqueJobStartNumber() const;

        //! Allows to change <b>UniqueJobStartNumber</b> parameter value
        /*!
         *  See #getUniqueJobStartNumber() for more information.
         */
        void setUniqueJobStartNumber(const int value);

        //! Returns a value of <b>ResultsPerlScript</b> parameter
        /*!
         *  There are URL’s (not disk paths) of the scripts to be called by the search 
         *  engine at the completion of a search. A successful search calls 
         *  <b>ResultsPerlScript</b>, while a search that didn’t find any hits calls 
         *  <b>NoResultsScript</b>. In the scripts supplied with Mascot, both 
         *  eventualities are handled by a single script: <tt>master_results.pl</tt> 
         *  - this is the default value for both of them.
         */
        std::string getResultsPerlScript() const;

        //! Allows to set <b>ResultsPerlScript</b> parameter
        /*!
         *  See #getResultsPerlScript() for more information.
         */
        void setResultsPerlScript(const char* str);

        //! Returns a value of <b>NoResultsScript</b> parameter
        /*!
         *  See #getResultsPerlScript() for more information.
         */
        std::string getNoResultsScript() const;

        //! Allows to set <b>NoResultsScript</b> parameter
        /*!
         *  See #getResultsPerlScript() for more information.
         */
        void setNoResultsScript(const char* str);

        //! Returns a value of <b>ResultsFullURL</b> parameter
        /*!
         *  The <b>ResultsFullURL</b> entry is used when results are emailed to a user. 
         *  Since the email will probably be received on another system, 
         *  the URL used needs to have the full name including the Web server.
         *  Default value is <tt>http://www.matrixscience.com/cgi/master_results.pl</tt>.
         */
        std::string getResultsFullURL() const;

        //! Allows to set <b>ResultsFullURL</b> parameter
        /*!
         *  See #getResultsFullURL() for more information.
         */
        void setResultsFullURL(const char* str);

        //! Returns <b>LogoImageFile</b> parameter value
        /*!
         *  This is the URL of the Matrix Science logo, used at the top of a 
         *  search progress report. You can customise this by substituting the 
         *  URL of your own logo. For optimum appearance, the image should be 
         *  88 pixels wide and 31 pixels high.
         *  Default value is <tt>../images/88x31_logo_white.gif</tt>.
         */
        std::string getLogoImageFile() const;

        //! Allows to change <b>LogoImageFile</b> property
        /*!
         *  See #getLogoImageFile() for more information.
         */
        void setLogoImageFile(const char* str);

        //! Returns <b>MassDecimalPlaces</b> parameter value
        /*!
         *  Mascot calculates all masses to an accuracy of 1/65535 Daltons. 
         *  By default, values are displayed to two decimal places, 
         *  which should be sufficient for most purposes. 
         *  The displayed precision of peptide and fragment ion masses 
         *  in reports can be altered by changing the values of <b>MassDecimalPlaces</b>.
         *  Default value is <b>2</b>.
         */
        int getMassDecimalPlaces() const;

        //! Allows to change <b>MassDecimalPlaces</b> property
        /*!
         *  See #getMassDecimalPlaces() for more information.
         */
        void setMassDecimalPlaces(const int value);

        //! Returns <b>IonsDecimalPlaces</b> parameter value
        /*!
         *  Mascot calculates all masses to an accuracy of 1/65535 Daltons. 
         *  By default, values are displayed to two decimal places, 
         *  which should be sufficient for most purposes. 
         *  The displayed precision of fragment ion masses in reports 
         *  can be altered by changing the values of <b>IonsDecimalPlaces</b>.
         *  Default value is <b>2</b>.
         */
        int getIonsDecimalPlaces() const;

        //! Allows to change <b>IonsDecimalPlaces</b> property
        /*!
         *  See #getIonsDecimalPlaces() for more information.
         */
        void setIonsDecimalPlaces(const int value);

        //! Returns <b>IntensitySigFigs</b> parameter value
        /*!
         *  By default, all values are displayed to two decimal places, 
         *  which should be sufficient for most purposes. 
         *  The precision of intensity values written to the result file 
         *  is determined by <b>IntensitySigFigs</b>.
         *  Default value is <b>2</b>.
         */
        int getIntensitySigFigs() const;

        //! Allows to change <b>IntensitySigFigs</b> property
        /*!
         *  See #getIntensitySigFigs() for more information.
         */
        void setIntensitySigFigs(const int value);

        //! Returns <b>TRUE</b> if <b>EmailUsersEnabled</b> property is enabled in the file
        /*!
         *  When the search engine executes as a CGI application, 
         *  email can be used to send the results of a search to a user 
         *  who accidentally or deliberately disconnected before the search was complete. 
         *  This facility can be enabled by setting <b>EmailUsersEnabled</b> to <b>1</b> or 
         *  disabled by setting it to <b>0</b>.
         *  Default value is <b>0</b>.<BR>
         *  If <b>EmailUsersEnabled</b> is set to <b>1</b>, 
         *  search results will be emailed to a user if their web browser does not 
         *  respond within the number of seconds specified in <b>EmailTimeOutPeriod</b>
         *  following the completion of a search. See #getEmailTimeOutPeriod().<BR>
         *  On some systems, email access may use a dial-up line. 
         *  To minimise the charges, email messages can be sent in batches 
         *  at intervals specified by <b>MonitorEmailCheckFreq</b> (in seconds). 
         *  See #getMonitorEmailCheckFreq().<BR>
         *  <b>MailTempFile</b> is the name of the temporary file used 
         *  to store email messages until they can be sent. See #getMailTempFile().<BR>
         *  A number of other parameters are used to define how email should be sent. 
         *  You can find some more information about other parameters at #getMailTransport().
         */
        bool isEmailUsersEnabled() const;

        //! Allows to change <b>EmailUsersEnabled</b> property
        /*!
         *  See #isEmailUsersEnabled() for more information.
         */
        void setEmailUsersEnabled(const bool value);

        //! Returns <b>TRUE</b> if <b>EmailErrorsEnabled</b> property is enabled in the file
        /*!
         *  Serious error messages can be emailed to an administrator. 
         *  This facility can be enabled by setting <b>EmailErrorsEnabled</b> to <b>1</b>
         *  or disabled by setting it to <b>0</b>. Default value is <b>0</b>.<BR>
         *  Error messages that are considered 
         *  serious are identified in the file <tt>errors.html</tt>. 
         *  This file can be found in the root directory of the installation 
         *  CD-ROM.<BR>
         *  A number of other parameters are required to send e-mail messages.
         *  Have a look at the documentation for #isEmailUsersEnabled(), #getErrMessageEmailTo(), 
         *  and #getMailTransport() for more information.
         */
        bool isEmailErrorsEnabled() const;

        //! Allows to change <b>EmailErrorsEnabled</b> property
        /*!
         *  See #isEmailErrorsEnabled() for more information.
         */
        void setEmailErrorsEnabled(const bool value);

        //! Returns one of the possible <b>MailTransport</b> parameter values.
        /*!
         *  <b>MailTransport</b> should be set to one of the following values:
         *  <ul>
         *  <li><b>0</b> for CMC. See #getEmailService(), #getEmailPassword() </li>
         *  <li><b>1</b> for MAPI. See #getEmailProfile(), #getEmailPassword() </li>
         *  <li><b>2</b> for sendmail (Unix only). See #getSendmailPath() </li>
         *  <li><b>3</b> for Blat. See #getSendmailPath().</li>
         *  </ul>
         *  Default value is <b>2</b>.<BR>
         *  E-mail functionality has to be enabled by one of the following
         *  parameters: #isEmailUsersEnabled(), #isEmailErrorsEnabled().<BR>
         *  Some other parameters are common for all e-mail transports.
         *  \sa #getEmailFromUser(), #getEmailFromTextName(), #getErrMessageEmailTo(),
         *  #getMailTempFile().
         */
        int getMailTransport() const;

        //! Allows to change <b>MailTransport</b> property
        /*!
         *  See #getMailTransport() for more information.
         */
        void setMailTransport(const int value);

        //! Returns a value of <b>EmailService</b> parameter
        /*!
         *  <b>EmailService</b> is the service name (CMC only). By default it is empty.
         *  \sa #getMailTransport(), #isEmailUsersEnabled(), #isEmailErrorsEnabled().
         */
        std::string getEmailService() const;

        //! Allows to change <b>EmailService</b> property
        /*!
         *  See #getEmailService() for more information.
         */
        void setEmailService(const char* str);

        //! Returns a value of <b>EmailPassword</b> parameter
        /*!
         *  <b>EmailPassword</b> is the password (if any) required to 
         *  log onto MAPI or CMC. By default it is empty.
         *  \sa #getMailTransport(), #isEmailUsersEnabled(), #isEmailErrorsEnabled().
         */
        std::string getEmailPassword() const;

        //! Allows to change <b>EmailPassword</b> property
        /*!
         *  See #getEmailPassword() for more information.
         */
        void setEmailPassword(const char* str);

        //! Returns a value of <b>EmailPassword</b> parameter
        /*!
         *  <b>EmailProfile</b> is the MAPI profile name. By default it is empty.
         *  \sa #getMailTransport(), #isEmailUsersEnabled(), #isEmailErrorsEnabled().
         */
        std::string getEmailProfile() const;

        //! Allows to change <b>EmailPassword</b> property
        /*!
         *  See #getEmailProfile() for more information.
         */
        void setEmailProfile(const char* str);

        //! Returns a value of <b>sendmailPath</b> parameter
        /*!
         *  <b>sendmailPath</b> is the path to sendmail or BLAT programs.
         *  Default value is <tt>/usr/lib/sendmail</tt>.
         *  \sa #getMailTransport(), #isEmailUsersEnabled(), #isEmailErrorsEnabled().
         */
        std::string getSendmailPath() const;

        //! Allows to change <b>sendmailPath</b> property
        /*!
         *  See #getSendmailPath() for more information.
         */
        void setSendmailPath(const char* str);

        //! Returns a value of <b>EmailFromUser</b> parameter
        /*!
         *  <b>EmailFromUser</b> is the name which will appear in the <tt>From</tt> 
         *  field of the email message. Used by all e-mail configurations.
         *  By default it is empty.
         *  \sa #getMailTransport(), #isEmailUsersEnabled(), #isEmailErrorsEnabled().
         */
        std::string getEmailFromUser() const;

        //! Allows to change <b>EmailFromUser</b> property
        /*!
         *  See #getEmailFromUser() for more information.
         */
        void setEmailFromUser(const char* str);

        //! Returns a value of <b>EmailFromTextName</b> parameter
        /*!
         *  <b>EmailFromTextName</b> will appear in the <tt>Title</tt> field of the message. 
         *  Used by all e-mail configurations. By default it is empty.
         *  \sa #getMailTransport(), #isEmailUsersEnabled(), #isEmailErrorsEnabled().
         */
        std::string getEmailFromTextName() const;

        //! Allows to change <b>EmailFromTextName</b> property
        /*!
         *  See #getEmailFromTextName() for more information.
         */
        void setEmailFromTextName(const char* str);

        //! Returns a value of <b>EmailTimeOutPeriod</b> parameter
        /*!
         *  Default value is <b>120</b> (sec).
         *  See #isEmailUsersEnabled() for information on this parameter.
         */
        int getEmailTimeOutPeriod() const;

        //! Allows to change <b>EmailTimeOutPeriod</b> property
        /*!
         *  See #getEmailTimeOutPeriod() for more information.
         */
        void setEmailTimeOutPeriod(const int value);

        //! Returns a value of <b>MonitorEmailCheckFreq</b> parameter
        /*!
         *  Default value is <b>300</b> (sec.).
         *  See #isEmailUsersEnabled() for information on this parameter.
         */
        int getMonitorEmailCheckFreq() const;

        //! Allows to change <b>MonitorEmailCheckFreq</b> property
        /*!
         *  See #getMonitorEmailCheckFreq() for more information.
         */
        void setMonitorEmailCheckFreq(const int value);

        //! Returns a value of <b>MailTempFile</b> parameter
        /*!
         *  Default value is <tt>C:/TEMP/MXXXXXX</tt>
         *  See #isEmailUsersEnabled() for information on this parameter.
         */
        std::string getMailTempFile() const;

        //! Allows to change <b>MailTempFile</b> property
        /*!
         *  See #getMailTempFile() for more information.
         */
        void setMailTempFile(const char* str);

        //! Returns a value of <b>ErrMessageEmailTo</b> parameter
        /*!
         *  If <b>EmailErrorsEnabled</b> is set to <b>1</b>, serious error messages 
         *  will be emailed to <b>ErrMessageEmailTo</b>. By default it is empty.
         *  \sa #isEmailErrorsEnabled().
         */
        std::string getErrMessageEmailTo() const;

        //! Allows to change <b>ErrMessageEmailTo</b> property
        /*!
         *  See #getErrMessageEmailTo() for more information.
         */
        void setErrMessageEmailTo(const char* str);

        //! Returns a value of <b>MonitorTestTimeout</b> parameter
        /*!
         *  A time-out can be applied to the test searches used to validate 
         *  a new database. If the test search on a new database does not produce 
         *  a valid result within the number of seconds specified by 
         *  <b>MonitorTestTimeout</b>, the problem is assumed to be with 
         *  the new database, and the exchange process is halted.
         *  Default value is <b>1200</b> (sec.).
         */
        int getMonitorTestTimeout() const;

        //! Allows to change <b>MonitorTestTimeout</b> property
        /*!
         *  See #getMonitorTestTimeout() for more information.
         */
        void setMonitorTestTimeout(const int value);

        //! Returns a value of <b>NTMonitorGroup</b> parameter
        /*!
         *  Under Windows NT, the Mascot service is generally run using the 
         *  <tt>Local System</tt> account. It has to create, write and read the memory 
         *  mapped files. The CGI scripts (such as <tt>nph-mascot.exe</tt>) 
         *  are run by the Web server, and will be run using a different 
         *  user name with different permissions from the service. These programs 
         *  also need to be able to read and write to these files. For example, 
         *  with the Microsoft Web server (IIS), a new user with the name 
         *  <tt>IUSR_<name_of_pc></tt> is created when the server is installed, and the 
         *  scripts are run using this user name. The installation program 
         *  sets these values appropriately. Other Web servers may use 
         *  different user names, with different permissions.<BR>
         *  <b>NTMonitorGroup</b> is used to define one of the groups 
         *  that the administrator belongs to. In most cases, 
         *  this can be left at the default of <tt>Administrators</tt>.<BR>
         *  <b>NTIUserGroup</b> is the name of one of the groups that 
         *  the user name used to run the CGI scripts belongs to. 
         */
        std::string getNTMonitorGroup() const;

        //! Allows to change <b>NTMonitorGroup</b> property
        /*!
         *  See #getNTMonitorGroup() for more information.
         */
        void setNTMonitorGroup(const char* str);

        //! Returns a value of <b>NTIUserGroup</b> parameter
        /*!
         *  Deafault value is <tt>Guests</tt>.
         * See #getNTMonitorGroup() for information on this parameter.
         */
        std::string getNTIUserGroup() const;

        //! Allows to change <b>NTIUserGroup</b> property
        /*!
         *  See #getNTMonitorGroup() for more information.
         */
        void setNTIUserGroup(const char* str);

        //! Returns a value of <b>UnixWebUserGroup</b> parameter
        /*!
         *  Entry <b>UnixWebUserGroup</b>, if present, will restrict access 
         *  to the files created by <tt>ms-monitor.exe</tt>, and hence improve system 
         *  security. <b>UnixWebUserGroup</b> parameter value is the number of the 
         *  group used by the web server to run CGI programs.
         *  With Apache, the group name will generally be <tt>nobody</tt> 
         *  (that corresponds to the default value <b>-1</b>), and you will need 
         *  to ascertain the group number from the group file. For other Web servers, 
         *  check the documentation that comes with the server to find out which 
         *  user name is used for running CGI programs.<BR>
         *  A value of <b>-2</b> can be used if the same user name is used to run Web 
         *  server scripts as runs <tt>ms-monitor.exe</tt>.
         *  In this case, the files created by <tt>ms-monitor.exe</tt> will not be world 
         *  accessible, and <tt>chown</tt> is not used on the files to change ownership.
         */
        int getUnixWebUserGroup() const;

        //! Allows to change <b>UnixWebUserGroup</b> property
        /*!
         *  See #getUnixWebUserGroup() for more information.
         */
        void setUnixWebUserGroup(const int value);

        //! Returns <b>TRUE</b> if <b>ForkForUnixApache</b> parameter is set to <b>1</b> in the file
        /*!
         *  If a user presses <tt>Stop</tt> or goes to another page in their browser 
         *  when a search is running, then the intended behaviour is that the 
         *  search should continue, and the user be emailed with their results. 
         *  However, when running some versions of Apache under Unix, 
         *  the search is terminated by Apache when the connection to the browser 
         *  is lost. To stop this from happening, set this value to <b>1</b>.
         *  Default value is <b>0</b>.
         */
        bool isForkForUnixApache() const;

        //! Allows to change <b>ForkForUnixApache</b> property
        /*!
         *  See #isForkForUnixApache() for more information.
         */
        void setForkForUnixApache(const bool value);

        //! Returns <b>SeparateLockMem</b> parameter value
        /*!
         *  Setting this value to <b>1</b> indicates that 
         *  <tt>ms-monitor.exe</tt> will run a separate program 
         *  (<tt>ms-lockmem.exe</tt>) that will lock the memory blocks. 
         *  A value greater than <b>1</b> specifies the block size in Mb. 
         *  For example, if there is a 1.5 Gb <tt>*.s00</tt> file, and this 
         *  parameter is set to <b>750</b>, then two instances of <tt>ms-lockmem.exe</tt>
         *  will be run.
         *  Default value is <b>0</b>.
         */
        int getSeparateLockMem() const;

        //! Allows to change <b>SeparateLockMem</b> property
        /*!
         *  See #getSeparateLockMem() for more information.
         */
        void setSeparateLockMem(const int value);

        //! Returns <b>FormVersion</b> parameter value
        /*!
         *  Mascot users may save search forms off-line, 
         *  or submit searches using scripts or private forms. 
         *  When the search engine is upgraded, there is the possibility 
         *  that old scripts or forms may contain invalid or obsolete 
         *  parameters. If a search is submitted to Mascot without a 
         *  version number, or if the version number is lower than that 
         *  specified by <b>FormVersion</b>, a warning will be included 
         *  in the results file and in the master results report.
         *  Default value is <b>'1.01'</b>.
         */
        std::string getFormVersion() const;

        //! Allows to change <b>FormVersion</b> property
        /*!
         *  See #getFormVersion() for more information.
         */
        void setFormVersion(const char* str);

        //! Returns <b>MaxSequenceLen</b> parameter value
        /*!
         *  This parameters defines the maximum length of a database entry in characters, 
         *  (bases for nucleic acids or residues for amino acids databases respectively).
         *  The default value is <b>50,000</b>.
         */
        int getMaxSequenceLen() const;

        //! Allows to change <b>MaxSequenceLen</b> property
        /*!
         *  See #getMaxSequenceLen() for more information.
         */
        void setMaxSequenceLen(const int value);

        //! Returns <b>MaxConcurrentSearches</b> parameter value
        /*!
         *  This parameter limits the maximum number of concurrent searches 
         *  so as to avoid overloading the Mascot server. Default is <b>10</b>.
         */
        int getMaxConcurrentSearches() const;

        //! Allows to change <b>MaxConcurrentSearches</b> property
        /*!
         *  See #getMaxConcurrentSearches() for more information.
         */
        void setMaxConcurrentSearches(const int value);

        //! Returns <b>MaxSearchesPerUser</b> parameter value
        /*!
         *  Sets the maximum number of concurrent searches from a single IP-address. 
         *  A default value of <b>0</b> means no limit.
         */
        int getMaxSearchesPerUser() const;

        //! Allows to change <b>MaxSearchesPerUser</b> property
        /*!
         *  See #getMaxSearchesPerUser() for more information.
         */
        void setMaxSearchesPerUser(const int value);

        //! Returns <b>CentroidWidth</b> parameter value
        /*!
         *  The width in Daltons of the sliding window used for re-centroiding profile data. 
         *  Default is <b>0.25</b> (Da).
         */
        double getCentroidWidth() const;

        //! Allows to change <b>CentroidWidth</b> property
        /*!
         *  See #getCentroidWidth() for more information.
         */
        void setCentroidWidth(const double value);

        //! Returns <b>MaxDescriptionLen</b> parameter value
        /*!
         *  Long FASTA title (description) lines will be truncated at 
         *  this number of characters. Default value is <b>100</b>.
         */
        int getMaxDescriptionLen() const;

        //! Allows to change <b>MaxDescriptionLen</b> property
        /*!
         *  See #getMaxDescriptionLen() for more information.
         */
        void setMaxDescriptionLen(const int value);

        //! Returns <b>MaxNumPeptides</b> parameter value
        /*!
         *  The maximum number of peptides which can be expected from 
         *  the enzymatic digest of a single entry. 
         *  The default is  <b>MaxSequenceLen / 4</b>.
         */
        int getMaxNumPeptides() const;

        //! Allows to change <b>MaxNumPeptides</b> property
        /*!
         *  See #getMaxNumPeptides() for more information.
         */
        void setMaxNumPeptides(const int value);

        //! Returns <b>Vmemory</b> parameter value
        /*!
         *  Alpha Tru64 only. When setting resource limits for address space, 
         *  this is the maximum number of bytes requested. 
         *  If a value of <b>–1</b> is specified, then the maximum system 
         *  value is requested. The default is <b>–1</b>.
         */
        INT64 getVmemory() const;

        //! Allows to change <b>Vmemory</b> property
        /*!
         *  See #getVmemory() for more information.
         */
        void setVmemory(const INT64 value);

        //! Returns number of values specified in <b>ReportNumberChoices</b> parameter
        /*!
         *  If present, <b>ReportNumberChoices</b> list will define the choices 
         *  provided in the search form <tt>Report top</tt> drop down list box.
         *  If not, this member will return zero.
         *  #getReportNumberChoice() member is used to retrieve individual elements
         *  of the list.
         */
        int getNumberOfReportNumberChoices() const;

        //! Deletes all <b>ReportNumberChoices</b> values from the list
        void clearReportNumberChoices();

        //! Returns an element of the <b>ReportNumberChoices</b> list by its number
        /*!
         *  The function returns <b>-1</b> in case of <tt>AUTO</tt> string.
         *  For more information on the <b>ReportNumberChoices</b> parameter see 
         *  #getNumberOfReportNumberChoices().
         *  \param index element number from 0 to (#getNumberOfReportNumberChoices()-1)
         *  \return an element of the list that corresponds to the given index
         */
        int getReportNumberChoice(const int index) const;

        //! Allows to set individual elements of the <b>ReportNumberChoices</b> list
        /*!
         *  See #getReportNumberChoice() for more information.
         */
        void appendReportNumberChoice(const int value);

        //! Returns <b>ReviewColWidth</b> parameter value by number
        /*!
         *  This gets the widths of the columns in <tt>ms-review.exe</tt><br> 
         *  End users can specify the widths in mascot.dat to suit their requirements,
         *  for example if they use long titles.
         *  \param index A value from the MS_REVIEW_COL enumeration
         *  \return column width for the given number
         *  Default value for all columns is <b>0</b>.
         *  See also #setReviewColWidth().
         */
        int getReviewColWidth(const MS_REVIEW_COL index) const;

        //! Allows to change <b>ReviewColWidth</b> property
        /*!
         *  This gets the widths of the columns in <tt>ms-review.exe</tt><br> 
         *  End users can specify the widths in mascot.dat to suit their requirements,
         *  for example if they use long titles.
         *  \param index A value from the MS_REVIEW_COL enumeration
         *  \param value Is the new width to set
         *  \return column width for the given number
         *  Default value for all columns is <b>0</b>.
         *  See also #getReviewColWidth().
         */
        void setReviewColWidth(const MS_REVIEW_COL index, const int value);

        //! Returns <b>proxy_server</b> parameter value
        /*!
         *  This parameter allows a proxy server to be defined. By default it is empty.
         *  Other parameters defining proxy server are: <b>proxy_username</b> 
         *  (see #getProxyUsername()), <b>proxy_password</b> (see
         *  #getProxyPassword()).
         */
        std::string getProxyServer() const;

        //! Allows to change <b>proxy_server</b> property
        /*!
         *  See #getProxyServer() for more information.
         */
        void setProxyServer(const char* str);

        //! Returns <b>proxy_username</b> parameter value
        /*!
         *  By default it is empty.
         *  See #getProxyServer() for explanations.
         */
        std::string getProxyUsername() const;

        //! Allows to change <b>proxy_username</b> property
        /*!
         *  See #getProxyServer() for explanations.
         */
        void setProxyUsername(const char* str);

        //! Returns <b>proxy_password</b> parameter value
        /*!
         *  By default it is empty.
         *  See #getProxyServer() for explanations.
         */
        std::string getProxyPassword() const;

        //! Allows to change <b>proxy_password</b> property
        /*!
         *  See #getProxyServer() for explanations.
         */
        void setProxyPassword(const char* str);

        //! Returns <b>MinPepLenInPepSummary</b> parameter value
        /*!
         *  In a Peptide Summary report, two proteins are reported as 
         *  distinct matches if the peptide matches to one protein are 
         *  not identical to or a sub-set of the peptide matches to the 
         *  other protein. Since matches to very short peptides are usually 
         *  random, peptides shorter than <b>MinPepLenInPepSummary</b> 
         *  are not considered in this comparison. Default value is <b>5</b>.
         *  \sa #getProteinsInResultsFile().
         */
        int getMinPepLenInPepSummary() const;

        //! Allows to change <b>MinPepLenInPepSummary</b> property
        /*!
         *  See #getMinPepLenInPepSummary() for more information.
         */
        void setMinPepLenInPepSummary(const int value);

        //! Returns <b>MaxQueries</b> parameter value
        /*!
         *  The maximum number of mass values (PMF) or MS/MS spectra (MIS) 
         *  allowed in a single search. Default is <b>10,000</b>.
         */
        int getMaxQueries() const;

        //! Allows to change <b>MaxQueries</b> property
        /*!
         *  See #getMaxQueries() for more information.
         */
        void setMaxQueries(const int value);

        //! Returns <b>TRUE</b> if <b>ShowSubsets</b> parameter is set to value greater than 0
        bool isShowSubsets() const;

        //! Sets the <b>ShowSubsets</b> property
        void setShowSubsets(const bool value);

        //! Returns the ShowSubsets value
        double getShowSubsets() const;

        //! Sets the <b>ShowSubsets</b> property
        void setShowSubsets(const double value);

        //! Returns <b>TRUE</b> if <b>RequireBoldRed</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  If <b>RequireBoldRed</b> parameter is set to <b>1</b>, 
         *  in a peptide summary report, only protein matches which have 
         *  one or more 'bold red' peptide matches will be listed. 
         *  That is, proteins that include at least one top ranking 
         *  peptide match that has not already appeared in the report. 
         *  Default is <b>0</b>.
         */
        bool isRequireBoldRed() const;

        //! Allows to change <b>RequireBoldRed</b> property
        /*!
         *  See #getRequireBoldRed() for more information.
         */
        void setRequireBoldRed(const bool value);

        //! Returns the <b>SigThreshold</b> parameter value
        /*!
         * Significance threshold used in results reports, default is 0.5.
         * Valid range is 0.1 to 1e-18. This global default can be overridden
         * on an individual report URL be appending &_sigthreshold=X, where
         * X is the significance threshold.<br/>
         * Default value is <b>0.05</b>.
         */
        double getSigThreshold() const;

        //! Set the <b>SigThreshold</b> property
        /*!
         *  See #getSigThreshold() for more information.
         */
        void setSigThreshold(const double value);

        //! Returns <b>MaxVarMods</b> parameter value
        /*!
         *  The maximum number of variable modifications allowed in a standard search
         *  Default is <b>9</b>
         */
        int getMaxVarMods() const;

        //! Allows to change <b>MaxVarMods</b> property
        /*!
         *  See #getMaxVarMods() for more information.
         */
        void setMaxVarMods(const int value);

        //! Returns <b>MaxEtVarMods</b> parameter value
        /*!
         *  The maximum number of variable modifications allowed in an error-tolerant search
         *  Default is <b>2</b>
         */
        int getMaxEtVarMods() const;

        //! Allows to change <b>MaxEtVarMods</b> property
        /*!
         *  See #getMaxEtVarMods() for more information.
         */
        void setMaxEtVarMods(const int value);

        //! Returns <b>ErrTolMaxAccessions</b> parameter value
        /*!
         *  The maximum number of entries allowed for a error tolerant search. 
         *  Default is <b>0</b>, meaning no limit.
         */
        int getErrTolMaxAccessions() const;

        //! Allows to change <b>ErrTolMaxAccessions</b> property
        /*!
         *  See #getErrTolMaxAccessions() for more information.
         */
        void setErrTolMaxAccessions(const int value);

        //! Returns <b>IgnoreIonsScoreBelow</b> parameter value
        /*!
         *  Any ions score lower than this value will be set to zero and ignored. 
         *  The parameter is a floating point number, default <b>0.0</b>. 
         */
        double getIgnoreIonsScoreBelow() const;

        //! Allows to change <b>IgnoreIonsScoreBelow</b> property
        /*!
         *  See #getIgnoreIonsScoreBelow() for more information.
         */
        void setIgnoreIonsScoreBelow(const double value);

        //! Returns <b>MonitorPidFile</b> parameter value
        /*!
         *  The name for the file which holds the process ID number 
         *  for <tt>ms-monitor.exe</tt>. Default is <tt>../bin/monitor.pid</tt>.
         */
        std::string getMonitorPidFile() const;

        //! Allows to change <b>MonitorPidFile</b> property
        /*!
         *  See #getMonitorPidFile() for more information.
         */
        void setMonitorPidFile(const char* str);

        //! Returns <b>TRUE</b> if <b>StoreModPermutations</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  If set to <b>0</b>, only the highest scoring permutation of variable 
         *  modifications for each unique peptide sequence is retained in the 
         *  list of the top 10 ions scores. If set to <b>1</b>, 
         *  then different permutations of variable modifications 
         *  are treated as independent matches, creating the possibility 
         *  that all 10 top ions scores correspond to the same primary sequence. 
         *  Default is <b>1</b>.
         */
        bool isStoreModPermutations() const;

        //! Allows to change <b>StoreModPermutations</b> property
        /*!
         *  See #isStoreModPermutations() for more information.
         */
        void setStoreModPermutations(const bool value);

        //! Returns <b>ProteinsInResultsFile</b> parameter value
        /*!
         *  Determines the number of protein title lines saved to each results file.
         *  Possible values are:
         *  <ul>
         *  <li><b>1</b> - as in Mascot 1.7 and earlier, only proteins that appear in the 
         *  Summary section will appear in the Proteins section</li>
         *  <li><b>2</b> - include proteins with at least one top ranking peptide match 
         *  to a peptide of length greater than 
         *  <b>MinPepLengthInPepSummary</b>(see #getMinPepLenInPepSummary())</li>
         *  <li><b>3</b>include all proteins</li>
         *  </ul>
         *  Default value is <b>2</b>.
         */
        int getProteinsInResultsFile() const;

        //! Allows to change <b>ProteinsInResultsFile</b> property
        /*!
         *  See #getProteinsInResultsFile() for more information.
         */
        void setProteinsInResultsFile(const int value);

        //! Returns value of <b>MascotMessage</b> parameter
        /*!
         *  Displays the specified message while a search is running. By default it is empty.
         */
        std::string getMascotMessage() const;

        //! Allows to change <b>MascotMessage</b> parameter value
        /*!
         * See #getMascotMessage() for more information.
         */
        void setMascotMessage(const char* str);

        //! Returns value of <b>SplitNumberOfQueries</b> parameter
        /*!
         *  Used in conjunction with <b>SplitDataFileSize</b> parameter.
         *  When large amount of spectra is submitted the input file is split
         *  into several chunks. Each chunk contains no more than <b>SplitNumberOfQueries</b>
         *  queries and no more than <b>SplitDataFileSize</b> bytes long.
         *  Default value for this parameter is <b>1000</b> (queries).
         */
        int getSplitNumberOfQueries() const;

        //! Allows to change <b>SplitNumberOfQueries</b> parameter value
        /*!
         * See #getSplitNumberOfQueries() for more information.
         */
        void setSplitNumberOfQueries(const int value);

        //! Returns value of <b>SplitDataFileSize</b> parameter
        /*!
         *  Default value for this parameter is <b>10000000</b> (bytes).
         *  See #getSplitNumberOfQueries() for explanations.
         */
        int getSplitDataFileSize() const;

        //! Allows to change <b>SplitDataFileSize</b> parameter value
        /*!
         * See #getSplitNumberOfQueries() for more information.
         */
        void setSplitDataFileSize(const int value);

        //! Returns <b>TRUE</b> if <b>MoveOldDbToOldDir</b> parameter is set to <b>1</b>
        /*!
         *  After a successful database swap, the old fasta file and old reference 
         *  file are moved to the <tt>../old</tt> directory unless this is set to <b>0</b>
         *  in which case the files are simply deleted.
         *  Default value is <b>1</b>.
         */
        bool isMoveOldDbToOldDir() const;

        //! Allows to change <b>MoveOldDbToOldDir</b> parameter value
        /*!
         * See #isMoveOldDbToOldDir() for more information.
         */
        void setMoveOldDbToOldDir(const bool value);

        //! Returns <b>TRUE</b> if <b>RemoveOldIndexFiles</b> parameter is set to <b>1</b>
        /*!
         *  After a successful database swap, the compressed files in the current 
         *  directory are deleted unless this is set to <b>0</b>.
         *  Default value is <b>1</b>.
         */
        bool isRemoveOldIndexFiles() const;

        //! Allows to change <b>RemoveOldIndexFiles</b> parameter value
        /*!
         * See #isRemoveOldIndexFiles() for more information.
         */
        void setRemoveOldIndexFiles(const bool value);

        //! Returns <b>FeatureTableLength</b> parameter value
        /*!
         *  If a nucleic acid sequence is longer than <b>FeatureTableLength</b> 
         *  parameter value, the protein view report will automatically 
         *  switch to feature table mode and output the matches as a 
         *  GenBank feature table. Default value is <b>30,000</b>.
         */
        int getFeatureTableLength() const;

        //! Allows to change <b>FeatureTableLength</b> parameter value
        /*!
         * See #getFeatureTableLength() for more information.
         */
        void setFeatureTableLength(const int value);

        //! Returns <b>FeatureTableMinScore</b> parameter value
        /*!
         *  By default, only matches with significant scores (p < 0.05) 
         *  are output. A different score threshold can be specified using 
         *  the parameter <b>FeatureTableMinScore</b>. 
         *  It has no default value.
         */
        double getFeatureTableMinScore() const;

        //! Allows to change <b>FeatureTableMinScore</b> parameter value
        /*!
         * See #getFeatureTableMinScore() for more information.
         */
        void setFeatureTableMinScore(const double value);

        //! Returns the name of ICAT light modification
        /*!
         *  Parameter <b>ICATLight</b> gives the name of ICAT light modification. Full
         *  modification definition can be found in <tt>mod_file</tt>.
         *  Default value is "ICAT_light".<BR>
         *  \sa setICATLight()
         */
        std::string getICATLight() const;

        //! Sets <b>ICATLight</b> parameter value
        /*!
         *  See getICATLight() for more detailed information.
         */
        void setICATLight(const char* modName);

        //! Returns the name of ICAT heavy modification
        /*!
         *  Parameter <b>ICATHeavy</b> gives the name of ICAT heavy modification. Full
         *  modification definition can be found in <tt>mod_file</tt>.
         *  Default value is "ICAT_heavy".<BR>
         *  \sa setICATHeavy()
         */
        std::string getICATHeavy() const;

        //! Sets <b>ICATHeavy</b> parameter value
        /*!
         *  See getICATHeavy() for more detailed information.
         */
        void setICATHeavy(const char* modName);

        //! Returns the name of ICAT filter string
        /*!
         *  Parameter <b>ICATFilter</b> gives a residue that ICAT modifications apply to.
         *  Default value is "C".<BR>
         *  \sa setICATFilter()
         */
        std::string getICATFilter() const;

        //! Sets <b>ICATFilter</b> parameter value
        /*!
         *  See getICATFilter() for more detailed information.
         */
        void setICATFilter(const char* filterString);

        //! Returns the name of the quantitation method corresponding to ICAT
        /*!
         *  Parameter <b>ICATQuantitationMethod</b> gives a name of quantitation method to use for ICAT.
         *  Default value is "ICAT".<BR>
         *  \sa setICATQuantitationMethod()
         */
        std::string getICATQuantitationMethod() const;

        //! Sets <b>ICATQuantitationMethod</b> parameter value
        /*!
         *  See getICATQuantitationMethod() for more detailed information.
         */
        void setICATQuantitationMethod(const char* methodName);

        //! Returns the number of databases that shouldn't be checked for duplicate accessions
        /*!
         *  When fasta files are compressed, they are checked to ensure that there are no
         *  duplicate accession strings. For a large EST database with many millions of
         *  entries, this can take up a lot of memory. Adding a database to the list
         *  of values for the <b>IgnoreDupeAccessions</b> entry in mascot.dat 
         *  prevents this database from being tested.
         */
        unsigned int getNumberOfIgnoreDupeAccessions() const;


        //! Clears the list of databases that shouldn't be checked for duplicate accessions
        /*!
         *  When fasta files are compressed, they are checked to ensure that there are no
         *  duplicate accession strings. For a large EST database with many millions of
         *  entries, this can take up a lot of memory. Adding a database to the list
         *  of values for the <b>IgnoreDupeAccessions</b> entry in mascot.dat 
         *  prevents this database from being tested.
         */
        void clearIgnoreDupeAccessions();

        //! Returns one of the list databases that shouldn't be checked for duplicate accessions
        /*!
         *  When fasta files are compressed, they are checked to ensure that there are no
         *  duplicate accession strings. For a large EST database with many millions of
         *  entries, this can take up a lot of memory. Adding a database to the list
         *  of values for the <b>IgnoreDupeAccessions</b> entry in mascot.dat 
         *  prevents this database from being tested.
         */
        std::string getIgnoreDupeAccession(const unsigned int index) const;

        //! Appends a database to the list databases that shouldn't be checked for duplicate accessions
        /*!
         *  When fasta files are compressed, they are checked to ensure that there are no
         *  duplicate accession strings. For a large EST database with many millions of
         *  entries, this can take up a lot of memory. Adding a database to the list
         *  of values for the <b>IgnoreDupeAccessions</b> entry in mascot.dat 
         *  prevents this database from being tested.
         */
        void appendIgnoreDupeAccession(const std::string value);

        //! Checks to see if the passed database name is in the list databases that shouldn't be checked for duplicate accessions
        /*!
         *  When fasta files are compressed, they are checked to ensure that there are no
         *  duplicate accession strings. For a large EST database with many millions of
         *  entries, this can take up a lot of memory. Adding a database to the list
         *  of values for the <b>IgnoreDupeAccessions</b> entry in mascot.dat 
         *  prevents this database from being tested.
         */
        bool isInIgnoreDupeAccessionList(const std::string value) const;

        //! Specify the Unix permissions for the ‘daily’ result file directories and the logs directory. 
        /*! For example, 775 makes each directory world readable but not writeable.
         *  This option provides more fine grained control than UnixWebUserGroup
         */
        int getUnixDirPerm() const;


        //! Specify the Unix permissions for the ‘daily’ result file directories and the logs directory. 
        /*! For example, 775 makes each directory world readable but not writeable.
         *  This option provides more fine grained control than UnixWebUserGroup
         */
        void setUnixDirPerm(const int value);

        //! Returns true if the UnixDirPerm value is defined in mascot.dat
        bool isUnixDirPermDefined() const;


        //! Gets the number of queries at which the protein scoring switches to 'MudPIT' mode
        int getMudpit() const;

        //! Sets the number of queries at which the protein scoring switches to 'MudPIT' mode
        void setMudpit(const int value);

        //! Gets the first <b>PrecursorCutOut</b> parameter value
        /*!
         *  This parameter defines boundaries for cut-out window for precursor m/z value. 
         *  It consists of two numbers - the lower limit and the upper limit for precursor m/z window. If both 
         *  numbers are the same (i.e. if the window is of zero-length) Mascot uses a "smart" mechanism for filtering out
         *  peptide m/z from ms-spectra. Default value for <b>PrecursorCutOut</b> parameter is "-1, -1".
         */
        double getPrecursorCutOutLowerLimit() const;

        //! Sets the first <b>PrecursorCutOut</b> parameter value
        void setPrecursorCutOutLowerLimit(const double value);


        //! Gets the second <b>PrecursorCutOut</b> parameter value
        /*!
         *  See #getPrecursorCutOutLowerLimit()-method for more information.
         */
        double getPrecursorCutOutUpperLimit() const;

        //! Sets the second <b>PrecursorCutOut</b> parameter value
        void setPrecursorCutOutUpperLimit(const double value);

        //! Returns <b>TRUE</b> if <b>AutoSelectCharge</b> parameter is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  When this parameter is set to <b>1</b> only the charge state that gives the best ion score
         *  is reported for each query. Otherwise, each query will be transformed and split 
         *  into several queries (one for each possible charge state). Default value is <b>1</b>.
         */
        bool getAutoSelectCharge() const;

        //! Sets <b>AutoSelectCharge</b> parameter value
        void setAutoSelectCharge(const bool value);

        //! Returns value of <b>TaxBrowserURL</b> parameter
        /*!
         *  The URL used in reports to retrieve taxonomy information for a Protein View report. 
         */
        std::string getTaxBrowserUrl() const;

        //! Sets <b>TaxBrowserURL</b> parameter value
        void setTaxBrowserUrl(const std::string value);


        //! Returns value of <b>MinPepLenInSearch</b> parameter
        int getMinPepLenInSearch() const;

        //! Sets <b>MinPepLenInSearch</b> parameter value
        void setMinPepLenInSearch(const int value);


        //! Returns value of <b>MaxPepNumVarMods</b> parameter
        int getMaxPepNumVarMods() const;

        //! Sets <b>MaxPepNumVarMods</b> parameter value
        void setMaxPepNumVarMods(const int value);


        //! Returns value of <b>IteratePMFIntensities</b> parameter
        bool getIteratePMFIntensities() const;

        //! Sets <b>IteratePMFIntensities</b> parameter value
        void setIteratePMFIntensities(const bool value);

        //! Returns value of <b>ResultsFileFormatVersion</b> parameter
        std::string getResultsFileFormatVersion() const;

        //! Sets <b>ResultsFileFormatVersion</b> parameter value
        void setResultsFileFormatVersion(const char *value);

        //! Returns the default sort order for unassigned queries
        std::string getSortUnassigned() const;

        //! Sets the default sort order for unassigned queries
        void setSortUnassigned(const std::string newValue);

        //! Returns the number of queries at which the default report switches to the select summary
        int getSelectSwitch() const;

        //! Sets the number of queries at which the default report switches to the select summary
        void setSelectSwitch(const int value);

        //! Returns the value at which protein scoring switches from standard to mudpit scoring
        double getMudpitSwitch() const;

        //! Set the value at which protein scoring switches from standard to mudpit scoring
        void setMudpitSwitch(const double value);

        //////////////// Undocumented members go here //////////////////

        //! Returns <b>GetSeqBlastAccession</b> parameter value
        const ms_blastaccession* getGetSeqBlastAccession() const;

        //! Allows to change <b>GetSeqBlastAccession</b> parameter value
        void setGetSeqBlastAccession(const ms_blastaccession* value);

        // MatrixScience internal usage only
        int getMinPeakIteration() const;

        // MatrixScience internal usage only
        void setMinPeakIteration(const int value);

        // MatrixScience internal usage only
        bool isEncryptURL() const;

        // MatrixScience internal usage only
        void setEncryptURL(const bool value);

    private:
        bool        sectionAvailable_;

        bool        saveLastQueryAsc_;
        bool        saveEveryLastQueryAsc_;
        std::string lastQueryAscFile_;

        std::string errorLogFile_;
        std::string searchLogFile_;
        std::string monitorLogFile_;
        std::string interFileBasePath_;
        std::string interFileRelPath_;

        std::string mascotCmdLine_;
        std::string testDirectory_;
        std::string mascotControlFile_;
        std::string mascNodeCtrlFile_;
        std::string mascotJobIdFile_;
        std::string getSeqJobIdFile_;
        int         uniqueJobStartNumber_;

        std::string resultsPerlScript_;
        std::string noResultsScript_;
        std::string resultsFullURL_;
        std::string logoImageFile_;

        int         massDecimalPlaces_;
        int         ionsDecimalPlaces_;
        int         intensitySigFigs_;

        bool        emailUsersEnabled_;
        bool        emailErrorsEnabled_;
        int         mailTransport_;
        std::string emailService_;
        std::string emailPassword_;
        std::string emailProfile_;
        std::string sendmailPath_;
        std::string emailFromUser_;
        std::string emailFromTextName_;
        int         emailTimeOutPeriod_;
        int         monitorEmailCheckFreq_;
        std::string mailTempFile_;
        std::string errMessageEmailTo_;

        int         monitorTestTimeout_;
        std::string NTMonitorGroup_;
        std::string NTIUserGroup_;
        int         unixWebUserGroup_;
        bool        forkForUnixApache_;
        int         separateLockMem_;

        std::string formVersion_;
        int         maxSequenceLen_;
        int         maxConcurrentSearches_;
        int         maxSearchesPerUser_;
        double       centroidWidth_;
        int         maxDescriptionLen_;
        int         maxNumPeptides_;
        INT64       vmemory_;

        std::vector< int > reportNumberChoices_;

        int         reviewColWidths_[MS_REVIEW_COL____LAST___];

        std::string proxy_server_;
        std::string proxy_username_;
        std::string proxy_password_;

        int         maxVarMods_;
        int         maxEtVarMods_;
        int         errTolMaxAccessions_;

        int         minPepLenInPepSummary_;
        int         maxPepNumVarMods_;
        int         maxQueries_;
        double      showSubsets_;
        bool        requireBoldRed_;
        double      sigThreshold_;
        double      ignoreIonsScoreBelow_;

        std::string monitorPidFile_;
        bool        storeModPermutations_;
        int         proteinsInResultsFile_;
        std::string mascotMessage_;
        int         splitNumberOfQueries_;
        int         splitDataFileSize_;
        bool        moveOldDbToOldDir_;
        bool        removeOldIndexFiles_;

        int         featureTableLength_;
        double      featureTableMinScore_;

        std::string ICATLight_;
        std::string ICATHeavy_;
        std::string ICATFilter_;
        std::string ICATQuantitationMethod_;

        int         UnixDirPerm_;
        bool        UnixDirPermDefined_;
        int         mudpit_;

        typedef std::vector<std::string> t_dupeAccs;
        t_dupeAccs ignoreDupeAccessions_;

        double      precursorCutOutLowerLimit_;
        double      precursorCutOutUpperLimit_;

        bool        autoSelectCharge_;
        std::string taxBrowserUrl_;
        int         minPepLenInSearch_;
        bool        iteratePMFIntensities_;
        std::string resultsFileFormatVersion_;
        std::string sortUnassigned_;
        int         selectSwitch_;
        double      mudpitSwitch_;

        ms_blastaccession   getSeqBlastAccession_;
        int         minPeakIteration_;
        bool        encryptURL_;
    }; // class options
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_MASCOTOPTIONS_HPP

/*------------------------------- End of File -------------------------------*/
