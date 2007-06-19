// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// test_lognormal.cpp

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_OVERFLOW_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4100) // unreferenced formal parameter.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4510) // default constructor could not be generated.
#  pragma warning(disable: 4610) // can never be instantiated - user defined constructor required.
#  if !(defined _SCL_SECURE_NO_DEPRECATE) || (_SCL_SECURE_NO_DEPRECATE == 0)
#    pragma warning(disable: 4996) // 'std::char_traits<char>::copy' was declared deprecated.
#  endif
#endif

#include <boost/math/concepts/real_concept.hpp> // for real_concept
#include <boost/test/included/test_exec_monitor.hpp> // Boost.Test
#include <boost/test/floating_point_comparison.hpp>

#include <boost/math/distributions/lognormal.hpp>
	 using boost::math::lognormal_distribution;
#include <boost/math/tools/test.hpp>

#include <iostream>
	using std::cout;
	using std::endl;
	using std::setprecision;
#include <limits>
  using std::numeric_limits;

template <class RealType>
void check_lognormal(RealType loc, RealType scale, RealType x, RealType p, RealType q, RealType tol)
{
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         lognormal_distribution<RealType>(loc, scale),       // distribution.
         x),                                            // random variable.
         p,                                             // probability.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::cdf(
         complement(
            lognormal_distribution<RealType>(loc, scale),    // distribution.
            x)),                                        // random variable.
         q,                                             // probability complement.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         lognormal_distribution<RealType>(loc, scale),       // distribution.
         p),                                            // probability.
         x,                                             // random variable.
			tol);                                          // %tolerance.
   BOOST_CHECK_CLOSE(
      ::boost::math::quantile(
         complement(
            lognormal_distribution<RealType>(loc, scale),    // distribution.
            q)),                                        // probability complement.
         x,                                             // random variable.
			tol);                                          // %tolerance.
}

template <class RealType>
void test_spots(RealType T)
{
   // Basic santity checks
	RealType tolerance = 5e-3 * 100;
	// Some tests only pass at 1e-4 because values generated by
   // http://faculty.vassar.edu/lowry/VassarStats.html
   // give only 5 or 6 *fixed* places, so small values have fewer
   // digits.

	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;

   using std::exp;

   //
   // These test values were generated for the normal distribution
   // using the online calculator at http://faculty.vassar.edu/lowry/VassarStats.html
   // and then exponentiating the random variate x.
   //

   check_lognormal(
      static_cast<RealType>(0),     // location
      static_cast<RealType>(5),     // scale
      static_cast<RealType>(1),     // x
      static_cast<RealType>(0.5),   // p
      static_cast<RealType>(0.5),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(2),     // location
      static_cast<RealType>(2),     // scale
      static_cast<RealType>(exp(1.8)),     // x
      static_cast<RealType>(0.46017),   // p
      static_cast<RealType>(1-0.46017),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(2),     // location
      static_cast<RealType>(2),     // scale
      static_cast<RealType>(exp(2.2)),     // x
      static_cast<RealType>(1-0.46017),   // p
      static_cast<RealType>(0.46017),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(2),     // location
      static_cast<RealType>(2),     // scale
      static_cast<RealType>(exp(-1.4)),     // x
      static_cast<RealType>(0.04457),   // p
      static_cast<RealType>(1-0.04457),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(2),     // location
      static_cast<RealType>(2),     // scale
      static_cast<RealType>(exp(5.4)),     // x
      static_cast<RealType>(1-0.04457),   // p
      static_cast<RealType>(0.04457),   // q
      tolerance);

   check_lognormal(
      static_cast<RealType>(-3),     // location
      static_cast<RealType>(5),     // scale
      static_cast<RealType>(exp(-5.0)),     // x
      static_cast<RealType>(0.34458),   // p
      static_cast<RealType>(1-0.34458),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(-3),     // location
      static_cast<RealType>(5),     // scale
      static_cast<RealType>(exp(-1.0)),     // x
      static_cast<RealType>(1-0.34458),   // p
      static_cast<RealType>(0.34458),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(-3),     // location
      static_cast<RealType>(5),     // scale
      static_cast<RealType>(exp(-9.0)),     // x
      static_cast<RealType>(0.11507),   // p
      static_cast<RealType>(1-0.11507),   // q
      tolerance);
   check_lognormal(
      static_cast<RealType>(-3),     // location
      static_cast<RealType>(5),     // scale
      static_cast<RealType>(exp(3.0)),     // x
      static_cast<RealType>(1-0.11507),   // p
      static_cast<RealType>(0.11507),   // q
      tolerance);

   //
   // Tests for PDF
   //
   tolerance = boost::math::tools::epsilon<RealType>() * 5 * 100; // 5 eps as a percentage
	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;
   BOOST_CHECK_CLOSE(
      pdf(lognormal_distribution<RealType>(), static_cast<RealType>(1)),
      static_cast<RealType>(0.3989422804014326779399460599343818684759L), // 1/sqrt(2*pi)
      tolerance);
   BOOST_CHECK_CLOSE(
      pdf(lognormal_distribution<RealType>(3), exp(static_cast<RealType>(3))),
      static_cast<RealType>(0.3989422804014326779399460599343818684759L) / exp(static_cast<RealType>(3)),
      tolerance);
   BOOST_CHECK_CLOSE(
      pdf(lognormal_distribution<RealType>(3, 5), exp(static_cast<RealType>(3))),
      static_cast<RealType>(0.3989422804014326779399460599343818684759L / (5 * exp(static_cast<RealType>(3)))),
      tolerance);
   //
   // Spot checks for location = -5, scale = 6,
   // use relation to normal to test:
   //
   for(RealType x = -15; x < 5; x += 0.125)
   {
      BOOST_CHECK_CLOSE(
         pdf(lognormal_distribution<RealType>(-5, 6), exp(x)),
         pdf(boost::math::normal_distribution<RealType>(-5, 6), x) / exp(x),
         tolerance);
   }

   //
   // These test values were obtained by punching numbers into
   // a calculator, using the formulas at http://mathworld.wolfram.com/LogNormalDistribution.html
   //
   tolerance = (std::max)(
      boost::math::tools::epsilon<RealType>(),
      static_cast<RealType>(boost::math::tools::epsilon<double>())) * 5 * 100; // 5 eps as a percentage
	cout << "Tolerance for type " << typeid(T).name()  << " is " << tolerance << " %" << endl;
   lognormal_distribution<RealType> dist(8, 3);
   RealType x = static_cast<RealType>(0.125);
   using namespace std; // ADL of std names.
   // mean:
   BOOST_CHECK_CLOSE(
      mean(dist)
      , static_cast<RealType>(268337.28652087445695647967378715L), tolerance);
   // variance:
   BOOST_CHECK_CLOSE(
      variance(dist)
      , static_cast<RealType>(583389737628117.49553037857325892L), tolerance);
   // std deviation:
   BOOST_CHECK_CLOSE(
    standard_deviation(dist)
    , static_cast<RealType>(24153462.228594009489719473727471L), tolerance);
   // hazard:
   BOOST_CHECK_CLOSE(
    hazard(dist, x)
    , pdf(dist, x) / cdf(complement(dist, x)), tolerance);
   // cumulative hazard:
   BOOST_CHECK_CLOSE(
    chf(dist, x)
    , -log(cdf(complement(dist, x))), tolerance);
   // coefficient_of_variation:
   BOOST_CHECK_CLOSE(
    coefficient_of_variation(dist)
    , standard_deviation(dist) / mean(dist), tolerance);
   // mode:
   BOOST_CHECK_CLOSE(
    mode(dist)
    , static_cast<RealType>(0.36787944117144232159552377016146L), tolerance);

   BOOST_CHECK_CLOSE(
    median(dist)
    , static_cast<RealType>(exp(dist.location())), tolerance);

   BOOST_CHECK_CLOSE(
    median(dist),
    quantile(dist, static_cast<RealType>(0.5)), tolerance);

   // skewness:
   BOOST_CHECK_CLOSE(
    skewness(dist)
    , static_cast<RealType>(729551.38304660255658441529235697L), tolerance);
   // kertosis:
   BOOST_CHECK_CLOSE(
    kurtosis(dist)
    , static_cast<RealType>(4312295840576303.2363383232038251L), tolerance);
   // kertosis excess:
   BOOST_CHECK_CLOSE(
    kurtosis_excess(dist)
    , static_cast<RealType>(4312295840576300.2363383232038251L), tolerance);

   BOOST_CHECK_CLOSE(
    range(dist).first
    , static_cast<RealType>(0), tolerance);

   //
   // Special cases:
   //
   BOOST_CHECK(pdf(dist, 0) == 0);
   BOOST_CHECK(cdf(dist, 0) == 0);
   BOOST_CHECK(cdf(complement(dist, 0)) == 1);
   BOOST_CHECK(quantile(dist, 0) == 0);
   BOOST_CHECK(quantile(complement(dist, 1)) == 0);

   //
   // Error checks:
   //
   BOOST_CHECK_THROW(lognormal_distribution<RealType>(0, -1), std::domain_error);
   BOOST_CHECK_THROW(pdf(dist, -1), std::domain_error);
   BOOST_CHECK_THROW(cdf(dist, -1), std::domain_error);
   BOOST_CHECK_THROW(cdf(complement(dist, -1)), std::domain_error);
   BOOST_CHECK_THROW(quantile(dist, 1), std::overflow_error);
   BOOST_CHECK_THROW(quantile(complement(dist, 0)), std::overflow_error);

} // template <class RealType>void test_spots(RealType)

int test_main(int, char* [])
{

	// Check that can generate lognormal distribution using the two convenience methods:
	boost::math::lognormal myf1(1., 2); // Using typedef
	lognormal_distribution<> myf2(1., 2); // Using default RealType double.

  // Test range and support using double only,
  // because it supports numeric_limits max for a pseudo-infinity.
  BOOST_CHECK_EQUAL(range(myf2).first, 0); // range 0 to +infinity
  BOOST_CHECK_EQUAL(range(myf2).second, (std::numeric_limits<double>::max)());
  BOOST_CHECK_EQUAL(support(myf2).first, 0); // support 0 to + infinity.
  BOOST_CHECK_EQUAL(support(myf2).second, (std::numeric_limits<double>::max)());


	 // Basic sanity-check spot values.
	// (Parameter value, arbitrarily zero, only communicates the floating point type).
  test_spots(0.0F); // Test float. OK at decdigits = 0 tolerance = 0.0001 %
  test_spots(0.0); // Test double. OK at decdigits 7, tolerance = 1e07 %
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
  test_spots(0.0L); // Test long double.
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x0582))
  test_spots(boost::math::concepts::real_concept(0.)); // Test real concept.
#endif
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif

   return 0;
} // int test_main(int, char* [])

/*

------ Rebuild All started: Project: test_lognormal, Configuration: Debug Win32 ------
Deleting intermediate and output files for project 'test_lognormal', configuration 'Debug|Win32'
Compiling...
test_lognormal.cpp
Linking...
Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\test_lognormal.exe"
Running 1 test case...
Tolerance for type float is 0.5 %
Tolerance for type float is 5.96046e-005 %
Tolerance for type float is 5.96046e-005 %
../../../../../../boost-sandbox/libs/math_functions/test/test_lognormal.cpp(222): error in "test_main_caller( argc, argv )": difference between mode(dist){24154952} and static_cast<RealType>(0.36787944117144232159552377016146L){0.36787945} exceeds 5.96046e-005%
../../../../../../boost-sandbox/libs/math_functions/test/test_lognormal.cpp(226): error in "test_main_caller( argc, argv )": difference between skewness(dist){729506.375} and static_cast<RealType>(729551.38304660255658441529235697L){729551.375} exceeds 5.96046e-005%
../../../../../../boost-sandbox/libs/math_functions/test/test_lognormal.cpp(239): error in "test_main_caller( argc, argv )": check pdf(dist, 0) == 0 failed
unknown location(0): fatal error in "test_main_caller( argc, argv )": std::overflow_error: Error in function float __cdecl boost::math::erfc_inv<float>(float): Overflow
..\..\..\..\..\..\boost-sandbox\libs\math_functions\test\test_lognormal.cpp(242): last checkpoint
*** 4 failures detected in test suite "Test Program"
Project : error PRJ0019: A tool returned an error code from "Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\test_lognormal.exe""
Build Time 0:06
Build log was saved at "file://i:\boost-06-05-03-1300\libs\math\test\Math_test\test_lognormal\Debug\BuildLog.htm"
test_lognormal - 5 error(s), 0 warning(s)
========== Rebuild All: 0 succeeded, 1 failed, 0 skipped ==========


*/

