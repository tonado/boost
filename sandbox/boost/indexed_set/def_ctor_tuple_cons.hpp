/* Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_DEF_CTOR_TUPLE_CONS_HPP
#define BOOST_INDEXED_SET_DEF_CTOR_TUPLE_CONS_HPP

#include <boost/config.hpp>

#if defined(BOOST_MSVC)
/* In MSVC, tuples::cons is not default constructible. We provide a
 * tiny wrapper around tuple::cons filling that hole.
 */

#include <boost/tuple/tuple.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

template<typename Cons>
struct default_constructible_tuple_cons:Cons
{
  default_constructible_tuple_cons():
    Cons(
      Cons::head_type(),
      static_cast<const Cons::tail_type&>(
        default_constructible_tuple_cons<Cons::tail_type>()))
  {}

  default_constructible_tuple_cons(const Cons& cons):Cons(cons){}
};

template<>
struct default_constructible_tuple_cons<tuples::null_type>:tuples::null_type
{
  default_constructible_tuple_cons(){}
  default_constructible_tuple_cons(const tuples::null_type&){}
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif /* BOOST_MSVC */

#endif
