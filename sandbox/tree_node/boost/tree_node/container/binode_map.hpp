// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_CONTAINER_BINODE_MAP_HPP_INCLUDED
#define BOOST_TREE_NODE_CONTAINER_BINODE_MAP_HPP_INCLUDED

#include <boost/mpl/bool.hpp>
#include <boost/tree_node/preprocessor.hpp>
#include <boost/tree_node/container/binode_associative.hpp>
#include <boost/tree_node/container/binode_map_fwd.hpp>

namespace boost { namespace tree_node {

    template <
        typename Key
      , typename Mapped
      , typename NodeGenerator
      , typename Balancer
      , typename CompareSelector
    >
    class binode_map
      : public
        //[reference__binode_map__bases
        binode_associative_container<
            NodeGenerator
          , Key
          , Mapped
          , ::boost::mpl::false_
          , CompareSelector
          , Balancer
        >
        //]
    {
        typedef binode_associative_container<
                    NodeGenerator
                  , Key
                  , Mapped
                  , ::boost::mpl::false_
                  , CompareSelector
                  , Balancer
                >
                super_t;

     public:
        //[reference__binode_map__mapped_type
        typedef Mapped mapped_type;
        //]

        BOOST_TREE_NODE_ASSOCIATIVE_CONTAINER_DERIVED_BODY(binode_map, super_t)
    };
}}  // namespace boost::tree_node

namespace boost { namespace tree_node {

    template <
        typename Key
      , typename Mapped
      , typename NodeGenerator
      , typename Balancer
      , typename CompareSelector
    >
    class binode_multimap
      : public
        //[reference__binode_multimap__bases
        binode_associative_container<
            NodeGenerator
          , Key
          , Mapped
          , ::boost::mpl::true_
          , CompareSelector
          , Balancer
        >
        //]
    {
        typedef binode_associative_container<
                    NodeGenerator
                  , Key
                  , Mapped
                  , ::boost::mpl::true_
                  , CompareSelector
                  , Balancer
                >
                super_t;

     public:
        //[reference__binode_multimap__mapped_type
        typedef Mapped mapped_type;
        //]

        BOOST_TREE_NODE_ASSOCIATIVE_CONTAINER_DERIVED_BODY(
            binode_multimap
          , super_t
        )
    };
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_CONTAINER_BINODE_MAP_HPP_INCLUDED

