#ifndef GREG_DATE_HPP___
#define GREG_DATE_HPP___
/* Copyright (c) 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */

#include "boost/gdtl/date.hpp"
#include "boost/gdtl/special_defs.hpp"
#include "boost/gdtl/gregorian/greg_calendar.hpp"
#include "boost/gdtl/gregorian/greg_duration.hpp"


namespace boost {
namespace gregorian {

  //bring special enum values into the namespace
  using gdtl::special_values;
  using gdtl::not_special;
  using gdtl::neg_infin;
  using gdtl::pos_infin;
  using gdtl::not_a_date_time;
  using gdtl::max_date_time;
  using gdtl::min_date_time;

  //! A date type based on gregorian_calendar
  /*! This class is the primary interface for programming with 
      greogorian dates.  The is a lightweight type that can be
      freely passed by value.  All comparison operators are 
      supported.  
      \ingroup date_basics
  */
  class date : public gdtl::date<date, gregorian_calendar, date_duration>
  {
   public:
    typedef gregorian_calendar::year_type year_type;
    typedef gregorian_calendar::month_type month_type;
    typedef gregorian_calendar::day_type day_type;
    typedef gregorian_calendar::ymd_type ymd_type;
    typedef gregorian_calendar::date_rep_type date_rep_type;
    typedef gregorian_calendar::date_int_type date_int_type;
    typedef date_duration  duration_type;
    //! Main constructor with year, month, day
    date(year_type year, month_type month, day_type day) 
      : gdtl::date<date, gregorian_calendar, date_duration>(year, month, day)
    {}
    //! Constructor from a ymd_type structure
    explicit date(const ymd_type& ymd) 
      : gdtl::date<date, gregorian_calendar, date_duration>(ymd)
    {}
    //! Needed copy constructor
    date(const date_int_type& rhs):
      gdtl::date<date,gregorian_calendar, date_duration>(rhs)
    {}
    //! Needed copy constructor
    date(date_rep_type rhs):
      gdtl::date<date,gregorian_calendar, date_duration>(rhs)
    {}
    //! Constructor for infinities, not a date, max and min date
    explicit date(special_values sv):
      gdtl::date<date, gregorian_calendar, date_duration>(date_rep_type::from_special(sv))
    {}
    //! Return the day number from the calendar
    date_int_type day_number() const
    {
      return days_;
    }

   private:

  };
  


} } //namespace gregorian

/* Copyright (c) 2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */


#endif
