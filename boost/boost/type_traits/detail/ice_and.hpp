// (C) Copyright John Maddock and Steve Cleary 2000.
//
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_DETAIL_ICE_AND_HPP_INCLUDED
#define BOOST_TT_DETAIL_ICE_AND_HPP_INCLUDED

#include "boost/config.hpp"

namespace boost {
namespace type_traits {

template <bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true>
struct ice_and;

template <bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7>
struct ice_and
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};

template <>
struct ice_and<true, true, true, true, true, true, true>
{
    BOOST_STATIC_CONSTANT(bool, value = true);
};

} // namespace type_traits
} // namespace boost

#endif // BOOST_TT_DETAIL_ICE_AND_HPP_INCLUDED
