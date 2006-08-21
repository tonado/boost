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
// This example program demonstrates how to interact with child processes
// under a POSIX system, taking advantage of features not available under
// all operating systems.  The program starts up a D-BUS daemon and
// shows how to read information from non-standard streams.
//

// Avoid building this example under non-POSIX systems.
#include <boost/process/config.hpp>
#if defined(BOOST_PROCESS_POSIX_API)

// quickbook:begin(code)
extern "C" {
#include <unistd.h>
}

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/process.hpp>

//
// Error out early if we are trying to build this non-portable example
// code under a platform that does not provide the required posix_*
// classes.
//
#if !defined(BOOST_PROCESS_POSIX_API)
#   error "Unsupported platform."
#endif

namespace bp = ::boost::process;

int
main(int argc, char* argv[])
{
    //
    // Constructs a command line to launch a new D-BUS session daemon.
    //
    bp::command_line cl("/usr/pkg/bin/dbus-daemon");
    cl.argument("--fork");
    cl.argument("--session");

    //
    // The following arguments ask the dbus-daemon program to print the
    // new daemon's bind address and PID into two non-standard streams
    // (i.e. not stdout nor stderr).
    //
    cl.argument("--print-address=3");
    cl.argument("--print-pid=4");

    //
    // Constructs the launcher for the previous command line.  We ask
    // it to inherit our stdout and stderr for simplicity and we capture
    // the two non-standard streams into which the daemon will print the
    // communication information.
    //
    bp::posix_launcher l;
    l.set_output_behavior(STDOUT_FILENO, bp::inherit_stream);
    l.set_output_behavior(STDERR_FILENO, bp::inherit_stream);
    l.set_output_behavior(3, bp::redirect_stream);
    l.set_output_behavior(4, bp::redirect_stream);

    //
    // Spawns the child process.
    //
    bp::posix_child c = l.start(cl);

    //
    // Reads the information printed by the dbus-daemon child process
    // from the two non-standard channels.
    //
    std::string address;
    pid_t pid;
    c.get_output(3) >> address;
    c.get_output(4) >> pid;

    //
    // Waits until the process exits and parses its termination status.
    // Note that we can check for non-regular exit termination because
    // POSIX systems can report it; under Windows, the second branch
    // would never execute.
    //
    bp::status s = c.wait();
    if (s.exited()) {
        if (s.exit_status() == EXIT_SUCCESS) {
            std::cout << "D-BUS daemon's address is: " << address << std::endl;
            std::cout << "D-BUS daemon's PID is: " << pid << std::endl;
        } else
            std::cout << "D-BUS daemon returned error condition: "
                      << s.exit_status() << std::endl;
    } else {
        std::cout << "D-BUS daemon terminated abnormally" << std::endl;
    }

    return s.exited() ? s.exit_status() : EXIT_FAILURE;
}
// quickbook:end(code)

#else // !defined(BOOST_PROCESS_POSIX_API)

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    std::cerr << "This example program is not supported in this platform."
              << std::endl;
    return EXIT_FAILURE;
}

#endif // defined(BOOST_PROCESS_POSIX_API)
