//-----------------------------------------------------------------------------
// boost mpl/test/back.cpp source file
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

#include "boost/mpl/back.hpp"
#include "boost/mpl/range_c.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/static_assert.hpp"

namespace mpl = boost::mpl;

int main()
{
    typedef mpl::range_c<int,0,1> numbers1;
    typedef mpl::range_c<int,0,10> numbers2;
    typedef mpl::range_c<int,-10,0> numbers3;
    
    BOOST_STATIC_ASSERT(mpl::back<numbers1>::value == 0);
    BOOST_STATIC_ASSERT(mpl::back<numbers2>::value == 9);
    BOOST_STATIC_ASSERT(mpl::back<numbers3>::value == -1);

    return 0;
}
