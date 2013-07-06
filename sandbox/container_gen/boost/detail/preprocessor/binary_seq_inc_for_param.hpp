// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_PREPROCESSOR_BINARY_SEQ_INC_FOR_PARAM_HPP_INCLUDED
#define BOOST_DETAIL_PREPROCESSOR_BINARY_SEQ_INC_FOR_PARAM_HPP_INCLUDED

#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM_OP(s, result_tuple, element) \
( \
    BOOST_PP_SEQ_PUSH_BACK( \
        BOOST_PP_TUPLE_ELEM(2, 0, result_tuple) \
      , BOOST_PP_IIF( \
            BOOST_PP_TUPLE_ELEM(2, 1, result_tuple) \
          , BOOST_PP_IIF(element, 0, 1) \
          , element \
        ) \
    ) \
  , BOOST_PP_IIF(BOOST_PP_TUPLE_ELEM(2, 1, result_tuple), element, 0) \
) \
//!

#define BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM_IMPL(seq_and_flag) \
BOOST_PP_IIF( \
    BOOST_PP_TUPLE_ELEM(2, 1, seq_and_flag) \
  , BOOST_PP_SEQ_PUSH_BACK(BOOST_PP_TUPLE_ELEM(2, 0, seq_and_flag), 0) \
  , BOOST_PP_TUPLE_ELEM(2, 0, seq_and_flag) \
) \
//!

// This macro treats the specified Boost.Preprocessor sequence of 1s and 0s
// like a binary number in reverse and returns a Boost.Preprocessor sequence
// representing the next value up.  However, if the input sequence contains
// all 1s, then the output sequence will contain one more element but all 0s.
//
// Examples:
// seq = (1)(0)(1)(0) --> return (0)(1)(1)(0)
// seq = (1)(1)(1)(0) --> return (0)(0)(0)(1)
// seq = (1)(1)(1)(1) --> return (0)(0)(0)(0)(0)
#define BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM(seq) \
BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM_IMPL( \
    BOOST_PP_SEQ_FOLD_LEFT( \
        BOOST_DETAIL_PP_BINARY_SEQ_INC_FOR_PARAM_OP \
      , (BOOST_PP_SEQ_NIL, 1) \
      , seq \
    ) \
) \
//!

#endif  // BOOST_DETAIL_PREPROCESSOR_BINARY_SEQ_INC_FOR_PARAM_HPP_INCLUDED

