/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 2002-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A parser for summing a list of numbers.
//  [ demonstrating phoenix ]
//
//  [ JDG 6/28/2002 ]
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace boost::spirit;
using namespace phoenix;

///////////////////////////////////////////////////////////////////////////////
//
//  Our adder
//
///////////////////////////////////////////////////////////////////////////////
template <typename IteratorT>
bool adder(IteratorT first, IteratorT last, double& n)
{
    return parse(first, last,

        //  Begin grammar
        (
            real_p[var(n) = arg1] >> *real_p[var(n) += arg1]
        )
        ,
        //  End grammar

        space_p).full;
}

////////////////////////////////////////////////////////////////////////////
//
//  Main program
//
////////////////////////////////////////////////////////////////////////////
int
main()
{
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "\t\tA parser for summing a list of numbers...\n\n";
    cout << "/////////////////////////////////////////////////////////\n\n";

    cout << "Give me a space separated list of numbers.\n";
    cout << "The numbers are added using Phoenix.\n";
    cout << "Type [q or Q] to quit\n\n";

    string str;
    while (getline(cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        double n;
        if (adder(str.begin(), str.end(), n))
        {
            cout << "-------------------------\n";
            cout << "Parsing succeeded\n";
            cout << str << " Parses OK: " << endl;

            cout << "sum = " << n;
            cout << "\n-------------------------\n";
        }
        else
        {
            cout << "-------------------------\n";
            cout << "Parsing failed\n";
            cout << "-------------------------\n";
        }
    }

    cout << "Bye... :-) \n\n";
    return 0;
}


