// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//


#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>

void check_array()
{
    using namespace std;
    using namespace boost;
    using namespace boost::assign;

    typedef array<float,6> Array;


#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)    
    Array a = (list_of(1),2,3,4,5,6).to_array(a);
#else
    Array a = (list_of(1),2,3,4,5,6);
#endif

    BOOST_CHECK_EQUAL( a[0], 1 );
    BOOST_CHECK_EQUAL( a[5], 6 );
    // last element is implicitly 0
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)    
    Array a2 = (list_of(1),2,3,4,5).to_array(a2);
#else
    Array a2 = (list_of(1),2,3,4,5);
#endif 
    BOOST_CHECK_EQUAL( a2[5], 0 );
    // two last elements are implicitly
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)    
    a2 = (list_of(1),2,3,4).to_array(a2);
#else    
    a2 = (list_of(1),2,3,4);
#endif    
    BOOST_CHECK_EQUAL( a2[4], 0 );
    BOOST_CHECK_EQUAL( a2[5], 0 );
    // too many arguments
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)    
   BOOST_CHECK_THROW( a2 = list_of(1)(2)(3)(4)(5)(6)(6).to_array(a2);
#else
    BOOST_CHECK_THROW( a2 = list_of(1)(2)(3)(4)(5)(6)(7), list_exception );
#endif
    
        
}

#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_array ) );

    return test;
}

