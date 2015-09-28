/*
##############################################################################
# file: ms_quant_localdef.hpp                                                #
# 'msparser' toolkit                                                         #
# Encapsulates "local_definition" element from "quantitation.xml"-file       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_localdef.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_LOCALDEF_HPP
#define MS_QUANT_LOCALDEF_HPP

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

    class ms_quant_specificity; // forward declaration
    class ms_quant_composition; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents "local_definition" element
    class MS_MASCOTRESFILE_API ms_quant_localdef: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_localdef();

        //! Copying constructor
        ms_quant_localdef(const ms_quant_localdef& src);

        //! Destructor
        virtual ~ms_quant_localdef();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_localdef* right);

        //! C++ style assignment operator
        ms_quant_localdef& operator=(const ms_quant_localdef& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of "specificity" elements held
        int getNumberOfSpecificities() const;

        //! Deletes all elements from the list
        void clearSpecificities();

        //! Adds a new "specificity" element at the end of the list
        void appendSpecificity(const ms_quant_specificity *specificity);

        //! Returns a "specificity" element object by its number
        const ms_quant_specificity * getSpecificity(const int idx) const;

        //! Update the information for a specific "specificity" element
        bool updateSpecificity(const int idx, const ms_quant_specificity* specificity);

        //! Remove a "specificity" element from the list
        bool deleteSpecificity(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getSpecificitySchemaType() const;


        //! Check presence of "delta" element
        bool haveDelta() const;

        //! Returns a pointer to the "delta" element
        const ms_quant_composition* getDelta() const;

        //! Call this member to supply custom content for the "delta" element
        void setDelta(const ms_quant_composition *delta);

        //! Deletes "delta" element
        void dropDelta();

        //! Obtain a symbolic name for the element's schema type
        std::string getDeltaSchemaType() const;


        //! Returns a number of "Ignore" elements held
        int getNumberOfIgnores() const;

        //! Deletes all "Ignore" elements from the list
        void clearIgnores();

        //! Adds a new "Ignore" element at the end of the list
        void appendIgnore(const ms_quant_composition *ignore);

        //! Returns a "Ignore" element object by its number
        const ms_quant_composition * getIgnore(const int idx) const;

        //! Update the information for a specific "Ignore" element
        bool updateIgnore(const int idx, const ms_quant_composition* ignore);

        //! Remove a "Ignore" element from the list
        bool deleteIgnore(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getIgnoreSchemaType() const;


        //! Check presence of "title" attribute
        bool haveTitle() const;

        //! Returns string value of the "title" attribute
        std::string getTitle() const;

        //! Call this member to set a custom value for the "title" attribute
        void setTitle(const char* value);

        //! Deletes "title" attribute
        void dropTitle();

        //! Obtain a symbolic name for the element's schema type
        std::string getTitleSchemaType() const;


    private:
        // elements
        typedef std::vector< ms_quant_specificity* > specificity_vector;
        specificity_vector _specificities;

        ms_quant_composition* _pDelta;
        bool _delta_set;

        typedef std::vector< ms_quant_composition* > composition_vector;
        composition_vector _ignores;

        // attributes
        std::string _title;
        bool _title_set;

    }; // class ms_quant_localdef

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_LOCALDEF_HPP

/*------------------------------- End of File -------------------------------*/
