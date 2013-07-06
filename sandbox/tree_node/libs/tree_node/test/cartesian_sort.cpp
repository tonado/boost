// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TREE_NODE_ALGORITHM_CARTESIAN_SORT_USE_BREADTH_FIRST

#include <boost/config.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/container_gen/selectors.hpp>
#include <boost/container_gen/container_gen.hpp>
#include <boost/container_gen/emplace_function_gen.hpp>
#include <boost/tree_node/algorithm/cartesian_sort.hpp>
#include <boost/test/minimal.hpp>

int test_main(int argc, char** argv)
{
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION || \
    defined BOOST_NO_SLIST
    boost::container_gen<boost::listS,std::size_t>::type numbers, sorted;
    boost::emplace_function_gen<boost::listS>::type emplacer;
#else
    boost::container_gen<boost::slistS,std::size_t>::type numbers, sorted;
    boost::emplace_function_gen<boost::slistS>::type emplacer;
#endif

    emplacer[numbers](9)(3)(7)(1)(8)(12)(10)(20)(15)(18)(5);
    emplacer[sorted](1)(3)(5)(7)(8)(9)(10)(12)(15)(18)(20);
    boost::tree_node::cartesian_sort_range(numbers);
    BOOST_CHECK(boost::range::equal(numbers, sorted));

    return 0;
}

