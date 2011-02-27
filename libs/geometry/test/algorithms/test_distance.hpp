// Boost.Geometry (aka GGL, Generic Geometry Library) test file
//
// Copyright Barend Gehrels 2010, Geodan, Amsterdam, the Netherlands
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_TEST_DISTANCE_HPP
#define BOOST_GEOMETRY_TEST_DISTANCE_HPP

#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/extensions/gis/io/wkt/read_wkt.hpp>
#include <boost/geometry/strategies/strategies.hpp>


// Define a custom distance strategy
// For this one, the "taxicab" distance,
// see http://en.wikipedia.org/wiki/Taxicab_geometry

// For a point-point-distance operation, one typename Point is enough.
// For a point-segment-distance operation, there is some magic inside
// using another point type and casting if necessary. Therefore,
// two point-types are necessary.
template <typename P1, typename P2 = P1>
struct taxicab_distance
{
    static inline typename bg::coordinate_type<P1>::type apply(
                    P1 const& p1, P2 const& p2)
    {
        using bg::get;
        using bg::math::abs;
        return abs(get<0>(p1) - get<1>(p2))
            + abs(get<1>(p1) - get<1>(p2));
    }
};



namespace boost { namespace geometry { namespace strategy { namespace distance { namespace services
{

template <typename P1, typename P2>
struct tag<taxicab_distance<P1, P2> >
{
    typedef strategy_tag_distance_point_point type;
};


template <typename P1, typename P2>
struct return_type<taxicab_distance<P1, P2> >
{
    typedef typename coordinate_type<P1>::type type;
};


template<typename P1, typename P2, typename PN1, typename PN2>
struct similar_type<taxicab_distance<P1, P2>, PN1, PN2>
{
    typedef taxicab_distance<PN1, PN2> type;
};


template<typename P1, typename P2, typename PN1, typename PN2>
struct get_similar<taxicab_distance<P1, P2>, PN1, PN2>
{
    static inline typename similar_type
        <
            taxicab_distance<P1, P2>, PN1, PN2
        >::type apply(taxicab_distance<P1, P2> const& )
    {
        return taxicab_distance<PN1, PN2>();
    }
};

template <typename P1, typename P2>
struct comparable_type<taxicab_distance<P1, P2> >
{
    typedef taxicab_distance<P1, P2> type;
};

template <typename P1, typename P2>
struct get_comparable<taxicab_distance<P1, P2> >
{
    static inline taxicab_distance<P1, P2> apply(taxicab_distance<P1, P2> const& input)
    {
        return input;
    }
};

template <typename P1, typename P2>
struct result_from_distance<taxicab_distance<P1, P2> >
{
    template <typename T>
    static inline typename coordinate_type<P1>::type apply(taxicab_distance<P1, P2> const& , T const& value)
    {
        return value;
    }
};


}}}}} // namespace bg::strategy::distance::services





template <typename Geometry1, typename Geometry2>
void test_distance(Geometry1 const& geometry1,
            Geometry2 const& geometry2,
            long double expected_distance)
{
    typename bg::distance_result<Geometry1>::type distance = bg::distance(geometry1, geometry2);

#ifdef GEOMETRY_TEST_DEBUG
    std::ostringstream out;
    out << typeid(typename bg::coordinate_type<Geometry1>::type).name()
        << std::endl
        << typeid(typename bg::distance_result<Geometry1>::type).name()
        << std::endl
        << "distance : " << bg::distance(geometry1, geometry2)
        << std::endl;
    std::cout << out.str();
#endif

    BOOST_CHECK_CLOSE(distance, expected_distance, 0.0001);
}


template <typename Geometry1, typename Geometry2>
void test_geometry(std::string const& wkt1, std::string const& wkt2, double expected_distance)
{
    Geometry1 geometry1;
    bg::read_wkt(wkt1, geometry1);
    Geometry2 geometry2;
    bg::read_wkt(wkt2, geometry2);

    test_distance(geometry1, geometry2, expected_distance);
}


#endif
