
#ifndef BOOST_MPL_SET_AUX_EMPTY_IMPL_HPP_INCLUDED
#define BOOST_MPL_SET_AUX_EMPTY_IMPL_HPP_INCLUDED

// Copyright (c) 2003-04 Aleksey Gurtovoy
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/empty_fwd.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/set/aux_/tag.hpp>

namespace boost { namespace mpl {

template<>
struct empty_impl< aux::set_tag >
{
    template< typename Set > struct apply
        : not_< typename Set::size >
    {
    };
};

}}

#endif // BOOST_MPL_SET_AUX_EMPTY_IMPL_HPP_INCLUDED
