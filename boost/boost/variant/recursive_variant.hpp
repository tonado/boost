//-----------------------------------------------------------------------------
// boost variant/recursive_variant.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_RECURSIVE_VARIANT_HPP
#define BOOST_VARIANT_RECURSIVE_VARIANT_HPP

#include "boost/incomplete.hpp" // only forward-declared in variant.hpp
#include "boost/mpl/arg.hpp"

namespace boost {

typedef mpl::arg<1> recursive_variant;

} // namespace boost

#endif // BOOST_VARIANT_RECURSIVE_VARIANT_HPP
