
#ifndef BOOST_MPL_DISTANCE_HPP_INCLUDED
#define BOOST_MPL_DISTANCE_HPP_INCLUDED

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

#include <boost/mpl/distance_fwd.hpp>
#include <boost/mpl/aux_/iter_distance.hpp>
#include <boost/mpl/aux_/iterator_category.hpp>
#include <boost/mpl/iterator_tag.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/aux_/na_spec.hpp>
#include <boost/config.hpp>

namespace boost {
namespace mpl {

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace aux {

// forward/bidirectional iterators
template< typename Category, typename First, typename Last >
struct distance_impl
    : iter_fold<
          iterator_range<First,Last>
        , integral_c<long, 0>
        , next<>
        >::type
{
};

template< typename First, typename Last >
struct distance_impl<random_access_iterator_tag,First,Last>
    : aux::iter_distance<First,Last>::type
{
};

} // namespace aux

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template<
      typename BOOST_MPL_AUX_NA_PARAM(First)
    , typename BOOST_MPL_AUX_NA_PARAM(Last)
    >
struct distance
// Aleksey claims borland doesn't like inheritance here, but it passes
// all the same tests; the workaround can easily be enabled again if
// verified.  -- dwa 2003/5/8
# if 1 || !defined(__BORLANDC__)  
    : aux::distance_impl<
          typename BOOST_MPL_AUX_ITERATOR_CATEGORY(First)
        , First
        , Last
      >
# endif 
{
# if 0 && defined(__BORLANDC__)
    typedef typename aux::distance_impl<
        typename BOOST_MPL_AUX_ITERATOR_CATEGORY(First)
      , First
      , Last
    >::type type;
    BOOST_STATIC_CONSTANT(typename type::value_type, value = type::value);
# endif 
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

#else

namespace aux {

// forward/bidirectional iterators
template< typename Category >
struct distance_impl
{
    template< typename First, typename Last > struct result_
        : iter_fold<
              iterator_range<First,Last>
            , integral_c<long, 0>
            , next<>
            >::type
    {
    };
};

template<>
struct distance_impl<random_access_iterator_tag>
{
    template< typename First, typename Last > struct result_
        : aux::iter_distance<First,Last>::type
    {
    };
};

} // namespace aux

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template<
      typename BOOST_MPL_AUX_NA_PARAM(First)
    , typename BOOST_MPL_AUX_NA_PARAM(Last)
    >
struct distance
#if !defined(BOOST_MSVC) || BOOST_MSVC != 1300
    : aux::distance_impl< typename BOOST_MPL_AUX_ITERATOR_CATEGORY(First) >
#else
    : aux::distance_impl< random_access_iterator_tag >
#endif
        ::template result_<First,Last>
{
};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

BOOST_MPL_AUX_NA_ALGORITHM_SPEC(2, distance)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_DISTANCE_HPP_INCLUDED
