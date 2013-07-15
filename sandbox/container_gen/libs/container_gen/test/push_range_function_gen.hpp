// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>

#if defined BOOST_MSVC
    #pragma warning (push)
    #pragma warning (disable : 4996)  // fn called w/params that may be unsafe
#endif

#include <boost/range/algorithm/equal.hpp>
#include <boost/container_gen/container_gen.hpp>
#include <boost/container_gen/is_associative_selector.hpp>
#include <boost/container_gen/emplace_function_gen.hpp>
#include <boost/container_gen/push_range_function_gen.hpp>
#include <boost/container_gen/insert_range_result_gen.hpp>
#include "type_definitions.hpp"
#include "result_range.hpp"
#include <boost/test/minimal.hpp>

//[test__push_range_function_gen
template <typename Selector, typename AddedContainer>
void test_push_range_function_gen(AddedContainer const& added_container)
{
    typename boost::container_gen<Selector,test_string>::type
        example_container, test_container, input_container;
    typename boost::emplace_function_gen<Selector>::type emplacer;
    typename boost::push_range_function_gen<Selector>::type range_pusher;

    emplacer[example_container]
        ("able")("fox")("easy")("baker")("charlie")("dog");
    test_container = example_container;
    emplacer[example_container]
        ("able")("fox")("iota")("hotel")("jupiter")("dog");
    emplacer[input_container]
        ("able")("fox")("iota")("hotel")("jupiter")("dog");

    typename boost::insert_range_result_gen<Selector,test_string>::type
        push_range_result = range_pusher(test_container, input_container);

    BOOST_CHECK(
        test_result_range(
            push_range_result
          , added_container
          , boost::is_associative_selector<Selector>()
        )
    );
    BOOST_CHECK(boost::range::equal(test_container, example_container));
}
//]

