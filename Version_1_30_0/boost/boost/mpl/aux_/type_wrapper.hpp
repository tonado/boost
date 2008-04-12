//-----------------------------------------------------------------------------
// boost mpl/aux/type_wrapper.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Peter Dimov, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_AUX_TYPE_WRAPPER_HPP_INCLUDED
#define BOOST_MPL_AUX_TYPE_WRAPPER_HPP_INCLUDED

namespace boost {
namespace mpl {
namespace aux {

template< typename T > struct type_wrapper
{
    typedef T type;
};

} // namespace aux
} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_AUX_TYPE_WRAPPER_HPP_INCLUDED
