/* boost random/lagged_fibonacci.hpp header file
 *
 * Copyright Jens Maurer 2000-2001
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
 *  2001-02-18  moved to individual header files
 */

#ifndef BOOST_RANDOM_LAGGED_FIBONACCI_HPP
#define BOOST_RANDOM_LAGGED_FIBONACCI_HPP

#include <iostream>
#include <algorithm>     // std::max
#include <iterator>
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/cstdint.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_01.hpp>

namespace boost {
namespace random {

template<class UIntType, int w, unsigned int p, unsigned int q,
         UIntType val = 0>
class lagged_fibonacci
{
public:
  typedef UIntType result_type;
  BOOST_STATIC_CONSTANT(bool, has_fixed_range = true);
  BOOST_STATIC_CONSTANT(result_type, min_value = 0);
  BOOST_STATIC_CONSTANT(result_type, max_value = (1 << w));
  BOOST_STATIC_CONSTANT(unsigned int, long_lag = p);
  BOOST_STATIC_CONSTANT(unsigned int, short_lag = q);

  result_type min() const { return min_value; }
  result_type max() const { return max_value; }

  lagged_fibonacci() { seed(); }
  explicit lagged_fibonacci(uint32_t value) { seed(value); }
  template<class It> lagged_fibonacci(It& first, It last)
  { seed(first, last); }
  // compiler-generated copy ctor and assignment operator are fine

  void seed(uint32_t value = 331u)
  {
    minstd_rand0 gen(value);
    UIntType mask = ~((~0) << w);
    for(unsigned int j = 0; j < long_lag; ++j)
      x[j] = gen() & mask;
    i = long_lag;
  }

  template<class It>
  void seed(It& first, It last)
  {
    // word size could be smaller than the seed values
    UIntType mask = ~((~0) << w);
    unsigned int j;
    for(j = 0; j < long_lag && first != last; ++j, ++first)
      x[j] = *first & mask;
    i = long_lag;
    if(first == last && j < long_lag)
      throw std::invalid_argument("lagged_fibonacci::seed");
  }

  result_type operator()()
  {
    if(i >= long_lag)
      fill();
    return x[i++];
  }

  static bool validation(result_type x)
  {
    return x == val;
  }
  
#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& os, const lagged_fibonacci& f)
  {
    os << f.i << " ";
    for(unsigned int i = 0; i < long_lag; ++i)
      os << f.x[i] << " ";
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, lagged_fibonacci& f)
  {
    is >> f.i >> std::ws;
    for(unsigned int i = 0; i < long_lag; ++i)
      is >> f.x[i] >> std::ws;
    return is;
  }
  friend bool operator==(const lagged_fibonacci& x, const lagged_fibonacci& y)
  { return x.i == y.i && std::equal(x.x, x.x+long_lag, y.x); }
  friend bool operator!=(const lagged_fibonacci& x,
                         const lagged_fibonacci& y)
  { return !(x == y); }
#else
  // Use a member function; Streamable concept not supported.
  bool operator==(const lagged_fibonacci& rhs) const
  { return i == rhs.i && std::equal(x, x+long_lag, rhs.x); }
  bool operator!=(const lagged_fibonacci& rhs) const
  { return !(x == y); }
#endif

private:
  void fill();
  unsigned int i;
  UIntType x[long_lag];
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
//  A definition is required even for integral static constants
template<class UIntType, int w, unsigned int p, unsigned int q, UIntType val>
const bool lagged_fibonacci<UIntType, w, p, q, val>::has_fixed_range;
template<class UIntType, int w, unsigned int p, unsigned int q, UIntType val>
const typename lagged_fibonacci<UIntType, w, p, q, val>::result_type
lagged_fibonacci<UIntType, w, p, q, val>::min_value;
template<class UIntType, int w, unsigned int p, unsigned int q, UIntType val>
const typename lagged_fibonacci<UIntType, w, p, q, val>::result_type
lagged_fibonacci<UIntType, w, p, q, val>::max_value;
template<class UIntType, int w, unsigned int p, unsigned int q, UIntType val>
const unsigned int lagged_fibonacci<UIntType, w, p, q, val>::long_lag;
template<class UIntType, int w, unsigned int p, unsigned int q, UIntType val>
const unsigned int lagged_fibonacci<UIntType, w, p, q, val>::short_lag;
#endif

template<class UIntType, int w, unsigned int p, unsigned int q, UIntType val>
void lagged_fibonacci<UIntType, w, p, q, val>::fill()
{
  // two loops to avoid costly modulo operations
  UIntType mask = ~((~0) << w);
  {  // extra scope for MSVC brokenness w.r.t. for scope
  for(unsigned int j = 0; j < short_lag; ++j)
    x[j] = (x[j] + x[j+(long_lag-short_lag)]) & mask;
  }
  for(unsigned int j = short_lag; j < long_lag; ++j) {
    x[j] = (x[j] + x[j-short_lag]) & mask;
  }
  i = 0;
}



// lagged Fibonacci generator for the range [0..1)
// contributed by Matthias Troyer
// for p=55, q=24 originally by G. J. Mitchell and D. P. Moore 1958

template<class T, unsigned int p, unsigned int q>
struct fibonacci_validation
{
  BOOST_STATIC_CONSTANT(bool, is_specialized = false);
  static T value() { return 0; }
  static T tolerance() { return 0; }
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
//  A definition is required even for integral static constants
template<class T, unsigned int p, unsigned int q>
const bool fibonacci_validation<T, p, q>::is_specialized;
#endif

#define BOOST_RANDOM_FIBONACCI_VAL(T,P,Q,V,E) \
template<> \
struct fibonacci_validation<T, P, Q>  \
{                                     \
  BOOST_STATIC_CONSTANT(bool, is_specialized = true);     \
  static T value() { return V; }      \
  static T tolerance()                \
    { return std::max(E, static_cast<T>(5*std::numeric_limits<T>::epsilon())); } \
};
// (The extra static_cast<T> in the std::max call above is actually
// unnecessary except for HP aCC 1.30, which claims that
// numeric_limits<double>::epsilon() doesn't actually return a double.)

BOOST_RANDOM_FIBONACCI_VAL(double, 607, 273, 0.4293817707235914, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 1279, 418, 0.9421630240437659, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 2281, 1252, 0.1768114046909004, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 3217, 576, 0.1956232694868209, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 4423, 2098, 0.9499762202147172, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 9689, 5502, 0.05737836943695162, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 19937, 9842, 0.5076528587449834, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 23209, 13470, 0.5414473810619185, 1e-14)
BOOST_RANDOM_FIBONACCI_VAL(double, 44497,21034, 0.254135073399297, 1e-14)

#undef BOOST_RANDOM_FIBONACCI_VAL

template<class RealType, int w, unsigned int p, unsigned int q>
class lagged_fibonacci_01
{
public:
  typedef RealType result_type;
  BOOST_STATIC_CONSTANT(bool, has_fixed_range = false);
  BOOST_STATIC_CONSTANT(unsigned int, long_lag = p);
  BOOST_STATIC_CONSTANT(unsigned int, short_lag = q);

  result_type min() const { return 0.0; }
  result_type max() const { return 1.0; }

  lagged_fibonacci_01() { seed(); }
  explicit lagged_fibonacci_01(uint32_t value) { seed(value); }
  template<class Generator>
  explicit lagged_fibonacci_01(Generator & gen) { seed(gen); }
  template<class It> lagged_fibonacci_01(It& first, It last)
  { seed(first, last); }
  // compiler-generated copy ctor and assignment operator are fine

  void seed(uint32_t value = 331u)
  {
    minstd_rand0 intgen(value);
    seed(intgen);
  }

  // For GCC, moving this function out-of-line prevents inlining, which may
  // reduce overall object code size.  However, MSVC does not grok
  // out-of-line template member functions.
  template<class Generator>
  void seed(Generator & gen)
  {
    uniform_01<Generator, RealType> gen01(gen);
    // I could have used std::generate_n, but it takes "gen" by value
    for(unsigned int j = 0; j < long_lag; ++j)
      x[j] = gen01();
    i = long_lag;
  }

  template<class It>
  void seed(It& first, It last)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::pow;
#endif
    // word size could be smaller than the seed values
    unsigned long mask = ~((~0) << w);
    RealType factor = pow(RealType(0.5), w);
    unsigned int j;
    for(j = 0; j < long_lag && first != last; ++j, ++first)
      _x[j] = (*first & mask) * factor;
    i = long_lag;
    if(first == last && j < long_lag)
      throw std::invalid_argument("lagged_fibonacci_01::seed");
  }

  result_type operator()()
  {
    if(i >= long_lag)
      fill();
    return x[i++];
  }

  static bool validation(result_type x)
  {
    result_type v = fibonacci_validation<result_type, p, q>::value();
    result_type epsilon = fibonacci_validation<result_type, p, q>::tolerance();
    // std::abs is a source of trouble: sometimes, it's not overloaded
    // for double, plus the usual namespace std noncompliance -> avoid it
    // using std::abs;
    // return abs(x - v) < 5 * epsilon
    return x > v - epsilon && x < v + epsilon;
  }
  
#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& os, const lagged_fibonacci_01&f)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::pow;
#endif
    os << f.i << " ";
    RealType factor = pow(RealType(2.0), w);
    for(unsigned int i = 0; i < long_lag; ++i)
      os << f.x[i] * factor << " ";
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, lagged_fibonacci_01& f)
  {
   is >> f.i >> std::ws;
   RealType factor = pow(RealType(2.0), w);
   for(unsigned int i = 0; i < long_lag; ++i) {
      RealType value;
      is >> value >> std::ws;
      f.x[i] = value / factor;
    }
    return is;
  }
  friend bool operator==(const lagged_fibonacci_01& x,
                         const lagged_fibonacci_01& y)
  { return x.i == y.i && std::equal(x.x, x.x+long_lag, y.x); }
  friend bool operator!=(const lagged_fibonacci_01& x,
                         const lagged_fibonacci_01& y)
  { return !(x == y); }
#else
  // Use a member function; Streamable concept not supported.
  bool operator==(const lagged_fibonacci_01& rhs) const
  { return i == rhs.i && std::equal(x, x+long_lag, rhs.x); }
  bool operator!=(const lagged_fibonacci_01& rhs) const
  { return !(x == y); }
#endif

private:
  void fill();
  unsigned int i;
  RealType x[long_lag];
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
//  A definition is required even for integral static constants
template<class RealType, int w, unsigned int p, unsigned int q>
const bool lagged_fibonacci_01<RealType, w, p, q>::has_fixed_range;
template<class RealType, int w, unsigned int p, unsigned int q>
const unsigned int lagged_fibonacci_01<RealType, w, p, q>::long_lag;
template<class RealType, int w, unsigned int p, unsigned int q>
const unsigned int lagged_fibonacci_01<RealType, w, p, q>::short_lag;

#endif

template<class RealType, int w, unsigned int p, unsigned int q>
void lagged_fibonacci_01<RealType, w, p, q>::fill()
{
  // two loops to avoid costly modulo operations
  {  // extra scope for MSVC brokenness w.r.t. for scope
  for(unsigned int j = 0; j < short_lag; ++j) {
    RealType t = x[j] + x[j+(long_lag-short_lag)];
    if(t >= 1.0)
      t -= 1.0;
    x[j] = t;
  }
  }
  for(unsigned int j = short_lag; j < long_lag; ++j) {
    RealType t = x[j] + x[j-short_lag];
    if(t >= 1.0)
      t -= 1.0;
    x[j] = t;
  }
  i = 0;
}

} // namespace random

typedef random::lagged_fibonacci_01<double, 48, 607, 273> lagged_fibonacci607;
typedef random::lagged_fibonacci_01<double, 48, 1279, 418> lagged_fibonacci1279;
typedef random::lagged_fibonacci_01<double, 48, 2281, 1252> lagged_fibonacci2281;
typedef random::lagged_fibonacci_01<double, 48, 3217, 576> lagged_fibonacci3217;
typedef random::lagged_fibonacci_01<double, 48, 4423, 2098> lagged_fibonacci4423;
typedef random::lagged_fibonacci_01<double, 48, 9689, 5502> lagged_fibonacci9689;
typedef random::lagged_fibonacci_01<double, 48, 19937, 9842> lagged_fibonacci19937;
typedef random::lagged_fibonacci_01<double, 48, 23209, 13470> lagged_fibonacci23209;
typedef random::lagged_fibonacci_01<double, 48, 4497, 21034> lagged_fibonacci44497;


// It is possible to partially specialize uniform_01<> on lagged_fibonacci_01<>
// to help the compiler generate efficient code.  For GCC, this seems useless,
// because GCC optimizes (x-0)/(1-0) to (x-0).  This is good enough for now.

} // namespace boost

#endif // BOOST_RANDOM_LAGGED_FIBONACCI_HPP
