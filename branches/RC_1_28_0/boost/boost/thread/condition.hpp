// Copyright (C) 2001
// William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifndef BOOST_CONDITION_WEK070601_HPP
#define BOOST_CONDITION_WEK070601_HPP

#include <boost/config.hpp>
#ifndef BOOST_HAS_THREADS
#   error   Thread support is unavailable!
#endif

#include <boost/thread/exceptions.hpp>
#include <boost/utility.hpp>
#include <boost/thread/detail/lock.hpp>

#if defined(BOOST_HAS_PTHREADS)
#   include <pthread.h>
#elif defined(BOOST_HAS_MPTASKS)
#   include "scoped_critical_region.hpp"
#endif

namespace boost {

struct xtime;

class condition : private noncopyable
{
public:
    condition();
    ~condition();

    void notify_one();
    void notify_all();

    template <typename L>
    void wait(L& lock)
    {
        if (!lock)
            throw lock_error();

        do_wait(lock.m_mutex);
    }

    template <typename L, typename Pr>
    void wait(L& lock, Pr pred)
    {
        if (!lock)
            throw lock_error();

        while (!pred())
            do_wait(lock.m_mutex);
    }

    template <typename L>
    bool timed_wait(L& lock, const xtime& xt)
    {
        if (!lock)
            throw lock_error();

        return do_timed_wait(lock.m_mutex, xt);
    }

    template <typename L, typename Pr>
    bool timed_wait(L& lock, const xtime& xt, Pr pred)
    {
        if (!lock)
            throw lock_error();

        while (!pred())
        {
            if (!do_timed_wait(lock.m_mutex, xt))
                return false;
        }

        return true;
    }

private:
    template <typename M>
    void do_wait(M& mutex)
    {
#if (defined(BOOST_HAS_WINTHREADS) || defined(BOOST_HAS_MPTASKS))
        enter_wait();
#endif

        typedef typename detail::thread::lock_ops<M> lock_ops;
        typename lock_ops::lock_state state;
        lock_ops::unlock(mutex, state);

#if defined(BOOST_HAS_PTHREADS)
        do_wait(state.pmutex);
#elif (defined(BOOST_HAS_WINTHREADS) || defined(BOOST_HAS_MPTASKS))
        do_wait();
#endif

        lock_ops::lock(mutex, state);
    }

    template <typename M>
    bool do_timed_wait(M& mutex, const xtime& xt)
    {
#if (defined(BOOST_HAS_WINTHREADS) || defined(BOOST_HAS_MPTASKS))
        enter_wait();
#endif

        typedef typename detail::thread::lock_ops<M> lock_ops;
        typename lock_ops::lock_state state;
        lock_ops::unlock(mutex, state);

        bool ret = false;

#if defined(BOOST_HAS_PTHREADS)
        ret = do_timed_wait(xt, state.pmutex);
#elif (defined(BOOST_HAS_WINTHREADS) || defined(BOOST_HAS_MPTASKS))
        ret = do_timed_wait(xt);
#endif

        lock_ops::lock(mutex, state);

        return ret;
    }

#if (defined(BOOST_HAS_WINTHREADS) || defined(BOOST_HAS_MPTASKS))
    void enter_wait();
    void do_wait();
    bool do_timed_wait(const xtime& xt);
#elif defined(BOOST_HAS_PTHREADS)
    void do_wait(pthread_mutex_t* pmutex);
    bool do_timed_wait(const xtime& xt, pthread_mutex_t* pmutex);
#endif

#if defined(BOOST_HAS_WINTHREADS)
    void* m_gate;
    void* m_queue;
    void* m_mutex;
    unsigned m_gone; // # threads that timed out and never made it to the m_queue
    unsigned long m_blocked; // # threads m_blocked m_waiting for the condition
    unsigned m_waiting; // # threads m_waiting no longer m_waiting for the condition but still
                      //   m_waiting to be removed from the m_queue
#elif defined(BOOST_HAS_PTHREADS)
    pthread_cond_t m_condition;
#elif defined(BOOST_HAS_MPTASKS)
    MPSemaphoreID m_gate;
    MPSemaphoreID m_queue;
    threads::mac::detail::scoped_critical_region m_mutex;
    threads::mac::detail::scoped_critical_region m_mutex_mutex;
    unsigned m_gone; // # threads that timed out and never made it to the m_queue
    unsigned long m_blocked; // # threads m_blocked m_waiting for the condition
    unsigned m_waiting; // # threads m_waiting no longer m_waiting for the condition but still
                      //   m_waiting to be removed from the m_queue
#endif
};

} // namespace boost

// Change Log:
//    8 Feb 01  WEKEMPF Initial version.
//   22 May 01  WEKEMPF Modified to use xtime for time outs.
//   23 May 01  WEKEMPF Removed "duration" timed_waits, as they are too difficult
//                      to use with spurious wakeups.

#endif // BOOST_CONDITION_WEK070601_HPP
