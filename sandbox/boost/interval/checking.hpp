/* Boost interval/checking.hpp template implementation file
 *
 * Copyright Jens Maurer 2000
 * Copyright Herv� Br�nnimann, Guillaume Melquiond, Sylvain Pion 2002
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * None of the above authors nor Polytechnic University make any
 * representation about the suitability of this software for any
 * purpose. It is provided "as is" without express or implied warranty.
 *
 * $Id$
 *
 * Revision history:
 *   2002-08-31	 Prepared for boost formal review
 *   2000-09-24	 Separated from interval.hpp
 */

#ifndef BOOST_INTERVAL_CHECKING_HPP
#define BOOST_INTERVAL_CHECKING_HPP

#include <stdexcept>
#include <cassert>
#include <boost/limits.hpp>

namespace boost {
  namespace interval_lib {

template<class T>
struct exception_create_empty
{
  T operator()()
  {
    throw std::runtime_error("boost::interval: empty interval created");
  }
};

template<class T>
struct exception_invalid_number
{
  void operator()()
  {
    throw std::invalid_argument("boost::interval: invalid number");
  }
};

template<class T>
struct checking_base
{
  static T inf()
  {
    assert(std::numeric_limits<T>::has_infinity);
    return std::numeric_limits<T>::infinity();
  }
  static T nan()
  {
    assert(std::numeric_limits<T>::has_quiet_NaN);
    return std::numeric_limits<T>::quiet_NaN();
  }
  static bool is_nan(const T& x)
  {
    return std::numeric_limits<T>::has_quiet_NaN && (x != x);
  }
  static T empty_lower()
  {
    return (std::numeric_limits<T>::has_quiet_NaN ?
	    std::numeric_limits<T>::quiet_NaN() : T(1));
  }
  static T empty_upper()
  {
    return (std::numeric_limits<T>::has_quiet_NaN ?
	    std::numeric_limits<T>::quiet_NaN() : T(0));
  }
  static bool is_empty(const T& l, const T& u)
  {
    return !(l <= u); // safety for partial orders
  }
};

template<class T, class Checking = checking_base<T>,
	 class Exception = exception_create_empty<T> >
struct checking_no_empty: Checking
{
  static T nan()
  {
    assert(false);
    return Checking::nan();
  }
  static T empty_lower()
  {
    Exception()();
    return Checking::empty_lower();
  }
  static T empty_upper()
  {
    Exception()();
    return Checking::empty_upper();
  }
  static bool is_empty(const T&, const T&)
  {
    return false;
  }
};

template<class T, class Checking>
struct checking_no_nan: Checking
{
  static bool is_nan(const T&)
  {
    return false;
  }
};

template<class T, class Checking,
	 class Exception = exception_invalid_number<T> >
struct checking_catch_nan: Checking
{
  static bool is_nan(const T& x)
  {
    if (Checking::is_nan(x)) Exception()();
    return false;
  }
};

template<class T>
struct checking_strict:
  checking_catch_nan<T, checking_no_empty<T> >
{};

    namespace detail {

template <class T> inline bool is_nan(const T& x) { return x != x; }

    } // namespace detail

  } // namespace interval
} // namespace boost

#endif // BOOST_INTERVAL_CHECKING_HPP
