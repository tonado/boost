///////////////////////////////////////////////////////////////////////////////
// epsilon_matcher.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_EPSILON_MATCHER_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_CORE_MATCHER_EPSILON_MATCHER_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/detail/core/quant_style.hpp>
#include <boost/xpressive/detail/core/state.hpp>

namespace boost { namespace xpressive { namespace detail
{

    ///////////////////////////////////////////////////////////////////////////////
    // epsilon_matcher
    //
    struct epsilon_matcher
      : quant_style_assertion
    {
        template<typename BidiIter, typename Next>
        static bool match(state_type<BidiIter> &state, Next const &next)
        {
            return next.match(state);
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // epsilon_mark_matcher
    //
    struct epsilon_mark_matcher
      : quant_style<quant_none, mpl::size_t<0>, mpl::false_>
    {
        int mark_number_; // signed because it could be negative

        epsilon_mark_matcher(int mark_number)
          : mark_number_(mark_number)
        {
        }

        template<typename BidiIter, typename Next>
        bool match(state_type<BidiIter> &state, Next const &next) const
        {
            sub_match_impl<BidiIter> &br = state.sub_match(this->mark_number_);

            bool old_matched = br.matched;
            br.matched = false;

            if(next.match(state))
            {
                return true;
            }

            br.matched = old_matched;
            return false;
        }
    };

}}}

#endif
