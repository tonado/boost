//-----------------------------------------------------------------------------
// boost move.hpp header file
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

#ifndef BOOST_MOVE_HPP
#define BOOST_MOVE_HPP

// Header Rationale:
//
// Because move and moveable are not interdependent, they are provided
// separately in respective headers.
//
// In user and library code alike, however, they are often used
// together, so this header includes both.

#include "boost/move/move.hpp"
#include "boost/move/moveable.hpp"

#endif // BOOST_MOVE_HPP
