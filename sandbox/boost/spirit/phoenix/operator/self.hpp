/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_SELF_HPP
#define PHOENIX_OPERATOR_SELF_HPP

#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/utility/type_deduction.hpp>
#include <boost/spirit/phoenix/operator/impl/unary_eval.hpp>
#include <boost/spirit/phoenix/operator/impl/unary_compose.hpp>

namespace boost { namespace phoenix
{
    struct reference_eval;
    struct dereference_eval;
    struct assign_eval;
    struct index_eval;

    BOOST_UNARY_RESULT_OF(&x, result_of_reference);
    BOOST_UNARY_RESULT_OF(*x, result_of_dereference);
    BOOST_BINARY_RESULT_OF(x = y, result_of_assign);
    BOOST_ASYMMETRIC_BINARY_RESULT_OF(x[y], result_of_index);

    namespace impl
    {
        template <typename T0, typename T1>
        struct make_assign_composite
        {
            typedef actor<typename as_composite<assign_eval, T0, T1>::type> type;
        };

        template <typename T0, typename T1>
        struct make_index_composite
        {
            typedef actor<typename as_composite<index_eval, T0, T1>::type> type;
        };
    }

    template <typename Base>
    template <typename T1>
    typename impl::make_assign_composite<actor<Base>, T1>::type
    actor<Base>::operator=(T1 const& a1) const
    {
        return compose<assign_eval>(*this, a1);
    }

    template <typename Base>
    template <typename T1>
    typename impl::make_index_composite<actor<Base>, T1>::type
    actor<Base>::operator[](T1 const& a1) const
    {
        return compose<index_eval>(*this, a1);
    }

#define x a0.eval(args, env)
#define y a1.eval(args, env)

    PHOENIX_UNARY_EVAL(reference_eval, result_of_reference, &x);
    PHOENIX_UNARY_EVAL(dereference_eval, result_of_dereference, *x);
    PHOENIX_UNARY_COMPOSE(reference_eval, &);
    PHOENIX_UNARY_COMPOSE(dereference_eval, *);

    PHOENIX_BINARY_EVAL(assign_eval, result_of_assign, x = y);
    PHOENIX_BINARY_EVAL(index_eval, result_of_index, x[y]);}}

#undef x
#undef y
#endif
