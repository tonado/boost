
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.

// Test file for macro BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS
// This file should not compile, if it does then
// BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS need not be defined.
// see boost_no_array_type_spec.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_array_type_spec.cxx on
// Sat Dec 13 12:29:38 GMTST 2003

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_NO_ARRAY_TYPE_SPECIALIZATIONS
#include "boost_no_array_type_spec.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
   return boost_no_array_type_specializations::test();
}  
   
