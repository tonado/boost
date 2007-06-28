/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_SUBTRACT_ASSIGN_SUBTRACT_ASSIGN_HPP
#define BOOST_ACT_INTERLOCKED_SUBTRACT_ASSIGN_SUBTRACT_ASSIGN_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_CONFIG_INTERLOCKED_HAS( subtract_assign, acq_rel )

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/act/interlocked/subtract_assign/subtract_assign_acq_rel.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename SourceType >
typename lazy_enable_if
<
  detail::are_valid_additive_params< TargetType, SourceType const >
, remove_cv< TargetType >
>
::type
subtract_assign( TargetType& destination, SourceType const& new_value )
{
  return interlocked::subtract_assign_acq_rel( destination, new_value );

}

} } }

#endif

#endif
