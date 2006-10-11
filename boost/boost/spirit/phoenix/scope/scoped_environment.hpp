/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2004 Daniel Wallin

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_SCOPE_SCOPED_ENVIRONMENT_HPP
#define PHOENIX_SCOPE_SCOPED_ENVIRONMENT_HPP

namespace boost { namespace phoenix
{
    template <typename Env, typename OuterEnv, typename Locals, typename Map>
    struct scoped_environment
    {
        typedef Env env_type;
        typedef OuterEnv outer_env_type;
        typedef Locals locals_type;
        typedef Map map_type;
        typedef typename Env::args_type args_type;
        typedef typename Env::tie_type tie_type;

        scoped_environment(
            Env const& env
          , OuterEnv const& outer_env
          , Locals& locals)
            : env(env)
            , outer_env(outer_env)
            , locals(locals) {}

        tie_type const& 
        args() const
        {
            return env.args();
        }

        Env const& env;
        OuterEnv const& outer_env;
        Locals& locals;
    };
}}

#endif
