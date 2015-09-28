/*
##############################################################################
# file: ms_umod_specificity.hpp                                              #
# 'msparser' toolkit                                                         #
# Represents 'specificity' object from unimod.xml file                       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_umod_specificity.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_UMOD_SPECIFICITY_HPP
#define MS_UMOD_SPECIFICITY_HPP

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

    class ms_umod_neutralloss; // forward declaration
    class ms_quant_specificity; // forward declaration
    class ms_umod_configfile;  // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents "amino acid" object in unimod.xml
    class MS_MASCOTRESFILE_API ms_umod_specificity
    {
        friend class ms_umod_xmlloader;
        friend class ms_umod_modification;
        friend class ms_umod_configfile;

    public:
        //! Default constructor
        ms_umod_specificity();

        //! Copying constructor
        ms_umod_specificity(const ms_umod_specificity& src);

        //! Copying constructor that takes an object from quantitation.xml
        ms_umod_specificity(const ms_quant_specificity& src, const ms_umod_configfile& umodFile);

        //! Destructor
        virtual ~ms_umod_specificity();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_umod_specificity* right);

        //! Copies all content from an object of quantitation.xml
        void copyFrom(const ms_quant_specificity* right, const ms_umod_configfile& umodFile);

        //! C++ style assignment operator
        ms_umod_specificity& operator=(const ms_umod_specificity& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of 'NeutralLoss' objects currently held in memory
        int getNumberOfNeutralLosses() const;

        //! Deletes all 'NeutralLoss' objects from the list
        void clearNeutralLosses();

        //! Adds a new 'NeutralLoss' object at the end of the list
        void appendNeutralLoss(const ms_umod_neutralloss *nl);

        //! Returns a read-only pointer to an 'NeutralLoss' object by its number
        const ms_umod_neutralloss* getNeutralLoss(const int idx) const;

        //! Update the information for a particular 'NeutralLoss' object
        bool updateNeutralLoss(const int idx, const ms_umod_neutralloss *nl);

        //! Remove an 'NeutralLoss' object from the list in memory
        bool deleteNeutralLoss(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getNeutralLossSchemaType() const;


        //! Returns a number of 'PepNeutralLoss' objects currently held in memory
        int getNumberOfPepNeutralLosses() const;

        //! Deletes all 'PepNeutralLoss' objects from the list
        void clearPepNeutralLosses();

        //! Adds a new 'PepNeutralLoss' object at the end of the list
        void appendPepNeutralLoss(const ms_umod_neutralloss *pnl);

        //! Returns a read-only pointer to an 'PepNeutralLoss' object by its number
        const ms_umod_neutralloss* getPepNeutralLoss(const int idx) const;

        //! Update the information for a particular 'PepNeutralLoss' object
        bool updatePepNeutralLoss(const int idx, const ms_umod_neutralloss *pnl);

        //! Remove an 'PepNeutralLoss' object from the list in memory
        bool deletePepNeutralLoss(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getPepNeutralLossSchemaType() const;


        //! Indicates presence of 'misc_notes' element
        bool haveMiscNotes() const;

        //! Returns 'misc_notes' element value
        std::string getMiscNotes() const;

        //! Call this member to set a custom value for 'misc_notes' element
        void setMiscNotes(const char* value);

        //! Delete 'misc_notes' element
        void dropMiscNotes();

        //! Obtain a symbolic name for the element's schema type
        std::string getMiscNotesSchemaType() const;


        //! Indicates presence of 'hidden' attribute
        bool haveHidden() const;

        //! Returns 'hidden' attribute value
        bool isHidden() const;

        //! Call this member to set a custom value for 'hidden' attribute
        void setHidden(const bool value);

        //! Delete 'hidden' attribute
        void dropHidden();

        //! Obtain a symbolic name for the element's schema type
        std::string getHiddenSchemaType() const;


        //! Indicates presence of 'site' attribute
        bool haveSite() const;

        //! Returns 'site' attribute value
        std::string getSite() const;

        //! Call this member to set a custom value for 'site' attribute
        void setSite(const char* value);

        //! Delete 'site' attribute
        void dropSite();

        //! Obtain a symbolic name for the element's schema type
        std::string getSiteSchemaType() const;


        //! Indicates presence of 'position' attribute
        bool havePosition() const;

        //! Returns 'position' attribute value
        std::string getPosition() const;

        //! Call this member to set a custom value for 'position' attribute
        void setPosition(const char* value);

        //! Delete 'position' attribute
        void dropPosition();

        //! Obtain a symbolic name for the element's schema type
        std::string getPositionSchemaType() const;


        //! Indicates presence of 'classification' attribute
        bool haveClassification() const;

        //! Returns 'classification' attribute value
        std::string getClassification() const;

        //! Call this member to set a custom value for 'classification' attribute
        void setClassification(const char* value);

        //! Delete 'classification' attribute
        void dropClassification();

        //! Obtain a symbolic name for the element's schema type
        std::string getClassificationSchemaType() const;


        //! Indicates presence of 'spec_group' attribute
        bool haveSpecGroup() const;

        //! Returns 'spec_group' attribute value
        int getSpecGroup() const;

        //! Call this member to set a custom value for 'spec_group' attribute
        void setSpecGroup(const int value);

        //! Delete 'spec_group' attribute
        void dropSpecGroup();

        //! Obtain a symbolic name for the element's schema type
        std::string getSpecGroupSchemaType() const;

    private:

        typedef std::vector< ms_umod_neutralloss* > nl_vector;
        nl_vector _neutrallosses;

        typedef std::vector< ms_umod_neutralloss* > pnl_vector;
        pnl_vector _pepneutrallosses;

        std::string _miscNotes;
        bool _miscNotes_set;

        bool _hidden;
        bool _hidden_set;

        std::string _site;
        bool _site_set;

        std::string _position;
        bool _position_set;

        std::string _classification;
        bool _classification_set;

        int _specGroup;
        bool _specGroup_set;

    }; // class ms_umod_specificity

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_UMOD_SPECIFICITY_HPP

/*------------------------------- End of File -------------------------------*/
