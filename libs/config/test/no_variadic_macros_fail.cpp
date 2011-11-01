//  This file was automatically generated on Tue Aug 17 09:59:01 2010
//  by libs/config/tools/generate.cpp
//  Copyright John Maddock 2002-4.
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for the most recent version.//
//  Revision $Id: generate.cpp 49281 2008-10-11 15:40:44Z johnmaddock $
//


// Test file for macro BOOST_NO_VARIADIC_MACROS
// This file should not compile, if it does then
// BOOST_NO_VARIADIC_MACROS should not be defined.
// See file boost_no_variadic_macros.ipp for details

// Must not have BOOST_ASSERT_CONFIG set; it defeats
// the objective of this file:
#ifdef BOOST_ASSERT_CONFIG
#  undef BOOST_ASSERT_CONFIG
#endif

#include <boost/config.hpp>
#include "test.hpp"

#ifdef BOOST_NO_VARIADIC_MACROS
#include "boost_no_variadic_macros.ipp"
#else
#error "this file should not compile"
#endif

int main( int, char *[] )
{
   return boost_no_variadic_macros::test();
}

