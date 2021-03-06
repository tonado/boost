// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2013 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry_test_common.hpp>

#include <boost/geometry/extensions/nsphere/nsphere.hpp>

#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include <boost/geometry/strategies/strategies.hpp>

#include <boost/geometry/io/wkt/read.hpp>

typedef bg::model::d2::point_xy<double> point_type;
typedef bg::model::nsphere<point_type, double> circle_type;

template <typename Geometry>
void test_circle(std::string const& wkt_geometry, bool expected_within, bool expected_covered_by)
{
    circle_type circle(point_type(1.0, 1.0), 3.0);

    Geometry geometry;
    bg::read_wkt(wkt_geometry, geometry);

    bool detected = bg::within(geometry, circle);

    BOOST_CHECK_MESSAGE(detected == expected_within,
        "circle (1,1) with radius 3 containing : " << wkt_geometry
        << " -> Expected: " << expected_within
        << " detected: " << detected);

    detected = bg::covered_by(geometry, circle);

    BOOST_CHECK_MESSAGE(detected == expected_covered_by,
        "circle (1,1) with radius 3 covering : " << wkt_geometry
        << " -> Expected: " << expected_covered_by
        << " detected: " << detected);
}

void test_circles()
{
    test_circle<point_type>("POINT(1 1)", true, true);
    test_circle<point_type>("POINT(1 4)", false, true);
    test_circle<point_type>("POINT(4 1)", false, true);
    test_circle<point_type>("POINT(4 4)", false, false);
}


int test_main( int , char* [] )
{
    test_circles();

    return 0;
}
