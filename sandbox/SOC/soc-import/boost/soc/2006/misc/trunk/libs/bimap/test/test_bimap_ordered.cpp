// Boost.Bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/test for the library home page.

// std
#include <set>
#include <map>
#include <string>

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost
#include <boost/assign/list_of.hpp>

// std
#include <functional>

// Set type specifications
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

// bimap container
#include <boost/bimap/bimap.hpp>

#include <libs/bimap/test/test_bimap.hpp>

struct  left_tag {};
struct right_tag {};

void test_bimap()
{
    using namespace boost::bimap;
    using namespace boost::mpl;

    typedef std::pair<int,double> std_pair;
    std::set< std_pair > data =
        boost::assign::list_of< std_pair >
        (1,0.1) (2,0.2) (3,0.3) (4,0.4);

    typedef std::map<int,double> left_data_type;
    left_data_type left_data;
    left_data.insert( left_data_type::value_type(1,0.1) );
    left_data.insert( left_data_type::value_type(2,0.2) );
    left_data.insert( left_data_type::value_type(3,0.3) );
    left_data.insert( left_data_type::value_type(4,0.4) );

    typedef std::map<double,int> right_data_type;
    right_data_type right_data;
    right_data.insert( right_data_type::value_type(0.1,1) );
    right_data.insert( right_data_type::value_type(0.2,2) );
    right_data.insert( right_data_type::value_type(0.3,3) );
    right_data.insert( right_data_type::value_type(0.4,4) );



    //--------------------------------------------------------------------
    {
        bimap< int, double > b;

        test_set_set_bimap(b,data,left_data,right_data);
    }
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    {
        bimap
        <
            tagged< multiset_of<int>   , left_tag  >,
            tagged< multiset_of<double>, right_tag >,
            multiset_of_relation< std::less<_> >

        > b;

        test_multiset_multiset_bimap(b,data,left_data,right_data);
        test_tagged_bimap<left_tag,right_tag>(b,data);
    }
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    {
        bimap<int,double,right_based> b;

        test_set_set_bimap(b,data,left_data,right_data);
    }
    //--------------------------------------------------------------------


    //--------------------------------------------------------------------
    {
        bimap
        <
            multiset_of< int, std::greater<int> >, set_of<std::string> ,
            multiset_of_relation< std::greater<_> >

        > b;
    }
    //--------------------------------------------------------------------

}


int test_main( int, char* [] )
{
    test_bimap();
    return 0;
}

