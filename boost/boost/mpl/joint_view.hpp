//-----------------------------------------------------------------------------
// boost mpl/joint_view.hpp header file
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

#ifndef BOOST_MPL_JOINT_VIEW_HPP_INCLUDED
#define BOOST_MPL_JOINT_VIEW_HPP_INCLUDED

#include "boost/mpl/aux_/joint_iter.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/plus.hpp"
#include "boost/mpl/size_fwd.hpp"

namespace boost {
namespace mpl {

namespace aux
{
  struct joint_view_tag;
}

template <>
struct size_traits< aux::joint_view_tag >
{
    template < typename JointView > struct algorithm
      : plus<
            size<typename JointView::sequence1>
          , size<typename JointView::sequence2>
        >
    {};
};

template<
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence1_)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence2_)
    >
struct joint_view
{
 private:
    friend struct size_traits< aux::joint_view_tag >;
    typedef Sequence1_ sequence1;
    typedef Sequence2_ sequence2;
    
    typedef typename begin<Sequence1_>::type first1_;
    typedef typename end<Sequence1_>::type last1_;
    typedef typename begin<Sequence2_>::type first2_;
    typedef typename end<Sequence2_>::type last2_;

 public:
    typedef aux::joint_view_tag tag;
    typedef typename aux::joint_iter<first1_,last1_,first2_> begin;
    typedef typename aux::joint_iter<last1_,last1_,last2_> end;
};

BOOST_MPL_AUX_VOID_SPEC(2, joint_view)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_JOINT_VIEW_HPP_INCLUDED
