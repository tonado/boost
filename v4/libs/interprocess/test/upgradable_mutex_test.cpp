//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/detail/config_begin.hpp>
#include "mutex_test_template.hpp"
#include "sharable_mutex_test_template.hpp"
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "util.hpp"

int main ()
{
   using namespace boost::interprocess;

   test::test_all_lock<interprocess_upgradable_mutex>();
   test::test_all_mutex<true, interprocess_upgradable_mutex>();
   test::test_all_sharable_mutex<true, interprocess_upgradable_mutex>();

   //Test lock transition
   {
      typedef interprocess_upgradable_mutex Mutex;
      Mutex mut;
      Mutex mut2;

      //Conversions to scoped_lock
      {
         scoped_lock<Mutex>      lock(mut);
         scoped_lock<Mutex>      e_lock(detail::move_impl(lock));
         lock.swap(detail::move_impl(e_lock));
      }
      {
         scoped_lock<Mutex>      lock(mut);
         scoped_lock<Mutex>      e_lock(mut2);
         e_lock = detail::move_impl(lock);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(detail::move_impl(u_lock));
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(detail::move_impl(u_lock));
         e_lock = detail::move_impl(moved);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls try_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(detail::move_impl(u_lock), try_to_lock);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls try_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(detail::move_impl(u_lock), try_to_lock);
         e_lock = detail::move_impl(moved);
      }
      {
         boost::posix_time::ptime t = test::delay(100);
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls timed_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(detail::move_impl(u_lock), t);
      }
      {
         boost::posix_time::ptime t = test::delay(100);
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls timed_unlock_upgradable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(detail::move_impl(u_lock), t);
         e_lock = detail::move_impl(moved);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls try_unlock_sharable_and_lock()
         scoped_lock<Mutex>      e_lock(detail::move_impl(s_lock), try_to_lock);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls try_unlock_sharable_and_lock()
         scoped_lock<Mutex>      e_lock(mut2);
         scoped_lock<Mutex>      moved(detail::move_impl(s_lock), try_to_lock);
         e_lock = detail::move_impl(moved);
      }
      //Conversions to upgradable_lock
      {
         upgradable_lock<Mutex>  lock(mut);
         upgradable_lock<Mutex>  u_lock(detail::move_impl(lock));
         lock.swap(detail::move_impl(u_lock));
      }
      {
         upgradable_lock<Mutex>  lock(mut);
         upgradable_lock<Mutex>  u_lock(mut2);
         upgradable_lock<Mutex>  moved(detail::move_impl(lock));
         u_lock = detail::move_impl(moved);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls unlock_sharable_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(detail::move_impl(s_lock), try_to_lock);
      }
      {
         sharable_lock<Mutex>    s_lock(mut);
         //This calls unlock_sharable_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(mut2);
         upgradable_lock<Mutex>  moved(detail::move_impl(s_lock), try_to_lock);
         u_lock = detail::move_impl(moved);
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(detail::move_impl(e_lock));
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_upgradable()
         upgradable_lock<Mutex>  u_lock(mut2);
         upgradable_lock<Mutex>  moved(detail::move_impl(e_lock));
         u_lock = detail::move_impl(moved);
      }
      //Conversions to sharable_lock
      {
         sharable_lock<Mutex>    lock(mut);
         sharable_lock<Mutex>    s_lock(detail::move_impl(lock));
         lock.swap(detail::move_impl(s_lock));
      }
      {
         sharable_lock<Mutex>    lock(mut);
         sharable_lock<Mutex>    s_lock(mut2);
         sharable_lock<Mutex>    moved(detail::move_impl(lock));
         s_lock = detail::move_impl(moved);
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(detail::move_impl(u_lock));
      }
      {
         upgradable_lock<Mutex>  u_lock(mut);
         //This calls unlock_upgradable_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(mut2);
         sharable_lock<Mutex>    moved(detail::move_impl(u_lock));
         s_lock = detail::move_impl(moved);
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(detail::move_impl(e_lock));
      }
      {
         scoped_lock<Mutex>      e_lock(mut);
         //This calls unlock_and_lock_sharable()
         sharable_lock<Mutex>    s_lock(mut2);
         sharable_lock<Mutex>    moved(detail::move_impl(e_lock));
         s_lock = detail::move_impl(moved);
      }
   }

   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
