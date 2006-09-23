// Copyright (C) 2001-2003 William E. Kempf
// Copyright (C) Michael Glassford 2004.
// Copyright (C) 2006 Roland Schwarz
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This work is a reimplementation along the design and ideas
// of William E. Kempf.

#ifndef BOOST_TLS_HOOKS_RS06092306_HPP
#define BOOST_TLS_HOOKS_RS06092306_HPP

#include <boost/thread/win32/config.hpp>

    typedef void (__cdecl *thread_exit_handler)(void);

    extern "C" BOOST_THREAD_DECL int at_thread_exit(
        thread_exit_handler exit_handler
        );
        //Add a function to the list of functions that will
            //be called when a thread is about to exit.
        //Currently only implemented for Win32, but should
            //later be implemented for all platforms.
        //Used by Win32 implementation of Boost.Threads
            //tss to perform cleanup.
        //Like the C runtime library atexit() function,
            //which it mimics, at_thread_exit() returns
            //zero if successful and a nonzero
            //value if an error occurs.

    extern "C" BOOST_THREAD_DECL void on_process_enter(void);
        //Function to be called when the exe or dll
            //that uses Boost.Threads first starts
            //or is first loaded.
        //Should be called only before the first call to
            //on_thread_enter().
        //Called automatically by Boost.Threads when
            //a method for doing so has been discovered.
        //May be omitted; may be called multiple times.

    extern "C" BOOST_THREAD_DECL void on_process_exit(void);
        //Function to be called when the exe or dll
            //that uses Boost.Threads first starts
            //or is first loaded.
        //Should be called only after the last call to
            //on_exit_thread().
        //Called automatically by Boost.Threads when
            //a method for doing so has been discovered.
        //Must not be omitted; may be called multiple times.

    extern "C" BOOST_THREAD_DECL void on_thread_enter(void);
        //Function to be called just after a thread starts
            //in an exe or dll that uses Boost.Threads.
        //Must be called in the context of the thread
            //that is starting.
        //Called automatically by Boost.Threads when
            //a method for doing so has been discovered.
        //May be omitted; may be called multiple times.

    extern "C" BOOST_THREAD_DECL void on_thread_exit(void);
        //Function to be called just be fore a thread ends
            //in an exe or dll that uses Boost.Threads.
        //Must be called in the context of the thread
            //that is ending.
        //Called automatically by Boost.Threads when
            //a method for doing so has been discovered.
        //Must not be omitted; may be called multiple times.

    extern "C" void tss_cleanup_implemented(void);
        //Dummy function used both to detect whether tss cleanup
            //cleanup has been implemented and to force
            //it to be linked into the Boost.Threads library.

#endif //BOOST_TLS_HOOKS_RS06092306_HPP
