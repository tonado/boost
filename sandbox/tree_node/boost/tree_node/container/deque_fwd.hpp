// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_CONTAINER_DEQUE_FWD_HPP_INCLUDED
#define BOOST_TREE_NODE_CONTAINER_DEQUE_FWD_HPP_INCLUDED

#include <boost/tree_node/binary_node.hpp>
#include <boost/tree_node/balancer/null.hpp>

namespace boost { namespace tree_node {

    template <
        typename T
      , typename NodeGenerator = binary_node_default_gen
      , typename Balancer = null_balancer
    >
    class deque;
}}  // namespace boost::tree_node

//[reference__deque__swap
namespace boost { namespace tree_node {

    template <typename T, typename NodeGenerator, typename Balancer>
    void
        swap(
            deque<T,NodeGenerator,Balancer>& x
          , deque<T,NodeGenerator,Balancer>& y
        );
}}  // namespace boost::tree_node
//]

#endif  // BOOST_TREE_NODE_CONTAINER_DEQUE_FWD_HPP_INCLUDED

