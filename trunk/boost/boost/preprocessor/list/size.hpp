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
# ifndef BOOST_PREPROCESSOR_LIST_SIZE_HPP
# define BOOST_PREPROCESSOR_LIST_SIZE_HPP
#
# include <boost/preprocessor/arithmetic/inc.hpp>
# include <boost/preprocessor/config/config.hpp>
# include <boost/preprocessor/control/while.hpp>
# include <boost/preprocessor/list/adt.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/tuple/rem.hpp>
#
# /* BOOST_PP_LIST_SIZE */
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_SIZE BOOST_PP_LIST_SIZE_I
# else
#    define BOOST_PP_LIST_SIZE(list) BOOST_PP_LIST_SIZE_I(list)
# endif
#
# define BOOST_PP_LIST_SIZE_I(list) BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_WHILE(BOOST_PP_LIST_SIZE_P, BOOST_PP_LIST_SIZE_O, (0, list)))
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_SIZE_P(d, rl) BOOST_PP_LIST_IS_CONS(BOOST_PP_TUPLE_ELEM(2, 1, rl))
# else
#    define BOOST_PP_LIST_SIZE_P(d, rl) BOOST_PP_LIST_SIZE_P_I(BOOST_PP_TUPLE_REM_2 rl)
#    define BOOST_PP_LIST_SIZE_P_I(im) BOOST_PP_LIST_SIZE_P_II(im)
#    define BOOST_PP_LIST_SIZE_P_II(r, l) BOOST_PP_LIST_IS_CONS(l)
# endif
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_SIZE_O(d, rl) (BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 0, rl)), BOOST_PP_LIST_REST(BOOST_PP_TUPLE_ELEM(2, 1, rl)))
# else
#    define BOOST_PP_LIST_SIZE_O(d, rl) BOOST_PP_LIST_SIZE_O_I(BOOST_PP_TUPLE_REM_2 rl)
#    define BOOST_PP_LIST_SIZE_O_I(im) BOOST_PP_LIST_SIZE_O_II(im)
#    define BOOST_PP_LIST_SIZE_O_II(r, l) (BOOST_PP_INC(r), BOOST_PP_LIST_REST(l))
# endif
#
# /* BOOST_PP_LIST_SIZE_D */
#
# if ~BOOST_PP_CONFIG_FLAGS & BOOST_PP_CONFIG_EDG
#    define BOOST_PP_LIST_SIZE_D BOOST_PP_LIST_SIZE_D_I
# else
#    define BOOST_PP_LIST_SIZE_D(d, list) BOOST_PP_LIST_SIZE_D_I(d, list)
# endif
#
# define BOOST_PP_LIST_SIZE_D_I(d, list) BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_WHILE_ ## d(BOOST_PP_LIST_SIZE_P, BOOST_PP_LIST_SIZE_O, (0, list)))
#
# endif
