// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/multimap_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::multimap.

#ifndef BOOST_BIMAP_VIEWS_MULTIMAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MULTIMAP_VIEW_HPP

#include <boost/bimap/container_adaptor/multimap_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with std::multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multimap.

See also const_multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class multimap_view
:
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        multimap_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    ),
    public ::boost::bimap::detail::map_view_base< multimap_view<Tag,BimapType>,Tag,BimapType >

{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        multimap_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(multimap_view,Tag,BimapType);

    public:

    multimap_view(typename base_::base_type & c)
        : base_(c) {}

    BOOST_BIMAP_MAP_VIEW_RANGE_IMPLEMENTATION(base_)

    multimap_view & operator=(const multimap_view & v) { this->base() = v.base(); return *this; }

    // Change the insert functions

    template <class InputIterator>
    void insert(InputIterator iterBegin, InputIterator iterEnd)
    {
        for( ; iterBegin != iterEnd ; ++iterBegin )
        {
            this->base().insert( this->template functor<typename base_::value_to_base>()(
                typename base_::value_type(*iterBegin)) );
        }
    }

    std::pair<typename base_::iterator, bool> insert(
        typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        std::pair< typename base_::base_type::iterator, bool > r(
            this->base().insert( this->template functor<typename base_::value_to_base>()(x) )
        );

        return std::pair<typename base_::iterator, bool>(
            this->template functor<typename base_::iterator_from_base>()(r.first),r.second
        );
    }

    typename base_::iterator insert(typename base_::iterator pos, 
        typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        return this->template functor<typename base_::iterator_from_base>()(
            this->base().insert(this->template functor<typename base_::iterator_to_base>()(pos),
            this->template functor<typename base_::value_to_base>()(x))
        );
    }
};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

