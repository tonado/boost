# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# /* Revised by Edward Diener (2011) */
#
# include <boost/preprocessor/debug.hpp>
# include <libs/preprocessor/test/test.h>

BEGIN sizeof(BOOST_PP_ASSERT_MSG(0, "text") "") / sizeof(char) != 1 END
BEGIN sizeof(BOOST_PP_ASSERT_MSG(1, "text") "") / sizeof(char) == 1 END

BOOST_PP_ASSERT(10)

# line BOOST_PP_LINE(100, __FILE__)
BEGIN __LINE__ == 100 END

# if BOOST_PP_VARIADICS && !BOOST_PP_VARIADICS_MSVC

BOOST_PP_ASSERT_IS_TUPLE((t,3,e,2))
BOOST_PP_ASSERT_IS_TUPLE(((y,s,w),3,e,2))
BOOST_PP_ASSERT_IS_ARRAY((4,(x,3,e,2)))
BOOST_PP_ASSERT_IS_ARRAY((6,(x,3,e,2,(4,(x,3,e,2)),$)))
BOOST_PP_ASSERT_IS_LIST((4,(x,BOOST_PP_NIL)))
BOOST_PP_ASSERT_IS_LIST(((3,6m,irj),(x,BOOST_PP_NIL)))
BOOST_PP_ASSERT_IS_SEQ((x)(y)(z)(2)(3)(4))
BOOST_PP_ASSERT_IS_SEQ((x)(y)(z)((1,2))(3)(4))
BOOST_PP_ASSERT_IS_SEQ((x)(y)(z)((1,2))(3)((4,(x,BOOST_PP_NIL))))
BOOST_PP_ASSERT_IS_SEQ((x)(y)((x)(y)(z)(2)(3)(4))((1,2))(3)((4,(x,BOOST_PP_NIL))))

# endif