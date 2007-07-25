//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/tools/remez.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/expm1.hpp>

void test_polynomial()
{
   double (*f)(double) = boost::math::expm1;
   std::cout << "Testing expm1 approximation, pinned to origin, abolute error, 6 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx1(f, 6, 0, -1, 1, true, false);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx1.iterate();
      std::cout << approx1.error_term() << " " << approx1.max_error() << " " << approx1.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   std::cout << "Testing expm1 approximation, pinned to origin, relative error, 6 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx2(f, 6, 0, -1, 1, true, true);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx2.iterate();
      std::cout << approx2.error_term() << " " << approx2.max_error() << " " << approx2.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

   f = std::exp;
   std::cout << "Testing exp approximation, not pinned to origin, abolute error, 6 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx3(f, 6, 0, -1, 1, false, false);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx3.iterate();
      std::cout << approx3.error_term() << " " << approx3.max_error() << " " << approx3.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   std::cout << "Testing exp approximation, not pinned to origin, relative error, 6 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx4(f, 6, 0, -1, 1, false, true);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx4.iterate();
      std::cout << approx4.error_term() << " " << approx4.max_error() << " " << approx4.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

   f = std::cos;
   std::cout << "Testing cos approximation, not pinned to origin, abolute error, 5 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx5(f, 5, 0, -1, 1, false, false);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx5.iterate();
      std::cout << approx5.error_term() << " " << approx5.max_error() << " " << approx5.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   std::cout << "Testing cos approximation, not pinned to origin, relative error, 5 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx6(f, 5, 0, -1, 1, false, true);
   for(unsigned i = 0; i < 7; ++i)
   {
      approx6.iterate();
      std::cout << approx6.error_term() << " " << approx6.max_error() << " " << approx6.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

   f = std::sin;
   std::cout << "Testing sin approximation, pinned to origin, abolute error, 4 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx7(f, 4, 0, 0, 1, true, false);
   for(unsigned i = 0; i < 7; ++i)
   {
      approx7.iterate();
      std::cout << approx7.error_term() << " " << approx7.max_error() << " " << approx7.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   std::cout << "Testing sin approximation, pinned to origin, relative error, 4 term polynomial\n";
   boost::math::tools::remez_minimax<double> approx8(f, 4, 0, 0, 1, true, true);
   for(unsigned i = 0; i < 7; ++i)
   {
      approx8.iterate();
      std::cout << approx8.error_term() << " " << approx8.max_error() << " " << approx8.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

void test_rational()
{
   double (*f)(double) = boost::math::expm1;
   std::cout << "Testing expm1 approximation, pinned to origin, abolute error, 3+3 term rational\n";
   boost::math::tools::remez_minimax<double> approx1(f, 3, 3, -1, 1, true, false);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx1.iterate();
      std::cout << approx1.error_term() << " " << approx1.max_error() << " " << approx1.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
#if 0
   //
   // This one causes UBLAS to fail on some systems, so disabled for now.
   //
   std::cout << "Testing expm1 approximation, pinned to origin, relative error, 3+3 term rational\n";
   boost::math::tools::remez_minimax<double> approx2(f, 3, 3, -1, 1, true, true);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx2.iterate();
      std::cout << approx2.error_term() << " " << approx2.max_error() << " " << approx2.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
#endif
   f = std::exp;
   std::cout << "Testing exp approximation, not pinned to origin, abolute error, 3+3 term rational\n";
   boost::math::tools::remez_minimax<double> approx3(f, 3, 3, -1, 1, false, false);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx3.iterate();
      std::cout << approx3.error_term() << " " << approx3.max_error() << " " << approx3.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   std::cout << "Testing exp approximation, not pinned to origin, relative error, 3+3 term rational\n";
   boost::math::tools::remez_minimax<double> approx4(f, 3, 3, -1, 1, false, true);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx4.iterate();
      std::cout << approx4.error_term() << " " << approx4.max_error() << " " << approx4.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

   f = std::cos;
   std::cout << "Testing cos approximation, not pinned to origin, abolute error, 2+2 term rational\n";
   boost::math::tools::remez_minimax<double> approx5(f, 2, 2, 0, 1, false, false);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx5.iterate();
      std::cout << approx5.error_term() << " " << approx5.max_error() << " " << approx5.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
   std::cout << "Testing cos approximation, not pinned to origin, relative error, 2+2 term rational\n";
   boost::math::tools::remez_minimax<double> approx6(f, 2, 2, 0, 1, false, true);
   std::cout << "Interpolation Error: " << approx1.max_error() << std::endl;
   for(unsigned i = 0; i < 7; ++i)
   {
      approx6.iterate();
      std::cout << approx6.error_term() << " " << approx6.max_error() << " " << approx6.max_change() << std::endl;
   }
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

int test_main(int, char* [])
{
   test_polynomial();
   test_rational();
   return 0;
}


