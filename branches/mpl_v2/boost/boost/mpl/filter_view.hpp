//-----------------------------------------------------------------------------
// boost mpl/filter_view.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_FILTER_VIEW_HPP_INCLUDED
#define BOOST_MPL_FILTER_VIEW_HPP_INCLUDED

#include "boost/mpl/find_if.hpp"
#include "boost/mpl/iterator_range.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/apply.hpp"
#include "boost/mpl/aux_/lambda_spec.hpp"

namespace boost {
namespace mpl {

// forward declaration for next_filter_iter
template< typename Iterator, typename LastIterator, typename F >
struct filter_iter;

namespace aux {
template<
      typename Iterator
    , typename LastIterator
    , typename F
    >
struct next_filter_iter
{
 private:
    typedef typename find_if<
          iterator_range<Iterator,LastIterator>
        , F
        >::type base_iter_;
 
 public:
    typedef filter_iter<base_iter_,LastIterator,F> type;
};
} // namespace aux

template<
      typename Iterator
    , typename LastIterator
    , typename F
    >
struct filter_iter
{
    typedef Iterator base;
    typedef typename base::category category;
    typedef typename aux::next_filter_iter<
          typename base::next
        , LastIterator
        , F
        >::type next;
    
    typedef typename base::type type;
};

template< typename Sequence, typename F >
struct filter_view
    : iterator_range<
          typename aux::next_filter_iter<
              typename begin<Sequence>::type
            , typename end<Sequence>::type
            , F
            >::type
        , filter_iter<
              typename end<Sequence>::type
            , typename end<Sequence>::type
            , F
            >
        >
{
};

BOOST_MPL_AUX_PASS_THROUGH_LAMBDA_SPEC(3,filter_iter)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_FILTER_VIEW_HPP_INCLUDED
