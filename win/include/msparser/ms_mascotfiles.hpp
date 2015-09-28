/*
##############################################################################
# file: ms_mascotfiles.hpp                                                   #
# 'msparser' toolkit                                                         #
# Contains pathes to some configuration files                                #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_mascotfiles.hpp         $ #
#     $Author: davidc $ #
#       $Date: 2007-01-16 13:16:11 $ #
#   $Revision: 1.5 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTFILES_HPP
#define MS_MASCOTFILES_HPP

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


namespace matrix_science {
    /** @addtogroup config_group
     *  
     *  @{
     */

    //! An instance of this class contains configuration files paths (except for <tt>mascot.dat</tt>)
    /*!
     *  An instance of this class is normally created and populated within ms_datfile class.
     *  It contains paths for <tt>mod_file</tt>, <tt>enzymes</tt>, 
     *  <tt>frequencies</tt> and<tt>nodelist.txt</tt>.
     */
    class MS_MASCOTRESFILE_API ms_mascotfiles: public ms_customproperty
    {
        friend class ms_datfile;
    public:

        //! Default constructor
        ms_mascotfiles(); 

        //! Copying constructor
        ms_mascotfiles(const ms_mascotfiles& src);

        //! Destructor
        virtual ~ms_mascotfiles();

        //! Initialises the instance with default values
        virtual void defaultValues();

        //! Can be used to create a copy of another object
        void copyFrom(const ms_mascotfiles* right);

        //! Assignment operator for C++ client application
        ms_mascotfiles& operator=(const ms_mascotfiles& right);

        //! Returns path for <tt>mod_file</tt> file
        std::string getModifications() const;

        //! Allows to change path for <tt>mod_file</tt> file
        void setModifications(const char* filename);

        //! Returns path for <tt>enzymes</tt> file
        std::string getEnzymes() const;

        //! Allows to change path for <tt>enzymes</tt> file
        void setEnzymes(const char* filename);

        //! Returns path for <tt>freqs.dat</tt> file
        std::string getFrequencies() const;

        //! Allows to change path for <tt>freqs.dat</tt> file
        void setFrequencies(const char* filename);

        //! Returns path for <tt>nodelist.txt</tt> file
        std::string getNodeListFile() const;

        //! Allows to change path for <tt>nodelist.txt</tt> file
        void setNodeListFile(const char* filename);

    private:
        std::string mods_;
        std::string enzymes_;
        std::string freqs_;
        std::string nodeListFile_;
    };// class ms_mascotfiles
    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_MASCOTFILES_HPP

/*------------------------------- End of File -------------------------------*/
