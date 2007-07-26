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
#include <boost/type_traits/remove_cv.hpp>
#include <boost/act/interlocked/semantics.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/integer/selection.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl_meta.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_OP                       \
BOOST_PP_TUPLE_ELEM                                                            \
( 4, 2                                                                         \
, BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO                           \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_ADDITIVE              \
BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_AFFIX_             \
            , BOOST_PP_TUPLE_ELEM                                              \
              ( 4, 3                                                           \
              , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO             \
              )                                                                \
            )

// ToDo: Change to include only appropriate semantic version
#include <boost/act/interlocked/modify.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/act/interlocked/detail/forwarder.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_INFO                            \
( BOOST_PP_TUPLE_ELEM( 4, 0                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO      \
                     )                                                         \
, BOOST_PP_TUPLE_ELEM( 4, 1                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_INFO      \
                     )                                                         \
)

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_BEGIN()

namespace boost { namespace act { namespace interlocked {

namespace detail
{

template< typename OperandType >
class BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
                  , _unaligned_fun_object
                  )
{
public:
  explicit
  BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
              , _unaligned_fun_object
              )( OperandType const& operand_init )
    : operand_m( operand_init ) {}
  

  template< typename LeftType >
  typename remove_cv< LeftType >::type
  operator ()( LeftType& left ) const
  {
    return left BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_OP operand_m;
  }
private:
  OperandType const& operand_m;
};

struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
                   , _unaligned_impl
                   )
{
  template< typename LeftType, typename OperandType >
  static typename remove_cv< LeftType >::type
  execute( LeftType& left, OperandType const& operand )
  {
    return modify< BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SEMANTICS >
           ( left
           , BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_FULL_NAME
                         , _unaligned_fun_object
                         )< OperandType >
                         ( operand )
           );
  }
};

}


template< typename Semantics, typename TargetType, typename OperandType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SEMANTICS >
  , detail::
#if BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_ADDITIVE
    are_valid_additive_params
#else
    are_valid_arithmetic_params
#endif
    < TargetType, OperandType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_SHORT_NAME( TargetType& target
                                                 , OperandType const& operand
                                                 )
{
  return detail::impl_meta
         <
           detail::BOOST_PP_CAT
                   ( BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_FULL_NAME
                   , _impl
                   )
         , TargetType
         >
         ::execute( target, operand );
}

} } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_FORWARDER_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_FORWARDER_END()

#endif
