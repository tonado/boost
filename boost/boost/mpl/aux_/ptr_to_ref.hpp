
#ifndef BOOST_MPL_AUX_PTR_TO_REF_HPP_INCLUDED
#define BOOST_MPL_AUX_PTR_TO_REF_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2003-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

namespace boost { namespace mpl { namespace aux {

template< typename T > static T const& ptr_to_ref(T*);

}}}

#endif // BOOST_MPL_AUX_PTR_TO_REF_HPP_INCLUDED
