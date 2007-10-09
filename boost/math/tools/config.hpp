//  Copyright (c) 2006-7 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_CONFIG_HPP
#define BOOST_MATH_TOOLS_CONFIG_HPP

#include <boost/cstdint.hpp> // for boost::uintmax_t
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <algorithm>  // for min and max
#include <cmath>
#include <climits>
#if (defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__))
#  include <math.h>
#endif

#include <boost/math/tools/user.hpp>

#if defined(__CYGWIN__) || defined(__FreeBSD__)
#  define BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#endif
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
#  define BOOST_MATH_NO_REAL_CONCEPT_TESTS
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, < 1400)
//
// Define if constants too large for a float cause "bad"
// values to be stored in the data, rather than infinity
// or a suitably large value.
//
#  define BOOST_MATH_BUGGY_LARGE_FLOAT_CONSTANTS
#endif
//
// Tune performance options for specific compilers:
//
#ifdef BOOST_MSVC
#  define BOOST_MATH_POLY_METHOD 3
#elif defined(BOOST_INTEL)
#  define BOOST_MATH_POLY_METHOD 2
#  define BOOST_MATH_RATIONAL_METHOD 2
#elif defined(__GNUC__)
#  define BOOST_MATH_POLY_METHOD 3
#  define BOOST_MATH_RATIONAL_METHOD 3
#  define BOOST_MATH_INT_TABLE_TYPE(RT, IT) RT
#endif

//
// The maximum order of polynomial that will be evaluated 
// via an unrolled specialisation:
//
#ifndef BOOST_MATH_MAX_POLY_ORDER
#  define BOOST_MATH_MAX_POLY_ORDER 17
#endif 
//
// Set the method used to evaluate polynomials and rationals:
//
#ifndef BOOST_MATH_POLY_METHOD
#  define BOOST_MATH_POLY_METHOD 1
#endif 
#ifndef BOOST_MATH_RATIONAL_METHOD
#  define BOOST_MATH_RATIONAL_METHOD 0
#endif 
//
// decide whether to store constants as integers or reals:
//
#ifndef BOOST_MATH_INT_TABLE_TYPE
#  define BOOST_MATH_INT_TABLE_TYPE(RT, IT) IT
#endif
//
// Helper macro for using statements:
//
#define BOOST_MATH_STD_USING \
   using std::abs;\
   using std::acos;\
   using std::cos;\
   using std::fmod;\
   using std::modf;\
   using std::tan;\
   using std::asin;\
   using std::cosh;\
   using std::frexp;\
   using std::pow;\
   using std::tanh;\
   using std::atan;\
   using std::exp;\
   using std::ldexp;\
   using std::sin;\
   using std::atan2;\
   using std::fabs;\
   using std::log;\
   using std::sinh;\
   using std::ceil;\
   using std::floor;\
   using std::log10;\
   using std::sqrt;


namespace boost{ namespace math{
namespace tools
{

template <class T>
inline T max BOOST_PREVENT_MACRO_SUBSTITUTION(T a, T b, T c)
{
   return (std::max)((std::max)(a, b), c);
}

template <class T>
inline T max BOOST_PREVENT_MACRO_SUBSTITUTION(T a, T b, T c, T d)
{
   return (std::max)((std::max)(a, b), (std::max)(c, d));
}
} // namespace tools
}} // namespace boost namespace math

#ifdef __linux__

	#include <fenv.h>

	namespace boost{ namespace math{
	namespace detail
	{
	struct fpu_guard
	{
		fpu_guard()
		{
			fegetexceptflag(&m_flags, FE_ALL_EXCEPT);
			feclearexcept(FE_ALL_EXCEPT);
		}
		~fpu_guard()
		{
			fesetexceptflag(&m_flags, FE_ALL_EXCEPT);
		}
	private:
		fexcept_t m_flags;
	};

	} // namespace detail
	}} // namespaces

	#define BOOST_FPU_EXCEPTION_GUARD boost::math::detail::fpu_guard local_guard_object;
#else // All other platforms.
  #define BOOST_FPU_EXCEPTION_GUARD
#endif

#ifdef BOOST_MATH_INSTRUMENT
#define BOOST_MATH_INSTRUMENT_CODE(x) \
   std::cout << std::setprecision(35) << __FILE__ << ":" << __LINE__ << " " << x << std::endl;
#define BOOST_MATH_INSTRUMENT_VARIABLE(name) BOOST_MATH_INSTRUMENT_CODE(BOOST_STRINGIZE(name) << " = " << name)
#else
#define BOOST_MATH_INSTRUMENT_CODE(x)
#define BOOST_MATH_INSTRUMENT_VARIABLE(name)
#endif

#endif // BOOST_MATH_TOOLS_CONFIG_HPP



