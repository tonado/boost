
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_MSVC6_MEMBER_TEMPLATES
// This file should compile, if it does not then
// BOOST_MSVC6_MEMBER_TEMPLATES should not be defined.
// see boost_has_vc6_mem_templ.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_vc6_mem_templ.cxx on
// Fri Aug 17 11:23:03  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifdef BOOST_MSVC6_MEMBER_TEMPLATES
#include "boost_has_vc6_mem_templ.cxx"
#else
namespace boost_msvc6_member_templates = empty_boost;
#endif

int cpp_main( int, char *[] )
{
	return boost_msvc6_member_templates::test();
}	
	
