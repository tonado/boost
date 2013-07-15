// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_STD_FORWARD_LIST_HPP_INCLUDED
#define BOOST_TYPEOF_STD_FORWARD_LIST_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined BOOST_NO_CXX11_HDR_FORWARD_LIST

#include <forward_list>
#include <boost/typeof/typeof.hpp>
#include <boost/typeof/std/memory.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TEMPLATE(::std::forward_list, 1)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::forward_list, 2)

#endif  // !defined BOOST_NO_CXX11_HDR_FORWARD_LIST

#endif  // BOOST_TYPEOF_STD_FORWARD_LIST_HPP_INCLUDED

