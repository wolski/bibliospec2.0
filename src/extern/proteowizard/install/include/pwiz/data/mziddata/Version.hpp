//
// $Id: Version.hpp 1614 2009-12-11 21:24:26Z chambm $
//
//
// Original author: Matt Chambers <matt.chambers .@. vanderbilt.edu>
//
// Copyright 2009 Vanderbilt University - Nashville, TN 37232
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


#ifndef _PWIZ_MZIDDATA_VERSION_HPP_
#define _PWIZ_MZIDDATA_VERSION_HPP_

#include <string>

namespace pwiz {
namespace mziddata {

struct Version
{
    static int Major();
    static int Minor();
    static int Revision();
    static std::string str();
    static std::string LastModified();
};

} // namespace mziddata
} // namespace pwiz

#endif // _PWIZ_MZIDDATA_VERSION_HPP_
