/*
##############################################################################
# File: ms_fragmentvector.hpp                                                #
# Mascot Parser toolkit                                                      #
# Encapsulates a list (vector) of single fragment ions                       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Source: /vol/cvsroot/parser/inc/ms_fragmentvector.hpp,v $
#    $Author: davidc $ 
#      $Date: 2006-01-20 15:26:52 $ 
#  $Revision: 1.1 $
##############################################################################
*/

#if !defined(ms_fragmentvector_INCLUDED_)
#define ms_fragmentvector_INCLUDED_

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
#include <set>
#include <vector>
#include <map>


namespace matrix_science {
    /** @addtogroup tools_group
     *  
     *  @{
     */

    //! Class for holding a list of ms_fragment objects
    /*!
     * This class is required for the Perl and Java interfaces to Mascot Parser.<br>
     * Use this class when calling calcFragments() - simply create an
     * ms_fragmentvector object, and pass this to ms_aahelper::calcFragments()<br>
     * Then, iterate through the ms_fragment objects by calling 
     * ms_fragmentvector::getFragmentByNumber.<br>
     * C++ users will generally just want to use a std::vector<ms_fragment> 
     * as this is easier. Pass an empty std::vector<ms_fragment> to 
     * ms_aahelper::calcFragments()<br><br>
     * See the example code:
     * <ul>
     * <li> Perl: \link tools_aahelper.pl tools_aahelper.pl\endlink </li>
     * <li> Java: \link tools_aahelper.java tools_aahelper.java\endlink </li>
     * <li> C++:  \link tools_aahelper.cpp tools_aahelper.cpp\endlink </li>
     * </ul>
     */

    class MS_MASCOTRESFILE_API ms_fragmentvector
    {
    public:
        //! Default constructor
        ms_fragmentvector();

        //! Copying constructor
        ms_fragmentvector(const ms_fragmentvector& src);

        //! Destructor
        virtual ~ms_fragmentvector();

        //! Copies all content from another instance
        void copyFrom(const ms_fragmentvector* right);

        //! C++ style assignment operator
        ms_fragmentvector& operator=(const ms_fragmentvector& right);

        //! Returns a number of fragments currently held in the vector
        int getNumberOfFragments() const;

        //! Deletes all fragments from the vector
        void clearFragments();

        //! Adds a new fragment to the end of the vector
        void appendFragment(const ms_fragment * item);

        //! Return a fragment object by its number
        const ms_fragment * getFragmentByNumber(const unsigned int numFrag) const;

        typedef std::vector<ms_fragment> frag_vector;

        //! Return a pointer to the STL vector of ms_fragment objects
        frag_vector * getVector();

    private:
        frag_vector  entries_;
    }; // class ms_fragmentvector
    /** @} */ // end of tools_group
} // matrix_science

#endif // !defined(ms_fragmentvector_INCLUDED_)
