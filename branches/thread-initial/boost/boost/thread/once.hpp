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

#ifndef BOOST_ONCE_WEK080101_HPP
#define BOOST_ONCE_WEK080101_HPP

#include <boost/thread/config.hpp>
#ifndef BOOST_HAS_THREADS
#   error	Thread support is unavailable!
#endif

#if defined(BOOST_HAS_PTHREADS)
#   include <pthread.h>
#endif

namespace boost {

#if defined(BOOST_HAS_PTHREADS)
typedef pthread_once_t once_flag;
#define BOOST_ONCE_INIT PTHREAD_ONCE_INIT
#elif defined(BOOST_HAS_WINTHREADS)
typedef bool once_flag;
#define BOOST_ONCE_INIT false
#endif

void call_once(void (*func)(), once_flag& flag);

} // namespace boost

#endif // BOOST_ONCE_WEK080101_HPP