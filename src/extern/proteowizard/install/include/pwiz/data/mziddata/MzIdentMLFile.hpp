//
// $Id: MzIdentMLFile.hpp 1430 2009-10-19 22:05:34Z broter $
//
//
// Original author: Robert Burke <robert.burke@proteowizard.org>
//
// Copyright 2009 Spielberg Family Center for Applied Proteomics
//   University of Southern California, Los Angeles, California  90033
//
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software 
// distributed under the License is distributed on an "AS IS" BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and 
// limitations under the License.
//


#ifndef _MZIDENTMLFILE_HPP_
#define _MZIDENTMLFILE_HPP_


#include "pwiz/utility/misc/Export.hpp"
#include "MzIdentML.hpp"
#include "Reader.hpp"


namespace pwiz {
namespace mziddata {


/// Mziddata object plus file I/O
struct PWIZ_API_DECL MzIdentMLFile : public MzIdentML
{
    /// constructs Mziddata object backed by file;
    /// reader==0 -> use DefaultReaderList 
    MzIdentMLFile(const std::string& filename, 
                 const Reader* reader = 0);

    /// data format for write()
    enum PWIZ_API_DECL Format {Format_Text, Format_MzIdentML};

    /// configuration for write()
    struct PWIZ_API_DECL WriteConfig
    {
        Format format;

        WriteConfig(Format format = Format_MzIdentML)
        :   format(format)
        {}
    };

    /// static write function for any Mziddata object;
    static void write(const MzIdentML& mzid,
                      const std::string& filename,
                      const WriteConfig& config = WriteConfig());

    /// member write function 
    void write(const std::string& filename,
               const WriteConfig& config = WriteConfig());
};


PWIZ_API_DECL std::ostream& operator<<(std::ostream& os, MzIdentMLFile::Format format);
PWIZ_API_DECL std::ostream& operator<<(std::ostream& os, const MzIdentMLFile::WriteConfig& config);


} // namespace mziddata
} // namespace pwiz


#endif // _MZIDENTMLFILE_HPP_
