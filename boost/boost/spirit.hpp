/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 1998-2003 Joel de Guzman
    Copyright (c) 2001-2003 Daniel Nuffer
    Copyright (c) 2001-2003 Hartmut Kaiser
    Copyright (c) 2002-2003 Martin Wille
    Copyright (c) 2002 Juan Carlos Arevalo-Baeza
    Copyright (c) 2002 Raghavendra Satish
    Copyright (c) 2002 Jeff Westfahl
    Copyright (c) 2001 Bruce Florman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_HPP)
#define SPIRIT_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  If SPIRIT_DEBUG is defined, the following header includes the
//  Spirit.Debug layer, otherwise the non-debug Spirit.Core is included.
//
///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/core.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.ErrorHandling
//
///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/error_handling.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Iterators
//
///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/iterator.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Symbols
//
///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/symbols.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Utilities
//
///////////////////////////////////////////////////////////////////////////////
#include "boost/spirit/utility.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit.Attributes
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1300)
#include "boost/spirit/attribute.hpp"
#endif

#endif // !defined(SPIRIT_HPP)
