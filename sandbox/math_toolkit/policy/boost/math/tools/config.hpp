#ifndef BOOST_MATH_TOOLS_CONFIG_HPP
#define BOOST_MATH_TOOLS_CONFIG_HPP

#include <boost/cstdint.hpp> // for boost::uintmax_t
#include <boost/config.hpp>
#include <algorithm>  // for min and max

#define BOOST_MATH_MAX_ITER 1000000

#if defined(__CYGWIN__) || defined(__FreeBSD__)
#  define BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#endif
//
// Tune performance options for specific compilers:
//
#ifdef BOOST_MSVC
#  define BOOST_MATH_POLY_METHOD 3
#  define BOOST_MATH_RATIONAL_METHOD 3
#elif defined(BOOST_INTEL)
#  define BOOST_MATH_POLY_METHOD 2
#  define BOOST_MATH_RATIONAL_METHOD 0
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
#else
#define BOOST_MATH_INSTRUMENT_CODE(x)
#endif

#endif // BOOST_MATH_TOOLS_CONFIG_HPP


