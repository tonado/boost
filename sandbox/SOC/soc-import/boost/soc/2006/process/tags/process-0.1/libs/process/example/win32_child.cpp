//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
//

//
// This example program demonstrates how to start a process in a Win32
// operating system and later retrieve handles and identifiers to both
// the process and its primary thread.
//

// Avoid building this example under non-Win32 systems.
#include <boost/process/config.hpp>
#if defined(BOOST_PROCESS_WIN32_API)

// quickbook:begin(code)
extern "C" {
#include <windows.h>
}

#include <cstdlib>
#include <iostream>

#include <boost/process.hpp>

//
// Error out early if we are trying to build this non-portable example
// code under a platform that does not provide the required win32_*
// classes.
//
#if !defined(BOOST_PROCESS_WIN32_API)
#   error "Unsupported platform."
#endif

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    int exitstatus = EXIT_FAILURE;

    try {
        //
        // Constructs a command line to launch Notepad, looking for its
        // availability in the PATH.
        //
        bp::command_line cl("notepad");

        //
        // Constructs a Win32-specific launcher.  We do not need any of
        // its extra features (compared to the regular launcher), but we
        // must use it in order to construct a Win32-specific child.
        //
        bp::win32_launcher l;

        //
        // Starts the process.
        //
        bp::win32_child c = l.start(cl);

        //
        // Prints out information about the new process.  Note that,
        // except for the process handle, the other information is only
        // available because we are using the win32_child.
        //
        std::cout << "Process handle            : 0x"
                  << c.get_handle() << std::endl;
        std::cout << "Process identifier        : "
                  << c.get_id() << std::endl;
        std::cout << "Primary thread handle     : 0x"
                  << c.get_primary_thread_handle() << std::endl;
        std::cout << "Primary thread identifier : "
                  << c.get_primary_thread_id() << std::endl;

        //
        // Waits until the process terminates and reports status.
        //
        bp::status s = c.wait();
        if (s.exited()) {
            std::cout << "Application exited successfully" << std::endl;
            exitstatus = EXIT_SUCCESS;
        } else {
            std::cout << "The application returned an error" << std::endl;
        }
    } catch (bp::not_found_error< std::string > e) {
        std::cout << "Could not find notepad in path." << std::endl;
    }

    return exitstatus;
}
// quickbook:end(code)

#else // !defined(BOOST_PROCESS_WIN32_API)

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    std::cerr << "This example program is not supported in this platform."
              << std::endl;
    return EXIT_FAILURE;
}

#endif // defined(BOOST_PROCESS_WIN32_API)
