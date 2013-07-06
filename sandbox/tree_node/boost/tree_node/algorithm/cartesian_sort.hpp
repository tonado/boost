// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_HPP
#define BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_HPP

#include <iterator>
#include <functional>
#include <algorithm>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/utility/equivalence_function.hpp>
#include <boost/container_gen/selectors.hpp>
#include <boost/tree_node/binary_node.hpp>
#include <boost/tree_node/key/data.hpp>
#include <boost/tree_node/intrinsic/at_key.hpp>
#include <boost/tree_node/iterator/in_order.hpp>
#include <boost/tree_node/selector/compare.hpp>
#include <boost/assert.hpp>

#if defined BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_USE_BREADTH_FIRST
#include <boost/tree_node/iterator/breadth_first.hpp>
#else
#include <boost/tree_node/iterator/breadth_first_descendant.hpp>
#endif

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/container/allocator_traits.hpp>
#endif

//[reference__cartesian_sort
namespace boost { namespace tree_node {

    template <typename Iterator, typename Compare>
    void
        cartesian_sort(
            Iterator itr_begin
          , Iterator itr_end
          , Compare compare
        );

    template <typename Iterator>
    void cartesian_sort(Iterator itr_begin, Iterator itr_end);
}}  // namespace boost::tree_node
//]

//[reference__cartesian_sort_range
namespace boost { namespace tree_node {

    template <typename Range, typename Compare>
    Range& cartesian_sort_range(Range& r, Compare compare);

    template <typename Range>
    Range& cartesian_sort_range(Range& r);
}}  // namespace boost::tree_node
//]

namespace boost { namespace tree_node {

    //[impl__cartesian_sort__build_tree
    template <typename Iterator, typename Compare>
    void
        cartesian_sort(
            Iterator itr_begin
          , Iterator itr_end
          , Compare compare
        )
    {
        if (itr_begin == itr_end) return;

        typedef typename ::boost::iterator_value<Iterator>::type Value;
        typedef ::boost::tree_node::binary_node<Value> Node;
        typedef typename Node::traits::allocator Alloc;

        Alloc allocator;
//<-
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        typename Node::pointer root_ptr = allocator.allocate(1);

        allocator.construct(root_ptr, *itr_begin);
#else
//->
        typename Node::pointer root_ptr = ::boost::container::allocator_traits<
            Alloc
        >::allocate(allocator, 1);

        ::boost::container::allocator_traits<
            Alloc
        >::construct(allocator, root_ptr, *itr_begin);
//<-
#endif
//->

        typename Node::pointer right_ptr = root_ptr;
        typename Node::pointer ancestor_ptr;

        for (Iterator itr = itr_begin; ++itr != itr_end;)
        {
            if (compare(*itr, get(*root_ptr, data_key())))
            {
                right_ptr = root_ptr = root_ptr->emplace_parent_of_left(*itr);
            }
            else if (!compare(*itr, get(*right_ptr, data_key())))
            {
                right_ptr = &*right_ptr->emplace_right(*itr);
            }
            else
            {
                for (
                    ancestor_ptr = right_ptr;
                    ancestor_ptr->get_parent_ptr() && compare(
                        *itr
                      , get(*ancestor_ptr->get_parent_ptr(), data_key())
                    );
                    ancestor_ptr = ancestor_ptr->get_parent_ptr()
                )
                {
                }

                BOOST_ASSERT(!!ancestor_ptr->get_parent_ptr());
                right_ptr = ancestor_ptr->emplace_parent_of_left(*itr);
            }
        }
        //]

        //[impl__cartesian_sort__check_in_order_symmetry_with_input_range
        BOOST_ASSERT(
            ::std::equal(
                itr_begin
              , itr_end
              , ::boost::make_transform_iterator(
                    make_in_order_iterator(*root_ptr)
                  , at_key<data_key>()
                )
              , ::boost::make_equivalence_function(compare)
            )
        );
        //]

#if defined BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_USE_BREADTH_FIRST
        //[impl__cartesian_sort__breadth_first_iterate_with_priority
        for (
            breadth_first_iterator<
                Node const
              , ::boost::std_heap_selector< ::boost::tree_node::compareS>
            > itr(
                *root_ptr
              , ::boost::bind(compare, ::boost::arg<2>(), ::boost::arg<1>())
            );
            itr;
            ++itr
        )
        {
            BOOST_ASSERT(itr_begin != itr_end);
            *itr_begin = get(*itr, data_key());
            ++itr_begin;
        }
        //]
#else  // !defined BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_USE_BREADTH_FIRST
        //[impl__cartesian_sort__breadth_first_descendant_iterate_with_priority
        *itr_begin = get(*root_ptr, data_key());
        ++itr_begin;

        for (
            breadth_first_descendant_iterator<
                Node const
              , ::boost::std_heap_selector< ::boost::tree_node::compareS>
            > itr(
                *root_ptr
              , ::boost::bind(compare, ::boost::arg<2>(), ::boost::arg<1>())
            );
            itr;
            ++itr
        )
        {
            BOOST_ASSERT(itr_begin != itr_end);
            *itr_begin = get(*itr, data_key());
            ++itr_begin;
        }
        //]
#endif  // BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_USE_BREADTH_FIRST

        //[impl__cartesian_sort__cleanup
        BOOST_ASSERT(itr_begin == itr_end);
//<-
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        allocator.destroy(root_ptr);
        allocator.deallocate(root_ptr, 1);
#else
//->
        ::boost::container::allocator_traits<
            Alloc
        >::destroy(allocator, root_ptr);
        ::boost::container::allocator_traits<
            Alloc
        >::deallocate(allocator, root_ptr, 1);
//<-
#endif
//->
    }
    //]

    template <typename Iterator>
    inline void cartesian_sort(Iterator itr_begin, Iterator itr_end)
    {
        cartesian_sort(
            itr_begin
          , itr_end
          , ::std::less<typename ::boost::iterator_value<Iterator>::type>()
        );
    }

    template <typename Range, typename Compare>
    inline Range& cartesian_sort_range(Range& r, Compare compare)
    {
        cartesian_sort(::boost::begin(r), ::boost::end(r), compare);
        return r;
    }

    template <typename Range>
    inline Range& cartesian_sort_range(Range& r)
    {
        return cartesian_sort_range(
            r
          , ::std::less<
                typename ::boost::iterator_value<
                    typename ::boost::range_iterator<Range>::type
                >::type
            >()
        );
    }
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_HPP

