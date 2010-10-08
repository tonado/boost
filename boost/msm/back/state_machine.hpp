// Copyright 2008 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MSM_BACK_STATEMACHINE_H
#define BOOST_MSM_BACK_STATEMACHINE_H

#include <exception>
#include <vector>
#include <queue>
#include <functional>
#include <numeric>
#include <utility>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/deref.hpp>

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/as_set.hpp>
#include <boost/fusion/container/set.hpp>
#include <boost/fusion/include/set.hpp>
#include <boost/fusion/include/set_fwd.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <boost/assert.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/function.hpp>
#ifndef BOOST_NO_RTTI
#include <boost/any.hpp>
#endif

#include <boost/serialization/base_object.hpp> 

#include <boost/msm/row_tags.hpp>
#include <boost/msm/back/metafunctions.hpp>
#include <boost/msm/back/history_policies.hpp>
#include <boost/msm/back/bind_helpers.hpp>
#include <boost/msm/back/common_types.hpp>
#include <boost/msm/back/args.hpp>
#include <boost/msm/back/default_compile_policy.hpp>
#include <boost/msm/back/dispatch_table.hpp>

BOOST_MPL_HAS_XXX_TRAIT_DEF(accept_sig)
BOOST_MPL_HAS_XXX_TRAIT_DEF(no_automatic_create)
BOOST_MPL_HAS_XXX_TRAIT_DEF(non_forwarding_flag)
BOOST_MPL_HAS_XXX_TRAIT_DEF(direct_entry)
BOOST_MPL_HAS_XXX_TRAIT_DEF(initial_event)
BOOST_MPL_HAS_XXX_TRAIT_DEF(do_serialize)

#ifndef BOOST_MSM_CONSTRUCTOR_ARG_SIZE
#define BOOST_MSM_CONSTRUCTOR_ARG_SIZE 5 // default max number of arguments for constructors
#endif

namespace boost { namespace msm { namespace back
{
// event used internally for wrapping a direct entry
template <class StateType,class Event>
struct direct_entry_event 
{
    typedef int direct_entry;
    typedef StateType active_state;

    direct_entry_event(Event const& evt):m_event(evt){}
    Event const& m_event;
};

// This declares the statically-initialized dispatch_table instance.
template <class Fsm,class Stt, class Event,class CompilePolicy>
const boost::msm::back::dispatch_table<Fsm,Stt, Event,CompilePolicy>
dispatch_table<Fsm,Stt, Event,CompilePolicy>::instance;

// library-containing class for state machines.  Pass the actual FSM class as
// the Concrete parameter.
template<class Derived,class HistoryPolicy=NoHistory,class CompilePolicy=favor_runtime_speed>
class state_machine : public Derived
{
private: 
    typedef boost::msm::back::state_machine<Derived,
        HistoryPolicy,CompilePolicy>                library_sm;

    typedef ::boost::function<
        execute_return ()>                          transition_fct;
    typedef ::boost::function<
        execute_return () >                         deferred_fct;
    typedef std::deque<deferred_fct >               deferred_events_queue_t;
    typedef std::queue<transition_fct >             events_queue_t;
    typedef bool (*flag_handler)(library_sm&);

    // all state machines are friend with each other to allow embedding any of them in another fsm
    template <class ,class , class
    > friend class boost::msm::back::state_machine;

    // helper to add, if needed, visitors to all states
    // version without visitors
    template <class StateType,class Enable=void>
    struct visitor_fct_helper 
    {
    public:
        visitor_fct_helper(){}
        void fill_visitors(int)
        {
        }
        template <class FCT>
        void insert(int,FCT)
        {
        }
        template <class VISITOR>
        void execute(int,VISITOR)
        {
        }
    };
    // version with visitors
    template <class StateType>
    struct visitor_fct_helper<StateType,typename ::boost::enable_if<has_accept_sig<StateType> >::type> 
    {
    public:
        visitor_fct_helper():m_state_visitors(){}
        void fill_visitors(int number_of_states)
        {
            m_state_visitors.resize(number_of_states);
        }
        template <class FCT>
        void insert(int index,FCT fct)
        {
            m_state_visitors[index]=fct;
        }
        void execute(int index)
        {
            m_state_visitors[index]();
        }

#define MSM_VISITOR_HELPER_EXECUTE_SUB(z, n, unused) ARG ## n vis ## n
#define MSM_VISITOR_HELPER_EXECUTE(z, n, unused)                                    \
        template <BOOST_PP_ENUM_PARAMS(n, class ARG)>                               \
        void execute(int index BOOST_PP_COMMA_IF(n)                                 \
                     BOOST_PP_ENUM(n, MSM_VISITOR_HELPER_EXECUTE_SUB, ~ ) )         \
        {                                                                           \
            m_state_visitors[index](BOOST_PP_ENUM_PARAMS(n,vis));                   \
        }
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(BOOST_MSM_VISITOR_ARG_SIZE,1), MSM_VISITOR_HELPER_EXECUTE, ~)
#undef MSM_VISITOR_HELPER_EXECUTE
#undef MSM_VISITOR_HELPER_EXECUTE_SUB
    private:
        typedef typename StateType::accept_sig::type                  visitor_fct;
        typedef std::vector<visitor_fct>                              visitors;

        visitors                                                      m_state_visitors;
    };

    template <class StateType,class Enable=void>
    struct deferred_msg_queue_helper 
    {
    };
    template <class StateType>
    struct deferred_msg_queue_helper<StateType,
        typename ::boost::enable_if< 
            typename ::boost::msm::back::has_fsm_deferred_events<StateType>::type >::type> 
    {
    public:
        deferred_msg_queue_helper():m_deferred_events_queue(){}
        deferred_events_queue_t     m_deferred_events_queue;
    };

 public: 
    // tags
    typedef int composite_tag;

    // in case someone needs to know
    typedef HistoryPolicy               history_policy;

    struct InitEvent { };
    // flag handling
    struct Flag_AND
    {
        typedef std::logical_and<bool> type;
    };
    struct Flag_OR
    {
     typedef std::logical_or<bool> type;
    };
    typedef typename Derived::BaseAllStates     BaseState;
    typedef Derived                             ConcreteSM;

    // if the front-end fsm provides an initial_event typedef, replace InitEvent by this one
    typedef typename ::boost::mpl::eval_if< 
        typename has_initial_event<Derived>::type,
        get_initial_event<Derived>,
        ::boost::mpl::identity<InitEvent>
    >::type fsm_initial_event;


    template <class ExitPoint>
    struct exit_pt : public ExitPoint
    {
        // tags
        typedef ExitPoint           wrapped_exit;
        typedef int                 pseudo_exit;
        typedef library_sm          owner;
        typedef int                 no_automatic_create;
        typedef typename 
            ExitPoint::event        Event;
        typedef ::boost::function<execute_return (Event const&)>
                                    forwarding_function;

        // forward event to the higher-level FSM
        template <class ForwardEvent>
        void forward_event(ForwardEvent const& incomingEvent)
        {
            // use helper to forward or not
            ForwardHelper< ::boost::is_convertible<ForwardEvent,Event>::value>::helper(incomingEvent,m_forward);
        }
        void set_forward_fct(::boost::function<execute_return (Event const&)> fct)
        {
            m_forward = fct;
        }    
        exit_pt():m_forward(){}
        // by assignments, we keep our forwarding functor unchanged as our containing SM did not change
    template <class RHS>
        exit_pt(RHS& rhs):m_forward(){}
        exit_pt<ExitPoint>& operator= (const exit_pt<ExitPoint>& ) 
        { 
            return *this; 
        } 
    private:
         forwarding_function          m_forward;

         // using partial specialization instead of enable_if because of VC8 bug
        template <bool OwnEvent, int Dummy=0>
        struct ForwardHelper
        {
            template <class ForwardEvent>
            static void helper(ForwardEvent const& ,forwarding_function& )
            {
                // Not our event, ignore
            }
        };
        template <int Dummy>
        struct ForwardHelper<true,Dummy>
        {
            template <class ForwardEvent>
            static void helper(ForwardEvent const& incomingEvent,forwarding_function& forward_fct)
            {
                // call if handler set, if not, this state is simply a terminate state
                if (forward_fct)
                    forward_fct(incomingEvent);
            }
        };

    };
    template <class EntryPoint>
    struct entry_pt : public EntryPoint
    {
        // tags
        typedef EntryPoint          wrapped_entry;
        typedef int                 pseudo_entry;
        typedef library_sm          owner;
        typedef int                 no_automatic_create;
    };
    template <class EntryPoint>
    struct direct : public EntryPoint
    {
        // tags
        typedef EntryPoint          wrapped_entry;
        typedef int                 explicit_entry_state;
        typedef library_sm          owner;
        typedef int                 no_automatic_create;
    };
    typedef typename get_number_of_regions<typename Derived::initial_state>::type nr_regions;
    // Template used to form rows in the transition table
    template<
        typename ROW
    >
    struct row_
    {
        //typedef typename ROW::Source T1;
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        // if the source is an exit pseudo state, then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename ::boost::mpl::eval_if<
                typename has_pseudo_exit<T1>::type,
                get_owner<T1,library_sm>,
                ::boost::mpl::identity<typename ROW::Source> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::is_sequence<T2>::type,
            get_fork_owner<T2,library_sm>,
            ::boost::mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,library_sm>,
                    ::boost::mpl::identity<T2> >
        >::type next_state_type;

        // if a guard condition is here, call it to check that the event is accepted
        static bool check_guard(library_sm& fsm,transition_event const& evt)
        {
            if ( ROW::guard_call(fsm,evt,
                                 ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                                 ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                                 fsm.m_substate_list ) )
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {

            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (has_pseudo_exit<T1>::type::value && 
                !is_exit_state_active<T1,get_owner<T1,library_sm> >(fsm))
            {
                return HANDLED_FALSE;
            }
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return HANDLED_GUARD_REJECT;
            }

            // the guard condition has already been checked
            execute_exit<current_state_type>
                (::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),evt,fsm);

            // then call the action method
            ROW::action_call(fsm,evt,
                             ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                             ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                             fsm.m_substate_list);

            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>
                (::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),evt,fsm);
            fsm.m_states[region_index]=next_state;
            return HANDLED_TRUE;
        }
    };

    // row having only a guard condition
    template<
        typename ROW
    >
    struct g_row_
    {
        //typedef typename ROW::Source T1;
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        // if the source is an exit pseudo state, then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename ::boost::mpl::eval_if<
                typename has_pseudo_exit<T1>::type,
                get_owner<T1,library_sm>,
                ::boost::mpl::identity<typename ROW::Source> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::is_sequence<T2>::type,
            get_fork_owner<T2,library_sm>,
            ::boost::mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,library_sm>,
                    ::boost::mpl::identity<T2> >
        >::type next_state_type;

        // if a guard condition is defined, call it to check that the event is accepted
        static bool check_guard(library_sm& fsm,transition_event const& evt)
        {
            if ( ROW::guard_call(fsm,evt,
                                 ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                                 ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                                 fsm.m_substate_list ))
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (has_pseudo_exit<T1>::type::value && 
                !is_exit_state_active<T1,get_owner<T1,library_sm> >(fsm))
            {
                return HANDLED_FALSE;
            }
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return HANDLED_GUARD_REJECT;
            }
            // the guard condition has already been checked
            execute_exit<current_state_type>
                (::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),evt,fsm);

            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>
                (::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),evt,fsm);

            fsm.m_states[region_index]=next_state;
            return HANDLED_TRUE;
        }
    };

    // row having only an action method
    template<
        typename ROW
    >
    struct a_row_ 
    {
        //typedef typename ROW::Source T1;
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        // if the source is an exit pseudo state, then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename ::boost::mpl::eval_if<
                typename has_pseudo_exit<T1>::type,
                get_owner<T1,library_sm>,
                ::boost::mpl::identity<typename ROW::Source> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::is_sequence<T2>::type,
            get_fork_owner<T2,library_sm>,
            ::boost::mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,library_sm>,
                    ::boost::mpl::identity<T2> >
        >::type next_state_type;

        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));

            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (has_pseudo_exit<T1>::type::value && 
                !is_exit_state_active<T1,get_owner<T1,library_sm> >(fsm))
            {
                return HANDLED_FALSE;
            }
            // no need to check the guard condition
            // first call the exit method of the current state
            execute_exit<current_state_type>
                (::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),evt,fsm);

            // then call the action method
            ROW::action_call(fsm,evt,
                            ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                            ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                            fsm.m_substate_list);

            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>
                (::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),evt,fsm);

            fsm.m_states[region_index]=next_state;
            return HANDLED_TRUE;
        }
    };

    // row having no guard condition or action, simply transitions
    template<
        typename ROW
    >
    struct _row_
    {
        //typedef typename ROW::Source T1;
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        // if the source is an exit pseudo state, then
        // current_state_type becomes the result of get_owner
        // meaning the containing SM from which the exit occurs
        typedef typename ::boost::mpl::eval_if<
                typename has_pseudo_exit<T1>::type,
                get_owner<T1,library_sm>,
                ::boost::mpl::identity<typename ROW::Source> >::type current_state_type;

        // if Target is a sequence, then we have a fork and expect a sequence of explicit_entry
        // else if Target is an explicit_entry, next_state_type becomes the result of get_owner
        // meaning the containing SM if the row is "outside" the containing SM or else the explicit_entry state itself
        typedef typename ::boost::mpl::eval_if<
            typename ::boost::mpl::is_sequence<T2>::type,
            get_fork_owner<T2,library_sm>,
            ::boost::mpl::eval_if<
                    typename has_no_automatic_create<T2>::type,
                    get_owner<T2,library_sm>,
                    ::boost::mpl::identity<T2> >
        >::type next_state_type;

        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_STATIC_CONSTANT(int, next_state = (get_state_id<stt,next_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));

            // if T1 is an exit pseudo state, then take the transition only if the pseudo exit state is active
            if (has_pseudo_exit<T1>::type::value && 
                !is_exit_state_active<T1,get_owner<T1,library_sm> >(fsm))
            {
                return HANDLED_FALSE;
            }
            // first call the exit method of the current state
            execute_exit<current_state_type>
                (::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),evt,fsm);

            // and finally the entry method of the new current state
            convert_event_and_execute_entry<next_state_type,T2>
                (::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),evt,fsm);

            fsm.m_states[region_index]=next_state;
            return HANDLED_TRUE;
        }
    };
    // "i" rows are rows for internal transitions
    template<
        typename ROW
    >
    struct irow_
    {
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        typedef typename ROW::Source current_state_type;
        typedef T2 next_state_type;

        // if a guard condition is here, call it to check that the event is accepted
        static bool check_guard(library_sm& fsm,transition_event const& evt)
        {
            if ( ROW::guard_call(fsm,evt,
                                 ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                                 ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                                 fsm.m_substate_list))
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int , int state, transition_event const& evt)
        {

            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return HANDLED_GUARD_REJECT;
            }

            // call the action method
            ROW::action_call(fsm,evt,
                             ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                             ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                             fsm.m_substate_list);
            return HANDLED_TRUE;
        }
    };

    // row having only a guard condition
    template<
        typename ROW
    >
    struct g_irow_
    {
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        typedef typename ROW::Source current_state_type;
        typedef T2 next_state_type;

        // if a guard condition is defined, call it to check that the event is accepted
        static bool check_guard(library_sm& fsm,transition_event const& evt)
        {
            if ( ROW::guard_call(fsm,evt,
                                 ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                                 ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                                 fsm.m_substate_list) )
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int , int state, transition_event const& evt)
        {
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return HANDLED_GUARD_REJECT;
            }
            return HANDLED_TRUE;
        }
    };

    // row having only an action method
    template<
        typename ROW
    >
    struct a_irow_ 
    {
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;

        typedef typename ROW::Evt transition_event;
        typedef typename ROW::Source current_state_type;
        typedef T2 next_state_type;

        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));

            // call the action method
            ROW::action_call(fsm,evt,
                            ::boost::fusion::at_key<current_state_type>(fsm.m_substate_list),
                            ::boost::fusion::at_key<next_state_type>(fsm.m_substate_list),
                            fsm.m_substate_list);

            return HANDLED_TRUE;
        }
    };
    // row simply ignoring the event
    template<
        typename ROW
    >
    struct _irow_ 
    {
        typedef typename make_entry<typename ROW::Source,library_sm>::type T1;
        typedef typename make_exit<typename ROW::Target,library_sm>::type T2;
        typedef typename ROW::Evt transition_event;
        typedef typename ROW::Source current_state_type;
        typedef T2 next_state_type;

        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& , int , int state, transition_event const& )
        {
            BOOST_STATIC_CONSTANT(int, current_state = (get_state_id<stt,current_state_type>::type::value));
            BOOST_ASSERT(state == (current_state));
            return HANDLED_TRUE;
        }
    };
    // transitions internal to this state machine (no substate involved)
    template<
        typename ROW,
        typename StateType
    >
    struct internal_
    {
        typedef StateType current_state_type;
        typedef StateType next_state_type;
        typedef typename ROW::Evt transition_event;

        // if a guard condition is here, call it to check that the event is accepted
        static bool check_guard(library_sm& fsm,transition_event const& evt)
        {
            if ( ROW::guard_call(fsm,evt,
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                fsm.m_substate_list) )
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return HANDLED_GUARD_REJECT;
            }

            // then call the action method
            ROW::action_call(fsm,evt,
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                fsm.m_substate_list);
            return HANDLED_TRUE;
        }
    };
    template<
        typename ROW,
        typename StateType
    >
    struct a_internal_
    {
        typedef StateType current_state_type;
        typedef StateType next_state_type;
        typedef typename ROW::Evt transition_event;

        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            // then call the action method
            ROW::action_call(fsm,evt,
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                fsm.m_substate_list);
            return HANDLED_TRUE;
        }
    };
    template<
        typename ROW,
        typename StateType
    >
    struct g_internal_
    {
        typedef StateType current_state_type;
        typedef StateType next_state_type;
        typedef typename ROW::Evt transition_event;

        // if a guard condition is here, call it to check that the event is accepted
        static bool check_guard(library_sm& fsm,transition_event const& evt)
        {
            if ( ROW::guard_call(fsm,evt,
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                ::boost::fusion::at_key<StateType>(fsm.m_substate_list),
                fsm.m_substate_list) )
                return true;
            return false;
        }
        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int state, transition_event const& evt)
        {
            if (!check_guard(fsm,evt))
            {
                // guard rejected the event, we stay in the current one
                return HANDLED_GUARD_REJECT;
            }
            return HANDLED_TRUE;
        }
    };
    template<
        typename ROW,
        typename StateType
    >
    struct _internal_
    {
        typedef StateType current_state_type;
        typedef StateType next_state_type;
        typedef typename ROW::Evt transition_event;
        static HandledEnum execute(library_sm& , int , int , transition_event const& )
        {
            return HANDLED_TRUE;
        }
    };
    // Template used to form forwarding rows in the transition table for every row of a composite SM
    template<
        typename T1
        , class Evt
    >
    struct frow
    {
        typedef T1                  current_state_type;
        typedef T1                  next_state_type;
        typedef Evt                 transition_event;

        // Take the transition action and return the next state.
        static HandledEnum execute(library_sm& fsm, int region_index, int , transition_event const& evt)
        {
             execute_return res = 
                (::boost::fusion::at_key<current_state_type>(fsm.m_substate_list)).process_event(evt); 
             fsm.m_states[region_index]=get_state_id<stt,T1>::type::value;
             return res;
        }
    };

    template <class Tag, class Transition,class StateType>
    struct create_backend_stt
    {
    };
    template <class Transition,class StateType>
    struct create_backend_stt<g_row_tag,Transition,StateType>
    {
        typedef g_row_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<a_row_tag,Transition,StateType>
    {
        typedef a_row_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<_row_tag,Transition,StateType>
    {
        typedef _row_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<row_tag,Transition,StateType>
    {
        typedef row_<Transition> type;
    };
    // internal transitions
    template <class Transition,class StateType>
    struct create_backend_stt<g_irow_tag,Transition,StateType>
    {
        typedef g_irow_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<a_irow_tag,Transition,StateType>
    {
        typedef a_irow_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<irow_tag,Transition,StateType>
    {
        typedef irow_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<_irow_tag,Transition,StateType>
    {
        typedef _irow_<Transition> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<sm_a_i_row_tag,Transition,StateType>
    {
        typedef a_internal_<Transition,StateType> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<sm_g_i_row_tag,Transition,StateType>
    {
        typedef g_internal_<Transition,StateType> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<sm_i_row_tag,Transition,StateType>
    {
        typedef internal_<Transition,StateType> type;
    };
    template <class Transition,class StateType>
    struct create_backend_stt<sm__i_row_tag,Transition,StateType>
    {
        typedef _internal_<Transition,StateType> type;
    };
    template <class Transition,class StateType=void>
    struct make_row_tag
    {
        typedef typename create_backend_stt<typename Transition::row_type_tag,Transition,StateType>::type type;
    };

    // add to the stt the initial states which could be missing (if not being involved in a transition)
    template <class BaseType, class stt_simulated = typename BaseType::transition_table>
    struct create_real_stt 
    {
        //typedef typename BaseType::transition_table stt_simulated;
        typedef typename ::boost::mpl::fold<
            stt_simulated,mpl::vector0<>,
            ::boost::mpl::push_back< ::boost::mpl::placeholders::_1,
                                     make_row_tag< ::boost::mpl::placeholders::_2 , BaseType > >
        >::type type;
    };

    template <class Table,class Intermediate,class StateType>
    struct add_forwarding_row_helper
    {
        typedef typename generate_event_set<Table>::type all_events;
        typedef typename ::boost::mpl::fold<
            all_events, Intermediate,
            ::boost::mpl::push_back< ::boost::mpl::placeholders::_1,
            frow<StateType, ::boost::mpl::placeholders::_2> > >::type type;
    };
    // gets the transition table from a composite and make from it a forwarding row
    template <class StateType,class IsComposite>
    struct get_internal_transition_table
    {
        // first get the table of a composite
        typedef typename recursive_get_transition_table<StateType>::type original_table;

        // add the internal events defined in the internal_transition_table
        // Note: these are added first because they must have a lesser prio
        // than the deeper transitions in the sub regions
        typedef typename StateType::internal_transition_table istt_simulated;
        typedef typename ::boost::mpl::fold<
            istt_simulated,::boost::mpl::vector0<>,
            ::boost::mpl::push_back< ::boost::mpl::placeholders::_1,
                                     make_row_tag< ::boost::mpl::placeholders::_2 , StateType> >
        >::type intermediate;

        // and add for every event a forwarding row
        typedef typename ::boost::mpl::eval_if<
                typename CompilePolicy::add_forwarding_rows,
                add_forwarding_row_helper<original_table,intermediate,StateType>,
                ::boost::mpl::identity<intermediate>
        >::type type;
    };
    template <class StateType>
    struct get_internal_transition_table<StateType, ::boost::mpl::false_ >
    {
        typedef typename create_real_stt<StateType, typename StateType::internal_transition_table >::type type;
    };
    typedef typename create_real_stt<Derived>::type real_transition_table;
    typedef typename create_stt<library_sm>::type stt;
    typedef typename get_initial_states<typename Derived::initial_state>::type initial_states;
    typedef typename generate_state_set<stt>::type state_list;
    typedef typename HistoryPolicy::template apply<nr_regions::value>::type concrete_history;

    typedef typename ::boost::fusion::result_of::as_set<state_list>::type substate_list;

    // extends the transition table with rows from composite states
    template <class Composite>
    struct extend_table
    {
        // add the init states
        //typedef typename create_stt<Composite>::type stt;
        typedef typename Composite::stt Stt;
        // for every state, add its transition table (if any)
        // transformed as frow
        typedef typename ::boost::mpl::fold<state_list,Stt,
                ::boost::mpl::insert_range< 
                        ::boost::mpl::placeholders::_1, 
                        ::boost::mpl::end< ::boost::mpl::placeholders::_1>,
                        get_internal_transition_table< 
                                ::boost::mpl::placeholders::_2,
                                is_composite_state< ::boost::mpl::placeholders::_2> > > 
        >::type type;
    };
    // extend the table with tables from composite states
    typedef typename extend_table<library_sm>::type complete_table;
     // build a sequence of regions
     typedef typename get_regions_as_sequence<typename Derived::initial_state>::type seq_initial_states;
    // Member functions

    // start the state machine (calls entry of the initial state)
    void start()
    {
        // call on_entry on this SM
        (static_cast<Derived*>(this))->on_entry(fsm_initial_event(),*this);
        ::boost::mpl::for_each<initial_states, boost::msm::wrap<mpl::placeholders::_1> >
            (call_init<fsm_initial_event>(fsm_initial_event(),this));
        // give a chance to handle an anonymous (eventless) transition
        handle_eventless_transitions_helper<library_sm> eventless_helper(this,true);
        eventless_helper.process_completion_event();

    }

    // Main function used by clients of the derived FSM to make
    // transitions. Can also be called for internally (for example in an action method) generated events.
    template<class Event>
    execute_return process_event(Event const& evt)
    {
        HandledEnum ret_handled=HANDLED_FALSE;
        // if the state machine has terminate or interrupt flags, check them, otherwise skip
        if (is_event_handling_blocked_helper<Event>
                ( ::boost::mpl::bool_<has_fsm_blocking_states<library_sm>::type::value>() ) )
            return HANDLED_TRUE;
        // if a message queue is needed and processing is on the way
        if (!do_pre_msg_queue_helper<Event>
                (evt,::boost::mpl::bool_<is_no_message_queue<library_sm>::type::value>()) )
        {
            // wait for the end of current processing
            return HANDLED_TRUE;
        }
        else
        {
            // prepare the next deferred event for handling
            // if one defer is found in the SM, otherwise skip
            handle_defer_helper<library_sm> defer_helper(m_deferred_events_queue);
            defer_helper.do_pre_handle_deferred();
            // process event
            HandledEnum handled = this->do_process_helper<Event>
                (evt,::boost::mpl::bool_<is_no_exception_thrown<library_sm>::type::value>());
            if (handled)
            {
                ret_handled = HANDLED_TRUE;
            }

            // process completion transitions BEFORE any other event in the pool (UML Standard 2.3 �15.3.14)
            handle_eventless_transitions_helper<library_sm> eventless_helper(this,(handled == HANDLED_TRUE));
            eventless_helper.process_completion_event();

            // after handling, take care of the deferred events
            defer_helper.do_post_handle_deferred(handled);

            // now check if some events were generated in a transition and was not handled
            // because of another processing, and if yes, start handling them
            do_post_msg_queue_helper(::boost::mpl::bool_<is_no_message_queue<library_sm>::type::value>());

            return ret_handled;
        }       
    }

    // Getter that returns the current state of the FSM
    const int* current_state() const
    {
        return this->m_states;
    }

    template <class Archive>
    struct serialize_state
    {
        serialize_state(Archive& ar):ar_(ar){}

        template<typename T>
        typename ::boost::enable_if< 
            typename ::boost::mpl::or_<
                typename has_do_serialize<T>::type,
                typename is_composite_state<T>::type
            >::type
            ,void 
        >::type
        operator()(T& t) const
        {
            ar_ & t;
        }
        template<typename T>
        typename ::boost::disable_if< 
            typename ::boost::mpl::or_<
                typename has_do_serialize<T>::type,
                typename is_composite_state<T>::type
            >::type
            ,void 
        >::type
        operator()(T& t) const
        {
            // no state to serialize
        }
        Archive& ar_;
    };
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        // invoke serialization of the base class 
        (serialize_state<Archive>(ar))(boost::serialization::base_object<Derived>(*this));
        // now our attributes
        ar & m_states;
        // queues cannot be serialized => skip
        ar & m_history;
        ar & m_event_processing;
        ar & m_is_included;
        // visitors cannot be serialized => skip
        ::boost::fusion::for_each(m_substate_list, serialize_state<Archive>(ar));
    }

    // linearly search for the state with the given id
    struct get_state_id_helper 
    {
        get_state_id_helper(int id,const BaseState** res,const library_sm* self_):
        result_state(res),searched_id(id),self(self_) {}

        template <class StateType>
        void operator()(boost::msm::wrap<StateType> const&)
        {
            // look for the state id until found
            BOOST_STATIC_CONSTANT(int, id = (get_state_id<stt,StateType>::value));
            if (!*result_state && (id == searched_id))
            {
                *result_state = &::boost::fusion::at_key<StateType>(self->m_substate_list);
            }
        }
        const BaseState**  result_state;
        int                searched_id;
        const library_sm* self;
    };
    // return the state whose id is passed or 0 if not found
    // caution if you need this, you probably need polymorphic states
    // complexity: O(number of states)
    const BaseState* get_state_by_id(int id) const
    {
        const BaseState*  result_state=0;
        ::boost::mpl::for_each<state_list, 
            ::boost::msm::wrap< ::boost::mpl::placeholders::_1> > (get_state_id_helper(id,&result_state,this));
        return result_state;
    }
    // true if the sm is used in another sm
    bool is_contained() const
    {
        return m_is_included;
    }
    // get a state
    // as a pointer
    template <class State>
    typename ::boost::enable_if<typename ::boost::is_pointer<State>::type,State >::type
    get_state(::boost::msm::back::dummy<0> = 0)
    {
        return &(static_cast<typename boost::add_reference<typename ::boost::remove_pointer<State>::type>::type > 
        (::boost::fusion::at_key<typename ::boost::remove_pointer<State>::type>(m_substate_list)));
    }
    // as a reference
    template <class State>
    typename ::boost::enable_if<typename ::boost::is_reference<State>::type,State >::type
    get_state(::boost::msm::back::dummy<1> = 0)
    {
        return ::boost::fusion::at_key<typename ::boost::remove_reference<State>::type>(m_substate_list);
    }

    // checks if a flag is active using the BinaryOp as folding function
    template <class Flag,class BinaryOp>
    bool is_flag_active()
    {
        flag_handler* flags_entries = get_entries_for_flag<Flag>();

        return std::accumulate(m_states,
            m_states+nr_regions::value,false,
            ::boost::bind(typename BinaryOp::type(),
                    ::boost::bind(::boost::apply<bool>(),
                    ::boost::bind(::boost::msm::back::deref<flag_handler>(),
                        ::boost::bind(::boost::msm::back::plus2<flag_handler*,int>(),
                        flags_entries, _2)),
                        ::boost::ref(*this)), _1));
    }
    // checks if a flag is active using no binary op if 1 region, or OR if > 1 regions
    template <class Flag>
    bool is_flag_active()
    {
        return FlagHelper<Flag,(nr_regions::value>1)>::helper(*this,get_entries_for_flag<Flag>());
    }
    // visit the currently active states (if these are defined as visitable
    // by implementing accept)
    void visit_current_states()
    {
        for (int i=0; i<nr_regions::value;++i)
        {
            m_visitors.execute(m_states[i]);
        }
    }
#define MSM_VISIT_STATE_SUB(z, n, unused) ARG ## n vis ## n
#define MSM_VISIT_STATE_EXECUTE(z, n, unused)                                    \
        template <BOOST_PP_ENUM_PARAMS(n, class ARG)>                               \
        void visit_current_states(BOOST_PP_ENUM(n, MSM_VISIT_STATE_SUB, ~ ) )         \
        {                                                                           \
            for (int i=0; i<nr_regions::value;++i)                                                      \
            {                                                                                           \
                m_visitors.execute(m_states[i],BOOST_PP_ENUM_PARAMS(n,vis));                            \
            }                                                                                           \
        }
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(BOOST_MSM_VISITOR_ARG_SIZE,1), MSM_VISIT_STATE_EXECUTE, ~)
#undef MSM_VISIT_STATE_EXECUTE
#undef MSM_VISIT_STATE_SUB

    // puts the given event into the deferred queue
    template <class Event>
    void defer_event(Event const& e)
    {
        // to call this function, you need either a state with a deferred_events typedef
        // or that the fsm provides the activate_deferred_events typedef
        BOOST_MPL_ASSERT(( has_fsm_deferred_events<library_sm> ));
        execute_return (library_sm::*pf) (Event const& evt)= &library_sm::process_event;
        Event temp (e);
        ::boost::function<execute_return () > f= ::boost::bind(pf, this,temp);
        post_deferred_event(f);
    }

 protected:    // interface for the derived class

     // helper used to fill the initial states
     struct init_states
     {
         init_states(int* const init):m_initial_states(init),m_index(-1){}

         // History initializer function object, used with mpl::for_each
         template <class State>
         void operator()(::boost::msm::wrap<State> const&)
         {
             m_initial_states[++m_index]=get_state_id<stt,State>::type::value;
         }
         int* const m_initial_states;
         int m_index;
     };
     public:
     // Construct with the default initial states
     state_machine<Derived,HistoryPolicy,CompilePolicy   >()
         :Derived()
         ,m_events_queue() 
         ,m_deferred_events_queue()
         ,m_history()
         ,m_event_processing(false)
         ,m_is_included(false)
         ,m_visitors()
         ,m_substate_list()
     {
         // initialize our list of states with the ones defined in Derived::initial_state
         ::boost::mpl::for_each< seq_initial_states, ::boost::msm::wrap<mpl::placeholders::_1> >
                        (init_states(m_states));
         m_history.set_initial_states(m_states);
         // create states
         fill_states(this);
     }

     // Construct with the default initial states and some default argument(s)
#define MSM_CONSTRUCTOR_HELPER_EXECUTE_SUB(z, n, unused) ARG ## n t ## n
#define MSM_CONSTRUCTOR_HELPER_EXECUTE(z, n, unused)                                \
        template <BOOST_PP_ENUM_PARAMS(n, class ARG)>                               \
        state_machine<Derived,HistoryPolicy,CompilePolicy                           \
        >(BOOST_PP_ENUM(n, MSM_CONSTRUCTOR_HELPER_EXECUTE_SUB, ~ ) )                \
        :Derived(BOOST_PP_ENUM_PARAMS(n,t))                                         \
         ,m_events_queue()                                                          \
         ,m_deferred_events_queue()                                                 \
         ,m_history()                                                               \
         ,m_event_processing(false)                                                 \
         ,m_is_included(false)                                                      \
         ,m_visitors()                                                              \
         ,m_substate_list()                                                         \
     {                                                                              \
         ::boost::mpl::for_each< seq_initial_states, ::boost::msm::wrap<mpl::placeholders::_1> > \
                        (init_states(m_states));                                    \
         m_history.set_initial_states(m_states);                                    \
         fill_states(this);                                                         \
     }
     BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(BOOST_MSM_CONSTRUCTOR_ARG_SIZE,1), MSM_CONSTRUCTOR_HELPER_EXECUTE, ~)
#undef MSM_CONSTRUCTOR_HELPER_EXECUTE
#undef MSM_CONSTRUCTOR_HELPER_EXECUTE_SUB



     // assignment operator using the copy policy to decide if non_copyable, shallow or deep copying is necessary
     library_sm& operator= (library_sm const& rhs)
     {
         if (this != &rhs) 
         {
            Derived::operator=(rhs);
            // initialize our list of states with the ones defined in Derived::initial_state
            fill_states(this);
            do_copy(rhs);
         }
        return *this;
     }
     state_machine<Derived,HistoryPolicy,CompilePolicy> 
         (library_sm const& rhs)
         : Derived(rhs)
     {
        if (this != &rhs) 
        {
            // initialize our list of states with the ones defined in Derived::initial_state
            fill_states(this);
            do_copy(rhs);
        }
     }

    // the following 2 functions handle the terminate/interrupt states handling
    // if one of these states is found, the first one is used
    template <class Event>
    bool is_event_handling_blocked_helper( ::boost::mpl::true_ const &)
    {
        // if the state machine is terminated, do not handle any event
        if (is_flag_active< ::boost::msm::TerminateFlag>())
            return true;
        // if the state machine is interrupted, do not handle any event
        // unless the event is the end interrupt event
        if ( is_flag_active< ::boost::msm::InterruptedFlag>() && 
            !is_flag_active< ::boost::msm::EndInterruptFlag<Event> >())
            return true;
        return false;
    }
    // otherwise simple handling, no flag => continue
    template <class Event>
    bool is_event_handling_blocked_helper( ::boost::mpl::false_ const &)
    {
        // no terminate/interrupt states detected
        return false;
    }
    // the following functions handle pre/post-process handling  of a message queue
    template <class StateType,class EventType>
    bool do_pre_msg_queue_helper(EventType const& evt, ::boost::mpl::true_ const &)
    {
        // no message queue needed
        return true;
    }
    template <class StateType,class EventType>
    bool do_pre_msg_queue_helper(EventType const& evt, ::boost::mpl::false_ const &)
    {
        execute_return (library_sm::*pf) (EventType const& evt) = 
            &library_sm::process_event; 
        // if we are already processing an event
        if (m_event_processing)
        {
            // event has to be put into the queue
            transition_fct f = ::boost::bind(pf,this,evt);
            m_events_queue.m_events_queue.push(f);
            return false;
        }
        // event can be handled, processing
        m_event_processing = true;
        return true;
    }
    void do_post_msg_queue_helper( ::boost::mpl::true_ const &)
    {
        // no message queue needed
    }
    void do_post_msg_queue_helper( ::boost::mpl::false_ const &)
    {
        m_event_processing = false;
        process_message_queue(this);
    }
    // the following 2 functions handle the processing either with a try/catch protection or without
    template <class StateType,class EventType>
    HandledEnum do_process_helper(EventType const& evt, ::boost::mpl::true_ const &)
    {
        return this->do_process_event(evt);
    }
    template <class StateType,class EventType>
    HandledEnum do_process_helper(EventType const& evt, ::boost::mpl::false_ const &)
    {
        try
        {
            return this->do_process_event(evt);
        }
        catch (std::exception& e)
        {
            // give a chance to the concrete state machine to handle
            this->exception_caught(evt,*this,e);
        } 
        return HANDLED_FALSE;
    }
    // handling of deferred events
    // if none is found in the SM, take the following empty main version
    template <class StateType, class Enable = void> 
    struct handle_defer_helper
    {
        handle_defer_helper(deferred_msg_queue_helper<library_sm>& ){}
        void do_pre_handle_deferred()
        {
        }

        void do_post_handle_deferred(HandledEnum)
        {
        }
    };
    // otherwise the standard version handling the deferred events
    template <class StateType>
    struct handle_defer_helper
        <StateType, typename enable_if< typename ::boost::msm::back::has_fsm_deferred_events<StateType>::type >::type>
    {
        handle_defer_helper(deferred_msg_queue_helper<library_sm>& a_queue):
            events_queue(a_queue),next_deferred_event(){}
        void do_pre_handle_deferred()
        {
            if (!events_queue.m_deferred_events_queue.empty())
            {
                next_deferred_event = events_queue.m_deferred_events_queue.back();
                events_queue.m_deferred_events_queue.pop_back();
            }
        }

        void do_post_handle_deferred(HandledEnum handled)
        {
            if (((handled & HANDLED_DEFERRED) == HANDLED_DEFERRED) && next_deferred_event )
            {
                // the event was already deferred, no reason to process another deferred event
                events_queue.m_deferred_events_queue.push_back(next_deferred_event);
                return;
            }
            else if (next_deferred_event)
            {
                next_deferred_event();
            }
        }

    private:
        deferred_msg_queue_helper<library_sm>&  events_queue;
        deferred_fct                            next_deferred_event;
    };

    // handling of eventless transitions
    // if none is found in the SM, nothing to do
    template <class StateType, class Enable = void> 
    struct handle_eventless_transitions_helper
    {
        handle_eventless_transitions_helper(library_sm* , bool ){}
        void process_completion_event(){}
    };
    // otherwise 
    template <class StateType>
    struct handle_eventless_transitions_helper
        <StateType, typename enable_if< typename ::boost::msm::back::has_fsm_eventless_transition<StateType>::type >::type>
    {
        handle_eventless_transitions_helper(library_sm* self_, bool handled_):self(self_),handled(handled_){}
        void process_completion_event()
        {
            typedef typename ::boost::mpl::deref<
                typename ::boost::mpl::begin<
                    typename find_completion_events<StateType>::type
                        >::type
            >::type first_completion_event;
            if (handled)
            {
                self->process_event(first_completion_event() );
            }
        }
 
    private:
        library_sm* self;
        bool        handled;
    };

    template <class StateType,class Enable=void>
    struct region_processing_helper 
    {
    public:
        region_processing_helper(library_sm* self_,HandledEnum& result_)
            :self(self_),result(result_){}
        template<class Event>
        void process(Event const& evt)
        {
            // use this table as if it came directly from the user
            typedef dispatch_table<library_sm,complete_table,Event,CompilePolicy> table;
            HandledEnum res =
                table::instance.entries[self->m_states[0]](
                *self, 0, self->m_states[0], evt);
            result = (HandledEnum)((int)result | (int)res);
        }
        library_sm*     self;
        HandledEnum&    result;
    };
    // version with visitors
    template <class StateType>
    struct region_processing_helper<StateType,typename ::boost::enable_if< 
                        ::boost::mpl::is_sequence<typename StateType::initial_state> >::type> 
    {
        private:
        // process event in one region
        template <class region_id,int Dummy=0>
        struct In
        {
            template<class Event>
            static void process(Event const& evt,library_sm* self_,HandledEnum& result_)
            {
                // use this table as if it came directly from the user
                typedef dispatch_table<library_sm,complete_table,Event,CompilePolicy> table;
                HandledEnum res =
                    table::instance.entries[self_->m_states[region_id::value]](
                    *self_, region_id::value , self_->m_states[region_id::value], evt);
                result_ = (HandledEnum)((int)result_ | (int)res);
                In< ::boost::mpl::int_<region_id::value+1> >::process(evt,self_,result_);
            }
        };
        template <int Dummy>
        struct In< ::boost::mpl::int_<nr_regions::value>,Dummy>
        {
            // end of processing
            template<class Event>
            static void process(Event const& evt,library_sm*,HandledEnum&){}
        };
        public:
        region_processing_helper(library_sm* self_,HandledEnum& result_)
            :self(self_),result(result_){}
        template<class Event>
        void process(Event const& evt)
        {
            // use this table as if it came directly from the user
            typedef dispatch_table<library_sm,complete_table,Event,CompilePolicy> table;
            In< ::boost::mpl::int_<0> >::process(evt,self,result);
        }

        library_sm*     self;
        HandledEnum&    result;
    };

    // minimum event processing without exceptions, queues, etc.
    template<class Event>
    HandledEnum do_process_event(Event const& evt)
    {
        HandledEnum handled = HANDLED_FALSE;
        // dispatch the event to every region
        region_processing_helper<Derived> helper(this,handled);
        helper.process(evt);

        // if the event has not been handled and we have orthogonal zones, then
        // generate an error on every active state 
        // for state machine states contained in other state machines, do not handle
        // but let the containing sm handle the error
        // completion events do not produce an error
        if (!handled && !is_contained() && !is_completion_event<Event>::type::value)
        {
            for (int i=0; i<nr_regions::value;++i)
            {
                this->no_transition(evt,*this,this->m_states[i]);
            }
        }
        return handled;
    }

    // default row arguments for the compilers which accept this
    template <class Event>
    bool no_guard(Event const&){return true;}
    template <class Event>
    void no_action(Event const&){}

#ifndef BOOST_NO_RTTI
    HandledEnum process_any_event( ::boost::any const& evt);
#endif

private:
    // composite accept implementation. First calls accept on the composite, then accept on all its active states.
    void composite_accept()
    {
        this->accept();
        this->visit_current_states();
    }

#define MSM_COMPOSITE_ACCEPT_SUB(z, n, unused) ARG ## n vis ## n
#define MSM_COMPOSITE_ACCEPT_EXECUTE(z, n, unused)                                      \
        template <BOOST_PP_ENUM_PARAMS(n, class ARG)>                                   \
        void composite_accept(BOOST_PP_ENUM(n, MSM_COMPOSITE_ACCEPT_SUB, ~ ) )               \
        {                                                                               \
            this->accept(BOOST_PP_ENUM_PARAMS(n,vis));                                        \
            this->visit_current_states(BOOST_PP_ENUM_PARAMS(n,vis));                          \
        }
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(BOOST_MSM_VISITOR_ARG_SIZE,1), MSM_COMPOSITE_ACCEPT_EXECUTE, ~)
#undef MSM_COMPOSITE_ACCEPT_EXECUTE
#undef MSM_COMPOSITE_ACCEPT_SUB

    // helper used to call the init states at the start of the state machine
    template <class Event>
    struct call_init
    {
        call_init(Event const& an_event,library_sm* self_):
                evt(an_event),self(self_){}
        template <class State>
        void operator()(boost::msm::wrap<State> const&)
        {
            execute_entry(::boost::fusion::at_key<State>(self->m_substate_list),evt,*self);
        }
    private:
        Event const& evt;
        library_sm* self;
    };
    // helper for flag handling. Uses OR by default on orthogonal zones.
    template <class Flag,bool orthogonalStates>
    struct FlagHelper 
    {
        static bool helper(library_sm& sm,flag_handler* )
        {
            // by default we use OR to accumulate the flags
            return sm.is_flag_active<Flag,Flag_OR>();
        }
    };
    template <class Flag>
    struct FlagHelper<Flag,false>
    {
        static bool helper(library_sm& sm,flag_handler* flags_entries)
        {
            // just one active state, so we can call operator[] with 0
            return flags_entries[sm.current_state()[0]](sm);
        }
    };
    // handling of flag
    // defines a true and false functions plus a forwarding one for composite states
    template <class StateType,class Flag>
    struct FlagHandler
    {
        static bool flag_true(library_sm& )
        {
            return true;
        }
        static bool flag_false(library_sm& )
        {
            return false;
        }
        static bool forward(library_sm& fsm)
        {
            return ::boost::fusion::at_key<StateType>(fsm.m_substate_list).template is_flag_active<Flag>();
        }
    };
    template <class Flag>
    struct init_flags
    {
    private:
        // helper function, helps hiding the forward function for non-state machines states.
        template <class T>
        void helper (flag_handler* an_entry,int offset, ::boost::mpl::true_ const &  )
        {
            // composite => forward
            an_entry[offset] = &FlagHandler<T,Flag>::forward;
        }
        template <class T>
        void helper (flag_handler* an_entry,int offset, ::boost::mpl::false_ const &  )
        {
            // default no flag
            an_entry[offset] = &FlagHandler<T,Flag>::flag_false;
        }
        // attributes
        flag_handler* entries;

    public:
        init_flags(flag_handler* entries_)
            : entries(entries_)
        {}

        // Flags initializer function object, used with mpl::for_each
        template <class StateType>
        void operator()( ::boost::msm::wrap<StateType> const& )
        {
            typedef typename StateType::flag_list flags;
            typedef typename ::boost::mpl::contains<flags,Flag >::type found;
            typedef typename is_composite_state<StateType>::type composite;

            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,StateType>::type::value));
            if (found::type::value)
            {
                // the type defined the flag => true
                entries[state_id] = &FlagHandler<StateType,Flag>::flag_true;
            }
            else
            {
                // false or forward
                typedef typename ::boost::mpl::and_<
                            typename is_composite_state<StateType>::type,
                            typename ::boost::mpl::not_<
                                    typename has_non_forwarding_flag<Flag>::type>::type >::type composite_no_forward;

                helper<StateType>(entries,state_id,::boost::mpl::bool_<composite_no_forward::type::value>());
            }
        }
    };
    // maintains for every flag a static array containing the flag value for every state
    template <class Flag>
    flag_handler* get_entries_for_flag()
    {
        BOOST_STATIC_CONSTANT(int, max_state = (mpl::size<state_list>::value));

        static flag_handler flags_entries[max_state];
        // build a state list
        ::boost::mpl::for_each<state_list, boost::msm::wrap< ::boost::mpl::placeholders::_1> >
                        (init_flags<Flag>(flags_entries));
        return flags_entries;
    }

    // helper used to create a state using the correct constructor
    template <class State, class Enable=void>
    struct create_state_helper
    {
        static void set_sm(library_sm* )
        {
            // state doesn't need its sm
        }
    };
    // create a state requiring a pointer to the state machine
    template <class State>
    struct create_state_helper<State,typename boost::enable_if<typename State::needs_sm >::type>
    {
        static void set_sm(library_sm* sm)
        {
            // create and set the fsm
            ::boost::fusion::at_key<State>(sm->m_substate_list).set_sm_ptr(sm);
        }
    };
        // main unspecialized helper class
        template <class StateType,int ARGS>
        struct visitor_args;

#define MSM_VISITOR_ARGS_SUB(z, n, unused) BOOST_PP_CAT(_,BOOST_PP_ADD(n,1))
#define MSM_VISITOR_ARGS_TYPEDEF_SUB(z, n, unused) typename StateType::accept_sig::argument ## n

#define MSM_VISITOR_ARGS_EXECUTE(z, n, unused)                                              \
    template <class StateType>                                                              \
    struct visitor_args<StateType,n>                                                        \
    {                                                                                       \
        template <class State>                                                              \
        static typename enable_if_c<!is_composite_state<State>::value,void >::type          \
        helper (library_sm* sm,                                                             \
        int id,StateType& astate)                                                           \
        {                                                                                   \
            sm->m_visitors.insert(id, boost::bind(&StateType::accept,                       \
                ::boost::ref(astate) BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, MSM_VISITOR_ARGS_SUB, ~) ));   \
        }                                                                                   \
        template <class State>                                                              \
        static typename enable_if_c<is_composite_state<State>::value,void >::type           \
        helper (library_sm* sm,                                                             \
        int id,StateType& astate)                                                           \
        {                                                                                   \
            void (StateType::*caccept)(BOOST_PP_ENUM(n, MSM_VISITOR_ARGS_TYPEDEF_SUB, ~ ) )           \
                                        = &StateType::composite_accept;                     \
            sm->m_visitors.insert(id, boost::bind(caccept,             \
            ::boost::ref(astate) BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(n, MSM_VISITOR_ARGS_SUB, ~) ));                 \
        }                                                                                   \
};
BOOST_PP_REPEAT(BOOST_PP_ADD(BOOST_MSM_VISITOR_ARG_SIZE,1), MSM_VISITOR_ARGS_EXECUTE, ~)
#undef MSM_VISITOR_ARGS_EXECUTE
#undef MSM_VISITOR_ARGS_SUB

// the IBM compiler seems to have problems with nested classes
// the same seems to apply to the Apple version of gcc 4.0.1 (just in case we do for < 4.1)
// and also to MS VC < 8
#if defined (__IBMCPP__) || (defined (__APPLE_CC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 1)) || (defined(_MSC_VER) && (_MSC_VER < 1400))
     public:
#endif
    template<class ContainingSM>
    void set_containing_sm(ContainingSM* sm)
    {
        m_is_included=true;
        ::boost::fusion::for_each(m_substate_list,add_state<ContainingSM>(this,sm));
    }
#if defined (__IBMCPP__) || (defined (__APPLE_CC__) && (__GNUC__ == 4 && __GNUC_MINOR__ < 1)) || (defined(_MSC_VER) && (_MSC_VER < 1400))
     private:
#endif
    // A function object for use with mpl::for_each that stuffs
    // states into the state list.
    template<class ContainingSM>
    struct add_state
    {
        add_state(library_sm* self_,ContainingSM* sm)
            : self(self_),containing_sm(sm){}

        // State is a sub fsm with exit pseudo states and gets a pointer to this fsm, so it can build a callback
        template <class StateType>
        typename ::boost::enable_if<
            typename is_composite_state<StateType>::type,void >::type
        new_state_helper(boost::msm::back::dummy<0> = 0) const
        {
            ::boost::fusion::at_key<StateType>(self->m_substate_list).set_containing_sm(containing_sm);
        }
        // State is a sub fsm without exit pseudo states and does not get a callback to this fsm
        // or state is a normal state and needs nothing except creation
        template <class StateType>
        typename ::boost::enable_if<
            typename boost::mpl::and_<typename boost::mpl::not_
                                                    <typename is_composite_state<StateType>::type>::type,
                                      typename boost::mpl::not_
                                                    <typename is_pseudo_exit<StateType>::type>::type
                   >::type,void>::type
        new_state_helper( ::boost::msm::back::dummy<1> = 0) const
        {
            //nothing to do
        }
        // state is exit pseudo state and gets callback to target fsm
        template <class StateType>
        typename ::boost::enable_if<typename is_pseudo_exit<StateType>::type,void >::type
        new_state_helper( ::boost::msm::back::dummy<2> = 0) const
        {
            execute_return (ContainingSM::*pf) (typename StateType::event const& evt)= 
                &ContainingSM::process_event;
            ::boost::function<execute_return (typename StateType::event const&)> fct = 
                ::boost::bind(pf,containing_sm,_1);
            ::boost::fusion::at_key<StateType>(self->m_substate_list).set_forward_fct(fct);
        }
        // for every defined state in the sm
        template <class State>
        void operator()( State const&) const
        {
            //create a new state with the defined id and type
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,State>::value));

            this->new_state_helper<State>(),
            create_state_helper<State>::set_sm(self);
            // create a visitor callback
            visitor_helper(state_id,::boost::fusion::at_key<State>(self->m_substate_list),
                           ::boost::mpl::bool_<has_accept_sig<State>::type::value>());
        }
    private:
        // support possible use of a visitor if accept_sig is defined
        template <class StateType>
        void visitor_helper(int id,StateType& astate, ::boost::mpl::true_ const & ) const
        {
            visitor_args<StateType,StateType::accept_sig::args_number>::
                template helper<StateType>(self,id,astate);
        }
        template <class StateType>
        void visitor_helper(int ,StateType& , ::boost::mpl::false_ const &) const
        {
            // nothing to do
        }

        library_sm*      self;
        ContainingSM*    containing_sm;
    };

     // helper used to copy every state if needed
     struct copy_helper
     {
         copy_helper(library_sm* sm):
           m_sm(sm){}
         template <class StateType>
         void operator()( ::boost::msm::wrap<StateType> const& )
         {
            BOOST_STATIC_CONSTANT(int, state_id = (get_state_id<stt,StateType>::type::value));
            // possibly also set the visitor
            visitor_helper<StateType>(state_id);

            // and for states that keep a pointer to the fsm, reset the pointer
            create_state_helper<StateType>::set_sm(m_sm);
         }
         template <class StateType>
         typename ::boost::enable_if<typename has_accept_sig<StateType>::type,void >::type
             visitor_helper(int id) const
         {
             visitor_args<StateType,StateType::accept_sig::args_number>::template helper<StateType>
                 (m_sm,id,::boost::fusion::at_key<StateType>(m_sm->m_substate_list));
         }
         template <class StateType>
         typename ::boost::disable_if<typename has_accept_sig<StateType>::type,void >::type
             visitor_helper(int id) const
         {
             // nothing to do
         }

         library_sm*     m_sm;
     };
     // helper to copy the active states attribute
     template <class region_id,int Dummy=0>
     struct region_copy_helper
     {
         static void do_copy(library_sm* self_,library_sm const& rhs)
         {
             self_->m_states[region_id::value] = rhs.m_states[region_id::value];
             region_copy_helper< ::boost::mpl::int_<region_id::value+1> >::do_copy(self_,rhs);
         }
     };
     template <int Dummy>
     struct region_copy_helper< ::boost::mpl::int_<nr_regions::value>,Dummy>
     {
         // end of processing
         static void do_copy(library_sm*,library_sm const& ){}
     };
     // copy functions for deep copy (no need of a 2nd version for NoCopy as noncopyable handles it)
     void do_copy (library_sm const& rhs,
              ::boost::msm::back::dummy<0> = 0)
     {
         // deep copy simply assigns the data
         region_copy_helper< ::boost::mpl::int_<0> >::do_copy(this,rhs);
         m_events_queue = rhs.m_events_queue;
         m_deferred_events_queue = rhs.m_deferred_events_queue;
         m_history = rhs.m_history;
         m_event_processing = rhs.m_event_processing;
         m_is_included = rhs.m_is_included;
         m_substate_list = rhs.m_substate_list;
         // except for the states themselves, which get duplicated

         ::boost::mpl::for_each<state_list, ::boost::msm::wrap< ::boost::mpl::placeholders::_1> >
                        (copy_helper(this));
     }

     // helper used to call the correct entry/exit method
     // unfortunately in O(number of states in the sub-sm) but should be better than a virtual call
     template<class Event,bool is_entry> 
     struct entry_exit_helper
     {
         entry_exit_helper(int id,Event const& e,library_sm* self_):
            state_id(id),evt(e),self(self_){}
         // helper for entry actions
         template <class IsEntry,class State>
         typename ::boost::enable_if<typename IsEntry::type,void >::type
         helper( ::boost::msm::back::dummy<0> = 0)
         {
             BOOST_STATIC_CONSTANT(int, id = (get_state_id<stt,State>::value));
             if (id == state_id)
             {
                 execute_entry<State>(::boost::fusion::at_key<State>(self->m_substate_list),evt,*self);
             }
         }
         // helper for exit actions
         template <class IsEntry,class State>
         typename boost::disable_if<typename IsEntry::type,void >::type
         helper( ::boost::msm::back::dummy<1> = 0)
         {
             BOOST_STATIC_CONSTANT(int, id = (get_state_id<stt,State>::value));
             if (id == state_id)
             {
                 execute_exit<State>(::boost::fusion::at_key<State>(self->m_substate_list),evt,*self);
             }
         }
         // iterates through all states to find the one to be activated
         template <class State>
         void operator()( ::boost::msm::wrap<State> const&)
         {
             entry_exit_helper<Event,is_entry>::template helper< ::boost::mpl::bool_<is_entry>,State >();
         }
     private:
         int            state_id;
         Event const&   evt;
         library_sm*    self;
     };

     // helper to start the fsm
     template <class region_id,int Dummy=0>
     struct region_start_helper
     {
         template<class Event>
         static void do_start(library_sm* self_,Event const& incomingEvent)
         {
             //forward the event for handling by sub state machines
             ::boost::mpl::for_each<state_list, ::boost::msm::wrap< ::boost::mpl::placeholders::_1> >
                 (entry_exit_helper<Event,true>(self_->m_states[region_id::value],incomingEvent,self_));
             region_start_helper
                 < ::boost::mpl::int_<region_id::value+1> >::do_start(self_,incomingEvent);
         }
     };
     template <int Dummy>
     struct region_start_helper< ::boost::mpl::int_<nr_regions::value>,Dummy>
     {
         // end of processing
         template<class Event>
         static void do_start(library_sm*,Event const& ){}
     };
     // start for states machines which are themselves embedded in other state machines (composites)
     template <class Event>
     void start(Event const& incomingEvent)
     {
         region_start_helper< ::boost::mpl::int_<0> >::do_start(this,incomingEvent);
     }

     // helper used to set the correct state as active state upon entry into a fsm
     struct direct_event_start_helper 
     {
         direct_event_start_helper(library_sm* self_):self(self_){}
         // this variant is for the standard case, entry due to activation of the containing FSM
         template <class EventType,class FsmType>
         typename ::boost::disable_if<typename has_direct_entry<EventType>::type,void>::type
             operator()(EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<0> = 0)
         {
             (static_cast<Derived*>(self))->on_entry(evt,fsm);
             self->start(evt);
         }

         // this variant is for the direct entry case (just one entry, not a sequence of entries)
         template <class EventType,class FsmType>
         typename ::boost::enable_if<
             typename ::boost::mpl::and_<
                        typename ::boost::mpl::not_< typename is_pseudo_entry<
                                    typename EventType::active_state>::type >::type,
                        typename ::boost::mpl::and_<typename has_direct_entry<EventType>::type,
                                                    typename ::boost::mpl::not_<typename ::boost::mpl::is_sequence
                                                            <typename EventType::active_state>::type >::type 
                                                    >::type>::type,void
                                  >::type
         operator()(EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<1> = 0)
         {
             (static_cast<Derived*>(self))->on_entry(evt,fsm);
             int state_id = get_state_id<stt,typename EventType::active_state::wrapped_entry>::value;
             BOOST_STATIC_ASSERT(EventType::active_state::zone_index >= 0);
             BOOST_STATIC_ASSERT(EventType::active_state::zone_index <= nr_regions::value);
             // just set the correct zone, the others will be default/history initialized
             self->m_states[EventType::active_state::zone_index] = state_id;
             self->start(evt.m_event);
         }

         // this variant is for the fork entry case (a sequence on entries)
         template <class EventType,class FsmType>
         typename ::boost::enable_if<
             typename ::boost::mpl::and_<
                    typename ::boost::mpl::not_<
                                    typename is_pseudo_entry<typename EventType::active_state>::type >::type,
                    typename ::boost::mpl::and_<typename has_direct_entry<EventType>::type,
                                                typename ::boost::mpl::is_sequence<
                                                                typename EventType::active_state>::type 
                                                >::type>::type,void 
                                >::type
         operator()(EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<2> = 0)
         {
             (static_cast<Derived*>(self))->on_entry(evt,fsm);
             ::boost::mpl::for_each<typename EventType::active_state, 
                                    ::boost::msm::wrap< ::boost::mpl::placeholders::_1> >
                                                        (fork_helper<EventType>(self,evt));
             // set the correct zones, the others (if any) will be default/history initialized
             self->start(evt.m_event);
         }

         // this variant is for the pseudo state entry case
         template <class EventType,class FsmType>
         typename ::boost::enable_if<
             typename is_pseudo_entry<typename EventType::active_state >::type,void
                                    >::type
         operator()(EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<3> = 0)
         {
             // entry on the FSM
             (static_cast<Derived*>(self))->on_entry(evt,fsm);
             int state_id = get_state_id<stt,typename EventType::active_state::wrapped_entry>::value;
             // given region starts with the entry pseudo state as active state
             self->m_states[EventType::active_state::zone_index] = state_id;
             self->start(evt.m_event);
             // and we process the transition in the zone of the newly active state
             // (entry pseudo states are, according to UML, a state connecting 1 transition outside to 1 inside
             self->process_event(evt.m_event);
         }
     private:
         // helper for the fork case, does almost like the direct entry
         library_sm* self;
         template <class EventType>
         struct fork_helper
         {
             fork_helper(library_sm* self_,EventType const& evt_):
                helper_self(self_),helper_evt(evt_){}
             template <class StateType>
             void operator()( ::boost::msm::wrap<StateType> const& )
             {
                 int state_id = get_state_id<stt,typename StateType::wrapped_entry>::value;
                 BOOST_STATIC_ASSERT(StateType::zone_index >= 0);
                 BOOST_STATIC_ASSERT(StateType::zone_index <= nr_regions::value);
                 helper_self->m_states[StateType::zone_index] = state_id;
             }
         private:
             library_sm*        helper_self;
             EventType const&   helper_evt;
         };
     };

     // helper for entry
     template <class region_id,int Dummy=0>
     struct region_entry_exit_helper
     {
         template<class Event>
         static void do_entry(library_sm* self_,Event const& incomingEvent)
         {
             self_->m_states[region_id::value] = 
                 self_->m_history.history_entry(incomingEvent)[region_id::value];
             region_entry_exit_helper
                 < ::boost::mpl::int_<region_id::value+1> >::do_entry(self_,incomingEvent);
         }
         template<class Event>
         static void do_exit(library_sm* self_,Event const& incomingEvent)
         {
             ::boost::mpl::for_each<state_list, ::boost::msm::wrap< ::boost::mpl::placeholders::_1> >
                 (entry_exit_helper<Event,false>(self_->m_states[region_id::value],incomingEvent,self_));
             region_entry_exit_helper
                 < ::boost::mpl::int_<region_id::value+1> >::do_exit(self_,incomingEvent);
         }
     };
     template <int Dummy>
     struct region_entry_exit_helper< ::boost::mpl::int_<nr_regions::value>,Dummy>
     {
         // end of processing
         template<class Event>
         static void do_entry(library_sm*,Event const& ){}
         template<class Event>
         static void do_exit(library_sm*,Event const& ){}
     };
     // entry/exit for states machines which are themselves embedded in other state machines (composites)
     template <class Event,class FsmType>
     void do_entry(Event const& incomingEvent,FsmType& fsm)
     {
        // by default we activate the history/init states, can be overwritten by direct_event_start_helper
        region_entry_exit_helper< ::boost::mpl::int_<0> >::do_entry(this,incomingEvent);
        // block immediate handling of events
        m_event_processing = true;
        // if the event is generating a direct entry/fork, set the current state(s) to the direct state(s)
        direct_event_start_helper(this)(incomingEvent,fsm);
        // handle messages which were generated and blocked in the init calls
        m_event_processing = false;
        process_message_queue(this);
     }
     template <class Event,class FsmType>
     void do_exit(Event const& incomingEvent,FsmType& fsm)
     {
        // first recursively exit the sub machines
        // forward the event for handling by sub state machines
        region_entry_exit_helper< ::boost::mpl::int_<0> >::do_exit(this,incomingEvent);
        // then call our own exit
        (static_cast<Derived*>(this))->on_exit(incomingEvent,fsm);
        // give the history a chance to handle this (or not).
        m_history.history_exit(this->m_states);
     }

    // the IBM and VC<8 compilers seem to have problems with the friend declaration of dispatch_table
#if defined (__IBMCPP__) || (defined(_MSC_VER) && (_MSC_VER < 1400))
     public:
#endif
    // no transition for event.
    template <class Event>
    static HandledEnum call_no_transition(library_sm& , int , int , Event const& )
    {
        return HANDLED_FALSE;
    }
    // called for deferred events. Address set in the dispatch_table at init
    template <class Event>
    static HandledEnum defer_transition(library_sm& fsm, int , int , Event const& e)
    {
        fsm.defer_event(e);
        return HANDLED_DEFERRED;
    }
    // called for completion events. Default address set in the dispatch_table at init
    // prevents no-transition detection for completion events
    template <class Event>
    static HandledEnum default_eventless_transition(library_sm& fsm, int, int , Event const& e)
    {
        return HANDLED_FALSE;
    }
#if defined (__IBMCPP__) || (defined(_MSC_VER) && (_MSC_VER < 1400))
     private:
#endif
    // puts a deferred event in the queue
    void post_deferred_event(deferred_fct& deferred)
    {
        m_deferred_events_queue.m_deferred_events_queue.push_front(deferred);
    }
    // removes one event from the message queue and processes it
    template <class StateType>
    typename ::boost::disable_if<typename is_no_message_queue<StateType>::type,void >::type
    process_message_queue(StateType*)
    {
        if (!m_events_queue.m_events_queue.empty())
        {
            transition_fct to_call = m_events_queue.m_events_queue.front();
            m_events_queue.m_events_queue.pop();
            to_call();
        }
    }
    template <class StateType>
    typename ::boost::enable_if<typename is_no_message_queue<StateType>::type,void >::type
        process_message_queue(StateType*)
    {
        // nothing to process
    }
    // calls the entry/exit or on_entry/on_exit depending on the state type
    // (avoids calling virtually)
    // variant for FSMs
    template <class StateType,class EventType,class FsmType>
    static
        typename boost::enable_if<typename is_composite_state<StateType>::type,void >::type
        execute_entry(StateType& astate,EventType const& evt,FsmType& fsm,boost::msm::back::dummy<0> = 0)
    {
        // calls on_entry on the fsm then handles direct entries, fork, entry pseudo state
        astate.do_entry(evt,fsm);
    }
    // variant for states
    template <class StateType,class EventType,class FsmType>
    static
        typename ::boost::disable_if<
            typename ::boost::mpl::or_<typename is_composite_state<StateType>::type,
                                       typename is_pseudo_exit<StateType>::type >::type,void >::type
    execute_entry(StateType& astate,EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<1> = 0)
    {
        // simple call to on_entry
        astate.on_entry(evt,fsm);
    }
    // variant for exit pseudo states
    template <class StateType,class EventType,class FsmType>
    static
        typename ::boost::enable_if<typename is_pseudo_exit<StateType>::type,void >::type
    execute_entry(StateType& astate,EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<2> = 0)
    {
        // calls on_entry on the state then forward the event to the transition which should be defined inside the 
        // contained fsm
        astate.on_entry(evt,fsm);
        astate.forward_event(evt);
    }
    template <class StateType,class EventType,class FsmType>
    static
        typename ::boost::enable_if<typename is_composite_state<StateType>::type,void >::type
    execute_exit(StateType& astate,EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<0> = 0)
    {
        astate.do_exit(evt,fsm);
    }
    template <class StateType,class EventType,class FsmType>
    static
        typename ::boost::disable_if<typename is_composite_state<StateType>::type,void >::type
    execute_exit(StateType& astate,EventType const& evt,FsmType& fsm, ::boost::msm::back::dummy<1> = 0)
    {
        // simple call to on_exit
        astate.on_exit(evt,fsm);
    }

    // helper allowing special handling of direct entries / fork
    template <class StateType,class TargetType,class EventType,class FsmType>
    static
        typename ::boost::disable_if<
            typename ::boost::mpl::or_<typename has_explicit_entry_state<TargetType>::type,
                                       ::boost::mpl::is_sequence<TargetType> >::type,void>::type
    convert_event_and_execute_entry(StateType& astate,EventType const& evt, FsmType& fsm, ::boost::msm::back::dummy<1> = 0)
    {
        // if the target is a normal state, do the standard entry handling
        execute_entry<StateType>(astate,evt,fsm);
    }
    template <class StateType,class TargetType,class EventType,class FsmType>
    static
        typename ::boost::enable_if<
            typename ::boost::mpl::or_<typename has_explicit_entry_state<TargetType>::type,
                                       ::boost::mpl::is_sequence<TargetType> >::type,void >::type
    convert_event_and_execute_entry(StateType& astate,EventType const& evt, FsmType& fsm, ::boost::msm::back::dummy<0> = 0)
    {
        // for the direct entry, pack the event in a wrapper so that we handle it differently during fsm entry
        execute_entry(astate,msm::back::direct_entry_event<TargetType,EventType>(evt),fsm);
    }

    // creates all the states
    template <class ContainingSM>
    void fill_states(ContainingSM* containing_sm=0)
    {
        BOOST_STATIC_CONSTANT(int, max_state = (mpl::size<state_list>::value));
        // allocate the place without reallocation
        m_visitors.fill_visitors(max_state);
        ::boost::fusion::for_each(m_substate_list,add_state<ContainingSM>(this,containing_sm));

    }

private:
    template <class StateType,class Enable=void>
    struct msg_queue_helper 
    {
    public:
        msg_queue_helper():m_events_queue(){}
        events_queue_t              m_events_queue;
    };
    template <class StateType>
    struct msg_queue_helper<StateType,
        typename ::boost::enable_if<typename is_no_message_queue<StateType>::type >::type> 
    {
    };

    template <class Fsm,class Stt, class Event, class Compile>
    friend struct dispatch_table;

    // data members
    int                             m_states[nr_regions::value];
    msg_queue_helper<library_sm>    m_events_queue;
    deferred_msg_queue_helper
        <library_sm>                m_deferred_events_queue;
    concrete_history                m_history;
    bool                            m_event_processing;
    bool                            m_is_included;
    visitor_fct_helper<BaseState>   m_visitors;
    substate_list                   m_substate_list;


};

} } }// boost::msm::back
#endif //BOOST_MSM_BACK_STATEMACHINE_H

