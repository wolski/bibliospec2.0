/*
##############################################################################
# file: ms_quant_modgroup.hpp                                                #
# 'msparser' toolkit                                                         #
# Encapsulates modification_group-element from "quantitation.xml"-file       #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_modgroup.hpp,v $
 * @(#)$Revision: 1.4 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_MODGROUP_HPP
#define MS_QUANT_MODGROUP_HPP

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

    class ms_quant_unmodified; // forward declaration
    class ms_quant_localdef; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! An object of this class represent a single modification group element in quantitation.xml
    /*!
     * Grouping to enable mode constraint (e.g. exclusive) to apply to a set of modifications
     */
    class MS_MASCOTRESFILE_API ms_quant_modgroup: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;

    public:
        //! Default constructor
        ms_quant_modgroup();

        //! Copying constructor
        ms_quant_modgroup(const ms_quant_modgroup& src);

        //! Destructor
        virtual ~ms_quant_modgroup();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_modgroup* right);

        //! C++ style assignment operator
        ms_quant_modgroup& operator=(const ms_quant_modgroup& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Returns a number of nested "mod_file" elements
        int getNumberOfModFiles() const;

        //! Deletes all "mod_file" elements from the list
        void clearModFiles();

        //! Adds a new "mod_file" at the end of the list
        void appendModFile(const char* mod);

        //! Return zero-based index of a mod_file if it is found in the list, -1 otherwise
        int findModFile(const char* mod) const;

        //! Returns "mod_file" element content by its number
        std::string getModFile(const int idx) const;

        //! Removes "mod_file" element from the list
        bool deleteModFile(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getModFileSchemaType() const;


        //! Returns a number of nested "unmodified" elements
        int getNumberOfUnmodified() const;

        //! Deletes all "unmodified" elements from the list
        void clearUnmodified();

        //! Adds a new "unmodified" element at the end of the list
        void appendUnmodified(const ms_quant_unmodified* unmodified);

        //! Returns "unmodified" element object by its number
        const ms_quant_unmodified* getUnmodified(const int idx) const;

        //! Supply new content for one of the "unmodified" elements in the list
        bool updateUnmodified(const int idx, const ms_quant_unmodified* unmodified);

        //! Removes "unmodified" element from the list
        bool deleteUnmodified(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getUnmodifiedSchemaType() const;


        //! Returns a number of nested "local_definition" elements
        int getNumberOfLocalDefinitions() const;

        //! Deletes all "local_definition" elements from the list
        void clearLocalDefinitions();

        //! Adds a new "local_definition" element at the end of the list
        void appendLocalDefinition(const ms_quant_localdef* localdef);

        //! Returns "local_definition" element object by its number
        const ms_quant_localdef* getLocalDefinition(const int idx) const;

        //! Supply new content for one of the "local_definition" elements in the list
        bool updateLocalDefinition(const int idx, const ms_quant_localdef* localdef);

        //! Removes "local_definition" element from the list
        bool deleteLocalDefinition(const int idx);

        //! Obtain a symbolic name for the element's schema type
        std::string getLocalDefinitionSchemaType() const;


        //! Check for presence of "name" attribute"
        bool haveName() const;

        //! Returns "name"-attribute value
        std::string getName() const;

        //! Call this member to set a custom value for "name"-attribute
        void setName(const char* value);

        //! Delete "name" attribute
        void dropName();

        //! Obtain a symbolic name for the element's schema type
        std::string getNameSchemaType() const;


        //! Indicates presence of "mode" attribute
        bool haveMode() const;

        //! Returns "mode" attribute value
        std::string getMode() const;

        //! Call this member to set a custom value for "mode" attribute
        void setMode(const char* mode);

        //! Delete "mode" attribute
        void dropMode();

        //! Obtain a symbolic name for the element's schema type
        std::string getModeSchemaType() const;

    private:

        typedef std::vector< std::string > modfile_vector;
        modfile_vector _modFiles;

        typedef std::vector< ms_quant_unmodified* > unmodified_vector;
        unmodified_vector _unmodified;

        typedef std::vector< ms_quant_localdef* > localdef_vector;
        localdef_vector _localdefs;

        std::string _name;
        bool _name_set;

        std::string _mode;
        bool _mode_set;

    }; // class ms_quant_modgroup

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_MODGROUP_HPP

/*------------------------------- End of File -------------------------------*/

