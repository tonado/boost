//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_EXPM1_INCLUDED
#define BOOST_MATH_EXPM1_INCLUDED

#include <cmath>
#include <math.h> // platform's ::expm1
#include <boost/limits.hpp>
#include <boost/math/tools/config.hpp>
#include <boost/math/tools/series.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/math/policy/error_handling.hpp>
#include <boost/math/tools/rational.hpp>
#include <boost/math/special_functions/math_fwd.hpp>
#include <boost/mpl/less_equal.hpp>

#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#  include <boost/static_assert.hpp>
#else
#  include <boost/assert.hpp>
#endif

namespace boost{ namespace math{

namespace detail
{
  // Functor expm1_series returns the next term in the Taylor series
  // x^k / k!
  // each time that operator() is invoked.
  //
  template <class T>
  struct expm1_series
  {
     typedef T result_type;

     expm1_series(T x)
        : k(0), m_x(x), m_term(1) {}

     T operator()()
     {
        ++k;
        m_term *= m_x;
        m_term /= k;
        return m_term;
     }

     int count()const
     {
        return k;
     }

  private:
     int k;
     const T m_x;
     T m_term;
     expm1_series(const expm1_series&);
     expm1_series& operator=(const expm1_series&);
  };

//
// Algorithm expm1 is part of C99, but is not yet provided by many compilers.
//
// This version uses a Taylor series expansion for 0.5 > |x| > epsilon.
//
template <class T, class Policy>
T expm1_imp(T x, const mpl::int_<0>&, const Policy& pol)
{
   using namespace std;

   T a = fabs(x);
   if(a > T(0.5L))
      return exp(x) - T(1);
   if(a < tools::epsilon<T>())
      return x;
   detail::expm1_series<T> s(x);
   boost::uintmax_t max_iter = BOOST_MATH_MAX_ITER;
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
   T result = tools::sum_series(s, policy::digits<T, Policy>(), max_iter);
#else
   T zero = 0;
   T result = tools::sum_series(s, policy::digits<T, Policy>(), max_iter, zero);
#endif
   policy::check_series_iterations("boost::math::expm1<%1%>(%1%)", max_iter, pol);
   return result;
}

template <class T, class P>
T expm1_imp(T x, const mpl::int_<53>&, const P&)
{
   using namespace std;

   T a = fabs(x);
   if(a > T(0.5L))
      return exp(x) - T(1);
   if(a < tools::epsilon<T>())
      return x;

   static const float Y = 0.10281276702880859e1f;
   static const T n[] = { -0.28127670288085937e-1, 0.51278186299064534e0, -0.6310029069350198e-1, 0.11638457975729296e-1, -0.52143390687521003e-3, 0.21491399776965688e-4 };
   static const T d[] = { 1, -0.45442309511354755e0, 0.90850389570911714e-1, -0.10088963629815502e-1, 0.63003407478692265e-3, -0.17976570003654402e-4 };

   T result = x * Y + x * tools::evaluate_polynomial(n, x) / tools::evaluate_polynomial(d, x);
   return result;
}

template <class T, class P>
T expm1_imp(T x, const mpl::int_<64>&, const P&)
{
   using namespace std;

   T a = fabs(x);
   if(a > T(0.5L))
      return exp(x) - T(1);
   if(a < tools::epsilon<T>())
      return x;

   static const float Y = 0.10281276702880859375e1f;
   static const T n[] = { 
      -0.281276702880859375e-1L, 
       0.512980290285154286358e0L, 
       -0.667758794592881019644e-1L,
       0.131432469658444745835e-1L,
       -0.72303795326880286965e-3L,
       0.447441185192951335042e-4L,
       -0.714539134024984593011e-6L
   };
   static const T d[] = { 
      1, 
      -0.461477618025562520389e0L,
      0.961237488025708540713e-1L,
      -0.116483957658204450739e-1L,
      0.873308008461557544458e-3L,
      -0.387922804997682392562e-4L,
      0.807473180049193557294e-6L
   };

   T result = x * Y + x * tools::evaluate_polynomial(n, x) / tools::evaluate_polynomial(d, x);
   return result;
}

template <class T, class P>
T expm1_imp(T x, const mpl::int_<113>&, const P&)
{
   using namespace std;

   T a = fabs(x);
   if(a > T(0.5L))
      return exp(x) - T(1);
   if(a < tools::epsilon<T>())
      return x;

   static const float Y = 0.10281276702880859375e1f;
   static const T n[] = { 
      -0.28127670288085937499999999999999999854e-1L,
      0.51278156911210477556524452177540792214e0L,
      -0.63263178520747096729500254678819588223e-1L,
      0.14703285606874250425508446801230572252e-1L,
      -0.8675686051689527802425310407898459386e-3L,
      0.88126359618291165384647080266133492399e-4L,
      -0.25963087867706310844432390015463138953e-5L,
      0.14226691087800461778631773363204081194e-6L,
      -0.15995603306536496772374181066765665596e-8L,
      0.45261820069007790520447958280473183582e-10L
   };
   static const T d[] = { 
      1,
      -0.45441264709074310514348137469214538853e0L,
      0.96827131936192217313133611655555298106e-1L,
      -0.12745248725908178612540554584374876219e-1L,
      0.11473613871583259821612766907781095472e-2L,
      -0.73704168477258911962046591907690764416e-4L,
      0.34087499397791555759285503797256103259e-5L,
      -0.11114024704296196166272091230695179724e-6L,
      0.23987051614110848595909588343223896577e-8L,
      -0.29477341859111589208776402638429026517e-10L,
      0.13222065991022301420255904060628100924e-12L
   };

   T result = x * Y + x * tools::evaluate_polynomial(n, x) / tools::evaluate_polynomial(d, x);
   return result;
}

} // namespace detail

template <class T, class Policy>
inline typename tools::promote_args<T>::type expm1(T x, const Policy& /* pol */)
{
   typedef typename tools::promote_args<T>::type result_type;
   typedef typename policy::evaluation<result_type, Policy>::type value_type;
   typedef typename policy::precision<result_type, Policy>::type precision_type;
   typedef typename policy::normalise<
      Policy, 
      policy::promote_float<false>, 
      policy::promote_double<false>, 
      policy::discrete_quantile<>,
      policy::assert_undefined<> >::type forwarding_policy;

   typedef typename mpl::if_c<
      ::std::numeric_limits<result_type>::is_specialized == 0,
      mpl::int_<0>,  // no numeric_limits, use generic solution
      typename mpl::if_<
         typename mpl::less_equal<precision_type, mpl::int_<53> >::type,
         mpl::int_<53>,  // double
         typename mpl::if_<
            typename mpl::less_equal<precision_type, mpl::int_<64> >::type,
            mpl::int_<64>, // 80-bit long double
            typename mpl::if_<
               typename mpl::less_equal<precision_type, mpl::int_<113> >::type,
               mpl::int_<113>, // 128-bit long double
               mpl::int_<0> // too many bits, use generic version.
            >::type
         >::type
      >::type
   >::type tag_type;

   return policy::checked_narrowing_cast<result_type, forwarding_policy>(detail::expm1_imp(
      static_cast<value_type>(x),
      tag_type(), forwarding_policy()), "boost::math::expm1<%1%>(%1%)");
}

#ifdef expm1
#  ifndef BOOST_HAS_expm1
#     define BOOST_HAS_expm1
#  endif
#  undef expm1
#endif

#ifdef BOOST_HAS_EXPM1
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
inline float expm1(float x, const policy::policy<>&){ return ::expm1f(x); }
inline long double expm1(long double x, const policy::policy<>&){ return ::expm1l(x); }
#else
inline float expm1(float x, const policy::policy<>&){ return ::expm1(x); }
#endif
inline double expm1(double x, const policy::policy<>&){ return ::expm1(x); }
#endif

template <class T>
inline typename tools::promote_args<T>::type expm1(T x)
{
   return expm1(x, policy::policy<>());
}

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
inline float expm1(float z)
{
   return expm1<float>(z);
}
inline double expm1(double z)
{
   return expm1<double>(z);
}
inline long double expm1(long double z)
{
   return expm1<long double>(z);
}
#endif

} // namespace math
} // namespace boost

#endif // BOOST_MATH_HYPOT_INCLUDED

