/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <iostream>
#include <cassert>

using namespace std;

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>
using namespace boost::spirit;

///////////////////////////////////////////////////////////////////////////////
//
//  Rule tests
//
///////////////////////////////////////////////////////////////////////////////
void
aliasing_tests()
{
    rule<>  a = ch_p('a');
    rule<>  b = ch_p('b');
    rule<>  c = ch_p('c');

    BOOST_SPIRIT_DEBUG_RULE(a);
    BOOST_SPIRIT_DEBUG_RULE(b);
    BOOST_SPIRIT_DEBUG_RULE(c);

    rule<>  start;
    BOOST_SPIRIT_DEBUG_RULE(start);

    rule<>  d;
    d = start;  // aliasing

    parse_info<char const*> pi;

    start = *(a | b | c);
    pi = parse("abcabcacb", d);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 9);
    assert(*pi.stop == 0);

    start   = (a | b) >> (start | b);
    pi = parse("aaaabababaaabbb", d);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 15);
    assert(*pi.stop == 0);
}

void
rule_tests()
{
    rule<>  a = ch_p('a');
    rule<>  b = ch_p('b');
    rule<>  c = ch_p('c');

    BOOST_SPIRIT_DEBUG_RULE(a);
    BOOST_SPIRIT_DEBUG_RULE(b);
    BOOST_SPIRIT_DEBUG_RULE(c);

    parse_info<char const*> pi;

    rule<>  start = *(a | b | c);

    BOOST_SPIRIT_DEBUG_RULE(start);

    pi = parse("abcabcacb", start);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 9);
    assert(*pi.stop == 0);

    start   = (a | b) >> (start | b);
    pi = parse("aaaabababaaabbb", start);
    assert(pi.hit);
    assert(pi.full);
    assert(pi.length == 15);
    assert(*pi.stop == 0);

    pi = parse("aaaabababaaabba", start);
    assert(pi.hit);
    assert(!pi.full);
    assert(pi.length == 14);

    aliasing_tests();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Main
//
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    rule_tests();
    cout << "Tests concluded successfully\n";
    return 0;
}

