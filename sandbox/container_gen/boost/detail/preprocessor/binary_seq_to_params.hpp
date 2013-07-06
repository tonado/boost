// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAIL_PREPROCESSOR_BINARY_SEQ_TO_PARAMS_HPP_INCLUDED
#define BOOST_DETAIL_PREPROCESSOR_BINARY_SEQ_TO_PARAMS_HPP_INCLUDED

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/push_back.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_DETAIL_PP_BINARY_SEQ_AUGMENT_INDEX_FOLD_OP(s, out_seq, in_elem) \
BOOST_PP_SEQ_PUSH_BACK(out_seq, (in_elem, BOOST_PP_SEQ_SIZE(out_seq))) \
//!

#define BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAM_SEQ_FOLD_OP(s, io_seq, seq_elem) \
( \
    BOOST_PP_SEQ_PUSH_BACK( \
        BOOST_PP_SEQ_ELEM(0, io_seq) \
      , BOOST_PP_CAT( \
            BOOST_PP_SEQ_ELEM(1, io_seq) \
          , BOOST_PP_TUPLE_ELEM(2, 1, seq_elem) \
        ) BOOST_PP_IIF( \
            BOOST_PP_TUPLE_ELEM(2, 0, seq_elem) \
          , BOOST_PP_SEQ_ELEM(2, io_seq) \
          , BOOST_PP_SEQ_ELEM(3, io_seq) \
        ) BOOST_PP_CAT( \
            BOOST_PP_SEQ_ELEM(4, io_seq) \
          , BOOST_PP_TUPLE_ELEM(2, 1, seq_elem) \
        ) \
    ) \
)(BOOST_PP_SEQ_ELEM(1, io_seq))(BOOST_PP_SEQ_ELEM(2, io_seq)) \
(BOOST_PP_SEQ_ELEM(3, io_seq))(BOOST_PP_SEQ_ELEM(4, io_seq)) \
//!

// This macro converts the specified Boost.Preprocessor sequence of 1s and 0s
// into a formal function parameter list.
//
// Example:
//
// BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS((1)(0)(1)(0), P, &, const&, p)
// expands to
// P0 & p0, P1 const& p1, P2 & p2, P3 const& p3

#define BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, i1, i0, p) \
BOOST_PP_SEQ_ENUM( \
    BOOST_PP_SEQ_ELEM( \
        0 \
      , BOOST_PP_SEQ_FOLD_LEFT( \
            BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAM_SEQ_FOLD_OP \
          , (BOOST_PP_SEQ_NIL)(P)(i1)(i0)(p) \
          , BOOST_PP_SEQ_FOLD_LEFT( \
                BOOST_DETAIL_PP_BINARY_SEQ_AUGMENT_INDEX_FOLD_OP \
              , BOOST_PP_EMPTY() \
              , seq \
            ) \
        ) \
    ) \
) \
//!

#endif  // BOOST_DETAIL_PREPROCESSOR_BINARY_SEQ_TO_PARAMS_HPP_INCLUDED

