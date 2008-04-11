//  (C) Copyright John Maddock 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for most recent version.

//  MACRO:         BOOST_HAS_PTHREADS
//  TITLE:         POSIX Threads
//  DESCRIPTION:   The platform supports POSIX style threads.

#include <pthread.h>


namespace boost_has_pthreads{

void* thread_proc(void* arg)
{
   return arg;
}

int test()
{
   pthread_mutex_t mut;
   int result = pthread_mutex_init(&mut, 0);
   if(0 == result)
   {
      result |= pthread_mutex_lock(&mut);
      result |= pthread_mutex_unlock(&mut);
      result |= pthread_mutex_trylock(&mut);
      result |= pthread_mutex_unlock(&mut);
      result |= pthread_mutex_destroy(&mut);
      //
      // Check that we can actually create a thread:
      //
      pthread_t t;
      int r = pthread_create(&t, 0, &thread_proc, 0);
      result |= r;
      if(r == 0)
      {
         void* arg;
         r = pthread_join(t, &arg);
         result |= r;
      }
   }
   return result;
}

}





