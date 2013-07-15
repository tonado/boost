// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_STD_UNORDERED_SET_HPP_INCLUDED
#define BOOST_TYPEOF_STD_UNORDERED_SET_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined BOOST_NO_CXX11_HDR_UNORDERED_SET

#include <unordered_set>
#include <boost/typeof/typeof.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_set, 1)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_set, 2)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_set, 3)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_set, 4)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multiset, 1)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multiset, 2)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multiset, 3)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multiset, 4)

#endif  // !defined BOOST_NO_CXX11_HDR_UNORDERED_SET

#endif  // BOOST_TYPEOF_STD_UNORDERED_SET_HPP_INCLUDED

