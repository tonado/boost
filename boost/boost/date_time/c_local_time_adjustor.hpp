#ifndef DATE_TIME_C_LOCAL_TIME_ADJUSTOR_HPP__
#define DATE_TIME_C_LOCAL_TIME_ADJUSTOR_HPP__

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */
 
/*! @file c_local_time_adjustor.hpp
  Time adjustment calculations based on machine
*/

#include <stdexcept>
#include "boost/date_time/c_time.hpp"

namespace boost {
namespace date_time {

  //! Adjust to / from utc using the C API
  /*! Warning!!! This class assumes that timezone settings of the
   *  machine are correct.  This can be a very dangerous assumption.
   */
  template<class time_type>
  class c_local_adjustor
  {
  public:
    typedef typename time_type::time_duration_type time_duration_type;
    typedef typename time_type::date_type date_type;
    typedef typename date_type::duration_type date_duration_type;
    //! Convert a utc time to local time
    static time_type utc_to_local(const time_type& t)
    {
      date_type time_t_start_day(1970,1,1);
      time_type time_t_start_time(time_t_start_day,time_duration_type(0,0,0));
      if (t < time_t_start_time) {
        throw std::out_of_range("Cannot convert dates prior to Jan 1, 1970");
      }
      date_duration_type dd = t.date() - time_t_start_day;
      time_duration_type td = t.time_of_day();
      std::time_t t2 = dd.days()*86400 + td.hours()*3600 + td.minutes()*60 + td.seconds();
      std::tm* tms = std::localtime(&t2);
      date_type d(static_cast<unsigned short>(tms->tm_year + 1900),
                  static_cast<unsigned short>(tms->tm_mon + 1), 
                  static_cast<unsigned short>(tms->tm_mday));
      time_duration_type td2(tms->tm_hour,
                             tms->tm_min,
			     tms->tm_sec,
                             t.time_of_day().fractional_seconds());

      return time_type(d,td2);
    }
  };



} } //namespace date_time



#endif
