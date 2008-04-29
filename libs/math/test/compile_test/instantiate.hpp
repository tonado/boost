//  Copyright John Maddock 2006.
//  Copyright Paul A. Bristow 2007.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_LIBS_MATH_TEST_INSTANTIATE_HPP
#define BOOST_LIBS_MATH_TEST_INSTANTIATE_HPP

#ifndef BOOST_MATH_ASSERT_UNDEFINED_POLICY
#  define BOOST_MATH_ASSERT_UNDEFINED_POLICY false
#endif

#include <boost/math/distributions.hpp>

#include <boost/math/special_functions.hpp>
#include <boost/math/concepts/distributions.hpp>

#ifndef BOOST_MATH_INSTANTIATE_MINIMUM

typedef boost::math::policies::policy<> test_policy;

namespace test{

BOOST_MATH_DECLARE_SPECIAL_FUNCTIONS(test_policy)

}

namespace dist_test{

BOOST_MATH_DECLARE_DISTRIBUTIONS(double, test_policy)

}
#endif

namespace boost{ namespace math{
//
// The non central beta doesn't define some properties,
// define some stub methods here so that we can concept
// check everything else:
//
template <class T, class Policy>
inline T mean(const non_central_beta_distribution<T, Policy>&){ return 0; }
template <class T, class Policy>
inline T variance(const non_central_beta_distribution<T, Policy>&){ return 0; }
template <class T, class Policy>
inline T skewness(const non_central_beta_distribution<T, Policy>&){ return 0; }
template <class T, class Policy>
inline T kurtosis_excess(const non_central_beta_distribution<T, Policy>&){ return 0; }
template <class T, class Policy>
inline T kurtosis(const non_central_beta_distribution<T, Policy>&){ return 0; }

}} // namespaces

template <class RealType>
void instantiate(RealType)
{
   using namespace boost;
   using namespace boost::math;
   using namespace boost::math::concepts;

   function_requires<DistributionConcept<bernoulli_distribution<RealType> > >();
   function_requires<DistributionConcept<beta_distribution<RealType> > >();
   function_requires<DistributionConcept<binomial_distribution<RealType> > >();
   function_requires<DistributionConcept<cauchy_distribution<RealType> > >();
   function_requires<DistributionConcept<chi_squared_distribution<RealType> > >();
   function_requires<DistributionConcept<exponential_distribution<RealType> > >();
   function_requires<DistributionConcept<extreme_value_distribution<RealType> > >();
   function_requires<DistributionConcept<fisher_f_distribution<RealType> > >();
   function_requires<DistributionConcept<gamma_distribution<RealType> > >();
   function_requires<DistributionConcept<lognormal_distribution<RealType> > >();
   function_requires<DistributionConcept<negative_binomial_distribution<RealType> > >();
   function_requires<DistributionConcept<normal_distribution<RealType> > >();
   function_requires<DistributionConcept<rayleigh_distribution<RealType> > >();
   function_requires<DistributionConcept<pareto_distribution<RealType> > >();
   function_requires<DistributionConcept<poisson_distribution<RealType> > >();
   function_requires<DistributionConcept<students_t_distribution<RealType> > >();
   function_requires<DistributionConcept<triangular_distribution<RealType> > >();
   function_requires<DistributionConcept<uniform_distribution<RealType> > >();
   function_requires<DistributionConcept<weibull_distribution<RealType> > >();
   function_requires<DistributionConcept<non_central_chi_squared_distribution<RealType> > >();
   function_requires<DistributionConcept<non_central_beta_distribution<RealType> > >();
   function_requires<DistributionConcept<non_central_f_distribution<RealType> > >();
   function_requires<DistributionConcept<non_central_t_distribution<RealType> > >();
#ifndef BOOST_MATH_INSTANTIATE_MINIMUM
   function_requires<DistributionConcept<bernoulli_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<beta_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<binomial_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<cauchy_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<chi_squared_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<exponential_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<extreme_value_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<fisher_f_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<gamma_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<lognormal_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<non_central_chi_squared_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<negative_binomial_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<normal_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<rayleigh_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<pareto_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<poisson_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<students_t_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<triangular_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<uniform_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<weibull_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<non_central_chi_squared_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<non_central_beta_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<non_central_f_distribution<RealType, test_policy> > >();
   function_requires<DistributionConcept<non_central_t_distribution<RealType, test_policy> > >();

   function_requires<DistributionConcept<dist_test::bernoulli > >();
   function_requires<DistributionConcept<dist_test::beta > >();
   function_requires<DistributionConcept<dist_test::binomial > >();
   function_requires<DistributionConcept<dist_test::cauchy > >();
   function_requires<DistributionConcept<dist_test::chi_squared > >();
   function_requires<DistributionConcept<dist_test::exponential > >();
   function_requires<DistributionConcept<dist_test::extreme_value > >();
   function_requires<DistributionConcept<dist_test::fisher_f > >();
   function_requires<DistributionConcept<dist_test::gamma > >();
   function_requires<DistributionConcept<dist_test::lognormal > >();
   function_requires<DistributionConcept<dist_test::negative_binomial > >();
   function_requires<DistributionConcept<dist_test::normal > >();
   function_requires<DistributionConcept<dist_test::rayleigh > >();
   function_requires<DistributionConcept<dist_test::pareto > >();
   function_requires<DistributionConcept<dist_test::poisson > >();
   function_requires<DistributionConcept<dist_test::students_t > >();
   function_requires<DistributionConcept<dist_test::triangular > >();
   function_requires<DistributionConcept<dist_test::uniform > >();
   function_requires<DistributionConcept<dist_test::weibull > >();
   function_requires<DistributionConcept<dist_test::non_central_chi_squared > >();
   function_requires<DistributionConcept<dist_test::non_central_beta > >();
   function_requires<DistributionConcept<dist_test::non_central_f > >();
   function_requires<DistributionConcept<dist_test::non_central_t > >();
#endif
   int i;
   RealType v1(0.5), v2(0.5), v3(0.5);
   boost::math::tgamma(v1);
   boost::math::tgamma1pm1(v1);
   boost::math::lgamma(v1);
   boost::math::lgamma(v1, &i);
   boost::math::digamma(v1);
   boost::math::tgamma_ratio(v1, v2);
   boost::math::tgamma_delta_ratio(v1, v2);
   boost::math::factorial<RealType>(i);
   boost::math::unchecked_factorial<RealType>(i);
   i = boost::math::max_factorial<RealType>::value;
   boost::math::double_factorial<RealType>(i);
   boost::math::rising_factorial(v1, i);
   boost::math::falling_factorial(v1, i);
   boost::math::tgamma(v1, v2);
   boost::math::tgamma_lower(v1, v2);
   boost::math::gamma_p(v1, v2);
   boost::math::gamma_q(v1, v2);
   boost::math::gamma_p_inv(v1, v2);
   boost::math::gamma_q_inv(v1, v2);
   boost::math::gamma_p_inva(v1, v2);
   boost::math::gamma_q_inva(v1, v2);
   boost::math::erf(v1);
   boost::math::erfc(v1);
   boost::math::erf_inv(v1);
   boost::math::erfc_inv(v1);
   boost::math::beta(v1, v2);
   boost::math::beta(v1, v2, v3);
   boost::math::betac(v1, v2, v3);
   boost::math::ibeta(v1, v2, v3);
   boost::math::ibetac(v1, v2, v3);
   boost::math::ibeta_inv(v1, v2, v3);
   boost::math::ibetac_inv(v1, v2, v3);
   boost::math::ibeta_inva(v1, v2, v3);
   boost::math::ibetac_inva(v1, v2, v3);
   boost::math::ibeta_invb(v1, v2, v3);
   boost::math::ibetac_invb(v1, v2, v3);
   boost::math::gamma_p_derivative(v2, v3);
   boost::math::ibeta_derivative(v1, v2, v3);
   (boost::math::fpclassify)(v1);
   (boost::math::isfinite)(v1);
   (boost::math::isnormal)(v1);
   (boost::math::isnan)(v1);
   (boost::math::isinf)(v1);
   boost::math::log1p(v1);
   boost::math::expm1(v1);
   boost::math::cbrt(v1);
   boost::math::sqrt1pm1(v1);
   boost::math::powm1(v1, v2);
   boost::math::legendre_p(1, v1);
   boost::math::legendre_p(1, 0, v1);
   boost::math::legendre_q(1, v1);
   boost::math::legendre_next(2, v1, v2, v3);
   boost::math::legendre_next(2, 2, v1, v2, v3);
   boost::math::laguerre(1, v1);
   boost::math::laguerre(2, 1, v1);
   boost::math::laguerre(2u, 1u, v1);
   boost::math::laguerre_next(2, v1, v2, v3);
   boost::math::laguerre_next(2, 1, v1, v2, v3);
   boost::math::hermite(1, v1);
   boost::math::hermite_next(2, v1, v2, v3);
   boost::math::spherical_harmonic_r(2, 1, v1, v2);
   boost::math::spherical_harmonic_i(2, 1, v1, v2);
   boost::math::ellint_1(v1);
   boost::math::ellint_1(v1, v2);
   boost::math::ellint_2(v1);
   boost::math::ellint_2(v1, v2);
   boost::math::ellint_3(v1, v2);
   boost::math::ellint_3(v1, v2, v3);
   boost::math::ellint_rc(v1, v2);
   boost::math::ellint_rd(v1, v2, v3);
   boost::math::ellint_rf(v1, v2, v3);
   boost::math::ellint_rj(v1, v2, v3, v1);
   boost::math::hypot(v1, v2);
   boost::math::sinc_pi(v1);
   boost::math::sinhc_pi(v1);
   boost::math::asinh(v1);
   boost::math::acosh(v1);
   boost::math::atanh(v1);
   boost::math::sin_pi(v1);
   boost::math::cos_pi(v1);
   boost::math::cyl_neumann(v1, v2);
   boost::math::cyl_neumann(i, v2);
   boost::math::cyl_bessel_j(v1, v2);
   boost::math::cyl_bessel_j(i, v2);
   boost::math::cyl_bessel_i(v1, v2);
   boost::math::cyl_bessel_i(i, v2);
   boost::math::cyl_bessel_k(v1, v2);
   boost::math::cyl_bessel_k(i, v2);
   boost::math::sph_bessel(i, v2);
   boost::math::sph_bessel(i, 1);
   boost::math::sph_neumann(i, v2);
   boost::math::sph_neumann(i, i);
   boost::math::expint(v1);
   boost::math::expint(i);
   boost::math::expint(i, v2);
   boost::math::expint(i, i);
   boost::math::zeta(v1);
   boost::math::zeta(i);
   boost::math::trunc(v1);
   boost::math::itrunc(v1);
   boost::math::ltrunc(v1);
   boost::math::round(v1);
   boost::math::iround(v1);
   boost::math::lround(v1);
   boost::math::modf(v1, &v1);
   boost::math::modf(v1, &i);
   long l;
   boost::math::modf(v1, &l);
#ifdef BOOST_HAS_LONG_LONG
   boost::math::lltrunc(v1);
   boost::math::llround(v1);
   boost::long_long_type ll;
   boost::math::modf(v1, &ll);
#endif
   boost::math::pow<2>(v1);
#ifndef BOOST_MATH_INSTANTIATE_MINIMUM
   //
   // All over again, with a policy this time:
   //
   test_policy pol;
   boost::math::tgamma(v1, pol);
   boost::math::tgamma1pm1(v1, pol);
   boost::math::lgamma(v1, pol);
   boost::math::lgamma(v1, &i, pol);
   boost::math::digamma(v1, pol);
   boost::math::tgamma_ratio(v1, v2, pol);
   boost::math::tgamma_delta_ratio(v1, v2, pol);
   boost::math::factorial<RealType>(i, pol);
   boost::math::unchecked_factorial<RealType>(i);
   i = boost::math::max_factorial<RealType>::value;
   boost::math::double_factorial<RealType>(i, pol);
   boost::math::rising_factorial(v1, i, pol);
   boost::math::falling_factorial(v1, i, pol);
   boost::math::tgamma(v1, v2, pol);
   boost::math::tgamma_lower(v1, v2, pol);
   boost::math::gamma_p(v1, v2, pol);
   boost::math::gamma_q(v1, v2, pol);
   boost::math::gamma_p_inv(v1, v2, pol);
   boost::math::gamma_q_inv(v1, v2, pol);
   boost::math::gamma_p_inva(v1, v2, pol);
   boost::math::gamma_q_inva(v1, v2, pol);
   boost::math::erf(v1, pol);
   boost::math::erfc(v1, pol);
   boost::math::erf_inv(v1, pol);
   boost::math::erfc_inv(v1, pol);
   boost::math::beta(v1, v2, pol);
   boost::math::beta(v1, v2, v3, pol);
   boost::math::betac(v1, v2, v3, pol);
   boost::math::ibeta(v1, v2, v3, pol);
   boost::math::ibetac(v1, v2, v3, pol);
   boost::math::ibeta_inv(v1, v2, v3, pol);
   boost::math::ibetac_inv(v1, v2, v3, pol);
   boost::math::ibeta_inva(v1, v2, v3, pol);
   boost::math::ibetac_inva(v1, v2, v3, pol);
   boost::math::ibeta_invb(v1, v2, v3, pol);
   boost::math::ibetac_invb(v1, v2, v3, pol);
   boost::math::gamma_p_derivative(v2, v3, pol);
   boost::math::ibeta_derivative(v1, v2, v3, pol);
   (boost::math::fpclassify)(v1);
   (boost::math::isfinite)(v1);
   (boost::math::isnormal)(v1);
   (boost::math::isnan)(v1);
   (boost::math::isinf)(v1);
   boost::math::log1p(v1, pol);
   boost::math::expm1(v1, pol);
   boost::math::cbrt(v1, pol);
   boost::math::sqrt1pm1(v1, pol);
   boost::math::powm1(v1, v2, pol);
   boost::math::legendre_p(1, v1, pol);
   boost::math::legendre_p(1, 0, v1, pol);
   boost::math::legendre_q(1, v1, pol);
   boost::math::legendre_next(2, v1, v2, v3);
   boost::math::legendre_next(2, 2, v1, v2, v3);
   boost::math::laguerre(1, v1, pol);
   boost::math::laguerre(2, 1, v1, pol);
   boost::math::laguerre_next(2, v1, v2, v3);
   boost::math::laguerre_next(2, 1, v1, v2, v3);
   boost::math::hermite(1, v1, pol);
   boost::math::hermite_next(2, v1, v2, v3);
   boost::math::spherical_harmonic_r(2, 1, v1, v2, pol);
   boost::math::spherical_harmonic_i(2, 1, v1, v2, pol);
   boost::math::ellint_1(v1, pol);
   boost::math::ellint_1(v1, v2, pol);
   boost::math::ellint_2(v1, pol);
   boost::math::ellint_2(v1, v2, pol);
   boost::math::ellint_3(v1, v2, pol);
   boost::math::ellint_3(v1, v2, v3, pol);
   boost::math::ellint_rc(v1, v2, pol);
   boost::math::ellint_rd(v1, v2, v3, pol);
   boost::math::ellint_rf(v1, v2, v3, pol);
   boost::math::ellint_rj(v1, v2, v3, v1, pol);
   boost::math::hypot(v1, v2, pol);
   boost::math::sinc_pi(v1, pol);
   boost::math::sinhc_pi(v1, pol);
   boost::math::asinh(v1, pol);
   boost::math::acosh(v1, pol);
   boost::math::atanh(v1, pol);
   boost::math::sin_pi(v1, pol);
   boost::math::cos_pi(v1, pol);
   boost::math::cyl_neumann(v1, v2, pol);
   boost::math::cyl_neumann(i, v2, pol);
   boost::math::cyl_bessel_j(v1, v2, pol);
   boost::math::cyl_bessel_j(i, v2, pol);
   boost::math::cyl_bessel_i(v1, v2, pol);
   boost::math::cyl_bessel_i(i, v2, pol);
   boost::math::cyl_bessel_k(v1, v2, pol);
   boost::math::cyl_bessel_k(i, v2, pol);
   boost::math::sph_bessel(i, v2, pol);
   boost::math::sph_bessel(i, 1, pol);
   boost::math::sph_neumann(i, v2, pol);
   boost::math::sph_neumann(i, i, pol);
   boost::math::expint(v1, pol);
   boost::math::expint(i, pol);
   boost::math::expint(i, v2, pol);
   boost::math::expint(i, i, pol);
   boost::math::zeta(v1, pol);
   boost::math::zeta(i, pol);
   //
   // These next functions are intended to be found via ADL:
   //
   BOOST_MATH_STD_USING
   trunc(v1, pol);
   itrunc(v1, pol);
   ltrunc(v1, pol);
   round(v1, pol);
   iround(v1, pol);
   lround(v1, pol);
   modf(v1, &v1, pol);
   modf(v1, &i, pol);
   modf(v1, &l, pol);
#ifdef BOOST_HAS_LONG_LONG
   using boost::math::lltrunc;
   using boost::math::llround;
   lltrunc(v1, pol);
   llround(v1, pol);
   modf(v1, &ll, pol);
#endif
   boost::math::pow<2>(v1, pol);
   //
   // All over again with the versions in test::
   //
   test::tgamma(v1);
   test::tgamma1pm1(v1);
   test::lgamma(v1);
   test::lgamma(v1, &i);
   test::digamma(v1);
   test::tgamma_ratio(v1, v2);
   test::tgamma_delta_ratio(v1, v2);
   test::factorial<RealType>(i);
   test::unchecked_factorial<RealType>(i);
   i = test::max_factorial<RealType>::value;
   test::double_factorial<RealType>(i);
   test::rising_factorial(v1, i);
   test::falling_factorial(v1, i);
   test::tgamma(v1, v2);
   test::tgamma_lower(v1, v2);
   test::gamma_p(v1, v2);
   test::gamma_q(v1, v2);
   test::gamma_p_inv(v1, v2);
   test::gamma_q_inv(v1, v2);
   test::gamma_p_inva(v1, v2);
   test::gamma_q_inva(v1, v2);
   test::erf(v1);
   test::erfc(v1);
   test::erf_inv(v1);
   test::erfc_inv(v1);
   test::beta(v1, v2);
   test::beta(v1, v2, v3);
   test::betac(v1, v2, v3);
   test::ibeta(v1, v2, v3);
   test::ibetac(v1, v2, v3);
   test::ibeta_inv(v1, v2, v3);
   test::ibetac_inv(v1, v2, v3);
   test::ibeta_inva(v1, v2, v3);
   test::ibetac_inva(v1, v2, v3);
   test::ibeta_invb(v1, v2, v3);
   test::ibetac_invb(v1, v2, v3);
   test::gamma_p_derivative(v2, v3);
   test::ibeta_derivative(v1, v2, v3);
   (test::fpclassify)(v1);
   (test::isfinite)(v1);
   (test::isnormal)(v1);
   (test::isnan)(v1);
   (test::isinf)(v1);
   test::log1p(v1);
   test::expm1(v1);
   test::cbrt(v1);
   test::sqrt1pm1(v1);
   test::powm1(v1, v2);
   test::legendre_p(1, v1);
   test::legendre_p(1, 0, v1);
   test::legendre_q(1, v1);
   test::legendre_next(2, v1, v2, v3);
   test::legendre_next(2, 2, v1, v2, v3);
   test::laguerre(1, v1);
   test::laguerre(2, 1, v1);
   test::laguerre_next(2, v1, v2, v3);
   test::laguerre_next(2, 1, v1, v2, v3);
   test::hermite(1, v1);
   test::hermite_next(2, v1, v2, v3);
   test::spherical_harmonic_r(2, 1, v1, v2);
   test::spherical_harmonic_i(2, 1, v1, v2);
   test::ellint_1(v1);
   test::ellint_1(v1, v2);
   test::ellint_2(v1);
   test::ellint_2(v1, v2);
   test::ellint_3(v1, v2);
   test::ellint_3(v1, v2, v3);
   test::ellint_rc(v1, v2);
   test::ellint_rd(v1, v2, v3);
   test::ellint_rf(v1, v2, v3);
   test::ellint_rj(v1, v2, v3, v1);
   test::hypot(v1, v2);
   test::sinc_pi(v1);
   test::sinhc_pi(v1);
   test::asinh(v1);
   test::acosh(v1);
   test::atanh(v1);
   test::sin_pi(v1);
   test::cos_pi(v1);
   test::cyl_neumann(v1, v2);
   test::cyl_neumann(i, v2);
   test::cyl_bessel_j(v1, v2);
   test::cyl_bessel_j(i, v2);
   test::cyl_bessel_i(v1, v2);
   test::cyl_bessel_i(i, v2);
   test::cyl_bessel_k(v1, v2);
   test::cyl_bessel_k(i, v2);
   test::sph_bessel(i, v2);
   test::sph_bessel(i, 1);
   test::sph_neumann(i, v2);
   test::sph_neumann(i, i);
   test::expint(v1);
   test::expint(i);
   test::expint(i, v2);
   test::expint(i, i);
   test::zeta(v1);
   test::zeta(i);
   test::trunc(v1);
   test::itrunc(v1);
   test::ltrunc(v1);
   test::round(v1);
   test::iround(v1);
   test::lround(v1);
   test::modf(v1, &v1);
   test::modf(v1, &i);
   test::modf(v1, &l);
#ifdef BOOST_HAS_LONG_LONG
   test::lltrunc(v1);
   test::llround(v1);
   test::modf(v1, &ll);
#endif
   test::pow<2>(v1);
#endif
}

template <class RealType>
void instantiate_mixed(RealType)
{
   using namespace boost;
   using namespace boost::math;
#ifndef BOOST_MATH_INSTANTIATE_MINIMUM
   int i = 1;
   long l = 1;
   short s = 1;
   float fr = 0.5F;
   double dr = 0.5;
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   long double lr = 0.5L;
#else
   double lr = 0.5L;
#endif

   boost::math::tgamma(i);
   boost::math::tgamma1pm1(i);
   boost::math::lgamma(i);
   boost::math::lgamma(i, &i);
   boost::math::digamma(i);
   boost::math::tgamma_ratio(i, l);
   boost::math::tgamma_ratio(fr, lr);
   boost::math::tgamma_delta_ratio(i, s);
   boost::math::tgamma_delta_ratio(fr, lr);
   boost::math::rising_factorial(s, i);
   boost::math::falling_factorial(s, i);
   boost::math::tgamma(i, l);
   boost::math::tgamma(fr, lr);
   boost::math::tgamma_lower(i, s);
   boost::math::tgamma_lower(fr, lr);
   boost::math::gamma_p(i, s);
   boost::math::gamma_p(fr, lr);
   boost::math::gamma_q(i, s);
   boost::math::gamma_q(fr, lr);
   boost::math::gamma_p_inv(i, fr);
   boost::math::gamma_q_inv(s, fr);
   boost::math::gamma_p_inva(i, lr);
   boost::math::gamma_q_inva(i, lr);
   boost::math::erf(i);
   boost::math::erfc(i);
   boost::math::erf_inv(i);
   boost::math::erfc_inv(i);
   boost::math::beta(i, s);
   boost::math::beta(fr, lr);
   boost::math::beta(i, s, l);
   boost::math::beta(fr, dr, lr);
   boost::math::betac(l, i, s);
   boost::math::betac(fr, dr, lr);
   boost::math::ibeta(l, i, s);
   boost::math::ibeta(fr, dr, lr);
   boost::math::ibetac(l, i, s);
   boost::math::ibetac(fr, dr, lr);
   boost::math::ibeta_inv(l, s, i);
   boost::math::ibeta_inv(fr, dr, lr);
   boost::math::ibetac_inv(l, i, s);
   boost::math::ibetac_inv(fr, dr, lr);
   boost::math::ibeta_inva(l, i, s);
   boost::math::ibeta_inva(fr, dr, lr);
   boost::math::ibetac_inva(l, i, s);
   boost::math::ibetac_inva(fr, dr, lr);
   boost::math::ibeta_invb(l, i, s);
   boost::math::ibeta_invb(fr, dr, lr);
   boost::math::ibetac_invb(l, i, s);
   boost::math::ibetac_invb(fr, dr, lr);
   boost::math::gamma_p_derivative(i, l);
   boost::math::gamma_p_derivative(fr, lr);
   boost::math::ibeta_derivative(l, i, s);
   boost::math::ibeta_derivative(fr, dr, lr);
   (boost::math::fpclassify)(i);
   (boost::math::isfinite)(s);
   (boost::math::isnormal)(l);
   (boost::math::isnan)(i);
   (boost::math::isinf)(l);
   boost::math::log1p(i);
   boost::math::expm1(s);
   boost::math::cbrt(l);
   boost::math::sqrt1pm1(s);
   boost::math::powm1(i, s);
   boost::math::powm1(fr, lr);
   //boost::math::legendre_p(1, i);
   boost::math::legendre_p(1, 0, s);
   boost::math::legendre_q(1, i);
   boost::math::laguerre(1, i);
   boost::math::laguerre(2, 1, i);
   boost::math::laguerre(2u, 1u, s);
   boost::math::hermite(1, s);
   boost::math::spherical_harmonic_r(2, 1, s, i);
   boost::math::spherical_harmonic_i(2, 1, fr, lr);
   boost::math::ellint_1(i);
   boost::math::ellint_1(i, s);
   boost::math::ellint_1(fr, lr);
   boost::math::ellint_2(i);
   boost::math::ellint_2(i, l);
   boost::math::ellint_2(fr, lr);
   boost::math::ellint_3(i, l);
   boost::math::ellint_3(fr, lr);
   boost::math::ellint_3(s, l, i);
   boost::math::ellint_3(fr, dr, lr);
   boost::math::ellint_rc(i, s);
   boost::math::ellint_rc(fr, lr);
   boost::math::ellint_rd(s, i, l);
   boost::math::ellint_rd(fr, lr, dr);
   boost::math::ellint_rf(s, l, i);
   boost::math::ellint_rf(fr, dr, lr);
   boost::math::ellint_rj(i, i, s, l);
   boost::math::ellint_rj(i, fr, dr, lr);
   boost::math::hypot(i, s);
   boost::math::hypot(fr, lr);
   boost::math::sinc_pi(i);
   boost::math::sinhc_pi(i);
   boost::math::asinh(s);
   boost::math::acosh(l);
   boost::math::atanh(l);
   boost::math::sin_pi(s);
   boost::math::cos_pi(s);
   boost::math::cyl_neumann(fr, dr);
   boost::math::cyl_neumann(i, s);
   boost::math::cyl_bessel_j(fr, lr);
   boost::math::cyl_bessel_j(i, s);
   boost::math::cyl_bessel_i(fr, lr);
   boost::math::cyl_bessel_i(i, s);
   boost::math::cyl_bessel_k(fr, lr);
   boost::math::cyl_bessel_k(i, s);
   boost::math::sph_bessel(i, fr);
   boost::math::sph_bessel(i, 1);
   boost::math::sph_neumann(i, lr);
   boost::math::sph_neumann(i, i);

   boost::math::policies::policy<> pol;


   boost::math::tgamma(i, pol);
   boost::math::tgamma1pm1(i, pol);
   boost::math::lgamma(i, pol);
   boost::math::lgamma(i, &i, pol);
   boost::math::digamma(i, pol);
   boost::math::tgamma_ratio(i, l, pol);
   boost::math::tgamma_ratio(fr, lr, pol);
   boost::math::tgamma_delta_ratio(i, s, pol);
   boost::math::tgamma_delta_ratio(fr, lr, pol);
   boost::math::rising_factorial(s, i, pol);
   boost::math::falling_factorial(s, i, pol);
   boost::math::tgamma(i, l, pol);
   boost::math::tgamma(fr, lr, pol);
   boost::math::tgamma_lower(i, s, pol);
   boost::math::tgamma_lower(fr, lr, pol);
   boost::math::gamma_p(i, s, pol);
   boost::math::gamma_p(fr, lr, pol);
   boost::math::gamma_q(i, s, pol);
   boost::math::gamma_q(fr, lr, pol);
   boost::math::gamma_p_inv(i, fr, pol);
   boost::math::gamma_q_inv(s, fr, pol);
   boost::math::gamma_p_inva(i, lr, pol);
   boost::math::gamma_q_inva(i, lr, pol);
   boost::math::erf(i, pol);
   boost::math::erfc(i, pol);
   boost::math::erf_inv(i, pol);
   boost::math::erfc_inv(i, pol);
   boost::math::beta(i, s, pol);
   boost::math::beta(fr, lr, pol);
   boost::math::beta(i, s, l, pol);
   boost::math::beta(fr, dr, lr, pol);
   boost::math::betac(l, i, s, pol);
   boost::math::betac(fr, dr, lr, pol);
   boost::math::ibeta(l, i, s, pol);
   boost::math::ibeta(fr, dr, lr, pol);
   boost::math::ibetac(l, i, s, pol);
   boost::math::ibetac(fr, dr, lr, pol);
   boost::math::ibeta_inv(l, s, i, pol);
   boost::math::ibeta_inv(fr, dr, lr, pol);
   boost::math::ibetac_inv(l, i, s, pol);
   boost::math::ibetac_inv(fr, dr, lr, pol);
   boost::math::ibeta_inva(l, i, s, pol);
   boost::math::ibeta_inva(fr, dr, lr, pol);
   boost::math::ibetac_inva(l, i, s, pol);
   boost::math::ibetac_inva(fr, dr, lr, pol);
   boost::math::ibeta_invb(l, i, s, pol);
   boost::math::ibeta_invb(fr, dr, lr, pol);
   boost::math::ibetac_invb(l, i, s, pol);
   boost::math::ibetac_invb(fr, dr, lr, pol);
   boost::math::gamma_p_derivative(i, l, pol);
   boost::math::gamma_p_derivative(fr, lr, pol);
   boost::math::ibeta_derivative(l, i, s, pol);
   boost::math::ibeta_derivative(fr, dr, lr, pol);
   boost::math::log1p(i, pol);
   boost::math::expm1(s, pol);
   boost::math::cbrt(l, pol);
   boost::math::sqrt1pm1(s, pol);
   boost::math::powm1(i, s, pol);
   boost::math::powm1(fr, lr, pol);
   //boost::math::legendre_p(1, i, pol);
   boost::math::legendre_p(1, 0, s, pol);
   boost::math::legendre_q(1, i, pol);
   boost::math::laguerre(1, i, pol);
   boost::math::laguerre(2, 1, i, pol);
   boost::math::laguerre(2u, 1u, s, pol);
   boost::math::hermite(1, s, pol);
   boost::math::spherical_harmonic_r(2, 1, s, i, pol);
   boost::math::spherical_harmonic_i(2, 1, fr, lr, pol);
   boost::math::ellint_1(i, pol);
   boost::math::ellint_1(i, s, pol);
   boost::math::ellint_1(fr, lr, pol);
   boost::math::ellint_2(i, pol);
   boost::math::ellint_2(i, l, pol);
   boost::math::ellint_2(fr, lr, pol);
   boost::math::ellint_3(i, l, pol);
   boost::math::ellint_3(fr, lr, pol);
   boost::math::ellint_3(s, l, i, pol);
   boost::math::ellint_3(fr, dr, lr, pol);
   boost::math::ellint_rc(i, s, pol);
   boost::math::ellint_rc(fr, lr, pol);
   boost::math::ellint_rd(s, i, l, pol);
   boost::math::ellint_rd(fr, lr, dr, pol);
   boost::math::ellint_rf(s, l, i, pol);
   boost::math::ellint_rf(fr, dr, lr, pol);
   boost::math::ellint_rj(i, i, s, l, pol);
   boost::math::ellint_rj(i, fr, dr, lr, pol);
   boost::math::hypot(i, s, pol);
   boost::math::hypot(fr, lr, pol);
   boost::math::sinc_pi(i, pol);
   boost::math::sinhc_pi(i, pol);
   boost::math::asinh(s, pol);
   boost::math::acosh(l, pol);
   boost::math::atanh(l, pol);
   boost::math::sin_pi(s, pol);
   boost::math::cos_pi(s, pol);
   boost::math::cyl_neumann(fr, dr, pol);
   boost::math::cyl_neumann(i, s, pol);
   boost::math::cyl_bessel_j(fr, lr, pol);
   boost::math::cyl_bessel_j(i, s, pol);
   boost::math::cyl_bessel_i(fr, lr, pol);
   boost::math::cyl_bessel_i(i, s, pol);
   boost::math::cyl_bessel_k(fr, lr, pol);
   boost::math::cyl_bessel_k(i, s, pol);
   boost::math::sph_bessel(i, fr, pol);
   boost::math::sph_bessel(i, 1, pol);
   boost::math::sph_neumann(i, lr, pol);
   boost::math::sph_neumann(i, i, pol);


   test::tgamma(i);
   test::tgamma1pm1(i);
   test::lgamma(i);
   test::lgamma(i, &i);
   test::digamma(i);
   test::tgamma_ratio(i, l);
   test::tgamma_ratio(fr, lr);
   test::tgamma_delta_ratio(i, s);
   test::tgamma_delta_ratio(fr, lr);
   test::rising_factorial(s, i);
   test::falling_factorial(s, i);
   test::tgamma(i, l);
   test::tgamma(fr, lr);
   test::tgamma_lower(i, s);
   test::tgamma_lower(fr, lr);
   test::gamma_p(i, s);
   test::gamma_p(fr, lr);
   test::gamma_q(i, s);
   test::gamma_q(fr, lr);
   test::gamma_p_inv(i, fr);
   test::gamma_q_inv(s, fr);
   test::gamma_p_inva(i, lr);
   test::gamma_q_inva(i, lr);
   test::erf(i);
   test::erfc(i);
   test::erf_inv(i);
   test::erfc_inv(i);
   test::beta(i, s);
   test::beta(fr, lr);
   test::beta(i, s, l);
   test::beta(fr, dr, lr);
   test::betac(l, i, s);
   test::betac(fr, dr, lr);
   test::ibeta(l, i, s);
   test::ibeta(fr, dr, lr);
   test::ibetac(l, i, s);
   test::ibetac(fr, dr, lr);
   test::ibeta_inv(l, s, i);
   test::ibeta_inv(fr, dr, lr);
   test::ibetac_inv(l, i, s);
   test::ibetac_inv(fr, dr, lr);
   test::ibeta_inva(l, i, s);
   test::ibeta_inva(fr, dr, lr);
   test::ibetac_inva(l, i, s);
   test::ibetac_inva(fr, dr, lr);
   test::ibeta_invb(l, i, s);
   test::ibeta_invb(fr, dr, lr);
   test::ibetac_invb(l, i, s);
   test::ibetac_invb(fr, dr, lr);
   test::gamma_p_derivative(i, l);
   test::gamma_p_derivative(fr, lr);
   test::ibeta_derivative(l, i, s);
   test::ibeta_derivative(fr, dr, lr);
   (test::fpclassify)(i);
   (test::isfinite)(s);
   (test::isnormal)(l);
   (test::isnan)(i);
   (test::isinf)(l);
   test::log1p(i);
   test::expm1(s);
   test::cbrt(l);
   test::sqrt1pm1(s);
   test::powm1(i, s);
   test::powm1(fr, lr);
   //test::legendre_p(1, i);
   test::legendre_p(1, 0, s);
   test::legendre_q(1, i);
   test::laguerre(1, i);
   test::laguerre(2, 1, i);
   test::laguerre(2u, 1u, s);
   test::hermite(1, s);
   test::spherical_harmonic_r(2, 1, s, i);
   test::spherical_harmonic_i(2, 1, fr, lr);
   test::ellint_1(i);
   test::ellint_1(i, s);
   test::ellint_1(fr, lr);
   test::ellint_2(i);
   test::ellint_2(i, l);
   test::ellint_2(fr, lr);
   test::ellint_3(i, l);
   test::ellint_3(fr, lr);
   test::ellint_3(s, l, i);
   test::ellint_3(fr, dr, lr);
   test::ellint_rc(i, s);
   test::ellint_rc(fr, lr);
   test::ellint_rd(s, i, l);
   test::ellint_rd(fr, lr, dr);
   test::ellint_rf(s, l, i);
   test::ellint_rf(fr, dr, lr);
   test::ellint_rj(i, i, s, l);
   test::ellint_rj(i, fr, dr, lr);
   test::hypot(i, s);
   test::hypot(fr, lr);
   test::sinc_pi(i);
   test::sinhc_pi(i);
   test::asinh(s);
   test::acosh(l);
   test::atanh(l);
   test::sin_pi(s);
   test::cos_pi(s);
   test::cyl_neumann(fr, dr);
   test::cyl_neumann(i, s);
   test::cyl_bessel_j(fr, lr);
   test::cyl_bessel_j(i, s);
   test::cyl_bessel_i(fr, lr);
   test::cyl_bessel_i(i, s);
   test::cyl_bessel_k(fr, lr);
   test::cyl_bessel_k(i, s);
   test::sph_bessel(i, fr);
   test::sph_bessel(i, 1);
   test::sph_neumann(i, lr);
   test::sph_neumann(i, i);
#endif
}


#endif // BOOST_LIBS_MATH_TEST_INSTANTIATE_HPP
