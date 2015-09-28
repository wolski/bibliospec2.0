//
// $Id: ChromatogramList_mzML.hpp 1189 2009-08-14 17:36:06Z chambm $
//
//
// Original author: Darren Kessner <darren@proteowizard.org>
//
// Copyright 2008 Spielberg Family Center for Applied Proteomics
//   Cedars-Sinai Medical Center, Los Angeles, California  90048
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


#ifndef _CHROMATOGRAMLIST_MZML_HPP_
#define _CHROMATOGRAMLIST_MZML_HPP_


#include "pwiz/utility/misc/Export.hpp"
#include "ChromatogramListBase.hpp"
#include <iosfwd>


namespace pwiz {
namespace msdata {


/// implementation of ChromatogramList, backed by an mzML file
class PWIZ_API_DECL ChromatogramList_mzML : public ChromatogramListBase
{
    public:

    static ChromatogramListPtr create(boost::shared_ptr<std::istream> is,
                                      const MSData& msd,
                                      bool indexed = true);
};


} // namespace msdata
} // namespace pwiz


#endif // _CHROMATOGRAMLIST_MZML_HPP_
