
#ifndef BOOST_MPL_AUX_ORDER_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_ORDER_IMPL_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2003-2004
// Copyright (c) David Abrahams 2003-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/order_fwd.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/aux_/static_cast.hpp>
#include <boost/mpl/aux_/type_wrapper.hpp>
#include <boost/mpl/aux_/ptr_to_ref.hpp>
#include <boost/mpl/aux_/traits_lambda_spec.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/static_constant.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

namespace boost {
namespace mpl {

// default implementation; requires 'Seq' to provide corresponding overloads 
// of 'operator/'

template< typename Seq, typename Key > struct x_order_impl
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300) || BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x561))
{
    BOOST_STATIC_CONSTANT(long, value = 
          sizeof( 
              *BOOST_MPL_AUX_STATIC_CAST(Seq*, 0)
                / BOOST_MPL_AUX_STATIC_CAST(aux::type_wrapper<Key>*, 0)
            )
        );

#   if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x561))
    typedef long_<(x_order_impl::value)> type;
#   else
    typedef long_<value> type;
#   endif

#else // ISO98 C++
    : long_< 
          sizeof( 
              aux::ptr_to_ref(BOOST_MPL_AUX_STATIC_CAST(Seq*, 0))
                / BOOST_MPL_AUX_STATIC_CAST(aux::type_wrapper<Key>*, 0)
            )
        >
{
#endif
};

template< typename Tag >
struct order_impl
{
    template< typename Seq, typename Key > struct apply
        : if_< 
              typename has_key_impl<Tag>::template apply<Seq,Key>
            , x_order_impl<Seq,Key>
            , void_
            >::type
    {
    };
};

BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(2,order_impl)

}}

#endif // BOOST_MPL_AUX_ORDER_IMPL_HPP_INCLUDED
