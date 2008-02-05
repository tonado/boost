
// Copyright 2006-2007 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <algorithm>
#include <map>
#include <list>
#include "../helpers/tracker.hpp"
#include "../helpers/invariants.hpp"

#include <iostream>

template <class Container, class Iterator>
void test_equal_insertion(Iterator begin, Iterator end)
{
    typedef test::ordered<Container> tracker;

    Container x1;
    tracker x2 = test::create_ordered(x1);

    for(Iterator it = begin; it != end; ++it) {
        x1.insert(*it);
        x2.insert(*it);
        x2.compare_key(x1, *it);
    }

    x2.compare(x1);
    test::check_equivalent_keys(x1);
}

void set_tests()
{
    int values[][5] = {
        {1},
        {54, 23},
        {-13, 65},
        {77, 77},
        {986, 25, 986}
    };

    test_equal_insertion<boost::unordered_set<int> >(values[0], values[0] + 1);
    test_equal_insertion<boost::unordered_set<int> >(values[1], values[1] + 2);
    test_equal_insertion<boost::unordered_set<int> >(values[2], values[2] + 2);
    test_equal_insertion<boost::unordered_set<int> >(values[3], values[3] + 2);
    test_equal_insertion<boost::unordered_set<int> >(values[4], values[4] + 3);

    test_equal_insertion<boost::unordered_multiset<int> >(values[0], values[0] + 1);
    test_equal_insertion<boost::unordered_multiset<int> >(values[1], values[1] + 2);
    test_equal_insertion<boost::unordered_multiset<int> >(values[2], values[2] + 2);
    test_equal_insertion<boost::unordered_multiset<int> >(values[3], values[3] + 2);
    test_equal_insertion<boost::unordered_multiset<int> >(values[4], values[4] + 3);
}

void map_tests()
{
    typedef std::list<std::pair<int const, int> > values_type;
    values_type v[5];
    v[0].push_back(std::pair<int const, int>(1,1));
    v[1].push_back(std::pair<int const, int>(28,34));
    v[1].push_back(std::pair<int const, int>(16,58));
    v[1].push_back(std::pair<int const, int>(-124, 62));
    v[2].push_back(std::pair<int const, int>(432,12));
    v[2].push_back(std::pair<int const, int>(9,13));
    v[2].push_back(std::pair<int const, int>(432,24));

    for(int i = 0; i < 5; ++i)
        test_equal_insertion<boost::unordered_map<int, int> >(
            v[i].begin(), v[i].end());

    for(int i2 = 0; i2 < 5; ++i2)
        test_equal_insertion<boost::unordered_multimap<int, int> >(
            v[i2].begin(), v[i2].end());
}

int main()
{
    set_tests();
    map_tests();

    return boost::report_errors();
}
