//  (C) Copyright Jeremy Siek 2004. Permission to copy, use, modify,
//  sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

//  Revision History
//  03 Jan 2004   Jeremy Siek
//       First draft.


#include <boost/config.hpp>
#include <iostream>

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/same_traits.hpp>

struct none { };
void blah(none) { }

struct zow { };

struct my_ptr1 {
  typedef const volatile zow element_type;
};

struct my_ptr2 { };
namespace boost {
  template <> struct referent<my_ptr2> {
    typedef const volatile zow type;
  };
}

int main()
{
  {
    typedef boost::indirect_iterator<
      int**,
      /* Value = */ boost::use_default, 
      /* Category = */ boost::use_default, 
      /* Reference = */ boost::use_default,
      /* Difference = */ boost::use_default> Iter;
    
    BOOST_STATIC_ASSERT((boost::is_same<Iter::value_type, int>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Iter::reference, int&>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::pointer, int*>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::difference_type, std::ptrdiff_t>::value));
    
    BOOST_STATIC_ASSERT((boost::is_convertible<Iter::iterator_category,
			 std::random_access_iterator_tag>::value));
    BOOST_STATIC_ASSERT((boost::is_convertible<boost::iterator_traversal<Iter>::type,
			 boost::random_access_traversal_tag>::value));
  }
  {
    typedef boost::indirect_iterator<
      my_ptr1*,
      /* Value = */ boost::use_default, 
      /* Category = */ boost::use_default, 
      /* Reference = */ boost::use_default,
      /* Difference = */ boost::use_default> Iter;
    
    BOOST_STATIC_ASSERT((boost::is_same<Iter::value_type, zow>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Iter::reference, zow&>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::pointer, zow*>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::difference_type, std::ptrdiff_t>::value));
    
    BOOST_STATIC_ASSERT((boost::is_convertible<Iter::iterator_category,
			 std::random_access_iterator_tag>::value));
    BOOST_STATIC_ASSERT((boost::is_convertible<boost::iterator_traversal<Iter>::type,
			 boost::random_access_traversal_tag>::value));
  }
#if 0
  // This is a bug.
  {
    typedef boost::indirect_iterator<
      my_ptr2*,
      /* Value = */ boost::use_default, 
      /* Category = */ boost::use_default, 
      /* Reference = */ boost::use_default,
      /* Difference = */ boost::use_default> Iter;
    
    BOOST_STATIC_ASSERT((boost::is_same<Iter::value_type, zow>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Iter::reference, zow&>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::pointer, zow*>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::difference_type, std::ptrdiff_t>::value));
    
    BOOST_STATIC_ASSERT((boost::is_convertible<Iter::iterator_category,
			 std::random_access_iterator_tag>::value));
    BOOST_STATIC_ASSERT((boost::is_convertible<boost::iterator_traversal<Iter>::type,
			 boost::random_access_traversal_tag>::value));
  }
#endif
  {
    typedef boost::indirect_iterator<
      int**,
      const volatile int,
      /* Category = */ boost::use_default, 
      /* Reference = */ boost::use_default,
      /* Difference = */ boost::use_default> Iter;

    BOOST_STATIC_ASSERT((boost::is_same<Iter::value_type, volatile int>::value));
    BOOST_STATIC_ASSERT((boost::is_same<Iter::reference, const volatile int&>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::pointer, const volatile int*>::value));  
    BOOST_STATIC_ASSERT((boost::is_same<Iter::difference_type, std::ptrdiff_t>::value));
    
    BOOST_STATIC_ASSERT((boost::is_convertible<Iter::iterator_category,
			 std::random_access_iterator_tag>::value));
    BOOST_STATIC_ASSERT((boost::is_convertible<boost::iterator_traversal<Iter>::type,
			 boost::random_access_traversal_tag>::value));
  }
}
