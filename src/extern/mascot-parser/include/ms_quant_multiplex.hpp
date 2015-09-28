/*
##############################################################################
# file: ms_quant_multiplex.hpp                                           #
# 'msparser' toolkit                                                         #
# Encapsulates normalisation-element from "quantitation.xml"-file            #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_multiplex.hpp,v $
 * @(#)$Revision: 1.7 $
 * @(#)$Date: 2007-02-08 19:28:43 $
##############################################################################
 */

#ifndef MS_QUANT_MULTIPLEX_HPP
#define MS_QUANT_MULTIPLEX_HPP

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

namespace matrix_science {

    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! An object of this class represent "multiplex" element in quantitation.xml
    /*!
     * Use intensities of sequence ion fragment peaks within an MS/MS spectrum.
     */
    class MS_MASCOTRESFILE_API ms_quant_multiplex: public ms_quant_parameters
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_multiplex();

        //! Copying constructor
        ms_quant_multiplex(const ms_quant_multiplex& src);

        //! Destructor
        virtual ~ms_quant_multiplex();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_multiplex* right);

        //! C++ style assignment operator
        ms_quant_multiplex& operator=(const ms_quant_multiplex& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Check for presence of "ion_series" element
        bool haveIonSeries() const;

        //! Returns "ion_series" element value
        std::string getIonSeries() const;

        //! Call this member to supply a custom content for "ion_series" element
        void setIonSeries(const std::string ionSeries);

        //! Delete "ion_series" element
        void dropIonSeries();

        //! Obtain a symbolic name for the element's schema type
        std::string getIonSeriesSchemaType() const;


        //! Indicates presence of "exclude_internal_label" attribute
        bool haveExcludeInternalLabel() const;

        //! Returns "exclude_internal_label" attribute value
        bool isExcludeInternalLabel() const;

        //! Call this member to set a custom value for "exclude_internal_label" attribute
        void setExcludeInternalLabel(const bool value);

        //! Delete "exclude_internal_label" attribute
        void dropExcludeInternalLabel();

        //! Obtain a symbolic name for the element's schema type
        std::string getExcludeInternalLabelSchemaType() const;


        //! Indicates presence of "ion_intensity_threshold" attribute
        bool haveIonIntensityThreshold() const;

        //! Returns "ion_intensity_threshold" attribute value
        std::string getIonIntensityThreshold() const;

        //! Call this member to set a custom value for "ion_intensity_threshold" attribute
        void setIonIntensityThreshold(const char* value);

        //! Delete "ion_intensity_threshold" attribute
        void dropIonIntensityThreshold();

        //! Obtain a symbolic name for the element's schema type
        std::string getIonIntensityThresholdSchemaType() const;


        //! Indicates presence of "exclude_isobaric_fragments" attribute
        bool haveExcludeIsobaricFragments() const;

        //! Returns "exclude_isobaric_fragments" attribute value
        bool isExcludeIsobaricFragments() const;

        //! Call this member to set a custom value for "exclude_isobaric_fragments" attribute
        void setExcludeIsobaricFragments(const bool value);

        //! Delete "exclude_isobaric_fragments" attribute
        void dropExcludeIsobaricFragments();

        //! Obtain a symbolic name for the element's schema type
        std::string getExcludeIsobaricFragmentsSchemaType() const;


        //! Indicates presence of "min_ion_pairs" attribute
        bool haveMinIonPairs() const;

        //! Returns "min_ion_pairs" attribute value
        int getMinIonPairs() const;

        //! Call this member to set a custom value for "min_ion_pairs" attribute
        void setMinIonPairs(const int value);

        //! Delete "min_ion_pairs" attribute
        void dropMinIonPairs();

        //! Obtain a symbolic name for the element's schema type
        std::string getMinIonPairsSchemaType() const;

    private:

        std::string _ionSeries;
        bool _ionSeries_set;

        bool _excludeInternalLabel;
        bool _excludeInternalLabel_set;

        std::string _ionIntensityThreshold;
        bool _ionIntensityThreshold_set;

        bool _excludeIsobaricFragments;
        bool _excludeIsobaricFragments_set;

        int _minIonPairs;
        bool _minIonPairs_set;
    }; // class ms_quant_multiplex

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_MULTIPLEX_HPP

/*------------------------------- End of File -------------------------------*/
