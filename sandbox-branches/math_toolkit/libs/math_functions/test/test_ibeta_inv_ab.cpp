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

#ifdef TEST_GSL
#include <gsl/gsl_errno.h>
#include <gsl/gsl_message.h>
#endif

#include "handle_test_result.hpp"

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the incomplete beta function inverses 
// ibeta_inva and ibetac_inva. There are three sets of tests:
// 1) TODO!!!! Accuracy tests use values generated with NTL::RR at 
// 1000-bit precision and our generic versions of these functions.
// 2) Round trip sanity checks, use the test data for the forward
// functions, and verify that we can get (approximately) back
// where we started.
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
   const char* largest_type;
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   if(boost::math::tools::digits<double>() == boost::math::tools::digits<long double>())
   {
      largest_type = "(long\\s+)?double";
   }
   else
   {
      largest_type = "long double";
   }
#else
   largest_type = "(long\\s+)?double";
#endif

   //
   // Catch all cases come last:
   //
   // TODO!!!!
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      ".*",                          // test data group
      ".*", 500, 500);               // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "float|double",                // test type(s)
      ".*",                          // test data group
      ".*", 5, 3);                   // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      ".*",                          // test data group
      ".*", 1000000, 500000);        // test function

   //
   // Finish off by printing out the compiler/stdlib/platform names,
   // we do this to make it easier to mark up expected error rates.
   //
   std::cout << "Tests run with " << BOOST_COMPILER << ", " 
      << BOOST_STDLIB << ", " << BOOST_PLATFORM << std::endl;
}

template <class T>
void test_inverses(const T& data)
{
   using namespace std;
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   value_type precision = static_cast<value_type>(ldexp(1.0, 1-boost::math::tools::digits<value_type>()/2)) * 100;
   if(boost::math::tools::digits<value_type>() < 50)
      precision = 1;   // 1% or two decimal digits, all we can hope for when the input is truncated

   for(unsigned i = 0; i < data.size(); ++i)
   {
      //
      // These inverse tests are thrown off if the output of the
      // incomplete beta is too close to 1: basically there is insuffient
      // information left in the value we're using as input to the inverse
      // to be able to get back to the original value.
      //
      if(data[i][5] == 0)
      {
         BOOST_CHECK_EQUAL(boost::math::ibeta_inva(data[i][1], data[i][2], data[i][5]), boost::math::tools::max_value<value_type>());
         BOOST_CHECK_EQUAL(boost::math::ibeta_invb(data[i][0], data[i][2], data[i][5]), boost::math::tools::min_value<value_type>());
      }
      else if((1 - data[i][5] > 0.001) && (fabs(data[i][5]) >= boost::math::tools::min_value<value_type>()))
      {
         value_type inv = boost::math::ibeta_inva(data[i][1], data[i][2], data[i][5]);
         BOOST_CHECK_CLOSE(data[i][0], inv, precision);
         inv = boost::math::ibeta_invb(data[i][0], data[i][2], data[i][5]);
         BOOST_CHECK_CLOSE(data[i][1], inv, precision);
      }
      else if(1 == data[i][5])
      {
         BOOST_CHECK_EQUAL(boost::math::ibeta_inva(data[i][1], data[i][2], data[i][5]), boost::math::tools::min_value<value_type>());
         BOOST_CHECK_EQUAL(boost::math::ibeta_invb(data[i][0], data[i][2], data[i][5]), boost::math::tools::max_value<value_type>());
      }

      if(data[i][6] == 0)
      {
         BOOST_CHECK_EQUAL(boost::math::ibetac_inva(data[i][1], data[i][2], data[i][6]), boost::math::tools::min_value<value_type>());
         BOOST_CHECK_EQUAL(boost::math::ibetac_invb(data[i][0], data[i][2], data[i][6]), boost::math::tools::max_value<value_type>());
      }
      else if((1 - data[i][6] > 0.001) && (fabs(data[i][6]) >= boost::math::tools::min_value<value_type>()))
      {
         value_type inv = boost::math::ibetac_inva(data[i][1], data[i][2], data[i][6]);
         BOOST_CHECK_CLOSE(data[i][0], inv, precision);
         inv = boost::math::ibetac_invb(data[i][0], data[i][2], data[i][6]);
         BOOST_CHECK_CLOSE(data[i][1], inv, precision);
      }
      else if(data[i][6] == 1)
      {
         BOOST_CHECK_EQUAL(boost::math::ibetac_inva(data[i][1], data[i][2], data[i][6]), boost::math::tools::max_value<value_type>());
         BOOST_CHECK_EQUAL(boost::math::ibetac_invb(data[i][0], data[i][2], data[i][6]), boost::math::tools::min_value<value_type>());
      }
   }
}

template <class T>
void test_inverses2(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type, value_type, value_type);
   pg funcp = boost::math::ibeta_inva;

   using namespace boost::lambda;

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test ibeta_inva(T, T, T) against data:
   //
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1]), ret<value_type>(_1[2])),
      ret<value_type>(_1[3]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::ibeta_inva", test_name);
   //
   // test ibetac_inva(T, T, T) against data:
   //
   funcp = boost::math::ibetac_inva;
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1]), ret<value_type>(_1[2])),
      ret<value_type>(_1[4]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::ibetac_inva", test_name);
   //
   // test ibeta_invb(T, T, T) against data:
   //
   funcp = boost::math::ibeta_invb;
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1]), ret<value_type>(_1[2])),
      ret<value_type>(_1[5]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::ibeta_invb", test_name);
   //
   // test ibetac_invb(T, T, T) against data:
   //
   funcp = boost::math::ibetac_invb;
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1]), ret<value_type>(_1[2])),
      ret<value_type>(_1[6]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::ibetac_invb", test_name);
}

template <class T>
void test_beta(T, const char* name)
{
   //
   // The actual test data is rather verbose, so it's in a separate file
   //
   // The contents are as follows, each row of data contains
   // five items, input value a, input value b, integration limits x, beta(a, b, x) and ibeta(a, b, x):
   //
   std::cout << "Running sanity checks for type " << name << std::endl;

#  include "ibeta_small_data.ipp"

   test_inverses(ibeta_small_data);

#  include "ibeta_data.ipp"

   test_inverses(ibeta_data);

#  include "ibeta_large_data.ipp"

   test_inverses(ibeta_large_data);
#ifndef FULL_TEST
   if(boost::is_floating_point<T>::value){
#endif
   //
   // This accuracy test is normally only enabled for "real"
   // floating point types and not for class real_concept.
   // The reason is that these tests are exceptionally slow
   // to complete when T doesn't have Lanczos support defined for it.
   //
#  include "ibeta_inva_data.ipp"

   test_inverses2(ibeta_inva_data, name, "Inverse incomplete beta");
#ifndef FULL_TEST
   }
#endif
}

int test_main(int, char* [])
{
   expected_results();
#ifdef TEST_GSL
   gsl_set_error_handler_off();
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



