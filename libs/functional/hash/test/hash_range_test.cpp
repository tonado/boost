
// Copyright 2005-2008 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./config.hpp"

#ifdef TEST_EXTENSIONS
#  ifdef TEST_STD_INCLUDES
#    include <functional>
#  else
#    include <boost/functional/hash.hpp>
#  endif
#endif

#include <boost/detail/lightweight_test.hpp>

#ifdef TEST_EXTENSIONS

#include <boost/limits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <vector>

void hash_range_tests()
{
    std::vector<int> empty, values1, values2, values3, values4, values5;
    values1.push_back(0);
    values2.push_back(10);
    values3.push_back(10);
    values3.push_back(20);
    values4.push_back(15);
    values4.push_back(75);
    values5.push_back(10);
    values5.push_back(20);
    values5.push_back(15);
    values5.push_back(75);
    values5.push_back(10);
    values5.push_back(20);

    std::vector<int> x;

    std::size_t x_seed = 0;
    BOOST_TEST(x_seed == HASH_NAMESPACE::hash_range(x.begin(), x.end()));

    BOOST_TEST(HASH_NAMESPACE::hash_range(empty.begin(), empty.end())
        == HASH_NAMESPACE::hash_range(x.begin(), x.end()));
    BOOST_TEST(HASH_NAMESPACE::hash_range(empty.begin(), empty.end())
        != HASH_NAMESPACE::hash_range(values1.begin(), values1.end()));

    x.push_back(10);
    HASH_NAMESPACE::hash_combine(x_seed, 10);
    BOOST_TEST(x_seed == HASH_NAMESPACE::hash_range(x.begin(), x.end()));

    BOOST_TEST(HASH_NAMESPACE::hash_range(empty.begin(), empty.end())
        != HASH_NAMESPACE::hash_range(x.begin(), x.end()));
    BOOST_TEST(HASH_NAMESPACE::hash_range(values2.begin(), values2.end())
        == HASH_NAMESPACE::hash_range(x.begin(), x.end()));

    x.push_back(20);
    HASH_NAMESPACE::hash_combine(x_seed, 20);
    BOOST_TEST(x_seed == HASH_NAMESPACE::hash_range(x.begin(), x.end()));

    BOOST_TEST(HASH_NAMESPACE::hash_range(empty.begin(), empty.end())
        != HASH_NAMESPACE::hash_range(x.begin(), x.end()));
    BOOST_TEST(HASH_NAMESPACE::hash_range(values2.begin(), values2.end())
        != HASH_NAMESPACE::hash_range(x.begin(), x.end()));
    BOOST_TEST(HASH_NAMESPACE::hash_range(values3.begin(), values3.end())
        == HASH_NAMESPACE::hash_range(x.begin(), x.end()));

    std::size_t seed = HASH_NAMESPACE::hash_range(values3.begin(), values3.end());
    HASH_NAMESPACE::hash_range(seed, values4.begin(), values4.end());
    HASH_NAMESPACE::hash_range(seed, x.begin(), x.end());
    BOOST_TEST(seed == HASH_NAMESPACE::hash_range(values5.begin(), values5.end()));
}

#endif

int main()
{
    hash_range_tests();

    return boost::report_errors();
}

