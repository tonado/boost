//  (C) Copyright Boost.org 1999. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Test file for config setup
// This file should compile, if it does not then
// one or more macros need to be defined.
// see boost_*.cxx for more details

// Do not edit this file, it was generated automatically by
// ../tools/generate from boost_*.cxx on
// Tue Nov 12 11:07:30  2002

#include <boost/config.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include "test.hpp"

#ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#include "boost_no_arg_dep_lookup.cxx"
#else
namespace boost_no_argument_dependent_lookup = empty_boost;
#endif
#ifndef BOOST_NO_AUTO_PTR
#include "boost_no_auto_ptr.cxx"
#else
namespace boost_no_auto_ptr = empty_boost;
#endif
#ifndef BOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#include "boost_no_bcb_partial_spec.cxx"
#else
namespace boost_bcb_partial_specialization_bug = empty_boost;
#endif
#ifndef BOOST_NO_CTYPE_FUNCTIONS
#include "boost_no_ctype_functions.cxx"
#else
namespace boost_no_ctype_functions = empty_boost;
#endif
#ifndef BOOST_NO_CV_SPECIALIZATIONS
#include "boost_no_cv_spec.cxx"
#else
namespace boost_no_cv_specializations = empty_boost;
#endif
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
#include "boost_no_cv_void_spec.cxx"
#else
namespace boost_no_cv_void_specializations = empty_boost;
#endif
#ifndef BOOST_NO_CWCHAR
#include "boost_no_cwchar.cxx"
#else
namespace boost_no_cwchar = empty_boost;
#endif
#ifndef BOOST_NO_CWCTYPE
#include "boost_no_cwctype.cxx"
#else
namespace boost_no_cwctype = empty_boost;
#endif
#ifndef BOOST_DEDUCED_TYPENAME
#include "boost_no_ded_typename.cxx"
#else
namespace boost_deduced_typename = empty_boost;
#endif
#ifndef BOOST_NO_DEPENDENT_NESTED_DERIVATIONS
#include "boost_no_dep_nested_class.cxx"
#else
namespace boost_no_dependent_nested_derivations = empty_boost;
#endif
#ifndef BOOST_NO_DEPENDENT_TYPES_IN_TEMPLATE_VALUE_PARAMETERS
#include "boost_no_dep_val_param.cxx"
#else
namespace boost_no_dependent_types_in_template_value_parameters = empty_boost;
#endif
#ifndef BOOST_NO_EXCEPTION_STD_NAMESPACE
#include "boost_no_excep_std.cxx"
#else
namespace boost_no_exception_std_namespace = empty_boost;
#endif
#ifndef BOOST_NO_EXCEPTIONS
#include "boost_no_exceptions.cxx"
#else
namespace boost_no_exceptions = empty_boost;
#endif
#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
#include "boost_no_exp_func_tem_arg.cxx"
#else
namespace boost_no_explicit_function_template_arguments = empty_boost;
#endif
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
#include "boost_no_func_tmp_order.cxx"
#else
namespace boost_no_function_template_ordering = empty_boost;
#endif
#ifndef BOOST_NO_MS_INT64_NUMERIC_LIMITS
#include "boost_no_i64_limits.cxx"
#else
namespace boost_no_ms_int64_numeric_limits = empty_boost;
#endif
#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#include "boost_no_inline_memb_init.cxx"
#else
namespace boost_no_inclass_member_initialization = empty_boost;
#endif
#ifndef BOOST_NO_INTEGRAL_INT64_T
#include "boost_no_integral_int64_t.cxx"
#else
namespace boost_no_integral_int64_t = empty_boost;
#endif
#ifndef BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#include "boost_no_iter_construct.cxx"
#else
namespace boost_no_templated_iterator_constructors = empty_boost;
#endif
#ifndef BOOST_NO_LIMITS
#include "boost_no_limits.cxx"
#else
namespace boost_no_limits = empty_boost;
#endif
#ifndef BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#include "boost_no_limits_const_exp.cxx"
#else
namespace boost_no_limits_compile_time_constants = empty_boost;
#endif
#ifndef BOOST_NO_LONG_LONG_NUMERIC_LIMITS
#include "boost_no_ll_limits.cxx"
#else
namespace boost_no_long_long_numeric_limits = empty_boost;
#endif
#ifndef BOOST_NO_MEMBER_FUNCTION_SPECIALIZATIONS
#include "boost_no_mem_func_spec.cxx"
#else
namespace boost_no_member_function_specializations = empty_boost;
#endif
#ifndef BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#include "boost_no_mem_tem_keyword.cxx"
#else
namespace boost_no_member_template_keyword = empty_boost;
#endif
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#include "boost_no_mem_templ_frnds.cxx"
#else
namespace boost_no_member_template_friends = empty_boost;
#endif
#ifndef BOOST_NO_MEMBER_TEMPLATES
#include "boost_no_mem_templates.cxx"
#else
namespace boost_no_member_templates = empty_boost;
#endif
#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
#include "boost_no_ops_in_namespace.cxx"
#else
namespace boost_no_operators_in_namespace = empty_boost;
#endif
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include "boost_no_partial_spec.cxx"
#else
namespace boost_no_template_partial_specialization = empty_boost;
#endif
#ifndef BOOST_NO_PRIVATE_IN_AGGREGATE
#include "boost_no_priv_aggregate.cxx"
#else
namespace boost_no_private_in_aggregate = empty_boost;
#endif
#ifndef BOOST_NO_POINTER_TO_MEMBER_CONST
#include "boost_no_ptr_mem_const.cxx"
#else
namespace boost_no_pointer_to_member_const = empty_boost;
#endif
#ifndef BOOST_NO_STRINGSTREAM
#include "boost_no_sstream.cxx"
#else
namespace boost_no_stringstream = empty_boost;
#endif
#ifndef BOOST_NO_STD_ALLOCATOR
#include "boost_no_std_allocator.cxx"
#else
namespace boost_no_std_allocator = empty_boost;
#endif
#ifndef BOOST_NO_STD_DISTANCE
#include "boost_no_std_distance.cxx"
#else
namespace boost_no_std_distance = empty_boost;
#endif
#ifndef BOOST_NO_STD_ITERATOR_TRAITS
#include "boost_no_std_iter_traits.cxx"
#else
namespace boost_no_std_iterator_traits = empty_boost;
#endif
#ifndef BOOST_NO_STD_ITERATOR
#include "boost_no_std_iterator.cxx"
#else
namespace boost_no_std_iterator = empty_boost;
#endif
#ifndef BOOST_NO_STD_LOCALE
#include "boost_no_std_locale.cxx"
#else
namespace boost_no_std_locale = empty_boost;
#endif
#ifndef BOOST_NO_STD_MESSAGES
#include "boost_no_std_messages.cxx"
#else
namespace boost_no_std_messages = empty_boost;
#endif
#ifndef BOOST_NO_STD_MIN_MAX
#include "boost_no_std_min_max.cxx"
#else
namespace boost_no_std_min_max = empty_boost;
#endif
#ifndef BOOST_NO_STD_OUTPUT_ITERATOR_ASSIGN
#include "boost_no_std_oi_assign.cxx"
#else
namespace boost_no_std_output_iterator_assign = empty_boost;
#endif
#ifndef BOOST_NO_STD_USE_FACET
#include "boost_no_std_use_facet.cxx"
#else
namespace boost_no_std_use_facet = empty_boost;
#endif
#ifndef BOOST_NO_STD_WSTREAMBUF
#include "boost_no_std_wstreambuf.cxx"
#else
namespace boost_no_std_wstreambuf = empty_boost;
#endif
#ifndef BOOST_NO_STD_WSTRING
#include "boost_no_std_wstring.cxx"
#else
namespace boost_no_std_wstring = empty_boost;
#endif
#ifndef BOOST_NO_STDC_NAMESPACE
#include "boost_no_stdc_namespace.cxx"
#else
namespace boost_no_stdc_namespace = empty_boost;
#endif
#ifndef BOOST_NO_SWPRINTF
#include "boost_no_swprintf.cxx"
#else
namespace boost_no_swprintf = empty_boost;
#endif
#ifndef BOOST_NO_TEMPLATE_TEMPLATES
#include "boost_no_template_template.cxx"
#else
namespace boost_no_template_templates = empty_boost;
#endif
#ifndef BOOST_NO_USING_TEMPLATE
#include "boost_no_using_template.cxx"
#else
namespace boost_no_using_template = empty_boost;
#endif
#ifndef BOOST_NO_VOID_RETURNS
#include "boost_no_void_returns.cxx"
#else
namespace boost_no_void_returns = empty_boost;
#endif
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
#include "boost_no_wchar_t.cxx"
#else
namespace boost_no_intrinsic_wchar_t = empty_boost;
#endif
#ifdef BOOST_HAS_TWO_ARG_USE_FACET
#include "boost_has_2arg_use_facet.cxx"
#else
namespace boost_has_two_arg_use_facet = empty_boost;
#endif
#ifdef BOOST_HAS_BETHREADS
#include "boost_has_bethreads.cxx"
#else
namespace boost_has_bethreads = empty_boost;
#endif
#ifdef BOOST_HAS_CLOCK_GETTIME
#include "boost_has_clock_gettime.cxx"
#else
namespace boost_has_clock_gettime = empty_boost;
#endif
#ifdef BOOST_HAS_DIRENT_H
#include "boost_has_dirent_h.cxx"
#else
namespace boost_has_dirent_h = empty_boost;
#endif
#ifdef BOOST_HAS_FTIME
#include "boost_has_ftime.cxx"
#else
namespace boost_has_ftime = empty_boost;
#endif
#ifdef BOOST_HAS_GETTIMEOFDAY
#include "boost_has_gettimeofday.cxx"
#else
namespace boost_has_gettimeofday = empty_boost;
#endif
#ifdef BOOST_HAS_HASH
#include "boost_has_hash.cxx"
#else
namespace boost_has_hash = empty_boost;
#endif
#ifdef BOOST_HAS_LONG_LONG
#include "boost_has_long_long.cxx"
#else
namespace boost_has_long_long = empty_boost;
#endif
#ifdef BOOST_HAS_MACRO_USE_FACET
#include "boost_has_macro_use_facet.cxx"
#else
namespace boost_has_macro_use_facet = empty_boost;
#endif
#ifdef BOOST_HAS_MS_INT64
#include "boost_has_ms_int64.cxx"
#else
namespace boost_has_ms_int64 = empty_boost;
#endif
#ifdef BOOST_HAS_NANOSLEEP
#include "boost_has_nanosleep.cxx"
#else
namespace boost_has_nanosleep = empty_boost;
#endif
#ifdef BOOST_HAS_NL_TYPES_H
#include "boost_has_nl_types_h.cxx"
#else
namespace boost_has_nl_types_h = empty_boost;
#endif
#ifdef BOOST_HAS_NRVO
#include "boost_has_nrvo.cxx"
#else
namespace boost_has_nrvo = empty_boost;
#endif
#ifdef BOOST_HAS_PARTIAL_STD_ALLOCATOR
#include "boost_has_part_alloc.cxx"
#else
namespace boost_has_partial_std_allocator = empty_boost;
#endif
#ifdef BOOST_HAS_PTHREAD_DELAY_NP
#include "boost_has_pthread_delay_np.cxx"
#else
namespace boost_has_pthread_delay_np = empty_boost;
#endif
#ifdef BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#include "boost_has_pthread_ma_st.cxx"
#else
namespace boost_has_pthread_mutexattr_settype = empty_boost;
#endif
#ifdef BOOST_HAS_PTHREAD_YIELD
#include "boost_has_pthread_yield.cxx"
#else
namespace boost_has_pthread_yield = empty_boost;
#endif
#ifdef BOOST_HAS_PTHREADS
#include "boost_has_pthreads.cxx"
#else
namespace boost_has_pthreads = empty_boost;
#endif
#ifdef BOOST_HAS_SCHED_YIELD
#include "boost_has_sched_yield.cxx"
#else
namespace boost_has_sched_yield = empty_boost;
#endif
#ifdef BOOST_HAS_SGI_TYPE_TRAITS
#include "boost_has_sgi_type_traits.cxx"
#else
namespace boost_has_sgi_type_traits = empty_boost;
#endif
#ifdef BOOST_HAS_SLIST
#include "boost_has_slist.cxx"
#else
namespace boost_has_slist = empty_boost;
#endif
#ifdef BOOST_HAS_STDINT_H
#include "boost_has_stdint_h.cxx"
#else
namespace boost_has_stdint_h = empty_boost;
#endif
#ifdef BOOST_HAS_STLP_USE_FACET
#include "boost_has_stlp_use_facet.cxx"
#else
namespace boost_has_stlp_use_facet = empty_boost;
#endif
#ifdef BOOST_HAS_UNISTD_H
#include "boost_has_unistd_h.cxx"
#else
namespace boost_has_unistd_h = empty_boost;
#endif
#ifdef BOOST_MSVC6_MEMBER_TEMPLATES
#include "boost_has_vc6_mem_templ.cxx"
#else
namespace boost_msvc6_member_templates = empty_boost;
#endif
#ifdef BOOST_MSVC_STD_ITERATOR
#include "boost_has_vc_iterator.cxx"
#else
namespace boost_msvc_std_iterator = empty_boost;
#endif
#ifdef BOOST_HAS_WINTHREADS
#include "boost_has_winthreads.cxx"
#else
namespace boost_has_winthreads = empty_boost;
#endif

int test_main( int, char *[] )
{
   BOOST_TEST(0 == boost_no_intrinsic_wchar_t::test());
   BOOST_TEST(0 == boost_no_void_returns::test());
   BOOST_TEST(0 == boost_no_using_template::test());
   BOOST_TEST(0 == boost_no_template_templates::test());
   BOOST_TEST(0 == boost_no_swprintf::test());
   BOOST_TEST(0 == boost_no_stdc_namespace::test());
   BOOST_TEST(0 == boost_no_std_wstring::test());
   BOOST_TEST(0 == boost_no_std_wstreambuf::test());
   BOOST_TEST(0 == boost_no_std_use_facet::test());
   BOOST_TEST(0 == boost_no_std_output_iterator_assign::test());
   BOOST_TEST(0 == boost_no_std_min_max::test());
   BOOST_TEST(0 == boost_no_std_messages::test());
   BOOST_TEST(0 == boost_no_std_locale::test());
   BOOST_TEST(0 == boost_no_std_iterator::test());
   BOOST_TEST(0 == boost_no_std_iterator_traits::test());
   BOOST_TEST(0 == boost_no_std_distance::test());
   BOOST_TEST(0 == boost_no_std_allocator::test());
   BOOST_TEST(0 == boost_no_stringstream::test());
   BOOST_TEST(0 == boost_no_pointer_to_member_const::test());
   BOOST_TEST(0 == boost_no_private_in_aggregate::test());
   BOOST_TEST(0 == boost_no_template_partial_specialization::test());
   BOOST_TEST(0 == boost_no_operators_in_namespace::test());
   BOOST_TEST(0 == boost_no_member_templates::test());
   BOOST_TEST(0 == boost_no_member_template_friends::test());
   BOOST_TEST(0 == boost_no_member_template_keyword::test());
   BOOST_TEST(0 == boost_no_member_function_specializations::test());
   BOOST_TEST(0 == boost_no_long_long_numeric_limits::test());
   BOOST_TEST(0 == boost_no_limits_compile_time_constants::test());
   BOOST_TEST(0 == boost_no_limits::test());
   BOOST_TEST(0 == boost_no_templated_iterator_constructors::test());
   BOOST_TEST(0 == boost_no_integral_int64_t::test());
   BOOST_TEST(0 == boost_no_inclass_member_initialization::test());
   BOOST_TEST(0 == boost_no_ms_int64_numeric_limits::test());
   BOOST_TEST(0 == boost_no_function_template_ordering::test());
   BOOST_TEST(0 == boost_no_explicit_function_template_arguments::test());
   BOOST_TEST(0 == boost_no_exceptions::test());
   BOOST_TEST(0 == boost_no_exception_std_namespace::test());
   BOOST_TEST(0 == boost_no_dependent_types_in_template_value_parameters::test());
   BOOST_TEST(0 == boost_no_dependent_nested_derivations::test());
   BOOST_TEST(0 == boost_deduced_typename::test());
   BOOST_TEST(0 == boost_no_cwctype::test());
   BOOST_TEST(0 == boost_no_cwchar::test());
   BOOST_TEST(0 == boost_no_cv_void_specializations::test());
   BOOST_TEST(0 == boost_no_cv_specializations::test());
   BOOST_TEST(0 == boost_no_ctype_functions::test());
   BOOST_TEST(0 == boost_bcb_partial_specialization_bug::test());
   BOOST_TEST(0 == boost_no_auto_ptr::test());
   BOOST_TEST(0 == boost_no_argument_dependent_lookup::test());
   BOOST_TEST(0 == boost_has_winthreads::test());
   BOOST_TEST(0 == boost_msvc_std_iterator::test());
   BOOST_TEST(0 == boost_msvc6_member_templates::test());
   BOOST_TEST(0 == boost_has_unistd_h::test());
   BOOST_TEST(0 == boost_has_stlp_use_facet::test());
   BOOST_TEST(0 == boost_has_stdint_h::test());
   BOOST_TEST(0 == boost_has_slist::test());
   BOOST_TEST(0 == boost_has_sgi_type_traits::test());
   BOOST_TEST(0 == boost_has_sched_yield::test());
   BOOST_TEST(0 == boost_has_pthreads::test());
   BOOST_TEST(0 == boost_has_pthread_yield::test());
   BOOST_TEST(0 == boost_has_pthread_mutexattr_settype::test());
   BOOST_TEST(0 == boost_has_pthread_delay_np::test());
   BOOST_TEST(0 == boost_has_partial_std_allocator::test());
   BOOST_TEST(0 == boost_has_nrvo::test());
   BOOST_TEST(0 == boost_has_nl_types_h::test());
   BOOST_TEST(0 == boost_has_nanosleep::test());
   BOOST_TEST(0 == boost_has_ms_int64::test());
   BOOST_TEST(0 == boost_has_macro_use_facet::test());
   BOOST_TEST(0 == boost_has_long_long::test());
   BOOST_TEST(0 == boost_has_hash::test());
   BOOST_TEST(0 == boost_has_gettimeofday::test());
   BOOST_TEST(0 == boost_has_ftime::test());
   BOOST_TEST(0 == boost_has_dirent_h::test());
   BOOST_TEST(0 == boost_has_clock_gettime::test());
   BOOST_TEST(0 == boost_has_bethreads::test());
   BOOST_TEST(0 == boost_has_two_arg_use_facet::test());
   return 0;
}
