// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/vector_map_view.hpp
/// \brief View of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP

#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/vector_map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core.

See also const_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class vector_map_view
:
    public container_adaptor::vector_map_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,

        typename bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    typedef vector_map_view this_type;

    public:

    vector_map_view() {}
    vector_map_view(typename this_type::base_type & c) :
        this_type::vector_map_adaptor_(c) {}

};

/// \brief Constant view of a side of a bimap that is signature compatible with std::map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::map.

See also map_view.
                                                                                    **/
template< class Tag, class BimapType >
class const_vector_map_view
:
    public container_adaptor::vector_map_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    public:

    const_vector_map_view() {}
    const_vector_map_view(typename const_vector_map_view::base_type & c) :
        const_vector_map_view::vector_map_adaptor_(c) {}
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_VECTOR_MAP_VIEW_HPP

