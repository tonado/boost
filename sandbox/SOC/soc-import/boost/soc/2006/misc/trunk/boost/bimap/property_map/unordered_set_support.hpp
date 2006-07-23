// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file property_map/unordered_set_support.hpp
/// \brief Support for the property map concept.

#ifndef BOOST_BIMAP_PROPERTY_MAP_UNORDERED_SET_SUPPORT_HPP
#define BOOST_BIMAP_PROPERTY_MAP_UNORDERED_SET_SUPPORT_HPP

#include <boost/property_map.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/support/data_type_of.hpp>
#include <boost/bimap/support/key_type_of.hpp>

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {

template< class Tag, class Bimap >
struct property_traits< bimap::views::unordered_map_view<Tag,Bimap> >
{
    typedef typename bimap::support::data_type_of<Tag,Bimap>::type value_type;
    typedef typename bimap::support:: key_type_of<Tag,Bimap>::type   key_type;

    typedef readable_property_map_tag category;
};


template< class Tag, class Bimap >
const typename bimap::support::data_type_of<Tag,Bimap>::type &
    get(const bimap::views::unordered_map_view<Tag,Bimap> & m,
        const typename bimap::support::key_type_of<Tag,Bimap>::type & key)
{
    return m.find(key)->second;
}

} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_PROPERTY_MAP_UNORDERED_SET_SUPPORT_HPP
