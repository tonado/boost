// Copyright (C) 2002-2003
// David Moore, William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#include <boost/thread/detail/config.hpp>

#include <boost/assert.hpp>
#include <boost/thread/read_write_mutex.hpp>
#include <boost/thread/xtime.hpp>

#ifdef BOOST_HAS_WINTHREADS
    #include <windows.h>
    #include <tchar.h>

    #if !((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))
        inline bool IsDebuggerPresent(void)
        {
            return false;
        }
    #endif
#endif

#if defined(BOOST_ASSERT)
    #define BOOST_ASSERT_ELSE(expr) if ((BOOST_ASSERT(expr)), true)
#else
    #define BOOST_ASSERT_ELSE(expr) if (true)
#endif

bool boost_error(char const* expr, char const* func, char const* file, long line)
{
    #if WINVER
        #ifndef ELEMENTS
        #define ELEMENTS(a) (sizeof(a)/sizeof(*(a)))
        #endif

        TCHAR message[200];
        _sntprintf(message,ELEMENTS(message),TEXT("Assertion failed (func=%s, file=%s, line=%d): %s"), func, file, line, expr);

        ::OutputDebugString(message);

        if(::IsDebuggerPresent())
            ::DebugBreak();
    #endif

    return false;
}

namespace boost {
namespace detail {
namespace thread {

inline bool valid_lock(int state)
{
    return (state >= 0) || (state == -1);
}

inline bool valid_read_lock(int state)
{
    return state > 0;
}

inline bool valid_read_lockable(int state)
{
    return state >= 0;
}

inline bool valid_write_lock(int state)
{
    return state == -1;
}

inline bool valid_write_lockable(int state)
{
    return state == 0;
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_read_lock()
{
    Mutex::scoped_lock l(m_prot);
    BOOST_ASSERT(valid_lock(m_state));

    // Wait until no exclusive lock is held.
    if (m_sp == read_write_scheduling_policy::reader_priority)
    {
        //If readers have priority, only wait if a 
        //writer actually has the lock
        while (m_state == -1)
        {
            ++m_num_waiting_readers;
            m_waiting_readers.wait(l);
            --m_num_waiting_readers;
        }
    }
    else BOOST_ASSERT_ELSE(m_sp == read_write_scheduling_policy::writer_priority || m_sp == read_write_scheduling_policy::alternating_many_reads || m_sp == read_write_scheduling_policy::alternating_single_read)
    {
        //Otherwise, wait if a) a writer has the lock, or 
        //b) a reader has the lock and there are waiting writers
        while ((m_state == -1) || (m_state > 0 && m_num_waiting_writers > 0))
        {
            ++m_num_waiting_readers;
            m_waiting_readers.wait(l);
            --m_num_waiting_readers;
        }
    }

    // Increase the reader count
    BOOST_ASSERT(valid_read_lockable(m_state));
    ++m_state;

    /*
    Set m_readers_next in the lock function rather than the 
    unlock function to prevent thread starvation that can happen,
    e.g., like this: if all writer threads demote themselves
    to reader threads before unlocking, they will unlock using 
    do_read_unlock() which will set m_readers_next = false;
    if there are enough writer threads, this will prevent any
    "true" reader threads from ever obtaining the lock.
    */

    m_readers_next = false;

    BOOST_ASSERT(valid_read_lock(m_state));
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_write_lock()
{
    typename Mutex::scoped_lock l(m_prot);
    BOOST_ASSERT(valid_lock(m_state));

    // Wait until no exclusive lock is held.
    while (m_state != 0)
    {
        ++m_num_waiting_writers;
        m_waiting_writers.wait(l);
        --m_num_waiting_writers;
    }

    BOOST_ASSERT(valid_write_lockable(m_state));
    m_state = -1;

    //See note in read_write_mutex_impl<>::do_read_lock() as to why 
    //m_readers_next should be set here

    m_readers_next = true;

    BOOST_ASSERT(valid_write_lock(m_state));
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_try_read_lock()
{
    Mutex::scoped_try_lock l(m_prot, blocking_mode::non_blocking);
    BOOST_ASSERT(valid_lock(m_state));

    if (!l.locked())
        return false;

    bool ret;
    if (m_state == -1)
    {
        // We are already locked exclusively.  A try_read_lock always returns
        //   immediately in this case
        ret =  false;
    }
    else if (m_num_waiting_writers > 0)
    {
        // There are also waiting writers.  Use scheduling policy.
        if (m_sp == read_write_scheduling_policy::reader_priority)
        {
            BOOST_ASSERT(valid_read_lockable(m_state));
            ++m_state;
            ret = true;
        }
        else if (m_sp == read_write_scheduling_policy::writer_priority)
        {
            // A writer is waiting - don't grant this try lock, and 
            //   return immediately (don't increase waiting_readers count)
            ret = false;
        }
        else BOOST_ASSERT_ELSE(m_sp == read_write_scheduling_policy::alternating_many_reads || m_sp == read_write_scheduling_policy::alternating_single_read)
        {
            // For alternating scheduling priority, 
            // I don't think that try_ locks should step in front of others
            //   who have already indicated that they are waiting.
            // It seems that this could "game" the system and defeat
            //   the alternating mechanism.
            ret = false;
        }
    }
    else BOOST_ASSERT_ELSE(m_state >= 0 && m_num_waiting_writers == 0)
    {
        // No waiting writers.  Grant (additonal) read lock regardless of
        //   scheduling policy.
        BOOST_ASSERT(valid_read_lockable(m_state));
        ++m_state;
        ret = true;
    }

    if (ret)
    {
        //See note in read_write_mutex_impl<>::do_read_lock() as to why 
        //m_readers_next should be set here

        m_readers_next = false;
    }

    BOOST_ASSERT(valid_read_lock(m_state));
    return ret;
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_try_write_lock()
{
    typename Mutex::scoped_try_lock l(m_prot, blocking_mode::non_blocking);
    BOOST_ASSERT(valid_lock(m_state));

    if (!l.locked())
        return false;

    bool ret;
    if (m_state != 0)
    {
        // We are already busy and locked.
        // Scheduling priority doesn't matter here.
        ret = false;
    }
    else //(m_state == 0)
    {
        BOOST_ASSERT(valid_write_lockable(m_state));
        m_state = -1;
        ret = true;
    }

    if (ret)
    {
        //See note in read_write_mutex_impl<>::do_read_lock() as to why 
        //m_readers_next should be set here

        m_readers_next = true;
    }

    BOOST_ASSERT(valid_write_lock(m_state));
    return ret;
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_timed_read_lock(const boost::xtime &xt)
{
    typename Mutex::scoped_timed_lock l(m_prot,xt);
    BOOST_ASSERT(valid_lock(m_state));

    if (!l.locked())
        return false;

    // Wait until no exclusive lock is held.
    if (m_sp == read_write_scheduling_policy::reader_priority)
    {
        //If readers have priority, only wait if a 
        //writer actually has the lock
        while (m_state == -1)
        {
            ++m_num_waiting_readers;
            if (!m_waiting_readers.timed_wait(l,xt))
            {
                --m_num_waiting_readers;
                return false;
            }
            --m_num_waiting_readers;
        }
    }
    else BOOST_ASSERT_ELSE(m_sp == read_write_scheduling_policy::writer_priority || m_sp == read_write_scheduling_policy::alternating_many_reads || m_sp == read_write_scheduling_policy::alternating_single_read)
    {
        //Otherwise, wait if a) a writer has the lock, or 
        //b) a reader has the lock and there are waiting writers
        while ((m_state == -1) || (m_state > 0 && m_num_waiting_writers > 0))
        {
            ++m_num_waiting_readers;
            if (!m_waiting_readers.timed_wait(l,xt))
            {
                --m_num_waiting_readers;
                return false;
            }
            --m_num_waiting_readers;
        }
    }

    // Increase the reader count
    BOOST_ASSERT(valid_read_lockable(m_state));
    ++m_state;

    //See note in read_write_mutex_impl<>::do_read_lock() as to why 
    //m_readers_next should be set here

    m_readers_next = false;

    BOOST_ASSERT(valid_read_lock(m_state));
    return true;
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_timed_write_lock(const boost::xtime &xt)
{
    typename Mutex::scoped_timed_lock l(m_prot,xt);
    BOOST_ASSERT(valid_lock(m_state));

    if (!l.locked())
        return false;

    // Wait until no exclusive lock is held.
    while (m_state != 0)
    {
        ++m_num_waiting_writers;
        if (!m_waiting_writers.timed_wait(l,xt))
        {
            --m_num_waiting_writers;
            return false;
        }
        --m_num_waiting_writers;
    }

    BOOST_ASSERT(valid_write_lockable(m_state));
    m_state = -1;

    //See note in read_write_mutex_impl<>::do_read_lock() as to why 
    //m_readers_next should be set here

    m_readers_next = true;

    BOOST_ASSERT(valid_write_lock(m_state));
    return true;
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_read_unlock()
{
    typename Mutex::scoped_lock l(m_prot);
    BOOST_ASSERT(valid_read_lock(m_state));

    if (m_state > 0)
        --m_state;
    else //not read-locked
        throw lock_error();

    if (m_state_waiting_promotion && m_state == 1)
        m_waiting_promotion.notify_one();
    else if (m_state == 0)
        do_unlock_scheduling_impl();

    BOOST_ASSERT(valid_lock(m_state));
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_write_unlock()
{
    typename Mutex::scoped_lock l(m_prot);
    BOOST_ASSERT(valid_write_lock(m_state));

    if (m_state == -1)
        m_state = 0;
    else BOOST_ASSERT_ELSE(m_state >= 0)
        throw lock_error();      // Trying to release a reader-locked or unlocked mutex???

    if (m_state_waiting_promotion)
        m_waiting_promotion.notify_one();
    else
        do_unlock_scheduling_impl();

    BOOST_ASSERT(valid_lock(m_state));
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_demote_to_read_lock_impl()
{
    BOOST_ASSERT(valid_write_lock(m_state));

    //:if (!m_prot.locked())
    //:    throw lock_error();

    if (m_state == -1) 
    {
        //Convert from write lock to read lock
        m_state = 1;

        //If the conditions are right, release other readers
        if (m_num_waiting_readers > 0)
        {
            if (m_num_waiting_writers == 0 || m_sp == read_write_scheduling_policy::reader_priority || (m_sp == read_write_scheduling_policy::alternating_many_reads && m_readers_next))
                m_waiting_readers.notify_all();
        }

        //Lock demoted
        BOOST_ASSERT(valid_read_lock(m_state));
        return true;
    }
    else BOOST_ASSERT_ELSE(m_state >= 0)
    {
        //Lock is read-locked or unlocked can't be demoted
        throw lock_error();
        return false;
    }
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_demote_to_read_lock()
{
    typename Mutex::scoped_lock l(m_prot);
    BOOST_ASSERT(valid_write_lock(m_state));

    do_demote_to_read_lock_impl();
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_try_demote_to_read_lock()
{
    typename Mutex::scoped_try_lock l(m_prot, blocking_mode::non_blocking);
    BOOST_ASSERT(valid_write_lock(m_state));

    if (!l.locked())
        return false;
    else //(l.locked())
        return do_demote_to_read_lock_impl();
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_timed_demote_to_read_lock(const boost::xtime &xt)
{
    typename Mutex::scoped_timed_lock l(m_prot,xt);
    BOOST_ASSERT(valid_write_lock(m_state));

    if (!l.locked())
        return false;
    else //(l.locked())
        return do_demote_to_read_lock_impl();
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_promote_to_write_lock()
{
    typename Mutex::scoped_lock l(m_prot);
    BOOST_ASSERT(valid_read_lock(m_state));

    if (m_state == 1)
    {
        //Convert from read lock to write lock
        m_state = -1;

        //Lock promoted
        BOOST_ASSERT(valid_write_lock(m_state));
    }
    else if (m_state <= 0)
    {
        //Lock is write-locked or unlocked can't be promoted
        throw lock_error();
    }
    else if (m_state_waiting_promotion)
    {
        //Someone else is already trying to promote. Avoid deadlock by throwing exception.
        throw lock_error();
    }
    else BOOST_ASSERT_ELSE(m_state > 1 && !m_state_waiting_promotion)
    {
        ++m_num_waiting_writers;
        m_state_waiting_promotion = true;
        while (m_state > 1)
            m_waiting_promotion.wait(l);
        m_state_waiting_promotion = false;
        --m_num_waiting_writers;
        
        BOOST_ASSERT(m_num_waiting_writers >= 0);
        BOOST_ASSERT(m_state == 1);

        //Convert from read lock to write lock
        m_state = -1;
        
        //Lock promoted
        BOOST_ASSERT(valid_write_lock(m_state));
    }
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_try_promote_to_write_lock()
{
    typename Mutex::scoped_try_lock l(m_prot, blocking_mode::non_blocking);
    BOOST_ASSERT(valid_read_lock(m_state));

    if (!l.locked())
        return false;
    else
    {
        if (m_state == 1)
        {
            //Convert from read lock to write lock
            m_state = -1;

            //Lock promoted
            BOOST_ASSERT(valid_write_lock(m_state));
            return true;
        }
        else if (m_state <= 0)
        {
            //Lock is write-locked or unlocked can't be promoted
            throw lock_error();
        }
        else if (m_state_waiting_promotion)
        {
            //Someone else is already trying to promote. Avoid deadlock by returning false.
            return false;
        }
        else BOOST_ASSERT_ELSE(m_state > 1 && !m_state_waiting_promotion)
        {
            //There are other readers, so we can't promote
            return false;
        }
    }
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::do_timed_promote_to_write_lock(const boost::xtime &xt)
{
    typename Mutex::scoped_timed_lock l(m_prot,xt);
    BOOST_ASSERT(valid_read_lock(m_state));

    if (!l.locked())
        return false;
    else
    {
        if (m_state == 1)
        {
            //Convert from read lock to write lock
            m_state = -1;
            
            //Lock promoted
            BOOST_ASSERT(valid_write_lock(m_state));
            return true;
        }
        else if (m_state <= 0)
        {
            //Lock is not read-locked and can't be promoted
            throw lock_error();
        }
        else if (m_state_waiting_promotion)
        {
            //Someone else is already trying to promote. Avoid deadlock by returning false.
            return false;
        }
        else BOOST_ASSERT_ELSE(m_state > 1 && !m_state_waiting_promotion)
        {   
            ++m_num_waiting_writers;
            m_state_waiting_promotion = true;
            while (m_state > 1)
            {
                if (!m_waiting_promotion.timed_wait(l, xt))
                {
                    m_state_waiting_promotion = false;
                    --m_num_waiting_writers;
                    return false;
                }
            }
            m_state_waiting_promotion = false;
            --m_num_waiting_writers;
            
            BOOST_ASSERT(m_num_waiting_writers >= 0);
            BOOST_ASSERT(m_state == 1);

            //Convert from read lock to write lock
            m_state = -1;
            
            //Lock promoted
            BOOST_ASSERT(valid_write_lock(m_state));
            return true;
        }
    }
}

template<typename Mutex>
bool read_write_mutex_impl<Mutex>::locked()
{
    int state = m_state;
    BOOST_ASSERT(valid_lock(state));

    return state != 0;
}

template<typename Mutex>
read_write_lock_state::read_write_lock_state read_write_mutex_impl<Mutex>::state()
{
    int state = m_state;
    BOOST_ASSERT(valid_lock(state));

    if (state > 0)
    {
        BOOST_ASSERT(valid_read_lock(state));
        return read_write_lock_state::read_locked;
    }
    else if (state == -1)
    {
        BOOST_ASSERT(valid_write_lock(state));
        return read_write_lock_state::write_locked;
    }
    else BOOST_ASSERT_ELSE(state == 0)
        return read_write_lock_state::unlocked;
}

template<typename Mutex>
void read_write_mutex_impl<Mutex>::do_unlock_scheduling_impl()
{
    //:if (!m_prot.locked())
    //:    throw lock_error();
    
    if (m_state != 0)
        throw lock_error();

    if (m_num_waiting_writers > 0 && m_num_waiting_readers > 0)
    {
        // We have both types waiting, and -either- could proceed.
        //    Choose which to release based on scheduling policy.
        if (m_sp == read_write_scheduling_policy::reader_priority)
        {
            m_waiting_readers.notify_all();
        }
        else if (m_sp == read_write_scheduling_policy::writer_priority)
        {
            m_waiting_writers.notify_one();
        }
        else BOOST_ASSERT_ELSE(m_sp == read_write_scheduling_policy::alternating_many_reads || m_sp == read_write_scheduling_policy::alternating_single_read)
        {
            if (m_readers_next)
            {
                if (m_sp == read_write_scheduling_policy::alternating_many_reads)
                    m_waiting_readers.notify_all();
                else BOOST_ASSERT_ELSE(m_sp == read_write_scheduling_policy::alternating_single_read)
                    m_waiting_readers.notify_one();
            }
            else //(!m_readers_next)
                m_waiting_writers.notify_one();
        }
    }
    else if (m_num_waiting_writers > 0)
    {
        // Only writers - scheduling doesn't matter
        m_waiting_writers.notify_one();
    }
    else if (m_num_waiting_readers > 0)
    {
        // Only readers - scheduling doesn't matter
        m_waiting_readers.notify_all();
    }
}

    }   // namespace thread
    }   // namespace detail


void read_write_mutex::do_read_lock()
{
    m_impl.do_read_lock();
}

void read_write_mutex::do_write_lock()
{
    m_impl.do_write_lock();
}

void read_write_mutex::do_read_unlock()
{
    m_impl.do_read_unlock();
}

void read_write_mutex::do_write_unlock()
{
    m_impl.do_write_unlock();
}

void read_write_mutex::do_demote_to_read_lock()
{
    m_impl.do_demote_to_read_lock();
}

void read_write_mutex::do_promote_to_write_lock()
{
    m_impl.do_promote_to_write_lock();
}

bool read_write_mutex::locked()
{
    return m_impl.locked();
}

read_write_lock_state::read_write_lock_state read_write_mutex::state()
{
    return m_impl.state();
}

void try_read_write_mutex::do_read_lock()
{
    m_impl.do_read_lock();
}

void try_read_write_mutex::do_write_lock()
{
    m_impl.do_write_lock();

}

void try_read_write_mutex::do_write_unlock()
{
    m_impl.do_write_unlock();
}

void try_read_write_mutex::do_read_unlock()
{
    m_impl.do_read_unlock();
}

bool try_read_write_mutex::do_try_read_lock()
{
    return m_impl.do_try_read_lock();
}

bool try_read_write_mutex::do_try_write_lock()
{
    return m_impl.do_try_write_lock();
}

void try_read_write_mutex::do_demote_to_read_lock()
{
    m_impl.do_demote_to_read_lock();
}

bool try_read_write_mutex::do_try_demote_to_read_lock()
{
    return m_impl.do_try_demote_to_read_lock();
}

void try_read_write_mutex::do_promote_to_write_lock()
{
    m_impl.do_promote_to_write_lock();
}

bool try_read_write_mutex::do_try_promote_to_write_lock()
{
    return m_impl.do_try_promote_to_write_lock();
}

bool try_read_write_mutex::locked()
{
    return m_impl.locked();
}

read_write_lock_state::read_write_lock_state try_read_write_mutex::state()
{
    return m_impl.state();
}

void timed_read_write_mutex::do_read_lock()
{
    m_impl.do_read_lock();
}

void timed_read_write_mutex::do_write_lock()
{
    m_impl.do_write_lock();

}

void timed_read_write_mutex::do_read_unlock()
{
    m_impl.do_read_unlock();
}

void timed_read_write_mutex::do_write_unlock()
{
    m_impl.do_write_unlock();
}

bool timed_read_write_mutex::do_try_read_lock()
{
    return m_impl.do_try_read_lock();
}

bool timed_read_write_mutex::do_try_write_lock()
{
    return m_impl.do_try_write_lock();
}

bool timed_read_write_mutex::do_timed_read_lock(const xtime &xt)
{
    return m_impl.do_timed_read_lock(xt);
}

bool timed_read_write_mutex::do_timed_write_lock(const xtime &xt)
{
    return m_impl.do_timed_write_lock(xt);
}

void timed_read_write_mutex::do_demote_to_read_lock()
{
    m_impl.do_demote_to_read_lock();
}

bool timed_read_write_mutex::do_try_demote_to_read_lock()
{
    return m_impl.do_try_demote_to_read_lock();
}

bool timed_read_write_mutex::do_timed_demote_to_read_lock(const xtime &xt)
{
    return m_impl.do_timed_demote_to_read_lock(xt);
}

void timed_read_write_mutex::do_promote_to_write_lock()
{
    m_impl.do_promote_to_write_lock();
}

bool timed_read_write_mutex::do_try_promote_to_write_lock()
{
    return m_impl.do_try_promote_to_write_lock();
}

bool timed_read_write_mutex::do_timed_promote_to_write_lock(const xtime &xt)
{
    return m_impl.do_timed_promote_to_write_lock(xt);
}

bool timed_read_write_mutex::locked()
{
    return m_impl.locked();
}

read_write_lock_state::read_write_lock_state timed_read_write_mutex::state()
{
    return m_impl.state();
}

//Explicit instantiations of read/write locks to catch syntax errors in templates

template class boost::detail::thread::scoped_read_write_lock<read_write_mutex>;
template class boost::detail::thread::scoped_read_write_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_read_write_lock<timed_read_write_mutex>;

//template class boost::detail::thread::scoped_try_read_write_lock<read_write_mutex>;
template class boost::detail::thread::scoped_try_read_write_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_try_read_write_lock<timed_read_write_mutex>;

//template class boost::detail::thread::scoped_timed_read_write_lock<read_write_mutex>;
//template class boost::detail::thread::scoped_timed_read_write_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_timed_read_write_lock<timed_read_write_mutex>;

//Explicit instantiations of read locks to catch syntax errors in templates

template class boost::detail::thread::scoped_read_lock<read_write_mutex>;
template class boost::detail::thread::scoped_read_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_read_lock<timed_read_write_mutex>;

//template class boost::detail::thread::scoped_try_read_lock<read_write_mutex>;
template class boost::detail::thread::scoped_try_read_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_try_read_lock<timed_read_write_mutex>;

//template class boost::detail::thread::scoped_timed_read_lock<read_write_mutex>;
//template class boost::detail::thread::scoped_timed_read_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_timed_read_lock<timed_read_write_mutex>;

//Explicit instantiations of write locks to catch syntax errors in templates

template class boost::detail::thread::scoped_write_lock<read_write_mutex>;
template class boost::detail::thread::scoped_write_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_write_lock<timed_read_write_mutex>;

//template class boost::detail::thread::scoped_try_write_lock<read_write_mutex>;
template class boost::detail::thread::scoped_try_write_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_try_write_lock<timed_read_write_mutex>;

//template class boost::detail::thread::scoped_timed_write_lock<read_write_mutex>;
//template class boost::detail::thread::scoped_timed_write_lock<try_read_write_mutex>;
template class boost::detail::thread::scoped_timed_read_write_lock<timed_read_write_mutex>;
} // namespace boost

// Change Log:
//  10 Mar 02 
//      Original version.
//   4 May 04 GlassfordM
//      For additional changes, see read_write_mutex.hpp.
//      Add many assertions to test validity of mutex state and operations.
//      Rework scheduling algorithm due to addition of lock promotion and 
//         demotion.
//      Add explicit template instantiations to catch syntax errors 
//         in templates.
