//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/tools/ntl.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

#include <boost/math/tools/test_data.hpp>
#include "ntl_rr_lanczos.hpp"

using namespace boost::math::tools;

//
// Force trunctation to float precision of input values:
// we must ensure that the input values are exactly representable
// in whatever type we are testing, or the output values will all
// be thrown off:
//
float external_f;
float force_truncate(const float* f)
{
   external_f = *f;
   return external_f;
}

float truncate_to_float(NTL::RR r)
{
   float f = boost::math::tools::real_cast<float>(r);
   return force_truncate(&f);
}

struct gamma_inverse_generator_a
{
   std::tr1::tuple<NTL::RR, NTL::RR> operator()(const NTL::RR x, const NTL::RR p)
   {
      NTL::RR x1 = boost::math::gamma_P_inva(x, p);
      NTL::RR x2 = boost::math::gamma_Q_inva(x, p);
      std::cout << "Inverse for " << x << " " << p << std::endl;
      return std::tr1::make_tuple(x1, x2);
   }
};


int test_main(int argc, char*argv [])
{
   NTL::RR::SetPrecision(1000);
   NTL::RR::SetOutputPrecision(100);

   bool cont;
   std::string line;

   parameter_info<NTL::RR> arg1, arg2;
   test_data<NTL::RR> data;

   std::cout << "Welcome.\n"
      "This program will generate spot tests for the inverse incomplete gamma function:\n"
      "  gamma_P_inva(a, p) and gamma_Q_inva(a, q)\n\n";

   arg1 = make_power_param<NTL::RR>(NTL::RR(0), -4, 24);
   arg2 = make_random_param<NTL::RR>(NTL::RR(0), NTL::RR(1), 15);
   data.insert(gamma_inverse_generator_a(), arg1, arg2);
 
   line = "igamma_inva_data.ipp";
   std::ofstream ofs(line.c_str());
   write_code(ofs, data, "igamma_inva_data");
   
   return 0;
}

