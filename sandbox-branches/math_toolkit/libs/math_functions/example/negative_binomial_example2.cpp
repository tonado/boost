// negative_binomial_example2.cpp

// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Simple examples demonstrating use of the Negative Binomial Distribution.
// (See other examples for practical applications).

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#endif

#include <boost/math/distributions/negative_binomial.hpp>
  using boost::math::negative_binomial_distribution;
  using boost::math::negative_binomial; // typedef

// In a sequence of trials or events
// (Bernoulli, independent, yes or no, succeed or fail)
// with success_fraction probability p,
// negative_binomial is the probability that k or fewer failures
// preceed the r th trial's success.

#include <iostream>
using std::cout;
using std::endl;
using std::setprecision;
using std::showpoint;
using std::setw;
using std::left;
using std::right;
#include <limits>
using std::numeric_limits;

int main()
{
#ifdef BOOST_MATH_THROW_ON_DOMAIN_ERROR
  cout << "BOOST_MATH_THROW_ON_DOMAIN_ERROR" << " is defined to throw on domain error." << endl;
#else
  cout << "BOOST_MATH_THROW_ON_DOMAIN_ERROR" << " is NOT defined, so NO throw on domain error." << endl;
#endif
  cout << "negative_binomial distribution - simples example 2" << endl;

  // Construct distribution: 8 successes (r), 0.25 success fraction = 25% or 1 in 4 successes.
  // negative_binomial_distribution<double> my8dist(8, 0.25);
  negative_binomial my8dist(8, 0.25); // Shorter method using typedef.

  cout.precision(17); //  max_digits10 precision
  cout << "mean(my8dist) = " << mean(my8dist) << endl; // 24
  cout << "my8dist.successes() = " << my8dist.successes()  << endl;
  // r th successful trial, after k failures, is r + k th trial.
  cout << "my8dist.success_fraction() = " << my8dist.success_fraction()  << endl; 
  // success_fraction = failures/successes or k/r = 0.25 or 25%. 
  cout << "my8dist.percent success  = " << my8dist.success_fraction() * 100 << "%"  << endl;
  cout << "cdf(my8dist, 2.) = " << cdf(my8dist, 2.) << endl; // 0.000415802001953125
  cout << "cdf(my8dist, 8.) = " << cdf(my8dist, 8.) << endl; // 0.027129956288263202
  cout << "cdf(complement(my8dist, 8.)) = " << cdf(complement(my8dist, 8.)) << endl; // 0.9728700437117368
  // Check that cdf plus its complement is unity.
  cout << "cdf + complement = " << cdf(my8dist, 8.) + cdf(complement(my8dist, 8.))  << endl; // 1
  // Note: No complement for pdf! 

  // Compare cdf with sum of pdfs.
  double sum = 0.; // of pdfs
  int k = 20;
  for(signed i = 0; i <= k; ++i)
  {
    sum += pdf(my8dist, double(i));
  }
  // Compare with 
  double cdf8 = cdf(my8dist, static_cast<double>(k));
  double diff = sum - cdf8;

  cout << setprecision(17) << "Sum pdfs = " << sum << ' '  // 0.40025683281803698
  << ", cdf = " << cdf(my8dist, static_cast<double>(k)) //  cdf = 0.40025683281803687
  << ", difference = " 
  << diff/ std::numeric_limits<double>::epsilon() // difference = 0.50000000000000000
  << " in epsilon units." << endl;

  // Note: Use boost::math::tools::epsilon rather than std::numeric_limits to cover
  // RealTypes that do not specialize numeric_limits.

  // Print a list of values that can be used to plot
  // using Excel, or some other superior graphical display tool.

  cout << setprecision(17) << showpoint << endl; // max_digits10 precision, including trailing zeros.
  int maxk = static_cast<int>(2. * my8dist.successes() /  my8dist.success_fraction());
  // This maxk shows most of the range of interest, probability about 0.0001 to 0.9999.
  cout << " k            pdf                      cdf""\n" << endl;
  for (int k = 0; k < maxk; k++)
  {
    cout << right << setprecision(17) << showpoint
      << right << setw(3) << k  << ", "
      << left << setw(25) << pdf(my8dist, static_cast<double>(k)) << ", "
      << left << setw(25) << cdf(my8dist, static_cast<double>(k))
      << endl;
  }
  cout << endl;

  return 0;
} // int main()

/*

Output is

------ Build started: Project: negative_binomial_example2, Configuration: Debug Win32 ------
Compiling...
negative_binomial_example2.cpp
Linking...
Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\negative_binomial_example2.exe"
BOOST_MATH_THROW_ON_DOMAIN_ERROR is defined to throw on domain error.
negative_binomial distribution - simples example 2
mean(my8dist) = 24
my8dist.successes() = 8
my8dist.success_fraction() = 0.25
my8dist.percent success  = 25%
cdf(my8dist, 2.) = 0.000415802001953125
cdf(my8dist, 8.) = 0.027129956288263202
cdf(complement(my8dist, 8.)) = 0.9728700437117368
cdf + complement = 1
Sum pdfs = 0.40025683281803698 , cdf = 0.40025683281803687, difference = 0.5 in epsilon units.
 k            pdf                      cdf
  0, 1.5258789062499998e-005  , 1.5258789062499998e-005  
  1, 9.1552734374999959e-005  , 0.00010681152343750000   
  2, 0.00030899047851562522   , 0.00041580200195312500   
  3, 0.00077247619628906239   , 0.0011882781982421875    
  4, 0.0015932321548461931    , 0.0027815103530883789    
  5, 0.0028678178787231463    , 0.0056493282318115234    
  6, 0.0046602040529251142    , 0.010309532284736633     
  7, 0.0069903060793876605    , 0.017299838364124298     
  8, 0.0098301179241388984    , 0.027129956288263202     
  9, 0.013106823898851870     , 0.040236780187115073     
 10, 0.016711200471036133     , 0.056947980658151209     
 11, 0.020509200578089803     , 0.077457181236241013     
 12, 0.024354675686481628     , 0.10181185692272265      
 13, 0.028101548869017230     , 0.12991340579173993      
 14, 0.031614242477644432     , 0.16152764826938440      
 15, 0.034775666725408917     , 0.19630331499479325      
 16, 0.037492515688331465     , 0.23379583068312471      
 17, 0.039697957787645122     , 0.27349378847076977      
 18, 0.041352039362130291     , 0.31484582783290005      
 19, 0.042440250924291587     , 0.35728607875719176      
 20, 0.042970754060845266     , 0.40025683281803687      
 21, 0.042970754060845245     , 0.44322758687888220      
 22, 0.042482450037426567     , 0.48571003691630876      
 23, 0.041558918514873776     , 0.52726895543118257      
 24, 0.040260202311284021     , 0.56752915774246648      
 25, 0.038649794218832613     , 0.60617895196129912      
 26, 0.036791631035234945     , 0.64297058299653398      
 27, 0.034747651533277434     , 0.67771823452981139      
 28, 0.032575923312447595     , 0.71029415784225891      
 29, 0.030329307911589130     , 0.74062346575384819      
 30, 0.028054609818219924     , 0.76867807557206813      
 31, 0.025792141284492545     , 0.79447021685656061      
 32, 0.023575629142856457     , 0.81804584599941710      
 33, 0.021432390129869489     , 0.83947823612928651      
 34, 0.019383705779220179     , 0.85886194190850684      
 35, 0.017445335201298224     , 0.87630727710980494      
 36, 0.015628112784496322     , 0.89193538989430121      
 37, 0.013938587078064250     , 0.90587397697236549      
 38, 0.012379666154859706     , 0.91825364312722524      
 39, 0.010951243136991251     , 0.92920488626421649      
 40, 0.0096507830144735539    , 0.93885566927869002      
 41, 0.0084738582566109364    , 0.94732952753530097      
 42, 0.0074146259745345557    , 0.95474415350983555      
 43, 0.0064662435824429246    , 0.96121039709227851      
 44, 0.0056212231142827853    , 0.96683162020656122      
 45, 0.0048717266990450708    , 0.97170334690560634      
 46, 0.0042098073105878604    , 0.97591315421619418      
 47, 0.0036275999165703964    , 0.97954075413276465      
 48, 0.0031174686783026818    , 0.98265822281106729      
 49, 0.0026721160099737293    , 0.98533033882104104      
 50, 0.0022846591885275322    , 0.98761499800956853      
 51, 0.0019486798960970148    , 0.98956367790566557      
 52, 0.0016582516423517925    , 0.99122192954801736      
 53, 0.0014079495076571762    , 0.99262987905567457      
 54, 0.0011928461106539983    , 0.99382272516632852      
 55, 0.0010084971662802015    , 0.99483122233260868      
 56, 0.00085091948404891532   , 0.99568214181665760      
 57, 0.00071656377604119553   , 0.99639870559269883      
 58, 0.00060228420831048650   , 0.99700098980100937      
 59, 0.00050530624256557675   , 0.99750629604357488      
 60, 0.00042319397814867180   , 0.99792949002172360      
 61, 0.00035381791615708398   , 0.99828330793788067      
 62, 0.00029532382517950324   , 0.99857863176306016      
 63, 0.00024610318764958566   , 0.99882473495070978      
Build Time 0:03
Build log was saved at "file://i:\boost-06-05-03-1300\libs\math\test\Math_test\neative_binomial_example2\Debug\BuildLog.htm"
negative_binomial_example2 - 0 error(s), 0 warning(s)
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========


*/
