// Boost.Signals library

// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS_SIGNAL10_HEADER
#define BOOST_SIGNALS_SIGNAL10_HEADER

#define BOOST_SIGNALS_NUM_ARGS 10
#define BOOST_SIGNALS_TEMPLATE_PARMS typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10
#define BOOST_SIGNALS_TEMPLATE_ARGS T1, T2, T3, T4, T5, T6, T7, T8, T9, T10
#define BOOST_SIGNALS_PARMS T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10
#define BOOST_SIGNALS_ARGS a1, a2, a3, a4, a5, a6, a7, a8, a9, a10
#define BOOST_SIGNALS_BOUND_ARGS args->a1, args->a2, args->a3, args->a4, args->a5, args->a6, args->a7, args->a8, args->a9, args->a10
#define BOOST_SIGNALS_ARGS_AS_MEMBERS T1 a1;T2 a2;T3 a3;T4 a4;T5 a5;T6 a6;T7 a7;T8 a8;T9 a9;T10 a10;
#define BOOST_SIGNALS_COPY_PARMS T1 ia1, T2 ia2, T3 ia3, T4 ia4, T5 ia5, T6 ia6, T7 ia7, T8 ia8, T9 ia9, T10 ia10
#define BOOST_SIGNALS_INIT_ARGS :a1(ia1), a2(ia2), a3(ia3), a4(ia4), a5(ia5), a6(ia6), a7(ia7), a8(ia8), a9(ia9), a10(ia10)
#define BOOST_SIGNALS_ARG_TYPES typedef T1 arg2_type; typedef T2 arg3_type; typedef T3 arg4_type; typedef T4 arg5_type; typedef T5 arg6_type; typedef T6 arg7_type; typedef T7 arg8_type; typedef T8 arg9_type; typedef T9 arg10_type; typedef T10 arg11_type;
#define BOOST_SIGNALS_TRAITS_ARG_TYPES typename traits::arg1_type, typename traits::arg2_type, typename traits::arg3_type, typename traits::arg4_type, typename traits::arg5_type, typename traits::arg6_type, typename traits::arg7_type, typename traits::arg8_type, typename traits::arg9_type, typename traits::arg10_type

#include <boost/signals/signal_template.hpp>

#undef BOOST_SIGNALS_TRAITS_ARG_TYPES
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

#endif // BOOST_SIGNALS_SIGNAL10_HEADER
