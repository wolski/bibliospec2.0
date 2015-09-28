/*
##############################################################################
# File: ms_fragment.hpp                                                      #
# Mascot Parser toolkit                                                      #
# Encapsulates a single fragment ion                                         #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Source: /export/cvsroot/parser/inc/ms_fragment.hpp,v $
#    $Author: davidc $ 
#      $Date: 2006-12-20 14:23:55 $ 
#  $Revision: 1.3 $
##############################################################################
*/


#if !defined(ms_fragment_INCLUDED_)
#define ms_fragment_INCLUDED_

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

    //! Class representing a single peptide fragment
    class MS_MASCOTRESFILE_API ms_fragment
    {
    public:

        //! Default constructor (don't use it)
        ms_fragment();

        //! Copying constructor
        ms_fragment(const ms_fragment &src);

        //! Assignment operator for c++ programs
        ms_fragment& operator=(const ms_fragment& right);

        //! Copies the whole content of another instance
        void copyFrom(const ms_fragment* src);

        //! A constructor for regular series ions
        ms_fragment(const int seriesID, const double massValue, const double nlValue, const int col, const int chargeState = 1);

        //! A constructor for regular series ions
        ms_fragment(const char* seriesName, const double massValue, const double nlValue, const int col, const int chargeState = 1);

        //! A constructor for internal series ions only
        ms_fragment(const int seriesID, 
                    const double massValue,
                    const double nlValue,
                    const char* peptideStr,
                    const int startPos, 
                    const int endPos, 
                    const int chargeState = 1);

        //! A constructor for immonium ion
        ms_fragment(const char residueName, const double massValue, const double nlValue, const int col = -1, const int chargeState = 1);

        //! Returns <b>TRUE</b> for <tt>a, b, c, d, y, v, w, x, z</tt> series and their derivatives
        bool isRegular() const;

        //! Returns <b>TRUE</b> for internal fragments <tt>ya</tt> and <tt>yb</tt>
        bool isInternal() const;

        //! Returns <b>TRUE</b> for immonium ions
        bool isImmonium() const;

        //! Returns the ion mass value
        double getMass() const;

        //! Returns the neutral loss values applied to the fragment
        double getNeutralLoss() const;

        //! Returns a symbolic label for the like "a*(6)++"
        std::string getLabel() const;

        //! Returns the ion series name
        std::string getSeriesName() const;

        //! Returns the position index in case of a regular series
        int getColumn() const;

        //! Returns the start position in case of an internal fragment
        int getStart() const;

        //! Returns the last position in case of an internal fragment
        int getEnd() const;

        //! Returns the charge state (1, 2, 3 etc.) for the ion
        int getCharge() const;

    protected:
        enum ION_TYPE {ION_TYPE_UNKNOWN,
                       ION_TYPE_REGULAR,
                       ION_TYPE_INTERNAL,
                       ION_TYPE_IMMONIUM};
        double      mass;
        double      nl;
        std::string label;
        std::string series;
        int         start;
        int         end;
        int         charge;
        ION_TYPE    type;
    }; //class ms_fragment
    /** @} */ // end of tools_group
} // matrix_science

#endif // !defined(ms_fragment_INCLUDED_)
