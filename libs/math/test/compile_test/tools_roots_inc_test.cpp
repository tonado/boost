//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/tools/roots.hpp>
// #includes all the files that it needs to.
//
#include <boost/math/tools/roots.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

inline void check_result_imp(std::pair<float, float>, std::pair<float, float>){}
inline void check_result_imp(std::pair<double, double>, std::pair<double, double>){}
inline void check_result_imp(std::pair<long double, long double>, std::pair<long double, long double>){}

void check()
{
   typedef double (*F)(double);
   typedef std::pair<double, double> (*F2)(double);
   typedef std::tr1::tuple<double, double, double> (*F3)(double);
   typedef boost::math::tools::eps_tolerance<double> Tol;
   Tol tol(u);
   boost::uintmax_t max_iter = 0;
   F f = 0;
   F2 f2 = 0;
   F3 f3 = 0;

   check_result<std::pair<double, double> >(boost::math::tools::bisect<F, double, Tol>(f, d, d, tol, max_iter));
   check_result<std::pair<double, double> >(boost::math::tools::bisect<F, double, Tol>(f, d, d, tol));
   check_result<double>(boost::math::tools::newton_raphson_iterate<F2, double>(f2, d, d, d, i, max_iter));
   check_result<double>(boost::math::tools::halley_iterate<F3, double>(f3, d, d, d, i, max_iter));
   check_result<double>(boost::math::tools::schroeder_iterate<F3, double>(f3, d, d, d, i, max_iter));
}

