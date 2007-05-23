// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <vector>
#include "../../../boost/explore/explore.hpp"
#include "../../../boost/explore/stream_container.hpp"

BOOST_AUTO_TEST_CASE( basic_vector_print_test )
{
	std::stringstream str_out;
	
	std::vector<int> vi;
	explore::print(vi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[]");
	
	str_out.str("");
	
	vi.push_back(1);
	explore::print(vi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1]");
	
	str_out.str("");
	
	vi.push_back(2);
	vi.push_back(3);
	explore::print(vi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE( basic_vector_stream_test )
{
	using namespace boost;
	std::stringstream str_out;
	
	std::vector<int> vi;
	str_out << vi;
	BOOST_CHECK_EQUAL(str_out.str(), "[]");
	
	str_out.str("");
	
	vi.push_back(1);
	str_out << vi;
	BOOST_CHECK_EQUAL(str_out.str(), "[1]");
	
	str_out.str("");
	
	vi.push_back(2);
	vi.push_back(3);
	str_out << vi;
	BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}

BOOST_AUTO_TEST_CASE( vector_in_vector_print_test )
{
	std::stringstream str_out;
	
	std::vector<int> vi;
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	
	std::vector<std::vector<int> > vvi;
	explore::print(vvi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[]");
	
	str_out.str("");
	
	vvi.push_back(vi);
	explore::print(vvi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");
	
	str_out.str("");
	
	vvi.push_back(vi);
	vvi.push_back(vi);
	explore::print(vvi, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}

BOOST_AUTO_TEST_CASE( vector_in_vector_stream_test )
{
	using namespace boost;
	std::stringstream str_out;
	
	std::vector<int> vi;
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	
	std::vector<std::vector<int> > vvi;
	str_out << vvi;
	BOOST_CHECK_EQUAL(str_out.str(), "[]");
	
	str_out.str("");
	
	vvi.push_back(vi);
	str_out << vvi;
	BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3]]");
	
	str_out.str("");
	
	vvi.push_back(vi);
	vvi.push_back(vi);
	str_out << vvi;
	BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}

// This test shows that you can use a string to cause the same outputs
// as other print statements.
BOOST_AUTO_TEST_CASE( vector_with_ugly_string_case_print_test )
{
	std::stringstream str_out;
	
	std::vector<std::string> vs;
	vs.push_back("[1, 2, 3], [1, 2, 3], [1, 2, 3]");
	explore::print(vs, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}

BOOST_AUTO_TEST_CASE( vector_with_ugly_string_case_stream_test )
{
	using namespace boost;
	std::stringstream str_out;
	
	std::vector<std::string> vs;
	vs.push_back("[1, 2, 3], [1, 2, 3], [1, 2, 3]");
	str_out << vs;
	BOOST_CHECK_EQUAL(str_out.str(), "[[1, 2, 3], [1, 2, 3], [1, 2, 3]]");
}
