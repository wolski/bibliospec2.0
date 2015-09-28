/*
##############################################################################
# file: ms_quant_moverz.hpp                                                  #
# 'msparser' toolkit                                                         #
# Encapsulates "moverz" element from "quantitation.xml"-file                 #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_moverz.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2007-02-08 19:28:43 $
##############################################################################
 */

#ifndef MS_QUANT_MOVERZ_HPP
#define MS_QUANT_MOVERZ_HPP

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

namespace matrix_science {

    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Parameter name and value pair
    class MS_MASCOTRESFILE_API ms_quant_moverz: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_moverz();

        //! Copying constructor
        ms_quant_moverz(const ms_quant_moverz& src);

        //! Destructor
        virtual ~ms_quant_moverz();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_moverz* right);

        //! C++ style assignment operator
        ms_quant_moverz& operator=(const ms_quant_moverz& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "monoisotopic" attribute
        bool haveMonoisotopic() const;

        //! Returns "monoisotopic" attribute value
        std::string getMonoisotopic() const;

        //! Call this member to set a custom value for "monoisotopic" attribute
        void setMonoisotopic(const char* value);

        //! Delete "monoisotopic" attribute
        void dropMonoisotopic();

        //! Obtain a symbolic name for the element's schema type
        std::string getMonoisotopicSchemaType() const;


        //! Indicates presence of "average" attribute
        bool haveAverage() const;

        //! Returns "average"-attribute value
        std::string getAverage() const;

        //! Call this member to set a custom value for "average"-attribute
        void setAverage(const char* value);

        //! Delete "average" attribute
        void dropAverage();

        //! Obtain a symbolic name for the element's schema type
        std::string getAverageSchemaType() const;

    private:
        std::string _monoisotopic;
        bool _monoisotopic_set;

        std::string _average;
        bool _average_set;

    }; // class ms_quant_moverz

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_MOVERZ_HPP

/*------------------------------- End of File -------------------------------*/

