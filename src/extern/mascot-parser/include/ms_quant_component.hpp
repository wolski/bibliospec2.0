/*
##############################################################################
# file: ms_quant_component.hpp                                               #
# 'msparser' toolkit                                                         #
# Encapsulates component-element from "quantitation.xml"-file                #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_component.hpp,v $
 * @(#)$Revision: 1.6 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_COMPONENT_HPP
#define MS_QUANT_COMPONENT_HPP

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

    class ms_quant_modgroup; // forward declaration
    class ms_quant_moverz; // forward declaration
    class ms_quant_correction; // forward declaration
    class ms_quant_isotope; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! An object of this class represent a single component element in quantitation.xml
    /*!
     * Identifies a component used to calculate a ratio.
     */
    class MS_MASCOTRESFILE_API ms_quant_component: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_component();

        //! Copying constructor
        ms_quant_component(const ms_quant_component& src);

        //! Destructor
        virtual ~ms_quant_component();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_component* right);

        //! C++ style assignment operator
        ms_quant_component& operator=(const ms_quant_component& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Check for presence of "moverz" element
        bool haveMoverz() const;

        //! Returns "moverz" element value
        const ms_quant_moverz* getMoverz() const;

        //! Call this member to supply a custom content for "moverz" element
        void setMoverz(const ms_quant_moverz* moverz);

        //! Delete "moverz" element
        void dropMoverz();

        //! Obtain a symbolic name for the element's schema type
        std::string getMoverzSchemaType() const;


        //! Returns a number of nested modification groups
        int getNumberOfModificationGroups() const;

        //! Deletes all modification groups from the list
        void clearModificationGroups();

        //! Adds a new modification group at the end of the list
        void appendModificationGroup(const ms_quant_modgroup *item);

        //! Returns a modification group object by its number
        const ms_quant_modgroup * getModificationGroupByNumber(const int idx) const;

        //! Returns a modification group object by its name or NULL in case of not found
        const ms_quant_modgroup * getModificationGroupByName(const char *name) const;

        //! Update the information for a specific modification group refering to it by its index
        bool updateModificationGroupByNumber(const int idx, const ms_quant_modgroup* modgroup);

        //! Update the information for a specific modification group refering to it by its unique name
        bool updateModificationGroupByName(const char *name, const ms_quant_modgroup* modgroup);

        //! Remove a modification group from the list in memory by its index
        bool deleteModificationGroupByNumber(const int idx);

        //! Remove a modification group from the list in memory by its unique name
        bool deleteModificationGroupByName(const char *name);

        //! Obtain a symbolic name for the element's schema type
        std::string getModificationGroupSchemaType() const;


        //! Returns a number of "isotope" elements held
        int getNumberOfIsotopes() const;

        //! Deletes all elements from the list
        void clearIsotopes();

        //! Adds a new "isotope" element at the end of the list
        void appendIsotope(const ms_quant_isotope* isotope);

        //! Returns a "isotope" element object by its number
        const ms_quant_isotope* getIsotope(const int idx) const;

        //! Update the information for a specific "isotope" element
        bool updateIsotope(const int idx, const ms_quant_isotope* isotope);

        //! Remove a "isotope" element from the list
        bool deleteIsotope(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getIsotopeSchemaType() const;



        //! Check for presence of "file_index" element
        bool haveFileIndex() const;

        //! Returns "file_index" element value
        int getFileIndex() const;

        //! Call this member to supply a custom content for "file_index" element
        void setFileIndex(const int file_index);

        //! Delete "file_index" element
        void dropFileIndex();

        //! Obtain a symbolic name for the element's schema type
        std::string getFileIndexSchemaType() const;


        //! Returns a number of "correction" elements held
        int getNumberOfCorrections() const;

        //! Deletes all elements from the list
        void clearCorrections();

        //! Adds a new "correction" element at the end of the list
        void appendCorrection(const ms_quant_correction* correction);

        //! Returns a "correction" element object by its number
        const ms_quant_correction* getCorrection(const int idx) const;

        //! Update the information for a specific "correction" element
        bool updateCorrection(const int idx, const ms_quant_correction* isotope);

        //! Remove a "correction" element from the list
        bool deleteCorrection(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getCorrectionSchemaType() const;


        //! Check for presence of "name" attribute
        bool haveName() const;

        //! Returns "name" attribute value
        std::string getName() const;

        //! Call this member to set a custom value for "name" attribute
        void setName(const char* value);

        //! Delete "name" attribute
        void dropName();

        //! Obtain a symbolic name for the element's schema type
        std::string getNameSchemaType() const;

    private:
        // elements
        ms_quant_moverz *_pMoverz;
        bool _moverz_set;

        typedef std::vector< ms_quant_modgroup* > modgroup_vector;
        modgroup_vector _modgroups;

        typedef std::vector< ms_quant_isotope* > isotope_vector;
        isotope_vector _isotopes;

        int _fileIndex;
        bool _fileIndex_set;

        typedef std::vector< ms_quant_correction* > correction_vector;
        correction_vector _corrections;

        // attributes
        std::string _name;
        bool _name_set;

    }; // class ms_quant_component

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_COMPONENT_HPP

/*------------------------------- End of File -------------------------------*/

