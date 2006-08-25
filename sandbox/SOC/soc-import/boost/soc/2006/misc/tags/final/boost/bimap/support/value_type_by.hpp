// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file support/value_type_by.hpp
/// \brief Metafunction to access the value types (std::pair compatibles) of a bimap

#ifndef BOOST_BIMAP_SUPPORT_VALUE_TYPE_BY_HPP
#define BOOST_BIMAP_SUPPORT_VALUE_TYPE_BY_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct boost::bimap::support::value_type_by

\brief Metafunction to obtain the value type of a bimap indexed by one of the sides.

The tag parameter can be either a user defined tag or \c member_at::{side}.
The returned type is compatible with std::pair.

\code

template< class Tag, class Bimap >
struct value_type_by
{
    typedef typename Bimap::{side}_value_type type;
};

\endcode

See also member_at.
\ingroup bimap_group
                                                                    **/

namespace boost {
namespace bimap {
namespace support {

// Implementation of value type by metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    value_type_by,
    left_value_type,
    right_value_type
);



} // namespace support
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_SUPPORT_VALUE_TYPE_BY_HPP

