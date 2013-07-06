// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_METAFUNCTION_IS_MERGEABLE_HEAP_HPP_INCLUDED
#define BOOST_DETAIL_METAFUNCTION_IS_MERGEABLE_HEAP_HPP_INCLUDED

#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/function_types/property_tags.hpp>
#include <boost/detail/metafunction/has_mfunc_merge.hpp>
#include <boost/detail/metafunction/is_heap.hpp>

namespace boost { namespace detail { namespace metafunction {

    template <typename T>
    struct is_mergeable_heap
      : ::boost::mpl::eval_if<
            is_heap<T>
          , has_member_function_merge<
                T
              , void
              , ::boost::mpl::vector1<T&>
              , ::boost::function_types::non_const
            >
          , ::boost::mpl::false_
        >::type
    {
        BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_mergeable_heap,(T))
    };
}}}  // namespace boost::detail::metafunction

#endif  // BOOST_DETAIL_METAFUNCTION_IS_MERGEABLE_HEAP_HPP_INCLUDED

