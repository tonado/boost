//  Demonstrate and test boost/operators.hpp on std::iterators  -------------//

//  (C) Copyright Jeremy Siek 1999. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  13 Jun 00 Added const version of the iterator tests (Jeremy Siek)
//  12 Dec 99 Initial version with iterator operators (Jeremy Siek)

#include <iostream>

#include <functional>
#include <boost/iterator_adaptors.hpp>
#include <boost/iterator_tests.hpp>

struct my_iterator_tag : public std::random_access_iterator_tag { };


using boost::dummyT;

struct my_iter_traits {
  typedef dummyT value_type;
  typedef dummyT* pointer;
  typedef dummyT& reference;
  typedef my_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;
};

struct my_const_iter_traits {
  typedef dummyT value_type;
  typedef const dummyT* pointer;
  typedef const dummyT& reference;
  typedef my_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;
};

typedef boost::iterator_adaptors
  <dummyT*, const dummyT*, 
   my_iter_traits, my_const_iter_traits> My;

struct mult_functor {
  typedef int result_type;
  typedef int argument_type;
  // Functors used with transform_iterator must be
  // DefaultConstructible, as the transform_iterator must be
  // DefaultConstructible to satisfy the requirements for
  // TrivialIterator.
  mult_functor() { }
  mult_functor(int aa) : a(aa) { }
  int operator()(int b) const { return a * b; }
  int a;
};

int
main()
{
  dummyT array[] = { dummyT(0), dummyT(1), dummyT(2), 
		     dummyT(3), dummyT(4), dummyT(5) };
  const int N = sizeof(array)/sizeof(dummyT);

  // sanity check, if this doesn't pass the test is buggy
  boost::random_access_iterator_test(array,N,array);

  // Test the iterator_adaptors
  {
    My::iterator i = array;
    boost::random_access_iterator_test(i, N, array);
    
    My::const_iterator j = array;
    boost::random_access_iterator_test(j, N, array);
    boost::const_nonconst_iterator_test(i, ++j);
  }
  // Test transform_iterator
  {
    int x[N], y[N];
    for (int k = 0; k < N; ++k)
      x[k] = k;
    std::copy(x, x + N, y);

    for (int k = 0; k < N; ++k)
      x[k] = x[k] * 2;

    boost::transform_iterator<mult_functor, int*>::type i(y, mult_functor(2));
    boost::random_access_iterator_test(i, N, x);
  }
  // Test indirect_iterator
  {
    dummyT* ptr[N];
    for (int k = 0; k < N; ++k)
      ptr[k] = array + k;
    
    typedef dummyT* DummyPtr;
    typedef boost::indirect_iterators<DummyPtr*, const DummyPtr*> Indirect;
    Indirect::iterator i = ptr;
    boost::random_access_iterator_test(i, N, array);

    Indirect::const_iterator j = ptr;
    boost::random_access_iterator_test(j, N, array);

    boost::const_nonconst_iterator_test(i, ++j);    
  }
  // Test reverse_iterators
  {
    dummyT reversed[N];
    std::copy(array, array + N, reversed);
    std::reverse(reversed, reversed + N);
    
    typedef boost::reverse_iterators<dummyT*, const dummyT*> Reverse;
    Reverse::iterator i = reversed + N;
    boost::random_access_iterator_test(i, N, array);

    Reverse::const_iterator j = reversed + N;
    boost::random_access_iterator_test(j, N, array);

    boost::const_nonconst_iterator_test(i, ++j);    
  }

  // Test integer_range's iterators
  {
    int int_array[] = { 0, 1, 2, 3, 4, 5 };
    boost::integer_range<int> r(0, 5);
    boost::random_access_iterator_test(r.begin(), r.size(), int_array);
  }

  std::cout << "test successful " << std::endl;

  return 0;
}
