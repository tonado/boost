// Boost.Signals library
//
// Copyright (C) 2001 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS_SIGNAL6_HEADER
#define BOOST_SIGNALS_SIGNAL6_HEADER

#define BOOST_SIGNALS_NUM_ARGS 6
#define BOOST_SIGNALS_TEMPLATE_PARMS typename T1, typename T2, typename T3, typename T4, typename T5, typename T6
#define BOOST_SIGNALS_TEMPLATE_ARGS T1, T2, T3, T4, T5, T6
#define BOOST_SIGNALS_PARMS T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6
#define BOOST_SIGNALS_ARGS a1, a2, a3, a4, a5, a6
#define BOOST_SIGNALS_BOUND_ARGS args->a1, args->a2, args->a3, args->a4, args->a5, args->a6
#define BOOST_SIGNALS_ARGS_AS_MEMBERS T1 a1;T2 a2;T3 a3;T4 a4;T5 a5;T6 a6;
#define BOOST_SIGNALS_COPY_PARMS T1 ia1, T2 ia2, T3 ia3, T4 ia4, T5 ia5, T6 ia6
#define BOOST_SIGNALS_INIT_ARGS :a1(ia1), a2(ia2), a3(ia3), a4(ia4), a5(ia5), a6(ia6)
#define BOOST_SIGNALS_ARG_TYPES typedef T1 arg2_type; typedef T2 arg3_type; typedef T3 arg4_type; typedef T4 arg5_type; typedef T5 arg6_type; typedef T6 arg7_type; 

#include <boost/signals/signal_template.hpp>

#undef BOOST_SIGNALS_ARG_TYPES
#undef BOOST_SIGNALS_INIT_ARGS
#undef BOOST_SIGNALS_COPY_PARMS
#undef BOOST_SIGNALS_ARGS_AS_MEMBERS
#undef BOOST_SIGNALS_BOUND_ARGS
#undef BOOST_SIGNALS_ARGS
#undef BOOST_SIGNALS_PARMS
#undef BOOST_SIGNALS_TEMPLATE_ARGS
#undef BOOST_SIGNALS_TEMPLATE_PARMS
#undef BOOST_SIGNALS_NUM_ARGS

#endif // BOOST_SIGNALS_SIGNAL6_HEADER
