// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/support/value_type_of.hpp
/// \brief value_type_of<tag,relation> metafunction

#ifndef BOOST_BIMAP_RELATION_SUPPORT_VALUE_TYPE_OF_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_VALUE_TYPE_OF_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct boost::bimap::relation::support::value_type_of

\brief Metafunction to obtain the value type of one of the sides.

\code

template< class Tag, class Relation >
struct value_type_of
{
        typedef typename Relation::{side}_type type;
};

\endcode

See also member_at, get().
\ingroup relation_group
                                                                    **/

namespace boost {
namespace bimap {
namespace relation {
namespace support {

// Metafunction value_type_of
/*

    template< class Tag, class Relation >
    struct value_type_of
    {
        typedef -unspecified- type;
    };

*/

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    value_type_of,
    left_value_type,
    right_value_type
);

} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_SUPPORT_VALUE_TYPE_OF_HPP

