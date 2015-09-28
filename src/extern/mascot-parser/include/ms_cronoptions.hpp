/*
##############################################################################
# file: ms_cronoptions.hpp                                                   #
# 'msparser' toolkit                                                         #
# Represents parameters from cron-section of 'mascot.dat' configuration file #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_cronoptions.hpp         $ #
#     $Author: davidc $ #
#       $Date: 2007-01-16 13:16:11 $ #
#   $Revision: 1.5 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_CRONOPTIONS_HPP
#define MS_CRONOPTIONS_HPP

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

    //! Represent a single entry from <tt>cron</tt>-section of <tt>mascot.dat</tt> file
    /*!
     *  A number of instances of this class can be held in ms_cronoptions-class.
     */
    class MS_MASCOTRESFILE_API ms_cronjob
    {
        friend class ms_datfile;
        friend class ms_cronoptions;

    public:
        //! Default constructor
        ms_cronjob();

        //! Copying constructor
        ms_cronjob(const ms_cronjob& src);

        //! Default destructor
        virtual ~ms_cronjob();

        //! Initialises the instance with default values
        void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_cronjob* right);

        //! Assignment operator for C++ client applications
        ms_cronjob& operator=(const ms_cronjob& right);

        //! Returns <b>TRUE</b> for a valid minute number, <b>FALSE</b> otherwise
        /*!
         *  A cron job entry in the configuration file specifies time to run a command.
         *  Use this function to retrieve 'minute' part of this specification.
         *  \param min a minute number (0..59)
         *  By default <b>FALSE</b> is returned for all minutes.
         */
        bool isMinute(const int min) const;

        //! Allows to set valid/illegal minutes
        /*!
         *  \param min a minute number (0..59)
         *  \param value a new boolean flag for the given minute
         *  \sa #isMinute()
         */
        void setMinute(const int min, const bool value);

        //! Returns <b>TRUE</b> for a valid hour number, <b>FALSE</b> otherwise
        /*!
         *  A cron job entry in the configuration file specifies time to run a command.
         *  Use this function to retrieve 'hour' part of this specification.
         *  \param hour an hour number (0..23)
         *  By default <b>FALSE</b> is returned for all hours.
         */
        bool isHour(const int hour) const;

        //! Allows to set valid/illegal hours
        /*!
         *  \param hour an hour number (0..23)
         *  \param value a new boolean flag for the given hour
         *  \sa #isHour()
         */
        void setHour(const int hour, const bool value);

        //! Returns <b>TRUE</b> for a valid day number, <b>FALSE</b> otherwise
        /*!
         *  A cron job entry in the configuration file specifies time to run a command.
         *  Use this function to retrieve 'day' part of this specification.
         *  \param day a day number (1..31)
         *  By default <b>FALSE</b> is returned for all days.
         */
        bool isDayOfMonth(const int day) const;

        //! Allows to set valid/illegal days
        /*!
         *  \param day an hour number (1..31)
         *  \param value a new boolean flag for the given day
         *  \sa #isDayOfMonth()
         */
        void setDayOfMonth(const int day, const bool value);

        //! Returns <b>TRUE</b> for a valid month number, <b>FALSE</b> otherwise
        /*!
         *  A cron job entry in the configuration file specifies time to run a command.
         *  Use this function to retrieve 'month' part of this specification.
         *  \param month a month number (1..12)
         *  By default <b>FALSE</b> is returned for all months.
         */
        bool isMonthOfYear(const int month) const;

        //! Allows to set valid/illegal months
        /*!
         *  \param month a month number (1..12)
         *  \param value a new boolean flag for the given month
         *  \sa #isMonthOfYear()
         */
        void setMonthOfYear(const int month, const bool value);

        //! Returns <b>TRUE</b> for a valid day of week number, <b>FALSE</b> otherwise
        /*!
         *  A cron job entry in the configuration file specifies time to run a command.
         *  Use this function to retrieve 'day of week' part of this specification.
         *  \param day a day of week number (0..6, 0=Sunday)
         *  By default <b>FALSE</b> is returned for all days.
         */
        bool isDayOfWeek(const int day) const;

        //! Allows to set valid/illegal days
        /*!
         *  \param day a day of week number (0..6, 0=Sunday)
         *  \param value a new boolean flag for the given day
         *  \sa #isDayOfWeek()
         */
        void setDayOfWeek(const int day, const bool value);

        //! Returns a string that is executed by the shell (command prompt) at the specified times
        /*!
         *  By default it is empty.
         */
        std::string getCommandStr() const;

        //! Allows to change is a string that is executed by the shell (command prompt) at the specified times
        void setCommandStr(const char* str);

        // used internally
        std::string getStringValue() const;

    private:

        bool  minute_[60];            /* index of 0-59 - Value of true means do it  */
        bool  hour_[24];              /* index of 0-23 - Value of true means do it  */
        bool  dayOfMonth_[32];        /* index of 1-31 - Value of true means do it  */
        bool  monthOfYear_[13];       /* index of 1-12 - Value of true means do it  */
        bool  dayOfWeek_[7];          /* index of  0-6 (0 = Sunday)                 */
        std::string szCommand_;

        void setCustomString();
        std::string customString_;
    }; // class ms_cronjob

    //! Contains parameters from <tt>cron</tt>-section of <tt>mascot.dat</tt> file
    /*!
     *  On Unix systems, <tt>cron</tt> can be used to automate routine procedures 
     *  such as the overnight updating of sequence database files. 
     *  Windows 2000 includes an equivalent utility called <tt>Scheduled Tasks</tt>. 
     *  Windows NT does not include a suitable utility so, as a convenience to 
     *  Windows NT users, <tt>ms_monitor.exe</tt> can be configured to emulate 
     *  the functionality of cron.<BR>
     *  For detailed information on any of the cron-parameters please 
     *  consult Mascot manual.<BR>
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
    class MS_MASCOTRESFILE_API ms_cronoptions: public ms_customproperty
    {
        friend class ms_datfile;
    public:

        //! Default constructor
        ms_cronoptions();

        //! Copying constructor
        ms_cronoptions(const ms_cronoptions& src);

        //! Destructor
        virtual ~ms_cronoptions();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_cronoptions* right);

        //! Assignment operator for C++ client applications
        ms_cronoptions& operator=(const ms_cronoptions& right);

        //! Returns <b>TRUE</b> if the section was actually read from a file
        bool isSectionAvailable() const;

        //! Changes availability of the section, so, it should be or shouldn't be stored in a file
        void setSectionAvailable(const bool value);

        //! Returns <b>TRUE</b> if parameter <b>CronEnabled</b> is set to <b>1</b> and <b>FALSE</b> otherwise
        /*!
         *  First parameter in <tt>cron</tt>-section of <tt>mascot.dat</tt> 
         *  is always <b>CronEnabled</b> parameter. The rest is a list of cron-jobs. 
         *  See #getNumberOfCronJobs() for more information about them.<BR>
         *  <b>CronEnable</b> should be set to <b>1</b> to enable cron functionality, 
         *  <b>0</b> to disable. By default it is disabled.
         */
        bool isCronEnabled() const;

        //! Allows to change <b>CronEnabled</b> parameter value
        /*!
         *  See #isCronEnabled() for more information.
         */
        void setCronEnabled(const bool value);

        //! Returns a total number of cron-jobs configured
        /*!
         *  Every line (except the first one) in <tt>cron</tt>-section represents a
         *  cron-job. This function returns a number of such lines (except commented out).
         */
        int getNumberOfCronJobs() const;

        //! Returns an instance of class describing a single cron-job by its number
        /*!
         *  All cron-jobs assigned consecutive ID numbers (0 based). Use them in order
         *  to retrieve a particular cron-job definition.
         *  \param idx a number from 0 to (#getNumberOfCronJobs()-1)
         *  \return instance of ms_cronjob-class describing the correspondent entry 
         */
        const ms_cronjob* getCronJob(const int idx) const;

        //! Deletes all cron-job entries
        void clearCronJobs();

        //! Allows to add a new job entry at the end of the list
        void appendCronJob(const ms_cronjob* job);

    private:
        bool                        sectionAvailable_;
        bool                        cronEnabled_;
        std::vector< ms_cronjob* >  cronJobArray_;
    }; // ms_cronoptions
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_CRONOPTIONS_HPP

/*------------------------------- End of File -------------------------------*/
