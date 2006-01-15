/* boost random/normal_distribution.hpp header file
 *
 * Copyright Matthias Troyer and Wesley P. Petersen 2006
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#ifndef BOOST_RANDOM_MULTIVARIATE_NORMAL_DISTRIBUTION_HPP
#define BOOST_RANDOM_MULTIVARIATE_NORMAL_DISTRIBUTION_HPP

#include <cmath>
#include <cassert>
#include <iostream>
#include <boost/limits.hpp>
#include <boost/static_assert.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace boost {

// correlated multi-variate normally distributed numbers
template<class RealType = double>
class normal_distribution
{
public:
  typedef RealType input_type;
  typedef RealType result_type;
  
  typedef numeric::ublas::vector<RealType> vector_type;
  typedef numeric::ublas::matrix<RealType> matrix_type;

#if !defined(BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS) && !(defined(BOOST_MSVC) && BOOST_MSVC <= 1300)
    BOOST_STATIC_ASSERT(!std::numeric_limits<RealType>::is_integer);
#endif

  explicit normal_distribution(const vector_type& mean,
                               const matrix_type& cholesky)
    : mean_(mean)
    , cholesky_(cholesky)
    , buffer_(mean.size())
    , ptr_(buffer_.end())
  {
  }

  // compiler-generated copy constructor is NOT fine, need to purge cache
  normal_distribution(const normal_distribution& other)
    : mean_(other.mean_)
    , cholesky_(other.cholesky_)
    , buffer_(mean.size())
    , ptr_(buffer_.end())
  {
  }

  // compiler-generated copy ctor and assignment operator are fine

  vector_type const& mean() const { return mean_; }
  matrix_type const& cholesky() const { return cholesky_; }

  void reset() { ptr_ = buffer_.end(); }

  template<class Engine>
  result_type operator()(Engine& eng)
  {
#ifndef BOOST_NO_STDC_NAMESPACE
    // allow for Koenig lookup
    using std::sqrt; using std::log; using std::sin; using std::cos;
#endif
    if(!_valid) {
      _r1 = eng();
      _r2 = eng();
      _cached_rho = sqrt(-result_type(2) * log(result_type(1)-_r2));
      _valid = true;
    } else {
      _valid = false;
    }
    // Can we have a boost::mathconst please?
    const result_type pi = result_type(3.14159265358979323846);
    
    return _cached_rho * (_valid ?
                          cos(result_type(2)*pi*_r1) :
                          sin(result_type(2)*pi*_r1))
      * _sigma + _mean;
  }

#if !defined(BOOST_NO_OPERATORS_IN_NAMESPACE) && !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
  template<class CharT, class Traits>
  friend std::basic_ostream<CharT,Traits>&
  operator<<(std::basic_ostream<CharT,Traits>& os, const normal_distribution& nd)
  {
    os << nd._mean << " " << nd._sigma << " "
       << nd._valid << " " << nd._cached_rho << " " << nd._r1;
    return os;
  }

  template<class CharT, class Traits>
  friend std::basic_istream<CharT,Traits>&
  operator>>(std::basic_istream<CharT,Traits>& is, normal_distribution& nd)
  {
    is >> std::ws >> nd._mean >> std::ws >> nd._sigma
       >> std::ws >> nd._valid >> std::ws >> nd._cached_rho
       >> std::ws >> nd._r1;
    return is;
  }
#endif
private:
  vector_type mean_;
  matrix_type cholesky_;
  vector_type buffer_;
  typename vector_type::const_iterator ptr_;
};

} // namespace boost

#endif // BOOST_RANDOM_MULTIVARIATE_NORMAL_DISTRIBUTION_HPP
