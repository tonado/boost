//
// Boost.Process
// Regression tests for the child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <cstring> // XXX For EXIT_* codes; should be hidden by the library.
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/process/child.hpp>
#include <boost/process/types.hpp>
#include <boost/process/launcher.hpp>
#include <boost/test/unit_test.hpp>

// XXX Extremely ugly way to determine helpers' path...
#if defined(__APPLE__)
#   define HELPERS_PATH \
        (bfs::initial_path() / "./bin/darwin/debug/helpers").string()
#else
#   define HELPERS_PATH \
        (bfs::initial_path() / "./bin/gcc/debug/helpers").string()
#endif

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

void
test_input(void)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("stdin-to-stdout");
    bp::attributes a(cl);

    bp::launcher l;
    l.input(bp::STDIN).output(bp::STDOUT);
    bp::child c = l.start<bp::attributes>(a);

    bp::postream& os = c.get_input(bp::STDIN);
    bp::pistream& is = c.get_output(bp::STDOUT);

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

void
test_output(bp::desc_t desc,
            const std::string& realmsg,
            const std::string& expmsg)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("echo-one").argument(desc).argument(realmsg);
    bp::attributes a(cl);

    bp::launcher l;
    l.output(desc);
    bp::child c = l.start<bp::attributes>(a);

    bp::pistream& is = c.get_output(desc);
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, expmsg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static void
test_stderr_fail(void)
{
    test_output(bp::STDERR, "message-stderr", "fail-stderr");
}

// ------------------------------------------------------------------------

static void
test_stderr_pass(void)
{
    test_output(bp::STDERR, "message-stderr", "message-stderr");
}

// ------------------------------------------------------------------------

static void
test_stdout_fail(void)
{
    test_output(bp::STDOUT, "message-stdout", "fail-stdout");
}

// ------------------------------------------------------------------------

static void
test_stdout_pass(void)
{
    test_output(bp::STDOUT, "message-stdout", "message-stdout");
}

// ------------------------------------------------------------------------

void
test_merge(bp::desc_t desc1,
           bp::desc_t desc2,
           const std::string& msg)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("echo-two").argument(desc1).argument(desc2).argument(msg);
    bp::attributes a(cl);

    bp::launcher l;
    l.output(desc1);
    l.merge(desc2, desc1);
    bp::child c = l.start<bp::attributes>(a);

    bp::pistream& is = c.get_output(desc1);
    bp::desc_t dtmp;
    std::string word;
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc1);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc2);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_CHECK_EQUAL(s.exit_status(), EXIT_SUCCESS);
}

// ------------------------------------------------------------------------

static void
test_merge_out_err(void)
{
    test_merge(bp::STDOUT, bp::STDERR, "message");
}

// ------------------------------------------------------------------------

static void
test_merge_err_out(void)
{
    test_merge(bp::STDERR, bp::STDOUT, "message");
}

// ------------------------------------------------------------------------

static void
test_merge_non_std(void)
{
    test_merge(4, 5, "message");
    test_merge(10, 20, "message");
}

// ------------------------------------------------------------------------

static void
test_default_work_directory(void)
{
    bp::command_line cl(HELPERS_PATH);
    cl.argument("pwd");
    bp::attributes a(cl);

    bp::launcher l;
    l.output(bp::STDOUT);
    bp::child c = l.start<bp::attributes>(a);

    bp::pistream& is = c.get_output(bp::STDOUT);
    std::string dir;
    is >> dir;

    bp::status s = c.wait();
    BOOST_REQUIRE(s.exited());
    BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

    BOOST_CHECK_EQUAL(dir, a.get_work_directory());
}

// ------------------------------------------------------------------------

static void
test_explicit_work_directory(void)
{
    bfs::path wdir = bfs::current_path() / "test.dir";

    bp::command_line cl(HELPERS_PATH);
    cl.argument("pwd");
    bp::attributes a(cl, wdir.string());

    BOOST_REQUIRE_NO_THROW(bfs::create_directory(wdir));
    try {
        bp::launcher l;
        l.output(bp::STDOUT);
        bp::child c = l.start<bp::attributes>(a);

        bp::pistream& is = c.get_output(bp::STDOUT);
        std::string dir;
        is >> dir;

        bp::status s = c.wait();
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        BOOST_REQUIRE(s.exited());
        BOOST_REQUIRE_EQUAL(s.exit_status(), EXIT_SUCCESS);

        BOOST_CHECK_EQUAL(dir, a.get_work_directory());
    } catch(...) {
        BOOST_CHECK_NO_THROW(bfs::remove_all(wdir));
        throw;
    }
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("child test suite");

    bfs::initial_path();

    test->add(BOOST_TEST_CASE(&test_stdout_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stdout_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_pass), 0, 10);
    test->add(BOOST_TEST_CASE(&test_stderr_fail), 1, 10);
    test->add(BOOST_TEST_CASE(&test_merge_out_err), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge_err_out), 0, 10);
    test->add(BOOST_TEST_CASE(&test_merge_non_std), 0, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_work_directory), 0, 10);
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory), 0, 10);

    return test;
}
