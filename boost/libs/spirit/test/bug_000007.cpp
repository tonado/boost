/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// handling of trailing whitespace bug (ast_parse/pt_parse related)

#include <boost/test/included/unit_test_framework.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/tree/parse_tree.hpp>

using namespace boost;
using namespace unit_test_framework;
using namespace boost::spirit;

///////////////////////////////////////////////////////////////////////////////
//  test function
void bug_000007_test()
{
    BOOST_CHECK(parse("test ", str_p("test"), space_p).full);
    BOOST_CHECK(pt_parse("test ", str_p("test"), space_p).full);
    BOOST_CHECK(ast_parse("test ", str_p("test"), space_p).full);
}

///////////////////////////////////////////////////////////////////////////////
//  Boost.Test entry point
test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE("bug_000007 test");

    test->add(BOOST_TEST_CASE(&bug_000007_test));

    return test;
}

