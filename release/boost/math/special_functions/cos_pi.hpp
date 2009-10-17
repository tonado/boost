//  Copyright (c) 2007 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_COS_PI_HPP
#define BOOST_MATH_COS_PI_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/config/no_tr1/cmath.hpp>
#include <boost/math/tools/config.hpp>
#include <boost/math/special_functions/trunc.hpp>
#include <boost/math/tools/promotion.hpp>
#include <boost/math/constants/constants.hpp>

namespace boost{ namespace math{ namespace detail{

template <class T, class Policy>
T cos_pi_imp(T x, const Policy& pol)
{
   BOOST_MATH_STD_USING // ADL of std names
   // cos of pi*x:
   bool invert = false;
   if(x < 0.5)
      return cos(constants::pi<T>() * x);
   if(x < 1)
   {
      x = -x;
   }

   T rem = floor(x);
   if(itrunc(rem, pol) & 1)
      invert = !invert;
   rem = x - rem;
   if(rem > 0.5f)
   {
      rem = 1 - rem;
      invert = !invert;
   }
   if(rem == 0.5f)
      return 0;
   
   rem = cos(constants::pi<T>() * rem);
   return invert ? T(-rem) : rem;
}

} // namespace detail

template <class T, class Policy>
inline typename tools::promote_args<T>::type cos_pi(T x, const Policy& pol)
{
   typedef typename tools::promote_args<T>::type result_type;
   return boost::math::detail::cos_pi_imp<result_type>(x, pol);
}

template <class T>
inline typename tools::promote_args<T>::type cos_pi(T x)
{
   return boost::math::cos_pi(x, policies::policy<>());
}

} // namespace math
} // namespace boost
#endif

