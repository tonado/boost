
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_HAS_DIRENT_H
// This file should compile, if it does not then
// BOOST_HAS_DIRENT_H should not be defined.
// see boost_has_dirent_h.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_dirent_h.cxx on
// Thu Sep 12 12:24:12  2002

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_HAS_DIRENT_H
#include "boost_has_dirent_h.cxx"
#else
namespace boost_has_dirent_h = empty_boost;
#endif

int cpp_main( int, char *[] )
{
   return boost_has_dirent_h::test();
}  
   
