/* Copyright 2003-2004 Joaqu�n M L�pez Mu�oz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef BOOST_MULTI_INDEX_DETAIL_INDEX_NODE_BASE_HPP
#define BOOST_MULTI_INDEX_DETAIL_INDEX_NODE_BASE_HPP

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/access.hpp>
#include <boost/throw_exception.hpp> 
#endif

namespace boost{

namespace multi_index{

namespace detail{

/* index_node_base tops the node hierarchy of multi_index_container. It holds
 * the value of the element contained.
 */

template<typename Value>
struct index_node_base
{
  typedef Value value_type;
  value_type    value;

private:
  index_node_base();
  /* this class is not intended to be cted, merely allocated */

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  friend class boost::serialization::access;
  
  /* nodes do not emit any kind of serialization info. They are
   * fed to Boost.Serialization so that pointers to nodes are
   * tracked correctly.
   */

  template<class Archive>
  void serialize(Archive&,const unsigned int)
  {
  }
#endif

};

} /* namespace multi_index::detail */

} /* namespace multi_index */

#if !defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
/* Index nodes never get constructed directly by Boost.Serialization,
 * as archives are always fed pointers to previously existent
 * nodes. So, if this is called it means we are dealing with a
 * somehow invalid archive.
 */

namespace serialization{

template<class Archive,typename Value>
inline void load_construct_data(
  Archive&,multi_index::detail::index_node_base<Value>*,
  unsigned int)
{
  throw_exception(
    archive::archive_exception(archive::archive_exception::other_exception));
}

} /* namespace serialization */
#endif

} /* namespace boost */

#endif
