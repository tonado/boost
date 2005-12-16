//  (C) Copyright John Maddock 2005.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_COMPLEX_DETAILS_INCLUDED
#define BOOST_MATH_COMPLEX_DETAILS_INCLUDED
//
// This header contains all the support code that is common to the
// inverse trig complex functions, it also contains all the includes
// that we need to implement all these functions.
//
#include <boost/config.hpp>
#include <boost/config/no_tr1/complex.hpp>
#include <boost/limits.hpp>
#include <math.h> // isnan where available
#include <cmath>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std{ using ::sqrt; }
#endif

namespace boost{ namespace math{ namespace detail{

template <class T>
inline bool test_is_nan(T t)
{
   // Comparisons with Nan's always fail:
   return std::numeric_limits<T>::has_infinity && (!(t <= std::numeric_limits<T>::infinity()) || !(t >= -std::numeric_limits<T>::infinity()));
}
#ifdef isnan
template<> inline bool test_is_nan<float>(float t) { return isnan(t); }
template<> inline bool test_is_nan<double>(double t) { return isnan(t); }
template<> inline bool test_is_nan<long double>(long double t) { return isnan(t); }
#endif

template <class T>
inline T mult_minus_one(const T& t)
{
   return test_is_nan(t) ? t : -t;
}

template <class T>
inline std::complex<T> mult_i(const std::complex<T>& t)
{
   return std::complex<T>(mult_minus_one(t.imag()), t.real());
}

template <class T>
inline std::complex<T> mult_minus_i(const std::complex<T>& t)
{
   return std::complex<T>(t.imag(), mult_minus_one(t.real()));
}

template <class T>
inline T safe_max(T t)
{
   return std::sqrt((std::numeric_limits<T>::max)()) / t;
}
inline long double safe_max(long double t)
{
   // long double sqrt often returns infinity due to
   // insufficient internal precision:
   return std::sqrt((std::numeric_limits<double>::max)()) / t;
}
template <class T>
inline T safe_min(T t)
{
   return std::sqrt((std::numeric_limits<T>::min)()) * t;
}
inline long double safe_min(long double t)
{
   // long double sqrt often returns zero due to
   // insufficient internal precision:
   return std::sqrt((std::numeric_limits<double>::min)()) * t;
}

} } } // namespaces

#endif // BOOST_MATH_COMPLEX_DETAILS_INCLUDED
