
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_NO_STD_DISTANCE
// This file should not compile, if it does then
// BOOST_NO_STD_DISTANCE need not be defined.
// see boost_no_std_distance.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_no_std_distance.cxx on
// Fri Aug 17 11:23:03  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifdef BOOST_NO_STD_DISTANCE
#include "boost_no_std_distance.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
	return boost_no_std_distance::test();
}	
	
