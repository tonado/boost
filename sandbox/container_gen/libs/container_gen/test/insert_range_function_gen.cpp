// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "insert_range_function_gen.hpp"
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/container_gen/selectors.hpp>

//[test__insert_range_function_gen__main
int test_main(int argc, char** argv)
{
    boost::container_gen<boost::dequeS,test_string>::type added_container;
    boost::emplace_function_gen<boost::dequeS>::type emplacer;

    emplacer[added_container]
        ("able")("fox")("iota")("hotel")("jupiter")("dog");
    test_insert_range_function_gen<boost::vecS>(added_container);
    test_insert_range_function_gen<boost::dequeS>(added_container);
    test_insert_range_function_gen<boost::listS>(added_container);
    // ...
    //<-
    test_insert_range_function_gen<boost::ptr_vecS>(added_container);
    test_insert_range_function_gen<boost::ptr_dequeS>(added_container);
    test_insert_range_function_gen<boost::ptr_listS>(added_container);
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_insert_range_function_gen<boost::vector_selector<boost::mpl::true_> >(
        added_container
    );
    test_insert_range_function_gen<boost::stable_vecS>(added_container);
    test_insert_range_function_gen<
        boost::static_vector_selector<boost::mpl::int_<16> >
    >(added_container);
    test_insert_range_function_gen<boost::deque_selector<boost::mpl::true_> >(
        added_container
    );
    test_insert_range_function_gen<boost::list_selector<boost::mpl::true_> >(
        added_container
    );
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    added_container.clear();
    emplacer[added_container]
        ("able")("dog")("fox")("hotel")("iota")("jupiter");
    test_insert_range_function_gen<boost::multisetS>(added_container);
    test_insert_range_function_gen<boost::multimapS>(added_container);
    test_insert_range_function_gen<boost::ptr_multisetS>(added_container);
    test_insert_range_function_gen<boost::ptr_multimapS>(added_container);
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_insert_range_function_gen<
        boost::multiset_selector<boost::mpl::true_>
    >(added_container);
    test_insert_range_function_gen<
        boost::multimap_selector<boost::mpl::true_>
    >(added_container);
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION \
 || !defined BOOST_NO_SLIST
    added_container.clear();
    emplacer[added_container]
        ("dog")("jupiter")("hotel")("iota")("fox")("able");
    test_insert_range_function_gen<boost::slistS>(added_container);
#endif
    added_container.clear();
    emplacer[added_container]("hotel")("iota")("jupiter");
    test_insert_range_function_gen<boost::setS>(added_container);
    test_insert_range_function_gen<boost::mapS>(added_container);
    test_insert_range_function_gen<boost::ptr_setS>(added_container);
    test_insert_range_function_gen<boost::ptr_mapS>(added_container);
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_insert_range_function_gen<boost::set_selector<boost::mpl::true_> >(
        added_container
    );
    test_insert_range_function_gen<boost::map_selector<boost::mpl::true_> >(
        added_container
    );
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    //->

    return 0;
}
//]

