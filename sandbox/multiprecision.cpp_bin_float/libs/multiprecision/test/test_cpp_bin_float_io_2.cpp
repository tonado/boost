// Copyright John Maddock 2013.
// Copyright Paul A. Bristow 2013.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(_MSC_VER) || defined(__MINGW32__)
#  define _SCL_SECURE_NO_WARNINGS
//#  pragma warning (disable : 4389)
#  pragma warning (disable : 4512) // assignment operator could not be generated.
#  pragma warning(disable:4127)
#define BOOST_MP_MIN_EXPONENT_DIGITS 3
// else use BOOST_MP_MIN_EXPONENT_DIGITS default = 2
#endif

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <iomanip>

#include "test.hpp" // JM version of Boost test.

#ifdef BOOST_MSVC

#endif

void print_flags(std::ios_base::fmtflags f)
{
   std::cout << "Formatting flags were: ";
   if(f & std::ios_base::scientific)
      std::cout << "scientific ";
   if(f & std::ios_base::fixed)
      std::cout << "fixed ";
   if(f & std::ios_base::showpoint)
      std::cout << "showpoint ";
   if(f & std::ios_base::showpos)
      std::cout << "showpos ";
   std::cout << std::endl;
}


union dull
{ // Used to enable output of double in hex.
  double d;
  unsigned long long u;
};

double generate_random()
{
  dull z;
  // Generate 52 random-ish significand bits.
    z.u = rand();
    z.u <<= 15;
    z.u += rand();
    z.u <<= 15;
    z.u += rand();
    z.u <<= 15;
    z.u += rand();
    z.u &= 0x000FFFFFFFFFFFFF; // Clear any excess significand bits.

    double d = rand();
    d /=  static_cast<double>(0x7FE);
    d *= 0x7FE + 1;
    unsigned long long randexp = static_cast<unsigned long long>(d);
//    unsigned long long randexp = unsigned long long(((double) rand() / (double) 0x7FE) * 0x7FE + 1);
    // 11 exponent bits, range 1 to 0x7FE, for positive denormalized.
    //randexp = 0x3f2; // to test a particular exponent. 3f3,3f4, 3f5 & 3f6 are wrongly read, covering a tenfold range.
    randexp <<= 52; // Shift up over 52 significand bits.
    randexp &= 0x7FE0000000000000; // Normalized doubles only.
    // cout << hex << randexp << endl; // look random-ish.

    z.u += randexp;
    return z.d;
}// double generate_random()

template <class T>
T generate_random()
{
   typedef typename T::backend_type::exponent_type e_type;
   static boost::random::mt19937 gen;
   T val = gen();
   T prev_val = -1;
   while(val != prev_val)
   {
      val *= (gen.max)();
      prev_val = val;
      val += gen();
   }
   e_type e;
   val = frexp(val, &e);

   static boost::random::uniform_int_distribution<e_type> ui(0, std::numeric_limits<T>::max_exponent);
   return ldexp(val, ui(gen));
}

template <class T>
void test()
{ //! Output using all combinations of format flags and check is same as using double.

   typedef T mp_t;
   boost::array<std::ios_base::fmtflags, 9> formats =
   {{
      std::ios_base::fmtflags(0), // default fmtflags.
      std::ios_base::showpoint,
      std::ios_base::showpos,
      std::ios_base::scientific,
      std::ios_base::scientific|std::ios_base::showpos,
      std::ios_base::scientific|std::ios_base::showpoint,
      std::ios_base::fixed,
      std::ios_base::fixed|std::ios_base::showpoint,
      std::ios_base::fixed|std::ios_base::showpos
   }};

   for(unsigned j = 0; j < 1000; ++j)
   {
     double num = generate_random();
     double val = static_cast<double>(num);
     if((fabs(val) > 1) && (fabs(val) < 1e8))
     {
       unsigned col = 0;
       for(unsigned precision = 1; precision < 14; ++precision)
       {
         for(unsigned i = 0; i < formats.size(); ++i, ++col)
         {
            std::stringstream ss;
            ss.precision(precision);
            ss.flags(formats[i]);
            ss << mp_t(val);

            // Expect to output same string as using double.
            std::stringstream sse;
            sse.precision(precision);
            sse.flags(formats[i]);
            sse << static_cast<double>(val);
            std::string expect = sse.str();

            if(ss.str() != expect)
            {
               print_flags(formats[i]);
               std::cout << "Precision: " << precision << std::endl;
               std::cout << std::scientific << std::setprecision(17) << std::showpoint << "Test val: " << val << std::endl;
               // for scientific, precision is 17 digits *after* decimal point.
               std::cout << "Got:      "  << ss.str() << std::endl;
               std::cout << "Expected: "  << expect << std::endl;
               ++boost::detail::test_errors();
               //mp_t(val).str(precision, formats[i]); // for debugging

               boost::multiprecision::cpp_dec_float_50 f50(val);
               std::cout << "True non-rounded result = " << f50.str(50, formats[i]) << std::endl;

               std::stringstream ss3;
               ss3 << expect;
               double v2;
               ss3 >> v2;
               if(val != v2)
               {
                  std::cout << "Double value did not round trip!!\n";
               }

            }
         }
       }
      }
   }
} // template <class T> void test()

int main()
{
   using namespace boost::multiprecision;

   test<number<cpp_bin_float<53> > >();

   return boost::report_errors();
} // int main()
