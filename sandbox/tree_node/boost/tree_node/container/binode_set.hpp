// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_CONTAINER_BINODE_SET_HPP_INCLUDED
#define BOOST_TREE_NODE_CONTAINER_BINODE_SET_HPP_INCLUDED

#include <boost/mpl/bool.hpp>
#include <boost/tree_node/preprocessor.hpp>
#include <boost/tree_node/container/binode_associative.hpp>
#include <boost/tree_node/container/binode_set_fwd.hpp>

namespace boost { namespace tree_node {

    template <
        typename T
      , typename NodeGenerator
      , typename Balancer
      , typename CompareSelector
    >
    class binode_set
      : public
        //[reference__binode_set__bases
        binode_associative_container<
            NodeGenerator
          , T
          , void
          , ::boost::mpl::false_
          , CompareSelector
          , Balancer
        >
        //]
    {
        typedef binode_associative_container<
                    NodeGenerator
                  , T
                  , void
                  , ::boost::mpl::false_
                  , CompareSelector
                  , Balancer
                >
                super_t;

     public:
        BOOST_TREE_NODE_ASSOCIATIVE_CONTAINER_DERIVED_BODY(binode_set, super_t)
    };
}}  // namespace boost::tree_node

namespace boost { namespace tree_node {

    template <
        typename T
      , typename NodeGenerator
      , typename Balancer
      , typename CompareSelector
    >
    class binode_multiset
      : public
        //[reference__binode_multiset__bases
        binode_associative_container<
            NodeGenerator
          , T
          , void
          , ::boost::mpl::true_
          , CompareSelector
          , Balancer
        >
        //]
    {
        typedef binode_associative_container<
                    NodeGenerator
                  , T
                  , void
                  , ::boost::mpl::true_
                  , CompareSelector
                  , Balancer
                >
                super_t;

     public:
        BOOST_TREE_NODE_ASSOCIATIVE_CONTAINER_DERIVED_BODY(
            binode_multiset
          , super_t
        )
    };
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_CONTAINER_BINODE_SET_HPP_INCLUDED

