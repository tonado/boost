// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/vector_set_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::vector.

#ifndef BOOST_BIMAP_VIEWS_VECTOR_SET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_VECTOR_SET_VIEW_HPP

#include <boost/bimap/container_adaptor/vector_adaptor.hpp>
#include <boost/bimap/detail/set_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::vector.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::vector.

See also const_set_view.
                                                                                    **/

template< class IndexType >
class vector_set_view
:
    public ::boost::bimap::container_adaptor::vector_adaptor
    <
        IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator
    >,

    public ::boost::bimap::detail::set_view_base< vector_set_view< IndexType >, IndexType >
{
    BOOST_BIMAP_SET_VIEW_BASE_FRIEND(vector_set_view,IndexType);

    typedef ::boost::bimap::container_adaptor::vector_adaptor
    <
        IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator

    > base_;

    public:

    vector_set_view(typename base_::base_type & c) :
        base_(c) {}
};

/// \brief Constant view of a bimap that is signature compatible with std::vector.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::vector.

See also set_view.
                                                                                    **/


template< class IndexType >
class const_vector_set_view
:
    public ::boost::bimap::container_adaptor::vector_adaptor
    <
        const IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator
    >
{
    typedef ::boost::bimap::container_adaptor::vector_adaptor
    <
        const IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator

    > base_;

    public:

    const_vector_set_view(typename base_::base_type & c) :
        base_(c) {}

};


} // namespace views
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_VIEWS_VECTOR_SET_VIEW_HPP

