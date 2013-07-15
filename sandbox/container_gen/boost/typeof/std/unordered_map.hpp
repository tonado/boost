// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_STD_UNORDERED_MAP_HPP_INCLUDED
#define BOOST_TYPEOF_STD_UNORDERED_MAP_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined BOOST_NO_CXX11_HDR_UNORDERED_MAP

#include <unordered_map>
#include <boost/typeof/typeof.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_map, 2)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_map, 3)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_map, 4)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_map, 5)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multimap, 2)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multimap, 3)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multimap, 4)
BOOST_TYPEOF_REGISTER_TEMPLATE(::std::unordered_multimap, 5)

#endif  // !defined BOOST_NO_CXX11_HDR_UNORDERED_MAP

#endif  // BOOST_TYPEOF_STD_UNORDERED_MAP_HPP_INCLUDED

