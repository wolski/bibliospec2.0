/*
##############################################################################
# file: ms_umod_aminoacid.hpp                                                #
# 'msparser' toolkit                                                         #
# Represents 'amino acid' object from unimod.xml file                        #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_umod_aminoacid.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_UMOD_AMINOACID_HPP
#define MS_UMOD_AMINOACID_HPP

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

    class ms_umod_elemref; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents "amino acid" object in unimod.xml
    class MS_MASCOTRESFILE_API ms_umod_aminoacid: public ms_xml_IValidatable
    {
        friend class ms_umod_xmlloader;
        friend class ms_umod_configfile;

    public:
        //! Default constructor
        ms_umod_aminoacid();

        //! Copying constructor
        ms_umod_aminoacid(const ms_umod_aminoacid& src);

        //! Destructor
        virtual ~ms_umod_aminoacid();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_umod_aminoacid* right);

        //! C++ style assignment operator
        ms_umod_aminoacid& operator=(const ms_umod_aminoacid& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of element reference objects currently held in memory
        int getNumberOfElemRefs() const;

        //! Deletes all element reference objects from the list
        void clearElemRefs();

        //! Adds a new element reference object at the end of the list
        void appendElemRef(const ms_umod_elemref *elemref);

        //! Returns a read-only pointer to an element reference object by its number
        const ms_umod_elemref* getElemRef(const int idx) const;

        //! Update the information for a particular element reference object
        bool updateElemRef(const int idx, const ms_umod_elemref *elemref);

        //! Remove an element reference object from the list in memory
        bool deleteElemRef(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getElemRefSchemaType() const;


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


        //! Indicates presence of "three_letter" attribute
        bool haveThreeLetter() const;

        //! Returns "three_letter"-attribute value
        std::string getThreeLetter() const;

        //! Call this member to set a custom value for "three_letter"-attribute
        void setThreeLetter(const char* value);

        //! Delete "three_letter" attribute
        void dropThreeLetter();

        //! Obtain a symbolic name for the element's schema type
        std::string getThreeLetterSchemaType() const;


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

        //! Returns "avge_mass"-attribute value
        double getAvgeMass() const;

        //! Call this member to set a custom value for "avge_mass"-attribute
        void setAvgeMass(const double value);

        //! Delete "avge_mass" attribute
        void dropAvgeMass();

        //! Obtain a symbolic name for the element's schema type
        std::string getAvgeMassSchemaType() const;


        //! Indicates presence of "mono_mass" attribute
        bool haveMonoMass() const;

        //! Returns "mono_mass"-attribute value
        double getMonoMass() const;

        //! Call this member to set a custom value for "mono_mass"-attribute
        void setMonoMass(const double value);

        //! Delete "mono_mass" attribute
        void dropMonoMass();

        //! Obtain a symbolic name for the element's schema type
        std::string getMonoMassSchemaType() const;

    private:

        typedef std::vector< ms_umod_elemref* > elemref_vector;
        elemref_vector _elemRefs;

        std::string _title;
        bool _title_set;

        std::string _threeLetter;
        bool _threeLetter_set;

        std::string _fullName;
        bool _fullName_set;

        double _avgeMass;
        bool _avgeMass_set;

        double _monoMass;
        bool _monoMass_set;
    }; // class ms_umod_aminoacid

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_UMOD_AMINOACID_HPP

/*------------------------------- End of File -------------------------------*/
