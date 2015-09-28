/*
##############################################################################
# file: ms_quant_numerator.hpp                                               #
# 'msparser' toolkit                                                         #
# Encapsulates "numerator_component" and "denominator_component" elements    #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_numerator.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2007-02-08 19:28:43 $
##############################################################################
 */

#ifndef MS_QUANT_NUMERATOR_HPP
#define MS_QUANT_NUMERATOR_HPP

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

    //! Represent "numerator_component" and "denominator_component"
    class MS_MASCOTRESFILE_API ms_quant_numerator: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_numerator();

        //! Copying constructor
        ms_quant_numerator(const ms_quant_numerator& src);

        //! Destructor
        virtual ~ms_quant_numerator();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_numerator* right);

        //! C++ style assignment operator
        ms_quant_numerator& operator=(const ms_quant_numerator& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "name" attribute
        bool haveName() const;

        //! Returns "name" attribute value
        std::string getName() const;

        //! Call this member to set a custom value for "name" attribute
        void setName(const char* value);

        //! Delete "name" attribute
        void dropName();

        //! Obtain a symbolic name for the element's schema type
        std::string getNameSchemaType() const;


        //! Indicates presence of "coefficient" attribute
        bool haveCoefficient() const;

        //! Returns "coefficient"-attribute value
        std::string getCoefficient() const;

        //! Call this member to set a custom value for "coefficient"-attribute
        void setCoefficient(const char* value);

        //! Delete "coefficient" attribute
        void dropCoefficient();

        //! Obtain a symbolic name for the element's schema type
        std::string getCoefficientSchemaType() const;


        //! Returns string element content
        std::string getContent() const;

        //! Call this member to set a custom string value for the content
        void setContent(const char* value);

        //! Obtain a symbolic name for the element's schema type
        std::string getContentSchemaType() const;

    private:
        std::string _name;
        bool _name_set;

        std::string _coefficient;
        bool _coefficient_set;

        std::string _value;

    }; // class ms_quant_numerator

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_NUMERATOR_HPP

/*------------------------------- End of File -------------------------------*/
