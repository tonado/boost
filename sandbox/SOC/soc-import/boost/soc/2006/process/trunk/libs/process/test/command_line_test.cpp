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
#include <cstring>

#include <boost/process/command_line.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

namespace boost {
namespace process {

class launcher {
public:
#if defined(BOOST_PROCESS_POSIX_API)
    static std::pair< size_t, char ** > posix_argv(const command_line& cl)
    {
        return cl.posix_argv();
    }
#elif defined(BOOST_PROCESS_WIN32_API)
    static boost::shared_array< TCHAR > win32_cmdline(const command_line& cl)
    {
        return cl.win32_cmdline();
    }
#endif
};

} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

static void
test_arguments_addition(void)
{
    bp::command_line cl("program");
    cl.argument("first").argument("second").argument("third");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_REQUIRE_EQUAL(args.size(),
        static_cast<bp::command_line::arguments_vector::size_type>(4));
    BOOST_CHECK_EQUAL(args[0], "program");
    BOOST_CHECK_EQUAL(args[1], "first");
    BOOST_CHECK_EQUAL(args[2], "second");
    BOOST_CHECK_EQUAL(args[3], "third");
}

// ------------------------------------------------------------------------

static void
test_arguments_empty(void)
{
    bp::command_line cl("program");
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_CHECK_EQUAL(args.size(),
        static_cast<bp::command_line::arguments_vector::size_type>(1));
    BOOST_CHECK_EQUAL(args[0], "program");
}

// ------------------------------------------------------------------------

static void
test_arguments_types(void)
{
    bp::command_line cl("program");
    cl.argument("string").argument(123L);
    bp::command_line::arguments_vector args = cl.get_arguments();
    BOOST_REQUIRE_EQUAL(args.size(),
        static_cast<bp::command_line::arguments_vector::size_type>(3));
    BOOST_CHECK_EQUAL(args[0], "program");
    BOOST_CHECK_EQUAL(args[1], "string");
    BOOST_CHECK_EQUAL(std::atol(args[2].c_str()), 123L);
}

// ------------------------------------------------------------------------

static void
test_executable(void)
{
    bp::command_line cl("program");
    BOOST_CHECK_EQUAL(cl.get_executable(), "program");
}

// ------------------------------------------------------------------------

static void
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

static void
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

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_posix_argv(void)
{
    bp::command_line cl("program");
    cl.argument("arg1").argument("arg2").argument("arg3");

    std::pair< size_t, char** > args = bp::launcher::posix_argv(cl);
    size_t argc = args.first;
    char** argv = args.second;

    BOOST_REQUIRE_EQUAL(argc, static_cast<size_t>(4));

    BOOST_REQUIRE(std::strcmp(argv[0], "program") == 0);
    BOOST_REQUIRE(std::strcmp(argv[1], "arg1") == 0);
    BOOST_REQUIRE(std::strcmp(argv[2], "arg2") == 0);
    BOOST_REQUIRE(std::strcmp(argv[3], "arg3") == 0);
    BOOST_REQUIRE(argv[4] == NULL);

    delete [] argv[0];
    delete [] argv[1];
    delete [] argv[2];
    delete [] argv[3];
    delete [] argv;
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_WIN32_API)
static void
test_win32_cmdline(void)
{
    bp::command_line cl("program");
    cl.argument("arg1").argument("arg2").argument("arg3");

    boost::shared_array< TCHAR > cmdline = bp::launcher::win32_cmdline(cl);
    BOOST_REQUIRE(::_tcscmp(cmdline.get(),
                            TEXT("program arg1 arg2 arg3")) == 0);
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("command_line test suite");

    test->add(BOOST_TEST_CASE(&test_executable));
    test->add(BOOST_TEST_CASE(&test_progname));
    test->add(BOOST_TEST_CASE(&test_arguments_empty));
    test->add(BOOST_TEST_CASE(&test_arguments_addition));
    test->add(BOOST_TEST_CASE(&test_arguments_types));
    test->add(BOOST_TEST_CASE(&test_shell));

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_posix_argv));
#elif defined(BOOST_PROCESS_WIN32_API)
    test->add(BOOST_TEST_CASE(&test_win32_cmdline));
#endif

    return test;
}
