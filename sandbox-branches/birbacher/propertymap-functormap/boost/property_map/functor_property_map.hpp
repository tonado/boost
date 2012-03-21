//  (C) Copyright Frank Birbacher 2012.
//  Authors: Frank Birbacher

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/property_map for documentation.

#ifndef BOOST_FUNCTOR_PROPERTY_MAP_HPP
#define BOOST_FUNCTOR_PROPERTY_MAP_HPP

#include <boost/property_map/property_map.hpp>
#include <boost/function/function_fwd.hpp>
#include <boost/type_traits/add_lvalue_reference.hpp>

namespace boost {

  //=========================================================================
  // Adapter to turn a functor into a property map

  template <class Functor, 
    class Arg
     >
  class functor_property_map
  {
    /* to be done */
  };

  template <class Functor, class Arg>
  inline iterator_property_map<Functor, Arg>
  make_functor_property_map(Functor f) {
    /* to be done */
    //function_requires< ? >();
    typedef iterator_property_map<Functor, Arg> PA;
    return PA(f);
  }

#if !defined(BOOST_FUNCTION_NO_FUNCTION_TYPE_SYNTAX)
  // Preferred syntax
  template <typename Key, typename Value>
  struct property_traits<function<Value(Key)> > {
    typedef Key key_type;
    typedef Value value_type; 
    typedef typename add_lvalue_reference<Value>::type reference;
    typedef readable_property_map_tag category;
  };
  
  template<typename Key, typename Value>
  inline Value get(function<Value(Key)> const& f, Key const key)
  {
	return f(key);
  }  
#endif // have partial specialization

} // namespace boost

#endif /* BOOST_FUNCTOR_PROPERTY_MAP_HPP */

