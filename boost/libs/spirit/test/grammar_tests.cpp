/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 2001-2003 Joel de Guzman
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

//////////////////////////////////////////////////////////////////////////////
//
//  Grammar tests
//
///////////////////////////////////////////////////////////////////////////////
struct num_list : public grammar<num_list>
{
    template <typename ScannerT>
    struct definition {

        definition(num_list const& self)
        {
            num = int_p;
            r = num >> *(',' >> num);

            BOOST_SPIRIT_DEBUG_RULE(num);
            BOOST_SPIRIT_DEBUG_RULE(r);
        }

        rule<ScannerT> r, num;

        rule<ScannerT> const&
        start() const { return r; }
    };
};

void
grammar_tests()
{
    num_list nlist;
    BOOST_SPIRIT_DEBUG_GRAMMAR(nlist);

    parse_info<char const*> pi;
    pi = parse("123, 456, 789", nlist, space_p);
    assert(pi.hit);
    assert(pi.full);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Main
//
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    grammar_tests();
    cout << "Tests concluded successfully\n";
    return 0;
}

