
#ifndef BOOST_MPL_STRING_HPP_INCLUDED
#define BOOST_MPL_STRING_HPP_INCLUDED

// Copyright Eric Niebler 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: string.hpp 49239 2009-04-01 09:10:26Z eric_niebler $
// $Date: 2009-04-01 02:10:26 -0700 (Wed, 1 Apr 2009) $
// $Revision: 49239 $

#include <boost/mpl/char.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

namespace boost { namespace mpl
{
    #ifndef BOOST_MPL_STRING_MAX_LENGTH
    # define BOOST_MPL_STRING_MAX_LENGTH 32
    #endif

    #define BOOST_MPL_STRING_MAX_PARAMS BOOST_PP_DIV(BOOST_PP_ADD(BOOST_MPL_STRING_MAX_LENGTH, 3), 4)

    #define BOOST_MPL_MULTICHAR_LENGTH(c)   (std::size_t)((c>0xffffff)+(c>0xffff)+(c>0xff)+1)
    #define BOOST_MPL_MULTICHAR_AT(c,i)     (char)(0xff&(c>>(8*(BOOST_MPL_MULTICHAR_LENGTH(c)-(std::size_t)(i)-1))))

    struct string_tag;
    struct string_iterator_tag;

    template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_MPL_STRING_MAX_PARAMS, unsigned C, 0)>
    struct string;

    template<typename Sequence, unsigned I, unsigned J>
    struct string_iterator;

    template<typename Sequence>
    struct sequence_tag;

    template<typename Tag>
    struct size_impl;

    template<>
    struct size_impl<mpl::string_tag>
    {
        template<typename Sequence>
        struct apply;

        #define M0(z, n, data)                                                                      \
        + BOOST_MPL_MULTICHAR_LENGTH(BOOST_PP_CAT(C,n))

        #define M1(z, n, data)                                                                      \
        template<BOOST_PP_ENUM_PARAMS_Z(z, n, unsigned C)>                                          \
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)> >                                 \
          : mpl::size_t<(0 BOOST_PP_REPEAT_ ## z(n, M0, ~))>                                        \
        {};

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_MPL_STRING_MAX_PARAMS), M1, ~)
        #undef M0
        #undef M1
    };

    template<>
    struct size_impl<mpl::string_tag>::apply<mpl::string<> >
      : mpl::size_t<0>
    {};

    template<typename Tag>
    struct begin_impl;

    template<>
    struct begin_impl<mpl::string_tag>
    {
        template<typename Sequence>
        struct apply
        {
            typedef mpl::string_iterator<Sequence, 0, 0> type;
        };
    };

    template<typename Tag>
    struct end_impl;

    template<>
    struct end_impl<mpl::string_tag>
    {
        template<typename Sequence>
        struct apply;

        #define M0(z,n,data)                                                                        \
        template<BOOST_PP_ENUM_PARAMS_Z(z, n, unsigned C)>                                          \
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)> >                                 \
        {                                                                                           \
            typedef mpl::string_iterator<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)>, n, 0> type;  \
        };

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_MPL_STRING_MAX_PARAMS), M0, ~)
        #undef M0
    };

    template<>
    struct end_impl<mpl::string_tag>::apply<mpl::string<> >
    {
        typedef mpl::string_iterator<mpl::string<>, 0, 0> type;
    };

    template<typename Tag>
    struct push_back_impl;

    template<>
    struct push_back_impl<mpl::string_tag>
    {
        template<typename Sequence, typename Value, bool B = (4==BOOST_MPL_MULTICHAR_LENGTH(Sequence::back_))>
        struct apply
        {
            BOOST_MPL_ASSERT_MSG(
                (BOOST_MPL_STRING_MAX_LENGTH != mpl::size<Sequence>::type::value)
              , PUSH_BACK_FAILED_MPL_STRING_IS_FULL
              , (Sequence)
            );
            // If the above assertion didn't fire, then the string is sparse.
            // Repack the string and retry the push_back
            typedef
                typename mpl::push_back<
                    typename mpl::copy<
                        Sequence
                      , mpl::back_inserter<mpl::string<> >
                    >::type
                  , Value
                >::type
            type;
        };

        template<typename Value>
        struct apply<mpl::string<>, Value, false>
        {
            typedef mpl::string<(char)Value::value> type;
        };

        #define M0(z,n,data)                                                                        \
        template<BOOST_PP_ENUM_PARAMS_Z(z, n, unsigned C), typename Value>                          \
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)>, Value, false>                    \
        {                                                                                           \
            typedef                                                                                 \
                mpl::string<                                                                        \
                    BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_DEC(n), C)                                   \
                    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                              \
                    (BOOST_PP_CAT(C,BOOST_PP_DEC(n))>0xffffff)                                      \
                    ?BOOST_PP_CAT(C,BOOST_PP_DEC(n))                                                \
                    :(BOOST_PP_CAT(C,BOOST_PP_DEC(n))<<8)|(unsigned char)Value::value               \
                  , (BOOST_PP_CAT(C,BOOST_PP_DEC(n))>0xffffff)                                      \
                    ?(char)Value::value                                                             \
                    :0                                                                              \
                >                                                                                   \
            type;                                                                                   \
        };

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_STRING_MAX_PARAMS, M0, ~)
        #undef M0

        template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, unsigned C), typename Value>
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>, Value, false>
        {
            typedef
                mpl::string<
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(BOOST_MPL_STRING_MAX_PARAMS), C)
                  , (BOOST_PP_CAT(C,BOOST_PP_DEC(BOOST_MPL_STRING_MAX_PARAMS))<<8)|(unsigned char)Value::value
                >
            type;
        };
    };

    template<typename Tag>
    struct pop_back_impl;

    template<>
    struct pop_back_impl<mpl::string_tag>
    {
        template<typename Sequence>
        struct apply;

        #define M0(z,n,data)                                                                        \
        template<BOOST_PP_ENUM_PARAMS_Z(z, n, unsigned C)>                                          \
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)> >                                 \
        {                                                                                           \
            BOOST_MPL_ASSERT_MSG((C0 != 0), POP_BACK_FAILED_MPL_STRING_IS_EMPTY, (mpl::string<>));  \
            typedef                                                                                 \
                mpl::string<                                                                        \
                    BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_DEC(n), C)                                   \
                    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                              \
                    (BOOST_PP_CAT(C,BOOST_PP_DEC(n))>>8)                                            \
                >                                                                                   \
            type;                                                                                   \
        };

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_MPL_STRING_MAX_PARAMS), M0, ~)
        #undef M0
    };

    template<typename Tag>
    struct push_front_impl;

    template<>
    struct push_front_impl<mpl::string_tag>
    {
        template<typename Sequence, typename Value, bool B = (4==BOOST_MPL_MULTICHAR_LENGTH(Sequence::front_))>
        struct apply
        {
            BOOST_MPL_ASSERT_MSG(
                (BOOST_MPL_STRING_MAX_LENGTH != mpl::size<Sequence>::type::value)
              , PUSH_FRONT_FAILED_MPL_STRING_IS_FULL
              , (Sequence)
            );
            // If the above assertion didn't fire, then the string is sparse.
            // Repack the string and retry the push_front.
            typedef
                typename mpl::push_front<
                    typename mpl::reverse_copy<
                        Sequence
                      , mpl::front_inserter<string<> >
                    >::type
                  , Value
                >::type
            type;
        };

        template<typename Value>
        struct apply<mpl::string<>, Value, false>
        {
            typedef mpl::string<(char)Value::value> type;
        };

        #define M0(z,n,data)                                                                        \
        template<BOOST_PP_ENUM_PARAMS_Z(z, n, unsigned C), typename Value>                          \
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)>, Value, true>                     \
        {                                                                                           \
            typedef                                                                                 \
                mpl::string<                                                                        \
                    (char)Value::value                                                              \
                    BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, C)                                        \
                >                                                                                   \
            type;                                                                                   \
        };

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_STRING_MAX_PARAMS, M0, ~)
        #undef M0

        template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, unsigned C), typename Value>
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>, Value, false>
        {
            typedef
                mpl::string<
                    ((((unsigned char)Value::value)<<(BOOST_MPL_MULTICHAR_LENGTH(C0)*8))|C0)
                  , BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)
                >
            type;
        };
    };

    template<typename Tag>
    struct pop_front_impl;

    template<>
    struct pop_front_impl<mpl::string_tag>
    {
        template<typename Sequence, bool B = (1==BOOST_MPL_MULTICHAR_LENGTH(Sequence::front_))>
        struct apply;

        #define M0(z,n,data)                                                                        \
        template<BOOST_PP_ENUM_PARAMS_Z(z, n, unsigned C)>                                          \
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS_Z(z, n, C)>, true>                            \
        {                                                                                           \
            BOOST_MPL_ASSERT_MSG((C0 != 0), POP_FRONT_FAILED_MPL_STRING_IS_EMPTY, (mpl::string<>)); \
            typedef                                                                                 \
                mpl::string<BOOST_PP_ENUM_SHIFTED_PARAMS_Z(z, n, C)>                                \
            type;                                                                                   \
        };

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_STRING_MAX_PARAMS, M0, ~)
        #undef M0

        template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, unsigned C)>
        struct apply<mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>, false>
        {
            typedef
                mpl::string<
                    (((1<<((BOOST_MPL_MULTICHAR_LENGTH(C0)-1)*8))-1)&C0)
                  , BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)
                >
            type;
        };
    };

    template<typename Tag>
    struct insert_range_impl;

    template<>
    struct insert_range_impl<mpl::string_tag>
    {
        template<typename Sequence, typename Pos, typename Range>
        struct apply
          : mpl::copy<
                mpl::joint_view<
                    mpl::iterator_range<
                        mpl::string_iterator<Sequence, 0, 0>
                      , Pos
                    >
                  , mpl::joint_view<
                        Range
                      , mpl::iterator_range<
                            Pos
                          , typename mpl::end<Sequence>::type
                        >
                    >
                >
              , mpl::back_inserter<mpl::string<> >
            >
        {};
    };

    template<typename Tag>
    struct insert_impl;

    template<>
    struct insert_impl<mpl::string_tag>
    {
        template<typename Sequence, typename Pos, typename Value>
        struct apply
          : mpl::insert_range<Sequence, Pos, mpl::string<(char)Value::value> >
        {};
    };

    template<typename Tag>
    struct erase_impl;

    template<>
    struct erase_impl<mpl::string_tag>
    {
        template<typename Sequence, typename First, typename Last>
        struct apply
          : mpl::copy<
                mpl::joint_view<
                    mpl::iterator_range<
                        mpl::string_iterator<Sequence, 0, 0>
                      , First
                    >
                  , mpl::iterator_range<
                        typename mpl::if_na<Last, typename mpl::next<First>::type>::type
                      , typename mpl::end<Sequence>::type
                    >
                >
              , mpl::back_inserter<mpl::string<> >
            >
        {};
    };

    template<typename Tag>
    struct clear_impl;

    template<>
    struct clear_impl<mpl::string_tag>
    {
        template<typename>
        struct apply
        {
            typedef mpl::string<> type;
        };
    };

    #define M0(z, n, data)                                                                            \
    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, unsigned C), unsigned J>               \
    struct string_iterator<mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>, n, J>   \
    {                                                                                                 \
        typedef mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> string;             \
        typedef std::bidirectional_iterator_tag category;                                             \
        typedef                                                                                       \
            typename mpl::if_c<                                                                       \
                (BOOST_MPL_MULTICHAR_LENGTH(BOOST_PP_CAT(C, n)) == J + 1)                             \
              , mpl::string_iterator<string, n + 1, 0>                                                \
              , mpl::string_iterator<string, n, J + 1>                                                \
            >::type                                                                                   \
        next;                                                                                         \
        typedef                                                                                       \
            mpl::string_iterator<string, n, J-1>                                                      \
        prior;                                                                                        \
        typedef mpl::char_<BOOST_MPL_MULTICHAR_AT(BOOST_PP_CAT(C, n), J)> type;                       \
    };                                                                                                \
    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, unsigned C)>                           \
    struct string_iterator<mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)>, n, 0>   \
    {                                                                                                 \
        typedef mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> string;             \
        typedef std::bidirectional_iterator_tag category;                                             \
        typedef                                                                                       \
            typename mpl::if_c<                                                                       \
                (BOOST_MPL_MULTICHAR_LENGTH(BOOST_PP_CAT(C, n)) == 1)                                 \
              , mpl::string_iterator<string, n + 1, 0>                                                \
              , mpl::string_iterator<string, n, 1>                                                    \
            >::type                                                                                   \
        next;                                                                                         \
        typedef                                                                                       \
            mpl::string_iterator<                                                                     \
                string                                                                                \
              , n - 1                                                                                 \
              , BOOST_MPL_MULTICHAR_LENGTH(BOOST_PP_CAT(C, BOOST_PP_DEC(n))) - 1                      \
            >                                                                                         \
        prior;                                                                                        \
        typedef mpl::char_<BOOST_MPL_MULTICHAR_AT(BOOST_PP_CAT(C, n), 0)> type;                       \
    };

    BOOST_PP_REPEAT(BOOST_MPL_STRING_MAX_PARAMS, M0, ~)
    #undef M0

    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, unsigned C)>
    struct string
    {
        /// INTERNAL ONLY
        enum
        {
            front_  = C0
          , back_   = BOOST_PP_CAT(C, BOOST_PP_DEC(BOOST_MPL_STRING_MAX_PARAMS))
        };

        typedef string      type;
        typedef string_tag  tag;
    };

    namespace aux_
    {
        template<typename It, typename End>
        struct next_unless
          : mpl::next<It>
        {};

        template<typename End>
        struct next_unless<End, End>
        {
            typedef End type;
        };

        template<typename It, typename End>
        struct deref_unless
          : mpl::deref<It>
        {};

        template<typename End>
        struct deref_unless<End, End>
        {
            typedef mpl::char_<'\0'> type;
        };
    }

    template<typename Sequence>
    struct c_str
    {
        typedef typename mpl::end<Sequence>::type iend;
        typedef typename mpl::begin<Sequence>::type i0;
        #define M0(z, n, data)                                                                      \
        typedef                                                                                     \
            typename mpl::aux_::next_unless<BOOST_PP_CAT(i, n), iend>::type                         \
        BOOST_PP_CAT(i, BOOST_PP_INC(n));
        BOOST_PP_REPEAT(BOOST_MPL_STRING_MAX_LENGTH, M0, ~)
        #undef M0

        typedef c_str type;
        static char const value[];
    };

    template<typename Sequence>
    char const c_str<Sequence>::value[] =
    {
        #define M0(z, n, data)                                                                      \
        mpl::aux_::deref_unless<BOOST_PP_CAT(i, n), iend>::type::value,
        BOOST_PP_REPEAT(BOOST_MPL_STRING_MAX_LENGTH, M0, ~)
        #undef M0
        '\0'
    };

}} // namespace boost

#endif // BOOST_MPL_STRING_HPP_INCLUDED
