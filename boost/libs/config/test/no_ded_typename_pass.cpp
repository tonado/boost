
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_DEDUCED_TYPENAME
// This file should compile, if it does not then
// BOOST_DEDUCED_TYPENAME needs to be defined.
// see boost_no_ded_typename.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_ded_typename.cxx on
// Tue Nov 12 11:07:30  2002

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifndef BOOST_DEDUCED_TYPENAME
#include "boost_no_ded_typename.cxx"
#else
namespace boost_deduced_typename = empty_boost;
#endif

int cpp_main( int, char *[] )
{
   return boost_deduced_typename::test();
}  
   
