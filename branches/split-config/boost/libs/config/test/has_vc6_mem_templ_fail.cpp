
//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for macro BOOST_MSVC6_MEMBER_TEMPLATES
// This file should not compile, if it does then
// BOOST_MSVC6_MEMBER_TEMPLATES may be defined.
// see boost_BOOST_MSVC6_MEMBER_TEMPLATES.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_has_vc6_mem_templ.cxx on
// Mon Jul 16 11:45:27  2001

#include <boost/config.hpp>
#include <boost/test/cpp_main.cpp>
#include "test.hpp"

#ifndef BOOST_MSVC6_MEMBER_TEMPLATES
#include "boost_has_vc6_mem_templ.cxx"
#else
#error "this file should not compile"
#endif

int cpp_main( int, char *[] )
{
	return boost_msvc6_member_templates::test();
}	
	
