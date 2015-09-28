//
// $Id: DateTime.hpp 1821 2010-02-19 17:00:05Z chambm $
//
//
// Original author: Matt Chambers <matt.chambers .@. vanderbilt.edu>
//
// Copyright 2008 Vanderbilt University - Nashville, TN 37232
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

#ifndef _DATETIME_HPP_
#define _DATETIME_HPP_

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/math/special_functions/modf.hpp>

namespace bdt = boost::date_time;
namespace bpt = boost::posix_time;
namespace blt = boost::local_time;

using bpt::from_time_t;

#ifdef WIN32
using bpt::from_ftime;
#endif

using boost::gregorian::date;


namespace boost {
namespace date_time {

//! Create a time object from an OLE automation date value.
/*! Create a time object from an OLE automation date value.
 * An OLE automation date is implemented as a floating-point number *
 * whose value is the number of days from midnight, 30 December 1899.
 */
template<class time_type>
inline
time_type time_from_OADATE(double oa_date)
{
    typedef typename time_type::date_type date_type;
    typedef typename time_type::date_duration_type date_duration_type;
    typedef typename time_type::time_duration_type time_duration_type;
    using boost::math::modf;

    static const date_type base_date(1899, Dec, 30);
    static const time_type base_time(base_date, time_duration_type(0,0,0));

    int dayOffset, hourOffset, minuteOffset, secondOffset;
    double fraction = fabs(modf(oa_date, &dayOffset)) * 24; // fraction = hours
    fraction = modf(fraction, &hourOffset) * 60; // fraction = minutes
    fraction = modf(fraction, &minuteOffset) * 60; // fraction = seconds
    modf(fraction, &secondOffset);
    time_type t(base_time);
    t += time_duration_type(hourOffset, minuteOffset, secondOffset);
    t += date_duration_type(dayOffset);
    return t;
}

}
}


namespace pwiz {
namespace util {


/// returns a string representation suitable for an xsd:datetime attribute;
/// output string is UTC time (as denoted by the 'Z' suffix)
template<class time_type>
inline
std::string encode_xml_datetime(const time_type& t)
{
    blt::local_time_facet* output_facet = new blt::local_time_facet;
    output_facet->format("%Y-%m-%dT%H:%M:%SZ"); // 2007-06-27T15:23:45Z
    std::stringstream ss;
    ss.imbue(std::locale(std::locale::classic(), output_facet));
    ss << blt::local_date_time(t.utc_time(), blt::time_zone_ptr());
    return ss.str();
}


/// converts an xsd:datetime attribute to a local_date_time
inline blt::local_date_time decode_xml_datetime(const std::string& t)
{
    blt::local_time_input_facet* input_facet = new blt::local_time_input_facet;
    input_facet->format("%Y-%m-%dT%H:%M:%SZ");
    std::stringstream ss(t);
    ss.imbue(std::locale(std::locale::classic(), input_facet));
    blt::local_date_time result(bdt::not_a_date_time);
    ss >> result;
    return blt::local_date_time(result.utc_time(), blt::time_zone_ptr());
}


} // namespace util
} // namespace pwiz


#endif // _DATETIME_HPP_
