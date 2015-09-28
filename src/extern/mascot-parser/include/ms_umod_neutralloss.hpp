/*
##############################################################################
# file: ms_umod_neutralloss.hpp                                              #
# 'msparser' toolkit                                                         #
# Represents 'NeutralLoss' element in unimod.xml file                        #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_umod_neutralloss.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_UMOD_NEUTRALLOSS_HPP
#define MS_UMOD_NEUTRALLOSS_HPP

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
    class ms_umod_configfile;
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents "NeutralLoss" and "PepNeutralLoss" objects in unimod.xml
    /*!
     *  The only difference now between composition-object and this object is the attribute 'flag'
     * (which can be 'required' attribute for 'PepNeutralLoss'-element).
     */
    class MS_MASCOTRESFILE_API ms_umod_neutralloss: public ms_umod_composition
    {
        friend class ms_umod_xmlloader;

    public:
        //! Default constructor
        ms_umod_neutralloss();

        //! Copying constructor
        ms_umod_neutralloss(const ms_umod_neutralloss& src);

        //! Copying constructor
        ms_umod_neutralloss(const ms_quant_neutralloss& src, const ms_umod_configfile& umodFile);

        //! Copying constructor
        ms_umod_neutralloss(const ms_quant_pepneutralloss& src, const ms_umod_configfile& umodFile);

        //! Destructor
        virtual ~ms_umod_neutralloss();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_umod_neutralloss* right);

        //! Copies all content from another object
        void copyFrom(const ms_quant_neutralloss* right, const ms_umod_configfile& umodFile);

        //! Copies all content from another object
        void copyFrom(const ms_quant_pepneutralloss* right, const ms_umod_configfile& umodFile);

        //! C++ style assignment operator
        ms_umod_neutralloss& operator=(const ms_umod_neutralloss& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of 'flag' attribute
        bool haveFlag() const;

        //! Returns 'flag' attribute value
        bool isFlag() const;

        //! Call this member to set a custom value for 'flag' attribute
        void setFlag(const bool value);

        //! Delete 'flag' attribute
        void dropFlag();

        //! Obtain a symbolic name for the element's schema type
        std::string getFlagSchemaType() const;

    private:

        bool _flag;
        bool _flag_set;
    }; // class ms_umod_neutralloss

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_UMOD_NEUTRALLOSS_HPP

/*------------------------------- End of File -------------------------------*/
