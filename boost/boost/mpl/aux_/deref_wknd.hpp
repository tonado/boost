//-----------------------------------------------------------------------------
// boost mpl/aux_/deref_wknd.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-03
// Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MPL_AUX_DEREF_WNKD_HPP_INCLUDED
#define BOOST_MPL_AUX_DEREF_WNKD_HPP_INCLUDED

#include "boost/mpl/aux_/is_msvc_eti_arg.hpp"
#include "boost/mpl/aux_/config/eti.hpp"

#if defined(BOOST_MPL_MSVC_ETI_BUG)

namespace boost { namespace mpl { namespace aux {

#   if defined(BOOST_MPL_MSVC_60_ETI_BUG)

template< typename Iterator > struct deref_wknd
{
    typedef typename Iterator::type type;
};

template<> struct deref_wknd<int>
{
    typedef int type;
};

#   else

template< bool > struct deref_wknd_impl
{
    template< typename Iterator > struct result_
    {
        typedef typename Iterator::type type;
    };
};

template<> struct deref_wknd_impl<false>
{
    template< typename Iterator > struct result_
    {
        typedef int type;
    };
};

template< typename Iterator > struct deref_wknd
    : deref_wknd_impl< !aux::is_msvc_eti_arg<Iterator>::value >
        ::template result_<Iterator>
{
};

#   endif // BOOST_MPL_MSVC_60_ETI_BUG

}}} // namespace boost::mpl::aux

#   define BOOST_MPL_AUX_DEREF_WNKD(iter) ::boost::mpl::aux::deref_wknd<iter>::type

#else

#   define BOOST_MPL_AUX_DEREF_WNKD(iter) iter::type

#endif // BOOST_MPL_MSVC_ETI_BUG

#endif // BOOST_MPL_AUX_DEREF_WNKD_HPP_INCLUDED
