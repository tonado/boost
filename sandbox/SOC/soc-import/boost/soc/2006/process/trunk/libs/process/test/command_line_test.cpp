//
// Boost.Process
// Regression tests for the command_line class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstdlib>

#include <boost/process/child.hpp>
#include <boost/process/command_line.hpp>
#include <boost/process/launcher.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
void
test_arguments_addition(void)
{
    bp::command_line cl("program");
    cl.argument("first").argument("second").argument("third");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_REQUIRE_EQUAL(args.size(),
        static_cast< bp::command_line::arguments_vector::size_type >(4));
    BOOST_CHECK_EQUAL(args[0], "program");
    BOOST_CHECK_EQUAL(args[1], "first");
    BOOST_CHECK_EQUAL(args[2], "second");
    BOOST_CHECK_EQUAL(args[3], "third");
}

// ------------------------------------------------------------------------

static
void
test_arguments_empty(void)
{
    bp::command_line cl("program");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_CHECK_EQUAL(args.size(),
        static_cast< bp::command_line::arguments_vector::size_type >(1));
    BOOST_CHECK_EQUAL(args[0], "program");
}

// ------------------------------------------------------------------------

static
void
test_arguments_types(void)
{
    bp::command_line cl("program");
    cl.argument("string").argument(123L);
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_REQUIRE_EQUAL(args.size(),
        static_cast< bp::command_line::arguments_vector::size_type >(3));
    BOOST_CHECK_EQUAL(args[0], "program");
    BOOST_CHECK_EQUAL(args[1], "string");
    BOOST_CHECK_EQUAL(std::atol(args[2].c_str()), 123L);
}

// ------------------------------------------------------------------------

static
void
test_executable(void)
{
    bp::command_line cl("program");
    BOOST_CHECK_EQUAL(cl.get_executable(), "program");
}

// ------------------------------------------------------------------------

static
void
test_progname(void)
{
    bp::command_line cl1("program");
    BOOST_CHECK_EQUAL(cl1.get_arguments()[0], "program");

    bp::command_line cl2("program", "thetest");
    BOOST_CHECK_EQUAL(cl2.get_arguments()[0], "thetest");

    bp::command_line cl3("/path/to/program");
    BOOST_CHECK_EQUAL(cl3.get_arguments()[0], "program");

#if defined(BOOST_PROCESS_WIN32_API)
    bp::command_line cl4("C:\\path\\to\\program");
    BOOST_CHECK_EQUAL(cl4.get_arguments()[0], "program");
#endif
}

// ------------------------------------------------------------------------

static
std::string
get_argument(const std::string& word)
{
    bp::command_line cl = bp::command_line(get_helpers_path());
    cl.argument("echo-quoted").argument(word);

    bp::launcher l;
    l.set_stdout_behavior(bp::redirect_stream);

    bp::child c = l.start(cl);
    bp::pistream& is = c.get_stdout();

    std::string result;
    portable_getline(is, result);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);

    return result;
}

// ------------------------------------------------------------------------

static
void
test_quoting(void)
{
    BOOST_CHECK_EQUAL(get_argument("foo"), ">>>foo<<<");
    BOOST_CHECK_EQUAL(get_argument("foo "), ">>>foo <<<");
    BOOST_CHECK_EQUAL(get_argument(" foo"), ">>> foo<<<");
    BOOST_CHECK_EQUAL(get_argument("foo bar"), ">>>foo bar<<<");

    BOOST_CHECK_EQUAL(get_argument("*"), ">>>*<<<");
    BOOST_CHECK_EQUAL(get_argument("?*"), ">>>?*<<<");
    BOOST_CHECK_EQUAL(get_argument("[a-z]*"), ">>>[a-z]*<<<");
}

// ------------------------------------------------------------------------

static
void
test_shell(void)
{
    bp::command_line cl = bp::command_line::shell("test command");

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(cl.get_executable().find("sh") != std::string::npos);
    BOOST_REQUIRE_EQUAL(cl.get_arguments().size(),
        static_cast< bp::command_line::arguments_vector::size_type >(3));
    BOOST_CHECK_EQUAL(cl.get_arguments()[0], "sh");
    BOOST_CHECK_EQUAL(cl.get_arguments()[1], "-c");
    BOOST_CHECK_EQUAL(cl.get_arguments()[2], "test command");
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(cl.get_executable().find("cmd.exe") != std::string::npos);
    BOOST_REQUIRE_EQUAL(cl.get_arguments().size(),
        static_cast< bp::command_line::arguments_vector::size_type >(3));
    BOOST_CHECK_EQUAL(cl.get_arguments()[0], "cmd");
    BOOST_CHECK_EQUAL(cl.get_arguments()[1], "/c");
    BOOST_CHECK_EQUAL(cl.get_arguments()[2], "test command");
#endif
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("command_line test suite");

    test->add(BOOST_TEST_CASE(&test_executable));
    test->add(BOOST_TEST_CASE(&test_progname));
    test->add(BOOST_TEST_CASE(&test_arguments_empty));
    test->add(BOOST_TEST_CASE(&test_arguments_addition));
    test->add(BOOST_TEST_CASE(&test_arguments_types));
    test->add(BOOST_TEST_CASE(&test_shell));
    test->add(BOOST_TEST_CASE(&test_quoting));

    return test;
}
