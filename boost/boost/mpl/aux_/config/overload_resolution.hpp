//-----------------------------------------------------------------------------
// boost mpl/aux_/config/overload_resolution.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_AUX_CONFIG_OVERLOAD_RESOLUTION_HPP_INCLUDED
#define BOOST_MPL_AUX_CONFIG_OVERLOAD_RESOLUTION_HPP_INCLUDED

#include "boost/config.hpp"
#include "boost/detail/workaround.hpp"

#if !defined(BOOST_MPL_BROKEN_OVERLOAD_RESOLUTION)

#   if BOOST_WORKAROUND(__BORLANDC__, <= 0x561) || BOOST_WORKAROUND(__MWERKS__, < 0x3001)
                            
#      define BOOST_MPL_BROKEN_OVERLOAD_RESOLUTION

#   endif
#endif

#endif // BOOST_MPL_AUX_CONFIG_OVERLOAD_RESOLUTION_HPP_INCLUDED
