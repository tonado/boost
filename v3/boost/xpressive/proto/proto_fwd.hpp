///////////////////////////////////////////////////////////////////////////////
/// \file proto_fwd.hpp
/// Contains proto's forward declarations
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_PROTO_FWD_EAN_10_28_2007
#define BOOST_PROTO_PROTO_FWD_EAN_10_28_2007

#include <climits> // for INT_MAX
#include <boost/config.hpp>
#include <boost/preprocessor.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/aux_/template_arity.hpp>
#include <boost/mpl/aux_/lambda_arity_param.hpp>

#ifndef BOOST_PROTO_MAX_ARITY
# define BOOST_PROTO_MAX_ARITY 5
#endif

#ifndef BOOST_PROTO_MAX_LOGICAL_ARITY
# define BOOST_PROTO_MAX_LOGICAL_ARITY 8
#endif

#if BOOST_WORKAROUND(__GNUC__, == 3) \
 || BOOST_WORKAROUND(__EDG_VERSION__, BOOST_TESTED_AT(306))
# define BOOST_PROTO_BROKEN_CONST_OVERLOADS
#endif

#ifdef BOOST_PROTO_BROKEN_CONST_OVERLOADS
# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/is_const.hpp>
# define BOOST_PROTO_DISABLE_IF_IS_CONST(T)\
    , typename boost::disable_if<boost::is_const<T> >::type * = 0
#else
# define BOOST_PROTO_DISABLE_IF_IS_CONST(T)
#endif

namespace boost { namespace proto
{
    namespace wildns_
    {
        struct _;
    }

    using wildns_::_;

    namespace generatorns_
    {
        struct default_generator;

        template<template<typename> class Extends>
        struct generator;

        template<template<typename> class Extends>
        struct pod_generator;

        template<typename Generator = default_generator>
        struct by_value_generator;
    }

    using generatorns_::default_generator;
    using generatorns_::generator;
    using generatorns_::pod_generator;
    using generatorns_::by_value_generator;

    namespace domainns_
    {
        template<typename Generator = default_generator, typename Grammar = proto::_>
        struct domain;

        struct default_domain;

        struct deduce_domain;
    }

    using domainns_::domain;
    using domainns_::default_domain;
    using domainns_::deduce_domain;

    namespace argsns_
    {
        #ifdef BOOST_HAS_VARIADIC_TMPL
        template<typename... Args>
        struct cons;

        template<typename... Args>
        struct args;
        #else
        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PROTO_MAX_ARITY, typename A, void), typename Dummy = void>
        struct cons;

        template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PROTO_MAX_ARITY, typename A, void), typename Dummy = void>
        struct args;
        #endif

        template<typename Arg>
        struct term;
    }

    using argsns_::args;
    using argsns_::term;

    namespace exprns_
    {
        template<typename Tag, typename Args, long Arity = Args::size>
        struct expr;

        template<typename Expr, typename Derived, typename Domain = default_domain>
        struct extends;

        struct is_proto_expr;

        template<typename Expr, typename A>
        Expr construct(A const &a, typename boost::disable_if<is_function<A> >::type * = 0);

    #ifdef BOOST_HAS_VARIADIC_TMPL
        template<typename Expr, typename... A>
        Expr construct(A &... a);
    #else
        template<typename Expr, typename A>
        Expr construct(A &a BOOST_PROTO_DISABLE_IF_IS_CONST(A));
    #define TMP(Z, N, DATA)                                                                         \
        template<typename Expr BOOST_PP_ENUM_TRAILING_PARAMS_Z(Z, N, typename A)>                   \
        Expr construct(BOOST_PP_ENUM_BINARY_PARAMS_Z(Z, N, A, &a));                           \
        /**/
        BOOST_PP_REPEAT_FROM_TO(2, BOOST_PP_INC(BOOST_PROTO_MAX_ARITY), TMP, ~)
    #undef TMP
    #endif

    }

    using exprns_::expr;
    using exprns_::extends;
    using exprns_::is_proto_expr;
    using exprns_::construct;

    namespace tag
    {
        struct terminal;
        struct posit;
        struct negate;
        struct dereference;
        struct complement;
        struct address_of;
        struct logical_not;
        struct pre_inc;
        struct pre_dec;
        struct post_inc;
        struct post_dec;
        struct shift_left;
        struct shift_right;
        struct multiplies;
        struct divides;
        struct modulus;
        struct plus;
        struct minus;
        struct less;
        struct greater;
        struct less_equal;
        struct greater_equal;
        struct equal_to;
        struct not_equal_to;
        struct logical_or;
        struct logical_and;
        struct bitwise_and;
        struct bitwise_or;
        struct bitwise_xor;
        struct comma;
        struct mem_ptr;
        struct assign;
        struct shift_left_assign;
        struct shift_right_assign;
        struct multiplies_assign;
        struct divides_assign;
        struct modulus_assign;
        struct plus_assign;
        struct minus_assign;
        struct bitwise_and_assign;
        struct bitwise_or_assign;
        struct bitwise_xor_assign;
        struct subscript;
        struct if_else_;
        struct function;

        struct proto_expr;
        struct proto_expr_iterator;
        struct proto_flat_view;
    }

    namespace utility
    {
        template<typename T, typename Domain = default_domain>
        struct literal;
    }

    using utility::literal;

    namespace result_of
    {
        template<typename Expr>
        struct tag_of
        {
            typedef typename Expr::proto_tag type;
        };

        template<typename Expr, long N>
        struct value_at_c;

        template<typename Expr, long N>
        struct arg_c;

        template<typename Expr>
        struct left;

        template<typename Expr>
        struct right;

        template<typename T, typename Domain = default_domain, typename EnableIf = void>
        struct as_expr;

        template<typename T, typename Domain = default_domain, typename EnableIf = void>
        struct as_expr_ref;

        template<typename T, typename EnableIf = void>
        struct is_expr;

        template<typename Expr, typename Grammar>
        struct matches;

        template<typename T, typename EnableIf = void>
        struct is_domain;

        template<typename T, typename EnableIf = void>
        struct domain_of;

        template<typename Tag, typename DomainOrSequence, typename SequenceOrVoid = void, typename EnableIf = void>
        struct unpack_expr_ref;

        template<typename Tag, typename DomainOrSequence, typename SequenceOrVoid = void, typename EnableIf = void>
        struct unpack_expr;

        #ifdef BOOST_HAS_VARIADIC_TMPL
        template<typename Tag, typename... Args>
        struct make_expr_ref;

        template<typename Tag, typename... Args>
        struct make_expr;
        #else
        template<
            typename Tag
          , typename DomainOrArg
            BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                BOOST_PROTO_MAX_ARITY
              , typename A
              , = void BOOST_PP_INTERCEPT
            )
          , typename Dummy = void
        >
        struct make_expr_ref;

        template<
            typename Tag
          , typename DomainOrArg
            BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                BOOST_PROTO_MAX_ARITY
              , typename A
              , = void BOOST_PP_INTERCEPT
            )
          , typename Dummy = void
        >
        struct make_expr;
        #endif

        template<typename Expr>
        struct deep_copy;
    }

    using result_of::matches;
    using result_of::tag_of;
    using result_of::is_domain;
    using result_of::domain_of;
    using result_of::is_expr;

    template<long N, typename Expr>
    typename result_of::arg_c<Expr, N>::type
    arg_c(Expr &expr BOOST_PROTO_DISABLE_IF_IS_CONST(Expr));

    template<long N, typename Expr>
    typename result_of::arg_c<Expr const, N>::type
    arg_c(Expr const &expr);

    #ifdef BOOST_HAS_RVALUE_REFS
    template<typename T>
    typename result_of::as_expr_ref<T>::type const
    as_expr_ref(T &&t);
    #else
    template<typename T>
    typename result_of::as_expr_ref<T &>::type const
    as_expr_ref(T &t BOOST_PROTO_DISABLE_IF_IS_CONST(T));

    template<typename T>
    typename result_of::as_expr_ref<T const &>::type const
    as_expr_ref(T const &t);
    #endif

    namespace op
    {
        template<typename T> struct terminal;
        template<typename T> struct posit;
        template<typename T> struct negate;
        template<typename T> struct dereference;
        template<typename T> struct complement;
        template<typename T> struct address_of;
        template<typename T> struct logical_not;
        template<typename T> struct pre_inc;
        template<typename T> struct pre_dec;
        template<typename T> struct post_inc;
        template<typename T> struct post_dec;
        template<typename T, typename U> struct shift_left;
        template<typename T, typename U> struct shift_right;
        template<typename T, typename U> struct multiplies;
        template<typename T, typename U> struct divides;
        template<typename T, typename U> struct modulus;
        template<typename T, typename U> struct plus;
        template<typename T, typename U> struct minus;
        template<typename T, typename U> struct less;
        template<typename T, typename U> struct greater;
        template<typename T, typename U> struct less_equal;
        template<typename T, typename U> struct greater_equal;
        template<typename T, typename U> struct equal_to;
        template<typename T, typename U> struct not_equal_to;
        template<typename T, typename U> struct logical_or;
        template<typename T, typename U> struct logical_and;
        template<typename T, typename U> struct bitwise_and;
        template<typename T, typename U> struct bitwise_or;
        template<typename T, typename U> struct bitwise_xor;
        template<typename T, typename U> struct comma;
        template<typename T, typename U> struct mem_ptr;
        template<typename T, typename U> struct assign;
        template<typename T, typename U> struct shift_left_assign;
        template<typename T, typename U> struct shift_right_assign;
        template<typename T, typename U> struct multiplies_assign;
        template<typename T, typename U> struct divides_assign;
        template<typename T, typename U> struct modulus_assign;
        template<typename T, typename U> struct plus_assign;
        template<typename T, typename U> struct minus_assign;
        template<typename T, typename U> struct bitwise_and_assign;
        template<typename T, typename U> struct bitwise_or_assign;
        template<typename T, typename U> struct bitwise_xor_assign;
        template<typename T, typename U> struct subscript;
        template<typename T, typename U, typename V> struct if_else_;
        template<typename Tag, typename T> struct unary_expr;
        template<typename Tag, typename T, typename U> struct binary_expr;

        #if defined(BOOST_HAS_VARIADIC_TMPL) && defined(BOOST_HAS_RVALUE_REFS)
        template<typename... Args> struct function;
        template<typename Tag, typename... Args> struct nary_expr;
        #else
        template<
            BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PROTO_MAX_ARITY, typename A, void)
          , typename Dummy = void
        >
        struct function;

        template<
            typename Tag
            BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                BOOST_PROTO_MAX_ARITY
              , typename A
              , = void BOOST_PP_INTERCEPT
            )
          , typename Dummy = void
        >
        struct nary_expr;
        #endif
    }

    using namespace op;

    namespace control
    {
        namespace detail
        {
            template<typename T, typename EnableIf = void>
            struct is_vararg;
        }

        #ifdef BOOST_HAS_VARIADIC_TMPL
        template<typename... Alts>
        struct or_;

        template<typename... Alts>
        struct and_;
        #else
        template<
            typename Grammar0
          , typename Grammar1
            BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                BOOST_PP_SUB(BOOST_PROTO_MAX_LOGICAL_ARITY,2)
              , typename G
              , = void BOOST_PP_INTERCEPT
            )
        >
        struct or_;

        template<
            typename Grammar0
          , typename Grammar1
            BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(
                BOOST_PP_SUB(BOOST_PROTO_MAX_LOGICAL_ARITY,2)
              , typename G
              , = void BOOST_PP_INTERCEPT
            )
        >
        struct and_;
        #endif

        template<typename Grammar>
        struct not_;

        template<typename If, typename Then = _, typename Else = not_<_> >
        struct if_;

        template<typename Cases>
        struct switch_;

        template<typename T>
        struct exact;

        template<typename T>
        struct convertible_to;

        template<typename Grammar>
        struct vararg;

        int const N = INT_MAX;
    }

    using control::or_;
    using control::and_;
    using control::if_;
    using control::not_;
    using control::switch_;
    using control::exact;
    using control::convertible_to;
    using control::vararg;
    using control::N;

    template<typename T>
    struct is_callable;

    template<typename T>
    struct is_aggregate;

    namespace transform
    {
        #define BOOST_PROTO_CALLABLE() typedef void proto_is_callable_;

        struct callable
        {
            BOOST_PROTO_CALLABLE()
        };

        template<typename Grammar, typename Fun = Grammar>
        struct when;

        template<typename Fun>
        struct otherwise;

        template<typename Fun>
        struct call;

        template<typename Fun>
        struct make;

        template<typename Fun>
        struct bind;

        template<typename Sequence, typename State, typename Fun>
        struct fold;

        template<typename Sequence, typename State, typename Fun>
        struct reverse_fold;

        // BUGBUG can we replace fold_tree with fold<flatten(_), state, fun> ?
        template<typename Sequence, typename State, typename Fun>
        struct fold_tree;

        template<typename Sequence, typename State, typename Fun>
        struct reverse_fold_tree;

        struct _expr;
        struct _state;
        struct _visitor;

        template<int I>
        struct _arg_c;

        struct _arg0;
        struct _arg1;
        struct _arg2;
        struct _arg3;
        struct _arg4;
        struct _arg5;
        struct _arg6;
        struct _arg7;
        struct _arg8;
        struct _arg9;

        typedef _arg0 _arg;
        typedef _arg0 _left;
        typedef _arg1 _right;
    }

    using transform::when;
    using transform::otherwise;
    using transform::_arg0;
    using transform::_arg1;
    using transform::_arg2;
    using transform::_arg3;
    using transform::_arg4;
    using transform::_arg5;
    using transform::_arg6;
    using transform::_arg7;
    using transform::_arg8;
    using transform::_arg9;
    using transform::_arg;
    using transform::_left;
    using transform::_right;
    using transform::_expr;
    using transform::_state;
    using transform::_visitor;
    using transform::_arg_c;
    using transform::call;
    using transform::make;
    using transform::bind;
    using transform::fold;
    using transform::reverse_fold;
    using transform::fold_tree;
    using transform::reverse_fold_tree;
    using transform::callable;

    namespace context
    {
        struct null_context;

        template<typename Expr, typename Context, long Arity = Expr::proto_arity>
        struct null_eval;

        struct default_context;

        template<typename Expr, typename Context, typename Tag = typename Expr::proto_tag>
        struct default_eval;

        template<typename Derived, typename DefaultCtx = default_context>
        struct callable_context;

        template<typename Expr, typename Context, long Arity = Expr::proto_arity>
        struct callable_eval;
    }

    using context::null_context;
    using context::null_eval;
    using context::default_context;
    using context::default_eval;
    using context::callable_context;
    using context::callable_eval;

    namespace functional
    {
        struct left;
        struct right;
        struct deep_copy;

        template<typename Domain = default_domain>
        struct as_expr;

        template<typename Domain = default_domain>
        struct as_expr_ref;

        template<typename N = mpl::long_<0> >
        struct arg;

        template<long N>
        struct arg_c;

        template<typename Tag, typename Domain = deduce_domain>
        struct make_expr_ref;

        template<typename Tag, typename Domain = deduce_domain>
        struct unpack_expr_ref;

        template<typename Tag, typename Domain = deduce_domain>
        struct make_expr;

        template<typename Tag, typename Domain = deduce_domain>
        struct unpack_expr;

        typedef make_expr<tag::terminal>            make_terminal;
        typedef make_expr<tag::posit>               make_posit;
        typedef make_expr<tag::negate>              make_negate;
        typedef make_expr<tag::dereference>         make_dereference;
        typedef make_expr<tag::complement>          make_complement;
        typedef make_expr<tag::address_of>          make_address_of;
        typedef make_expr<tag::logical_not>         make_logical_not;
        typedef make_expr<tag::pre_inc>             make_pre_inc;
        typedef make_expr<tag::pre_dec>             make_pre_dec;
        typedef make_expr<tag::post_inc>            make_post_inc;
        typedef make_expr<tag::post_dec>            make_post_dec;
        typedef make_expr<tag::shift_left>          make_shift_left;
        typedef make_expr<tag::shift_right>         make_shift_right;
        typedef make_expr<tag::multiplies>          make_multiplies;
        typedef make_expr<tag::divides>             make_divides;
        typedef make_expr<tag::modulus>             make_modulus;
        typedef make_expr<tag::plus>                make_plus;
        typedef make_expr<tag::minus>               make_minus;
        typedef make_expr<tag::less>                make_less;
        typedef make_expr<tag::greater>             make_greater;
        typedef make_expr<tag::less_equal>          make_less_equal;
        typedef make_expr<tag::greater_equal>       make_greater_equal;
        typedef make_expr<tag::equal_to>            make_equal_to;
        typedef make_expr<tag::not_equal_to>        make_not_equal_to;
        typedef make_expr<tag::logical_or>          make_logical_or;
        typedef make_expr<tag::logical_and>         make_logical_and;
        typedef make_expr<tag::bitwise_and>         make_bitwise_and;
        typedef make_expr<tag::bitwise_or>          make_bitwise_or;
        typedef make_expr<tag::bitwise_xor>         make_bitwise_xor;
        typedef make_expr<tag::comma>               make_comma;
        typedef make_expr<tag::mem_ptr>             make_mem_ptr;
        typedef make_expr<tag::assign>              make_assign;
        typedef make_expr<tag::shift_left_assign>   make_shift_left_assign;
        typedef make_expr<tag::shift_right_assign>  make_shift_right_assign;
        typedef make_expr<tag::multiplies_assign>   make_multiplies_assign;
        typedef make_expr<tag::divides_assign>      make_divides_assign;
        typedef make_expr<tag::modulus_assign>      make_modulus_assign;
        typedef make_expr<tag::plus_assign>         make_plus_assign;
        typedef make_expr<tag::minus_assign>        make_minus_assign;
        typedef make_expr<tag::bitwise_and_assign>  make_bitwise_and_assign;
        typedef make_expr<tag::bitwise_or_assign>   make_bitwise_or_assign;
        typedef make_expr<tag::bitwise_xor_assign>  make_bitwise_xor_assign;
        typedef make_expr<tag::subscript>           make_subscript;
        typedef make_expr<tag::if_else_>            make_if_else;
        typedef make_expr<tag::function>            make_function;

        typedef make_expr_ref<tag::terminal>            make_terminal_ref;
        typedef make_expr_ref<tag::posit>               make_posit_ref;
        typedef make_expr_ref<tag::negate>              make_negate_ref;
        typedef make_expr_ref<tag::dereference>         make_dereference_ref;
        typedef make_expr_ref<tag::complement>          make_complement_ref;
        typedef make_expr_ref<tag::address_of>          make_address_of_ref;
        typedef make_expr_ref<tag::logical_not>         make_logical_not_ref;
        typedef make_expr_ref<tag::pre_inc>             make_pre_inc_ref;
        typedef make_expr_ref<tag::pre_dec>             make_pre_dec_ref;
        typedef make_expr_ref<tag::post_inc>            make_post_inc_ref;
        typedef make_expr_ref<tag::post_dec>            make_post_dec_ref;
        typedef make_expr_ref<tag::shift_left>          make_shift_left_ref;
        typedef make_expr_ref<tag::shift_right>         make_shift_right_ref;
        typedef make_expr_ref<tag::multiplies>          make_multiplies_ref;
        typedef make_expr_ref<tag::divides>             make_divides_ref;
        typedef make_expr_ref<tag::modulus>             make_modulus_ref;
        typedef make_expr_ref<tag::plus>                make_plus_ref;
        typedef make_expr_ref<tag::minus>               make_minus_ref;
        typedef make_expr_ref<tag::less>                make_less_ref;
        typedef make_expr_ref<tag::greater>             make_greater_ref;
        typedef make_expr_ref<tag::less_equal>          make_less_equal_ref;
        typedef make_expr_ref<tag::greater_equal>       make_greater_equal_ref;
        typedef make_expr_ref<tag::equal_to>            make_equal_to_ref;
        typedef make_expr_ref<tag::not_equal_to>        make_not_equal_to_ref;
        typedef make_expr_ref<tag::logical_or>          make_logical_or_ref;
        typedef make_expr_ref<tag::logical_and>         make_logical_and_ref;
        typedef make_expr_ref<tag::bitwise_and>         make_bitwise_and_ref;
        typedef make_expr_ref<tag::bitwise_or>          make_bitwise_or_ref;
        typedef make_expr_ref<tag::bitwise_xor>         make_bitwise_xor_ref;
        typedef make_expr_ref<tag::comma>               make_comma_ref;
        typedef make_expr_ref<tag::mem_ptr>             make_mem_ptr_ref;
        typedef make_expr_ref<tag::assign>              make_assign_ref;
        typedef make_expr_ref<tag::shift_left_assign>   make_shift_left_assign_ref;
        typedef make_expr_ref<tag::shift_right_assign>  make_shift_right_assign_ref;
        typedef make_expr_ref<tag::multiplies_assign>   make_multiplies_assign_ref;
        typedef make_expr_ref<tag::divides_assign>      make_divides_assign_ref;
        typedef make_expr_ref<tag::modulus_assign>      make_modulus_assign_ref;
        typedef make_expr_ref<tag::plus_assign>         make_plus_assign_ref;
        typedef make_expr_ref<tag::minus_assign>        make_minus_assign_ref;
        typedef make_expr_ref<tag::bitwise_and_assign>  make_bitwise_and_assign_ref;
        typedef make_expr_ref<tag::bitwise_or_assign>   make_bitwise_or_assign_ref;
        typedef make_expr_ref<tag::bitwise_xor_assign>  make_bitwise_xor_assign_ref;
        typedef make_expr_ref<tag::subscript>           make_subscript_ref;
        typedef make_expr_ref<tag::if_else_>            make_if_else_ref;
        typedef make_expr_ref<tag::function>            make_function_ref;

        struct flatten;
        struct pop_front;
        struct reverse;
    }

    typedef functional::make_terminal               _make_terminal;
    typedef functional::make_posit                  _make_posit;
    typedef functional::make_negate                 _make_negate;
    typedef functional::make_dereference            _make_dereference;
    typedef functional::make_complement             _make_complement;
    typedef functional::make_address_of             _make_address_of;
    typedef functional::make_logical_not            _make_logical_not;
    typedef functional::make_pre_inc                _make_pre_inc;
    typedef functional::make_pre_dec                _make_pre_dec;
    typedef functional::make_post_inc               _make_post_inc;
    typedef functional::make_post_dec               _make_post_dec;
    typedef functional::make_shift_left             _make_shift_left;
    typedef functional::make_shift_right            _make_shift_right;
    typedef functional::make_multiplies             _make_multiplies;
    typedef functional::make_divides                _make_divides;
    typedef functional::make_modulus                _make_modulus;
    typedef functional::make_plus                   _make_plus;
    typedef functional::make_minus                  _make_minus;
    typedef functional::make_less                   _make_less;
    typedef functional::make_greater                _make_greater;
    typedef functional::make_less_equal             _make_less_equal;
    typedef functional::make_greater_equal          _make_greater_equal;
    typedef functional::make_equal_to               _make_equal_to;
    typedef functional::make_not_equal_to           _make_not_equal_to;
    typedef functional::make_logical_or             _make_logical_or;
    typedef functional::make_logical_and            _make_logical_and;
    typedef functional::make_bitwise_and            _make_bitwise_and;
    typedef functional::make_bitwise_or             _make_bitwise_or;
    typedef functional::make_bitwise_xor            _make_bitwise_xor;
    typedef functional::make_comma                  _make_comma;
    typedef functional::make_mem_ptr                _make_mem_ptr;
    typedef functional::make_assign                 _make_assign;
    typedef functional::make_shift_left_assign      _make_shift_left_assign;
    typedef functional::make_shift_right_assign     _make_shift_right_assign;
    typedef functional::make_multiplies_assign      _make_multiplies_assign;
    typedef functional::make_divides_assign         _make_divides_assign;
    typedef functional::make_modulus_assign         _make_modulus_assign;
    typedef functional::make_plus_assign            _make_plus_assign;
    typedef functional::make_minus_assign           _make_minus_assign;
    typedef functional::make_bitwise_and_assign     _make_bitwise_and_assign;
    typedef functional::make_bitwise_or_assign      _make_bitwise_or_assign;
    typedef functional::make_bitwise_xor_assign     _make_bitwise_xor_assign;
    typedef functional::make_subscript              _make_subscript;
    typedef functional::make_if_else                _make_if_else;
    typedef functional::make_function               _make_function;

    typedef functional::make_terminal_ref               _make_terminal_ref;
    typedef functional::make_posit_ref                  _make_posit_ref;
    typedef functional::make_negate_ref                 _make_negate_ref;
    typedef functional::make_dereference_ref            _make_dereference_ref;
    typedef functional::make_complement_ref             _make_complement_ref;
    typedef functional::make_address_of_ref             _make_address_of_ref;
    typedef functional::make_logical_not_ref            _make_logical_not_ref;
    typedef functional::make_pre_inc_ref                _make_pre_inc_ref;
    typedef functional::make_pre_dec_ref                _make_pre_dec_ref;
    typedef functional::make_post_inc_ref               _make_post_inc_ref;
    typedef functional::make_post_dec_ref               _make_post_dec_ref;
    typedef functional::make_shift_left_ref             _make_shift_left_ref;
    typedef functional::make_shift_right_ref            _make_shift_right_ref;
    typedef functional::make_multiplies_ref             _make_multiplies_ref;
    typedef functional::make_divides_ref                _make_divides_ref;
    typedef functional::make_modulus_ref                _make_modulus_ref;
    typedef functional::make_plus_ref                   _make_plus_ref;
    typedef functional::make_minus_ref                  _make_minus_ref;
    typedef functional::make_less_ref                   _make_less_ref;
    typedef functional::make_greater_ref                _make_greater_ref;
    typedef functional::make_less_equal_ref             _make_less_equal_ref;
    typedef functional::make_greater_equal_ref          _make_greater_equal_ref;
    typedef functional::make_equal_to_ref               _make_equal_to_ref;
    typedef functional::make_not_equal_to_ref           _make_not_equal_to_ref;
    typedef functional::make_logical_or_ref             _make_logical_or_ref;
    typedef functional::make_logical_and_ref            _make_logical_and_ref;
    typedef functional::make_bitwise_and_ref            _make_bitwise_and_ref;
    typedef functional::make_bitwise_or_ref             _make_bitwise_or_ref;
    typedef functional::make_bitwise_xor_ref            _make_bitwise_xor_ref;
    typedef functional::make_comma_ref                  _make_comma_ref;
    typedef functional::make_mem_ptr_ref                _make_mem_ptr_ref;
    typedef functional::make_assign_ref                 _make_assign_ref;
    typedef functional::make_shift_left_assign_ref      _make_shift_left_assign_ref;
    typedef functional::make_shift_right_assign_ref     _make_shift_right_assign_ref;
    typedef functional::make_multiplies_assign_ref      _make_multiplies_assign_ref;
    typedef functional::make_divides_assign_ref         _make_divides_assign_ref;
    typedef functional::make_modulus_assign_ref         _make_modulus_assign_ref;
    typedef functional::make_plus_assign_ref            _make_plus_assign_ref;
    typedef functional::make_minus_assign_ref           _make_minus_assign_ref;
    typedef functional::make_bitwise_and_assign_ref     _make_bitwise_and_assign_ref;
    typedef functional::make_bitwise_or_assign_ref      _make_bitwise_or_assign_ref;
    typedef functional::make_bitwise_xor_assign_ref     _make_bitwise_xor_assign_ref;
    typedef functional::make_subscript_ref              _make_subscript_ref;
    typedef functional::make_if_else_ref                _make_if_else_ref;
    typedef functional::make_function_ref               _make_function_ref;

    typedef functional::flatten     _flatten;
    typedef functional::pop_front   _pop_front;
    typedef functional::reverse     _reverse;
    typedef functional::deep_copy   _deep_copy;

    template<typename T>
    struct is_extension;

    namespace exops
    {}
}}

#define BOOST_PROTO_UNCVREF(x)\
    typename boost::remove_cv<typename boost::remove_reference<x>::type>::type

#endif
