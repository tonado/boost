/*
	Template for Signa1, Signal2, ... classes that support signals
	with 1, 2, ... parameters

	Author: Frank Hess <frank.hess@nist.gov>
	Begin: 2007-01-23
*/
/* This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of
 * their official duties. Pursuant to title 17 Section 105 of the United
 * States Code this software is not subject to copyright protection and is
 * in the public domain. This is an experimental system. NIST assumes no
 * responsibility whatsoever for its use by other parties, and makes no
 * guarantees, expressed or implied, about its quality, reliability, or
 * any other characteristic. We would appreciate acknowledgement if the
 * software is used.
 */
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SIGNALS_COMMON_MACROS_HEADER
#define BOOST_SIGNALS_COMMON_MACROS_HEADER

#ifndef BOOST_SIGNALS_MAX_ARGS
#define BOOST_SIGNALS_MAX_ARGS 10
#endif

// argn
#define BOOST_SIGNAL_SIGNATURE_ARG_NAME(z, n, data) BOOST_PP_CAT(arg, BOOST_PP_INC(n))
// Tn argn
#define BOOST_SIGNAL_SIGNATURE_FULL_ARG(z, n, data) \
	BOOST_PP_CAT(T, BOOST_PP_INC(n)) BOOST_SIGNAL_SIGNATURE_ARG_NAME(~, n, ~)
// T1 arg1, T2 arg2, ..., Tn argn
#define BOOST_SIGNAL_SIGNATURE_FULL_ARGS(arity) \
	BOOST_PP_ENUM(arity, BOOST_SIGNAL_SIGNATURE_FULL_ARG, ~)
// arg1, arg2, ..., argn
#define BOOST_SIGNAL_SIGNATURE_ARG_NAMES(arity) BOOST_PP_ENUM(arity, BOOST_SIGNAL_SIGNATURE_ARG_NAME, ~)
// typename prefixR, typename prefixT1, typename prefixT2, ..., typename prefixTN
#define BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_DECL(arity, prefix) \
	typename BOOST_PP_CAT(prefix, R) BOOST_PP_COMMA_IF(arity) \
	BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(arity), typename BOOST_PP_CAT(prefix, T))
// typename R, typename T1, typename T2, ..., typename TN
#define BOOST_SIGNAL_SIGNATURE_TEMPLATE_DECL(arity) BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_DECL(arity,)
// prefixR, prefixT1, prefixT2, ..., prefixTN
#define BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_INSTANTIATION(arity, prefix) \
	BOOST_PP_CAT(prefix, R) BOOST_PP_COMMA_IF(arity) BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(arity), BOOST_PP_CAT(prefix, T))
// R, T1, T2, ..., TN
#define BOOST_SIGNAL_SIGNATURE_TEMPLATE_INSTANTIATION(arity) \
	BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_INSTANTIATION(arity,)
// functionN<R, T1, T2, ..., TN>
#define BOOST_FUNCTION_N_DECL(arity) BOOST_PP_CAT(function, arity)<\
	BOOST_SIGNAL_SIGNATURE_TEMPLATE_INSTANTIATION(arity) >
// slotN
#define BOOST_SLOT_CLASS_NAME(arity) BOOST_PP_CAT(slot, arity)
// typename function_traits<Signature>::argn_type
#define BOOST_SIGNAL_SIGNATURE_TO_ARGN_TYPE(z, n, Signature) \
	BOOST_PP_CAT(BOOST_PP_CAT(typename function_traits<Signature>::arg, BOOST_PP_INC(n)), _type)
// typename function_traits<Signature>::result_type,
// typename function_traits<Signature>::arg1_type,
// typename function_traits<Signature>::arg2_type,
// ...,
// typename function_traits<Signature>::argn_type
#define BOOST_SIGNAL_PORTABLE_SIGNATURE(arity, Signature) \
	typename function_traits<Signature>::result_type \
	BOOST_PP_COMMA_IF(arity) BOOST_PP_ENUM(arity, BOOST_SIGNAL_SIGNATURE_TO_ARGN_TYPE, Signature)

#endif	// BOOST_SIGNALS_COMMON_MACROS_HEADER
