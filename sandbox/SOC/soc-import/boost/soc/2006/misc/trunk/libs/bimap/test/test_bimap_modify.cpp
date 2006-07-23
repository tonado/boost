// Boost.Bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/test for the library home page.

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost.Bimap
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>

void test_bimap_modify()
{
    using namespace boost::bimap;

    typedef bimap<int,double> bm;

    bm b;
    b.insert( bm::relation(1,0.1) );

    BOOST_CHECK( b.left[1] == 0.1 );

    b.left.replace( b.left.begin(), bm::relation::left_pair(1,0.15) );

    BOOST_CHECK( b.left[1] == 0.15 );

    b.left .modify( b.left.begin() , _first = 2 );
    b.left .modify( b.left.begin() , _second = 0.1 );
    b.right.modify( b.right.begin(), ( _first = 0.1, _second = 1 ) );

    BOOST_CHECK( b.left[1] == 0.1 );

    b.left [  1] = 0.2;
    b.right[0.2] =   2;

    BOOST_CHECK( b.left[2] == 0.2 );

    BOOST_CHECK( b.size() == 1 );

    b.replace( b.begin(), bm::relation( 3, 0.3 ) );

    BOOST_CHECK( b.left[3] == 0.3 );

    b.modify( b.begin(), ( _left = 2, _right = 0.2 ) );

    BOOST_CHECK( b.left[2] == 0.2 );

    b.left [  1] = 0.1;

    BOOST_CHECK( b.right[0.1] == 1 );
    BOOST_CHECK( b.size() == 2 );
}


int test_main( int, char* [] )
{
    test_bimap_modify();
    return 0;
}

