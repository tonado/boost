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
#include <boost/container_gen/selectors.hpp>
#include <boost/container_gen/container_gen.hpp>
#include <boost/container_gen/is_associative_selector.hpp>
#include <boost/container_gen/emplace_function_gen.hpp>
#include <boost/container_gen/splice_function_gen.hpp>
#include <boost/container_gen/insert_range_result_gen.hpp>
#include "type_definitions.hpp"
#include "result_range.hpp"
#include <boost/test/minimal.hpp>

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <typename Selector, typename C>
void
    test_splice_function_gen_slist(
        C& ex_seq_1
      , C& ex_seq_2
      , C& input_seq
    )
{
    typename boost::emplace_function_gen<Selector>::type emplacer;

    emplacer[ex_seq_1]
        ("able")("fox")("easy")("kilo")("baker")("charlie")("dog");
    emplacer[ex_seq_2]
        ("kilo")("able")("fox")("easy")("baker")("charlie")("dog");
    emplacer[input_seq]
        ("able")("fox")("easy");
}

template <typename C, typename A>
void
    test_splice_function_gen_init(
        C& ex_seq_1
      , C& ex_seq_2
      , C& input_seq
      , boost::slist_selector<A>
    )
{
    test_splice_function_gen_slist<
        boost::slist_selector<A>
    >(ex_seq_1, ex_seq_2, input_seq);
}

#if !defined BOOST_NO_CXX11_HDR_FORWARD_LIST
template <typename C, typename A>
void
    test_splice_function_gen_init(
        C& ex_seq_1
      , C& ex_seq_2
      , C& input_seq
      , boost::forward_list_selector<A>
    )
{
    test_splice_function_gen_slist<
        boost::forward_list_selector<A>
    >(ex_seq_1, ex_seq_2, input_seq);
}
#endif
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//[test__splice_function_gen_in_place
template <typename C, typename Selector>
void
    test_splice_function_gen_init(
        C& ex_seq_1
      , C& ex_seq_2
      , C& input_seq
      , Selector
    )
{
    typename boost::emplace_function_gen<Selector>::type emplacer;

    emplacer[ex_seq_1]
        ("able")("fox")("easy")("kilo")("baker")("charlie")("dog");
    emplacer[ex_seq_2]
        ("able")("fox")("easy")("baker")("charlie")("dog")("kilo");
    emplacer[input_seq]
        ("baker")("charlie")("dog");
}

template <typename Selector>
void test_splice_function_gen_in_place(Selector)
{
    typedef typename boost::container_gen<Selector,test_string>::type C;

    C ex_seq_1, ex_seq_2, input_seq;

    test_splice_function_gen_init(ex_seq_1, ex_seq_2, input_seq, Selector());

    C test_seq(ex_seq_1);
    typename boost::splice_function_gen<Selector>::type splicer;
    typename boost::insert_range_result_gen<Selector,test_string>::type
        insert_range_result = splicer(
            test_seq
          , ++++++test_seq.begin()
          , test_seq
          , ++++++++test_seq.begin()
          , test_seq.end()
        );

    BOOST_CHECK(boost::range::equal(insert_range_result, input_seq));
    BOOST_CHECK(boost::range::equal(test_seq, ex_seq_2));

    typename C::iterator itr = splicer(
        test_seq
      , ++++++test_seq.begin()
      , test_seq
      , ++++++++++++test_seq.begin()
    );

    BOOST_CHECK(*itr == test_string("kilo"));
    BOOST_CHECK(boost::range::equal(test_seq, ex_seq_1));
    itr = splicer(
        test_seq
      , test_seq.end()
      , test_seq
      , ++++++test_seq.begin()
    );
    BOOST_CHECK(*itr == test_string("kilo"));
    BOOST_CHECK(boost::range::equal(test_seq, ex_seq_2));
    insert_range_result = splicer(
        test_seq
      , test_seq.end()
      , test_seq
      , ++++++test_seq.begin()
      , ++++++++++++test_seq.begin()
    );
    BOOST_CHECK(boost::range::equal(insert_range_result, input_seq));
    BOOST_CHECK(boost::range::equal(test_seq, ex_seq_1));
}
//]

//[test__splice_function_gen
template <typename Selector, typename AddedContainer>
void test_splice_function_gen(AddedContainer const& added_container)
{
    test_splice_function_gen_in_place(Selector());

    typedef typename boost::container_gen<Selector,test_string>::type C;

    C ex_seq_1, ex_seq_2, test_seq_1, test_seq_2, input_seq;
    typename boost::emplace_function_gen<Selector>::type emplacer;
    typename boost::splice_function_gen<Selector>::type splicer;

    emplacer[ex_seq_1]
        ("able")("fox")("easy")("kilo")("baker")("charlie")("dog");
    emplacer[ex_seq_2]
        ("able")("fox")("easy")
        ("able")("fox")("iota")("hotel")("jupiter")("dog")
        ("baker")("charlie")("dog");
    emplacer[test_seq_1]
        ("able")("fox")("easy")("baker")("charlie")("dog");
    emplacer[input_seq]
        ("able")("fox")("iota")("kilo")("hotel")("jupiter")("dog");
    emplacer[test_seq_2]
        ("able")("fox")("iota")("hotel")("jupiter")("dog");

    C test_seq_3(test_seq_1);
    typename C::iterator itr = splicer(
        test_seq_1
      , ++++++test_seq_1.begin()
      , input_seq
      , ++++++input_seq.begin()
    );

    BOOST_CHECK(*itr == test_string("kilo"));
    BOOST_CHECK(boost::range::equal(test_seq_1, ex_seq_1));
    BOOST_CHECK(boost::range::equal(test_seq_2, input_seq));

    typename boost::insert_range_result_gen<Selector,test_string>::type
        insert_range_result = splicer(
            test_seq_3
          , ++++++test_seq_3.begin()
          , input_seq
          , input_seq.begin()
          , input_seq.end()
        );

    BOOST_CHECK(
        test_result_range(
            insert_range_result
          , added_container
          , boost::is_associative_selector<Selector>()
        )
    );
    BOOST_CHECK(input_seq.empty());
    BOOST_CHECK(boost::range::equal(test_seq_3, ex_seq_2));
}
//]

