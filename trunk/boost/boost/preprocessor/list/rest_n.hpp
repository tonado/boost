# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Permission to copy, use, modify, sell and distribute this software is
#  * granted provided this copyright notice appears in all copies. This
#  * software is provided "as is" without express or implied warranty, and
#  * with no claim as to its suitability for any purpose.
#  */
#
# /* Revised by Paul Mensonides (2002) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_LIST_REST_N_HPP
# define BOOST_PREPROCESSOR_LIST_REST_N_HPP
#
# include <boost/preprocessor/arithmetic/dec.hpp>
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/control/while.hpp>
# include <boost/preprocessor/list/adt.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
#
# /* BOOST_PP_LIST_REST_N */
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_REST_N BOOST_PP_LIST_REST_N_I
# else
#    define BOOST_PP_LIST_REST_N(count, list) BOOST_PP_LIST_REST_N_I(count, list)
# endif
#
# define BOOST_PP_LIST_REST_N_I(count, list) BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_WHILE(BOOST_PP_LIST_REST_N_P, BOOST_PP_LIST_REST_N_O, (list, count)))
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_REST_N_P(d, lc) BOOST_PP_TUPLE_ELEM(2, 1, lc)
# else
#    define BOOST_PP_LIST_REST_N_P(d, lc) BOOST_PP_LIST_REST_N_P_I lc
#    define BOOST_PP_LIST_REST_N_P_I(list, count) count
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_REST_N_O(d, lc) (BOOST_PP_LIST_REST(BOOST_PP_TUPLE_ELEM(2, 0, lc)), BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 1, lc)))
# else
#    define BOOST_PP_LIST_REST_N_O(d, lc) BOOST_PP_LIST_REST_N_O_I lc
#    define BOOST_PP_LIST_REST_N_O_I(list, count) (BOOST_PP_LIST_REST(list), BOOST_PP_DEC(count))
# endif
#
# /* BOOST_PP_LIST_REST_N_D */
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_REST_N_D BOOST_PP_LIST_REST_N_D_I
# else
#    define BOOST_PP_LIST_REST_N_D(d, count, list) BOOST_PP_LIST_REST_N_D_I(d, count, list)
# endif
#
# define BOOST_PP_LIST_REST_N_D_I(d, count, list) BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_WHILE_ ## d(BOOST_PP_LIST_REST_N_P, BOOST_PP_LIST_REST_N_O, (list, count)))
#
# endif
