
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_HAS_PTHREADS
// This file should not compile, if it does then
// BOOST_HAS_PTHREADS may be defined.
// see boost_has_pthreads.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_pthreads.cxx on
// Fri Aug 17 11:23:03  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_HAS_PTHREADS
#include "boost_has_pthreads.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
	return boost_has_pthreads::test();
}	
	
