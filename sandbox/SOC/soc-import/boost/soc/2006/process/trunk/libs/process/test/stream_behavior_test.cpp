//
// Boost.Process
// Regression tests for the stream_behavior class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/stream_behavior.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

namespace boost {
namespace process {
namespace detail {

// Tests are declared in this class so that we can access stream_behavior's
// internals.  This is the only way to check the constructors.
class stream_info
{
public:
    static void test_capture(void)
    {
        bp::stream_behavior sb = bp::capture_stream();
        BOOST_CHECK_EQUAL(sb.get_type(), bp::stream_behavior::capture);
    }

    static void test_close(void)
    {
        bp::stream_behavior sb = bp::close_stream();
        BOOST_CHECK_EQUAL(sb.get_type(), bp::stream_behavior::close);
    }

    static void test_inherit(void)
    {
        bp::stream_behavior sb = bp::inherit_stream();
        BOOST_CHECK_EQUAL(sb.get_type(), bp::stream_behavior::inherit);
    }

    static void test_redirect_to_stdout(void)
    {
        bp::stream_behavior sb = bp::redirect_stream_to_stdout();
        BOOST_CHECK_EQUAL(sb.get_type(),
                          bp::stream_behavior::redirect_to_stdout);
    }

    static void test_silence(void)
    {
        bp::stream_behavior sb = bp::silence_stream();
        BOOST_CHECK_EQUAL(sb.get_type(), bp::stream_behavior::silence);
    }

#if defined(BOOST_PROCESS_POSIX_API)
    static void test_posix_redirect(void)
    {
        bp::stream_behavior sb1 = bp::posix_redirect_stream(1);
        BOOST_CHECK_EQUAL(sb1.get_type(),
                          bp::stream_behavior::posix_redirect);
        BOOST_CHECK_EQUAL(sb1.m_desc_to, 1);

        bp::stream_behavior sb2 = bp::posix_redirect_stream(2);
        BOOST_CHECK_EQUAL(sb2.get_type(),
                          bp::stream_behavior::posix_redirect);
        BOOST_CHECK_EQUAL(sb2.m_desc_to, 2);
    }
#endif
};

} // namespace detail
} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    using boost::process::detail::stream_info;

    but::test_suite* test = BOOST_TEST_SUITE("stream_behavior test suite");

    test->add(BOOST_TEST_CASE(&stream_info::test_capture));
    test->add(BOOST_TEST_CASE(&stream_info::test_close));
    test->add(BOOST_TEST_CASE(&stream_info::test_inherit));
    test->add(BOOST_TEST_CASE(&stream_info::test_redirect_to_stdout));
    test->add(BOOST_TEST_CASE(&stream_info::test_silence));
#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&stream_info::test_posix_redirect));
#endif

    return test;
}
