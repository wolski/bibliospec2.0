/*
##############################################################################
# file: ms_quant_quality.hpp                                                 #
# 'msparser' toolkit                                                         #
# Encapsulates quality-element from "quantitation.xml"-file                  #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_quality.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2007-02-08 19:28:43 $
##############################################################################
 */

#ifndef MS_QUANT_QUALITY_HPP
#define MS_QUANT_QUALITY_HPP

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

    //! An object of this class represent a single quality element in quantitation.xml
    class MS_MASCOTRESFILE_API ms_quant_quality: public ms_quant_parameters
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_quality();

        //! Copying constructor
        ms_quant_quality(const ms_quant_quality& src);

        //! Destructor
        virtual ~ms_quant_quality();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_quality* right);

        //! C++ style assignment operator
        ms_quant_quality& operator=(const ms_quant_quality& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "min_precursor_charge" attribute
        bool haveMinPrecursorCharge() const;

        //! Returns "min_precursor_charge"-attribute value
        int getMinPrecursorCharge() const;

        //! Call this member to set a custom value for "min_precursor_charge"-attribute
        void setMinPrecursorCharge(const int value);

        //! Delete "min_precursor_charge" attribute
        void dropMinPrecursorCharge();

        //! Obtain a symbolic name for the element's schema type
        std::string getMinPrecursorChargeSchemaType() const;


        //! Indicates presence of "isolated_precursor" attribute
        bool haveIsolatedPrecursor() const;

        //! Returns "isolated_precursor"-attribute value
        bool isIsolatedPrecursor() const;

        //! Call this member to set a custom value for "isolated_precursor"-attribute
        void setIsolatedPrecursor(const bool value);

        //! Delete "isolated_precursor" attribute
        void dropIsolatedPrecursor();

        //! Obtain a symbolic name for the element's schema type
        std::string getIsolatedPrecursorSchemaType() const;


        //! Indicates presence of "minimum_a1" attribute
        bool haveMinimumA1() const;

        //! Returns "minimum_a1"-attribute value
        std::string getMinimumA1() const;

        //! Call this member to set a custom value for "minimum_a1"-attribute
        void setMinimumA1(const char* value);

        //! Delete "minimum_a1" attribute
        void dropMinimumA1();

        //! Obtain a symbolic name for the element's schema type
        std::string getMinimumA1SchemaType() const;


        //! Indicates presence of "pep_threshold_type" attribute
        bool havePepThresholdType() const;

        //! Returns "pep_threshold_type"-attribute value
        std::string getPepThresholdType() const;

        //! Call this member to set a custom value for "pep_threshold_type"-attribute
        void setPepThresholdType(const char* value);

        //! Delete "pep_threshold_type" attribute
        void dropPepThresholdType();

        //! Obtain a symbolic name for the element's schema type
        std::string getPepThresholdTypeSchemaType() const;


        //! Indicates presence of "pep_threshold_value" attribute
        bool havePepThresholdValue() const;

        //! Returns "pep_threshold_value"-attribute value
        std::string getPepThresholdValue() const;

        //! Call this member to set a custom value for "pep_threshold_value"-attribute
        void setPepThresholdValue(const char* value);

        //! Delete "pep_threshold_value" attribute
        void dropPepThresholdValue();

        //! Obtain a symbolic name for the element's schema type
        std::string getPepThresholdValueSchemaType() const;

    private:

        int _minPrecursorCharge;
        bool _minPrecursorCharge_set;

        bool _isolatedPrecursor;
        bool _isolatedPrecursor_set;

        std::string _minimumA1;
        bool _minimumA1_set;

        std::string _pepThresholdType;
        bool _pepThresholdType_set;

        std::string _pepThresholdValue;
        bool _pepThresholdValue_set;

    }; // class ms_quant_quality

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_QUALITY_HPP

/*------------------------------- End of File -------------------------------*/

