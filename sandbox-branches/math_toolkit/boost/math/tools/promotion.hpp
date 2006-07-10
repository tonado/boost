// boost\math\tools\promotion.hpp

// Copyright John Maddock 2006.
// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Promote arguments functions to allow math functions to have arguments
// provided as integer OR real (floating-point, built-in or UDT)
// (called ArithmeticType in functions that use promotion)
// that help to reduce the risk of creating multiple instantiations.
// Allows creation of an inline wrapper that forwards to a foo(RT, RT) function,
// so you never get to instantiate any mixed foo(RT, IT) functions.

#ifndef BOOST_MATH_PROMOTION_HPP
#define BOOST_MATH_PROMOTION_HPP

// Boost type traits:
#include <boost/type_traits/is_floating_point.hpp> // for boost::is_floating_point;
#include <boost/type_traits/is_integral.hpp> // for boost::is_integral
#include <boost/type_traits/is_convertible.hpp> // for boost::is_convertible
#include <boost/type_traits/is_same.hpp>// for boost::is_same
// Boost Template meta programming:
#include <boost/mpl/if.hpp> // for boost::mpl::if_c.

namespace boost
{
  namespace math
  {
    namespace tools
    {
      // If either T1 or T2 is an integer type,
      // pretend it was a double (for the purposes of further analysis).
      // Then pick the wider of the two floating-point types
      // as the actual signature to forward to.
      // For example:
      // foo(int, short) -> double foo(double, double);
      // foo(int, float) -> double foo(double, double);
      // Note: NOT float foo(float, float)
      // foo(int, double) -> foo(double, double);
      // foo(double, float) -> double foo(double, double);
      // foo(double, float) -> double foo(double, double);
      // foo(any-int-or-float-type, long double) -> foo(long double, long double);
      // but ONLY float foo(float, float) is unchanged.
      // So the only way to get an entirely float version is to call foo(1.F, 2.F),
      // But since most (all?) the math functions convert to double internally,
      // probably there would not be the hoped-for gain by using float here.

      // This follows the C-compatible conversion rules of pow, etc
      // where pow(int, float) is converted to pow(double, double).

      template <class T>
      struct promote_arg
      { // If T is integral type, then promote to double.
        typedef typename mpl::if_<is_integral<T>, double, T>::type type;
      };

      template <class T1, class T2>
      struct promote_arg2
      { // Promote, if necessary, & pick the wider of the two floating-point types.
        // for both parameter types, if integral promote to double.
        typedef typename promote_arg<T1>::type T1P; // T1 perhaps promoted.
        typedef typename promote_arg<T2>::type T2P; // T2 perhaps promoted.

        typedef typename mpl::if_c<
          is_floating_point<T1P>::value && is_floating_point<T2P>::value, // both T1P and T2P are floating-point?
          typename mpl::if_c<is_same<long double, T1P>::value || is_same<long double, T2P>::value, // either long double?
            long double, // then result type is long double.
          typename mpl::if_c<is_same<double, T1P>::value || is_same<double, T2P>::value, // either double?
            double, // result type is double.
          float // else result type is float.
          >::type
          >::type,
          typename mpl::if_<is_convertible<T1P, T2P>, T2P, T1P>::type>::type type;
      }; // promote_arg2

      template <class T1, class T2, class T3>
      struct promote_arg3
      { // Apply the rules to the 1st two arguments,
        typedef typename promote_arg2<T1, T2>::type T12P; //
         // and then again to the result of that and the last (3rd) argument.
        typedef typename promote_arg2<T12P, T3>::type T123P;
        typedef T123P type;
      }; // promote_arg3
    } // namespace tools
  } // namespace math
} // namespace boost

#endif // BOOST_MATH_PROMOTION_HPP
