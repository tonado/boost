
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_HAS_GETTIMEOFDAY
// This file should not compile, if it does then
// BOOST_HAS_GETTIMEOFDAY may be defined.
// see boost_has_gettimeofday.ipp for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_gettimeofday.ipp on
// Tue Jun 15 11:47:07 GMTST 2004

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifndef BOOST_HAS_GETTIMEOFDAY
#include "boost_has_gettimeofday.ipp"
#else
#error "this file should not compile"
#endif

int main( int, char *[] )
{
   return boost_has_gettimeofday::test();
}  
   
