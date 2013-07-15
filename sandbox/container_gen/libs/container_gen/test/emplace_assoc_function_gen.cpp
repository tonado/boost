// Copyright (C) 2012-2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "emplace_assoc_function_gen.hpp"
#include <boost/mpl/bool.hpp>
#include <boost/container_gen/selectors.hpp>

int test_main(int argc, char** argv)
{
    test_empl_assoc_func_gen<boost::setS>();
    test_empl_assoc_func_gen<boost::mapS>();
    test_empl_assoc_func_gen<boost::multisetS>();
    test_empl_assoc_func_gen<boost::multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION || defined BOOST_HAS_HASH
    test_empl_assoc_func_gen<boost::hash_setS>();
    test_empl_assoc_func_gen<boost::hash_mapS>();
    test_empl_assoc_func_gen<boost::hash_multisetS>();
    test_empl_assoc_func_gen<boost::hash_multimapS>();
#endif
    test_empl_assoc_func_gen<boost::ptr_setS>();
    test_empl_assoc_func_gen<boost::ptr_mapS>();
    test_empl_assoc_func_gen<boost::ptr_multisetS>();
    test_empl_assoc_func_gen<boost::ptr_multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_empl_assoc_func_gen<boost::ptr_hash_setS>();
    test_empl_assoc_func_gen<boost::ptr_hash_mapS>();
    test_empl_assoc_func_gen<boost::ptr_hash_multisetS>();
    test_empl_assoc_func_gen<boost::ptr_hash_multimapS>();
    test_empl_assoc_func_gen<boost::set_selector<boost::mpl::true_> >();
    test_empl_assoc_func_gen<boost::map_selector<boost::mpl::true_> >();
    test_empl_assoc_func_gen<boost::multiset_selector<boost::mpl::true_> >();
    test_empl_assoc_func_gen<boost::multimap_selector<boost::mpl::true_> >();
#if !defined BOOST_NO_CXX11_HDR_UNORDERED_SET && \
    !defined BOOST_NO_CXX11_HDR_UNORDERED_MAP
    test_empl_assoc_func_gen<boost::unordered_setS>();
    test_empl_assoc_func_gen<boost::unordered_mapS>();
    test_empl_assoc_func_gen<boost::unordered_multisetS>();
    test_empl_assoc_func_gen<boost::unordered_multimapS>();
#endif
    test_empl_assoc_func_gen<boost::hash_set_selector<boost::mpl::true_> >();
    test_empl_assoc_func_gen<boost::hash_map_selector<boost::mpl::true_> >();
    test_empl_assoc_func_gen<
        boost::hash_multiset_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen<
        boost::hash_multimap_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen<boost::flat_setS>();
    test_empl_assoc_func_gen<boost::flat_mapS>();
    test_empl_assoc_func_gen<boost::flat_multisetS>();
    test_empl_assoc_func_gen<boost::flat_multimapS>();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    test_empl_assoc_func_gens<boost::setS,boost::ptr_setS>();
    test_empl_assoc_func_gens<boost::mapS,boost::ptr_mapS>();
    test_empl_assoc_func_gens<boost::multisetS,boost::ptr_multisetS>();
    test_empl_assoc_func_gens<boost::multimapS,boost::ptr_multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_empl_assoc_func_gens<
        boost::setS
      , boost::set_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gens<
        boost::mapS
      , boost::map_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gens<
        boost::multisetS
      , boost::multiset_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gens<
        boost::multimapS
      , boost::multimap_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gens<boost::setS,boost::flat_setS>();
    test_empl_assoc_func_gens<boost::mapS,boost::flat_mapS>();
    test_empl_assoc_func_gens<boost::multisetS,boost::flat_multisetS>();
    test_empl_assoc_func_gens<boost::multimapS,boost::flat_multimapS>();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    test_empl_assoc_func_gen_recursive<boost::ptr_setS>();
    test_empl_assoc_func_gen_recursive<boost::ptr_mapS>();
    test_empl_assoc_func_gen_recursive<boost::ptr_multisetS>();
    test_empl_assoc_func_gen_recursive<boost::ptr_multimapS>();
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    test_empl_assoc_func_gen_recursive<boost::ptr_hash_setS>();
    test_empl_assoc_func_gen_recursive<boost::ptr_hash_mapS>();
    test_empl_assoc_func_gen_recursive<boost::ptr_hash_multisetS>();
    test_empl_assoc_func_gen_recursive<boost::ptr_hash_multimapS>();
    test_empl_assoc_func_gen_recursive<
        boost::set_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<
        boost::map_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<
        boost::multiset_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<
        boost::multimap_selector<boost::mpl::true_>
    >();
#if !defined BOOST_NO_CXX11_HDR_UNORDERED_SET && \
    !defined BOOST_NO_CXX11_HDR_UNORDERED_MAP
    test_empl_assoc_func_gen_recursive<boost::unordered_setS>();
    test_empl_assoc_func_gen_recursive<boost::unordered_mapS>();
    test_empl_assoc_func_gen_recursive<boost::unordered_multisetS>();
    test_empl_assoc_func_gen_recursive<boost::unordered_multimapS>();
#endif
    test_empl_assoc_func_gen_recursive<
        boost::hash_set_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<
        boost::hash_map_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<
        boost::hash_multiset_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<
        boost::hash_multimap_selector<boost::mpl::true_>
    >();
    test_empl_assoc_func_gen_recursive<boost::flat_setS>();
    test_empl_assoc_func_gen_recursive<boost::flat_mapS>();
    test_empl_assoc_func_gen_recursive<boost::flat_multisetS>();
    test_empl_assoc_func_gen_recursive<boost::flat_multimapS>();
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    return 0;
}

