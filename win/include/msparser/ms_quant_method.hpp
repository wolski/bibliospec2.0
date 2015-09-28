/*
##############################################################################
# file: ms_quant_method.hpp                                                  #
# 'msparser' toolkit                                                         #
# Encapsulates method-element from "quantitation.xml"-file                 #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_method.hpp,v $
 * @(#)$Revision: 1.7 $
 * @(#)$Date: 2007-02-08 19:28:43 $
##############################################################################
 */

#ifndef MS_QUANT_METHOD_HPP
#define MS_QUANT_METHOD_HPP

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

    class ms_quant_modgroup; // forward declaration
    class ms_quant_component; // forward declaration
    class ms_quant_quality; // forward declaration
    class ms_quant_integration; // forward delcaration
    class ms_quant_outliers; // forward declaration
    class ms_quant_normalisation; // forward declaration
    class ms_quant_protocol; // forward declaration
    class ms_quant_ratio; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! An object of this class represent a single quantitation method from quantitation.xml
    class MS_MASCOTRESFILE_API ms_quant_method: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_method();

        //! Copying constructor
        ms_quant_method(const ms_quant_method& src);

        //! Destructor
        virtual ~ms_quant_method();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_method* right);

        //! C++ style assignment operator
        ms_quant_method& operator=(const ms_quant_method& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of nested modification groups
        int getNumberOfModificationGroups() const;

        //! Deletes all modification groups from the list
        void clearModificationGroups();

        //! Adds a new modification group at the end of the list
        void appendModificationGroup(const ms_quant_modgroup *item);

        //! Returns a modification group object by its number
        const ms_quant_modgroup * getModificationGroupByNumber(const int idx) const;

        //! Returns a modification group object by its name or NULL in case of not found
        const ms_quant_modgroup * getModificationGroupByName(const char *name) const;

        //! Update the information for a specific modification group refering to it by its index
        bool updateModificationGroupByNumber(const int idx, const ms_quant_modgroup* modgroup);

        //! Update the information for a specific modification group refering to it by its unique name
        bool updateModificationGroupByName(const char *name, const ms_quant_modgroup* modgroup);

        //! Remove a modification group from the list in memory by its index
        bool deleteModificationGroupByNumber(const int idx);

        //! Remove a modification group from the list in memory by its unique name
        bool deleteModificationGroupByName(const char *name);

        //! Obtain a symbolic name for the element's schema type
        std::string getModificationGroupSchemaType() const;


        //! Returns a number of nested components
        int getNumberOfComponents() const;

        //! Deletes all components from the list
        void clearComponents();

        //! Adds a new component at the end of the list
        void appendComponent(const ms_quant_component *item);

        //! Returns a component object by its number
        const ms_quant_component * getComponentByNumber(const int idx) const;

        //! Returns a component object by its name or NULL in case of not found
        const ms_quant_component * getComponentByName(const char *name) const;

        //! Update the information for a specific component refering to it by its index
        bool updateComponentByNumber(const int idx, const ms_quant_component* comp);

        //! Update the information for a specific component refering to it by its unique name
        bool updateComponentByName(const char *name, const ms_quant_component* comp);

        //! Remove a component from the list in memory by its index
        bool deleteComponentByNumber(const int idx);

        //! Remove a component from the list in memory by its unique name
        bool deleteComponentByName(const char *name);

        //! Obtain a symbolic name for the element's schema type
        std::string getComponentSchemaType() const;


        //! Returns a number of nested "report_ratio" elements
        int getNumberOfReportRatios() const;

        //! Deletes all "report_ratio" elements from the list
        void clearReportRatios();

        //! Adds a new "report_ratio" element at the end of the list
        void appendReportRatio(const ms_quant_ratio *ratio);

        //! Returns a "report_ratio" element object by its number
        const ms_quant_ratio * getReportRatioByNumber(const int idx) const;

        //! Returns a "report_ratio" element object by its name or NULL in case of not found
        const ms_quant_ratio * getReportRatioByName(const char *name) const;

        //! Update the information for a specific "report_ratio" element refering to it by its index
        bool updateReportRatioByNumber(const int idx, const ms_quant_ratio* ratio);

        //! Update the information for a specific "report_ratio" element refering to it by its unique name
        bool updateReportRatioByName(const char *name, const ms_quant_ratio* ratio);

        //! Remove a "report_ratio" element from the list in memory by its index
        bool deleteReportRatioByNumber(const int idx);

        //! Remove a "report_ratio" element from the list in memory by its unique name
        bool deleteReportRatioByName(const char *name);

        //! Obtain a symbolic name for the element's schema type
        std::string getReportRatioSchemaType() const;


        //! Returns a number of nested "exclusion" elements
        int getNumberOfExclusions() const;

        //! Deletes all "exclusion" elements from the list
        void clearExclusions();

        //! Adds a new "exclusion" element at the end of the list
        void appendExclusion(const char* exclusion);

        //! Returns a "exclusion" element object by its number
        std::string getExclusion(const int idx) const;

        //! Update the information for a specific "exclusion" element refering to it by its index
        bool updateExclusion(const int idx, const char* exclusion);

        //! Remove a "exclusion" element from the list in memory by its index
        bool deleteExclusion(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getExclusionSchemaType() const;


        //! Returns a number of nested "seq" elements
        int getNumberOfSeqs() const;

        //! Deletes all "seq" elements from the list
        void clearSeqs();

        //! Adds a new "seq" element at the end of the list
        void appendSeq(const char* seq);

        //! Returns a "seq" element object by its number
        std::string getSeq(const int idx) const;

        //! Update the information for a specific "seq" element refering to it by its index
        bool updateSeq(const int idx, const char* seq);

        //! Remove a "seq" element from the list in memory by its index
        bool deleteSeq(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getSeqSchemaType() const;


        //! Indicates whether "comp" element is present
        bool haveComp() const;

        //! Returns "comp"-element value if present
        std::string getComp() const;

        //! Call this member to set a custom value for "comp"-element
        void setComp(const char* value);

        //! Delete "comp" element
        void dropComp();

        //! Obtain a symbolic name for the element's schema type
        std::string getCompSchemaType() const;


        //! Indicates whether "quality" element is present
        bool haveQuality() const;

        //! Returns pointer to "quality"-element object
        const ms_quant_quality* getQuality() const;

        //! Call this member to supply custom content for "quality"-element object
        void setQuality(const ms_quant_quality* quality);

        //! Delete "quality" element
        void dropQuality();

        //! Obtain a symbolic name for the element's schema type
        std::string getQualitySchemaType() const;


        //! Indicates whether "integration" element is present
        bool haveIntegration() const;

        //! Returns pointer to "integration"-element object
        const ms_quant_integration* getIntegration() const;

        //! Call this member to supply custom content for "integration"-element object
        void setIntegration(const ms_quant_integration* integration);

        //! Delete "integration" element
        void dropIntegration();

        //! Obtain a symbolic name for the element's schema type
        std::string getIntegrationSchemaType() const;


        //! Indicates whether "outliers" element is present
        bool haveOutliers() const;

        //! Returns pointer to "outliers"-element object
        const ms_quant_outliers* getOutliers() const;

        //! Call this member to supply custom content for "outliers"-element object
        void setOutliers(const ms_quant_outliers* outliers);

        //! Delete "outliers" element
        void dropOutliers();

        //! Obtain a symbolic name for the element's schema type
        std::string getOutliersSchemaType() const;


        //! Indicates whether "normalisation" element is present
        bool haveNormalisation() const;

        //! Returns pointer to "normalisation"-element object
        const ms_quant_normalisation* getNormalisation() const;

        //! Call this member to supply custom content for "normalisation"-element object
        void setNormalisation(const ms_quant_normalisation* normalisation);

        //! Delete "normalisation" element
        void dropNormalisation();

        //! Obtain a symbolic name for the element's schema type
        std::string getNormalisationSchemaType() const;


        //! Indicates whether "protocol" element is present
        bool haveProtocol() const;

        //! Returns pointer to "protocol"-element object
        const ms_quant_protocol* getProtocol() const;

        //! Call this member to supply custom content for "protocol"-element object
        void setProtocol(const ms_quant_protocol* protocol);

        //! Delete "protocol" element
        void dropProtocol();

        //! Obtain a symbolic name for the element's schema type
        std::string getProtocolSchemaType() const;


        //! Indicates whether "name" attribute is present
        bool haveName() const;

        //! Returns "name"-attribute value
        std::string getName() const;

        //! Call this member to set a custom value for "name"-attribute
        void setName(const char* value);

        //! Delete "name" attribute
        void dropName();

        //! Obtain a symbolic name for the element's schema type
        std::string getNameSchemaType() const;


        //! Indicates whether "description" attribute is present
        bool haveDescription() const;

        //! Returns "description"-attribute value
        std::string getDescription() const;

        //! Call this member to set a custom value for "description"-attribute
        void setDescription(const char* value);

        //! Delete "description" attribute
        void dropDescription();

        //! Obtain a symbolic name for the element's schema type
        std::string getDescriptionSchemaType() const;


        //! Indicates whether "constrain_search" attribute is present
        bool haveConstrainSearch() const;

        //! Returns "constrain_search"-attribute value
        bool isConstrainSearch() const;

        //! Call this member to set a custom value for "constrain_search"-attribute
        void setConstrainSearch(const bool value);

        //! Delete "constrain_search" attribute
        void dropConstrainSearch();

        //! Obtain a symbolic name for the element's schema type
        std::string getConstrainSearchSchemaType() const;


        //! Indicates whether "protein_ratio_type" attribute is present
        bool haveProteinRatioType() const;

        //! Returns "protein_ratio_type"-attribute value
        std::string getProteinRatioType() const;

        //! Call this member to set a custom value for "protein_ratio_type"-attribute
        void setProteinRatioType(const char* value);

        //! Delete "protein_ratio_type" attribute
        void dropProteinRatioType();

        //! Obtain a symbolic name for the element's schema type
        std::string getProteinRatioTypeSchemaType() const;


        //! Indicates whether "report_detail" attribute is present
        bool haveReportDetail() const;

        //! Returns "report_detail"-attribute value
        bool isReportDetail() const;

        //! Call this member to set a custom value for "report_detail"-attribute
        void setReportDetail(const bool value);

        //! Delete "report_detail" attribute
        void dropReportDetail();

        //! Obtain a symbolic name for the element's schema type
        std::string getReportDetailSchemaType() const;


        //! Indicates whether "min_num_peptides" attribute is present
        bool haveMinNumPeptides() const;

        //! Returns "min_num_peptides"-attribute value
        int getMinNumPeptides() const;

        //! Call this member to set a custom value for "min_num_peptides"-attribute
        void setMinNumPeptides(const int value);

        //! Delete "min_num_peptides" attribute
        void dropMinNumPeptides();

        //! Obtain a symbolic name for the element's schema type
        std::string getMinNumPeptidesSchemaType() const;


        //! Indicates whether "prot_score_type" attribute is present
        bool haveProtScoreType() const;

        //! Returns "prot_score_type"-attribute value
        std::string getProtScoreType() const;

        //! Call this member to set a custom value for "prot_score_type"-attribute
        void setProtScoreType(const char* value);

        //! Delete "prot_score_type" attribute
        void dropProtScoreType();

        //! Obtain a symbolic name for the element's schema type
        std::string getProtScoreTypeSchemaType() const;


        //! Indicates whether "sig_threshold_value" attribute is present
        bool haveSigThresholdValue() const;

        //! Returns "sig_threshold_value"-attribute value
        std::string getSigThresholdValue() const;

        //! Call this member to set a custom value for "sig_threshold_value"-attribute
        void setSigThresholdValue(const char* value);

        //! Delete "sig_threshold_value" attribute
        void dropSigThresholdValue();

        //! Obtain a symbolic name for the element's schema type
        std::string getSigThresholdValueSchemaType() const;


        //! Indicates whether "show_sub_sets" attribute is present
        bool haveShowSubSets() const;

        //! Returns the "show_sub_sets" attribute value
        std::string getShowSubSets() const;

        //! Call this member to set a custom value for the "show_sub_sets" attribute
        void setShowSubSets(const char* value);

        //! Delete "show_sub_sets" attribute
        void dropShowSubSets();

        //! Obtain a symbolic name for the element's schema type
        std::string getShowSubSetsSchemaType() const;


        //! Indicates whether "require_bold_red" attribute is present
        bool haveRequireBoldRed() const;

        //! Returns "require_bold_red"-attribute value
        bool isRequireBoldRed() const;

        //! Call this member to set a custom value for "require_bold_red"-attribute
        void setRequireBoldRed(const bool value);

        //! Delete "require_bold_red" attribute
        void dropRequireBoldRed();

        //! Obtain a symbolic name for the element's schema type
        std::string getRequireBoldRedSchemaType() const;

    private:

        // elements - lists
        typedef std::vector< ms_quant_modgroup* > modgroup_vector;
        modgroup_vector _modgroups;

        typedef std::vector< ms_quant_component* > comp_vector;
        comp_vector _components;

        typedef std::vector< ms_quant_ratio* > ratio_vector;
        ratio_vector _reportRatios;

        typedef std::vector< std::string > string_vector;
        string_vector _exclusions;

        string_vector _seqs;

        // single instance elements
        std::string _comp;
        bool _comp_set;

        ms_quant_quality *_pQuality;
        bool _quality_set;

        ms_quant_integration *_pIntegration;
        bool _integration_set;

        ms_quant_outliers *_pOutliers;
        bool _outliers_set;

        ms_quant_normalisation *_pNormalisation;
        bool _normalisation_set;

        ms_quant_protocol *_pProtocol;
        bool _protocol_set;

        // attributes
        std::string _name;
        bool _name_set;

        std::string _description;
        bool _description_set;

        bool _constrainSearch;
        bool _constrainSearch_set;

        std::string _proteinRatioType;
        bool _proteinRatioType_set;

        bool _reportDetail;
        bool _reportDetail_set;

        int _minNumPeptides;
        bool _minNumPeptides_set;

        std::string _protScoreType;
        bool _protScoreType_set;

        std::string _sigThresholdValue;
        bool _sigThresholdValue_set;

        std::string _showSubSets;
        bool _showSubSets_set;

        bool _requireBoldRed;
        bool _requireBoldRed_set;

    }; // class ms_quant_method

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_METHOD_HPP

/*------------------------------- End of File -------------------------------*/

