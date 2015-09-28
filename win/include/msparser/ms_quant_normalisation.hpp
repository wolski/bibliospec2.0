/*
##############################################################################
# file: ms_quant_normalisation.hpp                                           #
# 'msparser' toolkit                                                         #
# Encapsulates normalisation-element from "quantitation.xml"-file            #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_normalisation.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_NORMALISATION_HPP
#define MS_QUANT_NORMALISATION_HPP

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

    //! An object of this class represent a single "normalisation" element in quantitation.xml
    /*!
     * Method of normailising ratios for a complete data set.
     */
    class MS_MASCOTRESFILE_API ms_quant_normalisation: public ms_quant_parameters
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_normalisation();

        //! Copying constructor
        ms_quant_normalisation(const ms_quant_normalisation& src);

        //! Destructor
        virtual ~ms_quant_normalisation();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_normalisation* right);

        //! C++ style assignment operator
        ms_quant_normalisation& operator=(const ms_quant_normalisation& right);


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

    private:

        std::string _method;
        bool _method_set;

    }; // class ms_quant_normalisation

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_NORMALISATION_HPP

/*------------------------------- End of File -------------------------------*/

