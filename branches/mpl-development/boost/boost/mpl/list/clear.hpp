//-----------------------------------------------------------------------------
// boost mpl/list/clear.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_LIST_CLEAR_HPP
#define BOOST_MPL_LIST_CLEAR_HPP

#include "boost/mpl/clear.hpp"
#include "boost/mpl/list/traits.hpp"

namespace boost {
namespace mpl {

template<>
struct clear_algorithm_traits<list_sequence_tag>
{
    template<typename List> struct algorithm
    {
        typedef typename mpl::list_traits_tag<List>::type   tag_;
        typedef typename mpl::list_traits<tag_>::null_node  sequence;
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LIST_CLEAR_HPP
