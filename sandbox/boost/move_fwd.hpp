//-----------------------------------------------------------------------------
// boost move_fwd.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MOVE_FWD_HPP
#define BOOST_MOVE_FWD_HPP

namespace boost {

// class template move_traits
//
// Traits template to facilitate *nothrow* move of objects of the
// specified type.
//
template <typename T> struct move_traits;

} // namespace boost

#endif // BOOST_MOVE_FWD_HPP