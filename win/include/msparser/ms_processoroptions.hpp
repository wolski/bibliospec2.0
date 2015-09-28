/*
##############################################################################
# file: ms_processoroptions.hpp                                              #
# 'msparser' toolkit                                                         #
# Represents parameters of "Processors" section of "mascot.dat"-file         #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_processoroptions.hpp    $ #
#     $Author: davidc $ #
#       $Date: 2007-01-16 13:16:11 $ #
#   $Revision: 1.5 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_PROCESSOROPTIONS_HPP
#define MS_PROCESSOROPTIONS_HPP

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

    //! Represent a single dabatase entry in <tt>Processors</tt> section
    class MS_MASCOTRESFILE_API ms_dbprocessors
    {
        friend class ms_datfile;
        friend class ms_processoroptions;
    public:
        //! Default constructor
        ms_dbprocessors();

        //! Copying constructor
        ms_dbprocessors(const ms_dbprocessors& src);

        //! Destructor
        ~ms_dbprocessors();

        //! Initialises the instance with default values
        void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_dbprocessors* right);

        //! Assinment operator for C++ client applications
        ms_dbprocessors& operator=(const ms_dbprocessors& right);

        //! Return database name for the entry
        std::string getName() const;

        //! Allows to change the database name for the entry
        void setName(const char * str);

        //! Returns a number of values (threads/processor IDs) specified for the database
        int getNumberOfThreads() const;

        //! Deletes all threads (processor IDs) from the entry
        void clearThreads();

        //! Returns a single processor ID by its index from <b>0</b> to (#getNumberOfThreads()-1)
        int getThreadProcessorID(const int threadIndex) const;

        //! Adds a new thread with a processor ID to the list
        void appendThreadProcessorID(const int proccessorID);
    private:
        std::string         name_;
        std::vector<int>    threads_;

        std::string getStringValue() const;
    }; // class ms_dbprocessors

    //! An instance of this class represents all the parameters specified in <b>Processors</b>-section of <tt>mascot.dat</tt>
    /*!
     *  An instance of this class is created and polulated in ms_datfile-class. 
     *  It can also be created separately and initialized with default values. 
     *  One can create an instance of the class or copy from another instance 
     *  in order to pass it then as an parameters-containing object.
     *  For detailed information on any of the options please consult Mascot manual.<BR>
     *  Most of the parameters here can be accessed using dabatase number which corresponds
     *  to databases indices in ms_databases-class.
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
    class MS_MASCOTRESFILE_API ms_processoroptions : public ms_customproperty
    {
        friend class ms_datfile;
    public:

        //! Default constructor
        ms_processoroptions();

        //! Copying constructor
        ms_processoroptions(const ms_processoroptions& src);

        //! Destructor
        virtual ~ms_processoroptions();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_processoroptions* right);

        //! Assignment operator for C++ client applications
        ms_processoroptions& operator=(const ms_processoroptions& right);

        //! Tells whether the section has been actually read from the file
        /*!
         *  By default a <tt>Processors</tt> section is unavailable until it has 
         *  been set to a different state.
         */
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. should it be saved in a file
        void setSectionAvailable(const bool value);

        //! Returns a number of processors specified in the <b>ProcessorSet</b> parameter
        /*!
         *  <b>ProcessorSet</b> line specifies the complete set of processors to be used. 
         *  The number in this list must be less than or equal to 
         *  the number processors licensed, or the system will not run.
         *  There can be up to 64 CPUs specified in the bit mask. Each bit can be 
         *  set either to <b>1</b> or <b>0</b> for a listed CPU or not listed one respectively.
         *  Following this parameter, the processors to be used for each database are 
         *  specified in <tt>mascot.dat</tt>. These numbers must be a subset 
         *  of the numbers in the <b>ProcessorSet</b>, and there must be the 
         *  same number of values as the number of threads specified earlier in 
         *  the database section. See other parameters: #getWhichForDB(), 
         *  #getDBValSetInMascotDat().
         *  By default the list is empty.
         */
        int getNumberOfProcessors() const;

        //! Deletes all processors from the <b>ProcessorSet</b> list
        void clearProcessors();

        //! Returns a single processor ID by its index
        int getProcessor(const int index) const;

        //! Adds a new processor to the list for <b>ProcessorSet</b> parameter
        void appendProcessor(const int processorID);

        //! Returns number of individual database entries in the section
        /*!
         *  Each of the lines corresponds to a certain database. 
         *  All CPU numbers are listed 
         *  in the same database entry and correspond to threads. 
         *  A number of CPUs listed for the database must be the same as number 
         *  of threads specified for the database in <tt>Databases</tt> section of 
         *  <tt>mascot.dat</tt> file. Also, all CPUs listed here must belong to the set of
         *  CPUs specified by <b>ProcessorSet</b> parameter.<BR>
         *  By default the list of databases is empty.
         */
        int getNumberOfDatabases() const;

        //! Returns an individual database entry
        const ms_dbprocessors* getDatabase(const int index) const;

        //! Deletes all database entries
        void clearDatabases();

        //! Adds a new database to the list
        void appendDatabase(const ms_dbprocessors* db);

    private:
        bool                            sectionAvailable_;
        std::vector<int>                processors_;
        std::vector<ms_dbprocessors*>   dbs_;
    }; // class ms_processoroptions
    /** @} */ // end of config_group
} // namespace matrix_science;

#endif // MS_PROCESSOROPTIONS_HPP

/*------------------------------- End of File -------------------------------*/
