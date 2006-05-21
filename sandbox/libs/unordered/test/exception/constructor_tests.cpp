
//  Copyright Daniel James 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./containers.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/exception_safety.hpp>
#include "../helpers/random_values.hpp"

struct objects
{
    test::exception::object obj;
    test::exception::hash hash;
    test::exception::equal_to equal_to;
    test::exception::allocator<test::exception::object> allocator;
};

template <class T>
struct construct_test1 : public objects, test::exception_base
{
    void run() const {
        T x;
    }
};

template <class T>
struct construct_test2 : public objects, test::exception_base
{
    void run() const {
        T x(300);
    }
};

template <class T>
struct construct_test3 : public objects, test::exception_base
{
    void run() const {
        T x(0, hash);
    }
};

template <class T>
struct construct_test4 : public objects, test::exception_base
{
    void run() const {
        T x(0, hash, equal_to);
    }
};

template <class T>
struct construct_test5 : public objects, test::exception_base
{
    void run() const {
        T x(50, hash, equal_to, allocator);
    }
};

template <class T>
struct range : public test::exception_base
{
    test::random_values<T> values;

    range() : values(5) {}
    range(unsigned int count) : values(count) {}
};

template <class T>
struct range_construct_test1 : public range<T>, objects
{
    void run() const {
        T x(this->values.begin(), this->values.end());
    }
};

template <class T>
struct range_construct_test2 : public range<T>, objects
{
    void run() const {
        T x(this->values.begin(), this->values.end(), 0);
    }
};

template <class T>
struct range_construct_test3 : public range<T>, objects
{
    void run() const {
        T x(this->values.begin(), this->values.end(), 0, hash);
    }
};

template <class T>
struct range_construct_test4 : public range<T>, objects
{
    void run() const {
        T x(this->values.begin(), this->values.end(), 100, hash, equal_to);
    }
};

// Need to run at least one test with a fairly large number
// of objects in case it triggers a rehash.
template <class T>
struct range_construct_test5 : public range<T>, objects
{
    range_construct_test5() : range<T>(60) {}

    void run() const {
        T x(this->values.begin(), this->values.end(), 0, hash, equal_to, allocator);
    }
};

// TODO: Write a test using an input iterator.

RUN_EXCEPTION_TESTS(
    (construct_test1)(construct_test2)(construct_test3)(construct_test4)(construct_test5)
    (range_construct_test1)(range_construct_test2)(range_construct_test3)(range_construct_test4)(range_construct_test5),
    CONTAINER_SEQ)
