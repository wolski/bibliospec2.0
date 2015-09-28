/*
##############################################################################
# file: ms_quant_specificity.hpp                                             #
# 'msparser' toolkit                                                         #
# Describes "specificity" element from "quantitation.xml"-file               #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_specificity.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_SPECIFICITY_HPP
#define MS_QUANT_SPECIFICITY_HPP

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

    class ms_quant_neutralloss; // forward declaration
    class ms_quant_pepneutralloss; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Describes "specificity" element in quantitation.xml document. Unimod style specificity.
    /*!
     * Objects of this type host a list of nested "NeutralLoss" elements.
     */
    class MS_MASCOTRESFILE_API ms_quant_specificity: public ms_xml_IValidatable // defined in "ms_xml_typeinfo.hpp"
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_specificity();

        //! Copying constructor
        ms_quant_specificity(const ms_quant_specificity& src);

        //! Destructor
        virtual ~ms_quant_specificity();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_specificity* right);

        //! C++ style assignment operator
        ms_quant_specificity& operator=(const ms_quant_specificity& right);

        // methods of ms_quant_IValidatable interface

        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of "NeutralLoss" elements held
        int getNumberOfNeutralLosses() const;

        //! Deletes all elements from the list
        void clearNeutralLosses();

        //! Adds a new "NeutralLoss" element at the end of the list
        void appendNeutralLoss(const ms_quant_neutralloss *neutralloss);

        //! Returns a "NeutralLoss" element object by its number
        const ms_quant_neutralloss * getNeutralLoss(const int idx) const;

        //! Update the information for a specific neutral loss
        bool updateNeutralLoss(const int idx, const ms_quant_neutralloss* neutralloss);

        //! Remove a "NeutralLoss" element from the list
        bool deleteNeutralLoss(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getNeutralLossSchemaType() const;


        //! Returns a number of "PepNeutralLoss" elements held
        int getNumberOfPepNeutralLosses() const;

        //! Deletes all elements from the list
        void clearPepNeutralLosses();

        //! Adds a new "PepNeutralLoss" element at the end of the list
        void appendPepNeutralLoss(const ms_quant_pepneutralloss *neutralloss);

        //! Returns a pointer to the "PepNeutralLoss" element by its number
        const ms_quant_pepneutralloss* getPepNeutralLoss(const int idx) const;

        //! Update the information for a specific peptide neutral loss
        bool updatePepNeutralLoss(const int idx, const ms_quant_pepneutralloss* neutralloss);

        //! Remove a "PepNeutralLoss" element from the list
        bool deletePepNeutralLoss(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getPepNeutralLossSchemaType() const;


        //! Indicates whether "site" attribute is present or not
        bool haveSite() const;

        //! Returns "site"-attribute value
        std::string getSite() const;

        //! Call this member to set a custom value for "site"-attribute
        void setSite(const char* site);

        //! Deletes "site" attribute
        void dropSite();

        //! Obtain a symbolic name for the element's schema type
        std::string getSiteSchemaType() const;


        //! Indicates whether "position" attribute is present or not
        bool havePosition() const;

        //! Returns "position"-attribute value
        std::string getPosition() const;

        //! Call this member to set a custom value for "position"-attribute
        void setPosition(const char* position);

        //! Deletes "position" attribute
        void dropPosition();

        //! Obtain a symbolic name for the element's schema type
        std::string getPositionSchemaType() const;

    private:
        typedef std::vector< ms_quant_neutralloss* > neutralloss_vector;
        neutralloss_vector _neutrallosses;
        
        typedef std::vector< ms_quant_pepneutralloss* > pepneutralloss_vector;
        pepneutralloss_vector _pepneutrallosses;

        std::string _site;
        bool _site_set;

        std::string _position;
        bool _position_set;
    }; // class ms_quant_specificity

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_SPECIFICITY_HPP

/*------------------------------- End of File -------------------------------*/
