/* boost random/subtract_with_carry.hpp header file
 *
 * Copyright Jens Maurer 2002
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 *
 * Revision history
 *  2002-03-02  created
 */

#ifndef BOOST_RANDOM_SUBTRACT_WITH_CARRY_HPP
#define BOOST_RANDOM_SUBTRACT_WITH_CARRY_HPP

#include <iostream>
#include <algorithm>     // std::equal
#include <stdexcept>
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>
#include <boost/random/linear_congruential.hpp>


namespace boost {
namespace random {

// subtract-with-carry generator
// Marsaglia and Zaman

template<class IntType, IntType m, unsigned int s, unsigned int r,
  IntType val>
class subtract_with_carry
{
public:
  typedef IntType result_type;
  BOOST_STATIC_CONSTANT(bool, has_fixed_range = true);
  BOOST_STATIC_CONSTANT(result_type, min_value = 0);
  BOOST_STATIC_CONSTANT(result_type, max_value = m-1);
  BOOST_STATIC_CONSTANT(result_type, modulus = m);
  BOOST_STATIC_CONSTANT(unsigned int, long_lag = r);
  BOOST_STATIC_CONSTANT(unsigned int, short_lag = s);

#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
  BOOST_STATIC_ASSERT(std::numeric_limits<result_type>::is_signed);
  BOOST_STATIC_ASSERT(std::numeric_limits<result_type>::is_integer);
#endif

  subtract_with_carry() { seed(); }
  explicit subtract_with_carry(uint32_t value) { seed(value); }
  template<class Generator>
  explicit subtract_with_carry(Generator & gen) { seed(gen); }
  template<class It> subtract_with_carry(It& first, It last) { seed(first,last); }

  // compiler-generated copy ctor and assignment operator are fine

  void seed(uint32_t value = 19780503u)
  {
    random::linear_congruential<int32_t, 40014, 0, 2147483563, 0> intgen(value);
    seed(intgen);
  }

  // For GCC, moving this function out-of-line prevents inlining, which may
  // reduce overall object code size.  However, MSVC does not grok
  // out-of-line template member functions.
  template<class Generator>
  void seed(Generator & gen)
  {
    // I could have used std::generate_n, but it takes "gen" by value
    for(unsigned int j = 0; j < long_lag; ++j)
      x[j] = gen() % modulus;
    carry = (x[long_lag-1] == 0);
    k = 0;
  }

  template<class It>
  void seed(It& first, It last)
  {
    unsigned int j;
    for(j = 0; j < long_lag && first != last; ++j, ++first)
      x[j] = *first % modulus;
    if(first == last && j < long_lag)
      throw std::invalid_argument("subtract_with_carry::seed");
    carry = (x[long_lag-1] == 0);
    k = 0;
   }

  result_type min() const { return min_value; }
  result_type max() const { return max_value; }

  result_type operator()()
  {
    int short_index = k - short_lag;
    if(short_index < 0)
      short_index += long_lag;
    IntType delta = x[short_index] - x[k] - carry;
    if(delta < 0) {
      delta += modulus;
      carry = 1;
    } else {
      carry = 0;
    }
    x[k] = delta;
    ++k;
    if(k >= long_lag)
      k = 0;
    return delta;
  }

public:
  static bool validation(result_type x) { return x == val; }
  
#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& os,
             const subtract_with_carry& f)
  {
    for(unsigned int j = 0; j < long_lag; ++j)
      os << f.compute(j) << " ";
    os << f.carry << " ";
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT,Traits>&
  operator>>(std::basic_istream<CharT,Traits>& is, subtract_with_carry& f)
  {
    for(unsigned int j = 0; j < long_lag; ++j)
      is >> f.x[j] >> std::ws;
    is >> f.carry >> std::ws;
    f.k = 0;
    return is;
  }

  friend bool operator==(const subtract_with_carry& x, const subtract_with_carry& y)
  {
    for(unsigned int j = 0; j < r; ++j)
      if(x.compute(j) != y.compute(j))
        return false;
    return true;
  }

  friend bool operator!=(const subtract_with_carry& x, const subtract_with_carry& y)
  { return !(x == y); }
#else
  // Use a member function; Streamable concept not supported.
  bool operator==(const subtract_with_carry& rhs) const
  {
    for(unsigned int j = 0; j < r; ++j)
      if(compute(j) != rhs.compute(j))
        return false;
    return true;
  }

  bool operator!=(const subtract_with_carry& rhs) const
  { return !(*this == rhs); }
#endif

private:
  // returns x(i-r+index), where index is in 0..r-1
  IntType compute(unsigned int index) const
  {
    return x[(k+index) % long_lag];
  }

  // state representation; next output (state) is x(i)
  //   x[0]  ... x[k] x[k+1] ... x[long_lag-1]     represents
  //  x(i-k) ... x(i) x(i+1) ... x(i-k+long_lag-1)
  // speed: base: 20-25 nsec
  // ranlux_4: 230 nsec, ranlux_7: 430 nsec, ranlux_14: 810 nsec
  // This state representation makes operator== and save/restore more
  // difficult, because we've already computed "too much" and thus
  // have to undo some steps to get at x(i-r) etc.

  // state representation: next output (state) is x(i)
  //   x[0]  ... x[k] x[k+1]          ... x[long_lag-1]     represents
  //  x(i-k) ... x(i) x(i-long_lag+1) ... x(i-k-1)
  // speed: base 28 nsec
  // ranlux_4: 370 nsec, ranlux_7: 688 nsec, ranlux_14: 1343 nsec
  IntType x[long_lag];
  unsigned int k;
  unsigned int carry;
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
//  A definition is required even for integral static constants
template<class IntType, IntType m, unsigned int s, unsigned int r, IntType val>
const bool subtract_with_carry<IntType, m, s, r, val>::has_fixed_range;
template<class IntType, IntType m, unsigned int s, unsigned int r, IntType val>
const IntType subtract_with_carry<IntType, m, s, r, val>::min_value;
template<class IntType, IntType m, unsigned int s, unsigned int r, IntType val>
const IntType subtract_with_carry<IntType, m, s, r, val>::max_value;
template<class IntType, IntType m, unsigned int s, unsigned int r, IntType val>
const IntType subtract_with_carry<IntType, m, s, r, val>::modulus;
template<class IntType, IntType m, unsigned int s, unsigned int r, IntType val>
const unsigned int subtract_with_carry<IntType, m, s, r, val>::long_lag;
template<class IntType, IntType m, unsigned int s, unsigned int r, IntType val>
const unsigned int subtract_with_carry<IntType, m, s, r, val>::short_lag;
#endif


// use a floating-point representation to produce values in [0..1)
template<class RealType, int w, unsigned int s, unsigned int r, int val=0>
class subtract_with_carry_01
{
public:
  typedef RealType result_type;
  BOOST_STATIC_CONSTANT(bool, has_fixed_range = false);
  BOOST_STATIC_CONSTANT(int, word_size = w);
  BOOST_STATIC_CONSTANT(unsigned int, long_lag = r);
  BOOST_STATIC_CONSTANT(unsigned int, short_lag = s);

#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
  BOOST_STATIC_ASSERT(!std::numeric_limits<result_type>::is_integer);
#endif

  subtract_with_carry_01() { init_modulus(); seed(); }
  explicit subtract_with_carry_01(uint32_t value)
  { init_modulus(); seed(value);   }
  template<class It> subtract_with_carry_01(It& first, It last)
  { init_modulus(); seed(first,last); }

private:
  void init_modulus()
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::pow;
#endif
    _modulus = pow(RealType(2), word_size);
  }

public:
  // compiler-generated copy ctor and assignment operator are fine

  void seed(uint32_t value = 19780503u)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::fmod;
#endif
    const unsigned long mask = ~((~0) << w);
    random::linear_congruential<int32_t, 40014, 0, 2147483563, 0> gen(value);
    for(unsigned int j = 0; j < long_lag; ++j)
      x[j] = fmod((gen() & mask) / _modulus, RealType(1));
    carry = (x[long_lag-1] == 0) / _modulus;
    k = 0;
  }

  template<class It>
  void seed(It& first, It last)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::fmod;
#endif
    const unsigned long mask = ~((~0) << w);
    unsigned int j;
    for(j = 0; j < long_lag && first != last; ++j, ++first)
      x[j] = fmod((*first & mask) / _modulus, RealType(1));
    if(first == last && j < long_lag)
      throw std::invalid_argument("subtract_with_carry::seed");
    carry = (x[long_lag-1] == 0) / _modulus;
    k = 0;
   }

  result_type min() const { return result_type(0); }
  result_type max() const { return result_type(1); }

  result_type operator()()
  {
    int short_index = k - short_lag;
    if(short_index < 0)
      short_index += long_lag;
    RealType delta = x[short_index] - x[k] - carry;
    if(delta < 0) {
      delta += RealType(1);
      carry = RealType(1)/_modulus;
    } else {
      carry = 0;
    }
    x[k] = delta;
    ++k;
    if(k >= long_lag)
      k = 0;
    return delta;
  }

  static bool validation(result_type x)
  { return x == val/pow(RealType(2), word_size); }
  
#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& os,
             const subtract_with_carry_01& f)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::pow;
#endif
    std::ios_base::fmtflags oldflags = os.flags(os.dec | os.fixed | os.left); 
    for(unsigned int j = 0; j < long_lag; ++j)
      os << (f.compute(j) * f._modulus) << " ";
    os << (f.carry * f._modulus);
    os.flags(oldflags);
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT,Traits>&
  operator>>(std::basic_istream<CharT,Traits>& is, subtract_with_carry_01& f)
  {
    RealType value;
    for(unsigned int j = 0; j < long_lag; ++j) {
      is >> value >> std::ws;
      f.x[j] = value / f._modulus;
    }
    is >> value >> std::ws;
    f.carry = value / f._modulus;
    f.k = 0;
    return is;
  }

  friend bool operator==(const subtract_with_carry_01& x,
                         const subtract_with_carry_01& y)
  {
    for(unsigned int j = 0; j < r; ++j)
      if(x.compute(j) != y.compute(j))
        return false;
    return true;
  }

  friend bool operator!=(const subtract_with_carry_01& x,
                         const subtract_with_carry_01& y)
  { return !(x == y); }
#else
  // Use a member function; Streamable concept not supported.
  bool operator==(const subtract_with_carry_01& rhs) const
  { 
    for(unsigned int j = 0; j < r; ++j)
      if(compute(j) != rhs.compute(j))
        return false;
    return true;
  }

  bool operator!=(const subtract_with_carry_01& rhs) const
  { return !(*this == rhs); }
#endif

private:
  RealType compute(unsigned int index) const;
  unsigned int k;
  RealType carry;
  RealType x[long_lag];
  RealType _modulus;
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
//  A definition is required even for integral static constants
template<class RealType, int w, unsigned int s, unsigned int r, int val>
const bool subtract_with_carry_01<RealType, w, s, r, val>::has_fixed_range;
template<class RealType, int w, unsigned int s, unsigned int r, int val>
const int subtract_with_carry_01<RealType, w, s, r, val>::word_size;
template<class RealType, int w, unsigned int s, unsigned int r, int val>
const unsigned int subtract_with_carry_01<RealType, w, s, r, val>::long_lag;
template<class RealType, int w, unsigned int s, unsigned int r, int val>
const unsigned int subtract_with_carry_01<RealType, w, s, r, val>::short_lag;
#endif

template<class RealType, int w, unsigned int s, unsigned int r, int val>
RealType subtract_with_carry_01<RealType, w, s, r, val>::compute(unsigned int index) const
{
  return x[(k+index) % long_lag];
}


} // namespace random
} // namespace boost

#endif // BOOST_RANDOM_SUBTRACT_WITH_CARRY_HPP
