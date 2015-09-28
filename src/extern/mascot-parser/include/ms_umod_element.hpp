/*
##############################################################################
# file: ms_umod_element.hpp                                                  #
# 'msparser' toolkit                                                         #
# Represents chemical element information from unimod.xml file               #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_umod_element.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_UMOD_ELEMENT_HPP
#define MS_UMOD_ELEMENT_HPP

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

    //! Represents "element" object in unimod.xml
    class MS_MASCOTRESFILE_API ms_umod_element: public ms_xml_IValidatable
    {
        friend class ms_umod_xmlloader;
        friend class ms_umod_configfile;

    public:
        //! Default constructor
        ms_umod_element();

        //! Copying constructor
        ms_umod_element(const ms_umod_element& src);

        //! Destructor
        virtual ~ms_umod_element();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_umod_element* right);

        //! C++ style assignment operator
        ms_umod_element& operator=(const ms_umod_element& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "title" attribute
        bool haveTitle() const;

        //! Returns "title" attribute value
        std::string getTitle() const;

        //! Call this member to set a custom value for "title" attribute
        void setTitle(const char* value);

        //! Delete "title" attribute
        void dropTitle();

        //! Obtain a symbolic name for the element's schema type
        std::string getTitleSchemaType() const;


        //! Indicates presence of "full_name" attribute
        bool haveFullName() const;

        //! Returns "full_name"-attribute value
        std::string getFullName() const;

        //! Call this member to set a custom value for "full_name"-attribute
        void setFullName(const char* value);

        //! Delete "full_name" attribute
        void dropFullName();

        //! Obtain a symbolic name for the element's schema type
        std::string getFullNameSchemaType() const;


        //! Indicates presence of "avge_mass" attribute
        bool haveAvgeMass() const;

        //! Returns "avge_mass"-attribute value as a string
        std::string getAvgeMass() const;

        //! Returns "avge_mass"-attribute value as a floating point number
        double getAvgeMassAsNumber() const;

        //! Call this member to set a custom string value for "avge_mass"-attribute
        bool setAvgeMass(const char* value, ms_errs* err = NULL);

        //! Delete "avge_mass" attribute
        void dropAvgeMass();

        //! Obtain a symbolic name for the element's schema type
        std::string getAvgeMassSchemaType() const;


        //! Indicates presence of "mono_mass" attribute
        bool haveMonoMass() const;

        //! Returns "mono_mass"-attribute value as a string
        std::string getMonoMass() const;

        //! Returns "mono_mass"-attribute value as a floating point number
        double getMonoMassAsNumber() const;

        //! Call this member to set a custom string value for "mono_mass"-attribute
        bool setMonoMass(const char* value, ms_errs* err = NULL);

        //! Delete "mono_mass" attribute
        void dropMonoMass();

        //! Obtain a symbolic name for the element's schema type
        std::string getMonoMassSchemaType() const;

    private:
        std::string _title;
        bool _title_set;

        std::string _fullName;
        bool _fullName_set;

        std::string _avgeMass;
        double _avgeMassDouble;
        bool _avgeMass_set;

        std::string _monoMass;
        double _monoMassDouble;
        bool _monoMass_set;
    }; // class ms_umod_element

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_UMOD_ELEMENT_HPP

/*------------------------------- End of File -------------------------------*/
