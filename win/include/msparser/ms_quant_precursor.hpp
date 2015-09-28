/*
##############################################################################
# file: ms_quant_precursor.hpp                                               #
# 'msparser' toolkit                                                         #
# Encapsulates precursor-element from "quantitation.xml"-file                #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2006 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
 * @(#)$Source: /export/cvsroot/parser/inc/ms_quant_precursor.hpp,v $
 * @(#)$Revision: 1.5 $
 * @(#)$Date: 2006-12-20 14:23:55 $
##############################################################################
 */

#ifndef MS_QUANT_PRECURSOR_HPP
#define MS_QUANT_PRECURSOR_HPP

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

    //! An object of this class represent a single "precursor" element in quantitation.xml
    /*!
     * Serves as a possible sub-element of "protocol" element
     */
    class MS_MASCOTRESFILE_API ms_quant_precursor: public ms_quant_parameters
    {
        friend class ms_quant_xmlloader;
    public:
        //! Default constructor
        ms_quant_precursor();

        //! Copying constructor
        ms_quant_precursor(const ms_quant_precursor& src);

        //! Destructor
        virtual ~ms_quant_precursor();

        //! Call this member if you want to start again
        void defaultValues();

        //! Copies all content from another instance
        void copyFrom(const ms_quant_precursor* right);

        //! C++ style assignment operator
        ms_quant_precursor& operator=(const ms_quant_precursor& right);


        //! Returns name of the schema type that can be used to validate this element
        virtual std::string getSchemaType() const;

        //! Performs simple validation of the top-level elements only
        virtual std::string validateShallow(const ms_xml_schema *pSchemaFileObj) const;

        //! Performs validation of all child elements in addition to 'shallow' validation
        virtual std::string validateDeep(const ms_xml_schema *pSchemaFileObj) const;


        //! Indicates presence of "allow_mass_time_match" attribute
        bool haveAllowMassTimeMatch() const;

        //! Returns "allow_mass_time_match" attribute value
        bool isAllowMassTimeMatch() const;

        //! Call this member to set a custom value for "allow_mass_time_match" attribute
        void setAllowMassTimeMatch(const bool value);

        //! Delete "allow_mass_time_match" attribute
        void dropAllowMassTimeMatch();

        //! Obtain a symbolic name for the element's schema type
        std::string getAllowMassTimeMatchSchemaType() const;


        //! Indicates presence of "allow_elution_shift" attribute
        bool haveAllowElutionShift() const;

        //! Returns "allow_elution_shift" attribute value
        bool isAllowElutionShift() const;

        //! Call this member to set a custom value for "allow_elution_shift" attribute
        void setAllowElutionShift(const bool value);

        //! Delete "allow_elution_shift" attribute
        void dropAllowElutionShift();

        //! Obtain a symbolic name for the attribute's schema type
        std::string getAllowElutionShiftSchemaType() const;


        //! Indicates presence of "all_charge_states" attribute
        bool haveAllChargeStates() const;

        //! Returns "all_charge_states" attribute value
        bool isAllChargeStates() const;

        //! Call this member to set a custom value for "all_charge_states" attribute
        void setAllChargeStates(const bool value);

        //! Delete "all_charge_states" attribute
        void dropAllChargeStates();

        //! Obtain a symbolic name for the attribute's schema type
        std::string getAllChargeStatesSchemaType() const;

    private:

        bool _allowMassTimeMatch;
        bool _allowMassTimeMatch_set;

        bool _allowElutionShift;
        bool _allowElutionShift_set;

        bool _allChargeStates;
        bool _allChargeStates_set;

    }; // class ms_quant_precursor

    /** @} */ // end of config_group

} // namespace matrix_science

#endif // MS_QUANT_PRECURSOR_HPP

/*------------------------------- End of File -------------------------------*/
