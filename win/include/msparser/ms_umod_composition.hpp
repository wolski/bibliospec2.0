/*
##############################################################################
# file: ms_umod_composition.hpp                                                #
# 'msparser' toolkit                                                         #
# Represents 'composition_t' type from unimod.xml file                       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_umod_composition.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_UMOD_COMPOSITION_HPP
#define MS_UMOD_COMPOSITION_HPP

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
    class ms_quant_composition; // forward declaration
    class ms_quant_component;  // forward declaration
    class ms_umod_configfile;  // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents 'composition' type in unimod.xml
    /*!
     *  This type is used for modification delta and 'ignore' elements in unimod.xml
     */
    class MS_MASCOTRESFILE_API ms_umod_composition: public ms_xml_IValidatable
    {
        friend class ms_umod_xmlloader;
        friend class ms_umod_configfile;

    public:
        //! Default constructor
        ms_umod_composition();

        //! Copying constructor
        ms_umod_composition(const ms_umod_composition& src);

        //! Copying constructor
        ms_umod_composition(const ms_quant_composition& src, const ms_umod_configfile& umodFile);

        //! Recalculates all deltas with isotope-substitution according to selected component in quantification.xml
        void updateMasses(const ms_umod_configfile& umodFile, const ms_quant_component& quantComp);

        //! Destructor
        virtual ~ms_umod_composition();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_umod_composition* right);

        //! Copies all content from another object
        void copyFrom(const ms_quant_composition* right, const ms_umod_configfile& umodFile);

        //! C++ style assignment operator
        ms_umod_composition& operator=(const ms_umod_composition& right);


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


        //! Indicates presence of 'composition' attribute
        bool haveCompositionString() const;

        //! Returns 'composition' attribute value
        std::string getCompositionString() const;

        //! Call this member to set a custom value for 'composition' attribute
        void setCompositionString(const char* value);

        //! Delete 'composition' attribute
        void dropCompositionString();

        //! Obtain a symbolic name for the element's schema type
        std::string getCompositionStringSchemaType() const;


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

        std::string _composition;
        bool _composition_set;

        double _avgeMass;
        bool _avgeMass_set;

        double _monoMass;
        bool _monoMass_set;
    }; // class ms_umod_composition

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_UMOD_COMPOSITION_HPP

/*------------------------------- End of File -------------------------------*/
