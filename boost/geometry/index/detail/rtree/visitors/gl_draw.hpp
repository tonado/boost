// Boost.Geometry Index
//
// R-tree OpenGL drawing visitor implementation
//
// Copyright (c) 2011-2013 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_INDEX_DETAIL_RTREE_VISITORS_GL_DRAW_HPP
#define BOOST_GEOMETRY_INDEX_DETAIL_RTREE_VISITORS_GL_DRAW_HPP

#include <boost/geometry/index/detail/indexable.hpp>

namespace boost { namespace geometry { namespace index {

namespace detail { namespace rtree { namespace visitors {

namespace dispatch {

template <typename Point, size_t Dimension>
struct gl_draw_point
{};

template <typename Point>
struct gl_draw_point<Point, 2>
{
    static inline void apply(Point const& p, typename index::detail::traits::coordinate_type<Point>::type z)
    {
        glBegin(GL_POINT);
        glVertex3f(geometry::get<0>(p), geometry::get<1>(p), z);
        glEnd();
    }
};

template <typename Box, size_t Dimension>
struct gl_draw_box
{};

template <typename Box>
struct gl_draw_box<Box, 2>
{
    static inline void apply(Box const& b, typename index::detail::traits::coordinate_type<Box>::type z)
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(geometry::get<min_corner, 0>(b), geometry::get<min_corner, 1>(b), z);
        glVertex3f(geometry::get<max_corner, 0>(b), geometry::get<min_corner, 1>(b), z);
        glVertex3f(geometry::get<max_corner, 0>(b), geometry::get<max_corner, 1>(b), z);
        glVertex3f(geometry::get<min_corner, 0>(b), geometry::get<max_corner, 1>(b), z);
        glEnd();
    }
};

template <typename Indexable, typename Tag>
struct gl_draw_indexable
{
};

template <typename Indexable>
struct gl_draw_indexable<Indexable, box_tag>
{
    static const size_t dimension = index::detail::traits::dimension<Indexable>::value;

    static inline void apply(Indexable const& i, typename index::detail::traits::coordinate_type<Indexable>::type z)
    {
        gl_draw_box<Indexable, dimension>::apply(i, z);
    }
};

template <typename Indexable>
struct gl_draw_indexable<Indexable, point_tag>
{
    static const size_t dimension = index::detail::traits::dimension<Indexable>::value;

    static inline void apply(Indexable const& i, typename index::detail::traits::coordinate_type<Indexable>::type z)
    {
        gl_draw_point<Indexable, dimension>::apply(i, z);
    }
};

} // namespace dispatch

namespace detail {

template <typename Indexable>
inline void gl_draw_indexable(Indexable const& i, typename index::detail::traits::coordinate_type<Indexable>::type z)
{
    dispatch::gl_draw_indexable<
        Indexable,
        typename index::detail::traits::tag<Indexable>::type
    >::apply(i, z);
}

} // namespace detail

template <typename Value, typename Options, typename Translator, typename Box, typename Allocators>
struct gl_draw : public rtree::visitor<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag, true>::type
{
    typedef typename rtree::internal_node<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type internal_node;
    typedef typename rtree::leaf<Value, typename Options::parameters_type, Box, Allocators, typename Options::node_tag>::type leaf;

    inline gl_draw(Translator const& t,
                   size_t level_first = 0,
                   size_t level_last = (std::numeric_limits<size_t>::max)(),
                   typename index::detail::traits::coordinate_type<Box>::type z_coord_level_multiplier = 1
    )
        : tr(t)
        , level_f(level_first)
        , level_l(level_last)
        , z_mul(z_coord_level_multiplier)
        , level(0)
    {}

    inline void operator()(internal_node const& n)
    {
        typedef typename rtree::elements_type<internal_node>::type elements_type;
        elements_type const& elements = rtree::elements(n);

        if ( level_f <= level )
        {
            size_t level_rel = level - level_f;

            if ( level_rel == 0 )
                glColor3f(0.75f, 0.0f, 0.0f);
            else if ( level_rel == 1 )
                glColor3f(0.0f, 0.75f, 0.0f);
            else if ( level_rel == 2 )
                glColor3f(0.0f, 0.0f, 0.75f);
            else if ( level_rel == 3 )
                glColor3f(0.75f, 0.75f, 0.0f);
            else if ( level_rel == 4 )
                glColor3f(0.75f, 0.0f, 0.75f);
            else if ( level_rel == 5 )
                glColor3f(0.0f, 0.75f, 0.75f);
            else
                glColor3f(0.5f, 0.5f, 0.5f);

            for (typename elements_type::const_iterator it = elements.begin();
                it != elements.end(); ++it)
            {
                detail::gl_draw_indexable(it->first, level_rel * z_mul);
            }
        }
        
        size_t level_backup = level;
        ++level;

        if ( level < level_l )
        {
            for (typename elements_type::const_iterator it = elements.begin();
                it != elements.end(); ++it)
            {
                rtree::apply_visitor(*this, *it->second);
            }
        }

        level = level_backup;
    }

    inline void operator()(leaf const& n)
    {
        typedef typename rtree::elements_type<leaf>::type elements_type;
        elements_type const& elements = rtree::elements(n);

        if ( level_f <= level )
        {
            size_t level_rel = level - level_f;

            glColor3f(0.25f, 0.25f, 0.25f);

            for (typename elements_type::const_iterator it = elements.begin();
                it != elements.end(); ++it)
            {
                detail::gl_draw_indexable(tr(*it), level_rel * z_mul);
            }
        }
    }

    Translator const& tr;
    size_t level_f;
    size_t level_l;
    typename index::detail::traits::coordinate_type<Box>::type z_mul;

    size_t level;
};

}}} // namespace detail::rtree::visitors

template <typename Value, typename Options, typename IndexableGetter, typename EqualTo, typename Allocator>
void gl_draw(rtree<Value, Options, IndexableGetter, EqualTo, Allocator> const& tree,
             size_t level_first = 0,
             size_t level_last = (std::numeric_limits<size_t>::max)(),
             typename index::detail::traits::coordinate_type<
                    typename rtree<Value, Options, IndexableGetter, EqualTo, Allocator>::box_type
                >::type z_coord_level_multiplier = 1
             )
{
    typedef rtree<Value, Options, IndexableGetter, EqualTo, Allocator> rtree_type;

    typedef typename rtree_type::value_type value_type;
    typedef typename rtree_type::options_type options_type;
    typedef typename rtree_type::translator_type translator_type;
    typedef typename rtree_type::box_type box_type;
    typedef typename rtree_type::allocators_type allocators_type;

    if ( !tree.empty() )
    {
        glColor3f(0.75f, 0.75f, 0.75f);
        detail::rtree::visitors::detail::gl_draw_indexable(tree.bounds(), 0);
    }

    detail::rtree::visitors::gl_draw<value_type, options_type, translator_type, box_type, allocators_type>
        gl_draw_v(tree.translator(), level_first, level_last, z_coord_level_multiplier);

    tree.apply_visitor(gl_draw_v);
}

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_INDEX_DETAIL_RTREE_VISITORS_GL_DRAW_HPP
