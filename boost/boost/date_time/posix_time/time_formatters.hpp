#ifndef POSIXTIME_FORMATTERS_HPP___
#define POSIXTIME_FORMATTERS_HPP___

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/iso_format.hpp"
#include "boost/date_time/date_format_simple.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include "boost/date_time/time_formatting_streams.hpp"
 
namespace boost {

namespace posix_time {

  //! Time duration to string -hh::mm::ss.fffffff. Example: 10:09:03.0123456
  /*!\ingroup time_format
   */
  inline std::string to_simple_string(time_duration td) {
    std::ostringstream ss;
    if(td.is_special()) {
      /* simply using 'ss << td.get_rep()' won't work on compilers
       * that don't support locales. This way does. */
      // switch copied from date_names_put.hpp
      switch(td.get_rep().as_special())
      {
      case not_a_date_time:
        //ss << "not-a-number";
        ss << "not-a-date-time";
        break;
      case pos_infin:
        ss << "+infinity";
        break;
      case neg_infin:
        ss << "-infinity";
        break;
      default:
        ss << "";
      }
    }
    else {
      if(td.is_negative()) {
        ss << '-';
      }
      ss  << std::setw(2) << std::setfill('0') 
          << date_time::absolute_value(td.hours()) << ":";
      ss  << std::setw(2) << std::setfill('0') 
          << date_time::absolute_value(td.minutes()) << ":";
      ss  << std::setw(2) << std::setfill('0') 
          << date_time::absolute_value(td.seconds());
      //TODO the following is totally non-generic, yelling FIXME
#if (defined(BOOST_MSVC) && (_MSC_VER <= 1200))  // 1200 == VC++ 6.0
      boost::int64_t frac_sec = 
        date_time::absolute_value(td.fractional_seconds());
      // JDG [7/6/02 VC++ compatibility]
      char buff[32];
      _i64toa(frac_sec, buff, 10);
#else
      time_duration::fractional_seconds_type frac_sec = 
        date_time::absolute_value(td.fractional_seconds());
#endif
      if (frac_sec != 0) {
        ss  << "." << std::setw(time_duration::num_fractional_digits())
            << std::setfill('0')
          
          // JDG [7/6/02 VC++ compatibility]
#if (defined(BOOST_MSVC) && (_MSC_VER <= 1200))  // 1200 == VC++ 6.0
            << buff;
#else
        << frac_sec;
#endif
      }
    }// else
    return ss.str();
  }

  //! Time duration in iso format -hhmmss,fffffff Example: 10:09:03,0123456
  /*!\ingroup time_format
   */
  inline 
  std::string 
  to_iso_string(time_duration td) 
  {
    std::ostringstream ss;
    if(td.is_special()) {
      /* simply using 'ss << td.get_rep()' won't work on compilers
       * that don't support locales. This way does. */
      // switch copied from date_names_put.hpp
      switch(td.get_rep().as_special()) {
      case not_a_date_time:
        //ss << "not-a-number";
        ss << "not-a-date-time";
        break;
      case pos_infin:
        ss << "+infinity";
        break;
      case neg_infin:
        ss << "-infinity";
        break;
      default:
        ss << "";
      }
    }
    else {
      if(td.is_negative()) {
        ss << '-';
      }
      ss  << std::setw(2) << std::setfill('0') 
          << date_time::absolute_value(td.hours());
      ss  << std::setw(2) << std::setfill('0') 
          << date_time::absolute_value(td.minutes());
      ss  << std::setw(2) << std::setfill('0') 
          << date_time::absolute_value(td.seconds());
      //TODO the following is totally non-generic, yelling FIXME
#if (defined(BOOST_MSVC) && (_MSC_VER <= 1200))  // 1200 == VC++ 6.0
      boost::int64_t frac_sec = 
        date_time::absolute_value(td.fractional_seconds());
      // JDG [7/6/02 VC++ compatibility]
      char buff[32];
      _i64toa(frac_sec, buff, 10);
#else
      time_duration::fractional_seconds_type frac_sec = 
        date_time::absolute_value(td.fractional_seconds());
#endif
      if (frac_sec != 0) {
        ss  << "." << std::setw(time_duration::num_fractional_digits())
            << std::setfill('0')
          
          // JDG [7/6/02 VC++ compatibility]
#if (defined(BOOST_MSVC) && (_MSC_VER <= 1200))  // 1200 == VC++ 6.0
            << buff;
#else
        << frac_sec;
#endif
      }
    }// else
    return ss.str();
  }
  
  //! Time to simple format CCYY-mmm-dd hh:mm:ss.fffffff
  /*!\ingroup time_format
   */
  inline 
  std::string 
  to_simple_string(ptime t) 
  {
    std::string ts = gregorian::to_simple_string(t.date());// + " ";
    if(!t.time_of_day().is_special()) {
      return ts + " " + to_simple_string(t.time_of_day());
    }
    else {
      return ts;
    }
  }

  //! Convert to string of form [YYYY-mmm-DD HH:MM::SS.ffffff/YYYY-mmm-DD HH:MM::SS.fffffff]
  /*!\ingroup time_format
   */
  inline 
  std::string 
  to_simple_string(time_period tp) 
  {
    std::string d1(to_simple_string(tp.begin()));
    std::string d2(to_simple_string(tp.last()));
    return std::string("[" + d1 + "/" + d2 +"]");
  }

  //! Convert iso short form YYYYMMDDTHHMMSS where T is the date-time separator
  /*!\ingroup time_format
   */
  inline 
  std::string to_iso_string(ptime t) 
  {
    std::string ts = gregorian::to_iso_string(t.date());// + "T";
    if(!t.time_of_day().is_special()) {
      return ts + "T" + to_iso_string(t.time_of_day());
    }
    else {
      return ts;
    }
  }

  //! Convert to form YYYY-MM-DDTHH:MM:SS where T is the date-time separator
  /*!\ingroup time_format
   */
  inline 
  std::string 
  to_iso_extended_string(ptime t) 
  {
    std::string ts = gregorian::to_iso_extended_string(t.date());// + "T";
    if(!t.time_of_day().is_special()) {
      return ts + "T" + to_simple_string(t.time_of_day());
    }
    else {
      return ts;
    }
  }

//The following code is removed for configurations with good std::locale support (eg: MSVC6, gcc 2.9x)
#ifndef BOOST_DATE_TIME_NO_LOCALE

  //! ostream operator for posix_time::time_duration
  template <class charT, class traits>
  inline
  std::basic_ostream<charT, traits>&
  operator<<(std::basic_ostream<charT, traits>& os, const time_duration& td)
  {
    typedef boost::date_time::ostream_time_duration_formatter<time_duration, charT> duration_formatter;
    duration_formatter::duration_put(td, os);
    return os;
  }

  //! ostream operator for posix_time::ptime
  template <class charT, class traits>
  inline
  std::basic_ostream<charT, traits>&
  operator<<(std::basic_ostream<charT, traits>& os, const ptime& t)
  {
    typedef boost::date_time::ostream_time_formatter<ptime, charT> time_formatter;
    time_formatter::time_put(t, os);
    return os;
  }

  //! ostream operator for posix_time::time_period
  template <class charT, class traits>
  inline
  std::basic_ostream<charT, traits>&
  operator<<(std::basic_ostream<charT, traits>& os, const time_period& tp)
  {
    typedef boost::date_time::ostream_time_period_formatter<time_period, charT> period_formatter;
    period_formatter::period_put(tp, os);
    return os;
  }



#endif //BOOST_DATE_TIME_NO_LOCALE


} } //namespace posix_time


#endif

