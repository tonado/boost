//
// Boost.Process
// Regression tests for the posix_launch class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <cstring>
#   include <string>

#   include <boost/format.hpp>
#   include <boost/process/posix_child.hpp>
#   include <boost/process/posix_context.hpp>
#   include <boost/process/posix_operations.hpp>

#   include "launch_base_test.hpp"

namespace bp = ::boost::process;
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
class launcher
{
public:
    bp::posix_child
    operator()(const std::vector< std::string > args,
               bp::posix_context ctx,
               bp::stream_behavior bstdin = bp::close_stream(),
               bp::stream_behavior bstdout = bp::close_stream(),
               bp::stream_behavior bstderr = bp::close_stream(),
               bool usein = false)
        const
    {
        if (bstdin.get_type() != bp::stream_behavior::close)
            ctx.m_input_behavior.insert
                (bp::behavior_map::value_type(STDIN_FILENO, bstdin));

        if (bstdout.get_type() != bp::stream_behavior::close)
            ctx.m_output_behavior.insert
                (bp::behavior_map::value_type(STDOUT_FILENO, bstdout));

        if (bstderr.get_type() != bp::stream_behavior::close)
            ctx.m_output_behavior.insert
                (bp::behavior_map::value_type(STDERR_FILENO, bstderr));

        return bp::posix_launch(get_helpers_path(), args, ctx);
    }
};
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
test_input(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("stdin-to-stdout");

    bp::posix_context ctx;
    ctx.m_input_behavior.insert
        (bp::behavior_map::value_type(STDIN_FILENO, bp::capture_stream()));
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(STDOUT_FILENO, bp::capture_stream()));
    bp::posix_child c = bp::posix_launch(get_helpers_path(), args, ctx);

    bp::postream& os = c.get_input(STDIN_FILENO);
    bp::pistream& is = c.get_output(STDOUT_FILENO);

    os << "message-to-process" << std::endl;
    os.close();

    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "message-to-process");

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.m_exit_status);
    BOOST_CHECK_EQUAL(s.m_exit_status.get(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
check_output(int desc, const std::string& msg)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("posix-echo-one");
    args.push_back(boost::str(boost::format("%1%") % desc));
    args.push_back(msg);

    bp::posix_context ctx;
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(desc, bp::capture_stream()));
    bp::posix_child c = posix_launch(get_helpers_path(), args, ctx);

    bp::pistream& is = c.get_output(desc);
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.m_exit_status);
    BOOST_CHECK_EQUAL(s.m_exit_status.get(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_output(void)
{
    check_output(STDOUT_FILENO, "message1-stdout");
    check_output(STDOUT_FILENO, "message2-stdout");
    check_output(STDERR_FILENO, "message1-stderr");
    check_output(STDERR_FILENO, "message2-stderr");
    check_output(10, "message1-10");
    check_output(10, "message2-10");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
void
check_redirect(int desc1, int desc2, const std::string& msg)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("posix-echo-two");
    args.push_back(boost::str(boost::format("%1%") % desc1));
    args.push_back(boost::str(boost::format("%1%") % desc2));
    args.push_back(msg);

    bp::posix_context ctx;
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(desc1, bp::capture_stream()));
    ctx.m_output_behavior.insert
        (bp::behavior_map::value_type(desc2,
                                      bp::posix_redirect_stream(desc1)));
    bp::posix_child c = posix_launch(get_helpers_path(), args, ctx);

    bp::pistream& is = c.get_output(desc1);
    int dtmp;
    std::string word;
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc1);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);
    is >> dtmp;
    BOOST_CHECK_EQUAL(dtmp, desc2);
    is >> word;
    BOOST_CHECK_EQUAL(word, msg);

    const bp::status s = c.wait();
    BOOST_REQUIRE(s.m_exit_status);
    BOOST_CHECK_EQUAL(s.m_exit_status.get(), EXIT_SUCCESS);
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_redirect(void)
{
    check_redirect(STDOUT_FILENO, STDERR_FILENO, "message");
    check_redirect(STDERR_FILENO, STDOUT_FILENO, "message");
    check_redirect(4, 5, "message");
    check_redirect(10, 20, "message");
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static
void
test_default_ids(void)
{
    bp::posix_context ctx;
    BOOST_CHECK_EQUAL(ctx.m_gid, ::getgid());
    BOOST_CHECK_EQUAL(ctx.m_egid, ::getegid());
    BOOST_CHECK_EQUAL(ctx.m_uid, ::getuid());
    BOOST_CHECK_EQUAL(ctx.m_euid, ::geteuid());
}
#endif

// ------------------------------------------------------------------------

#if !defined(BOOST_PROCESS_POSIX_API)
static
void
test_dummy(void)
{
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("posix_launch test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    add_tests_launch_base< launcher, bp::posix_context, bp::child >(test);
    add_tests_launch_base< launcher, bp::posix_context, bp::posix_child >
        (test);

    test->add(BOOST_TEST_CASE(&test_output), 0, 10);
    test->add(BOOST_TEST_CASE(&test_redirect), 0, 10);
    test->add(BOOST_TEST_CASE(&test_input), 0, 10);
    test->add(BOOST_TEST_CASE(&test_default_ids));
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
