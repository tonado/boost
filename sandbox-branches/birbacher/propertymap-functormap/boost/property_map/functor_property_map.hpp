//  (C) Copyright Frank Birbacher 2012.
//  Authors: Frank Birbacher

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/property_map for documentation.

#ifndef BOOST_FUNCTOR_PROPERTY_MAP_HPP
#define BOOST_FUNCTOR_PROPERTY_MAP_HPP

#include <boost/property_map/property_map.hpp>

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

} // namespace boost

#endif /* BOOST_FUNCTOR_PROPERTY_MAP_HPP */

