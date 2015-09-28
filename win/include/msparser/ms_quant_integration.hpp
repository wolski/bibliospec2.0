/*
##############################################################################
# file: ms_quant_integration.hpp                                             #
# 'msparser' toolkit                                                         #
# Encapsulates integration-element from "quantitation.xml"-file              #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_integration.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2007-02-08 19:28:43 $
##############################################################################
 */

#ifndef MS_QUANT_INTEGRATION_HPP
#define MS_QUANT_INTEGRATION_HPP

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

    //! An object of this class represent a single "integration" element in quantitation.xml
    /*!
     * Specifies method and parameters to be used to integrate precursor over time.
     */
    class MS_MASCOTRESFILE_API ms_quant_integration: public ms_quant_parameters
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_integration();

        //! Copying constructor
        ms_quant_integration(const ms_quant_integration& src);

        //! Destructor
        virtual ~ms_quant_integration();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_integration* right);

        //! C++ style assignment operator
        ms_quant_integration& operator=(const ms_quant_integration& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "method" attribute
        bool haveMethod() const;

        //! Returns "method" attribute value
        std::string getMethod() const;

        //! Call this member to set a custom value for "method" attribute
        void setMethod(const char* value);

        //! Delete "method" attribute
        void dropMethod();

        //! Obtain a symbolic name for the element's schema type
        std::string getMethodSchemaType() const;


        //! Indicates presence of "source" attribute
        bool haveSource() const;

        //! Returns "source" attribute value
        std::string getSource() const;

        //! Call this member to set a custom value for "source" attribute
        void setSource(const char* value);

        //! Delete "source" attribute
        void dropSource();

        //! Obtain a symbolic name for the element's schema type
        std::string getSourceSchemaType() const;


        //! Indicates presence of "mass_delta" attribute
        bool haveMassDelta() const;

        //! Returns "mass_delta" attribute value
        std::string getMassDelta() const;

        //! Call this member to set a custom value for "mass_delta" attribute
        void setMassDelta(const char* value);

        //! Delete "mass_delta" attribute
        void dropMassDelta();

        //! Obtain a symbolic name for the element's schema type
        std::string getMassDeltaSchemaType() const;


        //! Indicates presence of "mass_delta_unit" attribute
        bool haveMassDeltaUnit() const;

        //! Returns "mass_delta_unit" attribute value
        std::string getMassDeltaUnit() const;

        //! Call this member to set a custom value for "mass_delta_unit" attribute
        void setMassDeltaUnit(const char* value);

        //! Delete "mass_delta_unit" attribute
        void dropMassDeltaUnit();

        //! Obtain a symbolic name for the element's schema type
        std::string getMassDeltaUnitSchemaType() const;


        //! Indicates presence of "elution_time_delta" attribute
        bool haveElutionTimeDelta() const;

        //! Returns "elution_time_delta" attribute value
        std::string getElutionTimeDelta() const;

        //! Call this member to set a custom value for "elution_time_delta" attribute
        void setElutionTimeDelta(const char* value);

        //! Delete "elution_time_delta" attribute
        void dropElutionTimeDelta();

        //! Obtain a symbolic name for the element's schema type
        std::string getElutionTimeDeltaSchemaType() const;


        //! Indicates presence of "elution_time_delta_unit" attribute
        bool haveElutionTimeDeltaUnit() const;

        //! Returns "elution_time_delta_unit" attribute value
        std::string getElutionTimeDeltaUnit() const;

        //! Call this member to set a custom value for "elution_time_delta_unit" attribute
        void setElutionTimeDeltaUnit(const char* value);

        //! Delete "elution_time_delta_unit" attribute
        void dropElutionTimeDeltaUnit();

        //! Obtain a symbolic name for the element's schema type
        std::string getElutionTimeDeltaUnitSchemaType() const;


        //! Indicates presence of "elution_profile_correlation_threshold" attribute
        bool haveElutionProfileCorrelationThreshold() const;

        //! Returns "elution_profile_correlation_threshold" attribute value
        std::string getElutionProfileCorrelationThreshold() const;

        //! Call this member to set a custom value for "elution_profile_correlation_threshold" attribute
        void setElutionProfileCorrelationThreshold(const char* value);

        //! Delete "elution_profile_correlation_threshold" attribute
        void dropElutionProfileCorrelationThreshold();

        //! Obtain a symbolic name for the element's schema type
        std::string getElutionProfileCorrelationThresholdSchemaType() const;


        //! Indicates presence of "processing_options" attribute
        bool haveProcessingOptions() const;

        //! Returns "processing_options" attribute value
        std::string getProcessingOptions() const;

        //! Call this member to set a custom value for "processing_options" attribute
        void setProcessingOptions(const std::string value);

        //! Delete "processing_options" attribute
        void dropProcessingOptions();

        //! Obtain a symbolic name for the element's schema type
        std::string getProcessingOptionsSchemaType() const;


        //! Indicates presence of "precursor_range" attribute
        bool havePrecursorRange() const;

        //! Returns "precursor_range" attribute value
        std::string getPrecursorRange() const;

        //! Call this member to set a custom value for "precursor_range" attribute
        void setPrecursorRange(const char* value);

        //! Delete "precursor_range" attribute
        void dropPrecursorRange();

        //! Obtain a symbolic name for the element's schema type
        std::string getPrecursorRangeSchemaType() const;

    private:

        std::string _method;
        bool _method_set;

        std::string _source;
        bool _source_set;

        std::string _massDelta;
        bool _massDelta_set;

        std::string _massDeltaUnit;
        bool _massDeltaUnit_set;

        std::string _elutionTimeDelta;
        bool _elutionTimeDelta_set;

        std::string _elutionTimeDeltaUnit;
        bool _elutionTimeDeltaUnit_set;

        std::string _elutionProfileCorrelationThreshold;
        bool _elutionProfileCorrelationThreshold_set;

        std::string _processingOptions;
        bool _processingOptions_set;

        std::string _precursorRange;
        bool _precursorRange_set;

    }; // class ms_quant_integration

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_INTEGRATION_HPP

/*------------------------------- End of File -------------------------------*/

