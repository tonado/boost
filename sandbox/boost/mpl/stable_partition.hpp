//-----------------------------------------------------------------------------
// boost mpl/stable_partition.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_STABLE_PARTITION_HPP_INCLUDED
#define BOOST_MPL_STABLE_PARTITION_HPP_INCLUDED

#include "boost/mpl/clear.hpp"
#include "boost/mpl/copy_backward_if.hpp"
#include "boost/mpl/pair.hpp"
#include "boost/mpl/remove_if.hpp"
#include "boost/mpl/push_front.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_BEGIN

template <
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Predicate)
    >
struct stable_partition
{
private:

    typedef typename copy_backward_if<
          Sequence
        , typename clear<Sequence>::type
        , push_front<_,_>
        , Predicate
        >::type first_part;

    typedef typename remove_if<
          Sequence
        , Predicate
        >::type second_part;

public:

    typedef pair< first_part,second_part >
        type;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,stable_partition,(Sequence,Predicate))

};

BOOST_MPL_AUX_AGLORITHM_NAMESPACE_END

BOOST_MPL_AUX_ALGORITHM_VOID_SPEC(2, stable_partition)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_STABLE_PARTITION_HPP_INCLUDED
