///////////////////////////////////////////////////////////////
//  Copyright 2013 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_

#ifndef BOOST_MATH_CPP_BIN_FLOAT_HPP
#define BOOST_MATH_CPP_BIN_FLOAT_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/math/special_functions/trunc.hpp>
#include <boost/multiprecision/detail/float_string_cvt.hpp>

namespace boost{ namespace multiprecision{ namespace backends{

template <unsigned Bits>
class cpp_bin_float
{
public:
   typedef cpp_int_backend<Bits, Bits, unsigned_magnitude, unchecked, void> rep_type;
   typedef cpp_int_backend<2 * Bits, 2 * Bits, unsigned_magnitude, unchecked, void> double_rep_type;

   typedef typename rep_type::signed_types                        signed_types;
   typedef typename rep_type::unsigned_types                      unsigned_types;
   typedef boost::mpl::list<double, long double>                  float_types;
   typedef int                                                    exponent_type;

   static const int max_exponent = boost::integer_traits<int>::const_max - 2 * Bits;
   static const int min_exponent = boost::integer_traits<int>::const_min + 2 * static_cast<int>(Bits);

   static const int exponent_zero = max_exponent + 1;
   static const int exponent_infinity = max_exponent + 2;
   static const int exponent_nan = max_exponent + 3;

private:

   rep_type m_data;
   int m_exponent;
   bool m_sign;
public:
   cpp_bin_float() : m_data(), m_exponent(exponent_nan), m_sign(false) {}

   cpp_bin_float(const cpp_bin_float &o)
      : m_data(o.m_data), m_exponent(o.m_exponent), m_sign(o.m_sign) {}

   template <class Float>
   cpp_bin_float(const Float& f, typename boost::enable_if_c<number_category<Float>::value == number_kind_floating_point>::type const* = 0)
      : m_data(), m_exponent(0), m_sign(false)
   {
      this->assign_float(f);
   }

   cpp_bin_float& operator=(const cpp_bin_float &o)
   {
      m_data = o.m_data;
      m_exponent = o.m_exponent;
      m_sign = o.m_sign;
      return *this;
   }

   template <class Float>
   typename boost::enable_if_c<number_category<Float>::value == number_kind_floating_point, cpp_bin_float&>::type operator=(const Float& f)
   {
      return assign_float(f);
   }

   template <class Float>
   typename boost::enable_if_c<is_floating_point<Float>::value, cpp_bin_float&>::type assign_float(Float f)
   {
      BOOST_MATH_STD_USING
      using default_ops::eval_add;

      switch((boost::math::fpclassify)(f))
      {
      case FP_ZERO:
         m_data = limb_type(0);
         m_sign = false;
         m_exponent = exponent_zero;
         return *this;
      case FP_NAN:
         m_data = limb_type(0);
         m_sign = false;
         m_exponent = exponent_nan;
         return *this;
      case FP_INFINITE:
         m_data = limb_type(0);
         m_sign = false;
         m_exponent = exponent_infinity;
         return *this;
      }
      if(f < 0)
      {
         *this = -f;
         this->negate();
         return *this;
      }

      typedef typename mpl::front<unsigned_types>::type ui_type;
      m_data = static_cast<ui_type>(0u);
      m_sign = false;
      m_exponent = 0;

      static const int bits = sizeof(int) * CHAR_BIT - 1;
      int e;
      f = frexp(f, &e);
      while(f)
      {
         f = ldexp(f, bits);
         e -= bits;
         int ipart = itrunc(f);
         f -= ipart;
         m_exponent += bits;
         eval_add(*this, ipart);
      }
      m_exponent += e;
      return *this;
   }

   template <class Float>
   typename boost::enable_if_c<
      (number_category<Float>::value == number_kind_floating_point) 
         && !is_floating_point<Float>::value
         /*&& (std::numeric_limits<number<Float> >::radix == 2)*/, 
      cpp_bin_float&>::type assign_float(Float f)
   {
      BOOST_MATH_STD_USING
      using default_ops::eval_add;
      using default_ops::eval_get_sign;
      using default_ops::eval_convert_to;
      using default_ops::eval_subtract;

      typedef typename boost::multiprecision::detail::canonical<int, Float>::type f_int_type;
      typedef typename boost::multiprecision::detail::canonical<int, cpp_bin_float>::type bf_int_type;

      switch(eval_fpclassify(f))
      {
      case FP_ZERO:
         m_data = limb_type(0);
         m_sign = false;
         m_exponent = exponent_zero;
         return *this;
      case FP_NAN:
         m_data = limb_type(0);
         m_sign = false;
         m_exponent = exponent_nan;
         return *this;
      case FP_INFINITE:
         m_data = limb_type(0);
         m_sign = false;
         m_exponent = exponent_infinity;
         return *this;
      }
      if(eval_get_sign(f) < 0)
      {
         f.negate();
         *this = f;
         this->negate();
         return *this;
      }

      typedef typename mpl::front<unsigned_types>::type ui_type;
      m_data = static_cast<ui_type>(0u);
      m_sign = false;
      m_exponent = 0;

      static const int bits = sizeof(int) * CHAR_BIT - 1;
      int e;
      eval_frexp(f, f, &e);
      while(eval_get_sign(f) != 0)
      {
         eval_ldexp(f, f, bits);
         e -= bits;
         int ipart;
         eval_convert_to(&ipart, f);
         eval_subtract(f, static_cast<f_int_type>(ipart));
         m_exponent += bits;
         eval_add(*this, static_cast<bf_int_type>(ipart));
      }
      m_exponent += e;
      if(m_exponent > max_exponent)
         m_exponent = exponent_infinity;
      if(m_exponent < min_exponent)
      {
         m_data = limb_type(0u);
         m_exponent = exponent_zero;
         m_sign = false;
      }
      else if(eval_get_sign(m_data) == 0)
      {
         m_exponent = exponent_zero;
         m_sign = false;
      }
      return *this;
   }

   template <class I>
   typename boost::enable_if<is_integral<I>, cpp_bin_float&>::type operator=(const I& i)
   {
      using default_ops::eval_bit_test;
      if(!i)
      {
         m_data = static_cast<limb_type>(0);
         m_exponent = exponent_zero;
         m_sign = false;
      }
      else
      {
         I fi = boost::multiprecision::detail::abs(i);
         typedef typename boost::multiprecision::detail::canonical<I, rep_type>::type ar_type;
         m_data = static_cast<ar_type>(fi);
         unsigned shift = msb(fi);
         m_exponent = shift;
         eval_left_shift(m_data, Bits - shift - 1);
         BOOST_ASSERT(eval_bit_test(m_data, Bits-1));
         m_sign = i < 0;
      }
      return *this;
   }

   cpp_bin_float& operator=(const char *s)
   {
      const char* org_s = s;
      cpp_int n;
      int decimal_exp = 0;
      bool ss = false;
      //
      // Extract the sign:
      //
      if(*s == '-')
      {
         ss = true;
         ++s;
      }
      else if(*s == '+')
         ++s;
      //
      // Special cases first:
      //
      if((std::strcmp(s, "nan") == 0) || (std::strcmp(s, "NaN") == 0) || (std::strcmp(s, "NAN") == 0))
      {
         return *this = std::numeric_limits<number<cpp_bin_float<Bits> > >::quiet_NaN().backend();
      }
      if((std::strcmp(s, "inf") == 0) || (std::strcmp(s, "Inf") == 0) || (std::strcmp(s, "INF") == 0) || (std::strcmp(s, "infinity") == 0) || (std::strcmp(s, "Infinity") == 0) || (std::strcmp(s, "INFINITY") == 0))
      {
         *this = std::numeric_limits<number<cpp_bin_float<Bits> > >::infinity().backend();
         if(ss)
            negate();
         return *this;
      }
      //
      // Digits before the point:
      //
      while(*s && (*s >= '0') && (*s <= '9'))
      {
         n *= 10u;
         n += *s - '0';
         ++s;
      }
      // The decimal point (we really should localise this!!)
      if(*s && (*s == '.'))
         ++s;
      //
      // Digits after the point:
      //
      while(*s && (*s >= '0') && (*s <= '9'))
      {
         n *= 10u;
         n += *s - '0';
         --decimal_exp;
         ++s;
      }
      //
      // See if there's an exponent:
      //
      if(*s && ((*s == 'e') || (*s == 'E')))
      {
         ++s;
         int e = 0;
         int es = false;
         if(*s && (*s == '-'))
         {
            es = true;
            ++s;
         }
         else if(*s && (*s == '+'))
            ++s;
         while(*s && (*s >= '0') && (*s <= '9'))
         {
            e *= 10u;
            e += *s - '0';
            ++s;
         }
         if(es)
            e = -e;
         decimal_exp += e;
      }
      if(*s)
      {
         //
         // Oops unexpected input at the end of the number:
         //
         BOOST_THROW_EXCEPTION(std::runtime_error("Unable to parse string as a valid floating point number."));
      }
      if(n == 0)
      {
         // Result is necessarily zero:
         *this = 0;
         return *this;
      }
      if(decimal_exp > 300)
      {
         //
         // TODO, FIXME, temporary hack!!
         boost::multiprecision::detail::convert_from_string(*this, org_s);
      }
      else if(decimal_exp >= 0)
      {
         // Nice and simple, the result is an integer...
         n *= pow(cpp_int(5), decimal_exp);
         exponent() = (int)Bits - 1;
         exponent() += decimal_exp;
         copy_and_round(*this, n.backend());
         if(ss != sign())
            negate();
      }
      else if(decimal_exp > -300)
      {
         // Result is the ratio of two integers: we need to organise the
         // division so as to produce at least an N-bit result which we can
         // round according to the remainder.
         cpp_int d = pow(cpp_int(5), -decimal_exp);
         int shift = (int)Bits - msb(n) + msb(d);
         exponent() = Bits - 1 + decimal_exp;
         if(shift > 0)
         {
            n <<= shift;
            exponent() -= shift;
         }
         cpp_int q, r;
         divide_qr(n, d, q, r);
         int gb = msb(q);
         BOOST_ASSERT(gb >= Bits - 1);
         //
         // Check for rounding conditions we have to
         // handle ourselves:
         //
         if(gb == Bits - 1)
         {
            // Exactly the right number of bits, use the remainder to round:
            r *= 2;
            int c = r.compare(d);
            if(c == 0)
            {
               // Tie:
               if(q.backend().limbs()[0] & 1)
                  ++q;
            }
            else if(c > 0)
               ++q;
         }
         else if(bit_test(q, gb - (int)Bits) && ((int)lsb(q) == (gb - (int)Bits)))
         {
            // Too many bits in q and the bits in q indicate a tie, but we can break that using r:
            q >>= gb - (int)Bits + 1;
            BOOST_ASSERT(msb(q) >= Bits - 1);
            if(r)
               ++q;
            else if(q.backend().limbs()[0] & 1)
               ++q;
            exponent() += gb - (int)Bits + 1;
         }
         copy_and_round(*this, q.backend());
         if(ss != sign())
            negate();
      }
      else
      {
         // TODO, FIXME, temporary hack!!!
         boost::multiprecision::detail::convert_from_string(*this, org_s);
      }

      return *this;
   }

   void swap(cpp_bin_float &o) BOOST_NOEXCEPT
   {
      m_data.swap(o.m_data);
      std::swap(m_exponent, o.m_exponent);
      std::swap(m_sign, o.m_sign);
   }

   std::string str(std::streamsize dig, std::ios_base::fmtflags f) const
   {
      if(dig == 0)
         dig = std::numeric_limits<number<cpp_bin_float<Bits> > >::max_digits10;

      bool scientific = (f & std::ios_base::scientific) == std::ios_base::scientific;
      bool fixed = !scientific && (f & std::ios_base::fixed);

      if(exponent() <= cpp_bin_float<Bits>::max_exponent)
      {
         // How far to left-shift in order to demormalise the mantissa:
         int shift = (int)Bits - exponent() - 1;
         if(std::abs(exponent()) < 1000)
         {
            //
            // With a smallish exponent we can use exact integer arithmetic
            // to figure out what to print, basically we create an N digit
            // integer plus a remainder:
            //
            int digits_wanted = static_cast<int>(dig);
            int base10_exp = exponent() >= 0 ? static_cast<int>(std::floor(0.30103 * exponent())) : static_cast<int>(std::ceil(0.30103 * exponent()));
            //
            // For fixed formatting we want /dig/ digits after the decimal point,
            // so if the exponent is zero, allowing for the one digit before the
            // decimal point, we want 1 + dig digits etc.
            //
            if(fixed)
               digits_wanted += 1 + base10_exp;
            if(scientific)
               digits_wanted += 1;
            if(digits_wanted < -1)
            {
               // Fixed precision, no significant digits, and nothing to round!
               std::string s("0");
               if(sign())
                  s.insert(0, 1, '-');
               boost::multiprecision::detail::format_float_string(s, base10_exp, dig, f, true);
               return s;
            }
            //
            // power10 is the base10 exponent we need to multiply/divide by in order
            // to convert our denormalised number to an integer with the right number of digits:
            //
            int power10 = digits_wanted - base10_exp - 1;
            //
            // If we calculate 5^power10 rather than 10^power10 we need to move
            // 2^power10 into /shift/
            //
            shift -= power10;
            cpp_int i;
            std::string s;
            int roundup = 0; // 0=no rounding, 1=tie, 2=up
            do
            {
               //
               // Our integer is: bits() * 2^-shift * 5^power10
               //
               i = bits();
               if(shift < 0)
               {
                  i <<= -shift;
                  if(power10 > 0)
                     i *= pow(cpp_int(5), power10);
                  else if(power10 < 0)
                  {
                     cpp_int r;
                     cpp_int d = pow(cpp_int(5), -power10);
                     divide_qr(i, d, i, r);
                     r <<= 1;
                     int c = r.compare(d);
                     roundup = c < 0 ? 0 : c == 0 ? 1 : 2;
                  }
               }
               else
               {
                  //
                  // Our integer is bits() * 2^-shift * 10^power10
                  //
                  if(power10 >= 0)
                  {
                     if(power10)
                        i *= pow(cpp_int(5), power10);
                     if(shift && bit_test(i, shift - 1))
                     {
                        if((int)lsb(i) == shift - 1)
                           roundup = 1;
                        else
                           roundup = 2;
                     }
                     i >>= shift;
                  }
                  else
                  {
                     cpp_int r;
                     cpp_int d = pow(cpp_int(5), -power10);
                     d <<= shift;
                     divide_qr(i, d, i, r);
                     r <<= 1;
                     int c = r.compare(d);
                     roundup = c < 0 ? 0 : c == 0 ? 1 : 2;
                  }
               }
               s = i.str(0, std::ios_base::fmtflags(0));
               //
               // Check if we got the right number of digits, this
               // is really a test of whether we calculated the
               // decimal exponent correctly:
               //
               int digits_got = i ? s.size() : 0;
               if(digits_got != digits_wanted)
               {
                  base10_exp += digits_got - digits_wanted;
                  if(fixed)
                     digits_wanted = digits_got;  // strange but true.
                  power10 = digits_wanted - base10_exp - 1;
                  shift = (int)Bits - exponent() - 1 - power10;
                  if(fixed)
                     break;
                  roundup = 0;
               }
               else
                  break;
            }
            while(true);
            //
            // Check whether we need to round up: note that we could equally round up
            // the integer /i/ above, but since we need to perform the rounding *after*
            // the conversion to a string and the digit count check, we might as well
            // do it hear:
            //
            if((roundup == 2) || ((roundup == 1) && ((s[s.size() - 1] - '0') & 1)))
            {
               boost::multiprecision::detail::round_string_up_at(s, s.size() - 1, base10_exp);
            }

            if(sign())
               s.insert(0, 1, '-');

            boost::multiprecision::detail::format_float_string(s, base10_exp, dig, f, false);
            return s;
         }
      }
      // TODO, FIXME, temporary hack!!!
      return boost::multiprecision::detail::convert_to_string(*this, dig, f);
   }

   void negate()
   {
      if((m_exponent != exponent_zero) && (m_exponent != exponent_nan))
         m_sign = !m_sign;
   }

   int compare(const cpp_bin_float &o) const BOOST_NOEXCEPT
   {
      if(m_sign != o.m_sign)
         return m_sign ? -1 : 1;
      int result;
      if(m_exponent != o.m_exponent)
      {
         if(m_exponent == exponent_zero)
            result = -1;
         else if(o.m_exponent == exponent_zero)
            result = 1;
         else 
            result = m_exponent > o.m_exponent ? 1 : -1;
      }
      else
         result = m_data.compare(o.m_data);
      if(m_sign)
         result = -result;
      return result;
   }
   template <class A>
   int compare(const A& o) const BOOST_NOEXCEPT
   {
      cpp_bin_float b;
      b = o;
      return compare(b);
   }

   rep_type& bits() { return m_data; }
   const rep_type& bits()const { return m_data; }
   int& exponent() { return m_exponent; }
   const int& exponent()const { return m_exponent; }
   bool& sign() { return m_sign; }
   const bool& sign()const { return m_sign; }
   void check_invariants()
   {
      using default_ops::eval_bit_test;
      using default_ops::eval_is_zero;
      if((m_exponent <= max_exponent) && (m_exponent >= min_exponent))
      {
         BOOST_ASSERT(eval_bit_test(m_data, Bits - 1));
      }
      else
      {
         BOOST_ASSERT(m_exponent > max_exponent);
         BOOST_ASSERT(m_exponent <= exponent_nan);
         BOOST_ASSERT(eval_is_zero(m_data));
      }
   }
};

template <unsigned bits, class Int>
inline void copy_and_round(cpp_bin_float<bits> &res, Int &arg)
{
   // Precondition: exponent of res must have been set before this function is called
   // as we may need to adjust it based on how many bits in arg are set.
   using default_ops::eval_msb;
   using default_ops::eval_lsb;
   using default_ops::eval_left_shift;
   using default_ops::eval_bit_test;
   using default_ops::eval_right_shift;
   using default_ops::eval_increment;
   using default_ops::eval_get_sign;

   // cancellation may have resulted in arg being all zeros:
   if(eval_get_sign(arg) == 0)
   {
      res.exponent() = cpp_bin_float<bits>::exponent_zero;
      res.sign() = false;
      res.bits() = static_cast<limb_type>(0u);
      return;
   }
   unsigned msb = eval_msb(arg);
   if(bits > msb + 1)
   {
      // Must have had cancellation in subtraction, shift left and copy:
      eval_left_shift(arg, bits - msb - 1);
      res.exponent() -= bits - msb - 1;
   }
   else if(bits < msb + 1)
   {
      // We have more bits than we need, so round as required, 
      // first get the rounding bit:
      bool roundup = eval_bit_test(arg, msb - bits);
      // Then check for a tie:
      if(roundup && (msb - bits == eval_lsb(arg)))
      {
         // Ties round towards even:
         if(!eval_bit_test(arg, msb - bits + 1))
            roundup = false;
      }
      // Shift off the bits we don't need:
      eval_right_shift(arg, msb - bits + 1);
      res.exponent() += msb - bits + 1;
      if(roundup)
      {
         eval_increment(arg);
         if(eval_bit_test(arg, bits))
         {
            // This happens very very rairly:
            eval_right_shift(arg, 1u);
            ++res.exponent();
         }
      }
   }
   BOOST_ASSERT(eval_msb(arg) == bits - 1);
   res.bits() = arg;

   if(res.exponent() > cpp_bin_float<bits>::max_exponent)
   {
      // Overflow:
      res.exponent() = cpp_bin_float<bits>::exponent_infinity;
      res.bits() = static_cast<limb_type>(0u);
   }
   else if(res.exponent() < cpp_bin_float<bits>::min_exponent)
   {
      // Underflow:
      res.exponent() = cpp_bin_float<bits>::exponent_zero;
      res.bits() = static_cast<limb_type>(0u);
      res.sign() = false;
   }
}

template <unsigned bits>
inline void do_eval_add(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a, const cpp_bin_float<bits> &b)
{
   using default_ops::eval_add;
   using default_ops::eval_bit_test;

   typename cpp_bin_float<bits>::double_rep_type dt;

   // Special cases first:
   switch(a.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      res = b;
      if(res.sign())
         res.negate();
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      if(b.exponent() == cpp_bin_float<bits>::exponent_nan)
         res = b;
      else
         res = a;
      return; // ault is still infinite.
   case cpp_bin_float<bits>::exponent_nan:
      res = a;
      return; // ault is still a NaN.
   }
   switch(b.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      res = a;
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      res = b;
      if(res.sign())
         res.negate();
      return; // ault is infinite.
   case cpp_bin_float<bits>::exponent_nan:
      res = b;
      return; // ault is a NaN.
   }
   
   int e_diff = a.exponent() - b.exponent();
   bool s = a.sign();
   if(e_diff >= 0)
   {
      dt = a.bits();
      if(e_diff < bits)
      {
         eval_left_shift(dt, e_diff);
         res.exponent() = a.exponent() - e_diff;
         eval_add(dt, b.bits());
      }
      else
         res.exponent() = a.exponent();
   }
   else
   {
      dt= b.bits();
      if(-e_diff < bits)
      {
         eval_left_shift(dt, -e_diff);
         res.exponent() = b.exponent() + e_diff;
         eval_add(dt, a.bits());
      }
      else
         res.exponent() = b.exponent();
   }
   
   copy_and_round(res, dt);
   res.check_invariants();
   if(res.sign() != s)
      res.negate();
}

template <unsigned bits>
inline void do_eval_subtract(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a, const cpp_bin_float<bits> &b)
{
   using default_ops::eval_subtract;
   using default_ops::eval_bit_test;

   typename cpp_bin_float<bits>::double_rep_type dt;
   
   // Special cases first:
   switch(a.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      if(b.exponent() == cpp_bin_float<bits>::exponent_nan)
         res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
      else
      {
         res = b;
         if(!res.sign())
            res.negate();
      }
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      if((b.exponent() == cpp_bin_float<bits>::exponent_nan) || (b.exponent() == cpp_bin_float<bits>::exponent_infinity))
         res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
      else
         res = a;
      return;
   case cpp_bin_float<bits>::exponent_nan:
      res = a;
      return; // result is still a NaN.
   }
   switch(b.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      res = a;
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      res.exponent() = cpp_bin_float<bits>::exponent_nan;
      res.sign() = false;
      res.bits() = static_cast<limb_type>(0u);
      return; // result is a NaN.
   case cpp_bin_float<bits>::exponent_nan:
      res = b;
      return; // result is still a NaN.
   }

   int e_diff = a.exponent() - b.exponent();
   bool s = a.sign();
   if((e_diff > 0) || ((e_diff == 0) && a.bits().compare(b.bits()) >= 0))
   {
      dt = a.bits();
      if(e_diff < bits)
      {
         eval_left_shift(dt, e_diff);
         res.exponent() = a.exponent() - e_diff;
         eval_subtract(dt, b.bits());
      }
      else
         res.exponent() = a.exponent();
   }
   else
   {
      dt = b.bits();
      if(-e_diff < bits)
      {
         eval_left_shift(dt, -e_diff);
         res.exponent() = b.exponent() + e_diff;
         eval_subtract(dt, a.bits());
      }
      else
         res.exponent() = b.exponent();
      s = !s;
   }
   
   copy_and_round(res, dt);
   if(res.sign() != s)
      res.negate();
   res.check_invariants();
}

template <unsigned bits>
inline void eval_add(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a, const cpp_bin_float<bits> &b)
{
   if(a.sign() == b.sign())
      do_eval_add(res, a, b);
   else
      do_eval_subtract(res, a, b);
}

template <unsigned bits>
inline void eval_add(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a)
{
   return eval_add(res, res, a);
}

template <unsigned bits>
inline void eval_subtract(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a, const cpp_bin_float<bits> &b)
{
   if(a.sign() != b.sign())
      do_eval_add(res, a, b);
   else
      do_eval_subtract(res, a, b);
}

template <unsigned bits>
inline void eval_subtract(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a)
{
   return eval_subtract(res, res, a);
}

template <unsigned bits>
inline void eval_multiply(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a, const cpp_bin_float<bits> &b)
{
   using default_ops::eval_bit_test;
   using default_ops::eval_multiply;

   // Special cases first:
   switch(a.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      if(b.exponent() == cpp_bin_float<bits>::exponent_nan)
         res = b;
      else
         res = a;
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      switch(b.exponent())
      {
      case cpp_bin_float<bits>::exponent_zero:
         res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
         break;
      case cpp_bin_float<bits>::exponent_nan:
         res = b;
         break;
      default:
         res = a;
         break;
      }
      return;
   case cpp_bin_float<bits>::exponent_nan:
      res = a;
      return;
   }
   if(b.exponent() > cpp_bin_float<bits>::max_exponent)
   {
      res = b;
      return;
   }
   if((a.exponent() > 0) && (b.exponent() > 0))
   {
      if(cpp_bin_float<bits>::max_exponent + 2 - a.exponent() < b.exponent())
      {
         // We will certainly overflow:
         res.exponent() = cpp_bin_float<bits>::exponent_infinity;
         res.sign() = a.sign() != b.sign();
         res.bits() = static_cast<limb_type>(0u);
         return;
      }
   }
   if((a.exponent() < 0) && (b.exponent() < 0))
   {
      if(cpp_bin_float<bits>::min_exponent - 2 - a.exponent() > b.exponent())
      {
         // We will certainly underflow:
         res.exponent() = cpp_bin_float<bits>::exponent_zero;
         res.sign() = false;
         res.bits() = static_cast<limb_type>(0u);
         return;
      }
   }

   typename cpp_bin_float<bits>::double_rep_type dt;
   eval_multiply(dt, a.bits(), b.bits());
   res.exponent() = a.exponent() + b.exponent() - bits + 1;
   copy_and_round(res, dt);
   res.check_invariants();
   res.sign() = a.sign() != b.sign();
}

template <unsigned bits>
inline void eval_multiply(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &a)
{
   eval_multiply(res, res, a);
}

template <unsigned bits>
inline void eval_divide(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &u, const cpp_bin_float<bits> &v)
{
   using default_ops::eval_subtract;
   using default_ops::eval_qr;
   using default_ops::eval_bit_test;
   using default_ops::eval_get_sign;

   //
   // Special cases first:
   //
   switch(u.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      switch(v.exponent())
      {
      case cpp_bin_float<bits>::exponent_zero:
      case cpp_bin_float<bits>::exponent_nan:
         res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
         return;
      }
      res = u;
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      switch(v.exponent())
      {
      case cpp_bin_float<bits>::exponent_infinity:
      case cpp_bin_float<bits>::exponent_nan:
         res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
         return;
      }
      res = u;
      return;
   case cpp_bin_float<bits>::exponent_nan:
      res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
      return;
   }
   switch(v.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      {
      bool s = u.sign() != v.sign();
      res = std::numeric_limits<number<cpp_bin_float<bits> > >::infinity().backend();
      res.sign() = s;
      return;
      }
   case cpp_bin_float<bits>::exponent_infinity:
      res.exponent() = cpp_bin_float<bits>::exponent_zero;
      res.bits() = limb_type(0);
      res.sign() = false;
      return;
   case cpp_bin_float<bits>::exponent_nan:
      res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
      return;
   }

   // We can scale u and v so that both are integers, then perform integer
   // division to obtain quotient q and remainder r, such that:
   //
   // q * v + r = u
   //
   // and hense:
   //
   // q + r/v = u/v
   //
   // From this, assuming q has "bits" bits, we only need to determine whether
   // r/v is less than, equal to, or greater than 0.5 to determine rounding - 
   // this we can do with a shift and comparison.
   //
   // We can set the exponent and sign of the result up front:
   //
   res.exponent() = u.exponent() - v.exponent() - 1;
   res.sign() = u.sign() != v.sign();
   //
   // Now get the quotient and remainder:
   //
   typename cpp_bin_float<bits>::double_rep_type t(u.bits()), t2(v.bits()), q, r;
   eval_left_shift(t, bits);
   eval_qr(t, t2, q, r);
   //
   // We now have either "bits" or "bits+1" significant bits in q.
   //
   static const unsigned limb_bits = sizeof(limb_type) * CHAR_BIT;
   if(eval_bit_test(q, bits))
   {
      //
      // OK we have bits+1 bits, so we already have rounding info,
      // we just need to changes things if the last bit is 1 and the
      // remainder is non-zero (ie we do not have a tie).
      //
      BOOST_ASSERT(eval_msb(q) == bits);
      if((q.limbs()[0] & 1u) && eval_get_sign(r))
      {
         eval_left_shift(q, limb_bits);
         q.limbs()[0] = 1;
         res.exponent() -= limb_bits;
      }
   }
   else
   {
      //
      // We have exactly "bits" bits in q.
      // Get rounding info, which we can get by comparing 2r with v.
      // We want to call copy_and_round to handle rounding and general cleanup,
      // so we'll left shift q and add some fake bits on the end to represent
      // how we'll be rounding.
      //
      BOOST_ASSERT(eval_msb(q) == bits - 1);
      eval_left_shift(q, limb_bits);
      res.exponent() -= limb_bits;
      eval_left_shift(r, 1u);
      int c = r.compare(v.bits());
      if(c == 0)
         q.limbs()[0] = static_cast<limb_type>(1u) << (limb_bits - 1);
      else if(c > 0)
         q.limbs()[0] = (static_cast<limb_type>(1u) << (limb_bits - 1)) + static_cast<limb_type>(1u);
   }
   copy_and_round(res, q);
}

template <unsigned bits>
inline void eval_divide(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg)
{
   eval_divide(res, res, arg);
}

template <unsigned bits>
inline void eval_convert_to(long long *res, const cpp_bin_float<bits> &arg)
{
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      *res = 0;
      return;
   case cpp_bin_float<bits>::exponent_nan:
      BOOST_THROW_EXCEPTION(std::runtime_error("Could not convert NaN to integer."));
   case cpp_bin_float<bits>::exponent_infinity:
      *res = (std::numeric_limits<long long>::max)();
      if(arg.sign())
         *res = -*res;
      return;
   }
   typename cpp_bin_float<bits>::rep_type man(arg.bits());
   int shift = bits - 1 - arg.exponent();
   if(shift > bits - 1)
   {
      *res = 0;
      return;
   }
   else if(shift < 0)
   {
      // TODO: what if we have fewer bits than a long long?
      *res = (std::numeric_limits<long long>::max)();
      if(arg.sign())
         *res = -*res;
      return;
   }
   eval_right_shift(man, shift);
   eval_convert_to(res, man);
   if(arg.sign())
      *res = -*res;
}

template <unsigned bits>
inline void eval_convert_to(unsigned long long *res, const cpp_bin_float<bits> &arg)
{
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      *res = 0;
      return;
   case cpp_bin_float<bits>::exponent_nan:
      BOOST_THROW_EXCEPTION(std::runtime_error("Could not convert NaN to integer."));
   case cpp_bin_float<bits>::exponent_infinity:
      *res = (std::numeric_limits<unsigned long long>::max)();
      return;
   }
   typename cpp_bin_float<bits>::rep_type man(arg.bits());
   int shift = bits - 1 - arg.exponent();
   if(shift > bits - 1)
   {
      *res = 0;
      return;
   }
   else if(shift < 0)
   {
      // TODO: what if we have fewer bits than a long long?
      *res = (std::numeric_limits<long long>::max)();
      return;
   }
   eval_right_shift(man, shift);
   eval_convert_to(res, man);
}

template <unsigned bits>
inline void eval_convert_to(long double *res, const cpp_bin_float<bits> &arg)
{
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      *res = 0;
      return;
   case cpp_bin_float<bits>::exponent_nan:
      *res = std::numeric_limits<long double>::quiet_NaN();
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      *res = (std::numeric_limits<long double>::infinity)();
      if(arg.sign())
         *res = -*res;
      return;
   }
   int e = arg.exponent();
   e -= bits - 1;
   eval_convert_to(res, arg.bits());
   *res = std::ldexp(*res, e);
   if(arg.sign())
      *res = -*res;
}

template <unsigned bits>
inline void eval_frexp(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg, int *e)
{
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
   case cpp_bin_float<bits>::exponent_nan:
   case cpp_bin_float<bits>::exponent_infinity:
      *e = 0;
      res = arg;
      return;
   }
   res = arg;
   *e = arg.exponent() + 1;
   res.exponent() = -1;
}

template <unsigned bits>
inline void eval_ldexp(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg, int e)
{
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
   case cpp_bin_float<bits>::exponent_nan:
   case cpp_bin_float<bits>::exponent_infinity:
      res = arg;
      return;
   }
   res = arg;
   res.exponent() += e;
}

/*
* Sign manipulation
*/

template <unsigned bits>
inline void eval_abs(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg)
{
   res = arg;
   res.sign() = false;
}

template <unsigned bits>
inline void eval_fabs(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg)
{
   res = arg;
   res.sign() = false;
}

template <unsigned bits>
inline int eval_fpclassify(const cpp_bin_float<bits> &arg)
{
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
      return FP_ZERO;
   case cpp_bin_float<bits>::exponent_infinity:
      return FP_INFINITE;
   case cpp_bin_float<bits>::exponent_nan:
      return FP_NAN;
   }
   return FP_NORMAL;
}

template <unsigned bits>
inline void eval_sqrt(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg)
{
   using default_ops::eval_integer_sqrt;
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
   case cpp_bin_float<bits>::exponent_nan:
      res = arg;
      return;
   case cpp_bin_float<bits>::exponent_infinity:
      res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
      return;
   }
   if(arg.sign())
   {
      res = std::numeric_limits<number<cpp_bin_float<bits> > >::quiet_NaN().backend();
      return;
   }

   typename cpp_bin_float<bits>::double_rep_type t(arg.bits()), r, s;
   eval_left_shift(t, arg.exponent() & 1 ? bits : bits - 1);
   eval_integer_sqrt(s, r, t);

   if(!eval_bit_test(s, bits))
   {
      // We have exactly the right number of bits in the result, round as required:
      if(s.compare(r) < 0)
      {
         eval_increment(s);
      }
   }
   int ae = arg.exponent();
   res.exponent() = ae / 2;
   if((ae & 1) && (ae < 0))
      --res.exponent();
   copy_and_round(res, s);
}

template <unsigned bits>
inline void eval_floor(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg)
{
   using default_ops::eval_increment;
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
   case cpp_bin_float<bits>::exponent_nan:
   case cpp_bin_float<bits>::exponent_infinity:
      res = arg;
      return;
   }
   int shift = (int)bits - arg.exponent() - 1;
   if((arg.exponent() > cpp_bin_float<bits>::max_exponent) || (shift <= 0))
   {
      // Either arg is already an integer, or a special value:
      res = arg;
      return;
   }
   if(shift >= bits)
   {
      res = arg.sign() ? -1 : 0;
      return;
   }
   bool fractional = (int)eval_lsb(arg.bits()) < shift;
   res = arg;
   eval_right_shift(res.bits(), shift);
   if(fractional && res.sign())
   {
      eval_increment(res.bits());
      if(eval_msb(res.bits()) != bits - 1 - shift)
      {
         // Must have extended result by one bit in the increment:
         --shift;
         ++res.exponent();
      }
   }
   eval_left_shift(res.bits(), shift);
}

template <unsigned bits>
inline void eval_ceil(cpp_bin_float<bits> &res, const cpp_bin_float<bits> &arg)
{
   using default_ops::eval_increment;
   switch(arg.exponent())
   {
   case cpp_bin_float<bits>::exponent_zero:
   case cpp_bin_float<bits>::exponent_nan:
   case cpp_bin_float<bits>::exponent_infinity:
      res = arg;
      return;
   }
   int shift = (int)bits - arg.exponent() - 1;
   if((arg.exponent() > cpp_bin_float<bits>::max_exponent) || (shift <= 0))
   {
      // Either arg is already an integer, or a special value:
      res = arg;
      return;
   }
   if(shift >= bits)
   {
      res = arg.sign() ? 0 : 1;
      return;
   }
   bool fractional = (int)eval_lsb(arg.bits()) < shift;
   res = arg;
   eval_right_shift(res.bits(), shift);
   if(fractional && !res.sign())
   {
      eval_increment(res.bits());
      if(eval_msb(res.bits()) != bits - 1 - shift)
      {
         // Must have extended result by one bit in the increment:
         --shift;
         ++res.exponent();
      }
   }
   eval_left_shift(res.bits(), shift);
}

} // namespace backends

using backends::cpp_bin_float;

template<unsigned bits>
struct number_category<cpp_bin_float<bits> > : public boost::mpl::int_<boost::multiprecision::number_kind_floating_point>{};

typedef number<backends::cpp_bin_float<113>, et_off> bin_float128;

}} // namespaces

namespace std{

//
// numeric_limits [partial] specializations for the types declared in this header:
//
template<unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
class numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >
{
   typedef boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> number_type;
public:
   BOOST_STATIC_CONSTEXPR bool is_specialized = true;
   static number_type (min)()
   {
      initializer.do_nothing();
      static std::pair<bool, number_type> value;
      if(!value.first)
      {
         value.first = true;
         value.second = 1u;
         value.second.backend().exponent() = boost::multiprecision::cpp_bin_float<bits>::min_exponent;
      }
      return value.second;
   }
   static number_type (max)()
   {
      initializer.do_nothing();
      static std::pair<bool, number_type> value;
      if(!value.first)
      {
         value.first = true;
         eval_complement(value.second.backend().bits(), value.second.backend().bits());
         value.second.backend().exponent() = boost::multiprecision::cpp_bin_float<bits>::max_exponent;
      }
      return value.second;
   }
   BOOST_STATIC_CONSTEXPR number_type lowest()
   {
      return -(max)();
   }
   BOOST_STATIC_CONSTEXPR int digits = bits;
   BOOST_STATIC_CONSTEXPR int digits10 = bits * 301 / 1000;
   // Is this really correct???
   BOOST_STATIC_CONSTEXPR int max_digits10 = digits10 + 2;
   BOOST_STATIC_CONSTEXPR bool is_signed = true;
   BOOST_STATIC_CONSTEXPR bool is_integer = false;
   BOOST_STATIC_CONSTEXPR bool is_exact = false;
   BOOST_STATIC_CONSTEXPR int radix = 2;
   static number_type epsilon()
   {
      initializer.do_nothing();
      static std::pair<bool, number_type> value;
      if(!value.first)
      {
         value.first = true;
         value.second = 1;
         value.second = ldexp(value.second, 1 - (int)bits);
      }
      return value.second;
   }
   // What value should this be????
   static number_type round_error()
   {
      // returns 0.5
      initializer.do_nothing();
      static std::pair<bool, number_type> value;
      if(!value.first)
      {
         value.first = true;
         value.second = 1;
         value.second = ldexp(value.second, -1);
      }
      return value.second;
   }
   BOOST_STATIC_CONSTEXPR long min_exponent = boost::multiprecision::cpp_bin_float<bits>::min_exponent;
   BOOST_STATIC_CONSTEXPR long min_exponent10 = (min_exponent / 1000) * 301L;
   BOOST_STATIC_CONSTEXPR long max_exponent = boost::multiprecision::cpp_bin_float<bits>::max_exponent;
   BOOST_STATIC_CONSTEXPR long max_exponent10 = (max_exponent / 1000) * 301L;
   BOOST_STATIC_CONSTEXPR bool has_infinity = true;
   BOOST_STATIC_CONSTEXPR bool has_quiet_NaN = true;
   BOOST_STATIC_CONSTEXPR bool has_signaling_NaN = false;
   BOOST_STATIC_CONSTEXPR float_denorm_style has_denorm = denorm_absent;
   BOOST_STATIC_CONSTEXPR bool has_denorm_loss = false;
   static number_type infinity()
   {
      // returns epsilon/2
      initializer.do_nothing();
      static std::pair<bool, number_type> value;
      if(!value.first)
      {
         value.first = true;
         value.second.backend().exponent() = boost::multiprecision::cpp_bin_float<bits>::exponent_infinity;
      }
      return value.second;
   }
   static number_type quiet_NaN()
   {
      return number_type();
   }
   BOOST_STATIC_CONSTEXPR number_type signaling_NaN()
   {
      return number_type(0);
   }
   BOOST_STATIC_CONSTEXPR number_type denorm_min() { return number_type(0); }
   BOOST_STATIC_CONSTEXPR bool is_iec559 = false;
   BOOST_STATIC_CONSTEXPR bool is_bounded = true;
   BOOST_STATIC_CONSTEXPR bool is_modulo = false;
   BOOST_STATIC_CONSTEXPR bool traps = true;
   BOOST_STATIC_CONSTEXPR bool tinyness_before = false;
   BOOST_STATIC_CONSTEXPR float_round_style round_style = round_to_nearest;
private:
   struct data_initializer
   {
      data_initializer()
      {
         std::numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits> > >::epsilon();
         std::numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits> > >::round_error();
         (std::numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits> > >::min)();
         (std::numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits> > >::max)();
         std::numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits> > >::infinity();
         std::numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits> > >::quiet_NaN();
      }
      void do_nothing()const{}
   };
   static const data_initializer initializer;
};

template<unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
const typename numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::data_initializer numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::initializer;

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION

template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST int numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::digits;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST int numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::digits10;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST int numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::max_digits10;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::is_signed;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::is_integer;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::is_exact;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST int numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::radix;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST long numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::min_exponent;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST long numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::min_exponent10;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST long numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::max_exponent;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST long numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::max_exponent10;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::has_infinity;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::has_quiet_NaN;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::has_signaling_NaN;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST float_denorm_style numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::has_denorm;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::has_denorm_loss;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::is_iec559;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::is_bounded;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::is_modulo;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::traps;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST bool numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::tinyness_before;
template <unsigned bits, boost::multiprecision::expression_template_option ExpressionTemplates>
BOOST_CONSTEXPR_OR_CONST float_round_style numeric_limits<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<bits>, ExpressionTemplates> >::round_style;

#endif

} // namespace std

#endif
