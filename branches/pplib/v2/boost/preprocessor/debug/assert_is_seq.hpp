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
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/comparison/equal.hpp>
# include <boost/preprocessor/control/expr_iif.hpp>
# include <boost/preprocessor/control/iif.hpp>
# include <boost/preprocessor/control/while.hpp>
# include <boost/preprocessor/debug/assert.hpp>
# include <boost/preprocessor/facilities/expand.hpp>
# include <boost/preprocessor/facilities/is_empty.hpp>
# include <boost/preprocessor/facilities/is_tuple_begin.hpp>
# include <boost/preprocessor/logical/not.hpp>
# include <boost/preprocessor/variadic/size.hpp>
# include <boost/preprocessor/variadic/elem.hpp>
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
    BOOST_PP_IS_SEQ_DETAIL_CHECK_RETURN_FAILURE \
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
# define BOOST_PP_IS_SEQ_DETAIL_PRED(d,state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(state), \
      BOOST_PP_IS_SEQ_DETAIL_GEN_ONE, \
      BOOST_PP_IS_SEQ_DETAIL_PRED_CHECK_SUCCESS \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_PRED_CHECK_SUCCESS(state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_IS_SUCCESS(state), \
      BOOST_PP_IS_SEQ_DETAIL_GEN_ZERO, \
      BOOST_PP_IS_SEQ_DETAIL_PRED_CHECK_FAILURE \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_PRED_CHECK_FAILURE(state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_IS_FAILURE(state), \
      0, \
      1 \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_OP(d,state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_TUPLE_BEGIN(state), \
      BOOST_PP_IS_SEQ_DETAIL_CHECK_FIRST_TUPLE_SIZE, \
      BOOST_PP_IS_SEQ_DETAIL_GEN_RETURN_ASSERT \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_CHECK_FIRST_TUPLE_SIZE(state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE(state), \
      BOOST_PP_IS_SEQ_DETAIL_CHECK_AFTER_FIRST_TUPLE, \
      BOOST_PP_IS_SEQ_DETAIL_GEN_FAILURE \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_CHECK_AFTER_FIRST_TUPLE(state) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_IS_AFTER_FIRST_TUPLE(state), \
      BOOST_PP_IS_SEQ_DETAIL_GET_AFTER_FIRST_TUPLE, \
      BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE \
      ) \
    (state) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE(...) \
    BOOST_PP_EQUAL \
      ( \
      BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
      1 \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE_AFTER(...) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_EQUAL \
        ( \
        BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), \
        1 \
        ), \
      BOOST_PP_IS_SEQ_DETAIL_GEN_ONE_COMMA, \
      BOOST_PP_IS_SEQ_DETAIL_GEN_ZERO_COMMA \
      ) \
    () \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_EMPTY \
        ( \
        BOOST_PP_IS_SEQ_DETAIL_GET_AFTER_FIRST_TUPLE(x) \
        ), \
      BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE_EMPTY, \
      BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE_FULL \
      ) \
    (x) \
/**/
#
# if BOOST_PP_VARIADICS_MSVC
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE_EMPTY(x) \
    BOOST_PP_EXPAND(BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE x) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE_FULL(x) \
    BOOST_PP_VARIADIC_ELEM \
      ( \
      0, \
      BOOST_PP_EXPAND(BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE_AFTER x) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GET_AFTER_FIRST_TUPLE(x) \
    BOOST_PP_EXPAND(BOOST_PP_IS_SEQ_DETAIL_EMPTY x) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE(x) \
    BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE_GET \
      ( \
      BOOST_PP_EXPAND(BOOST_PP_IS_SEQ_DETAIL_EMPTY x) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_IS_AFTER_FIRST_TUPLE(x) \
    BOOST_PP_IS_TUPLE_BEGIN \
      ( \
      BOOST_PP_EXPAND(BOOST_PP_IS_SEQ_DETAIL_EMPTY x) \
      ) \
/**/
#
# else
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE_EMPTY(x) \
    BOOST_PP_IS_SEQ_DETAIL_APPLY(BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE,x) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_ASSERT_FIRST_TUPLE_SIZE_FULL(x) \
    BOOST_PP_VARIADIC_ELEM \
      ( \
      0, \
      BOOST_PP_IS_SEQ_DETAIL_APPLY(BOOST_PP_IS_SEQ_DETAIL_ASSERT_SIZE_AFTER,x) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GET_AFTER_FIRST_TUPLE(x) \
    BOOST_PP_IS_SEQ_DETAIL_APPLY(BOOST_PP_IS_SEQ_DETAIL_EMPTY,x) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE(x) \
    BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE_GET \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_APPLY(BOOST_PP_IS_SEQ_DETAIL_EMPTY,x) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_IS_AFTER_FIRST_TUPLE(x) \
    BOOST_PP_IS_TUPLE_BEGIN \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_APPLY(BOOST_PP_IS_SEQ_DETAIL_EMPTY,x) \
      ) \
/**/
#
# endif
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_NOT_TUPLE_GET(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_EMPTY \
        ( \
        x \
        ), \
      BOOST_PP_IS_SEQ_SUCCESS, \
      x \
      ) \
/**/
#
#define BOOST_PP_IS_SEQ_DETAIL_APPLY_I(macro, args) \
  macro args \
/**/
#define BOOST_PP_IS_SEQ_DETAIL_APPLY(macro, args) \
  BOOST_PP_IS_SEQ_DETAIL_APPLY_I(macro, args) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_RETURN_ASSERT(x) \
    BOOST_PP_IIF \
      ( \
      BOOST_PP_IS_SEQ_DETAIL_IS_SUCCESS(x), \
      BOOST_PP_IS_SEQ_SUCCESS, \
      BOOST_PP_IS_SEQ_FAILURE \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_EMPTY(...) BOOST_PP_IS_SEQ_DETAIL_EMPTY1(__VA_ARGS__)
#
# define BOOST_PP_IS_SEQ_DETAIL_EMPTY1(...)
#
# define BOOST_PP_IS_SEQ_DETAIL_CHECK_RETURN_FAILURE(x) \
    BOOST_PP_ASSERT \
      ( \
      BOOST_PP_NOT \
        ( \
        BOOST_PP_IS_SEQ_DETAIL_IS_FAILURE(x) \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_IS_SUCCESS(x) \
    BOOST_PP_IS_EMPTY \
      ( \
      BOOST_PP_CAT(BOOST_PP_IS_SEQ_DETAIL_SHELPER_,x) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_IS_FAILURE(x) \
    BOOST_PP_IS_EMPTY \
      ( \
      BOOST_PP_CAT(BOOST_PP_IS_SEQ_DETAIL_FHELPER_,x) \
      ) \
/**/
#
# define BOOST_PP_IS_SEQ_DETAIL_SHELPER_BOOST_PP_IS_SEQ_SUCCESS
#
# define BOOST_PP_IS_SEQ_DETAIL_FHELPER_BOOST_PP_IS_SEQ_FAILURE
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_ONE(x) 1
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_ZERO(x) 0
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_ONE_COMMA() 1,
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_ZERO_COMMA() 0,
#
# define BOOST_PP_IS_SEQ_DETAIL_GEN_FAILURE(x) BOOST_PP_IS_SEQ_FAILURE
#
# endif /* NDEBUG */
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_IS_SEQ_HPP */
