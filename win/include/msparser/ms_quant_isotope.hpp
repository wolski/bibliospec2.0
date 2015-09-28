/*
##############################################################################
# file: ms_quant_isotope.hpp                                              #
# 'msparser' toolkit                                                         #
# Encapsulates "isotope" element from "quantitation.xml"-file             #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_isotope.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_ISOTOPE_HPP
#define MS_QUANT_ISOTOPE_HPP

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

#include <string>
#include <vector>

namespace matrix_science {

    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents "isotope" element
    class MS_MASCOTRESFILE_API ms_quant_isotope: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_isotope();

        //! Copying constructor
        ms_quant_isotope(const ms_quant_isotope& src);

        //! Destructor
        virtual ~ms_quant_isotope();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_isotope* right);

        //! C++ style assignment operator
        ms_quant_isotope& operator=(const ms_quant_isotope& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "old" element
        bool haveOld() const;

        //! Returns "old" element value
        std::string getOld() const;

        //! Call this member to set a custom value for "old" element
        void setOld(const char* value);

        //! Delete "old" element
        void dropOld();

        //! Obtain a symbolic name for the element's schema type
        std::string getOldSchemaType() const;


        //! Indicates presence of "new" element
        bool haveNew() const;

        //! Returns "new"-element value
        std::string getNew() const;

        //! Call this member to set a custom value for "new"-element
        void setNew(const char* value);

        //! Delete "new" element
        void dropNew();

        //! Obtain a symbolic name for the element's schema type
        std::string getNewSchemaType() const;

    private:

        std::string _old;
        bool _old_set;

        std::string _new;
        bool _new_set;

    }; // class ms_quant_isotope

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_ISOTOPE_HPP

/*------------------------------- End of File -------------------------------*/
