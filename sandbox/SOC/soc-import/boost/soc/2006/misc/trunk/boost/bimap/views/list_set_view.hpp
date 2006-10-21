// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/list_set_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::list.

#ifndef BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP

#include <boost/config.hpp>

#include <boost/bimap/container_adaptor/list_adaptor.hpp>
#include <boost/bimap/detail/set_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::list.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::list.

See also const_list_set_view.
                                                                                    **/

template< class CoreIndex >
class list_set_view
:
    public BOOST_BIMAP_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(
        list_adaptor,
        CoreIndex,
        reverse_iterator, const_reverse_iterator
    ),

    public ::boost::bimap::detail::set_view_base< list_set_view< CoreIndex >, CoreIndex >
{
    BOOST_BIMAP_SET_VIEW_BASE_FRIEND(list_set_view,CoreIndex);

    typedef BOOST_BIMAP_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(
        list_adaptor,
        CoreIndex,
        reverse_iterator, const_reverse_iterator

    ) base_;

    public:

    list_set_view(typename base_::base_type & c) :
        base_(c) {}

    list_set_view & operator=(const list_set_view & v) { this->base() = v.base(); return *this; }

    BOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION
};

/// \brief Constant view of a bimap that is signature compatible with std::set.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::list.

See also set_view.
                                                                                    **/


template< class CoreIndex >
class const_list_set_view
:
    public BOOST_BIMAP_CONST_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(
        list_adaptor,
        CoreIndex,
        const_reverse_iterator
    )
{
    typedef BOOST_BIMAP_CONST_SEQUENCED_SET_VIEW_CONTAINER_ADAPTOR(
        list_adaptor,
        CoreIndex,
        const_reverse_iterator

    ) base_;

    public:

    const_list_set_view(typename base_::base_type & c) :
        base_(c) {}
};


} // namespace views
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP

