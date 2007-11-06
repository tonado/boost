//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/special_functions/factorials.hpp>
// #includes all the files that it needs to.
//
#include <boost/math/special_functions/factorials.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void check()
{
   check_result<float>(boost::math::factorial<float>(u));
   check_result<double>(boost::math::factorial<double>(u));
   check_result<long double>(boost::math::factorial<long double>(u));

   check_result<float>(boost::math::double_factorial<float>(u));
   check_result<double>(boost::math::double_factorial<double>(u));
   check_result<long double>(boost::math::double_factorial<long double>(u));

   check_result<float>(boost::math::rising_factorial<float>(f, i));
   check_result<double>(boost::math::rising_factorial<double>(d, i));
   check_result<long double>(boost::math::rising_factorial<long double>(l, i));

   check_result<float>(boost::math::falling_factorial<float>(f, u));
   check_result<double>(boost::math::falling_factorial<double>(d, u));
   check_result<long double>(boost::math::falling_factorial<long double>(l, u));
}

