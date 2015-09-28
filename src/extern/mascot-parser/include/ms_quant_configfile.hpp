/*
##############################################################################
# file: ms_quant_configfile.hpp                                              #
# 'msparser' toolkit                                                         #
# Encapsulates "quantitation.xml"-file that serves as quantitation       #
# configuration file                                                         #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_configfile.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2006-12-29 15:31:09 $
##############################################################################
 */

#ifndef MS_QUANT_CONFIGFILE_HPP
#define MS_QUANT_CONFIGFILE_HPP

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

    class ms_quant_method; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Use this class in order to read/write quantitation.xml file
    /*! 
     *  This file defines all necessary configuration information for performing
     *  quantitation experiments with Mascot.
     *  After reading a file and before using the object check validity by calling 
     *  #isValid()-member and retrieve error descriptions with #getLastErrorString() 
     *  if not valid. Similarly, after writing out the file #isValid() can be used to
     *  check if the file has been created correctly and is valid for further use.
     */
    class MS_MASCOTRESFILE_API ms_quant_configfile: public ms_errors
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_configfile();

        //! Copying constructor
        ms_quant_configfile(const ms_quant_configfile& src);

        //! Cconstructor that reads the given file on construction
        ms_quant_configfile(const char* fileName, const char* schemaFileName,
                   const ms_connection_settings * cs = 0);

        //! Destructor
        virtual ~ms_quant_configfile();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_configfile* right);

        //! C++ style assignment operator
        ms_quant_configfile& operator=(const ms_quant_configfile& right);

        //! Call this member to set a custom file name to read from a different location
        void setFileName(const char* name);

        //! Returns a file name that is used to read configuration information
        std::string getFileName() const;

        //! Call this member to specify a custom schema file name for validating xml-file
        void setSchemaFileName(const char* name);

        //! Returns the current schema file name that is used to validate xml-file
        std::string getSchemaFileName() const;

        //! Sets the sessionID and proxy server for use with an http transfer
        void setConnectionSettings(const ms_connection_settings & cs);

        //! Returns the sessionID and proxy server for use with an http transfer
        ms_connection_settings getConnectionSettings() const;

        //! Reads configuration information from the file.
        void read_file();

        //! Stores modification information in the file
        void save_file();

        //! Reads configuration information from a buffer.
        void read_buffer(const char *buffer);

        //! Creates a string representation of the xml-document with all the content of the object
        std::string save_buffer();

        //! Validates the whole document against schema and return errors as a string
        std::string validateDocument() const;

        //! Returns a number of quantitation methods currently held in memory
        int getNumberOfMethods() const;

        //! Deletes all quantitation methods from the list
        void clearMethods();

        //! Adds a new quantitation method at the end of the list
        void appendMethod(const ms_quant_method *item);

        //! Returns a quantitation method object by its number
        const ms_quant_method * getMethodByNumber(const int idx) const;

        //! Returns a quantitation method object by its name or NULL in case of not found
        const ms_quant_method * getMethodByName(const char *name) const;

        //! Update the information for a specific quantitation method
        bool updateMethodByNumber(const int idx, const ms_quant_method *mod);

        //! Update the information for a specific quantitation method
        bool updateMethodByName(const char *name, const ms_quant_method *mod);

        //! Remove a quantitation method from the list in memory
        bool deleteMethodByNumber(const int idx);

        //! Remove a quantitation method from the list in memory
        bool deleteMethodByName(const char *name);

    private:
        void read_xml(const bool bHttp = false);

        typedef std::vector<ms_quant_method* > entries_vector;
        entries_vector          _entries;

        std::string             _filename;
        std::string             _schemaFileName;
        ms_connection_settings  _cs;
    }; // class ms_quant_configfile

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_CONFIGFILE_HPP

/*------------------------------- End of File -------------------------------*/

