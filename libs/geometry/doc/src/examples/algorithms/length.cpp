// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2011, Geodan, Amsterdam, the Netherlands
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Quickbook Example

//[length
    //` The following simple example shows the calculation of the length of a linestring containing three points

#include <iostream>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/extensions/gis/io/wkt/wkt.hpp>


int main()
{
    using namespace boost::geometry;
    model::linestring<model::d2::point_xy<double> > line;
    read_wkt("linestring(0 0,1 1,4 8,3 2)", line);
    std::cout << "linestring length is "
        << length(line)
        << " units" << std::endl;

    return 0;
}

//]


//[length_output
/*`
Output:
[pre
linestring length is 15.1127 units
]
*/
//]
