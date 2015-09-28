/*
##############################################################################
# file: ms_quant_neutralloss.hpp                                             #
# 'msparser' toolkit                                                         #
# Encapsulates "NeutralLoss" element from "quantitation.xml"-file            #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_neutralloss.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_NEUTRALLOSS_HPP
#define MS_QUANT_NEUTRALLOSS_HPP

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

    //! A neutral loss for the sequence ions, e.g. loss of phosphate.
    class MS_MASCOTRESFILE_API ms_quant_neutralloss: public ms_quant_composition
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_neutralloss();

        //! Copying constructor
        ms_quant_neutralloss(const ms_quant_neutralloss& src);

        //! Destructor
        virtual ~ms_quant_neutralloss();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_neutralloss* right);

        //! C++ style assignment operator
        ms_quant_neutralloss& operator=(const ms_quant_neutralloss& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;
        

        //! Indicates presence of "flag" attribute
        bool haveFlag() const;

        //! Returns "flag" attribute value
        bool isFlag() const;

        //! Call this member to set a custom value for "flag"-attribute
        void setFlag(const bool flag);

        //! Deletes "flag" attribute
        void dropFlag();

        //! Obtain a symbolic name for the element's schema type
        std::string getFlagSchemaType() const;

    private:

        bool _flag;
        bool _flag_set;

    }; // class ms_quant_neutralloss

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_NEUTRALLOSS_HPP

/*------------------------------- End of File -------------------------------*/