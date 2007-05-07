//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/concepts/real_concept.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/beta.hpp>
#include <boost/math/tools/stats.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "test_beta_hooks.hpp"
#include "handle_test_result.hpp"

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the function beta.  There are two sets of tests, spot
// tests which compare our results with selected values computed
// using the online special function calculator at 
// functions.wolfram.com, while the bulk of the accuracy tests
// use values generated with NTL::RR at 1000-bit precision
// and our generic versions of these functions.
//
// Note that when this file is first run on a new platform many of
// these tests will fail: the default accuracy is 1 epsilon which
// is too tight for most platforms.  In this situation you will 
// need to cast a human eye over the error rates reported and make
// a judgement as to whether they are acceptable.  Either way please
// report the results to the Boost mailing list.  Acceptable rates of
// error are marked up below as a series of regular expressions that
// identify the compiler/stdlib/platform/data-type/test-data/test-function
// along with the maximum expected peek and RMS mean errors for that
// test.
//

void expected_results()
{
   //
   // Define the max and mean errors expected for
   // various compilers and platforms.
   //

   // Darwin:
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      "Mac OS.*",                    // platform
      "(long\\s+)?double",           // test type(s)
      "Beta Function: Medium.*",     // test data group
      "boost::math::beta", 200, 35); // test function


   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "(long\\s+)?double",           // test type(s)
      "Beta Function: Small.*",      // test data group
      "boost::math::beta", 8, 5);    // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "(long\\s+)?double",           // test type(s)
      "Beta Function: Medium.*",     // test data group
      "boost::math::beta", 130, 35); // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "(long\\s+)?double",           // test type(s)
      "Beta Function: Divergent.*",  // test data group
      "boost::math::beta", 20, 6);   // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "Beta Function: Small.*",      // test data group
      "boost::math::beta", 15, 6);   // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "Beta Function: Medium.*",     // test data group
      "boost::math::beta", 130, 35);  // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      "Beta Function: Divergent.*",  // test data group
      "boost::math::beta", 15, 8);   // test function

   //
   // Finish off by printing out the compiler/stdlib/platform names,
   // we do this to make it easier to mark up expected error rates.
   //
   std::cout << "Tests run with " << BOOST_COMPILER << ", " 
      << BOOST_STDLIB << ", " << BOOST_PLATFORM << std::endl;
}

template <class T>
void do_test_beta(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type, value_type);
   pg funcp = boost::math::beta;

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test beta against data:
   //
   result = boost::math::tools::test(
      data, 
      boost::lambda::bind(funcp, 
         boost::lambda::ret<value_type>(boost::lambda::_1[0]),
         boost::lambda::ret<value_type>(boost::lambda::_1[1])), 
      boost::lambda::ret<value_type>(boost::lambda::_1[2]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::beta", test_name);
#ifdef TEST_OTHER
   if(::boost::is_floating_point<value_type>::value){
      funcp = other::beta;
      result = boost::math::tools::test(
         data, 
         boost::lambda::bind(funcp, 
            boost::lambda::ret<value_type>(boost::lambda::_1[0]),
            boost::lambda::ret<value_type>(boost::lambda::_1[1])), 
         boost::lambda::ret<value_type>(boost::lambda::_1[2]));
      print_test_result(result, data[result.worst()], result.worst(), type_name, "other::beta");
   }
#endif
   std::cout << std::endl;
}
template <class T>
void test_beta(T, const char* name)
{
   //
   // The actual test data is rather verbose, so it's in a separate file
   //
   // The contents are as follows, each row of data contains
   // three items, input value a, input value b and beta(a, b):
   // 
#  include "beta_small_data.ipp"

   do_test_beta(beta_small_data, name, "Beta Function: Small Values");

#  include "beta_med_data.ipp"

   do_test_beta(beta_med_data, name, "Beta Function: Medium Values");

#  include "beta_exp_data.ipp"

   do_test_beta(beta_exp_data, name, "Beta Function: Divergent Values");
}

#undef small // VC++ #defines small char !!!!!!
template <class T>
void test_spots(T)
{
   //
   // Basic sanity checks, tolerance is 20 epsilon expressed as a percentage:
   //
   T tolerance = boost::math::tools::epsilon<T>() * 20 * 100;
   T small = boost::math::tools::epsilon<T>() / 1024;
   BOOST_CHECK_CLOSE(::boost::math::beta(static_cast<T>(1), static_cast<T>(1)), static_cast<T>(1), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::beta(static_cast<T>(1), static_cast<T>(4)), static_cast<T>(0.25), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::beta(static_cast<T>(4), static_cast<T>(1)), static_cast<T>(0.25), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::beta(small, static_cast<T>(4)), 1/small, tolerance);
   BOOST_CHECK_CLOSE(::boost::math::beta(static_cast<T>(4), small), 1/small, tolerance);
   BOOST_CHECK_CLOSE(::boost::math::beta(static_cast<T>(4), static_cast<T>(20)), static_cast<T>(0.00002823263692828910220214568040654997176736L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::beta(static_cast<T>(0.0125L), static_cast<T>(0.000023L)), static_cast<T>(43558.24045647538375006349016083320744662L), tolerance);
}

int test_main(int, char* [])
{
   expected_results();
   test_spots(0.0F);
   test_spots(0.0);
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(0.0L);
   test_spots(boost::math::concepts::real_concept(0.1));
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif

   test_beta(0.1F, "float");
   test_beta(0.1, "double");
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_beta(0.1L, "long double");
   test_beta(boost::math::concepts::real_concept(0.1), "real_concept");
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif
   return 0;
}


