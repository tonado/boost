/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DECREMENT_DETAIL_DECREMENT_ACQUIRE_DEF_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_DECREMENT_DETAIL_DECREMENT_ACQUIRE_DEF_IMPL_HPP

#include <boost/act/interlocked/assign_if_was/assign_if_was_acquire.hpp>
#include <boost/act/interlocked/retrieve.hpp>
#include <boost/act/detail/prior.hpp>
#include <boost/act/interlocked/decrement/detail/decrement_operation.hpp>
#include <boost/act/interlocked/detail/interlocked_result.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

struct decrement_acquire_default_impl
{
  template< typename TargetType >
  struct result
    : detail::unary_interlocked_result_returns_old< decrement_operation
                                                  , TargetType
                                                  > {};

  template< typename TargetType >
  static typename result< TargetType >::type execute( TargetType& target )
  {
    typedef typename result< TargetType >::type result_type;
    typedef typename remove_cv< TargetType >::type unqualified_type;

    unqualified_type new_value;

    for( unqualified_type curr_value = retrieve( target )
       ;    ( new_value = assign_if_was_acquire
                          ( target
                          , act::detail::prior( curr_value )
                          , curr_value
                          )
                          .old_value()
            )
         != curr_value
       ; curr_value = new_value
       );

    // Note: new_value is old value here
    return result_type( new_value );
  }
};

} } } }

#endif
