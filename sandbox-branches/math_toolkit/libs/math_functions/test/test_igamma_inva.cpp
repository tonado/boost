//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/concepts/real_concept.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/tools/stats.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "handle_test_result.hpp"

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the incomplete gamma function inverses 
// gamma_P_inva and gamma_Q_inva. There are two sets of tests:
// 2) TODO: Accuracy tests use values generated with NTL::RR at 
// 1000-bit precision and our generic versions of these functions.
// 3) Round trip sanity checks, use the test data for the forward
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
   // Linux:
   //
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "linux",                          // platform
      largest_type,                     // test type(s)
      "[^|]*",                          // test data group
      "[^|]*", 800, 200);               // test function

   //
   // Catch all cases come last:
   //
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*",                          // test data group
      "[^|]*", 2000, 1000);             // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      largest_type,                     // test type(s)
      "[^|]*",                          // test data group
      "[^|]*", 300, 100);               // test function
   // this one has to come last in case double *is* the widest
   // float type:
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      "float|double",                   // test type(s)
      "[^|]*",                          // test data group
      "[^|]*", 10, 5);                 // test function
   //
   // Finish off by printing out the compiler/stdlib/platform names,
   // we do this to make it easier to mark up expected error rates.
   //
   std::cout << "Tests run with " << BOOST_COMPILER << ", " 
      << BOOST_STDLIB << ", " << BOOST_PLATFORM << std::endl;
}

template <class T>
void do_test_gamma_2(const T& data, const char* type_name, const char* test_name)
{
   //
   // test gamma_P_inva(T, T) against data:
   //
   using namespace std;
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   std::cout << test_name << " with type " << type_name << std::endl;

   //
   // These sanity checks test for a round trip accuracy of one half
   // of the bits in T, unless T is type float, in which case we check
   // for just one decimal digit.  The problem here is the sensitivity
   // of the functions, not their accuracy.  This test data was generated
   // for the forward functions, which means that when it is used as
   // the input to the inverses then it is necessarily inexact.  This rounding
   // of the input is what makes the data unsuitable for use as an accuracy check,
   // and also demonstrates that you can't in general round-trip these functions.
   // It is however a useful sanity check.
   //
   value_type precision = static_cast<value_type>(ldexp(1.0, 1-boost::math::tools::digits<value_type>()/2)) * 100;
   if(boost::math::tools::digits<value_type>() < 50)
      precision = 1;   // 1% or two decimal digits, all we can hope for when the input is truncated to float

   for(unsigned i = 0; i < data.size(); ++i)
   {
      //
      // These inverse tests are thrown off if the output of the
      // incomplete gamma is too close to 1: basically there is insuffient
      // information left in the value we're using as input to the inverse
      // to be able to get back to the original value.
      //
      if(data[i][5] == 0)
         BOOST_CHECK_EQUAL(boost::math::gamma_P_inva(data[i][1], data[i][5]), boost::math::tools::max_value<value_type>());
      else if((1 - data[i][5] > 0.001) && (fabs(data[i][5]) >= boost::math::tools::min_value<value_type>()))
      {
         value_type inv = boost::math::gamma_P_inva(data[i][1], data[i][5]);
         BOOST_CHECK_CLOSE(data[i][0], inv, precision);
      }
      else if(1 == data[i][5])
         BOOST_CHECK_EQUAL(boost::math::gamma_P_inva(data[i][1], data[i][5]), boost::math::tools::min_value<value_type>());
      else
      {
         // not enough bits in our input to get back to x, but we should be in
         // the same ball park:
         value_type inv = boost::math::gamma_P_inva(data[i][1], data[i][5]);
         BOOST_CHECK_CLOSE(data[i][0], inv, 100);
      }

      if(data[i][3] == 0)
         BOOST_CHECK_EQUAL(boost::math::gamma_Q_inva(data[i][1], data[i][3]), boost::math::tools::min_value<value_type>());
      else if((1 - data[i][3] > 0.001) && (fabs(data[i][3]) >= boost::math::tools::min_value<value_type>()))
      {
         value_type inv = boost::math::gamma_Q_inva(data[i][1], data[i][3]);
         BOOST_CHECK_CLOSE(data[i][0], inv, precision);
      }
      else if(1 == data[i][3])
         BOOST_CHECK_EQUAL(boost::math::gamma_Q_inva(data[i][1], data[i][3]), boost::math::tools::max_value<value_type>());
      else
      {
         // not enough bits in our input to get back to x, but we should be in
         // the same ball park:
         value_type inv = boost::math::gamma_Q_inva(data[i][1], data[i][3]);
         BOOST_CHECK_CLOSE(data[i][0], inv, 100);
      }
   }
   std::cout << std::endl;
}

template <class T>
void do_test_gamma_inva(const T& data, const char* type_name, const char* test_name)
{
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type, value_type);
   pg funcp = boost::math::gamma_P_inva;

   using namespace boost::lambda;

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test gamma_P_inva(T, T) against data:
   //
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
      ret<value_type>(_1[2]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::gamma_P_inva", test_name);
   //
   // test gamma_Q_inva(T, T) against data:
   //
   funcp = boost::math::gamma_Q_inva;
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
      ret<value_type>(_1[3]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::gamma_Q_inva", test_name);
}

template <class T>
void test_gamma(T, const char* name)
{
   //
   // The actual test data is rather verbose, so it's in a separate file
   //
   // First the data for the incomplete gamma function, each
   // row has the following 6 entries:
   // Parameter a, parameter z,
   // Expected tgamma(a, z), Expected gamma_Q(a, z)
   // Expected tgamma_lower(a, z), Expected gamma_P(a, z)
   //
#  include "igamma_med_data.ipp"

   do_test_gamma_2(igamma_med_data, name, "Running round trip sanity checks on incomplete gamma medium sized values");

#  include "igamma_small_data.ipp"

   do_test_gamma_2(igamma_small_data, name, "Running round trip sanity checks on incomplete gamma small values");

#  include "igamma_big_data.ipp"

   do_test_gamma_2(igamma_big_data, name, "Running round trip sanity checks on incomplete gamma large values");

#  include "igamma_inva_data.ipp"

   do_test_gamma_inva(igamma_inva_data, name, "Incomplete gamma inverses.");
}

int test_main(int, char* [])
{
   expected_results();

   test_gamma(0.1F, "float");
   test_gamma(0.1, "double");
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_gamma(0.1L, "long double");
   test_gamma(boost::math::concepts::real_concept(0.1), "real_concept");
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif
   return 0;
}


