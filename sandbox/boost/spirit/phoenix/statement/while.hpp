/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_STATEMENT_WHILE_HPP
#define PHOENIX_STATEMENT_WHILE_HPP

#include <boost/spirit/phoenix/core/composite.hpp>
#include <boost/spirit/phoenix/core/compose.hpp>

namespace boost { namespace phoenix
{
    struct while_eval
    {
        template <typename Args, typename Env, typename Cond, typename Do>
        struct apply
        {
            typedef void type;
        };

        template <typename Args, typename Env, typename Cond, typename Do>
        static void
        eval(Args const& args, Env const& env, Cond& cond, Do& do_)
        {
            while (cond.eval(args, env))
                do_.eval(args, env);
        }
    };

    template <typename Cond>
    struct while_gen
    {
        while_gen(Cond const& cond)
            : cond(cond) {}

        template <typename Do>
        actor<typename as_composite<while_eval, Cond, Do>::type>
        operator[](Do const& do_) const
        {
            return compose<while_eval>(cond, do_);
        }

        Cond cond;
    };

    template <typename Cond>
    inline while_gen<Cond>
    while_(Cond const& cond)
    {
        return while_gen<Cond>(cond);
    }
}}

#endif
