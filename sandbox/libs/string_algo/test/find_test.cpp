//  Boost string_algo library substr_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <boost/string_algo/find.hpp>
#include <boost/string_algo/classification.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;
namespace sa=boost::string_algo;

void find_test()
{
    string str1("123abcxXxabcXxXabc321");
    string str2("abc");
    string str3("");
    char* pch1="123abcxxxabcXXXabc321";
    vector<int> vec1( str1.begin(), str1.end() );

    // find results ------------------------------------------------------------//
    iterator_range<string::iterator> nc_result;
    iterator_range<string::const_iterator> cv_result;
    
    iterator_range<vector<int>::iterator> nc_vresult;
    iterator_range<vector<int>::const_iterator> cv_vresult;

    iterator_range<const char*> ch_result;

    // basic tests ------------------------------------------------------------//


    // find_first
    BOOST_CHECKPOINT( "find_first" );

    nc_result=find_first( str1, string("abc") );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_first( const_cast<const string&>(str1), str2 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    cv_result=ifind_first( const_cast<const string&>(str1), "xXX" );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 6) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 9) );

    ch_result=find_first( pch1, "abc" );
    BOOST_CHECK(( (ch_result.begin() - pch1 ) == 3) && ( (ch_result.end() - pch1 ) == 6 ) );

    // find_last
    BOOST_CHECKPOINT( "find_last" );
    
    nc_result=find_last( str1, string("abc") );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 18) );

    cv_result=find_last( const_cast<const string&>(str1), str2 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 18) );

    cv_result=ifind_last( const_cast<const string&>(str1), "XXx" );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 12) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 15) );

    ch_result=find_last( pch1, "abc" );
    BOOST_CHECK(( (ch_result.begin() - pch1 ) == 15) && ( (ch_result.end() - pch1 ) == 18 ) );

    // find_nth
    BOOST_CHECKPOINT( "find_nth" );

    nc_result=find_nth( str1, string("abc"), 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 9) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 12) );

    cv_result=find_nth( const_cast<const string&>(str1), str2, 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 9) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 12) );
        
    cv_result=ifind_nth( const_cast<const string&>(str1), "xxx", 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 12) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 15) );

    ch_result=find_nth( pch1, "abc", 1 );
    BOOST_CHECK(( (ch_result.begin() - pch1 ) == 9) && ( (ch_result.end() - pch1 ) == 12 ) );

    // find_head
    BOOST_CHECKPOINT( "find_head" );

    nc_result=find_head( str1, 6 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 0) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_head( const_cast<const string&>(str1), 6 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 0) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    ch_result=find_head( pch1, 6 );
    BOOST_CHECK( ( (ch_result.begin() - pch1 ) == 0 ) && ( (ch_result.end() - pch1 ) == 6 ) );

    // find_tail
    BOOST_CHECKPOINT( "find_tail" );

    nc_result=find_tail( str1, 6 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 21) );

    cv_result=find_tail( const_cast<const string&>(str1), 6 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 21) );

    ch_result=find_tail( pch1, 6 );
    BOOST_CHECK( ( (ch_result.begin() - pch1 ) == 15 ) && ( (ch_result.end() - pch1 ) == 21 ) );

    // find_token
    BOOST_CHECKPOINT( "find_token" );

    nc_result=find_token( str1, is_any_of<char>("abc") );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_token( const_cast<const string&>(str1), is_any_of<char>("abc") );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    nc_result=find_token( str1, is_any_of<char>("abc"), false );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 4) );

    cv_result=find_token( const_cast<const string&>(str1), is_any_of<char>("abc"), false );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 4) );

    ch_result=find_token( pch1, is_any_of<char>("abc"),false );
    BOOST_CHECK( ( (ch_result.begin() - pch1 ) == 3 ) && ( (ch_result.end() - pch1 ) == 4 ) );

    // generic find
    BOOST_CHECKPOINT( "generic find" );

    nc_result=sa::find(str1, sa::first_finder(string("abc")));
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=sa::find(const_cast<const string&>(str1), sa::first_finder(str2) );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    // multi-type comparison test 
    BOOST_CHECKPOINT( "multi-type" );

    nc_vresult=find_first( vec1, string("abc") );
    BOOST_CHECK( 
        (distance<vector<int>::const_iterator>( vec1.begin(),nc_vresult.begin()) == 3) &&
        (distance<vector<int>::const_iterator>( vec1.begin(),nc_vresult.end()) == 6) );

    cv_vresult=find_first( const_cast<const vector<int>&>(vec1), str2 );
    BOOST_CHECK( 
        (distance<vector<int>::const_iterator>( vec1.begin(),cv_vresult.begin()) == 3) &&
        (distance<vector<int>::const_iterator>( vec1.begin(),cv_vresult.end()) == 6) );

    // overflow test
    BOOST_CHECKPOINT( "overflow" );
    
    nc_result=find_first( str2, string("abcd") );
    BOOST_CHECK( nc_result.begin()==nc_result.end() );
    cv_result=find_first( const_cast<const string&>(str2), string("abcd") );
    BOOST_CHECK( cv_result.begin()==cv_result.end() );

    cv_result=find_head( const_cast<const string&>(str2), 4 );
    BOOST_CHECK( string( cv_result.begin(), cv_result.end() )== string("abc") );
    cv_result=find_tail( const_cast<const string&>(str2), 4 );
    BOOST_CHECK( string( cv_result.begin(), cv_result.end() )== string("abc") );

    // Empty string test
    BOOST_CHECKPOINT( "empty" );
    
    nc_result=find_first( str3, string("abcd") );
    BOOST_CHECK( nc_result.begin()==nc_result.end() );
    nc_result=find_first( str1, string("") );
    BOOST_CHECK( nc_result.begin()==nc_result.end() );

    cv_result=find_first( const_cast<const string&>(str3), string("abcd") );
    BOOST_CHECK( cv_result.begin()==cv_result.end() );
    cv_result=find_first( const_cast<const string&>(str1), string("") );
    BOOST_CHECK( cv_result.begin()==cv_result.end() ); 
}

// test main 
int test_main( int, char*[] )
{
    find_test();
    
    return 0;
}
