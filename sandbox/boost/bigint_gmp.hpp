#ifndef BIGINT_GMP_RG08272003_HPP
#define BIGINT_GMP_RG08272003_HPP

//
// bigint_gmp.hpp - an arbitrary magnitude integer type. This code uses the same
// interface as bigint.hpp, though it is implemented using the GNU MP library
//

#include "gmp.h"
#include "boost/config.hpp"
#include "boost/operators.hpp"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <cctype>
#include <stdexcept>

namespace boost {

class bigint : boost::operators<bigint> {
  mpz_t gmp_value_;

  friend std::ostream& operator<<(std::ostream& os, bigint const& rhs);
  friend std::istream& operator>>(std::istream& is, bigint& rhs);
  
  std::ostream& print(std::ostream& os) const {
    
    if(!os.good()) return os;
    
    // RG - Add code to set up a buffer rather than allowing gmp to allocate
    std::ios_base::fmtflags flags = os.flags();
    if (flags & std::ios_base::dec) 
      return os << mpz_get_str(NULL, 10, gmp_value_);
    else if (flags & std::ios_base::hex)
      return os << mpz_get_str(NULL, 16, gmp_value_);
    else // if (flags & std::ios_base::oct)
      return os << mpz_get_str(NULL, 8, gmp_value_);
  }
  
  std::istream& from_istream(std::istream& is) {
    
    if(!is.good()) return is;
    
    // Read into an std::string, then set.
    std::string str;
    
    // skip whitespace
    char c;
    is >> c;
    // check for minus sign.
    if(!(c == '-' || std::isdigit(c))) {
      is.putback(c);
      // signal error
      is.clear(is.rdstate() | std::ios::failbit);
      return is;
    } else {
      str.push_back(c);
      
      // read in ASCII digits till it stops.
      while(is.get(c) && isdigit(c))
	str.push_back(c);
      if(is.fail())
        // clear error state
        is.clear();
      else
        // put back the last character retrieved
        is.putback(c);

      *this = bigint(str);
      return is;
    }
  }
  
  friend bigint operator-(bigint const& rhs);
  bigint& negate() {
    mpz_neg(gmp_value_,gmp_value_);
    return *this;
  }
  
  
public:
  
  bigint() {
    mpz_init(gmp_value_);
  }
  
  explicit bigint(std::string const& str) {
    if(mpz_init_set_str(gmp_value_, str.c_str(),10) < 0) {
      mpz_clear(gmp_value_);
      throw std::runtime_error("Bad value");
    }
  }
  
  bigint(long value) {
    mpz_init_set_si(gmp_value_,value);
  }
  
  bigint(const bigint& other) {
    mpz_init_set(gmp_value_,other.gmp_value_);
  }

  bigint& operator=(const bigint& other) {
    if (&other != this) {
      mpz_init_set(gmp_value_,other.gmp_value_);
    }
  }

  ~bigint() {
    mpz_clear(gmp_value_);
  }


  bool is_zero() const {
    return mpz_sgn(gmp_value_) == 0;
  }
    
  bool positive() const {
    return mpz_sgn(gmp_value_) > 0;
  }

  bool negative() const {
    return mpz_sgn(gmp_value_) < 0;
  }

  bool operator!() const {
    return this->is_zero();
  }
  
  bool operator<(bigint const& other) const {
    return mpz_cmp(gmp_value_,other.gmp_value_) < 0;
  }

  bool operator==(bigint const& other) const {
    return mpz_cmp(gmp_value_,other.gmp_value_) == 0;
  }

  bigint& operator+=(bigint const& other) {
    mpz_add(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator-=(bigint const& other) {
    mpz_sub(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator*=(bigint const& other) {
    mpz_mul(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator/=(bigint const& other) {

    mpz_tdiv_q(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator%=(bigint const& other) {
    mpz_tdiv_r(gmp_value_,gmp_value_,other.gmp_value_);
    return *this;
  }

  bigint& operator++() {
    mpz_add_ui(gmp_value_,gmp_value_,1);
    return *this;
  }

  bigint& operator--() {
    mpz_sub_ui(gmp_value_,gmp_value_,1);
    return *this;
  }

  void swap(bigint& other) {
    mpz_swap(gmp_value_,other.gmp_value_);
  }
};


void swap(bigint& lhs, bigint& rhs) {
  lhs.swap(rhs);
}

inline
std::ostream& operator<<(std::ostream& os, bigint const& rhs) {
  return rhs.print(os);
}

inline
std::istream& operator>>(std::istream& is, bigint& rhs) {
  return rhs.from_istream(is);
}

inline
bigint operator-(bigint const& rhs) {
  bigint result(rhs);
  return result.negate();
}

inline
bigint operator+(bigint const& rhs) {
  return rhs;
}


} // namespace boost


#endif // BIGINT_GMP_RG08272003_HPP
