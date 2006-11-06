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

#include "test_gamma_hooks.hpp"
#include "handle_test_result.hpp"

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the incomplete gamma functions tgamma,
// tgamma_lower, gamma_P and gamma_Q. There are two sets of tests, spot
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
   // These should not really be needed, but on *some* Linux
   // versions these error rates are quite large and appear to
   // be related to the accuracy of powl and expl.  On Itanium
   // or Xeon machines the error rates are much lower than this.
   // Worst cases appear to be AMD64 machines.
   //
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "linux",                          // platform
      largest_type,                     // test type(s)
      "[^|]*medium[^|]*",               // test data group
      "[^|]*", 1000, 200);                 // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "linux",                          // platform
      largest_type,                     // test type(s)
      "[^|]*integer[^|]*",               // test data group
      "[^|]*", 1000, 200);                 // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "linux",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*medium[^|]*",               // test data group
      "[^|]*", 600, 200);                // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "linux",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*integer[^|]*",               // test data group
      "[^|]*", 600, 200);                // test function

   //
   // HP-UX:
   //
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "HP-UX",                          // platform
      largest_type,                     // test type(s)
      "[^|]*medium[^|]*",               // test data group
      "[^|]*", 500, 50);                 // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "HP-UX",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*medium[^|]*",               // test data group
      "[^|]*", 500, 100);                // test function

   //
   // Large exponent range causes more extreme test cases to be evaluated:
   //
   if(std::numeric_limits<long double>::max_exponent > std::numeric_limits<double>::max_exponent)
   {
      add_expected_result(
         "[^|]*",                          // compiler
         "[^|]*",                          // stdlib
         "[^|]*",                          // platform
         largest_type,                     // test type(s)
         "[^|]*large[^|]*",                // test data group
         ".*", 40000, 3000);  // test function
   }


   //
   // Catch all cases come last:
   //
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      largest_type,                     // test type(s)
      "[^|]*medium[^|]*",               // test data group
      "[^|]*", 50, 20);                 // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      largest_type,                     // test type(s)
      "[^|]*small[^|]*",                // test data group
      "[^|]*", 20, 10);                  // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      largest_type,                     // test type(s)
      "[^|]*large[^|]*",                // test data group
      "boost::math::gamma_Q", 500, 50);  // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "Cygwin",                         // platform
      largest_type,                     // test type(s)
      "[^|]*large[^|]*",                // test data group
      "boost::math::gamma_P", 700, 50);  // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      largest_type,                     // test type(s)
      "[^|]*large[^|]*",                // test data group
      "boost::math::gamma_P", 300, 50);  // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      largest_type,                     // test type(s)
      "[^|]*integer[^|]*",              // test data group
      ".*", 20, 10);                    // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*medium[^|]*",               // test data group
      "[^|]*", 200, 50);                // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*small[^|]*",                // test data group
      ".*", 20, 10);                  // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*large[^|]*",                // test data group
      ".*", 1000000, 100000);        // test function
   add_expected_result(
      "[^|]*",                          // compiler
      "[^|]*",                          // stdlib
      "[^|]*",                          // platform
      "real_concept",                   // test type(s)
      "[^|]*integer[^|]*",              // test data group
      ".*", 40, 10);                    // test function

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
   typedef typename T::value_type row_type;
   typedef typename row_type::value_type value_type;

   typedef value_type (*pg)(value_type, value_type);
   pg funcp = boost::math::tgamma;

   using namespace boost::lambda;

   boost::math::tools::test_result<value_type> result;

   std::cout << "Testing " << test_name << " with type " << type_name
      << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

   //
   // test tgamma(T, T) against data:
   //
   if(data[0][2] > 0)
   {
      result = boost::math::tools::test(
         data,
         bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
         ret<value_type>(_1[2]));
      handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::tgamma", test_name);
      //
      // test tgamma_lower(T, T) against data:
      //
      funcp = boost::math::tgamma_lower;
      result = boost::math::tools::test(
         data,
         bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
         ret<value_type>(_1[4]));
      handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::tgamma_lower", test_name);
   }
   //
   // test gamma_Q(T, T) against data:
   //
   funcp = boost::math::gamma_Q;
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
      ret<value_type>(_1[3]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::gamma_Q", test_name);
#if defined(TEST_CEPHES) || defined(TEST_GSL)
   //
   // test other gamma_Q(T, T) against data:
   //
   if(boost::is_floating_point<value_type>::value)
   {
      funcp = other::gamma_Q;
      result = boost::math::tools::test(
         data,
         bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
         ret<value_type>(_1[3]));
      print_test_result(result, data[result.worst()], result.worst(), type_name, "other::gamma_Q");
   }
#endif
   //
   // test gamma_P(T, T) against data:
   //
   funcp = boost::math::gamma_P;
   result = boost::math::tools::test(
      data,
      bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
      ret<value_type>(_1[5]));
   handle_test_result(result, data[result.worst()], result.worst(), type_name, "boost::math::gamma_P", test_name);
#if defined(TEST_CEPHES) || defined(TEST_GSL)
   //
   // test other gamma_P(T, T) against data:
   //
   if(boost::is_floating_point<value_type>::value)
   {
      funcp = other::gamma_P;
      result = boost::math::tools::test(
         data,
         bind(funcp, ret<value_type>(_1[0]), ret<value_type>(_1[1])),
         ret<value_type>(_1[5]));
      print_test_result(result, data[result.worst()], result.worst(), type_name, "other::gamma_P");
   }
#endif
   std::cout << std::endl;
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

   do_test_gamma_2(igamma_med_data, name, "tgamma(a, z) medium values");

#  include "igamma_small_data.ipp"

   do_test_gamma_2(igamma_small_data, name, "tgamma(a, z) small values");

#  include "igamma_big_data.ipp"

   do_test_gamma_2(igamma_big_data, name, "tgamma(a, z) large values");

#  include "igamma_int_data.ipp"

   do_test_gamma_2(igamma_int_data, name, "tgamma(a, z) integer and half integer values");
}

template <class T>
void test_spots(T)
{
   //
   // basic sanity checks, tolerance is 10 epsilon expressed as a percentage:
   //
   T tolerance = boost::math::tools::epsilon<T>() * 1000;
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(5), static_cast<T>(1)), static_cast<T>(23.912163676143750903709045060494956383977723517065L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(5), static_cast<T>(5)), static_cast<T>(10.571838841565097874621959975919877646444998907920L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(5), static_cast<T>(10)), static_cast<T>(0.70206451384706574414638719662835463671916532623256L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(5), static_cast<T>(100)), static_cast<T>(3.8734332808745531496973774140085644548465762343719e-36L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(0.5), static_cast<T>(0.5)), static_cast<T>(0.56241823159440712427949495730204306902676756479651L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(0.5), static_cast<T>(9)/10), static_cast<T>(0.31853210360412109873859360390443790076576777747449L), tolerance*10);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(0.5), static_cast<T>(5)), static_cast<T>(0.0027746032604128093194908357272603294120210079791437L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma(static_cast<T>(0.5), static_cast<T>(100)), static_cast<T>(3.7017478604082789202535664481339075721362102520338e-45L), tolerance);

   BOOST_CHECK_CLOSE(::boost::math::tgamma_lower(static_cast<T>(5), static_cast<T>(1)), static_cast<T>(0.087836323856249096290954939505043616022276482935091L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma_lower(static_cast<T>(5), static_cast<T>(5)), static_cast<T>(13.428161158434902125378040024080122353555001092080L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma_lower(static_cast<T>(5), static_cast<T>(10)), static_cast<T>(23.297935486152934255853612803371645363280834673767L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::tgamma_lower(static_cast<T>(5), static_cast<T>(100)), static_cast<T>(23.999999999999999999999999999999999996126566719125L), tolerance);

   BOOST_CHECK_CLOSE(::boost::math::gamma_Q(static_cast<T>(5), static_cast<T>(1)), static_cast<T>(0.99634015317265628765454354418728984933240514654437L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_Q(static_cast<T>(5), static_cast<T>(5)), static_cast<T>(0.44049328506521241144258166566332823526854162116334L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_Q(static_cast<T>(5), static_cast<T>(10)), static_cast<T>(0.029252688076961072672766133192848109863298555259690L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_Q(static_cast<T>(5), static_cast<T>(100)), static_cast<T>(1.6139305336977304790405739225035685228527400976549e-37L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_Q(static_cast<T>(1.5), static_cast<T>(2)), static_cast<T>(0.26146412994911062220282207597592120190281060919079L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_Q(static_cast<T>(20.5), static_cast<T>(22)), static_cast<T>(0.34575332043467326814971590879658406632570278929072L), tolerance);

   BOOST_CHECK_CLOSE(::boost::math::gamma_P(static_cast<T>(5), static_cast<T>(1)), static_cast<T>(0.0036598468273437123454564558127101506675948534556288L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_P(static_cast<T>(5), static_cast<T>(5)), static_cast<T>(0.55950671493478758855741833433667176473145837883666L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_P(static_cast<T>(5), static_cast<T>(10)), static_cast<T>(0.97074731192303892732723386680715189013670144474031L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_P(static_cast<T>(5), static_cast<T>(100)), static_cast<T>(0.9999999999999999999999999999999999998386069466302L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_P(static_cast<T>(1.5), static_cast<T>(2)), static_cast<T>(0.73853587005088937779717792402407879809718939080921L), tolerance);
   BOOST_CHECK_CLOSE(::boost::math::gamma_P(static_cast<T>(20.5), static_cast<T>(22)), static_cast<T>(0.65424667956532673185028409120341593367429721070928L), tolerance);

   // naive check on derivative function:
   using namespace std;  // For ADL of std functions
   tolerance = boost::math::tools::epsilon<T>() * 5000; // 50 eps
   BOOST_CHECK_CLOSE(::boost::math::gamma_P_derivative(static_cast<T>(20.5), static_cast<T>(22)), 
      exp(static_cast<T>(-22)) * pow(static_cast<T>(22), static_cast<T>(19.5)) / boost::math::tgamma(static_cast<T>(20.5)), tolerance);

}

int test_main(int, char* [])
{
   expected_results();

   test_spots(0.0F);
   test_spots(0.0);
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(0.0L);
   test_spots(boost::math::concepts::real_concept(0.1));
#endif

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


