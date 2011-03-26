// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2011 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Quickbook Example

//[make_envelope
//` Shows how to return the envelope of a ring

#include <iostream>


#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <boost/assign.hpp>

/*<-*/ #include "create_svg_two.hpp" /*->*/

int main()
{
    using namespace boost::assign;

    typedef boost::geometry::model::d2::point_xy<double> point;

    boost::geometry::model::ring<point> ring;
    ring +=
        point(4.0, -0.5), point(3.5, 1.0),
        point(2.0, 1.5), point(3.5, 2.0),
        point(4.0, 3.5), point(4.5, 2.0),
        point(6.0, 1.5), point(4.5, 1.0),
        point(4.0, -0.5);

    typedef boost::geometry::model::box<point> box;

    std::cout
        << "make_envelope:"
        << boost::geometry::dsv(boost::geometry::make_envelope<box>(ring))
        << std::endl;

    /*<-*/ create_svg("make_envelope.svg", ring, boost::geometry::make_envelope<box>(ring)); /*->*/
    return 0;
}

//]


//[make_envelope_output
/*`
Output:
[pre
make_envelope:((2, -0.5), (6, 3.5))

[$img/algorithms/make_envelope.png]
]
*/
//]

