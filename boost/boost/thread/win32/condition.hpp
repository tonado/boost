//  (C) Copyright 2005-6 Anthony Williams
// Copyright 2006 Roland Schwarz.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_THREAD_RS06041001_HPP
#define BOOST_THREAD_RS06041001_HPP

#include <boost/thread/win32/config.hpp>

#include <boost/detail/interlocked.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/win32/thread_primitives.hpp>
#include <boost/thread/win32/xtime_utils.hpp>
#include <boost/thread/win32/interlocked_read.hpp>
#include <boost/assert.hpp>
#include <boost/utility.hpp>

namespace boost
{
    template<typename lockable_type>
    class basic_condition:
        noncopyable
    {
    private:
        struct waiting_list_entry
        {
            void* waiting_thread_handle;
            waiting_list_entry* next;
            waiting_list_entry* previous;
            long notified;

            void unlink()
            {
                next->previous=previous;
                previous->next=next;
                next=this;
                previous=this;
            }
        };

        typedef ::boost::mutex gate_type;
        gate_type state_change_gate;
        typedef gate_type::scoped_lock gate_scoped_lock;
        waiting_list_entry waiting_list;

        struct add_entry_to_list
        {
            basic_condition* self;
            waiting_list_entry& entry;
            lockable_type& m;

            add_entry_to_list(basic_condition* self_,waiting_list_entry& entry_,lockable_type& m_):
                self(self_),entry(entry_),m(m_)
            {
                entry.previous=&self->waiting_list;
                gate_scoped_lock lock(self->state_change_gate);
                    
                entry.next=self->waiting_list.next;
                self->waiting_list.next=&entry;
                entry.next->previous=&entry;
                    
                m.unlock();
            }
            ~add_entry_to_list()
            {
                if(!entry.notified)
                {
                    gate_scoped_lock lock(self->state_change_gate);
                        
                    if(!entry.notified)
                    {
                        entry.unlink();
                    }
                }
                detail::win32::CloseHandle(entry.waiting_thread_handle);
                m.lock();
            }
        };
        

        bool do_wait(lockable_type& m,boost::xtime const& target=::boost::detail::get_xtime_sentinel())
        {
            waiting_list_entry entry={0};
            void* const currentProcess=detail::win32::GetCurrentProcess();
            
            long const same_access_flag=2;
            bool const success=detail::win32::DuplicateHandle(currentProcess,detail::win32::GetCurrentThread(),currentProcess,&entry.waiting_thread_handle,0,false,same_access_flag)!=0;
            BOOST_ASSERT(success);
            
            {
                add_entry_to_list list_guard(this,entry,m);

                unsigned const woken_due_to_apc=0xc0;
                while(!::boost::detail::interlocked_read(&entry.notified) && 
                      detail::win32::SleepEx(::boost::detail::get_milliseconds_until_time(target),true)==woken_due_to_apc);
            }
            
            return ::boost::detail::interlocked_read(&entry.notified)!=0;
        }

        static void __stdcall notify_function(detail::win32::ulong_ptr)
        {
        }

        void notify_entry(waiting_list_entry * entry)
        {
            BOOST_INTERLOCKED_EXCHANGE(&entry->notified,true);
            if(entry->waiting_thread_handle)
            {
                detail::win32::QueueUserAPC(notify_function,entry->waiting_thread_handle,0);
            }
        }

    public:
        basic_condition()
        {
            waiting_list.next=&waiting_list;
            waiting_list.previous=&waiting_list;
        }

        void notify_one()
        {
            gate_scoped_lock lock(state_change_gate);
            if(waiting_list.previous!=&waiting_list)
            {
                waiting_list_entry* const entry=waiting_list.previous;
                entry->unlink();
                notify_entry(entry);
            }
        }
        
        void notify_all()
        {
            gate_scoped_lock lock(state_change_gate);
            waiting_list_entry* head=waiting_list.previous;
            waiting_list.previous=&waiting_list;
            waiting_list.next=&waiting_list;
            while(head!=&waiting_list)
            {
                waiting_list_entry* const previous=head->previous;
                notify_entry(head);
                head=previous;
            }
        }

        void wait(lockable_type& m)
        {
            do_wait(m);
        }

        template<typename predicate_type>
        void wait(lockable_type& m,predicate_type pred)
        {
            while(!pred()) do_wait(m);
        }

        bool timed_wait(lockable_type& m,const xtime& xt)
        {
            return do_wait(m,xt);
        }

        template<typename predicate_type>
        bool timed_wait(lockable_type& m,const xtime& xt,predicate_type pred)
        {
            while (!pred()) 
            {
                if (!timed_wait(m, xt)) return false;
            }
            return true;
        }
    };

    class condition:
        public basic_condition<boost::mutex::scoped_lock>
    {};
}

#endif // BOOST_THREAD_RS06041001_HPP
