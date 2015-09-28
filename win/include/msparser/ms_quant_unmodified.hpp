/*
##############################################################################
# file: ms_quant_unmodified.hpp                                              #
# 'msparser' toolkit                                                         #
# Encapsulates "unmodified" element from "quantitation.xml"-file             #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_unmodified.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_UNMODIFIED_HPP
#define MS_QUANT_UNMODIFIED_HPP

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

    //! Describes "unmodified" element
    class MS_MASCOTRESFILE_API ms_quant_unmodified: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_unmodified();

        //! Copying constructor
        ms_quant_unmodified(const ms_quant_unmodified& src);

        //! Destructor
        virtual ~ms_quant_unmodified();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_unmodified* right);

        //! C++ style assignment operator
        ms_quant_unmodified& operator=(const ms_quant_unmodified& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns string value of the element
        std::string getContent() const;

        //! Call this member to set a custom value for the element
        void setContent(const char* value);

        //! Obtain a symbolic name for the element's schema type
        std::string getContentSchemaType() const;


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
        std::string _content;

        std::string _site;
        bool _site_set;

        std::string _position;
        bool _position_set;

    }; // class ms_quant_unmodified

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_UNMODIFIED_HPP

/*------------------------------- End of File -------------------------------*/
