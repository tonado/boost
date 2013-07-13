// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_CONTAINER_BINODE_MAP_FWD_HPP_INCLUDED
#define BOOST_TREE_NODE_CONTAINER_BINODE_MAP_FWD_HPP_INCLUDED

#include <boost/container_gen/selectors.hpp>
#include <boost/tree_node/binary_node.hpp>
#include <boost/tree_node/balancer/null.hpp>

namespace boost { namespace tree_node {

    template <
        typename Key
      , typename Mapped
      , typename NodeGenerator = binary_node_default_gen
      , typename Balancer = null_balancer
      , typename CompareSelector = ::boost::less_than_selector
    >
    class binode_map;

    template <
        typename Key
      , typename Mapped
      , typename NodeGenerator = binary_node_default_gen
      , typename Balancer = null_balancer
      , typename CompareSelector = ::boost::less_than_selector
    >
    class binode_multimap;
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_CONTAINER_BINODE_MAP_FWD_HPP_INCLUDED

