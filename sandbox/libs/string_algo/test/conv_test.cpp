//  Boost string_algo library conv_test.cpp file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <iostream>
#include <boost/string_algo/case_conv.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void conv_test()
{
    string str1("AbCdEfG 123 xxxYYYzZzZ");
    string str2("AbCdEfG 123 xxxYYYzZzZ");
    string str3("");
    char* pch1=strdup("AbCdEfG 123 xxxYYYzZzZ");
    char* pch2=strdup("AbCdEfG 123 xxxYYYzZzZ");

    // *** iterator tests *** //

    string strout;
    to_lower_copy( back_inserter(strout), str1 );
    BOOST_CHECK( strout=="abcdefg 123 xxxyyyzzzz" );
    strout.clear();
    to_upper_copy( back_inserter(strout), str1 );
    BOOST_CHECK( strout=="ABCDEFG 123 XXXYYYZZZZ" );

    strout.clear();
    to_lower_copy( back_inserter(strout), "AbCdEfG 123 xxxYYYzZzZ" );
    BOOST_CHECK( strout=="abcdefg 123 xxxyyyzzzz" );
    strout.clear();
    to_upper_copy( back_inserter(strout), "AbCdEfG 123 xxxYYYzZzZ" );
    BOOST_CHECK( strout=="ABCDEFG 123 XXXYYYZZZZ" );

    strout.clear();
    to_lower_copy( back_inserter(strout), pch1 );
    BOOST_CHECK( strout=="abcdefg 123 xxxyyyzzzz" );
    strout.clear();
    to_upper_copy( back_inserter(strout), pch1 );
    BOOST_CHECK( strout=="ABCDEFG 123 XXXYYYZZZZ" );

    // *** value passing tests *** //

    BOOST_CHECK( to_lower_copy( str1 )=="abcdefg 123 xxxyyyzzzz" );
    BOOST_CHECK( to_upper_copy( str1 )=="ABCDEFG 123 XXXYYYZZZZ" );

    BOOST_CHECK( to_lower_copy( str3 )=="" );
    BOOST_CHECK( to_upper_copy( str3 )=="" );

    // *** inplace tests *** //

    to_lower( str1 );
    BOOST_CHECK( str1=="abcdefg 123 xxxyyyzzzz" );
    to_upper( str2 );
    BOOST_CHECK( str2=="ABCDEFG 123 XXXYYYZZZZ" );

    // c-string modification
    to_lower( pch1 );
    BOOST_CHECK( string(pch1)=="abcdefg 123 xxxyyyzzzz" );
    to_upper( pch2 );
    BOOST_CHECK( string(pch2)=="ABCDEFG 123 XXXYYYZZZZ" );

    to_lower( str3 );
    BOOST_CHECK( str3=="" );
    to_upper( str3 );
    BOOST_CHECK( str3=="" );

    free(pch1);
    free(pch2);
}

// test main 
int test_main( int, char*[] )
{
    conv_test();
    
    return 0;
}
