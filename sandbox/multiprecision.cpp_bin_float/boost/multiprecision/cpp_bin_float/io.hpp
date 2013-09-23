///////////////////////////////////////////////////////////////
//  Copyright 2013 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_

#ifndef BOOST_MP_CPP_BIN_FLOAT_IO_HPP
#define BOOST_MP_CPP_BIN_FLOAT_IO_HPP

namespace boost{ namespace multiprecision{ namespace cpp_bf_io_detail{

//
// Multiplies a by b and shifts the result so it fits inside max_bits bits, 
// returns by how much the result was shifted.
//
inline int restricted_multiply(cpp_int& result, const cpp_int& a, const cpp_int& b, int max_bits, boost::int64_t& error)
{
   result = a * b;
   int gb = msb(result);
   int rshift = 0;
   if(gb > max_bits)
   {
      rshift = gb - max_bits;
      int lb = lsb(result);
      int roundup = 0;
      // The error rate increases by the error of both a and b, 
      // this may be overly pessimistic in many case as we're assuming
      // that a and b have the same level of uncertainty...
      if(lb < rshift)
         error = error ? error * 2 : 1;
      if(bit_test(result, rshift - 1))
      {
         if(lb == rshift - 1)
            roundup = 1;
         else
            roundup = 2;
      }
      result >>= rshift;
      if((roundup == 2) || ((roundup == 1) && (result.backend().limbs()[0] & 1)))
         ++result;
   }
   return rshift;
}
//
// Computes a^e shifted to the right so it fits in max_bits, returns how far
// to the right we are shifted.
//
inline int restricted_pow(cpp_int& result, const cpp_int& a, int e, int max_bits, boost::int64_t& error)
{
   BOOST_ASSERT(&result != &a);
   int exp = 0;
   if(e == 1)
   {
      result = a;
      return exp;
   }
   else if(e == 2)
   {
      return restricted_multiply(result, a, a, max_bits, error);
   }
   else if(e == 3)
   {
      exp = restricted_multiply(result, a, a, max_bits, error);
      exp += restricted_multiply(result, result, a, max_bits, error);
      return exp;
   }
   int p = e / 2;
   exp = restricted_pow(result, a, p, max_bits, error);
   exp *= 2;
   exp += restricted_multiply(result, result, result, max_bits, error);
   if(e & 1)
      exp += restricted_multiply(result, result, a, max_bits, error);
   return exp;
}

inline int get_round_mode(const cpp_int& what, int location, boost::int64_t error)
{
   //
   // Can we round what at /location/, if the error in what is /error/ in
   // units of 0.5ulp.  Return:
   //
   // -1: Can't round.
   //  0: leave as is.
   //  1: tie.
   //  2: round up.
   //
   boost::int64_t error_radius = error & 1 ? (1 + error) / 2 : error / 2;
   if(error_radius && ((int)msb(error_radius) >= location))
      return -1;
   if(bit_test(what, location))
   {
      if((int)lsb(what) == location)
         return error ? -1 : 1;   // Either a tie or can't round depending on whether we have any error
      if(!error)
         return 2;  // no error, round up.
      cpp_int t = what - error_radius;
      if((int)lsb(t) >= location)
         return -1;
      return 2;
   }
   else if(error)
   {
      cpp_int t = what + error_radius;
      return bit_test(t, location) ? -1 : 0;
   }
   return 0;
}

inline int get_round_mode(cpp_int& r, cpp_int& d, boost::int64_t error, const cpp_int& q)
{
   //
   // Lets suppose we have an inexact division by d+delta, where the true
   // value for the divisor is d, and with |delta| <= error/2, then
   // we have calculated q and r such that:
   //
   // n                  r
   // ---       = q + -----------
   // d + error        d + error
   //
   // Rearranging for n / d we get:
   //
   //    n         delta*q + r
   //   --- = q + -------------
   //    d              d
   //
   // So rounding depends on whether 2r + error * q > d.
   //
   // We return:
   //  0 = down down.
   //  1 = tie.
   //  2 = round up.
   // -1 = couldn't decide.
   //
   r <<= 1;
   int c = r.compare(d);
   if(c == 0)
      return error ? -1 : 1;
   if(c > 0)
   {
      if(error)
      {
         r -= error * q;
         return r.compare(d) > 0 ? 2 : -1;
      }
      return 2;
   }
   if(error)
   {
      r += error * q;
      return r.compare(d) < 0 ? 0 : -1;
   }
   return 0;
}

} // namespace

namespace backends{

template <unsigned Bits>
cpp_bin_float<Bits>& cpp_bin_float<Bits>::operator=(const char *s)
{
   cpp_int n;
   int decimal_exp = 0;
   int digits_seen = 0;
   static const int max_digits_seen = 4 + (Bits * 301L) / 1000;
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
      if(digits_seen || (*s != '0'))
         ++digits_seen;
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
      if(digits_seen || (*s != '0'))
         ++digits_seen;
      ++s;
      if(digits_seen > max_digits_seen)
         break;
   }
   //
   // Digits we're skipping:
   //
   while(*s && (*s >= '0') && (*s <= '9'))
      ++s;
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

   static const unsigned limb_bits = sizeof(limb_type) * CHAR_BIT;
   //
   // Set our working precision - this is heuristic based, we want
   // a value as small as possible > Bits to avoid large computations
   // and excessive memory usage, but we also want to avoid having to
   // up the computation and start again at a higher precision.
   // So we round Bits up to the nearest whole number of limbs, and add
   // one limb for good measure.  This works very well for small exponents,
   // but for larger exponents we may may need to restart, we could add some
   // extra precision right from the start for larger exponents, but this
   // seems to be slightly slower in the *average* case:
   //
#ifdef BOOST_MP_STRESS_IO
   int max_bits = Bits + 32;
#else
   int max_bits = Bits + (Bits % limb_bits ? limb_bits - Bits % limb_bits : 0) + limb_bits;
#endif
   boost::int64_t error = 0;
   int calc_exp = 0;

   if(decimal_exp >= 0)
   {
      // Nice and simple, the result is an integer...
      do
      {
         cpp_int t;
         if(decimal_exp)
         {
            calc_exp = boost::multiprecision::cpp_bf_io_detail::restricted_pow(t, cpp_int(5), decimal_exp, max_bits, error);
            calc_exp += boost::multiprecision::cpp_bf_io_detail::restricted_multiply(t, t, n, max_bits, error);
         }
         exponent() = (int)Bits - 1;
         exponent() += decimal_exp;
         exponent() += calc_exp;
         int rshift = msb(t) - Bits + 1;
         if(rshift > 0)
         {
            exponent() += rshift;
            int roundup = boost::multiprecision::cpp_bf_io_detail::get_round_mode(t, rshift - 1, error);
            t >>= rshift;
            if((roundup == 2) || ((roundup == 1) && t.backend().limbs()[0] & 1))
               ++t;
            else if(roundup < 0)
            {
#ifdef BOOST_MP_STRESS_IO
               max_bits += 32;
#else
               max_bits *= 2;
#endif
               error = 0;
               continue;
            }
         }
         else
         {
            BOOST_ASSERT(!error);
         }
         copy_and_round(*this, t.backend());
         break;
      }
      while(true);

      if(ss != sign())
         negate();
   }
   else
   {
      // Result is the ratio of two integers: we need to organise the
      // division so as to produce at least an N-bit result which we can
      // round according to the remainder.
      //cpp_int d = pow(cpp_int(5), -decimal_exp);
      do
      {
         cpp_int d;
         calc_exp = boost::multiprecision::cpp_bf_io_detail::restricted_pow(d, cpp_int(5), -decimal_exp, max_bits, error);
         int shift = (int)Bits - msb(n) + msb(d);
         exponent() = Bits - 1 + decimal_exp - calc_exp;
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
         int roundup = 0;
         if(gb == Bits - 1)
         {
            // Exactly the right number of bits, use the remainder to round:
            roundup = boost::multiprecision::cpp_bf_io_detail::get_round_mode(r, d, error, q);
         }
         else if(bit_test(q, gb - (int)Bits) && ((int)lsb(q) == (gb - (int)Bits)))
         {
            // Too many bits in q and the bits in q indicate a tie, but we can break that using r,
            // note that the radius of error in r is error/2 * q:
            int shift = gb - (int)Bits + 1;
            q >>= shift;
            exponent() += shift;
            BOOST_ASSERT(msb(q) >= Bits - 1);
            if(error && (r < (error / 2) * q))
               roundup = -1;
            else if(error && (r + (error / 2) * q >= d))
               roundup = -1;
            else
               roundup = r ? 2 : 1;
         }
         else if(error && (((error / 2) * q + r >= d) || (r < (error / 2) * q)))
         {
            // We might have been rounding up, or got the wrong quotient: can't tell!
            roundup = -1;
         }
         if(roundup < 0)
         {
#ifdef BOOST_MP_STRESS_IO
            max_bits += 32;
#else
            max_bits *= 2;
#endif
            error = 0;
            if(shift > 0)
            {
               n >>= shift;
               exponent() += shift;
            }
            continue;
         }
         else if((roundup == 2) || ((roundup == 1) && q.backend().limbs()[0] & 1))
            ++q;
         copy_and_round(*this, q.backend());
         if(ss != sign())
            negate();
         break;
      }
      while(true);
   }

   return *this;
}

template <unsigned Bits>
std::string cpp_bin_float<Bits>::str(std::streamsize dig, std::ios_base::fmtflags f) const
{
   if(dig == 0)
      dig = std::numeric_limits<number<cpp_bin_float<Bits> > >::max_digits10;

   bool scientific = (f & std::ios_base::scientific) == std::ios_base::scientific;
   bool fixed = !scientific && (f & std::ios_base::fixed);

   std::string s;

   if(exponent() <= cpp_bin_float<Bits>::max_exponent)
   {
      // How far to left-shift in order to demormalise the mantissa:
      int shift = (int)Bits - exponent() - 1;
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
         s = "0";
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
      int roundup = 0; // 0=no rounding, 1=tie, 2=up
      static const unsigned limb_bits = sizeof(limb_type) * CHAR_BIT;
      //
      // Set our working precision - this is heuristic based, we want
      // a value as small as possible > Bits to avoid large computations
      // and excessive memory usage, but we also want to avoid having to
      // up the computation and start again at a higher precision.
      // So we round Bits up to the nearest whole number of limbs, and add
      // one limb for good measure.  This works very well for small exponents,
      // but for larger exponents we add a few extra limbs to max_bits:
      //
#ifdef BOOST_MP_STRESS_IO
      int max_bits = Bits + 32;
#else
      int max_bits = Bits + (Bits % limb_bits ? limb_bits - Bits % limb_bits : 0) + limb_bits;
      if(power10)
         max_bits += (msb(std::abs(power10)) / 8) * limb_bits;
#endif
      do
      {
         boost::int64_t error = 0;
         int calc_exp = 0;
         //
         // Our integer result is: bits() * 2^-shift * 5^power10
         //
         i = bits();
         if(shift < 0)
         {
            if(power10 >= 0)
            {
               // We go straight to the answer with all integer arithmetic,
               // the result is always exact and never needs rounding:
               i <<= -shift;
               if(power10)
                  i *= pow(cpp_int(5), power10);
            }
            else if(power10 < 0)
            {
               cpp_int d;
               calc_exp = boost::multiprecision::cpp_bf_io_detail::restricted_pow(d, cpp_int(5), -power10, max_bits, error);
               shift += calc_exp;
               BOOST_ASSERT(shift < 0); // Must still be true!
               i <<= -shift;
               cpp_int r;
               divide_qr(i, d, i, r);
               roundup = boost::multiprecision::cpp_bf_io_detail::get_round_mode(r, d, error, i);
               if(roundup < 0)
               {
#ifdef BOOST_MP_STRESS_IO
                  max_bits += 32;
#else
                  max_bits *= 2;
#endif
                  shift = (int)Bits - exponent() - 1 - power10;
                  continue;
               }
            }
         }
         else
         {
            //
            // Our integer is bits() * 2^-shift * 10^power10
            //
            if(power10 > 0)
            {
               if(power10)
               {
                  cpp_int t;
                  calc_exp = boost::multiprecision::cpp_bf_io_detail::restricted_pow(t, cpp_int(5), power10, max_bits, error);
                  calc_exp += boost::multiprecision::cpp_bf_io_detail::restricted_multiply(i, i, t, max_bits, error);
                  shift -= calc_exp;
               }
               if((shift < 0) || ((shift == 0) && error))
               {
                  // We only get here if we were asked for a crazy number of decimal digits -
                  // more than are present in a 2^max_bits number.
#ifdef BOOST_MP_STRESS_IO
                  max_bits += 32;
#else
                  max_bits *= 2;
#endif
                  shift = (int)Bits - exponent() - 1 - power10;
                  continue;
               }
               if(shift)
               {
                  roundup = boost::multiprecision::cpp_bf_io_detail::get_round_mode(i, shift - 1, error);
                  if(roundup < 0)
                  {
#ifdef BOOST_MP_STRESS_IO
                     max_bits += 32;
#else
                     max_bits *= 2;
#endif
                     shift = (int)Bits - exponent() - 1 - power10;
                     continue;
                  }
                  i >>= shift;
               }
            }
            else
            {
               // We're right shifting, *and* dividing by 5^-power10,
               // so 5^-power10 can never be that large or we'd simply
               // get zero as a result, and that case is already handled above:
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
      // do it here:
      //
      if((roundup == 2) || ((roundup == 1) && ((s[s.size() - 1] - '0') & 1)))
      {
         boost::multiprecision::detail::round_string_up_at(s, s.size() - 1, base10_exp);
      }

      if(sign())
         s.insert(0, 1, '-');

      boost::multiprecision::detail::format_float_string(s, base10_exp, dig, f, false);
   }
   else
   {
      switch(exponent())
      {
      case exponent_zero:
         s = "0";
         boost::multiprecision::detail::format_float_string(s, 0, dig, f, true);
         break;
      case exponent_nan:
         s = "nan";
         break;
      case exponent_infinity:
         s = sign() ? "-inf" : f & std::ios_base::showpos ? "+inf" : "inf";
         break;
      }
   }
   return s;
}

}}} // namespaces

#endif

