
#ifndef GDTL_TIME_HPP___
#define GDTL_TIME_HPP___
/* Copyright (c) 2000, 2002 CrystalClear Software, Inc.
 * Disclaimer & Full Copyright at end of file
 * Author: Jeff Garland 
 */

/*! @file time.hpp
  This file contains the interface for the time associated classes.
*/
#include "boost/gdtl/time_defs.hpp"
#include "boost/operators.hpp"
#include <string>

namespace boost {
namespace gdtl {

  //! Representation of a precise moment in time, including the date.
  /*! 
    This class is a skeleton for the interface of a temporal type
    with a resolution is higher than a day.  It is intended that 
    this class be the base class and that the actual time 
    class be derived using the BN pattern.  In this way, the derived 
    class can make decisions such as 'should there be a default constructor' 
    and what should it set its value to, should there be optional constructors
    say allowing only an time_durations that generate a time from a clock,etc.
    So, in fact multiple time types can be created for a time_system with
    different construction policies, and all of them can perform basic
    operations by only writing a copy constructor.  Finally, compiler 
    errors are also shorter. 
    
    The real behavior of the time class is provided by the time_system
    template parameter.  This class must provide all the logic
    for addition, subtraction, as well as define all the interface
    types.
    
  */

  template <class T, class time_system>
  class gdtl_time 
    : boost::less_than_comparable<T
    , boost::equality_comparable<T
      > >
  {
  public:
    typedef T time_type;
    typedef typename time_system::time_rep_type time_rep_type;
    typedef typename time_system::date_type date_type;
    typedef typename time_system::date_duration_type date_duration_type;
    typedef typename time_system::time_duration_type time_duration_type;
    //typedef typename time_system::hms_type hms_type;
    
    gdtl_time(const date_type& day, 
	      const time_duration_type& td, 
	      dst_flags dst=not_dst) :
      time_(time_system::get_time_rep(day, td, dst))
    {}
    gdtl_time(const time_rep_type& rhs) :
      time_(rhs)
    {}
    date_type date() const
    {
      return time_system::get_date(time_);
    }
    time_duration_type time_of_day() const
    {
      return time_system::get_time_of_day(time_);
    }
    std::string zone_name() const
    {
      return time_system::zone_name(time_);
    }
    //TODO add these special functions
//     //! check to see if date is not a value
//     bool is_not_a_date()  const
//     {
//       return traits_type::is_not_a_number(days_);
//     }
//     //! check to see if date is one of the infinity values
//     bool is_infinity()  const
//     {
//       return traits_type::is_infinity(days_);
//     }

//     //! check to see if date is greater than all possible dates
//     bool is_pos_infinity()  const
//     {
//       return traits_type::is_pos_infinity(days_);
//     }
//     //! check to see if date is greater than all possible dates
//     bool is_neg_infinity()  const
//     {
//       return traits_type::is_neg_infinity(days_);
//     }

    //!Equality operator -- others generated by boost::equality_comparable
    bool operator==(const time_type& rhs) const
    {
      return time_system::is_equal(time_,rhs.time_);
    }
    //!Equality operator -- others generated by boost::less_than_comparable
    bool operator<(const time_type& rhs) const
    {
      return time_system::is_less(time_,rhs.time_);
    }
    //! add date durations
    time_type operator+(const date_duration_type& dd) const
    {
      return time_system::add_days(time_, dd);
    }
    //! subtract date durations
    time_type operator-(const date_duration_type& dd) const
    {
      return time_system::subtract_days(time_, dd);
    }
    //! subtract time durations
    time_type operator-(const time_duration_type& rhs) const
    {
      return time_system::subtract_time_duration(time_, rhs);
    }
    //! difference between to times
    time_duration_type operator-(const time_type& rhs) const
    {
      return time_system::subtract_times(time_, rhs.time_);
    }
    //! add time durations
    time_type operator+(const time_duration_type& td) const
    {
      return time_type(time_system::add_time_duration(time_, td));
    }
    
  protected:
    time_rep_type time_;
  };





} } //namespace gdtl::boost

/* Copyright (c) 2000,2002
 * CrystalClear Software, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  CrystalClear Software makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#endif
