/*
##############################################################################
# file: ms_quant_protocol.hpp                                                #
# 'msparser' toolkit                                                         #
# Encapsulates protocol-element from "quantitation.xml"-file                 #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /vol/cvsroot/parser/inc/ms_quant_protocol.hpp,v $
 * @(#)$Revision: 1.7 $
 * @(#)$Date: 2007-01-16 16:11:53 $
##############################################################################
 */

#ifndef MS_QUANT_PROTOCOL_HPP
#define MS_QUANT_PROTOCOL_HPP

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

    class ms_quant_parameters; // forward declaration
    class ms_quant_reporter; // forward declaration
    class ms_quant_precursor; // forward declaration
    class ms_quant_replicate; // forward declaration
    class ms_quant_average; // forward declaration
    class ms_quant_multiplex; // forward declaration
    class ms_xml_schema; // forward declaration

    /** @addtogroup config_group
     *  
     *  @{
     */

    //! An object of this class represent "protocol" element in quantitation.xml
    /*!
     * 6 different protocols are currently supported, and an object of this class is 
     * a container for exactly one of those protocols. The getType function returns
     * a string which contains the protocol for the current quantitation method.
     * Use the return value from getType to determine which of the functions
     * to call to get further protocol details.
     */
    class MS_MASCOTRESFILE_API ms_quant_protocol: public ms_xml_IValidatable
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_protocol();

        //! Copying constructor
        ms_quant_protocol(const ms_quant_protocol& src);

        //! Destructor
        virtual ~ms_quant_protocol();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_protocol* right);

        //! C++ style assignment operator
        ms_quant_protocol& operator=(const ms_quant_protocol& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Call this method first to determine which sub element to retrieve
        std::string getType() const;

        //! Sets content of the "protocol" element to NULL and deletes all previously held information
        void setNull();


        //! Returns "reporter" element or NULL if not available
        const ms_quant_reporter* getReporter() const;

        //! Call this member to override content with this "reporter" element
        void setReporter(const ms_quant_reporter* reporter);

        //! Obtain a symbolic name for the element's schema type
        std::string getReporterSchemaType() const;


        //! Returns "precursor" element or NULL if not available
        const ms_quant_precursor* getPrecursor() const;

        //! Call this member to override content with this "precursor" element
        void setPrecursor(const ms_quant_precursor* precursor);

        //! Obtain a symbolic name for the element's schema type
        std::string getPrecursorSchemaType() const;


        //! Returns "multiplex" element or NULL if not available
        const ms_quant_multiplex* getMultiplex() const;

        //! Call this member to override content with this "multiplex" element
        void setMultiplex(const ms_quant_multiplex* multiplex);

        //! Obtain a symbolic name for the element's schema type
        std::string getMultiplexSchemaType() const;


        //! Returns "replicate" element or NULL if not available
        const ms_quant_replicate* getReplicate() const;

        //! Call this member to override content with this "replicate" element
        void setReplicate(const ms_quant_replicate* replicate);

        //! Obtain a symbolic name for the element's schema type
        std::string getReplicateSchemaType() const;


        //! Returns "average" element or NULL if not available
        const ms_quant_average* getAverage() const;

        //! Call this member to override content with this "average" element
        void setAverage(const ms_quant_average* average);

        //! Obtain a symbolic name for the element's schema type
        std::string getAverageSchemaType() const;
    private:

        // switch between choices
        std::string _protocolType;

        ms_quant_reporter   *_pReporter;
        ms_quant_precursor  *_pPrecursor;
        ms_quant_multiplex  *_pMultiplex;
        ms_quant_replicate  *_pReplicate;
        ms_quant_average    *_pAverage;

    }; // class ms_quant_protocol

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_PROTOCOL_HPP

/*------------------------------- End of File -------------------------------*/
