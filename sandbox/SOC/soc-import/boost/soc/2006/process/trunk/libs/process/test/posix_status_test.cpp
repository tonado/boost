//
// Boost.Process
// Regression tests for the posix_status class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)

#   include <boost/process/posix_status.hpp>

#   include "status_base_test.hpp"

namespace bp = ::boost::process;
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace but = ::boost::unit_test;

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

    but::test_suite* test = BOOST_TEST_SUITE("posix_status test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    add_tests_status_base< bp::posix_status >(test);
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
