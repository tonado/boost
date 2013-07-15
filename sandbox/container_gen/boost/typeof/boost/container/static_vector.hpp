// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_BOOST_CONTAINER_STATIC_VECTOR_HPP_INCLUDED
#define BOOST_TYPEOF_BOOST_CONTAINER_STATIC_VECTOR_HPP_INCLUDED

#include <boost/container/static_vector.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/typeof/boost/container/vector.hpp>

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TEMPLATE(
    ::boost::container::container_detail::static_storage_allocator
  , (typename)(BOOST_TYPEOF_INTEGRAL(::std::size_t))
)
BOOST_TYPEOF_REGISTER_TEMPLATE(
    ::boost::container::static_vector
  , (typename)(BOOST_TYPEOF_INTEGRAL(::std::size_t))
)

#endif  // BOOST_TYPEOF_BOOST_CONTAINER_STATIC_VECTOR_HPP_INCLUDED

