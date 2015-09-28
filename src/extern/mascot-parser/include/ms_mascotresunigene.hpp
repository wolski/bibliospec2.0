/*
##############################################################################
# file: ms_mascotrespeptide.hpp                                              #
# 'msparser' toolkit                                                         #
# Encapsulates a Unigene file (from NCBI)                                    #
#                                                                            #
##############################################################################
# COPYRIGHT NOTICE                                                           #
# Copyright 1998-2003 Matrix Science Limited  All Rights Reserved.           #
#                                                                            #
##############################################################################
#    $Archive:: /Mowse/ms_mascotresfile/include/ms_mascotresunigene.hpp    $ #
#     $Author: davidc $ #
#       $Date: 2008-04-04 17:17:55 $ #
#   $Revision: 1.7 $ #
# $NoKeywords::                                                            $ #
##############################################################################
*/

#ifndef MS_MASCOTRESUNIGENE_HPP
#define MS_MASCOTRESUNIGENE_HPP

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

// Includes from the standard template library
#include <stdio.h>
#include <string>
#include <vector>



namespace matrix_science {
    /** @addtogroup resfile_group
     *  
     *  @{
     */

    //! This class encapsulates a single entry from a unigene file
    /*!
     * A number of objects of this class will be created when
     * a ms_unigene object is created.
     *
     */
    class MS_MASCOTRESFILE_API ms_unigene_entry
    {
        public:
#ifdef __MINGW__
            //! The constructor should only be called from within the library. MINGW compiler crashes without 'inline'
            inline ms_unigene_entry(FILE * f, ms_unigene & unigene);
#else
            //! The constructor should only be called from within the library
            ms_unigene_entry(FILE * f, ms_unigene & unigene);
#endif

            //! Return the number of accessions (gi numbers) that comprise this entry
            int getNumAccessions() const     { return (int)accessions_.size(); }

            //! Return the 'ID' of this entry - e.g. Hs.4
            std::string getID() const        { return id_;                }

            //! Return the 'title' of this entry - e.g. "alcohol dehydrogenase..."
            std::string getTitle() const     { return title_;             }

            //! Return the gene name for this entry - e.g. "ADH1B"
            std::string getGene() const      { return gene_;              }

            //! Return the CYTOBAND - e.g. 4q21-q23
            std::string getCytoBand() const  { return cytoBand_;          }

            //! Return the LocusLink - e.g. 125
            std::string getLocusLink() const { return locuslink_;         }

            //! Return the EXPRESS entry. Can be very long - 5000 bytes
            std::string getExpress() const   { return express_;           }

            //! Return the chromosome that contains the unignene entry
            long getChromosome() const       { return chromosome_;        }

        protected:
            // Not safe to copy or assign this object.
            ms_unigene_entry(const ms_unigene_entry & rhs);
            ms_unigene_entry & operator=(const ms_unigene_entry & rhs);

        private:
            //! Private function to parse a string from an input line
            bool getString(const char * buf, const char * id,
                           const int idLen,  std::string &res);

            //! Private function to parse a long integer from an input line
            bool getLong(const char * buf, const char * id, 
                         const int idLen,  long &res);

            //! Private function to parse a gi number from an input line
            bool getGInum(const char * buf, const char * id, 
                          const int idLen,  unsigned long &res);
            std::string id_;
            std::string title_;
            std::string gene_;
            std::string cytoBand_;
            std::string locuslink_;
            std::string express_;
            long        chromosome_;
            std::vector<unsigned long> accessions_;
    };

    //! This class encapsulates a complete unigene file
    /*!
     * Creating one of these objects reads in the unigene file,
     * creating a number of ms_unigene_entry objects. The gi accession
     * numbers will be indexed.
     */
    class MS_MASCOTRESFILE_API ms_unigene
    {
        public:
            //! The constructor for a unignene object
            ms_unigene(ms_mascotresfile  &resfile, const char * filename);

            //! Given an accession, return a pointer to the relevant unigene entry
            const ms_unigene_entry * findEntry(const char * accession) const;

            //! Used internally to create a lookup of gi numbers to a list of accessions
            void addAccessionUnigenePair(const ms_unigene_entry * unigene, 
                                         unsigned long giNumber);

            //! Return the Unigene 'accession' (ID) for a given gi| number
            std::string getUnigeneForAccession(const std::string giNumber, 
                                               const int index);

        protected:
            // Not safe to copy or assign this object.
            ms_unigene(const ms_unigene & rhs);
            ms_unigene & operator=(const ms_unigene & rhs);

    private:
            ms_mascotresfile  &resfile_;
            std::vector<ms_unigene_entry *> entries_;
            std::string filename_;
            int numAccessions_;
            typedef std::multimap<unsigned long, const ms_unigene_entry *> unigenesForAcc;
            unigenesForAcc accessionToUnigene_;
    };
    /** @} */ // end of resfile_group
}   // matrix_science namespace

#endif // MS_MASCOTRESUNIGENE_HPP

/*------------------------------- End of File -------------------------------*/
