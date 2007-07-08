/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_BITOR_ASSIGN_BITOR_ASSIGN_ACQUIRE_HPP
#define BOOST_ACT_INTERLOCKED_BITOR_ASSIGN_BITOR_ASSIGN_ACQUIRE_HPP

#include <boost/act/interlocked/detail/binary_forwarder.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO                     \
( bitor_assign, acquire, &, not_additive )

#include BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER()

#endif
