/*
##############################################################################
# file: ms_parserule.hpp                                                     #
# 'msparser' toolkit                                                         #
# Represents PARSE section of mascot.dat file                                #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_parserule.hpp           $ #
#     $Author: davidc $ #
#       $Date: 2007-03-05 11:22:01 $ #
#   $Revision: 1.5 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_PARSERULE_HPP
#define MS_PARSERULE_HPP

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

// for the sake of #include <string>
#ifdef __ALPHA_UNIX__
#include <ctype.h>
#endif
#include <string>


namespace matrix_science {
    /** @addtogroup config_group
     *  
     *  @{
     */

    //! Represents a single regular expression parsing rule
    /*!
     *  A number of instances of this class are held in ms_parseoptions class instance
     *  as a list of parse rules specified in <tt>PARSE</tt> section 
     *  of <tt>mascot.dat</tt> file.
     */
    class MS_MASCOTRESFILE_API ms_parserule
    {
        friend class ms_datfile;
        friend class ms_parseoptions;
    public:
        //! Default constructor
        ms_parserule();

        //! Copying constructor
        ms_parserule(const ms_parserule& src);

        //! Destructor
        virtual ~ms_parserule();

        //! Initialises the instance
        void defaultValues();

        //! Copies all contents from another instance
        void copyFrom(const ms_parserule* right);

        //! Assignment operator for C++ client applications
        ms_parserule& operator=(const ms_parserule& right);

        //! Call this member to check if this rule has been specified in the file or just a placeholder
        /*!
         *  Parse rules in <tt>PARSE</tt> section are specified in random order
         *  with their unique numbers. There might be gaps and unspecified rules. 
         *  Therefore, it is recommended to check if the rule is actually available in the file
         *  before retrieving its properties.<BR>
         *  By default it returns <b>FALSE</b>
         *  
         */
        bool isAvailable() const;

        //! Allows to change availability of the rule
        /*!
         *  See #isAvailable() for more information.
         */
        void setAvailable(const bool value);

        //! Returns a string representing the rule
        std::string getRuleStr() const;

        //! Allows to change the rule
        void setRuleStr(const char* str);

    private:
        bool    available_;
        std::string szRule_;

        void *compiledExp;
        void compileAccessionRegex(ms_errs* pErr);
    };// class ms_parserule

    //! Represents <tt>PARSE</tt> section of <tt>mascot.dat</tt> file
    class MS_MASCOTRESFILE_API ms_parseoptions : public ms_customproperty
    {
        friend class ms_datfile;
    public:
        //! Default constructor
        ms_parseoptions();

        //! Copying constructor
        ms_parseoptions(const ms_parseoptions& src);

        //! Destructor
        virtual ~ms_parseoptions();
        
        //! Initializes the instance
        virtual void defaultValues();

        //! Can be used to copy content from another instance
        void copyFrom(const ms_parseoptions* right);

        //! Assignment operator for C++ client applications
        ms_parseoptions& operator=(const ms_parseoptions& right);

        //! Tells whether the section has been actually read from the file
        /*!
         *  By default the <tt>PARSE</tt> section is unavailable until it has 
         *  been set to a different state.
         */
        bool isSectionAvailable() const;

        //! Changes availability of the section, i.e. should it be saved in a file
        void setSectionAvailable(const bool value);

        //! Can be used to get upper limit for a parse rule number
        /*!
         *  The limit is currently hard-coded. Use this function to retrieve its value.
         */
        int getNumberOfParseRules() const;

        //! Makes all rules unavailable
        void clearParseRules();

        //! Returns a parse rule by number
        /*!
         *  Parse rules are not not stored consecutively. They are stored according
         *  to their numbers explicitly specified in the file. So, not every number will
         *  yield an available rule. See ms_parserule#isAvailable().
         *  \param index a number from 0 to (getNumberOfParseRules()-1)
         *  \return an instance of ms_parserule class representing a rule
         */
        const ms_parserule* getParseRule(const int index) const;

        //! Allows to set a new parse rule for a given number
        void setParseRule(const int index, const ms_parserule* rule);

        //! Makes a parse rule with the specified index unavailable
        /*!
         *  Unavailable rules are not shown in the file any more and are not valid entries.
         *  In order to make it available again, create a new rule and put
         *  it in using #setParseRule() member.
         */
        void dropParseRule(const int index);

        int findOrAddParseRule(const char * rule, bool & added);

    private:
        bool                sectionAvailable_;
        ms_parserule        *parseRules_;
    };
    /** @} */ // end of config_group
} // namespace matrix_science

#endif // MS_PARSERULE_HPP

/*------------------------------- End of File -------------------------------*/
