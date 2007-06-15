/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/remove_volatile.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_ADDITIVE              \
BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_AFFIX_             \
            , BOOST_PP_TUPLE_ELEM                                              \
              ( 3, 2                                                           \
              , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO             \
              )                                                                \
            )

#include <boost/act/interlocked/detail/forwarder.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO                            \
( BOOST_PP_TUPLE_ELEM( 3, 0                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO      \
                     )                                                         \
, BOOST_PP_TUPLE_ELEM( 3, 1                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO      \
                     )                                                         \
)

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_BEGIN()

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename OperandType >
typename lazy_enable_if
<
  mpl::and_
  <
    detail::
#if BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_ADDITIVE
    are_valid_additive_params
#else
    are_valid_arithmetic_params
#endif
    < TargetType, OperandType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME
              , _result
              )
              < TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME( TargetType& target
                                                , OperandType const& operand
                                                )
{
  typedef typename remove_volatile< TargetType >::type type;
  typedef typename BOOST_PP_CAT
                   ( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME
                   , _result
                   )
                   < TargetType >
                   ::type result_type;

  return detail::BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME
                             , _impl
                             )
         < result_type
         , type
         >
         ::execute( target, operand );
}

} } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_END()

#endif
