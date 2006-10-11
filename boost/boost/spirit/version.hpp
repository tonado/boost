/*=============================================================================
    Copyright (c) 2001-2006 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_VERSION_HPP)
#define SPIRIT_VERSION_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  This checks, whether the used Boost library is at least V1.34.0
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/version.hpp>

#if BOOST_VERSION < 103400
#error "Spirit v1.9.x needs at least Boost V1.34.0 to compile successfully."
#endif 

///////////////////////////////////////////////////////////////////////////////
//
//  This is the version of the current Spirit distribution
//
///////////////////////////////////////////////////////////////////////////////
#define SPIRIT_VERSION 0x1900
#define SPIRIT_PIZZA_VERSION SPIRIT_GERMAN_SAUCAGE  // :-)

#endif // defined(SPIRIT_VERSION_HPP)
