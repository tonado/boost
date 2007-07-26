/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_THUN_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_ASSIGN_THUN_HPP

#include <boost/act/config/interlocked/has.hpp>

#include <boost/act/interlocked/semantics/thread_unsafe.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>

#include <boost/mpl/and.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics, typename TargetType, typename SourceType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, thread_unsafe >
  , detail::are_valid_store_style_params< TargetType, SourceType const >
  >
, remove_cv< TargetType >
>
::type
assign( TargetType& destination, SourceType const& new_value )
{
  typename remove_cv< TargetType >::type result = destination;
  destination = new_value;
  return result;
}

} } }

#endif
