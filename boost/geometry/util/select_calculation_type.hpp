// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_UTIL_SELECT_CALCULATION_TYPE_HPP
#define BOOST_GEOMETRY_UTIL_SELECT_CALCULATION_TYPE_HPP


#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>

#include <boost/geometry/util/select_coordinate_type.hpp>


namespace boost { namespace geometry
{


/*!
    \brief Meta-function selecting the "calculation" type
    \details Based on two input geometry types, and an input calculation type,
        (which defaults to void in the calling function), this meta-function
        selects the most appropriate:
        - if calculation type is specified, that one is used,
        - if it is void, the most precise of the two points is used
    \ingroup utility
 */
template <typename Geometry1, typename Geometry2, typename CalculationType>
struct select_calculation_type
{
    typedef typename
        boost::mpl::if_
        <
            boost::is_void<CalculationType>,
            typename select_coordinate_type
                <
                    Geometry1,
                    Geometry2
                >::type,
            CalculationType
        >::type type;
};


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_UTIL_SELECT_CALCULATION_TYPE_HPP
