// Copyright (C) 2012-2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>

#if defined BOOST_MSVC
    #pragma warning (push)
    #pragma warning (disable : 4996)  // fn called w/params that may be unsafe
#endif

#include <boost/tuple/tuple.hpp>
#include <boost/typeof/boost/rational.hpp>
#include <boost/typeof/boost/container_gen/selectors.hpp>
#include <boost/container_gen/container_gen.hpp>
#include <boost/container_gen/emplace_function_gen.hpp>
#include <boost/tree_node/preprocessor.hpp>
#include <boost/tree_node/intrinsic/value_at_key.hpp>
#include <boost/typeof/boost/tree_node/with_count.hpp>
#include <boost/typeof/boost/tree_node/with_height.hpp>
#include <boost/typeof/boost/tree_node/with_position.hpp>

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/move/move.hpp>
#endif

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
#include <boost/typeof/boost/tree_node/with_accumulation.hpp>
#endif

#include "../example/type_definitions.hpp"
#include "iterator_functions.hpp"

typedef boost::emplace_function_gen<boost::dequeS>::type
        Emplacer;

#include <boost/typeof/boost/tree_node/binary_node.hpp>
#include <boost/tree_node/algorithm/binary_descendant.hpp>

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
typedef boost::tree_node::with_accumulation<
            boost::tree_node::with_height_base_gen<
                boost::tree_node::with_accumulation_base_gen<
                    boost::tree_node::with_position_base_gen<
                        boost::tree_node::with_accumulation_base_gen<
                            boost::tree_node::with_count_base_gen<
                                boost::tree_node::with_accumulation_base_gen<
                                    boost::tree_node::binary_node_base_gen<>
                                >
                            >
                          , AccuAccuKey
                        >
                    >
                  , AccuCountKey
                >
            >
          , boost::rational<long>
          , void
          , AccuHeightKey
        >
        ANode;
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
typedef boost::tree_node::with_height<
            boost::tree_node::with_position_base_gen<
                boost::tree_node::with_count_base_gen<
                    boost::tree_node::binary_node_base_gen<>
                >
            >
          , boost::rational<long>
        >
        ANode;
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION

typedef boost::container_gen<
            boost::dequeS
          , boost::tuples::tuple<
                boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::data_key
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::height_key
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::count_key
                >::type
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::accumulation_key<>
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , AccuHeightKey
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , AccuCountKey
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , AccuAccuKey
                >::type
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
            >
        >::type
        Values;
typedef boost::container_gen<
            boost::dequeS
          , boost::tuples::tuple<
                boost::tree_node::traversal_state
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::data_key
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::height_key
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::count_key
                >::type
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , boost::tree_node::accumulation_key<>
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , AccuHeightKey
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , AccuCountKey
                >::type
              , boost::tree_node::result_of::value_at_key<
                    ANode const
                  , AccuAccuKey
                >::type
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION
            >
        >::type
        DFValues;

int test_main(int argc, char** argv)
{
    ANode a_root(5);

    BOOST_CHECK(
        !a_root.get_parent_ptr()
//      , "Parent member uninitialized."
    );

    for (
        boost::tree_node::breadth_first_iterator<ANode> itr(a_root);
        itr;
        ++itr
    )
    {
        ANode::traits::data_type const& data = get(
            *itr
          , boost::tree_node::data_key()
        );

        if (1 < data)
        {
            ANode::iterator child_itr(itr->emplace_left(data - 2));
            ANode::const_pointer const_child(&*child_itr);

            BOOST_CHECK(
                child_itr->get_parent_ptr() == &*itr
//              , "Ctor not linking child to parent."
            );
            BOOST_CHECK(
                itr->get_left_child_ptr() == &*child_itr
//              , "Ctor not linking parent to child."
            );
            BOOST_CHECK(
                child_itr->get_parent_ptr() == const_child->get_parent_ptr()
//              , "Why are these pointers different?"
            );
            BOOST_CHECK(
                get(
                    *child_itr
                  , boost::tree_node::position_key()
                ) == child_itr
//              , "Position iterator incorrect."
            );

            child_itr = itr->emplace_right(data - 1);
            const_child = &*child_itr;

            BOOST_CHECK(
                child_itr->get_parent_ptr() == &*itr
//              , "Ctor not linking child to parent."
            );
            BOOST_CHECK(
                itr->get_right_child_ptr() == &*child_itr
//              , "Ctor not linking parent to child."
            );
            BOOST_CHECK(
                child_itr->get_parent_ptr() == const_child->get_parent_ptr()
//              , "Why are these pointers different?"
            );
            BOOST_CHECK(
                get(
                    *child_itr
                  , boost::tree_node::position_key()
                ) == child_itr
//              , "Position iterator incorrect."
            );
        }
    }

    {
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 4, 15, 26, 3, 51, boost::rational<long>(68, 15))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 4, 15, 26, 3, 51, boost::rational<long>(68, 15))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (5, 4, 15, 26, 3, 51, boost::rational<long>(68, 15));
        emplacer[in_vals]
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 4, 15, 26, 3, 51, boost::rational<long>(68, 15))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 4, 15,
            26, 3, 51, boost::rational<long>(68, 15))
                (boost::tree_node::pre_order_traversal, 3, 2, 5,
                7, 1, 11, boost::rational<long>(12, 5))
                    (boost::tree_node::pre_order_traversal, 1, 0, 1,
                    1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 1, 0, 1,
                    1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 5,
                7, 1, 11, boost::rational<long>(12, 5))
                (boost::tree_node::pre_order_traversal, 4, 3, 9,
                14, 2, 25, boost::rational<long>(10, 3))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::pre_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                (boost::tree_node::post_order_traversal, 4, 3, 9,
                14, 2, 25, boost::rational<long>(10, 3))
            (boost::tree_node::post_order_traversal, 5, 4, 15,
            26, 3, 51, boost::rational<long>(68, 15));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 4, 15)(3, 2, 5)(4, 3, 9)(1, 0, 1)(2, 1, 3)(2, 1, 3)(3, 2, 5)
            (0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1);
        emplacer[pre_vals]
            (5, 4, 15)(3, 2, 5)(1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(4, 3, 9)
            (2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 2, 5)(1, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1);
        emplacer[post_vals]
            (1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 5)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 5)(4, 3, 9)
            (5, 4, 15);
        emplacer[in_vals]
            (1, 0, 1)(3, 2, 5)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 4, 15)(0, 0, 1)
            (2, 1, 3)(1, 0, 1)(4, 3, 9)(1, 0, 1)(3, 2, 5)(0, 0, 1)(2, 1, 3)
            (1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 4, 15)
                (boost::tree_node::pre_order_traversal, 3, 2, 5)
                    (boost::tree_node::pre_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 5)
                (boost::tree_node::pre_order_traversal, 4, 3, 9)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::pre_order_traversal, 3, 2, 5)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 5)
                (boost::tree_node::post_order_traversal, 4, 3, 9)
            (boost::tree_node::post_order_traversal, 5, 4, 15);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode a_copy(a_root);
        boost::tree_node::breadth_first_iterator<ANode const>
            bf_root_itr(a_root), bf_copy_itr(a_copy);
        boost::tree_node::breadth_first_descendant_iterator<ANode const>
            bf_d_root_itr(a_root), bf_d_copy_itr(a_copy);
        boost::tree_node::pre_order_iterator<ANode const>
            pre_root_itr(a_root), pre_copy_itr(a_copy);
        boost::tree_node::pre_order_descendant_iterator<ANode const>
            pre_d_root_itr(a_root), pre_d_copy_itr(a_copy);
        boost::tree_node::post_order_iterator<ANode const>
            post_root_itr(a_root), post_copy_itr(a_copy);
        boost::tree_node::post_order_descendant_iterator<ANode const>
            post_d_root_itr(a_root), post_d_copy_itr(a_copy);
        boost::tree_node::in_order_iterator<ANode const>
            in_root_itr(a_root), in_copy_itr(a_copy);
        boost::tree_node::depth_first_iterator<ANode const>
            df_root_itr(a_root), df_copy_itr(a_copy);
        boost::tree_node::depth_first_descendant_iterator<ANode const>
            df_d_root_itr(a_root), df_d_copy_itr(a_copy);

        BOOST_CHECK(test_node_copies(bf_root_itr, bf_copy_itr));
        BOOST_CHECK(test_node_copies(bf_d_root_itr, bf_d_copy_itr));
        BOOST_CHECK(test_node_copies(pre_root_itr, pre_copy_itr));
        BOOST_CHECK(test_node_copies(pre_d_root_itr, pre_d_copy_itr));
        BOOST_CHECK(test_node_copies(post_root_itr, post_copy_itr));
        BOOST_CHECK(test_node_copies(post_d_root_itr, post_d_copy_itr));
        BOOST_CHECK(test_node_copies(in_root_itr, in_copy_itr));
        BOOST_CHECK(test_node_copies(df_root_itr, df_copy_itr));
        BOOST_CHECK(test_node_copies(df_d_root_itr, df_d_copy_itr));

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        ANode a_move(boost::move(a_copy));
        boost::tree_node::breadth_first_iterator<ANode const>
            bf_same_itr(a_root), bf_move_itr(a_move);
        boost::tree_node::breadth_first_descendant_iterator<ANode const>
            bf_d_same_itr(a_root), bf_d_move_itr(a_move);
        boost::tree_node::pre_order_iterator<ANode const>
            pre_same_itr(a_root), pre_move_itr(a_move);
        boost::tree_node::pre_order_descendant_iterator<ANode const>
            pre_d_same_itr(a_root), pre_d_move_itr(a_move);
        boost::tree_node::post_order_iterator<ANode const>
            post_same_itr(a_root), post_move_itr(a_move);
        boost::tree_node::post_order_descendant_iterator<ANode const>
            post_d_same_itr(a_root), post_d_move_itr(a_move);
        boost::tree_node::in_order_iterator<ANode const>
            in_same_itr(a_root), in_move_itr(a_move);
        boost::tree_node::depth_first_iterator<ANode const>
            df_same_itr(a_root), df_move_itr(a_move);
        boost::tree_node::depth_first_descendant_iterator<ANode const>
            df_d_same_itr(a_root), df_d_move_itr(a_move);

        BOOST_CHECK(test_node_copies(bf_same_itr, bf_move_itr));
        BOOST_CHECK(test_node_copies(bf_d_same_itr, bf_d_move_itr));
        BOOST_CHECK(test_node_copies(pre_same_itr, pre_move_itr));
        BOOST_CHECK(test_node_copies(pre_d_same_itr, pre_d_move_itr));
        BOOST_CHECK(test_node_copies(post_same_itr, post_move_itr));
        BOOST_CHECK(test_node_copies(post_d_same_itr, post_d_move_itr));
        BOOST_CHECK(test_node_copies(in_same_itr, in_move_itr));
        BOOST_CHECK(test_node_copies(df_same_itr, df_move_itr));
        BOOST_CHECK(test_node_copies(df_d_same_itr, df_d_move_itr));
        BOOST_CHECK(a_copy.empty());

        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 0, 1, 5, 0, 1, boost::rational<long>(5, 1));
        emplacer[pre_vals]
            (5, 0, 1, 5, 0, 1, boost::rational<long>(5, 1));
        emplacer[post_vals]
            (5, 0, 1, 5, 0, 1, boost::rational<long>(5, 1));
        emplacer[in_vals]
            (5, 0, 1, 5, 0, 1, boost::rational<long>(5, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 0, 1,
            5, 0, 1, boost::rational<long>(5, 1))
            (boost::tree_node::post_order_traversal, 5, 0, 1,
            5, 0, 1, boost::rational<long>(5, 1));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 0, 1);
        emplacer[pre_vals]
            (5, 0, 1);
        emplacer[post_vals]
            (5, 0, 1);
        emplacer[in_vals]
            (5, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 0, 1)
            (boost::tree_node::post_order_traversal, 5, 0, 1);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_copy
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    }

    {
        ANode::pointer p(
            a_root.get_left_child_ptr()->get_right_child_ptr()
        );
        ANode::iterator p_child_itr = p->insert_left(a_root);
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        BOOST_CHECK(5 == get(*p_child_itr, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(4 == get(*p_child_itr, boost::tree_node::height_key()));
        BOOST_CHECK(16 == get(*p_child_itr, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            26 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p_child_itr
            ).numerator()
        );
        BOOST_CHECK(3 == get(*p_child_itr, AccuHeightKey()));
        BOOST_CHECK(55 == get(*p_child_itr, AccuCountKey()));
        BOOST_CHECK(17 == get(*p_child_itr, AccuAccuKey()).numerator());
        BOOST_CHECK(4 == get(*p_child_itr, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 7, 30, 52, 6, 150, boost::rational<long>(107, 15))
            (3, 6, 20, 33, 5, 95, boost::rational<long>(33, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 5, 18, 29, 4, 74, boost::rational<long>(49, 9))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 7, 30, 52, 6, 150, boost::rational<long>(107, 15))
            (3, 6, 20, 33, 5, 95, boost::rational<long>(33, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 5, 18, 29, 4, 74, boost::rational<long>(49, 9))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 5, 18, 29, 4, 74, boost::rational<long>(49, 9))
            (3, 6, 20, 33, 5, 95, boost::rational<long>(33, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (5, 7, 30, 52, 6, 150, boost::rational<long>(107, 15));
        emplacer[in_vals]
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 6, 20, 33, 5, 95, boost::rational<long>(33, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 5, 18, 29, 4, 74, boost::rational<long>(49, 9))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 7, 30, 52, 6, 150, boost::rational<long>(107, 15))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 7, 30,
            52, 6, 150, boost::rational<long>(107, 15))
                (boost::tree_node::pre_order_traversal, 3, 6, 20,
                33, 5, 95, boost::rational<long>(33, 5))
                    (boost::tree_node::pre_order_traversal, 1, 0, 1,
                    1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 1, 0, 1,
                    1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::pre_order_traversal, 2, 5, 18,
                    29, 4, 74, boost::rational<long>(49, 9))
                        (boost::tree_node::pre_order_traversal, 5, 4, 16,
                        26, 3, 55, boost::rational<long>(17, 4))
                            (boost::tree_node::pre_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 4, 3, 9,
                            14, 2, 25, boost::rational<long>(10, 3))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 3, 2, 5,
                            7, 1, 11, boost::rational<long>(12, 5))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 3, 2, 5,
                            7, 1, 11, boost::rational<long>(12, 5))
                            (boost::tree_node::post_order_traversal, 4, 3, 9,
                            14, 2, 25, boost::rational<long>(10, 3))
                        (boost::tree_node::post_order_traversal, 5, 4, 16,
                        26, 3, 55, boost::rational<long>(17, 4))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 5, 18,
                    29, 4, 74, boost::rational<long>(49, 9))
                (boost::tree_node::post_order_traversal, 3, 6, 20,
                33, 5, 95, boost::rational<long>(33, 5))
                (boost::tree_node::pre_order_traversal, 4, 3, 9,
                14, 2, 25, boost::rational<long>(10, 3))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::pre_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                (boost::tree_node::post_order_traversal, 4, 3, 9,
                14, 2, 25, boost::rational<long>(10, 3))
            (boost::tree_node::post_order_traversal, 5, 7, 30,
            52, 6, 150, boost::rational<long>(107, 15));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 7, 30)(3, 6, 20)(4, 3, 9)(1, 0, 1)(2, 5, 18)(2, 1, 3)(3, 2, 5)
            (5, 4, 16)(1, 0, 1)(0, 0, 1)(1, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)
            (4, 3, 9)(0, 0, 1)(1, 0, 1)(1, 1, 2)(2, 1, 3)(2, 1, 3)(3, 2, 5)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 7, 30)(3, 6, 20)(1, 0, 1)(2, 5, 18)(5, 4, 16)(3, 2, 6)(1, 1, 2)
            (0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(4, 3, 9)(2, 1, 3)(0, 0, 1)
            (1, 0, 1)(3, 2, 5)(1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(1, 0, 1)
            (4, 3, 9)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 2, 5)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (1, 0, 1)(0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)
            (0, 0, 1)(1, 0, 1)(2, 1, 3)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)
            (3, 2, 5)(4, 3, 9)(5, 4, 16)(1, 0, 1)(2, 5, 18)(3, 6, 20)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 5)
            (4, 3, 9)(5, 7, 30);
        emplacer[in_vals]
            (1, 0, 1)(3, 6, 20)(0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)
            (1, 0, 1)(5, 4, 16)(0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 3, 9)(1, 0, 1)
            (3, 2, 5)(0, 0, 1)(2, 1, 3)(1, 0, 1)(2, 5, 18)(1, 0, 1)(5, 7, 30)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 3, 9)(1, 0, 1)(3, 2, 5)(0, 0, 1)
            (2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 7, 30)
                (boost::tree_node::pre_order_traversal, 3, 6, 20)
                    (boost::tree_node::pre_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::pre_order_traversal, 2, 5, 18)
                        (boost::tree_node::pre_order_traversal, 5, 4, 16)
                            (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 4, 3, 9)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 3, 2, 5)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 3, 2, 5)
                            (boost::tree_node::post_order_traversal, 4, 3, 9)
                        (boost::tree_node::post_order_traversal, 5, 4, 16)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 5, 18)
                (boost::tree_node::post_order_traversal, 3, 6, 20)
                (boost::tree_node::pre_order_traversal, 4, 3, 9)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::pre_order_traversal, 3, 2, 5)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 5)
                (boost::tree_node::post_order_traversal, 4, 3, 9)
            (boost::tree_node::post_order_traversal, 5, 7, 30);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );

        a_root = *p_child_itr;
        bf_vals.clear();
        pre_vals.clear();
        post_vals.clear();
        in_vals.clear();
        df_vals.clear();

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 4, 16,
            26, 3, 55, boost::rational<long>(17, 4))
                (boost::tree_node::pre_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::pre_order_traversal, 4, 3, 9,
                14, 2, 25, boost::rational<long>(10, 3))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::pre_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                (boost::tree_node::post_order_traversal, 4, 3, 9,
                14, 2, 25, boost::rational<long>(10, 3))
            (boost::tree_node::post_order_traversal, 5, 4, 16,
            26, 3, 55, boost::rational<long>(17, 4));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 4, 16)(3, 2, 6)(4, 3, 9)(1, 1, 2)(2, 1, 3)(2, 1, 3)(3, 2, 5)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 4, 16)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (4, 3, 9)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 2, 5)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 5)
            (4, 3, 9)(5, 4, 16);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 4, 16)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 3, 9)(1, 0, 1)(3, 2, 5)(0, 0, 1)
            (2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 4, 16)
                (boost::tree_node::pre_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 1, 2)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::pre_order_traversal, 4, 3, 9)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::pre_order_traversal, 3, 2, 5)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 5)
                (boost::tree_node::post_order_traversal, 4, 3, 9)
            (boost::tree_node::post_order_traversal, 5, 4, 16);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );

        *(p = a_root.get_right_child_ptr()) = a_root;
        test_node<a_node_case>(
            *p
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );

        bf_vals.clear();
        pre_vals.clear();
        post_vals.clear();
        in_vals.clear();
        df_vals.clear();

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 23, 38, 4, 92, boost::rational<long>(118, 23))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 5, 23, 38, 4, 92, boost::rational<long>(118, 23))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (5, 5, 23, 38, 4, 92, boost::rational<long>(118, 23));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 5, 23, 38, 4, 92, boost::rational<long>(118, 23))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 4, 16, 26, 3, 55, boost::rational<long>(17, 4))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 3, 9, 14, 2, 25, boost::rational<long>(10, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 23,
            38, 4, 92, boost::rational<long>(118, 23))
                (boost::tree_node::pre_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::pre_order_traversal, 5, 4, 16,
                26, 3, 55, boost::rational<long>(17, 4))
                    (boost::tree_node::pre_order_traversal, 3, 2, 6,
                    7, 1, 14, boost::rational<long>(2, 1))
                        (boost::tree_node::pre_order_traversal, 1, 1, 2,
                        1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 1, 1, 2,
                        1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 6,
                    7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 4, 3, 9,
                    14, 2, 25, boost::rational<long>(10, 3))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 3, 2, 5,
                        7, 1, 11, boost::rational<long>(12, 5))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 3, 2, 5,
                        7, 1, 11, boost::rational<long>(12, 5))
                    (boost::tree_node::post_order_traversal, 4, 3, 9,
                    14, 2, 25, boost::rational<long>(10, 3))
                (boost::tree_node::post_order_traversal, 5, 4, 16,
                26, 3, 55, boost::rational<long>(17, 4))
            (boost::tree_node::post_order_traversal, 5, 5, 23,
            38, 4, 92, boost::rational<long>(118, 23));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 23)(3, 2, 6)(5, 4, 16)(1, 1, 2)(2, 1, 3)(3, 2, 6)(4, 3, 9)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(1, 1, 2)(2, 1, 3)(2, 1, 3)(3, 2, 5)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 5, 23)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (5, 4, 16)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (4, 3, 9)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 2, 5)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 5)(4, 3, 9)
            (5, 4, 16)(5, 5, 23);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 5, 23)
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 4, 16)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 3, 9)(1, 0, 1)(3, 2, 5)(0, 0, 1)
            (2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 23)
                (boost::tree_node::pre_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 1, 2)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::pre_order_traversal, 5, 4, 16)
                    (boost::tree_node::pre_order_traversal, 3, 2, 6)
                        (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 4, 3, 9)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 3, 2, 5)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 3, 2, 5)
                    (boost::tree_node::post_order_traversal, 4, 3, 9)
                (boost::tree_node::post_order_traversal, 5, 4, 16)
            (boost::tree_node::post_order_traversal, 5, 5, 23);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode::pointer p(a_root.get_right_child_ptr()->rotate_left());
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        BOOST_CHECK(4 == get(*p, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(4 == get(*p, boost::tree_node::height_key()));
        BOOST_CHECK(16 == get(*p, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            26 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p
            ).numerator()
        );
        BOOST_CHECK(3 == get(*p, AccuHeightKey()));
        BOOST_CHECK(56 == get(*p, AccuCountKey()));
        BOOST_CHECK(69 == get(*p, AccuAccuKey()).numerator());
        BOOST_CHECK(16 == get(*p, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 23, 38, 4, 93, boost::rational<long>(119, 23))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 4, 16, 26, 3, 56, boost::rational<long>(69, 16))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 5, 23, 38, 4, 93, boost::rational<long>(119, 23))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 16, 26, 3, 56, boost::rational<long>(69, 16))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (4, 4, 16, 26, 3, 56, boost::rational<long>(69, 16))
            (5, 5, 23, 38, 4, 93, boost::rational<long>(119, 23));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 5, 23, 38, 4, 93, boost::rational<long>(119, 23))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 16, 26, 3, 56, boost::rational<long>(69, 16))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 5, 7, 1, 11, boost::rational<long>(12, 5))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 23,
            38, 4, 93, boost::rational<long>(119, 23))
                (boost::tree_node::pre_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::pre_order_traversal, 4, 4, 16,
                26, 3, 56, boost::rational<long>(69, 16))
                    (boost::tree_node::pre_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                    (boost::tree_node::pre_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 5,
                    7, 1, 11, boost::rational<long>(12, 5))
                (boost::tree_node::post_order_traversal, 4, 4, 16,
                26, 3, 56, boost::rational<long>(69, 16))
            (boost::tree_node::post_order_traversal, 5, 5, 23,
            38, 4, 93, boost::rational<long>(119, 23));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 23)(3, 2, 6)(4, 4, 16)(1, 1, 2)(2, 1, 3)(5, 3, 10)(3, 2, 5)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(3, 2, 6)(2, 1, 3)(1, 0, 1)(2, 1, 3)
            (1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)
            (0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 5, 23)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (4, 4, 16)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 2, 5)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 5)
            (4, 4, 16)(5, 5, 23);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 5, 23)
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 3, 10)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 16)(1, 0, 1)(3, 2, 5)(0, 0, 1)
            (2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 23)
                (boost::tree_node::pre_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 1, 2)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::pre_order_traversal, 4, 4, 16)
                    (boost::tree_node::pre_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 3, 2, 6)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 5, 3, 10)
                    (boost::tree_node::pre_order_traversal, 3, 2, 5)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 5)
                (boost::tree_node::post_order_traversal, 4, 4, 16)
            (boost::tree_node::post_order_traversal, 5, 5, 23);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode::pointer p(a_root.get_right_child_ptr()->get_right_child_ptr());
        ANode::iterator p_child_itr(p->emplace_right());
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        put(*p_child_itr, boost::tree_node::data_key(), 7);
        BOOST_CHECK(7 == get(*p_child_itr, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(2 == get(*p_child_itr, boost::tree_node::height_key()));
        BOOST_CHECK(4 == get(*p_child_itr, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            10 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p_child_itr
            ).numerator()
        );
        BOOST_CHECK(1 == get(*p_child_itr, AccuHeightKey()));
        BOOST_CHECK(9 == get(*p_child_itr, AccuCountKey()));
        BOOST_CHECK(7 == get(*p_child_itr, AccuAccuKey()).numerator());
        BOOST_CHECK(2 == get(*p_child_itr, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 24,
            45, 4, 100, boost::rational<long>(25, 4))
                (boost::tree_node::pre_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::pre_order_traversal, 4, 4, 17,
                33, 3, 62, boost::rational<long>(93, 17))
                    (boost::tree_node::pre_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                    (boost::tree_node::pre_order_traversal, 3, 3, 6,
                    14, 2, 16, boost::rational<long>(29, 6))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 7, 2, 4,
                        10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 7, 2, 4,
                        10, 1, 9, boost::rational<long>(7, 2))
                    (boost::tree_node::post_order_traversal, 3, 3, 6,
                    14, 2, 16, boost::rational<long>(29, 6))
                (boost::tree_node::post_order_traversal, 4, 4, 17,
                33, 3, 62, boost::rational<long>(93, 17))
            (boost::tree_node::post_order_traversal, 5, 5, 24,
            45, 4, 100, boost::rational<long>(25, 4));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 24)(3, 2, 6)(4, 4, 17)(1, 1, 2)(2, 1, 3)(5, 3, 10)(3, 3, 6)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(3, 2, 6)(2, 1, 3)(1, 0, 1)(7, 2, 4)
            (1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(0, 0, 1)
            (1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 5, 24)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 3, 6)(1, 0, 1)(7, 2, 4)
            (2, 1, 3)(0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(7, 2, 4)
            (3, 3, 6)(4, 4, 17)(5, 5, 24);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 5, 24)
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 3, 10)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)(3, 3, 6)(7, 2, 4)
            (0, 0, 1)(2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 24)
                (boost::tree_node::pre_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 1, 2)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::pre_order_traversal, 4, 4, 17)
                    (boost::tree_node::pre_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 3, 2, 6)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 5, 3, 10)
                    (boost::tree_node::pre_order_traversal, 3, 3, 6)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 7, 2, 4)
                    (boost::tree_node::post_order_traversal, 3, 3, 6)
                (boost::tree_node::post_order_traversal, 4, 4, 17)
            (boost::tree_node::post_order_traversal, 5, 5, 24);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode::pointer p(a_root.get_left_child_ptr()->rotate_right());
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        BOOST_CHECK(1 == get(*p, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(1 == get(*p, boost::tree_node::height_key()));
        BOOST_CHECK(6 == get(*p, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            7 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p
            ).numerator()
        );
        BOOST_CHECK(2 == get(*p, AccuHeightKey()));
        BOOST_CHECK(16 == get(*p, AccuCountKey()));
        BOOST_CHECK(17 == get(*p, AccuAccuKey()).numerator());
        BOOST_CHECK(6 == get(*p, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 24, 45, 4, 102, boost::rational<long>(155, 24))
            (1, 1, 6, 7, 2, 16, boost::rational<long>(17, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (3, 2, 4, 6, 1, 9, boost::rational<long>(5, 2))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 5, 24, 45, 4, 102, boost::rational<long>(155, 24))
            (1, 1, 6, 7, 2, 16, boost::rational<long>(17, 6))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (3, 2, 4, 6, 1, 9, boost::rational<long>(5, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 4, 6, 1, 9, boost::rational<long>(5, 2))
            (1, 1, 6, 7, 2, 16, boost::rational<long>(17, 6))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 5, 24, 45, 4, 102, boost::rational<long>(155, 24));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 6, 7, 2, 16, boost::rational<long>(17, 6))
            (3, 2, 4, 6, 1, 9, boost::rational<long>(5, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 5, 24, 45, 4, 102, boost::rational<long>(155, 24))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 24,
            45, 4, 102, boost::rational<long>(155, 24))
                (boost::tree_node::pre_order_traversal, 1, 1, 6,
                7, 2, 16, boost::rational<long>(17, 6))
                    (boost::tree_node::pre_order_traversal, 0, 0, 1,
                    0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 0, 0, 1,
                    0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::pre_order_traversal, 3, 2, 4,
                    6, 1, 9, boost::rational<long>(5, 2))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 4,
                    6, 1, 9, boost::rational<long>(5, 2))
                (boost::tree_node::post_order_traversal, 1, 1, 6,
                7, 2, 16, boost::rational<long>(17, 6))
                (boost::tree_node::pre_order_traversal, 4, 4, 17,
                33, 3, 62, boost::rational<long>(93, 17))
                    (boost::tree_node::pre_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                    (boost::tree_node::pre_order_traversal, 3, 3, 6,
                    14, 2, 16, boost::rational<long>(29, 6))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 7, 2, 4,
                        10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 7, 2, 4,
                        10, 1, 9, boost::rational<long>(7, 2))
                    (boost::tree_node::post_order_traversal, 3, 3, 6,
                    14, 2, 16, boost::rational<long>(29, 6))
                (boost::tree_node::post_order_traversal, 4, 4, 17,
                33, 3, 62, boost::rational<long>(93, 17))
            (boost::tree_node::post_order_traversal, 5, 5, 24,
            45, 4, 102, boost::rational<long>(155, 24));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 24)(1, 1, 6)(4, 4, 17)(0, 0, 1)(3, 2, 4)(5, 3, 10)(3, 3, 6)
            (2, 1, 3)(3, 2, 6)(2, 1, 3)(1, 0, 1)(7, 2, 4)(0, 0, 1)(1, 0, 1)
            (1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(0, 0, 1)
            (1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 5, 24)(1, 1, 6)(0, 0, 1)(3, 2, 4)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 3, 6)(1, 0, 1)(7, 2, 4)
            (2, 1, 3)(0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 4)(1, 1, 6)(0, 0, 1)
            (1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(7, 2, 4)
            (3, 3, 6)(4, 4, 17)(5, 5, 24);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 6)(3, 2, 4)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 5, 24)
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 3, 10)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)(3, 3, 6)(7, 2, 4)
            (0, 0, 1)(2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 24)
                (boost::tree_node::pre_order_traversal, 1, 1, 6)
                    (boost::tree_node::pre_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::pre_order_traversal, 3, 2, 4)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 4)
                (boost::tree_node::post_order_traversal, 1, 1, 6)
                (boost::tree_node::pre_order_traversal, 4, 4, 17)
                    (boost::tree_node::pre_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 3, 2, 6)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 5, 3, 10)
                    (boost::tree_node::pre_order_traversal, 3, 3, 6)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 7, 2, 4)
                    (boost::tree_node::post_order_traversal, 3, 3, 6)
                (boost::tree_node::post_order_traversal, 4, 4, 17)
            (boost::tree_node::post_order_traversal, 5, 5, 24);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode::pointer p(a_root.get_left_child_ptr()->rotate_left());
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        BOOST_CHECK(3 == get(*p, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(2 == get(*p, boost::tree_node::height_key()));
        BOOST_CHECK(6 == get(*p, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            7 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p
            ).numerator()
        );
        BOOST_CHECK(1 == get(*p, AccuHeightKey()));
        BOOST_CHECK(14 == get(*p, AccuCountKey()));
        BOOST_CHECK(2 == get(*p, AccuAccuKey()).numerator());
        BOOST_CHECK(1 == get(*p, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 5, 24, 45, 4, 100, boost::rational<long>(25, 4))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 24,
            45, 4, 100, boost::rational<long>(25, 4))
                (boost::tree_node::pre_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::pre_order_traversal, 4, 4, 17,
                33, 3, 62, boost::rational<long>(93, 17))
                    (boost::tree_node::pre_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 3, 2, 6,
                        7, 1, 14, boost::rational<long>(2, 1))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 5, 3, 10,
                    15, 2, 29, boost::rational<long>(31, 10))
                    (boost::tree_node::pre_order_traversal, 3, 3, 6,
                    14, 2, 16, boost::rational<long>(29, 6))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::pre_order_traversal, 7, 2, 4,
                        10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 7, 2, 4,
                        10, 1, 9, boost::rational<long>(7, 2))
                    (boost::tree_node::post_order_traversal, 3, 3, 6,
                    14, 2, 16, boost::rational<long>(29, 6))
                (boost::tree_node::post_order_traversal, 4, 4, 17,
                33, 3, 62, boost::rational<long>(93, 17))
            (boost::tree_node::post_order_traversal, 5, 5, 24,
            45, 4, 100, boost::rational<long>(25, 4));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 5, 24)(3, 2, 6)(4, 4, 17)(1, 1, 2)(2, 1, 3)(5, 3, 10)(3, 3, 6)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(3, 2, 6)(2, 1, 3)(1, 0, 1)(7, 2, 4)
            (1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(0, 0, 1)
            (1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 5, 24)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 3, 6)(1, 0, 1)(7, 2, 4)
            (2, 1, 3)(0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(7, 2, 4)
            (3, 3, 6)(4, 4, 17)(5, 5, 24);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 5, 24)
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 3, 10)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)(3, 3, 6)(7, 2, 4)
            (0, 0, 1)(2, 1, 3)(1, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 5, 24)
                (boost::tree_node::pre_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 1, 2)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::pre_order_traversal, 4, 4, 17)
                    (boost::tree_node::pre_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 3, 2, 6)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 5, 3, 10)
                    (boost::tree_node::pre_order_traversal, 3, 3, 6)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 7, 2, 4)
                    (boost::tree_node::post_order_traversal, 3, 3, 6)
                (boost::tree_node::post_order_traversal, 4, 4, 17)
            (boost::tree_node::post_order_traversal, 5, 5, 24);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode::pointer p(a_root.get_right_child_ptr()->emplace_parent_of_left(8));
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        BOOST_CHECK(8 == get(*p, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(5 == get(*p, boost::tree_node::height_key()));
        BOOST_CHECK(18 == get(*p, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            41 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p
            ).numerator()
        );
        BOOST_CHECK(4 == get(*p, AccuHeightKey()));
        BOOST_CHECK(80 == get(*p, AccuCountKey()));
        BOOST_CHECK(67 == get(*p, AccuAccuKey()).numerator());
        BOOST_CHECK(9 == get(*p, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 25, 53, 5, 119, boost::rational<long>(199, 25))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 6, 25, 53, 5, 119, boost::rational<long>(199, 25))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (5, 6, 25, 53, 5, 119, boost::rational<long>(199, 25));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 6, 25, 53, 5, 119, boost::rational<long>(199, 25))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 25,
            53, 5, 119, boost::rational<long>(199, 25))
                (boost::tree_node::pre_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                    (boost::tree_node::pre_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                    (boost::tree_node::post_order_traversal, 1, 1, 2,
                    1, 0, 3, boost::rational<long>(1, 2))
                    (boost::tree_node::pre_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::pre_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 0, 0, 1,
                        0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::pre_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 1, 0, 1,
                        1, 0, 1, boost::rational<long>(1, 1))
                    (boost::tree_node::post_order_traversal, 2, 1, 3,
                    3, 0, 5, boost::rational<long>(4, 3))
                (boost::tree_node::post_order_traversal, 3, 2, 6,
                7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::pre_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
                    (boost::tree_node::pre_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                        (boost::tree_node::pre_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                            (boost::tree_node::pre_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::pre_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                        (boost::tree_node::post_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                    (boost::tree_node::post_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                (boost::tree_node::post_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
            (boost::tree_node::post_order_traversal, 5, 6, 25,
            53, 5, 119, boost::rational<long>(199, 25));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 25)(3, 2, 6)(8, 5, 18)(1, 1, 2)(2, 1, 3)(4, 4, 17)(0, 0, 1)
            (0, 0, 1)(1, 0, 1)(5, 3, 10)(3, 3, 6)(3, 2, 6)(2, 1, 3)(1, 0, 1)
            (7, 2, 4)(1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)
            (0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 6, 25)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (8, 5, 18)(4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)
            (0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 3, 6)(1, 0, 1)
            (7, 2, 4)(2, 1, 3)(0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)(7, 2, 4)
            (3, 3, 6)(4, 4, 17)(8, 5, 18)(5, 6, 25);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 6, 25)
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 3, 10)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)(3, 3, 6)(7, 2, 4)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(8, 5, 18);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 25)
                (boost::tree_node::pre_order_traversal, 3, 2, 6)
                    (boost::tree_node::pre_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                    (boost::tree_node::post_order_traversal, 1, 1, 2)
                    (boost::tree_node::pre_order_traversal, 2, 1, 3)
                        (boost::tree_node::pre_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::pre_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 0, 1)
                    (boost::tree_node::post_order_traversal, 2, 1, 3)
                (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::pre_order_traversal, 8, 5, 18)
                    (boost::tree_node::pre_order_traversal, 4, 4, 17)
                        (boost::tree_node::pre_order_traversal, 5, 3, 10)
                            (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 3, 6)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 7, 2, 4)
                        (boost::tree_node::post_order_traversal, 3, 3, 6)
                    (boost::tree_node::post_order_traversal, 4, 4, 17)
                (boost::tree_node::post_order_traversal, 8, 5, 18)
            (boost::tree_node::post_order_traversal, 5, 6, 25);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    {
        ANode::pointer p(a_root.get_left_child_ptr()->emplace_parent_of_right(9));
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

        BOOST_CHECK(9 == get(*p, boost::tree_node::data_key()).numerator());
        BOOST_CHECK(3 == get(*p, boost::tree_node::height_key()));
        BOOST_CHECK(7 == get(*p, boost::tree_node::count_key()));
#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        BOOST_CHECK(
            16 == boost::tree_node::get<boost::tree_node::accumulation_key<> >(
                *p
            ).numerator()
        );
        BOOST_CHECK(2 == get(*p, AccuHeightKey()));
        BOOST_CHECK(21 == get(*p, AccuCountKey()));
        BOOST_CHECK(4 == get(*p, AccuAccuKey()).numerator());
        BOOST_CHECK(1 == get(*p, AccuAccuKey()).denominator());
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 26, 62, 5, 127, boost::rational<long>(112, 13))
            (9, 3, 7, 16, 2, 21, boost::rational<long>(4, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 6, 26, 62, 5, 127, boost::rational<long>(112, 13))
            (9, 3, 7, 16, 2, 21, boost::rational<long>(4, 1))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (9, 3, 7, 16, 2, 21, boost::rational<long>(4, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (5, 6, 26, 62, 5, 127, boost::rational<long>(112, 13));
        emplacer[in_vals]
            (9, 3, 7, 16, 2, 21, boost::rational<long>(4, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 6, 26, 62, 5, 127, boost::rational<long>(112, 13))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 26,
            62, 5, 127, boost::rational<long>(112, 13))
                (boost::tree_node::pre_order_traversal, 9, 3, 7,
                16, 2, 21, boost::rational<long>(4, 1))
                    (boost::tree_node::pre_order_traversal, 3, 2, 6,
                    7, 1, 14, boost::rational<long>(2, 1))
                        (boost::tree_node::pre_order_traversal, 1, 1, 2,
                        1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                        (boost::tree_node::post_order_traversal, 1, 1, 2,
                        1, 0, 3, boost::rational<long>(1, 2))
                        (boost::tree_node::pre_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                        (boost::tree_node::post_order_traversal, 2, 1, 3,
                        3, 0, 5, boost::rational<long>(4, 3))
                    (boost::tree_node::post_order_traversal, 3, 2, 6,
                    7, 1, 14, boost::rational<long>(2, 1))
                (boost::tree_node::post_order_traversal, 9, 3, 7,
                16, 2, 21, boost::rational<long>(4, 1))
                (boost::tree_node::pre_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
                    (boost::tree_node::pre_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                        (boost::tree_node::pre_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                            (boost::tree_node::pre_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::pre_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                        (boost::tree_node::post_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                    (boost::tree_node::post_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                (boost::tree_node::post_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
            (boost::tree_node::post_order_traversal, 5, 6, 26,
            62, 5, 127, boost::rational<long>(112, 13));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 26)(9, 3, 7)(8, 5, 18)(3, 2, 6)(4, 4, 17)(1, 1, 2)(2, 1, 3)
            (5, 3, 10)(3, 3, 6)(0, 0, 1)(0, 0, 1)(1, 0, 1)(3, 2, 6)(2, 1, 3)
            (1, 0, 1)(7, 2, 4)(1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)
            (0, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 6, 26)(9, 3, 7)(3, 2, 6)(1, 1, 2)(0, 0, 1)(2, 1, 3)(0, 0, 1)
            (1, 0, 1)(8, 5, 18)(4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)
            (2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 3, 6)
            (1, 0, 1)(7, 2, 4)(2, 1, 3)(0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(9, 3, 7)
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)
            (7, 2, 4)(3, 3, 6)(4, 4, 17)(8, 5, 18)(5, 6, 26);
        emplacer[in_vals]
            (9, 3, 7)(0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)
            (5, 6, 26)(0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)
            (5, 3, 10)(0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)(3, 3, 6)
            (7, 2, 4)(0, 0, 1)(2, 1, 3)(1, 0, 1)(8, 5, 18);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 26)
                (boost::tree_node::pre_order_traversal, 9, 3, 7)
                    (boost::tree_node::pre_order_traversal, 3, 2, 6)
                        (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                        (boost::tree_node::post_order_traversal, 1, 1, 2)
                        (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                        (boost::tree_node::post_order_traversal, 2, 1, 3)
                    (boost::tree_node::post_order_traversal, 3, 2, 6)
                (boost::tree_node::post_order_traversal, 9, 3, 7)
                (boost::tree_node::pre_order_traversal, 8, 5, 18)
                    (boost::tree_node::pre_order_traversal, 4, 4, 17)
                        (boost::tree_node::pre_order_traversal, 5, 3, 10)
                            (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 3, 6)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 7, 2, 4)
                        (boost::tree_node::post_order_traversal, 3, 3, 6)
                    (boost::tree_node::post_order_traversal, 4, 4, 17)
                (boost::tree_node::post_order_traversal, 8, 5, 18)
            (boost::tree_node::post_order_traversal, 5, 6, 26);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    a_root.begin()->clear();

    {
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 20, 55, 5, 101, boost::rational<long>(99, 10))
            (9, 0, 1, 9, 0, 1, boost::rational<long>(9, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 6, 20, 55, 5, 101, boost::rational<long>(99, 10))
            (9, 0, 1, 9, 0, 1, boost::rational<long>(9, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[post_vals]
            (9, 0, 1, 9, 0, 1, boost::rational<long>(9, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (5, 6, 20, 55, 5, 101, boost::rational<long>(99, 10));
        emplacer[in_vals]
            (9, 0, 1, 9, 0, 1, boost::rational<long>(9, 1))
            (5, 6, 20, 55, 5, 101, boost::rational<long>(99, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 20,
            55, 5, 101, boost::rational<long>(99, 10))
                (boost::tree_node::pre_order_traversal, 9, 0, 1,
                9, 0, 1, boost::rational<long>(9, 1))
                (boost::tree_node::post_order_traversal, 9, 0, 1,
                9, 0, 1, boost::rational<long>(9, 1))
                (boost::tree_node::pre_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
                    (boost::tree_node::pre_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                        (boost::tree_node::pre_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                            (boost::tree_node::pre_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::pre_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                        (boost::tree_node::post_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                    (boost::tree_node::post_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                (boost::tree_node::post_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
            (boost::tree_node::post_order_traversal, 5, 6, 20,
            55, 5, 101, boost::rational<long>(99, 10));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 20)(9, 0, 1)(8, 5, 18)(4, 4, 17)(5, 3, 10)(3, 3, 6)(3, 2, 6)
            (2, 1, 3)(1, 0, 1)(7, 2, 4)(1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(0, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 6, 20)(9, 0, 1)(8, 5, 18)(4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)
            (0, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (3, 3, 6)(1, 0, 1)(7, 2, 4)(2, 1, 3)(0, 0, 1)(1, 0, 1);
        emplacer[post_vals]
            (9, 0, 1)(0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)
            (0, 0, 1)(1, 0, 1)(2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(7, 2, 4)(3, 3, 6)(4, 4, 17)(8, 5, 18)(5, 6, 20);
        emplacer[in_vals]
            (9, 0, 1)(5, 6, 20)(0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)
            (1, 0, 1)(5, 3, 10)(0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)
            (3, 3, 6)(7, 2, 4)(0, 0, 1)(2, 1, 3)(1, 0, 1)(8, 5, 18);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 20)
                (boost::tree_node::pre_order_traversal, 9, 0, 1)
                (boost::tree_node::post_order_traversal, 9, 0, 1)
                (boost::tree_node::pre_order_traversal, 8, 5, 18)
                    (boost::tree_node::pre_order_traversal, 4, 4, 17)
                        (boost::tree_node::pre_order_traversal, 5, 3, 10)
                            (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 3, 6)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 7, 2, 4)
                        (boost::tree_node::post_order_traversal, 3, 3, 6)
                    (boost::tree_node::post_order_traversal, 4, 4, 17)
                (boost::tree_node::post_order_traversal, 8, 5, 18)
            (boost::tree_node::post_order_traversal, 5, 6, 20);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    *a_root.get_left_child_ptr() = boost::move(*a_root.get_right_child_ptr());

    {
        Values bf_vals, pre_vals, post_vals, in_vals;
        DFValues df_vals;
        Emplacer emplacer;

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 20, 54, 5, 101, boost::rational<long>(49, 5))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (8, 0, 1, 8, 0, 1, boost::rational<long>(8, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1));
        emplacer[pre_vals]
            (5, 6, 20, 54, 5, 101, boost::rational<long>(49, 5))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 0, 1, 8, 0, 1, boost::rational<long>(8, 1));
        emplacer[post_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (8, 0, 1, 8, 0, 1, boost::rational<long>(8, 1))
            (5, 6, 20, 54, 5, 101, boost::rational<long>(49, 5));
        emplacer[in_vals]
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (1, 1, 2, 1, 0, 3, boost::rational<long>(1, 2))
            (3, 2, 6, 7, 1, 14, boost::rational<long>(2, 1))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (5, 3, 10, 15, 2, 29, boost::rational<long>(31, 10))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (4, 4, 17, 33, 3, 62, boost::rational<long>(93, 17))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (3, 3, 6, 14, 2, 16, boost::rational<long>(29, 6))
            (7, 2, 4, 10, 1, 9, boost::rational<long>(7, 2))
            (0, 0, 1, 0, 0, 1, boost::rational<long>(0, 1))
            (2, 1, 3, 3, 0, 5, boost::rational<long>(4, 3))
            (1, 0, 1, 1, 0, 1, boost::rational<long>(1, 1))
            (8, 5, 18, 41, 4, 80, boost::rational<long>(67, 9))
            (5, 6, 20, 54, 5, 101, boost::rational<long>(49, 5))
            (8, 0, 1, 8, 0, 1, boost::rational<long>(8, 1));
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 20,
            54, 5, 101, boost::rational<long>(49, 5))
                (boost::tree_node::pre_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
                    (boost::tree_node::pre_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                        (boost::tree_node::pre_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                            (boost::tree_node::pre_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 1, 1, 2,
                            1, 0, 3, boost::rational<long>(1, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 3, 2, 6,
                            7, 1, 14, boost::rational<long>(2, 1))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                        (boost::tree_node::post_order_traversal, 5, 3, 10,
                        15, 2, 29, boost::rational<long>(31, 10))
                        (boost::tree_node::pre_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::pre_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                            (boost::tree_node::pre_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::pre_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::post_order_traversal, 0, 0, 1,
                            0, 0, 1, boost::rational<long>(0, 1))
                            (boost::tree_node::pre_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 1, 0, 1,
                            1, 0, 1, boost::rational<long>(1, 1))
                            (boost::tree_node::post_order_traversal, 2, 1, 3,
                            3, 0, 5, boost::rational<long>(4, 3))
                            (boost::tree_node::post_order_traversal, 7, 2, 4,
                            10, 1, 9, boost::rational<long>(7, 2))
                        (boost::tree_node::post_order_traversal, 3, 3, 6,
                        14, 2, 16, boost::rational<long>(29, 6))
                    (boost::tree_node::post_order_traversal, 4, 4, 17,
                    33, 3, 62, boost::rational<long>(93, 17))
                (boost::tree_node::post_order_traversal, 8, 5, 18,
                41, 4, 80, boost::rational<long>(67, 9))
                (boost::tree_node::pre_order_traversal, 8, 0, 1,
                8, 0, 1, boost::rational<long>(8, 1))
                (boost::tree_node::post_order_traversal, 8, 0, 1,
                8, 0, 1, boost::rational<long>(8, 1))
            (boost::tree_node::post_order_traversal, 5, 6, 20,
            54, 5, 101, boost::rational<long>(49, 5));
#else  // !defined BOOST_TREE_NODE_CAN_USE_FUSION
        emplacer[bf_vals]
            (5, 6, 20)(8, 5, 18)(8, 0, 1)(4, 4, 17)(5, 3, 10)(3, 3, 6)(3, 2, 6)
            (2, 1, 3)(1, 0, 1)(7, 2, 4)(1, 1, 2)(2, 1, 3)(0, 0, 1)(1, 0, 1)
            (2, 1, 3)(0, 0, 1)(0, 0, 1)(1, 0, 1)(0, 0, 1)(1, 0, 1);
        emplacer[pre_vals]
            (5, 6, 20)(8, 5, 18)(4, 4, 17)(5, 3, 10)(3, 2, 6)(1, 1, 2)(0, 0, 1)
            (2, 1, 3)(0, 0, 1)(1, 0, 1)(2, 1, 3)(0, 0, 1)(1, 0, 1)(3, 3, 6)
            (1, 0, 1)(7, 2, 4)(2, 1, 3)(0, 0, 1)(1, 0, 1)(8, 0, 1);
        emplacer[post_vals]
            (0, 0, 1)(1, 1, 2)(0, 0, 1)(1, 0, 1)(2, 1, 3)(3, 2, 6)(0, 0, 1)
            (1, 0, 1)(2, 1, 3)(5, 3, 10)(1, 0, 1)(0, 0, 1)(1, 0, 1)(2, 1, 3)
            (7, 2, 4)(3, 3, 6)(4, 4, 17)(8, 5, 18)(8, 0, 1)(5, 6, 20);
        emplacer[in_vals]
            (0, 0, 1)(1, 1, 2)(3, 2, 6)(0, 0, 1)(2, 1, 3)(1, 0, 1)(5, 3, 10)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(4, 4, 17)(1, 0, 1)(3, 3, 6)(7, 2, 4)
            (0, 0, 1)(2, 1, 3)(1, 0, 1)(8, 5, 18)(5, 6, 20)(8, 0, 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, 6, 20)
                (boost::tree_node::pre_order_traversal, 8, 5, 18)
                    (boost::tree_node::pre_order_traversal, 4, 4, 17)
                        (boost::tree_node::pre_order_traversal, 5, 3, 10)
                            (boost::tree_node::pre_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 1, 2)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 3, 2, 6)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                        (boost::tree_node::post_order_traversal, 5, 3, 10)
                        (boost::tree_node::pre_order_traversal, 3, 3, 6)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::pre_order_traversal, 7, 2, 4)
                            (boost::tree_node::pre_order_traversal, 2, 1, 3)
                            (boost::tree_node::pre_order_traversal, 0, 0, 1)
                            (boost::tree_node::post_order_traversal, 0, 0, 1)
                            (boost::tree_node::pre_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 1, 0, 1)
                            (boost::tree_node::post_order_traversal, 2, 1, 3)
                            (boost::tree_node::post_order_traversal, 7, 2, 4)
                        (boost::tree_node::post_order_traversal, 3, 3, 6)
                    (boost::tree_node::post_order_traversal, 4, 4, 17)
                (boost::tree_node::post_order_traversal, 8, 5, 18)
                (boost::tree_node::pre_order_traversal, 8, 0, 1)
                (boost::tree_node::post_order_traversal, 8, 0, 1)
            (boost::tree_node::post_order_traversal, 5, 6, 20);
#endif  // BOOST_TREE_NODE_CAN_USE_FUSION
        test_node<a_node_case>(
            a_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
    typedef boost::tree_node::with_accumulation<
                boost::tree_node::binary_node_base_gen<>
              , DataMap
              , void
              , AccuYourUintKey
            >
            FNode;
    typedef boost::container_gen<
                boost::dequeS
              , boost::tuples::tuple<
                    boost::tree_node::result_of::value_at_key<
                        FNode const
                      , example_keys::your_uint
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        FNode const
                      , example_keys::your_char
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        FNode const
                      , AccuYourUintKey
                    >::type
                >
            >::type
            MoreValues;
    typedef boost::container_gen<
                boost::dequeS
              , boost::tuples::tuple<
                    boost::tree_node::traversal_state
                  , boost::tree_node::result_of::value_at_key<
                        FNode const
                      , example_keys::your_uint
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        FNode const
                      , example_keys::your_char
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        FNode const
                      , AccuYourUintKey
                    >::type
                >
            >::type
            DFMoreValues;

    FNode f_root(
        boost::fusion::make_pair<example_keys::your_uint>(5)
      , boost::fusion::make_pair<example_keys::your_char>('5')
    );

    for (
        boost::tree_node::breadth_first_iterator<FNode> itr(f_root);
        itr;
        ++itr
    )
    {
        boost::tree_node::result_of::value_at_key<
            FNode const
          , example_keys::your_uint
        >::type data = boost::tree_node::get<example_keys::your_uint>(*itr);

        if (1 < data)
        {
            --data;
            itr->emplace_right(
                boost::fusion::make_pair<example_keys::your_uint>(data)
              , boost::fusion::make_pair<example_keys::your_char>('0' + data)
            );
            --data;
            itr->emplace_left(
                boost::fusion::make_pair<example_keys::your_uint>(data)
              , boost::fusion::make_pair<example_keys::your_char>('0' + data)
            );
        }
    }

    {
        MoreValues bf_vals, pre_vals, post_vals, in_vals;
        DFMoreValues df_vals;
        Emplacer emplacer;

        emplacer[bf_vals]
            (5, '5', 26)(3, '3', 7)(4, '4', 14)(1, '1', 1)(2, '2', 3)
            (2, '2', 3)(3, '3', 7)(0, '0', 0)(1, '1', 1)(0, '0', 0)
            (1, '1', 1)(1, '1', 1)(2, '2', 3)(0, '0', 0)(1, '1', 1);
        emplacer[pre_vals]
            (5, '5', 26)(3, '3', 7)(1, '1', 1)(2, '2', 3)(0, '0', 0)
            (1, '1', 1)(4, '4', 14)(2, '2', 3)(0, '0', 0)(1, '1', 1)
            (3, '3', 7)(1, '1', 1)(2, '2', 3)(0, '0', 0)(1, '1', 1);
        emplacer[post_vals]
            (1, '1', 1)(0, '0', 0)(1, '1', 1)(2, '2', 3)(3, '3', 7)
            (0, '0', 0)(1, '1', 1)(2, '2', 3)(1, '1', 1)(0, '0', 0)
            (1, '1', 1)(2, '2', 3)(3, '3', 7)(4, '4', 14)(5, '5', 26);
        emplacer[in_vals]
            (1, '1', 1)(3, '3', 7)(0, '0', 0)(2, '2', 3)(1, '1', 1)
            (5, '5', 26)(0, '0', 0)(2, '2', 3)(1, '1', 1)(4, '4', 14)
            (1, '1', 1)(3, '3', 7)(0, '0', 0)(2, '2', 3)(1, '1', 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, '5', 26)
                (boost::tree_node::pre_order_traversal, 3, '3', 7)
                    (boost::tree_node::pre_order_traversal, 1, '1', 1)
                    (boost::tree_node::post_order_traversal, 1, '1', 1)
                    (boost::tree_node::pre_order_traversal, 2, '2', 3)
                        (boost::tree_node::pre_order_traversal, 0, '0', 0)
                        (boost::tree_node::post_order_traversal, 0, '0', 0)
                        (boost::tree_node::pre_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 1, '1', 1)
                    (boost::tree_node::post_order_traversal, 2, '2', 3)
                (boost::tree_node::post_order_traversal, 3, '3', 7)
                (boost::tree_node::pre_order_traversal, 4, '4', 14)
                    (boost::tree_node::pre_order_traversal, 2, '2', 3)
                        (boost::tree_node::pre_order_traversal, 0, '0', 0)
                        (boost::tree_node::post_order_traversal, 0, '0', 0)
                        (boost::tree_node::pre_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 1, '1', 1)
                    (boost::tree_node::post_order_traversal, 2, '2', 3)
                    (boost::tree_node::pre_order_traversal, 3, '3', 7)
                        (boost::tree_node::pre_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 1, '1', 1)
                        (boost::tree_node::pre_order_traversal, 2, '2', 3)
                            (boost::tree_node::pre_order_traversal, 0, '0', 0)
                            (boost::tree_node::post_order_traversal, 0, '0', 0)
                            (boost::tree_node::pre_order_traversal, 1, '1', 1)
                            (boost::tree_node::post_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 2, '2', 3)
                    (boost::tree_node::post_order_traversal, 3, '3', 7)
                (boost::tree_node::post_order_traversal, 4, '4', 14)
            (boost::tree_node::post_order_traversal, 5, '5', 26);
        test_node<f_node_case>(
            f_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    put(
        *f_root.get_right_child_ptr()->get_left_child_ptr()
      , example_keys::your_uint()
      , 7
    );

    {
        MoreValues bf_vals, pre_vals, post_vals, in_vals;
        DFMoreValues df_vals;
        Emplacer emplacer;

        emplacer[bf_vals]
            (5, '5', 31)(3, '3', 7)(4, '4', 19)(1, '1', 1)(2, '2', 3)
            (7, '2', 8)(3, '3', 7)(0, '0', 0)(1, '1', 1)(0, '0', 0)
            (1, '1', 1)(1, '1', 1)(2, '2', 3)(0, '0', 0)(1, '1', 1);
        emplacer[pre_vals]
            (5, '5', 31)(3, '3', 7)(1, '1', 1)(2, '2', 3)(0, '0', 0)
            (1, '1', 1)(4, '4', 19)(7, '2', 8)(0, '0', 0)(1, '1', 1)
            (3, '3', 7)(1, '1', 1)(2, '2', 3)(0, '0', 0)(1, '1', 1);
        emplacer[post_vals]
            (1, '1', 1)(0, '0', 0)(1, '1', 1)(2, '2', 3)(3, '3', 7)
            (0, '0', 0)(1, '1', 1)(7, '2', 8)(1, '1', 1)(0, '0', 0)
            (1, '1', 1)(2, '2', 3)(3, '3', 7)(4, '4', 19)(5, '5', 31);
        emplacer[in_vals]
            (1, '1', 1)(3, '3', 7)(0, '0', 0)(2, '2', 3)(1, '1', 1)
            (5, '5', 31)(0, '0', 0)(7, '2', 8)(1, '1', 1)(4, '4', 19)
            (1, '1', 1)(3, '3', 7)(0, '0', 0)(2, '2', 3)(1, '1', 1);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 5, '5', 31)
                (boost::tree_node::pre_order_traversal, 3, '3', 7)
                    (boost::tree_node::pre_order_traversal, 1, '1', 1)
                    (boost::tree_node::post_order_traversal, 1, '1', 1)
                    (boost::tree_node::pre_order_traversal, 2, '2', 3)
                        (boost::tree_node::pre_order_traversal, 0, '0', 0)
                        (boost::tree_node::post_order_traversal, 0, '0', 0)
                        (boost::tree_node::pre_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 1, '1', 1)
                    (boost::tree_node::post_order_traversal, 2, '2', 3)
                (boost::tree_node::post_order_traversal, 3, '3', 7)
                (boost::tree_node::pre_order_traversal, 4, '4', 19)
                    (boost::tree_node::pre_order_traversal, 7, '2', 8)
                        (boost::tree_node::pre_order_traversal, 0, '0', 0)
                        (boost::tree_node::post_order_traversal, 0, '0', 0)
                        (boost::tree_node::pre_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 1, '1', 1)
                    (boost::tree_node::post_order_traversal, 7, '2', 8)
                    (boost::tree_node::pre_order_traversal, 3, '3', 7)
                        (boost::tree_node::pre_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 1, '1', 1)
                        (boost::tree_node::pre_order_traversal, 2, '2', 3)
                            (boost::tree_node::pre_order_traversal, 0, '0', 0)
                            (boost::tree_node::post_order_traversal, 0, '0', 0)
                            (boost::tree_node::pre_order_traversal, 1, '1', 1)
                            (boost::tree_node::post_order_traversal, 1, '1', 1)
                        (boost::tree_node::post_order_traversal, 2, '2', 3)
                    (boost::tree_node::post_order_traversal, 3, '3', 7)
                (boost::tree_node::post_order_traversal, 4, '4', 19)
            (boost::tree_node::post_order_traversal, 5, '5', 31);
        test_node<f_node_case>(
            f_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }

    //[example__gui__binary_node__type
    typedef boost::tree_node::with_accumulation<
                boost::tree_node::with_accumulation_base_gen<
                    boost::tree_node::binary_node_base_gen<>
                  , HeightSumKey
                >
              , GUITable
              , void
              , MinPriorityKey
            >
            GUINode;
    //]

    typedef boost::container_gen<
                boost::dequeS
              , boost::tuples::tuple<
                    boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , example_keys::row_number
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , example_keys::row_height
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , example_keys::row_priority
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , HeightSumKey
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , MinPriorityKey
                    >::type
                >
            >::type
            GUIValues;
    typedef boost::container_gen<
                boost::dequeS
              , boost::tuples::tuple<
                    boost::tree_node::traversal_state
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , example_keys::row_number
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , example_keys::row_height
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , example_keys::row_priority
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , HeightSumKey
                    >::type
                  , boost::tree_node::result_of::value_at_key<
                        GUINode const
                      , MinPriorityKey
                    >::type
                >
            >::type
            DFGUIValues;

    //[example__gui__binary_node__build
    GUINode gui_root(
        boost::fusion::make_pair<example_keys::row_number>(20)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(64)
    );
    gui_root.emplace_left(
        boost::fusion::make_pair<example_keys::row_number>(0)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(20)
    );
    gui_root.emplace_left(
        boost::fusion::make_pair<example_keys::row_number>(3)
      , boost::fusion::make_pair<example_keys::row_height>(4)
      , boost::fusion::make_pair<example_keys::row_priority>(88)
    )->emplace_right(
        boost::fusion::make_pair<example_keys::row_number>(4)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(79)
    );
    gui_root.emplace_left(
        boost::fusion::make_pair<example_keys::row_number>(5)
      , boost::fusion::make_pair<example_keys::row_height>(4)
      , boost::fusion::make_pair<example_keys::row_priority>(36)
    )->emplace_right(
        boost::fusion::make_pair<example_keys::row_number>(10)
      , boost::fusion::make_pair<example_keys::row_height>(4)
      , boost::fusion::make_pair<example_keys::row_priority>(5)
    )->emplace_right(
        boost::fusion::make_pair<example_keys::row_number>(12)
      , boost::fusion::make_pair<example_keys::row_height>(4)
      , boost::fusion::make_pair<example_keys::row_priority>(19)
    );
    gui_root.emplace_right(
        boost::fusion::make_pair<example_keys::row_number>(59)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(25)
    );
    gui_root.emplace_right(
        boost::fusion::make_pair<example_keys::row_number>(55)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(91)
    )->emplace_left(
        boost::fusion::make_pair<example_keys::row_number>(51)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(7)
    );
    gui_root.emplace_right(
        boost::fusion::make_pair<example_keys::row_number>(42)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(19)
    )->emplace_left(
        boost::fusion::make_pair<example_keys::row_number>(22)
      , boost::fusion::make_pair<example_keys::row_height>(12)
      , boost::fusion::make_pair<example_keys::row_priority>(54)
    );
    //]

    //[example__gui__binary_node__test
    for (
        boost::tree_node::breadth_first_descendant_iterator<GUINode const> itr(
            gui_root
        );
        itr;
        ++itr
    )
    {
        BOOST_CHECK(
            get(*itr->get_parent_ptr(), MinPriorityKey())
         <= get(*itr, MinPriorityKey())
        );
    }

    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 0, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 1, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 2, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 3, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 4, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 5, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 6, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 7, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 8, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 9, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 10, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        0 == get(
            *binary_descendant(&gui_root, 11, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        3 == get(
            *binary_descendant(&gui_root, 12, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        3 == get(
            *binary_descendant(&gui_root, 13, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        3 == get(
            *binary_descendant(&gui_root, 14, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        3 == get(
            *binary_descendant(&gui_root, 15, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        3 == get(
            *binary_descendant(&gui_root, 16, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 17, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 18, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 19, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 20, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 21, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 22, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 23, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 24, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 25, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 26, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        4 == get(
            *binary_descendant(&gui_root, 27, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        5 == get(
            *binary_descendant(&gui_root, 28, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        5 == get(
            *binary_descendant(&gui_root, 29, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        5 == get(
            *binary_descendant(&gui_root, 30, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        5 == get(
            *binary_descendant(&gui_root, 31, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        5 == get(
            *binary_descendant(&gui_root, 32, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        10 == get(
            *binary_descendant(&gui_root, 33, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        10 == get(
            *binary_descendant(&gui_root, 34, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        10 == get(
            *binary_descendant(&gui_root, 35, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        10 == get(
            *binary_descendant(&gui_root, 36, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        12 == get(
            *binary_descendant(&gui_root, 37, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        12 == get(
            *binary_descendant(&gui_root, 38, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        12 == get(
            *binary_descendant(&gui_root, 39, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 40, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 41, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 42, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 43, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 44, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 45, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 46, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 47, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 48, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 49, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 50, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 51, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        20 == get(
            *binary_descendant(&gui_root, 52, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 53, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 54, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 55, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 56, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 57, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 58, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 59, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 60, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 61, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 62, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        22 == get(
            *binary_descendant(&gui_root, 63, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 64, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 65, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 66, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 67, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 68, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 69, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 70, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 71, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 72, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 73, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 74, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 75, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        42 == get(
            *binary_descendant(&gui_root, 76, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 77, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 78, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 79, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 80, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 81, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 82, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 83, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 84, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 85, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 86, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        51 == get(
            *binary_descendant(&gui_root, 87, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 88, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 89, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 90, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 91, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 92, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 93, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 94, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 95, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 96, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 97, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 98, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 99, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        55 == get(
            *binary_descendant(&gui_root, 100, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 101, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 102, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 103, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 104, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 105, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 106, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 107, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 108, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 109, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 110, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    BOOST_CHECK(
        59 == get(
            *binary_descendant(&gui_root, 111, GUIBinaryCompare())
          , example_keys::row_number()
        )
    );
    //]

    {
        GUIValues bf_vals, pre_vals, post_vals, in_vals;
        DFGUIValues df_vals;
        Emplacer emplacer;

        emplacer[bf_vals]
            (20, 12, 64, 112, 5)(5, 4, 36, 40, 5)(42, 12, 19, 60, 7)
            (3, 4, 88, 28, 20)(10, 4, 5, 8, 5)(22, 12, 54, 12, 54)
            (55, 12, 91, 36, 7)(0, 12, 20, 12, 20)(4, 12, 79, 12, 79)
            (12, 4, 19, 4, 19)(51, 12, 7, 12, 7)(59, 12, 25, 12, 25);
        emplacer[pre_vals]
            (20, 12, 64, 112, 5)(5, 4, 36, 40, 5)(3, 4, 88, 28, 20)
            (0, 12, 20, 12, 20)(4, 12, 79, 12, 79)(10, 4, 5, 8, 5)
            (12, 4, 19, 4, 19)(42, 12, 19, 60, 7)(22, 12, 54, 12, 54)
            (55, 12, 91, 36, 7)(51, 12, 7, 12, 7)(59, 12, 25, 12, 25);
        emplacer[post_vals]
            (0, 12, 20, 12, 20)(4, 12, 79, 12, 79)(3, 4, 88, 28, 20)
            (12, 4, 19, 4, 19)(10, 4, 5, 8, 5)(5, 4, 36, 40, 5)
            (22, 12, 54, 12, 54)(51, 12, 7, 12, 7)(59, 12, 25, 12, 25)
            (55, 12, 91, 36, 7)(42, 12, 19, 60, 7)(20, 12, 64, 112, 5);
        emplacer[in_vals]
            (0, 12, 20, 12, 20)(3, 4, 88, 28, 20)(4, 12, 79, 12, 79)
            (5, 4, 36, 40, 5)(10, 4, 5, 8, 5)(12, 4, 19, 4, 19)
            (20, 12, 64, 112, 5)(22, 12, 54, 12, 54)(42, 12, 19, 60, 7)
            (51, 12, 7, 12, 7)(55, 12, 91, 36, 7)(59, 12, 25, 12, 25);
        emplacer[df_vals]
            (boost::tree_node::pre_order_traversal, 20, 12, 64, 112, 5)
                (boost::tree_node::pre_order_traversal, 5, 4, 36, 40, 5)
                    (boost::tree_node::pre_order_traversal, 3, 4, 88, 28, 20)
                        (boost::tree_node::pre_order_traversal, 0, 12, 20, 12, 20)
                        (boost::tree_node::post_order_traversal, 0, 12, 20, 12, 20)
                        (boost::tree_node::pre_order_traversal, 4, 12, 79, 12, 79)
                        (boost::tree_node::post_order_traversal, 4, 12, 79, 12, 79)
                    (boost::tree_node::post_order_traversal, 3, 4, 88, 28, 20)
                    (boost::tree_node::pre_order_traversal, 10, 4, 5, 8, 5)
                        (boost::tree_node::pre_order_traversal, 12, 4, 19, 4, 19)
                        (boost::tree_node::post_order_traversal, 12, 4, 19, 4, 19)
                    (boost::tree_node::post_order_traversal, 10, 4, 5, 8, 5)
                (boost::tree_node::post_order_traversal, 5, 4, 36, 40, 5)
                (boost::tree_node::pre_order_traversal, 42, 12, 19, 60, 7)
                    (boost::tree_node::pre_order_traversal, 22, 12, 54, 12, 54)
                    (boost::tree_node::post_order_traversal, 22, 12, 54, 12, 54)
                    (boost::tree_node::pre_order_traversal, 55, 12, 91, 36, 7)
                        (boost::tree_node::pre_order_traversal, 51, 12, 7, 12, 7)
                        (boost::tree_node::post_order_traversal, 51, 12, 7, 12, 7)
                        (boost::tree_node::pre_order_traversal, 59, 12, 25, 12, 25)
                        (boost::tree_node::post_order_traversal, 59, 12, 25, 12, 25)
                    (boost::tree_node::post_order_traversal, 55, 12, 91, 36, 7)
                (boost::tree_node::post_order_traversal, 42, 12, 19, 60, 7)
            (boost::tree_node::post_order_traversal, 20, 12, 64, 112, 5);
        test_node<gui_node_case>(
            gui_root
          , bf_vals
          , pre_vals
          , post_vals
          , in_vals
          , df_vals
        );
    }
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

    return 0;
}

#if defined BOOST_MSVC
   #pragma warning (pop)
#endif

