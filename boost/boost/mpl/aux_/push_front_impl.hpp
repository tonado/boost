
#ifndef BOOST_MPL_AUX_PUSH_FRONT_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_PUSH_FRONT_IMPL_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/push_front_fwd.hpp>
#include <boost/mpl/aux_/has_type.hpp>
#include <boost/mpl/aux_/traits_lambda_spec.hpp>

namespace boost { namespace mpl {

// agurt 05/feb/04: no default implementation; the stub definition is needed 
// to enable the default 'has_push_front' implementation below

template< typename Tag >
struct push_front_impl
{
    template< typename Sequence, typename T > struct apply {};
};

template< typename Tag >
struct has_push_front_impl
{
    template< typename Seq > struct apply
        : aux::has_type< push_front<Seq,int> >
    {
    };
};

BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(2, push_front_impl)
BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(1, has_push_front_impl)

}}

#endif // BOOST_MPL_AUX_PUSH_FRONT_IMPL_HPP_INCLUDED
