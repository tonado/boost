//  (C) Copyright David Abrahams 2001. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  04 Mar 2001 Patches for Intel C++ (Dave Abrahams)
//  19 Feb 2001 Take advantage of improved iterator_traits to do more tests
//              on MSVC. Reordered some #ifdefs for coherency.
//              (David Abrahams)
//  13 Feb 2001 Test new VC6 workarounds (David Abrahams)
//  11 Feb 2001 Final fixes for Borland (David Abrahams)
//  11 Feb 2001 Some fixes for Borland get it closer on that compiler
//              (David Abrahams)
//  07 Feb 2001 More comprehensive testing; factored out static tests for
//              better reuse (David Abrahams)
//  21 Jan 2001 Quick fix to my_iterator, which wasn't returning a
//              reference type from operator* (David Abrahams)
//  19 Jan 2001 Initial version with iterator operators (David Abrahams)

#include <boost/detail/iterator.hpp>
#include <boost/type_traits.hpp>
#include <boost/operators.hpp>
#include <boost/static_assert.hpp>
#include <iterator>
#include <vector>
#include <list>
#include <cassert>
#include <iostream>

// An iterator for which we can get traits.
struct my_iterator1
    : boost::forward_iterator_helper<my_iterator1, char, long, const char*, const char&>
{
    my_iterator1(const char* p) : m_p(p) {}
    
    bool operator==(const my_iterator1& rhs) const
        { return this->m_p == rhs.m_p; }

    my_iterator1& operator++() { ++this->m_p; return *this; }
    const char& operator*() { return *m_p; }
 private:
    const char* m_p;
};

// Used to prove that we don't require std::iterator<> in the hierarchy under
// MSVC6, and that we can compute all the traits for a standard-conforming UDT
// iterator.
struct my_iterator2
    : boost::equality_comparable<my_iterator2
    , boost::incrementable<my_iterator2
    , boost::dereferenceable<my_iterator2,const char*> > >
{
    typedef char value_type;
    typedef long difference_type;
    typedef const char* pointer;
    typedef const char& reference;
    typedef std::forward_iterator_tag iterator_category;
    
    my_iterator2(const char* p) : m_p(p) {}
    
    bool operator==(const my_iterator2& rhs) const
        { return this->m_p == rhs.m_p; }

    my_iterator2& operator++() { ++this->m_p; return *this; }
    const char& operator*() { return *m_p; }
 private:
    const char* m_p;
};

// Used to prove that we're not overly confused by the existence of
// std::iterator<> in the hierarchy under MSVC6 - we should find that
// boost::detail::iterator_traits<my_iterator3>::difference_type is int.
struct my_iterator3 : my_iterator1
{
    typedef int difference_type;
    my_iterator3(const char* p) : my_iterator1(p) {}
};

template <class Iterator,
    class value_type, class difference_type, class pointer, class reference, class category>
struct non_portable_tests
{
    // Unfortunately, the VC6 standard library doesn't supply these :(
    BOOST_STATIC_ASSERT((
        boost::is_same<
        typename boost::detail::iterator_traits<Iterator>::pointer,
        pointer
        >::value));
    
    BOOST_STATIC_ASSERT((
        boost::is_same<
        typename boost::detail::iterator_traits<Iterator>::reference,
        reference
        >::value));
};

template <class Iterator,
    class value_type, class difference_type, class pointer, class reference, class category>
struct portable_tests
{
    BOOST_STATIC_ASSERT((
        boost::is_same<
        typename boost::detail::iterator_traits<Iterator>::difference_type,
        difference_type
        >::value));
    
    BOOST_STATIC_ASSERT((
        boost::is_same<
        typename boost::detail::iterator_traits<Iterator>::iterator_category,
        category
        >::value));
};

// Test iterator_traits
template <class Iterator,
    class value_type, class difference_type, class pointer, class reference, class category>
struct input_iterator_test
    : portable_tests<Iterator,value_type,difference_type,pointer,reference,category>
{
    BOOST_STATIC_ASSERT((
        boost::is_same<
        typename boost::detail::iterator_traits<Iterator>::value_type,
        value_type
        >::value));
};

template <class Iterator,
    class value_type, class difference_type, class pointer, class reference, class category>
struct non_pointer_test
    : input_iterator_test<Iterator,value_type,difference_type,pointer,reference,category>
      , non_portable_tests<Iterator,value_type,difference_type,pointer,reference,category>
{
};

template <class Iterator,
    class value_type, class difference_type, class pointer, class reference, class category>
struct maybe_pointer_test
    : portable_tests<Iterator,value_type,difference_type,pointer,reference,category>
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
      , non_portable_tests<Iterator,value_type,difference_type,pointer,reference,category>
#endif
{
};

input_iterator_test<std::istream_iterator<int>, int, std::ptrdiff_t, int*, int&, std::input_iterator_tag>
        istream_iterator_test;

// 
#if defined(__BORLANDC__) && !defined(__SGI_STL_PORT)
typedef ::std::char_traits<char>::off_type distance;
non_pointer_test<std::ostream_iterator<int>,int,
    distance,int*,int&,std::output_iterator_tag> ostream_iterator_test;
#elif defined(BOOST_MSVC_STD_ITERATOR)
non_pointer_test<std::ostream_iterator<int>,
    int, void, void, void, std::output_iterator_tag>
        ostream_iterator_test;
#else
non_pointer_test<std::ostream_iterator<int>,
    void, void, void, void, std::output_iterator_tag>
        ostream_iterator_test;
#endif


#ifdef __KCC
  typedef long std_list_diff_type;
#else
  typedef std::ptrdiff_t std_list_diff_type;
#endif
non_pointer_test<std::list<int>::iterator, int, std_list_diff_type, int*, int&, std::bidirectional_iterator_tag>
        list_iterator_test;

maybe_pointer_test<std::vector<int>::iterator, int, std::ptrdiff_t, int*, int&, std::random_access_iterator_tag>
        vector_iterator_test;

maybe_pointer_test<int*, int, std::ptrdiff_t, int*, int&, std::random_access_iterator_tag>
        int_pointer_test;

non_pointer_test<my_iterator1, char, long, const char*, const char&, std::forward_iterator_tag>
       my_iterator1_test;
                    
non_pointer_test<my_iterator2, char, long, const char*, const char&, std::forward_iterator_tag>
       my_iterator2_test;
                    
non_pointer_test<my_iterator3, char, int, const char*, const char&, std::forward_iterator_tag>
       my_iterator3_test;
                    
int main()
{
    char chars[100];
    int ints[100];
    
    for (std::ptrdiff_t length = 3; length < 100; length += length / 3)
    {
        std::list<int> l(length);
        assert(boost::detail::distance(l.begin(), l.end()) == length);
        
        std::vector<int> v(length);
        assert(boost::detail::distance(v.begin(), v.end()) == length);

        assert(boost::detail::distance(&ints[0], ints + length) == length);
        assert(boost::detail::distance(my_iterator1(chars), my_iterator1(chars + length)) == length);
        assert(boost::detail::distance(my_iterator2(chars), my_iterator2(chars + length)) == length);
        assert(boost::detail::distance(my_iterator3(chars), my_iterator3(chars + length)) == length);
    }
    return 0;
}
