//-----------------------------------------------------------------------------
// boost mpl/arithmetic/divides.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000/02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_ARITHMETIC_DIVIDES_HPP_INCLUDED
#define BOOST_MPL_ARITHMETIC_DIVIDES_HPP_INCLUDED

#include "boost/mpl/integral_c.hpp"
#include "boost/mpl/aux_/typeof.hpp"
#include "boost/mpl/aux_/value_wknd.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<
      typename T, T N1, T N2, T N3 = 1, T N4 = 1, T N5 = 1
    >
struct divides_c
{
    BOOST_STATIC_CONSTANT(T, value = (N1 / N2 / N3 / N4 / N5));
#if !defined(__BORLANDC__)
    typedef integral_c<T,value> type;
#else
    typedef integral_c<T,(N1 / N2 / N3 / N4 / N5)> type;
#endif
};

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
// ETI workaround
template<>
struct divides_c<long,0,0,0,0,0>
{
};
#endif

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T1)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(T2)
    , typename T3 = integral_c<int,1>
    , typename T4 = integral_c<int,1>
    , typename T5 = integral_c<int,1>
    >
struct divides
    : divides_c<
          BOOST_MPL_AUX_TYPEOF(T1,
             T1::value / T2::value / T3::value / T4::value / T5::value
            )
        , BOOST_MPL_AUX_VALUE_WKND(T1)::value
        , BOOST_MPL_AUX_VALUE_WKND(T2)::value
        , BOOST_MPL_AUX_VALUE_WKND(T3)::value
        , BOOST_MPL_AUX_VALUE_WKND(T4)::value
        , BOOST_MPL_AUX_VALUE_WKND(T5)::value
        >
{
};

BOOST_MPL_AUX_VOID_SPEC_EXT(2,5,divides)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_ARITHMETIC_DIVIDES_HPP_INCLUDED
