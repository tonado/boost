// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2011 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_ALGORITHMS_DIFFERENCE_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DIFFERENCE_HPP

#include <algorithm>

#include <boost/geometry/algorithms/intersection_inserter.hpp>

namespace boost { namespace geometry
{


/*!
\brief_calc2{difference} \brief_strategy
\ingroup difference
\details \details_calc2{difference_inserter, spatial set theoretic difference}
    \brief_strategy. \details_inserter{difference}
\tparam GeometryOut output geometry type, must be specified
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam OutputIterator output iterator
\tparam Strategy \tparam_strategy_overlay
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param out \param_out{difference}
\param strategy \param_strategy{difference}
\return \return_out

\qbk{distinguish,with strategy}
*/
template
<
    typename GeometryOut,
    typename Geometry1,
    typename Geometry2,
    typename OutputIterator,
    typename Strategy
>
inline OutputIterator difference_inserter(Geometry1 const& geometry1,
            Geometry2 const& geometry2, OutputIterator out,
            Strategy const& strategy)
{
    concept::check<Geometry1 const>();
    concept::check<Geometry2 const>();
    concept::check<GeometryOut>();

    return detail::intersection::inserter<GeometryOut, true, overlay_difference>(
            geometry1, geometry2,
            out,
            strategy);
}

/*!
\brief_calc2{difference}
\ingroup difference
\details \details_calc2{difference_inserter, spatial set theoretic difference}.
    \details_inserter{difference}
\tparam GeometryOut output geometry type, must be specified
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam OutputIterator output iterator
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param out \param_out{difference}
\return \return_out

\qbk{[include reference/algorithms/difference_inserter.qbk]}
*/
template
<
    typename GeometryOut,
    typename Geometry1,
    typename Geometry2,
    typename OutputIterator
>
inline OutputIterator difference_inserter(Geometry1 const& geometry1,
            Geometry2 const& geometry2, OutputIterator out)
{
    concept::check<Geometry1 const>();
    concept::check<Geometry2 const>();
    concept::check<GeometryOut>();

    typedef strategy_intersection
        <
            typename cs_tag<GeometryOut>::type,
            Geometry1,
            Geometry2,
            typename geometry::point_type<GeometryOut>::type
        > strategy;

    return difference_inserter<GeometryOut>(geometry1, geometry2,
            out, strategy());
}


/*!
\brief_calc2{difference}
\ingroup difference
\details \details_calc2{difference, spatial set theoretic difference}.
\tparam Geometry1 \tparam_geometry
\tparam Geometry2 \tparam_geometry
\tparam Collection \tparam_output_collection
\param geometry1 \param_geometry
\param geometry2 \param_geometry
\param output_collection the output collection

\qbk{[include reference/algorithms/difference.qbk]}
*/
template
<
    typename Geometry1,
    typename Geometry2,
    typename Collection
>
inline void difference(Geometry1 const& geometry1,
            Geometry2 const& geometry2, Collection& output_collection)
{
    concept::check<Geometry1 const>();
    concept::check<Geometry2 const>();

    typedef typename boost::range_value<Collection>::type geometry_out;
    concept::check<geometry_out>();

    difference_inserter<geometry_out>(
            geometry1, geometry2,
            std::back_inserter(output_collection));
}


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_ALGORITHMS_DIFFERENCE_HPP
