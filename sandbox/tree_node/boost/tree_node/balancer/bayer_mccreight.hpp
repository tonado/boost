// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_BALANCER_BAYER_MCCREIGHT_HPP_INCLUDED
#define BOOST_TREE_NODE_BALANCER_BAYER_MCCREIGHT_HPP_INCLUDED

//[reference__bayer_mccreight_balancer
namespace boost { namespace tree_node {

    template <typename KnuthOrder>
    struct bayer_mccreight_balancer
    {
        template <typename NodePointer>
        static NodePointer post_insert(NodePointer node_ptr);

        template <typename Node>
        static bool choose_predecessor(Node const& node);

        template <typename Node>
        static bool pre_erase(Node const& node);

        template <typename NodePointer>
        static NodePointer post_erase_left(NodePointer node_ptr);

        template <typename NodePointer>
        static NodePointer post_erase_right(NodePointer node_ptr);
    };

    namespace balancer {

        template <typename KnuthOrder>
        struct bayer_mccreight
          : ::boost::tree_node::bayer_mccreight_balancer<KnuthOrder>
        {
        };
    }  // namespace balancer
}}  // namespace boost::tree_node
//]

#include <iterator>

namespace boost { namespace tree_node {

    template <typename KnuthOrder>
    template <typename NodePointer>
    inline NodePointer
        bayer_mccreight<KnuthOrder>::post_insert(NodePointer node_ptr)
    {
        for (
            NodePointer parent_ptr;
            (parent_ptr = node_ptr->get_parent_ptr());
            node_ptr = parent_ptr;
        )
        {
            if (parent_ptr->size() == KnuthOrder::value)
            {
                
            }
        }

        return node_ptr;
    }

    template <typename KnuthOrder>
    template <typename Node>
    inline bool
        bayer_mccreight<KnuthOrder>::choose_predecessor(Node const& node)
    {
        return true;
    }

    template <typename KnuthOrder>
    template <typename Node>
    inline bool bayer_mccreight<KnuthOrder>::pre_erase(Node const& node)
    {
        return true;
    }

    template <typename KnuthOrder>
    template <typename NodePointer>
    inline NodePointer
        bayer_mccreight<KnuthOrder>::post_erase_left(NodePointer node_ptr)
    {
        return node_ptr;
    }

    template <typename KnuthOrder>
    template <typename NodePointer>
    inline NodePointer
        bayer_mccreight<KnuthOrder>::post_erase_right(NodePointer node_ptr)
    {
        return node_ptr;
    }
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_BALANCER_BAYER_MCCREIGHT_HPP_INCLUDED

