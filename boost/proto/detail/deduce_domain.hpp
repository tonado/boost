///////////////////////////////////////////////////////////////////////////////
/// \file deduce_domain.hpp
/// Contains definition of deduce_domain\<\> class templates 
/// for finding the domain that is common among the specified
/// domains
//
//  Copyright 2010 Daniel Wallin, Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Many thanks to Daniel Wallin who first implemented this code. Thanks
// also to Jeremiah Willcock, John Bytheway and Krishna Achuthan who
// offered alternate solutions to this tricky programming problem.

#ifndef BOOST_PROTO_DEDUCE_DOMAIN_HPP_EAN_05_22_2010
#define BOOST_PROTO_DEDUCE_DOMAIN_HPP_EAN_05_22_2010

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/proto/proto_fwd.hpp>

#ifndef BOOST_PROTO_ASSERT_VALID_DOMAIN
# define BOOST_PROTO_ASSERT_VALID_DOMAIN(DOM) BOOST_MPL_ASSERT_NOT((boost::is_same<DOM, boost::proto::detail::not_a_domain>))
#endif

namespace boost
{
    namespace proto
    {
        namespace detail
        {
            template<int N>
            struct sized_type
            {
                typedef char (&type)[N];
            };

            template<typename Domain>
            struct domain_
              : domain_<typename Domain::proto_super_domain>
            {
                typedef Domain type;
                typedef domain_<typename Domain::proto_super_domain> base;
            #ifdef BOOST_NO_DECLTYPE
                using base::deduce98;
                static int const index = base::index + 1;
                static typename sized_type<index>::type deduce98(domain_<Domain>*);
            #else
                using base::deduce0x;
                static Domain deduce0x(domain_<Domain>*);
            #endif
            };

            template<>
            struct domain_<not_a_domain>
            {
                typedef not_a_domain type;
            #ifdef BOOST_NO_DECLTYPE
                static int const index = 1;
                static sized_type<1>::type deduce98(void*);
            #else
                static not_a_domain deduce0x(void*);
            #endif
            };

            template<>
            struct domain_<default_domain>
              : domain_<not_a_domain>
            {};

            sized_type<1>::type default_test(void*, void*);
            sized_type<2>::type default_test(domain_<default_domain>*, void*);
            sized_type<3>::type default_test(void*, domain_<default_domain>*);
            sized_type<4>::type default_test(domain_<default_domain>*, domain_<default_domain>*);

        #ifdef BOOST_NO_DECLTYPE
            template<int N, typename Domain>
            struct nth_domain
              : nth_domain<N - 2, typename Domain::base::base>
            {};

            template<typename Domain>
            struct nth_domain<0, Domain>
              : Domain
            {};

            template<typename Domain>
            struct nth_domain<1, Domain>
              : Domain::base
            {};
        #endif

            template<typename D0>
            struct common_domain1
            {
                typedef D0 type;
            };

            template<typename E0>
            struct deduce_domain1
              : domain_of<E0>
            {};

            template<
                typename D0
              , typename D1
              , int DefaultCase = sizeof(proto::detail::default_test((domain_<D0>*)0, (domain_<D1>*)0))
            >
            struct common_domain2
            {
            #ifdef BOOST_NO_DECLTYPE
                static int const index = domain_<D0>::index - sizeof(domain_<D0>::deduce98((domain_<D1>*)0));
                typedef typename nth_domain<index, domain_<D0> >::type type;
            #else
                typedef decltype(domain_<D0>::deduce0x((domain_<D1>*)0)) type;
            #endif
            };

            template<typename D0, typename D1>
            struct common_domain2<D0, D1, 2>
            {
                typedef D1 type;
            };

            template<typename D0, typename D1>
            struct common_domain2<D0, D1, 3>
            {
                typedef D0 type;
            };

            template<typename D0>
            struct common_domain2<D0, default_domain, 4>
            {
                typedef D0 type;
            };

            template<typename D1>
            struct common_domain2<default_domain, D1, 4>
            {
                typedef D1 type;
            };

            template<>
            struct common_domain2<default_domain, default_domain, 4>
            {
                typedef default_domain type;
            };

            template<typename E0, typename E1>
            struct deduce_domain2
              : common_domain2<
                    typename domain_of<E0>::type
                  , typename domain_of<E1>::type
                >
            {};

            #define M0(Z, N, DATA)                                                                  \
                typedef                                                                             \
                    typename common_domain2<common ## N, A ## N>::type                              \
                BOOST_PP_CAT(common, BOOST_PP_INC(N));                                              \
                /**/

            #define BOOST_PP_LOCAL_MACRO(N)                                                         \
                template<BOOST_PP_ENUM_PARAMS(N, typename A)>                                       \
                struct BOOST_PP_CAT(common_domain, N)                                               \
                {                                                                                   \
                    typedef A0 common1;                                                             \
                    BOOST_PP_REPEAT_FROM_TO(1, N, M0, ~)                                            \
                    typedef common ## N type;                                                       \
                    BOOST_PROTO_ASSERT_VALID_DOMAIN(type);                                          \
                };                                                                                  \
                                                                                                    \
                template<BOOST_PP_ENUM_PARAMS(N, typename E)>                                       \
                struct BOOST_PP_CAT(deduce_domain, N)                                               \
                  : BOOST_PP_CAT(common_domain, N)<                                                 \
                        BOOST_PP_ENUM_BINARY_PARAMS(                                                \
                            N                                                                       \
                          , typename domain_of<E, >::type BOOST_PP_INTERCEPT                        \
                        )                                                                           \
                    >                                                                               \
                {};                                                                                 \
                /**/

            #define BOOST_PP_LOCAL_LIMITS (3, BOOST_PROTO_MAX_ARITY)
            #include BOOST_PP_LOCAL_ITERATE()

            #undef M0
        }
    }
}

#endif // BOOST_PROTO_DEDUCE_DOMAIN_HPP_EAN_05_22_2010
