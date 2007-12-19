///////////////////////////////////////////////////////////////////////////////
/// \file make.hpp
/// Contains definition of the make<> transform.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_MAKE_HPP_EAN_12_02_2007
#define BOOST_PROTO_TRANSFORM_MAKE_HPP_EAN_12_02_2007

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/aux_/has_type.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/traits.hpp>
#include <boost/xpressive/proto/args.hpp>

namespace boost { namespace proto
{

    namespace transform
    {
        namespace detail
        {
            template<typename... T>
            struct typelist
            {
                typedef void type;
            };

            template<typename T, bool HasType = mpl::aux::has_type<T>::value>
            struct nested_type
            {
                typedef typename T::type type;
            };

            template<typename T>
            struct nested_type<T, false>
            {
                typedef T type;
            };

            template<typename T, typename Args, typename EnableIf = void>
            struct nested_type_if
              : nested_type<T>
            {};

            template<typename T, typename... Args>
            struct nested_type_if<T, typelist<Args...>, typename typelist<typename Args::not_applied_...>::type>
            {
                typedef T type;
                typedef void not_applied_;
            };

            template<typename R, typename Expr, typename State, typename Visitor
                , bool IsTransform = is_callable<R>::value
            >
            struct make_if_;

            template<typename R, typename Expr, typename State, typename Visitor>
            struct make_
            {
                typedef R type;
                typedef void not_applied_;
            };

            template<template<typename...> class R, typename... Args, typename Expr, typename State, typename Visitor>
            struct make_<R<Args...>, Expr, State, Visitor>
              : nested_type_if<
                    R<typename make_if_<Args, Expr, State, Visitor>::type...>
                  , typelist<make_if_<Args, Expr, State, Visitor>...>
                >
            {};

            template<typename R, typename Expr, typename State, typename Visitor>
            struct make_if_<R, Expr, State, Visitor, false>
              : make_<R, Expr, State, Visitor>
            {};

            template<typename R, typename Expr, typename State, typename Visitor>
            struct make_if_<R, Expr, State, Visitor, true>
              : remove_const<typename remove_reference<
                    typename boost::result_of<R(Expr, State, Visitor)>::type
                >::type>
            {};

            template<typename R, typename... Args, typename Expr, typename State, typename Visitor>
            struct make_if_<R(Args...), Expr, State, Visitor, false>
              : remove_const<typename remove_reference<
                    typename boost::result_of<when<_, R(Args...)>(Expr, State, Visitor)>::type
                >::type>
            {};

            template<typename R, typename... Args, typename Expr, typename State, typename Visitor>
            struct make_if_<R(*)(Args...), Expr, State, Visitor, false>
              : remove_const<typename remove_reference<
                    typename boost::result_of<when<_, R(Args...)>(Expr, State, Visitor)>::type
                >::type>
            {};

            //// work around GCC bug
            //template<typename Tag, typename Args, long N, typename Expr, typename State, typename Visitor>
            //struct make_if_<expr<Tag, Args, N>, Expr, State, Visitor, false>
            //{
            //    typedef expr<Tag, Args, N> type;
            //    typedef void not_applied_;
            //};

            template<typename Type, bool IsAggregate = is_aggregate<Type>::value>
            struct construct_
            {
                typedef Type result_type;

                template<typename... Args>
                Type operator()(Args &&... args) const
                {
                    return Type(args...);
                }
            };

            template<typename Type>
            struct construct_<Type, true>
            {
                typedef Type result_type;

                template<typename... Args>
                Type operator()(Args &&... args) const
                {
                    Type that = {args...};
                    return that;
                }

                Type operator()() const
                {
                    return Type();
                }
            };

            template<typename T, typename A, long N>
            struct construct_<expr<T, A, N>, true>
            {
                typedef expr<T, A, N> result_type;

                template<typename... Args>
                result_type operator()(Args &&... args) const
                {
                    return result_type::make(args...);
                }
            };

            template<typename Type, typename... Args>
            Type construct(Args &&... args)
            {
                return construct_<Type>()(args...);
            }
        }

        template<typename Return, typename... Args>
        struct make : callable
        {
            template<typename Sig>
            struct result;

            template<typename This, typename Expr, typename State, typename Visitor>
            struct result<This(Expr, State, Visitor)>
              : detail::make_<Return, Expr, State, Visitor>
            {};

            template<typename Expr, typename State, typename Visitor>
            typename result<make(Expr const &, State const &, Visitor &)>::type
            operator()(Expr const &expr, State const &state, Visitor &visitor) const
            {
                typedef typename result<make(Expr const &, State const &, Visitor &)>::type result_type;
                return detail::construct<result_type>(when<_, Args>()(expr, state, visitor)...);
            }
        };

        //// work around gcc bug
        //template<typename T, typename A, long N, typename... Args>
        //struct make<expr<T, A, N>, Args...> : callable
        //{
        //    template<typename Sig>
        //    struct result
        //    {
        //        typedef expr<T, A, N> type;
        //    };

        //    template<typename Expr, typename State, typename Visitor>
        //    expr<T, A, N> operator()(Expr const &expr, State const &state, Visitor &visitor) const
        //    {
        //        return proto::expr<T, A, N>::make(when<_, Args>()(expr, state, visitor)...);
        //    }
        //};

        template<typename Fun, typename... Args>
        struct make<Fun(Args...)>
          : make<Fun, Args...>
        {};

    }

    template<typename Fun, typename... Args>
    struct is_callable<transform::make<Fun, Args...> >
      : mpl::true_
    {};

}}

#endif
