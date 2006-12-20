// Copyright John Maddock 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_DISTRIBUTIONS_CHI_SQUARED_HPP
#define BOOST_MATH_DISTRIBUTIONS_CHI_SQUARED_HPP

#include <boost/math/special_functions/gamma.hpp> // for incomplete beta.
#include <boost/math/distributions/complement.hpp> // complements
#include <boost/math/distributions/detail/common_error_handling.hpp> // error checks
#include <boost/math/special_functions/fpclassify.hpp>

namespace boost{ namespace math{

template <class RealType = double>
class chi_squared_distribution
{
public:
   typedef RealType value_type;

   chi_squared_distribution(RealType i) : m_df(i)
   {
      RealType result;
      detail::check_df(
         BOOST_CURRENT_FUNCTION, m_df, &result);
   } // chi_squared_distribution

   RealType degrees_of_freedom()const
   {
      return m_df;
   }

   // Parameter estimation:
   static RealType estimate_degrees_of_freedom(
      RealType difference_from_variance,
      RealType alpha,
      RealType beta,
      RealType variance,
      RealType hint = 100);

private:
   //
   // Data members:
   //
   RealType m_df;  // degrees of freedom are a real number.
};

typedef chi_squared_distribution<double> chi_squared;

template <class RealType>
RealType pdf(const chi_squared_distribution<RealType>& dist, const RealType& chi_square)
{
   using namespace std;  // for ADL of std functions
   RealType degrees_of_freedom = dist.degrees_of_freedom();
   // Error check:
   RealType error_result;
   if(false == detail::check_df(
         BOOST_CURRENT_FUNCTION, degrees_of_freedom, &error_result))
      return error_result;

   if((chi_square < 0) || !(boost::math::isfinite)(chi_square))
   {
      return tools::domain_error<RealType>(
         BOOST_CURRENT_FUNCTION, "Chi Square parameter was %1%, but must be > 0 !", chi_square);
   }

   if(chi_square == 0)
   {
      // Handle special cases:
      if(degrees_of_freedom < 2)
      {
         return tools::overflow_error<RealType>(
            BOOST_CURRENT_FUNCTION, 0);
      }
      else if(degrees_of_freedom == 2)
      {
         return 0.5f;
      }
      else
      {
         return 0;
      }
   }

   return gamma_P_derivative(degrees_of_freedom / 2, chi_square / 2) / 2;
} // pdf

template <class RealType>
RealType cdf(const chi_squared_distribution<RealType>& dist, const RealType& chi_square)
{
   RealType degrees_of_freedom = dist.degrees_of_freedom();
   // Error check:
   RealType error_result;
   if(false == detail::check_df(
         BOOST_CURRENT_FUNCTION, degrees_of_freedom, &error_result))
      return error_result;

   if((chi_square < 0) || !(boost::math::isfinite)(chi_square))
   {
      return tools::domain_error<RealType>(
         BOOST_CURRENT_FUNCTION, "Chi Square parameter was %1%, but must be > 0 !", chi_square);
   }

   return boost::math::gamma_P(degrees_of_freedom / 2, chi_square / 2);
} // cdf

template <class RealType>
RealType quantile(const chi_squared_distribution<RealType>& dist, const RealType& p)
{
   RealType degrees_of_freedom = dist.degrees_of_freedom();
   // Error check:
   RealType error_result;
   if(false == detail::check_df(
         BOOST_CURRENT_FUNCTION, degrees_of_freedom, &error_result)
         && detail::check_probability(
            BOOST_CURRENT_FUNCTION, p, &error_result))
      return error_result;

   return 2 * boost::math::gamma_P_inv(degrees_of_freedom / 2, p);
} // quantile

template <class RealType>
RealType cdf(const complemented2_type<chi_squared_distribution<RealType>, RealType>& c)
{
   RealType const& degrees_of_freedom = c.dist.degrees_of_freedom();
   RealType const& chi_square = c.param;
   // Error check:
   RealType error_result;
   if(false == detail::check_df(
         BOOST_CURRENT_FUNCTION, degrees_of_freedom, &error_result))
      return error_result;

   if((chi_square < 0) || !(boost::math::isfinite)(chi_square))
   {
      return tools::domain_error<RealType>(
         BOOST_CURRENT_FUNCTION, "Chi Square parameter was %1%, but must be > 0 !", chi_square);
   }

   return boost::math::gamma_Q(degrees_of_freedom / 2, chi_square / 2);
}

template <class RealType>
RealType quantile(const complemented2_type<chi_squared_distribution<RealType>, RealType>& c)
{
   RealType const& degrees_of_freedom = c.dist.degrees_of_freedom();
   RealType const& q = c.param;
   // Error check:
   RealType error_result;
   if(false == detail::check_df(
         BOOST_CURRENT_FUNCTION, degrees_of_freedom, &error_result)
         && detail::check_probability(
            BOOST_CURRENT_FUNCTION, q, &error_result))
      return error_result;

   return 2 * boost::math::gamma_Q_inv(degrees_of_freedom / 2, q);
}

template <class RealType>
inline RealType mean(const chi_squared_distribution<RealType>& dist)
{ // Mean of Chi-Squared distribution = v.
  return dist.degrees_of_freedom();
} // mean

template <class RealType>
inline RealType variance(const chi_squared_distribution<RealType>& dist)
{ // Variance of Chi-Squared distribution = 2v.
  return 2 * dist.degrees_of_freedom();
} // variance

template <class RealType>
inline RealType mode(const chi_squared_distribution<RealType>& dist)
{
   RealType df = dist.degrees_of_freedom();
   if(df <= 2)
      return tools::domain_error<RealType>(
         BOOST_CURRENT_FUNCTION,
         "The Chi-Squared distribution only has a mode for degrees of freedom >= 2, but got degrees of freedom = %1%.",
         df);
   return df - 2;
}

template <class RealType>
inline RealType median(const chi_squared_distribution<RealType>& dist)
{ // Median is given by Quantile[dist, 1/2]
   RealType df = dist.degrees_of_freedom();
   if(df <= 1)
      return tools::domain_error<RealType>(
         BOOST_CURRENT_FUNCTION,
         "The Chi-Squared distribution only has a mode for degrees of freedom >= 2, but got degrees of freedom = %1%.",
         df);
   return df - RealType(2)/3;
}

template <class RealType>
inline RealType skewness(const chi_squared_distribution<RealType>& dist)
{
   using namespace std; // For ADL
   RealType df = dist.degrees_of_freedom();
   return sqrt (8 / df);  // == 2 * sqrt(2 / df);
}

template <class RealType>
inline RealType kurtosis(const chi_squared_distribution<RealType>& dist)
{
   RealType df = dist.degrees_of_freedom();
   return 3 + 12 / df;
}

template <class RealType>
inline RealType kurtosis_excess(const chi_squared_distribution<RealType>& dist)
{
   RealType df = dist.degrees_of_freedom();
   return 12 / df;
}

//
// Parameter estimation comes last:
//
namespace detail{

template <class RealType>
struct df_estimator
{
   df_estimator(RealType a, RealType b, RealType variance, RealType delta)
      : alpha(a), beta(b), ratio(delta/variance) {}

   RealType operator()(const RealType& df)
   {
      if(df <= tools::min_value<RealType>())
         return 1;
      chi_squared_distribution<RealType> cs(df);

      RealType result;
      if(ratio > 0)
      {
         RealType r = 1 + ratio;
         result = cdf(cs, quantile(complement(cs, alpha)) / r) - beta;
      }
      else
      {
         RealType r = 1 + ratio;
         result = cdf(complement(cs, quantile(cs, alpha) / r)) - beta;
      }
      return result;
   }
private:
   RealType alpha, beta, ratio;
};

}

template <class RealType>
RealType chi_squared_distribution<RealType>::estimate_degrees_of_freedom(
   RealType difference_from_variance,
   RealType alpha,
   RealType beta,
   RealType variance,
   RealType hint)
{
   //
   // Check for domain errors:
   //
   RealType error_result;
   if(false == detail::check_probability(
         BOOST_CURRENT_FUNCTION, alpha, &error_result)
         && detail::check_probability(BOOST_CURRENT_FUNCTION, beta, &error_result))
      return error_result;

   if(hint <= 0)
      hint = 1;

   detail::df_estimator<RealType> f(alpha, beta, variance, difference_from_variance);
   tools::eps_tolerance<RealType> tol(tools::digits<RealType>());
   boost::uintmax_t max_iter = 10000;
   std::pair<RealType, RealType> r = tools::bracket_and_solve_root(f, hint, RealType(2), false, tol, max_iter);
   RealType result = r.first + (r.second - r.first) / 2;
   if(max_iter == 10000)
   {
      tools::logic_error<RealType>(BOOST_CURRENT_FUNCTION, "Unable to locate solution in a reasonable time:"
         " either there is no answer to how many degrees of freedom are required"
         " or the answer is infinite.  Current best guess is %1%", result);
   }
   return result;
}



} // namespace math
} // namespace boost

// This include must be at the end, *after* the accessors
// for this distribution have been defined, in order to
// keep compilers that support two-phase lookup happy.
#include <boost/math/distributions/detail/derived_accessors.hpp>

#endif // BOOST_MATH_DISTRIBUTIONS_CHI_SQUARED_HPP
