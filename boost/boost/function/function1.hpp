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

#ifndef BOOST_FUNCTION1_HPP
#define BOOST_FUNCTION1_HPP

#include <boost/function/function_base.hpp>

#define BOOST_FUNCTION_NUM_ARGS 1
#define BOOST_FUNCTION_COMMA ,
#define BOOST_FUNCTION_TEMPLATE_PARMS typename T1
#define BOOST_FUNCTION_TEMPLATE_ARGS T1
#define BOOST_FUNCTION_OTHER_TEMPLATE_PARMS typename OtherT1
#define BOOST_FUNCTION_OTHER_TEMPLATE_ARGS OtherT1
#define BOOST_FUNCTION_PARMS T1 a1
#define BOOST_FUNCTION_ARGS a1
#define BOOST_FUNCTION_FUNCTION function1
#define BOOST_FUNCTION_INVOKER_BASE invoker_base1
#define BOOST_FUNCTION_FUNCTION_INVOKER function_invoker1
#define BOOST_FUNCTION_VOID_FUNCTION_INVOKER void_function_invoker1
#define BOOST_FUNCTION_FUNCTION_OBJ_INVOKER function_obj_invoker1
#define BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER void_function_obj_invoker1
#define BOOST_FUNCTION_GET_FUNCTION_INVOKER get_function_invoker1
#define BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER get_function_obj_invoker1

#include <boost/function/function_template.hpp>

#undef BOOST_FUNCTION_NUM_ARGS
#undef BOOST_FUNCTION_COMMA
#undef BOOST_FUNCTION_TEMPLATE_PARMS
#undef BOOST_FUNCTION_TEMPLATE_ARGS
#undef BOOST_FUNCTION_OTHER_TEMPLATE_PARMS
#undef BOOST_FUNCTION_OTHER_TEMPLATE_ARGS
#undef BOOST_FUNCTION_PARMS
#undef BOOST_FUNCTION_ARGS
#undef BOOST_FUNCTION_FUNCTION
#undef BOOST_FUNCTION_INVOKER_BASE
#undef BOOST_FUNCTION_FUNCTION_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_INVOKER
#undef BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER

#endif // BOOST_FUNCTION1_HPP
