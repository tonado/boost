#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include "../objects/exception.hpp"

typedef boost::unordered_set<
    test::exception::object,
    test::exception::hash,
    test::exception::equal_to,
    test::exception::allocator<test::exception::object> > test_set;
typedef boost::unordered_multiset<
    test::exception::object,
    test::exception::hash,
    test::exception::equal_to,
    test::exception::allocator<test::exception::object> > test_multiset;
typedef boost::unordered_map<
    test::exception::object,
    test::exception::object,
    test::exception::hash,
    test::exception::equal_to,
    test::exception::allocator<test::exception::object> > test_map;
typedef boost::unordered_multimap<
    test::exception::object,
    test::exception::object,
    test::exception::hash,
    test::exception::equal_to,
    test::exception::allocator<test::exception::object> > test_multimap;

#define CONTAINER_SEQ (test_set)(test_multiset)(test_map)(test_multimap)
