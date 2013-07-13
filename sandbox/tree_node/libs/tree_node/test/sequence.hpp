// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_TREE_NODE_TEST_SEQUENCE_HPP_INCLUDED
#define LIBS_TREE_NODE_TEST_SEQUENCE_HPP_INCLUDED

#include <boost/cstdint.hpp>

template <typename TreeSequence>
void test_sequence(TreeSequence& tree_sequence, ValueSequence& values)
{
    BOOST_CHECK(tree_sequence.empty());
    BOOST_CHECK(0 == tree_sequence.size());

    for (boost::int32_t i = -5; -15 < i; --i)
    {
        test_push_back(tree_sequence, values, i);
    }

    for (boost::int32_t i = 5; i < 15; ++i)
    {
        test_push_front(tree_sequence, values, i);
    }

    test_pop_back(tree_sequence, values);
    test_pop_front(tree_sequence, values);

    typename TreeSequence::size_type index = 0;

    for (boost::int32_t i = -4; i < 5; ++i)
    {
        test_insert(tree_sequence, values, index += 2, i);
    }
}

template <typename TreeSequence>
void test_sequence_more(TreeSequence& tree_sequence, ValueSequence& values)
{
    test_fill_construction(tree_sequence);
    test_range_construction(tree_sequence, values);

    ValueSequence in_range;

    for (boost::int32_t i = -20; -25 < i; --i)
    {
        in_range.push_back(i);
    }

    typename TreeSequence::size_type index = 1;

    test_range_insert(
        tree_sequence
      , values
      , index
      , in_range.begin()
      , in_range.end()
    );

    for (boost::int32_t i = 20; i < 25; ++i)
    {
        test_fill_insert(tree_sequence, values, index += 5, 3, i);
    }

    test_resize(tree_sequence, values, tree_sequence.size() + 4);
    test_resize(tree_sequence, values, tree_sequence.size() - 3);
    test_resize(tree_sequence, values, tree_sequence.size() + 5, 30);
}

#endif  // LIBS_TREE_NODE_TEST_SEQUENCE_HPP_INCLUDED

