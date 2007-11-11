//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/special_functions/ellint_2.hpp>
// #includes all the files that it needs to.
//
#include <boost/math/special_functions/ellint_2.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void check()
{
   check_result<float>(boost::math::ellint_2<float>(f, f));
   check_result<double>(boost::math::ellint_2<double>(d, d));
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   check_result<long double>(boost::math::ellint_2<long double>(l, l));
#endif

   check_result<float>(boost::math::ellint_2<float>(f));
   check_result<double>(boost::math::ellint_2<double>(d));
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   check_result<long double>(boost::math::ellint_2<long double>(l));
#endif
}
