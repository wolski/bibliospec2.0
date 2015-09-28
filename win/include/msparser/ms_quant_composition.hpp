/*
##############################################################################
# file: ms_quant_composition.hpp                                             #
# 'msparser' toolkit                                                         #
# Encapsulates "compositionType" from "quantitation.xml"-file                #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_composition.hpp,v $
 * @(#)$Revision: 1.7 $
 * @(#)$Date: 2008-04-04 17:17:55 $
##############################################################################
 */

#ifndef MS_QUANT_COMPOSITION_HPP
#define MS_QUANT_COMPOSITION_HPP

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

    //! "element" sub-element of "compositionType" in quantitation.xml
    class MS_MASCOTRESFILE_API ms_quant_element: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
#ifdef __MINGW__
        //! Default constructor. MINGW compiler crashes without 'inline'
        inline ms_quant_element();
#else
        //! Default constructor
        ms_quant_element();
#endif

        //! Copying constructor
        ms_quant_element(const ms_quant_element& src);

        //! Destructor
        virtual ~ms_quant_element();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_element* right);

        //! C++ style assignment operator
        ms_quant_element& operator=(const ms_quant_element& right);

        
        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "symbol" attribute
        bool haveSymbol() const;

        //! Returns "symbol" attribute value
        std::string getSymbol() const;

        //! Call this member to set a custom value for "symbol"-attribute
        void setSymbol(const char* symbol);

        //! Deletes "symbol" attribute
        void dropSymbol();

        //! Obtain a symbolic name for the element's schema type
        std::string getSymbolSchemaType() const;

        
        //! Indicates presence of "number" attribute
        bool haveNumber() const;

        //! Returns "number" attribute value
        int getNumber() const;

        //! Call this member to set a custom value for "number"-attribute
        void setNumber(const int number);

        //! Deletes "number" attribute
        void dropNumber();

        //! Obtain a symbolic name for the element's schema type
        std::string getNumberSchemaType() const;


    private:
        std::string _symbol;
        bool _symbol_set;

        int _number;
        bool _number_set;

    }; // class ms_quant_element

    //! Describes "compositionType" type in quantitation.xml document
    /*!
     * Objects of this type host a list of nested "element" elements.
     */
    class MS_MASCOTRESFILE_API ms_quant_composition: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_composition();

        //! Copying constructor
        ms_quant_composition(const ms_quant_composition& src);

        //! Destructor
        virtual ~ms_quant_composition();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_composition* right);

        //! C++ style assignment operator
        ms_quant_composition& operator=(const ms_quant_composition& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of elements held
        int getNumberOfElements() const;

        //! Deletes all elements from the list
        void clearElements();

        //! Adds a new element at the end of the list
        void appendElement(const ms_quant_element *element);

        //! Returns an element object by its number
        const ms_quant_element * getElement(const int idx) const;

        //! Update the information for a specific element
        bool updateElement(const int idx, const ms_quant_element* element);

        //! Remove an element from the list
        bool deleteElement(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getElementSchemaType() const;

    private:
        typedef std::vector< ms_quant_element* > element_vector;
        element_vector _elements;

    }; // class ms_quant_composition

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_COMPOSITION_HPP

/*------------------------------- End of File -------------------------------*/
