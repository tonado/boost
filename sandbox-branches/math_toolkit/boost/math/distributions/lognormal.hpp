//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STATS_LOGNORMAL_HPP
#define BOOST_STATS_LOGNORMAL_HPP

// http://www.itl.nist.gov/div898/handbook/eda/section3/eda3669.htm
// http://mathworld.wolfram.com/LogNormalDistribution.html
// http://en.wikipedia.org/wiki/Lognormal_distribution

#include <boost/math/distributions/normal.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/math/distributions/detail/common_error_handling.hpp>

#include <utility>

namespace boost{ namespace math
{
namespace detail
{

  template <class RealType>
  bool check_lognormal_x(
        const char* function,
        RealType const& x,
        RealType* result)
  {
     if((x < 0) || !(boost::math::isfinite)(x))
     {
        *result = tools::domain_error<RealType>(
           function,
           "Random variate is %1% but must be > 0 !", x);
        return false;
     }
     return true;
  }

} // namespace detail


template <class RealType = double>
class lognormal_distribution
{
public:
   typedef RealType value_type;

   lognormal_distribution(RealType location = 0, RealType scale = 1)
      : m_location(location), m_scale(scale)
   {
      RealType result;
      detail::check_scale(BOOST_CURRENT_FUNCTION, scale, &result);
   }

   RealType location()const
   {
      return m_location;
   }

   RealType scale()const
   {
      return m_scale;
   }
private:
   //
   // Data members:
   //
   RealType m_location;  // distribution location.
   RealType m_scale;     // distribution scale.
};

typedef lognormal_distribution<double> lognormal;

template <class RealType>
const std::pair<RealType, RealType> range(const lognormal_distribution<RealType>& dist)
{ // Range of permissible values for random variable x.
	using boost::math::tools::max_value;
	return const std::pair<RealType, RealType>(0, +max_value());
}

template <class RealType>
const std::pair<RealType, RealType> support(const lognormal_distribution<RealType>& dist)
{ // Range of supported values for random variable x.
	// This is range where cdf rises from 0 to 1, and outside it, the pdf is zero.
	using boost::math::tools::max_value;
	return const std::pair<RealType, RealType>(0,  +max_value());
}


template <class RealType>
RealType pdf(const lognormal_distribution<RealType>& dist, const RealType& x)
{
   using namespace std;  // for ADL of std functions

   RealType mu = dist.location();
   RealType sigma = dist.scale();

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;
   if(0 == detail::check_lognormal_x(BOOST_CURRENT_FUNCTION, x, &result))
      return result;

   if(x == 0)
      return 0;

   RealType exponent = log(x) - mu;
   exponent *= -exponent;
   exponent /= 2 * sigma * sigma;

   result = exp(exponent);
   result /= sigma * sqrt(2 * constants::pi<RealType>()) * x;

   return result;
}

template <class RealType>
inline RealType cdf(const lognormal_distribution<RealType>& dist, const RealType& x)
{
   using namespace std;  // for ADL of std functions

   RealType result;
   if(0 == detail::check_lognormal_x(BOOST_CURRENT_FUNCTION, x, &result))
      return result;

   if(x == 0)
      return 0;

   normal_distribution<RealType> norm(dist.location(), dist.scale());
   return cdf(norm, log(x));
}

template <class RealType>
RealType quantile(const lognormal_distribution<RealType>& dist, const RealType& p)
{
   using namespace std;  // for ADL of std functions

   RealType result;
   if(0 == detail::check_probability(BOOST_CURRENT_FUNCTION, p, &result))
      return result;

   if(p == 0)
      return 0;
   if(p == 1)
      return tools::overflow_error<RealType>(BOOST_CURRENT_FUNCTION, 0);

   normal_distribution<RealType> norm(dist.location(), dist.scale());
   return exp(quantile(norm, p));
}

template <class RealType>
RealType cdf(const complemented2_type<lognormal_distribution<RealType>, RealType>& c)
{
   using namespace std;  // for ADL of std functions

   RealType result;
   if(0 == detail::check_lognormal_x(BOOST_CURRENT_FUNCTION, c.param, &result))
      return result;

   if(c.param == 0)
      return 1;

   normal_distribution<RealType> norm(c.dist.location(), c.dist.scale());
   return cdf(complement(norm, log(c.param)));
}

template <class RealType>
RealType quantile(const complemented2_type<lognormal_distribution<RealType>, RealType>& c)
{
   using namespace std;  // for ADL of std functions

   RealType result;
   if(0 == detail::check_probability(BOOST_CURRENT_FUNCTION, c.param, &result))
      return result;

   if(c.param == 1)
      return 0;
   if(c.param == 0)
      return tools::overflow_error<RealType>(BOOST_CURRENT_FUNCTION, 0);

   normal_distribution<RealType> norm(c.dist.location(), c.dist.scale());
   return exp(quantile(complement(norm, c.param)));
}

template <class RealType>
inline RealType mean(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions

   RealType mu = dist.location();
   RealType sigma = dist.scale();

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;

   return exp(mu + sigma * sigma / 2);
}

template <class RealType>
inline RealType variance(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions

   RealType mu = dist.location();
   RealType sigma = dist.scale();

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;

   return expm1(sigma * sigma) * exp(2 * mu + sigma * sigma);
}

template <class RealType>
inline RealType mode(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions

   RealType mu = dist.location();
   RealType sigma = dist.scale();

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;

   return exp(mu - sigma * sigma);
}

template <class RealType>
inline RealType median(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions
   RealType mu = dist.location();
   return exp(mu); // e^mu
}

template <class RealType>
inline RealType skewness(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions

   //RealType mu = dist.location();
   RealType sigma = dist.scale();

   RealType ss = sigma * sigma;
   RealType ess = exp(ss);

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;

   return (ess + 2) * sqrt(expm1(ss));
}

template <class RealType>
inline RealType kurtosis(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions

   //RealType mu = dist.location();
   RealType sigma = dist.scale();
   RealType ss = sigma * sigma;

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;

   return exp(4 * ss) + 2 * exp(3 * ss) + 3 * exp(2 * ss) - 3;
}

template <class RealType>
inline RealType kurtosis_excess(const lognormal_distribution<RealType>& dist)
{
   using namespace std;  // for ADL of std functions

   // RealType mu = dist.location();
   RealType sigma = dist.scale();
   RealType ss = sigma * sigma;

   RealType result;
   if(0 == detail::check_scale(BOOST_CURRENT_FUNCTION, sigma, &result))
      return result;

   return exp(4 * ss) + 2 * exp(3 * ss) + 3 * exp(2 * ss) - 6;
}

} // namespace math
} // namespace boost

// This include must be at the end, *after* the accessors
// for this distribution have been defined, in order to
// keep compilers that support two-phase lookup happy.
#include <boost/math/distributions/detail/derived_accessors.hpp>

#endif // BOOST_STATS_STUDENTS_T_HPP

