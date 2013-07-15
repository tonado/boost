// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "splice_function_gen.hpp"
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>

//[test__splice_function_gen__main
int test_main(int argc, char** argv)
{
    boost::container_gen<boost::dequeS,test_string>::type c;
    boost::emplace_function_gen<boost::dequeS>::type emplacer;

    emplacer[c]("able")("fox")("iota")("hotel")("jupiter")("dog");
    test_splice_function_gen<boost::vecS>(c);
    test_splice_function_gen<boost::dequeS>(c);
    test_splice_function_gen<boost::listS>(c);
    test_splice_function_gen<boost::ptr_vecS>(c);
    test_splice_function_gen<boost::ptr_dequeS>(c);
    test_splice_function_gen<boost::ptr_listS>(c);
//<-
#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//->
    test_splice_function_gen<boost::vector_selector<boost::mpl::true_> >(c);
    test_splice_function_gen<boost::stable_vecS>(c);
    test_splice_function_gen<
        boost::static_vector_selector<boost::mpl::int_<16> >
    >(c);
    test_splice_function_gen<boost::deque_selector<boost::mpl::true_> >(c);
    test_splice_function_gen<boost::list_selector<boost::mpl::true_> >(c);
    c.clear();
    emplacer[c]("dog")("jupiter")("hotel")("iota")("fox")("able");
    test_splice_function_gen<boost::slistS>(c);
//<-
#if 0//!defined BOOST_NO_CXX11_HDR_FORWARD_LIST
    test_splice_function_gen<boost::forward_listS>(c);
#endif
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//->

    return 0;
}
//]

