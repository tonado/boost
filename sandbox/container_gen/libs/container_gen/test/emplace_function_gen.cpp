// Copyright (C) 2012-2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "emplace_function_gen.hpp"
#include <boost/mpl/int.hpp>
#include <boost/container_gen/selectors.hpp>

int test_main(int argc, char** argv)
{
    test_emplace_function_gen<boost::vecS>();
    test_emplace_function_gen<boost::dequeS>();
    test_emplace_function_gen<boost::listS>();
    test_emplace_function_gen<boost::ptr_vecS>();
    test_emplace_function_gen<boost::ptr_dequeS>();
    test_emplace_function_gen<boost::ptr_listS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION || \
    !defined BOOST_NO_SLIST
    test_emplace_function_gen<boost::slistS>();
#endif
    test_emplace_function_gen<boost::setS>();
    test_emplace_function_gen<boost::mapS>();
    test_emplace_function_gen<boost::multisetS>();
    test_emplace_function_gen<boost::multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION || defined BOOST_HAS_HASH
    test_emplace_function_gen<boost::hash_setS>();
    test_emplace_function_gen<boost::hash_mapS>();
    test_emplace_function_gen<boost::hash_multisetS>();
    test_emplace_function_gen<boost::hash_multimapS>();
#endif
    test_emplace_function_gen<boost::ptr_setS>();
    test_emplace_function_gen<boost::ptr_mapS>();
    test_emplace_function_gen<boost::ptr_multisetS>();
    test_emplace_function_gen<boost::ptr_multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#if !defined BOOST_NO_CXX11_HDR_FORWARD_LIST
    test_emplace_function_gen<boost::forward_listS>();
#endif
    test_emplace_function_gen<boost::ptr_hash_setS>();
    test_emplace_function_gen<boost::ptr_hash_mapS>();
    test_emplace_function_gen<boost::ptr_hash_multisetS>();
    test_emplace_function_gen<boost::ptr_hash_multimapS>();
    test_emplace_function_gen<boost::vector_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::stable_vecS>();
    test_emplace_function_gen<
        boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gen<boost::deque_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::list_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::set_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::map_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::multiset_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::multimap_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::hash_set_selector<boost::mpl::true_> >();
    test_emplace_function_gen<boost::hash_map_selector<boost::mpl::true_> >();
    test_emplace_function_gen<
        boost::hash_multiset_selector<boost::mpl::true_>
    >();
    test_emplace_function_gen<
        boost::hash_multimap_selector<boost::mpl::true_>
    >();
    test_emplace_function_gen<boost::flat_setS>();
    test_emplace_function_gen<boost::flat_mapS>();
    test_emplace_function_gen<boost::flat_multisetS>();
    test_emplace_function_gen<boost::flat_multimapS>();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    test_emplace_function_gens<boost::vecS,boost::dequeS>();
    test_emplace_function_gens<boost::vecS,boost::listS>();
    test_emplace_function_gens<boost::vecS,boost::ptr_vecS>();
    test_emplace_function_gens<boost::vecS,boost::ptr_dequeS>();
    test_emplace_function_gens<boost::vecS,boost::ptr_listS>();
    test_emplace_function_gens<boost::dequeS,boost::listS>();
    test_emplace_function_gens<boost::dequeS,boost::ptr_vecS>();
    test_emplace_function_gens<boost::dequeS,boost::ptr_dequeS>();
    test_emplace_function_gens<boost::dequeS,boost::ptr_listS>();
    test_emplace_function_gens<boost::listS,boost::ptr_vecS>();
    test_emplace_function_gens<boost::listS,boost::ptr_dequeS>();
    test_emplace_function_gens<boost::listS,boost::ptr_listS>();
    test_emplace_function_gens<boost::ptr_vecS,boost::ptr_dequeS>();
    test_emplace_function_gens<boost::ptr_vecS,boost::ptr_listS>();
    test_emplace_function_gens<boost::ptr_dequeS,boost::ptr_listS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION || defined BOOST_HAS_HASH
    test_emplace_function_gens<boost::hash_setS,boost::hash_mapS>();
    test_emplace_function_gens<boost::hash_multisetS,boost::hash_multimapS>();
#endif
    test_emplace_function_gens<boost::setS,boost::mapS>();
    test_emplace_function_gens<boost::setS,boost::ptr_setS>();
    test_emplace_function_gens<boost::setS,boost::ptr_mapS>();
    test_emplace_function_gens<boost::mapS,boost::ptr_setS>();
    test_emplace_function_gens<boost::mapS,boost::ptr_mapS>();
    test_emplace_function_gens<boost::ptr_setS,boost::ptr_mapS>();
    test_emplace_function_gens<boost::multisetS,boost::multimapS>();
    test_emplace_function_gens<boost::multisetS,boost::ptr_multisetS>();
    test_emplace_function_gens<boost::multisetS,boost::ptr_multimapS>();
    test_emplace_function_gens<boost::multimapS,boost::ptr_multisetS>();
    test_emplace_function_gens<boost::multimapS,boost::ptr_multimapS>();
    test_emplace_function_gens<boost::ptr_multisetS,boost::ptr_multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_emplace_function_gens<boost::ptr_hash_setS,boost::ptr_hash_mapS>();
    test_emplace_function_gens<
        boost::ptr_hash_multisetS
      , boost::ptr_hash_multimapS
    >();
    test_emplace_function_gens<
        boost::vecS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::vecS,boost::stable_vecS>();
    test_emplace_function_gens<
        boost::vecS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::vecS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::vecS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::dequeS,boost::stable_vecS>();
    test_emplace_function_gens<
        boost::dequeS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::dequeS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::dequeS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::dequeS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::listS,boost::stable_vecS>();
    test_emplace_function_gens<
        boost::listS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::listS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::listS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::listS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::ptr_vecS,boost::stable_vecS>();
    test_emplace_function_gens<
        boost::ptr_vecS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::ptr_vecS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_vecS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_vecS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::ptr_dequeS,boost::stable_vecS>();
    test_emplace_function_gens<
        boost::ptr_dequeS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::ptr_dequeS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_dequeS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_dequeS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::ptr_listS,boost::stable_vecS>();
    test_emplace_function_gens<
        boost::ptr_listS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::ptr_listS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_listS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_listS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::stable_vecS
      , boost::static_vector_selector<boost::mpl::int_<16> >
    >();
    test_emplace_function_gens<
        boost::stable_vecS
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::stable_vecS
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::stable_vecS
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::static_vector_selector<boost::mpl::int_<16> >
      , boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::static_vector_selector<boost::mpl::int_<16> >
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::static_vector_selector<boost::mpl::int_<16> >
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::vector_selector<boost::mpl::true_>
      , boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::vector_selector<boost::mpl::true_>
      , boost::list_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::deque_selector<boost::mpl::true_>
      , boost::list_selector<boost::mpl::true_>
    >();
#if !defined BOOST_NO_CXX11_HDR_FORWARD_LIST
    test_emplace_function_gens<boost::forward_listS,boost::slistS>();
#endif
    test_emplace_function_gens<
        boost::setS
      , boost::set_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::mapS
      , boost::map_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::multisetS
      , boost::multiset_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::multimapS
      , boost::multimap_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_setS
      , boost::set_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_mapS
      , boost::map_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_multisetS
      , boost::multiset_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<
        boost::ptr_multimapS
      , boost::multimap_selector<boost::mpl::true_>
    >();
    test_emplace_function_gens<boost::setS,boost::flat_setS>();
    test_emplace_function_gens<boost::mapS,boost::flat_mapS>();
    test_emplace_function_gens<boost::multisetS,boost::flat_multisetS>();
    test_emplace_function_gens<boost::multimapS,boost::flat_multimapS>();
    test_emplace_function_gens<boost::ptr_setS,boost::flat_setS>();
    test_emplace_function_gens<boost::ptr_mapS,boost::flat_mapS>();
    test_emplace_function_gens<boost::ptr_multisetS,boost::flat_multisetS>();
    test_emplace_function_gens<boost::ptr_multimapS,boost::flat_multimapS>();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    test_emplace_function_gen_recursive<boost::ptr_vecS>();
    test_emplace_function_gen_recursive<boost::ptr_dequeS>();
    test_emplace_function_gen_recursive<boost::ptr_listS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_emplace_function_gen_recursive<boost::slistS>();
    test_emplace_function_gen_recursive<boost::stable_vecS>();
    test_emplace_function_gen_recursive<
        boost::vector_selector<boost::mpl::true_>
    >();
    test_emplace_function_gen_recursive<
        boost::deque_selector<boost::mpl::true_>
    >();
    test_emplace_function_gen_recursive<
        boost::list_selector<boost::mpl::true_>
    >();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    test_emplace_stack<boost::stackS>();
    test_emplace_queue<boost::queueS>();
    test_emplace_heap<boost::std_heapS>(boost::mpl::false_());
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_emplace_heap<boost::priority_queueS>();
    test_emplace_heap<boost::d_ary_heap_selector<boost::heap::arity<4> > >();
    test_emplace_heap<
        boost::d_ary_heap_selector<
            boost::heap::mutable_<true>
          , boost::heap::arity<4>
        >
    >();
    test_emplace_heap<boost::binomial_heapS>();
    test_emplace_heap<boost::fibonacci_heapS>();
    test_emplace_heap<boost::pairing_heapS>();
    test_emplace_heap<boost::skew_heapS>();
    test_emplace_heap<
        boost::skew_heap_selector<boost::heap::mutable_<true> >
    >();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    return 0;
}

