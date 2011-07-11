# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_SEQ_HPP
# define BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_SEQ_HPP
#
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/comparison/equal.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/control/while.hpp>
# include <boost/preprocessor/debug/assert.hpp>
# include <boost/preprocessor/facilities/is_empty.hpp>
# include <boost/preprocessor/facilities/is_tuple_begin.hpp>
# include <boost/preprocessor/logical/not.hpp>
# include <boost/preprocessor/variadic/size.hpp>
#
# /* BOOST_PP_ASSERT_IS_SEQ */
#
# if BOOST_PP_VARIADICS
# if defined(NDEBUG)
# define BOOST_PP_ASSERT_IS_SEQ(x)
#
# else
#
# define BOOST_PP_ASSERT_IS_SEQ(x) \
    BOOST_PP_IS_SEQ_DETAIL_GEN_EMPTY \
      ( \
      BOOST_PP_WHILE \
        ( \
        BOOST_PP_IS_SEQ_DETAIL_PRED, \
        BOOST_PP_IS_SEQ_DETAIL_OP, \
        x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_EMPTY(x) \
    BOOST_PP_IS_SEQ_DETAIL_GEN_EMPTY_I(x) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_EMPTY_I(x)
#
# define BOOST_PP_IS_SEQ_DETAIL_PRED(d,state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(state), \
      BOOST_PP_IS_SEQ_DETAIL_GEN_ONE, \
      BOOST_PP_IS_SEQ_DETAIL_CHECK_NIL \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_OP(d,state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_IS_AFTER_FIRST_TUPLE(x), \
      BOOST_PP_IS_SEQ_DETAIL_GET_AFTER_FIRST_TUPLE, \
      BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE(x) \
    BOOST_PP_IS_SEQ_DETAIL_EMPTY \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE x \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GET_AFTER_FIRST_TUPLE(x) \
    BOOST_PP_IS_SEQ_DETAIL_EMPTY x \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE(x) \
    BOOST_PP_IS_SEQ_DETAIL_GEN_NIL \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_IS_AFTER_FIRST_TUPLE(x) \
    BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE(x) \
    BOOST_PP_NOT \
      ( \
      BOOST_PP_IS_EMPTY \
        ( \
        BOOST_PP_IS_SEQ_DETAIL_EMPTY x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE(...) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_EQUAL \
        ( \
        BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
        1 \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_EMPTY(...)
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_ONE(x) \
    1 \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_CHECK_NIL(x) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_NIL(x) \
      ) \
    0 \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_NIL(x) \
    BOOST_PP_IS_EMPTY \
      ( \
      BOOST_PP_CAT \
        ( \
        BOOST_PP_IS_SEQ_DETAIL_NIL_HELPER_, \
        x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_NIL_HELPER_BOOST_PP_IS_SEQ_DETAIL_GEN_NIL
#
# endif /* NDEBUG */
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_SEQ_HPP */
