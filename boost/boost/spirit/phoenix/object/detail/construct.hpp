/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_PP_IS_ITERATING
#ifndef PHOENIX_OBJECT_DETAIL_CONSTRUCT_HPP
#define PHOENIX_OBJECT_DETAIL_CONSTRUCT_HPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, PHOENIX_COMPOSITE_LIMIT,                                            \
    "boost/spirit/phoenix/object/detail/construct.hpp"))
#include BOOST_PP_ITERATE()

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <typename T, BOOST_PP_ENUM_PARAMS(N, typename A)>
    inline actor<typename as_composite<detail::construct_eval<T>
        , BOOST_PP_ENUM_PARAMS(N, A)>::type>
    construct(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& _))
    {
        return compose<detail::construct_eval<T> >(BOOST_PP_ENUM_PARAMS(N, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


