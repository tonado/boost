# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Edward Diener (2011) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
# define BOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
#
# include <boost/preprocessor/config/config.hpp>
#
# /* BOOST_PP_IS_EMPTY */
#
# if BOOST_PP_VARIADICS
#
# if BOOST_PP_VARIADICS_MSVC
#
# include <boost/preprocessor/arithmetic/dec.hpp>
# include <boost/preprocessor/variadic/size.hpp>
#
# define BOOST_PP_IS_EMPTY_DETAIL_IS_TUPLE_BEGIN(x) \
    BOOST_PP_DEC \
      ( \
      BOOST_PP_VARIADIC_SIZE \
        ( \
        BOOST_PP_IS_EMPTY_DETAIL_IS_TUPLE_BEGIN_DETAIL_EXPAND x \
        ) \
      ) \
/**/
#
# define BOOST_PP_IS_EMPTY_DETAIL_IS_TUPLE_BEGIN_DETAIL_EXPAND(...) \
    1,1 \
/**/
#
#define BOOST_PP_IS_EMPTY_DETAIL_PRIMITIVE_CAT(a, b) a ## b
#define BOOST_PP_IS_EMPTY_DETAIL_IIF(bit) \
    BOOST_PP_IS_EMPTY_DETAIL_PRIMITIVE_CAT(BOOST_PP_IS_EMPTY_DETAIL_IIF_,bit) \
/**/
#define BOOST_PP_IS_EMPTY_DETAIL_IIF_0(t, b) b
#define BOOST_PP_IS_EMPTY_DETAIL_IIF_1(t, b) t
#define BOOST_PP_IS_EMPTY_DETAIL_GEN_0(param) 0
#define BOOST_PP_IS_EMPTY_DETAIL_NON_FUNCTION(param) \
    BOOST_PP_IS_EMPTY_DETAIL_IS_TUPLE_BEGIN \
      ( \
      BOOST_PP_IS_EMPTY_DETAIL_IS_EMPTY_NON_FUNCTION_C param () \
      ) \
/**/
#define BOOST_PP_IS_EMPTY(param) \
    BOOST_PP_IS_EMPTY_DETAIL_IIF \
      ( \
      BOOST_PP_IS_EMPTY_DETAIL_IS_TUPLE_BEGIN \
        ( \
        param \
        ) \
      ) \
      ( \
      BOOST_PP_IS_EMPTY_DETAIL_GEN_0, \
      BOOST_PP_IS_EMPTY_DETAIL_NON_FUNCTION \
      ) \
    (param) \
/**/
# else
#define BOOST_PP_IS_EMPTY_DETAIL_CAT(a, ...) BOOST_PP_IS_EMPTY_DETAIL_PRIMITIVE_CAT(a,__VA_ARGS__)
#define BOOST_PP_IS_EMPTY_DETAIL_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_PP_IS_EMPTY_DETAIL_IIF(bit) \
    BOOST_PP_IS_EMPTY_DETAIL_PRIMITIVE_CAT(BOOST_PP_IS_EMPTY_DETAIL_IIF_,bit) \
/**/
#define BOOST_PP_IS_EMPTY_DETAIL_IIF_0(t, ...) __VA_ARGS__
#define BOOST_PP_IS_EMPTY_DETAIL_IIF_1(t, ...) t
#define BOOST_PP_IS_EMPTY_DETAIL_SPLIT(i, ...) \
    BOOST_PP_IS_EMPTY_DETAIL_PRIMITIVE_CAT(BOOST_PP_IS_EMPTY_DETAIL_SPLIT_,i)(__VA_ARGS__) \
/**/
#define BOOST_PP_IS_EMPTY_DETAIL_SPLIT_0(a, ...) a
#define BOOST_PP_IS_EMPTY_DETAIL_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC(...) \
    BOOST_PP_IS_EMPTY_DETAIL_SPLIT \
      ( \
      0, \
      BOOST_PP_IS_EMPTY_DETAIL_CAT \
        ( \
        BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC_R_, \
        BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC_C __VA_ARGS__ \
        ) \
      ) \
/**/
#define BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC_C(...) 1
#define BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC_R_1 1,
#define BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC_R_BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC_C 0,
#define BOOST_PP_IS_EMPTY(...) \
    BOOST_PP_IS_EMPTY_DETAIL_IIF \
      ( \
      BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC \
        ( \
        __VA_ARGS__ \
        ) \
      ) \
      ( \
      0, \
      BOOST_PP_IS_EMPTY_DETAIL_IS_VARIADIC \
        ( \
        BOOST_PP_IS_EMPTY_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ () \
        ) \
      ) \
/**/
# endif
#define BOOST_PP_IS_EMPTY_DETAIL_IS_EMPTY_NON_FUNCTION_C() ()
#
# else
#
# include <boost/preprocessor/cat.hpp>
# include <boost/preprocessor/tuple/elem.hpp>
# include <boost/preprocessor/facilities/empty.hpp>
# include <boost/preprocessor/detail/split.hpp>
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC() && ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MWCC()
#    define BOOST_PP_IS_EMPTY(x) BOOST_PP_IS_EMPTY_I(x BOOST_PP_IS_EMPTY_HELPER)
#    define BOOST_PP_IS_EMPTY_I(contents) BOOST_PP_TUPLE_ELEM(2, 1, (BOOST_PP_IS_EMPTY_DEF_ ## contents()))
#    define BOOST_PP_IS_EMPTY_DEF_BOOST_PP_IS_EMPTY_HELPER 1, 1 BOOST_PP_EMPTY
#    define BOOST_PP_IS_EMPTY_HELPER() , 0
# else
#    if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
#        define BOOST_PP_IS_EMPTY(x) BOOST_PP_IS_EMPTY_I(BOOST_PP_IS_EMPTY_HELPER x ())
#        define BOOST_PP_IS_EMPTY_I(test) BOOST_PP_IS_EMPTY_II(BOOST_PP_SPLIT(0, BOOST_PP_CAT(BOOST_PP_IS_EMPTY_DEF_, test)))
#        define BOOST_PP_IS_EMPTY_II(id) id
#    else
#        define BOOST_PP_IS_EMPTY(x) BOOST_PP_IS_EMPTY_I((BOOST_PP_IS_EMPTY_HELPER x ()))
#        define BOOST_PP_IS_EMPTY_I(par) BOOST_PP_IS_EMPTY_II ## par
#        define BOOST_PP_IS_EMPTY_II(test) BOOST_PP_SPLIT(0, BOOST_PP_CAT(BOOST_PP_IS_EMPTY_DEF_, test))
#    endif
#    define BOOST_PP_IS_EMPTY_HELPER() 1
#    define BOOST_PP_IS_EMPTY_DEF_1 1, BOOST_PP_NIL
#    define BOOST_PP_IS_EMPTY_DEF_BOOST_PP_IS_EMPTY_HELPER 0, BOOST_PP_NIL
# endif
#
# endif
#
# endif
