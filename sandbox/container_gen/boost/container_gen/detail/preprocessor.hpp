//=============================================================================
// Copyright (C) 2013 Cromwell D. Enage
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef BOOST_CONTAINER_GEN_DETAIL_PREPROCESSOR_HPP_INCLUDED
#define BOOST_CONTAINER_GEN_DETAIL_PREPROCESSOR_HPP_INCLUDED

#include <boost/config.hpp>

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/detail/preprocessor/binary_seq_inc_for_param.hpp>

#if !defined BOOST_CONTAINER_GEN_MAX_PARAMETER_COUNT
#define BOOST_CONTAINER_GEN_MAX_PARAMETER_COUNT 7
#endif

#define BOOST_CONTAINER_GEN_PP_PARAM_PRED(r, binary_seq) \
BOOST_PP_NOT_EQUAL( \
    BOOST_PP_INC(BOOST_CONTAINER_GEN_MAX_PARAMETER_COUNT) \
  , BOOST_PP_SEQ_SIZE(binary_seq) \
) \
//!

#define BOOST_CONTAINER_GEN_PP_PARAM_PRED_WITH_DATA(r, tuple) \
BOOST_PP_NOT_EQUAL( \
    BOOST_PP_INC(BOOST_CONTAINER_GEN_MAX_PARAMETER_COUNT) \
  , BOOST_PP_SEQ_SIZE(BOOST_PP_TUPLE_ELEM(2, 0, tuple)) \
) \
//!

#define BOOST_CONTAINER_GEN_PP_PARAM_INC(r, binary_seq) \
BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM(binary_seq) \
//!

#define BOOST_CONTAINER_GEN_PP_PARAM_INC_WITH_DATA(r, tuple) \
( \
    BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM( \
        BOOST_PP_TUPLE_ELEM(2, 0, tuple) \
    ) \
  , BOOST_PP_TUPLE_ELEM(2, 1, tuple) \
) \
//!

#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#include <boost/container/detail/preprocessor.hpp>

#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif  // BOOST_CONTAINER_GEN_DETAIL_PREPROCESSOR_HPP_INCLUDED

