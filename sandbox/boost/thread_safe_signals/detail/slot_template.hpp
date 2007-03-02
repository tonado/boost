// Boost.Signals library

// Copyright Frank Mori Hess 2007.
// Copyright Timmo Stange 2007.
// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// This file is included iteratively, and should not be protected from multiple inclusion

#define BOOST_SIGNALS_NUM_ARGS BOOST_PP_ITERATION()

#define BOOST_SLOT_IMPL_CLASS_NAME BOOST_PP_CAT(BOOST_SLOT_CLASS_NAME, _impl)
#define BOOST_SLOT_IMPL_BASE_CLASS_NAME BOOST_PP_CAT(BOOST_SLOT_CLASS_NAME, _impl_base)

namespace boost
{
	template<typename Signature, typename SlotFunction> class slot;

	// slot class template.
	template<BOOST_SIGNAL_SIGNATURE_TEMPLATE_DECL(BOOST_SIGNALS_NUM_ARGS),
		typename SlotFunction = BOOST_FUNCTION_N_DECL(BOOST_SIGNALS_NUM_ARGS)>
	class BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS): public signalslib::detail::slot_base
	{
	public:
		template<BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_DECL(BOOST_SIGNALS_NUM_ARGS, Other), typename OtherSlotFunction>
		friend class BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS);

		typedef SlotFunction slot_function_type;
		typedef R result_type;
// typedef Tn argn_type;
#define BOOST_SIGNAL_MISC_STATEMENT(z, n, data) \
	typedef BOOST_PP_CAT(T, BOOST_PP_INC(n)) BOOST_PP_CAT(BOOST_PP_CAT(arg, BOOST_PP_INC(n)), _type);
				BOOST_PP_REPEAT(BOOST_SIGNALS_NUM_ARGS, BOOST_SIGNAL_MISC_STATEMENT, ~)
#undef BOOST_SIGNAL_MISC_STATEMENT
#if BOOST_SIGNALS_NUM_ARGS == 1
		typedef arg1_type argument_type;
#elif BOOST_SIGNALS_NUM_ARGS == 2
		typedef arg1_type first_argument_type;
		typedef arg2_type second_argument_type;
#endif
		static const int arity = BOOST_SIGNALS_NUM_ARGS;

		template<typename F>
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)(const F& f): _slot_function(signalslib::detail::get_invocable_slot(f, signalslib::detail::tag_type(f)))
		{
		}
		// copy constructors
		template<BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_DECL(BOOST_SIGNALS_NUM_ARGS, Other), typename OtherSlotFunction>
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)(const BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)
			<BOOST_SIGNAL_PREFIXED_SIGNATURE_TEMPLATE_INSTANTIATION(BOOST_SIGNALS_NUM_ARGS, Other), OtherSlotFunction> &other_slot):
			signalslib::detail::slot_base(other_slot), _slot_function(other_slot._slot_function)
		{
		}
		template<typename Signature, typename OtherSlotFunction>
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)(const slot<Signature, OtherSlotFunction> &other_slot):
			signalslib::detail::slot_base(other_slot), _slot_function(other_slot._slot_function)
		{
		}
		// bind syntactic sugar
// ArgTypeN argN
#define BOOST_SLOT_BINDING_ARG_DECL(z, n, data) \
	BOOST_PP_CAT(ArgType, n) BOOST_PP_CAT(arg, n)
// template<typename Func, typename ArgType0, typename ArgType1, ..., typename ArgTypen-1> slotN(...
#define BOOST_SLOT_BINDING_CONSTRUCTOR(z, n, data) \
		template<typename Func BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename ArgType)> \
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)(Func func BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, BOOST_SLOT_BINDING_ARG_DECL, ~)): \
			_slot_function(bind(func BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, arg))) \
		{}
#define BOOST_SLOT_MAX_BINDING_ARGS 10
		BOOST_PP_REPEAT_FROM_TO(1, BOOST_SLOT_MAX_BINDING_ARGS, BOOST_SLOT_BINDING_CONSTRUCTOR, ~)
#undef BOOST_SLOT_MAX_BINDING_ARGS
#undef BOOST_SLOT_BINDING_ARG_DECL
#undef BOOST_SLOT_BINDING_CONSTRUCTOR
		R operator()(BOOST_SIGNAL_SIGNATURE_FULL_ARGS(BOOST_SIGNALS_NUM_ARGS))
		{
			return _slot_function(BOOST_SIGNAL_SIGNATURE_ARG_NAMES(BOOST_SIGNALS_NUM_ARGS));
		}
		R operator()(BOOST_SIGNAL_SIGNATURE_FULL_ARGS(BOOST_SIGNALS_NUM_ARGS)) const
		{
			return _slot_function(BOOST_SIGNAL_SIGNATURE_ARG_NAMES(BOOST_SIGNALS_NUM_ARGS));
		}
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)& track(const weak_ptr<void> &tracked)
		{
			_trackedObjects.push_back(tracked);
			return *this;
		}
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)& track(const slot_base &slot)
		{
			tracked_container_type::const_iterator it;
			for(it = slot.tracked_objects().begin(); it != slot.tracked_objects().end(); ++it)
			{
				track(*it);
			}
			return *this;
		}
		BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)& track(const signalslib::detail::signal_base &signal)
		{
			// call base class function, since it is a friend of signal_base and can call lock_pimpl()
			track_signal(signal);
			return *this;
		}
		const slot_function_type& slot_function() const {return _slot_function;}
	private:
		SlotFunction _slot_function;
	};
	namespace signalslib
	{
		namespace detail
		{
			template<unsigned arity, typename Signature, typename SlotFunction>
			class slotN;
			// partial template specialization
			template<typename Signature, typename SlotFunction>
			class slotN<BOOST_SIGNALS_NUM_ARGS, Signature, SlotFunction>
			{
			public:
				typedef BOOST_SLOT_CLASS_NAME(BOOST_SIGNALS_NUM_ARGS)<
					BOOST_SIGNAL_PORTABLE_SIGNATURE(BOOST_SIGNALS_NUM_ARGS, Signature),
					SlotFunction> type;
			};
		}
	}
} // end namespace boost

#undef BOOST_SIGNALS_NUM_ARGS
#undef BOOST_SLOT_IMPL_CLASS_NAME
#undef BOOST_SLOT_IMPL_BASE_CLASS_NAME
