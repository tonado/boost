//
// Boost.Process
// Common regression tests for Launcher implementations.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <string>
#include <utility>

#include <boost/process/pistream.hpp>
#include <boost/process/postream.hpp>
#include <boost/process/status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;

// ------------------------------------------------------------------------

namespace launcher_base_test {

// ------------------------------------------------------------------------

//
// Overview
// --------
//
// The functions below implements tests for the basic Launcher
// implementation.  This (detail::launcher_base) is used to easily
// construct the other launchers in the library.  In order to ensure
// appropriate behavior, the derivated classes must behave as the parent
// one; keeping this set of tests generic makes it easy to check this
// restriction because the tests can easily be applied to any specific
// Launcher implementation.
//
// Start concept
// -------------
//
// The functions in this file all rely on a Start concept.  This concept
// provides a class whose () operator starts a new process given a Launcher
// object and a single Command_Line, returning the appropriate Child.
// The operator also receives a boolean, defaulting to false, that
// indicates if the child process focuses on testing stdin input.  This is
// needed when testing pipelines to properly place the dummy processes.
//

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_close_stdin(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("is-closed-stdin");

    Launcher l1;
    bp::status s1 = Start()(l1, args, true).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    Launcher l2;
    l2.set_stdin_behavior(bp::redirect_stream);
    Child c2 = Start()(l2, args, true);
    c2.get_stdin() << "foo" << std::endl;
    c2.get_stdin().close();
    bp::status s2 = c2.wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_close_stdout(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("is-closed-stdout");

    Launcher l1;
    bp::status s1 = Start()(l1, args).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    Launcher l2;
    l2.set_stdout_behavior(bp::redirect_stream);
    bp::status s2 = Start()(l2, args).wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_close_stderr(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("is-closed-stderr");

    Launcher l1;
    bp::status s1 = Start()(l1, args).wait();
    BOOST_REQUIRE(s1.exited());
    BOOST_CHECK_EQUAL(s1.exit_status(), EXIT_SUCCESS);

    Launcher l2;
    l2.set_stderr_behavior(bp::redirect_stream);
    bp::status s2 = Start()(l2, args).wait();
    BOOST_REQUIRE(s2.exited());
    BOOST_CHECK_EQUAL(s2.exit_status(), EXIT_FAILURE);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_input(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("stdin-to-stdout");

    Launcher l;
    l.set_stdin_behavior(bp::redirect_stream);
    l.set_stdout_behavior(bp::redirect_stream);
    Child c = Start()(l, args);

    bp::postream& os = c.get_stdin();
    bp::pistream& is = c.get_stdout();

    os << "message-to-process" << std::endl;
    os.close();

    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_output(bool out, const std::string& msg)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back(out ? "echo-stdout" : "echo-stderr");
    args.push_back(msg);

    Launcher l;
    if (out)
       l.set_stdout_behavior(bp::redirect_stream);
    else
       l.set_stderr_behavior(bp::redirect_stream);
    Child c = Start()(l, args);

    std::string word;
    if (out) {
        bp::pistream& is = c.get_stdout();
        is >> word;
    } else {
        bp::pistream& is = c.get_stderr();
        is >> word;
    }
    BOOST_CHECK_EQUAL(word, msg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_stderr(void)
{
    test_output< Launcher, Child, Start >(false, "message1-stderr");
    test_output< Launcher, Child, Start >(false, "message2-stderr");
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_stdout(void)
{
    test_output< Launcher, Child, Start >(true, "message1-stdout");
    test_output< Launcher, Child, Start >(true, "message2-stdout");
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_merge_out_err(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("echo-stdout-stderr");
    args.push_back("message-to-two-streams");

    Launcher l;
    l.set_stdout_behavior(bp::redirect_stream);
    l.set_merge_out_err(true);
    Child c = Start()(l, args);

    bp::pistream& is = c.get_stdout();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "stdout");
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-two-streams");
    is >> word;
    BOOST_CHECK_EQUAL(word, "stderr");
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-two-streams");

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
check_work_directory(const std::string& wdir)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("pwd");

    Launcher l;
    l.set_stdout_behavior(bp::redirect_stream);
    if (wdir.empty())
        BOOST_CHECK(bfs::equivalent(l.get_work_directory(),
                                    bfs::current_path().string()));
    else
        l.set_work_directory(wdir);
    Child c = Start()(l, args);

    bp::pistream& is = c.get_stdout();
    std::string dir;
    portable_getline(is, dir);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(bfs::path(dir), bfs::path(l.get_work_directory()));
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_work_directory(void)
{
    check_work_directory< Launcher, Child, Start >("");

    bfs::path wdir = bfs::current_path() / "test.dir";
    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        check_work_directory< Launcher, Child, Start >(wdir.string());
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
    } catch(...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
std::pair< bool, std::string >
get_var_value(Launcher& l, const std::string& var)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("query-env");
    args.push_back("TO_BE_SET");

    l.set_stdout_behavior(bp::redirect_stream);
    Child c = Start()(l, args);

    bp::pistream& is = c.get_stdout();
    std::string status;
    is >> status;
    std::string gotval;
    if (status == "defined")
        is >> gotval;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    return std::pair< bool, std::string >(status == "defined", gotval);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_clear_environment(void)
{
#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("TO_BE_QUERIED", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_QUERIED") != NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable("TO_BE_QUERIED", "test") != 0);
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_QUERIED", buf, 5) != 0);
#endif

    Launcher l;
    l.clear_environment();

    std::pair< bool, std::string > p1 =
        get_var_value< Launcher, Child, Start >(l, "TO_BE_QUERIED");
    BOOST_REQUIRE(!p1.first);

    std::pair< bool, std::string > p2 =
        get_var_value< Launcher, Child, Start >(l, "PATH");
    BOOST_REQUIRE(!p2.first);

    std::pair< bool, std::string > p3 =
        get_var_value< Launcher, Child, Start >(l, "HOME");
    BOOST_REQUIRE(!p3.first);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_unset_environment(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("query-env");
    args.push_back("TO_BE_UNSET");

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("TO_BE_UNSET", "test", 1) != -1);
    BOOST_REQUIRE(::getenv("TO_BE_UNSET") != NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable("TO_BE_UNSET", "test") != 0);
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_UNSET", buf, 5) != 0);
#endif

    Launcher l;
    l.unset_environment("TO_BE_UNSET");
    std::pair< bool, std::string > p =
        get_var_value< Launcher, Child, Start >(l, "TO_BE_SET");
    BOOST_CHECK(!p.first);
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_set_environment_var(const std::string& value)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("query-env");
    args.push_back("TO_BE_SET");

#if defined(BOOST_PROCESS_POSIX_API)
    ::unsetenv("TO_BE_SET");
    BOOST_REQUIRE(::getenv("TO_BE_SET") == NULL);
#elif defined(BOOST_PROCESS_WIN32_API)
    TCHAR buf[5];
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 0) == 0 ||
                  ::SetEnvironmentVariable("TO_BE_SET", NULL) != 0);
    BOOST_REQUIRE(::GetEnvironmentVariable("TO_BE_SET", buf, 5) == 0);
#endif

    Launcher l;
    l.set_environment("TO_BE_SET", value);
    std::pair< bool, std::string > p =
        get_var_value< Launcher, Child, Start >(l, "TO_BE_SET");
    BOOST_CHECK(p.first);
    BOOST_CHECK_EQUAL(p.second, "'" + value + "'");
}

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
static
void
test_set_environment(void)
{
    if (bp_api_type == posix_api)
        test_set_environment_var< Launcher, Child, Start >("");
    test_set_environment_var< Launcher, Child, Start >("some-value-1");
    test_set_environment_var< Launcher, Child, Start >("some-value-2");
}

// ------------------------------------------------------------------------

} // namespace launcher_base_test

// ------------------------------------------------------------------------

template< class Launcher, class Child, class Start >
void
add_tests_launcher_base(boost::unit_test::test_suite* ts)
{
    using namespace launcher_base_test;

    ts->add(BOOST_TEST_CASE
            (&(test_close_stdin< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_close_stdout< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_close_stderr< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_stdout< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_stderr< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_merge_out_err< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_input< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_work_directory< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_clear_environment< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_unset_environment< Launcher, Child, Start >)), 0, 10);
    ts->add(BOOST_TEST_CASE
            (&(test_set_environment< Launcher, Child, Start >)), 0, 10);
}
