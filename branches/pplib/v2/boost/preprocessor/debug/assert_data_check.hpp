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
# ifndef BOOST_PREPROCESSOR_DEBUG_ASSERT_DATA_CHECK_HPP
# define BOOST_PREPROCESSOR_DEBUG_ASSERT_DATA_CHECK_HPP
#
# if BOOST_PP_VARIADICS
#
# if !defined(BOOST_PP_ASSERT_DATA)
# if defined(NDEBUG)
# define BOOST_PP_ASSERT_DATA 0
# else
# define BOOST_PP_ASSERT_DATA 1
# endif
# endif
#
# endif /* BOOST_PP_VARIADICS */
# endif /* BOOST_PREPROCESSOR_DEBUG_ASSERT_DATA_CHECK_HPP */
