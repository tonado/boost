// negative_binomial_example1.cpp

// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_MATH_THROW_ON_DOMAIN_ERROR

#ifdef _MSC_VER
#  pragma warning(disable: 4127) // conditional expression is constant.
#  pragma warning(disable: 4512) // assignment operator could not be generated.
#  pragma warning(disable: 4996) // 'std::char_traits<char>::copy' was declared deprecated.
#endif

// Example 1 of using negative_binomial

// http://en.wikipedia.org/wiki/Negative_binomial_distribution

// (After a problem by Dr. Diane Evans,
// Professor of mathematics at Rose-Hulman Institute of Technology)

// Pat is required to sell candy bars to raise money for the 6th grade field trip.
// There are thirty houses in the neighborhood,
// and Pat is not supposed to return home until five candy bars have been sold.
// So the child goes door to door, selling candy bars.
// At each house, there is a 0.4 probability of selling one candy bar
// and a 0.6 probability of selling nothing.

// What's the probability mass function for selling the last candy bar at the nth house?

// Recall that the Negative Binomial(r, p) distribution describes the probability of k failures
// and r successes in k+r Bernoulli(p) trials with success on the last trial.
// Selling five candy bars means getting five successes, so r = 5.
// The total number of trials (in this case, houses) this takes is therefore
//    sucesses + failures or k + r = k + 5 = n.
// The random variable we are interested in is the number of houses k
// that must be visited to sell five candy bars,
// so we substitute k = n − 5 into a NegBin(5, 0.4) mass function
// and obtain the following mass function of the distribution of houses (for n ≥ 5):

// What's the probability that Pat finishes ON the tenth house?

//    f(10) = 0.1003290624, or about 1 in 10

// What's the probability that Pat finishes ON OR BEFORE reaching the eighth house?

// To finish on or before the eighth house,
// Pat must finish at the fifth, sixth, seventh, or eighth house.
// Sum those probabilities:

    // f(5) = 0.01024
    // f(6) = 0.03072
    // f(7) = 0.055296
    // f(8) = 0.0774144
    // sum {j=5 to 8} f(j) = 0.17367

// What's the probability that Pat exhausts all 30 houses in the neighborhood,
// and doesn't sell the required 5 candy bars?

// 1 - sum{j=5 to 30} f(j) = 1 - incomplete beta (p = 0.4)(5, 30-5+1) =~ 1 - 0.99849 = 0.00151 

// see also http://www.math.uah.edu/stat/bernoulli/Introduction.xhtml
// http://www.codecogs.com/pages/catgen.php?category=stats/dists/discrete


#include <boost/math/special_functions/beta.hpp>
	using boost::math::ibeta;  // incomplete beta function.
#include <boost/math/special_functions/negative_binomial.hpp>
	using boost::math::negative_binomial;  // Probability of negative_binomial.
	using boost::math::negative_binomial_inv;  // inverse negative_binomial - Probability of success.

#include <iostream>
	using std::cout;
	using std::endl;
#include <iomanip>
	using std::setprecision;

#include <boost/math/special_functions/factorials.hpp>
	//using boost::math::checked_factorial;  // 
#include <boost/math/special_functions/gamma.hpp>
	//using boost::math::lgamma;  // log gamma

// This needs to be a template function and to go into negative-binomial.hpp
// but leave until re-factor all functions.  TODO

double negative_binomial_pdf(double k, double r, double p)
{ // negative binomial probability density/mass function pdf.
  // According to definition, parameters are integral really?
	using boost::math::lgamma;  // log gamma
  if (k < 0)
  {
    return 0; //  return domain_error<RealType>(BOOST_CURRENT_FUNCTION, "k argument is %1%, but must be >= 0 !", k);

  }
  if((p > 1) || (p < 0))
  { //  logic_error<RealType>(BOOST_CURRENT_FUNCTION, "success fraction is %1%, but must be >= 0 and <= 1 !", success_probability);
    //return static_cast<RealType>(0.); // Constrain to zero if domain_error does not throw.
    return 0;
  }
  if (r <= 0)
  {
    //  return domain_error<RealType>(BOOST_CURRENT_FUNCTION, "n argument is %1%, but must be >= 0 !", n);
    return 0;
  }
  return exp(lgamma(r + k) - lgamma(r) - lgamma(k+1)) * pow(p, r) * pow((1-p), k);
} // negative_binomial_pdf

int negative_binomial_inv_k(double cdf, double r, double p)
{
  using std::numeric_limits;
  if((p > 1) || (p < 0))
  { //  domain_error<RealType>(BOOST_CURRENT_FUNCTION, "success fraction is %1%, but must be >= 0 and <= 1 !", success_probability);
    //return static_cast<RealType>(0.); // Constrain to zero if domain_error does not throw.
    return 0;
  }
  if((cdf > 1) || (cdf < 0))
  { //  ldomain_error<RealType>(BOOST_CURRENT_FUNCTION, "success fraction is %1%, but must be >= 0 and <= 1 !", success_probability);
    //return static_cast<RealType>(0.); // Constrain to zero if domain_error does not throw.
    return 0;
  }
  if (p == 1.)
  {
    return (numeric_limits<int>::max)();
  }
  // Method: simple discrete approach,
  // calling & summing the probability density function repeatedly with increasing k,
  // until >= cdf.

  cdf -= numeric_limits<double>::epsilon() * 2;
  double cumcdf = 0.;
  int i = 0; // static_cast<int>(r);
  do
  {
    cumcdf += negative_binomial_pdf(i, r, p);
    cout << i << ' ' << cumcdf << endl;
    if (cumcdf >= cdf)
    {
       return i;
    }
    i++;
  }
  while (i < 100);
  return (numeric_limits<int>::max)();
} // negative_binomial_inv_k


int main()
{
	cout << "Example 1 using negative_binomial function.";
  #if defined(__FILE__) && defined(__TIMESTAMP__)
  	cout << "  " << __FILE__ << ' ' << __TIMESTAMP__ << ' '<< _MSC_FULL_VER << "\n";
  #endif
	cout << endl;

  try
  {
    double successful_sales = 5; // Pat's sales quota.
    double success_fraction = 0.4; // So 0.6 fail_fraction.
    int all_houses = 30; // on the estate.
    cout <<"Selling candy bars - an example of using the negative binomial distribution. " << setprecision(3)
      << "\n""An example by Dr. Diane Evans,"
      "\n""Professor of Mathematics at Rose-Hulman Institute of Technology,"
      << "\n""see http://en.wikipedia.org/wiki/Negative_binomial_distribution""\n" 
      << endl;
    cout << "Pat has a sales per house success rate of " << success_fraction
      << ",""\n""so he would, on average, sell " << success_fraction * 100 << " bars after trying 100 houses." << endl;

    cout << "With a success rate of " << success_fraction  << ", he might expect, on average, to need to visit about "
      << success_fraction * all_houses << " houses in order to sell all " << successful_sales << " candy bars. " << endl;

    // To finish on or before the 8th house, Pat must finish at the 5th, 6th, 7th or 8th house.
    // Sum those probabilities to find the probability that he will finish exactly on the tenth house.

    cout << "Probability that Pat finishes on the " << successful_sales << "th house is " << "f(5) = " << negative_binomial_pdf(successful_sales - successful_sales, successful_sales, success_fraction) << endl;
    cout << "Probability that Pat finishes on the 6th house is " << "f(6) = " << negative_binomial_pdf(6 - successful_sales, successful_sales, success_fraction) << endl;
    cout << "Probability that Pat finishes on the 7th house is " << "f(7) = " << negative_binomial_pdf(7 - successful_sales, successful_sales, success_fraction) << endl;
    cout << "Probability that Pat finishes on the 8th house is " << "f(8) = " << negative_binomial_pdf(8 - successful_sales, successful_sales, success_fraction) << endl;

    cout << "Probability that Pat finishes on or before the 8th house is " << "f(successful_sales) + f(6) + f(7) + f(8) = "
      << negative_binomial_pdf(successful_sales - successful_sales, successful_sales, success_fraction) // Sum each of the mass/density probabilities for houses successful_sales, 6, 7, & 8.
      + negative_binomial_pdf(6 - successful_sales, successful_sales, success_fraction) 
      + negative_binomial_pdf(7 - successful_sales, successful_sales, success_fraction)
      + negative_binomial_pdf(8 - successful_sales, successful_sales, success_fraction)
      << endl;

    // Or using the negative binomial **cumulative** distribution function (cdf instead of the pdf):
    cout << "\n""Probability of selling his quota of " << successful_sales
      << " candy bars""\n""on or before the " << 8 << "th house is "
      << negative_binomial(8 - successful_sales, successful_sales, success_fraction) << endl;

    cout << "Probability that Pat finishes on the 10th house is " << "f(10) = " << negative_binomial_pdf(10 - successful_sales, successful_sales, success_fraction) << endl;
    cout << "\n""Probability of selling his quota of " << successful_sales
      << " candy bars""\n""on or before the " << 10 << "th house is "
      << negative_binomial(10 - successful_sales, successful_sales, success_fraction) << endl;

    cout << "Probability that Pat finishes on the 11th house is " << "f(11) = " << negative_binomial_pdf(11 - successful_sales, successful_sales, success_fraction) << endl;
    cout << "\n""Probability of selling his quota of " << successful_sales
      << " candy bars""\n""on or before the " << 11 << "th house is "
      << negative_binomial(11 - successful_sales, successful_sales, success_fraction) << endl;

    cout << "Probability that Pat finishes on the 12th house is " << "f(10) = " << negative_binomial_pdf(12 - successful_sales, successful_sales, success_fraction) << endl;
    cout << "\n""Probability of selling his quota of " << successful_sales
      << " candy bars""\n""on or before the " << 12 << "th house is "
      << negative_binomial(12 - successful_sales, successful_sales, success_fraction) << endl;

    cout << "\n""Probability of failing to sell his quota of " << successful_sales
      << " candy bars""\n""even after visiting all " << all_houses << "  houses is "
      << 1 - negative_binomial(all_houses - successful_sales, successful_sales, success_fraction) << endl;
    // cumulative negative_binomial(30 - 5 + 1, 5, 0.4) = negative_binomial(26, 5, 0.4)

    // This is the Cephes and Codecogs inverse.
    cout.precision(17);
    double p =  negative_binomial(8-5, 5, 0.4);
    cout << "Probability of sales quota on or before 8th house is "<< p << endl;
    // Probability of sales quota on or before 8th house is 0.17367040000000009

      cout << "Probability of sales success is " << negative_binomial_inv(8-5, 5, p) << endl;
    // Probability of sales success is 0.40000000000000002

     // But finding the smallest k as provided by MathCAD inverse would be useful.
     // qnbinom returns smallest k, not the probability, for which z = negative_binomial(k, n, p) >= p
     // k = negative_binomial_inv(z, n, p)

      double z = negative_binomial_inv_k(0.17367040000000009, 5, 0.4);
      cout << "" << z << ' ' << z + 5 << endl;
   }
  catch(const std::exception& e)
   {
      std::cout << 
          "\n""Message from thrown exception was:\n   " << e.what() << std::endl; 
   }

	return 0;
}  // int main()

/*

Output is:

Example 1 using negative_binomial function.  ..\..\..\..\..\..\boost-sandbox\lib
s\math_functions\example\negative_binomial_example1.cpp Wed Jul 12 14:45:16 2006
 140050727

Selling candy bars - an example of using the negative binomial distribution.
An example by Dr. Diane Evans,
Professor of Mathematics at Rose-Hulman Institute of Technology,
see http://en.wikipedia.org/wiki/Negative_binomial_distribution

Pat has a sales per house success rate of 0.4,
so he would, on average, sell 40 bars after trying 100 houses.
With a success rate of 0.4, he would, on average,
expect to need to visit 12 houses in order to sell all 5 candy bars.
Probability that Pat finishes on the 5th house is f(5) = 0.01024
Probability that Pat finishes on the 6th house is f(6) = 0.03072
Probability that Pat finishes on the 7th house is f(7) = 0.055296
Probability that Pat finishes on the 8th house is f(8) = 0.077414
Probability that Pat finishes on or before the 8th house is f(5) + f(6) + f(7) + f(8) = 0.17367

Probability of selling his quota of 5 candy bars on or before the 8th house is 0.17367
Probability that Pat finishes on the 10th house is f(10) = 0.10033

Probability of selling his quota of 5 candy bars on or before the 10th house is 0.3669
Probability that Pat finishes on the 11th house is f(11) = 0.10033

Probability of selling his quota of 5 candy bars on or before the 11th house is 0.46723
Probability that Pat finishes on the 12th house is f(10) = 0.094596

Probability of selling his quota of 5 candy bars on or before the 12th house is 0.56182

Probability of failing to sell his quota of 5 candy bars even after visiting all 30 is 0.0015101

Press any key to continue . . .
*/





