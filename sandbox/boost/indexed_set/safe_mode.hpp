/* Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_SAFE_MODE_HPP
#define BOOST_INDEXED_SET_SAFE_MODE_HPP

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <algorithm>
#include <boost/indexed_set/access_specifier.hpp>
#include <boost/indexed_set/safe_mode_errors.hpp>
#include <boost/noncopyable.hpp>

namespace boost{

namespace indexed_sets{

/* Safe mode machinery, in the spirit of Cay Hortmann's "Safe STL"
 * (http://www.horstmann.com/safestl.html).
 * In this mode, containers of type Container are derived from
 * safe_container<Container>, and their corresponding iterators
 * are derived from safe_iterator<Container>. These classes provide
 * an internal record of which iterators are at a given moment associated
 * to a given container, and properly mark the iterators as invalid
 * when the container gets destroyed.
 * Iterators are chained in a single attached list, whose header is
 * kept by the container. More elaborate data structures would yield better
 * performance, but I decided to keep complexity to a minimum since
 * speed is not an issue here.
 * This is not a full-fledged safe mode framework, and is only inteded
 * for use within the limits of Boost.IndexedSet.
 */

namespace safe_mode{

/* Invalidates all iterators equivalent to that given. Defined before
 * safe_iterator_base and safe_container_base as these contain friendship
 * declarations to this function.
 */

template<typename Iterator>
inline void detach_equivalent_iterators(Iterator& it)
{
  if(it.valid()){
    Iterator *prev_,*next_;
    for(
      prev_=static_cast<Iterator*>(&it.cont->header);
      (next_=static_cast<Iterator*>(prev_->next))!=0;){
      if(next_!=&it&&*next_==it){
        prev_->next=next_->next;
        next_->cont=0;
      }
      else prev_=next_;
    }
    it.detach();
  }
}

} /* namespace indexed_sets::safe_mode */

namespace detail{

class safe_container_base;

class safe_iterator_base
{
public:
  bool valid()const{return cont!=0;}
  inline void detach();

protected:
  safe_iterator_base():cont(0),next(0){}
  explicit safe_iterator_base(safe_container_base* cont_){attach(cont_);}
  safe_iterator_base(const safe_iterator_base& it){attach(it.cont);}

  safe_iterator_base& operator=(const safe_iterator_base& it)
  {
    safe_container_base* new_cont=it.cont;
    if(cont!=new_cont){
      detach();
      attach(new_cont);
    }
    return *this;
  }

  ~safe_iterator_base()
  {
    detach();
  }

  const safe_container_base* owner()const{return cont;}

BOOST_INDEXED_SET_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS:
  friend class safe_container_base;

#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
  template<typename Iterator> friend void safe_mode::detach_equivalent_iterators(Iterator&);
#endif

  inline void attach(safe_container_base* cont_);

  safe_container_base* cont;
  safe_iterator_base*  next;
};

class safe_container_base:private noncopyable
{
public:
  safe_container_base(){}

  ~safe_container_base()
  {
    for(safe_iterator_base* it=header.next;it;it=it->next)it->cont=0;
  }

  void swap(safe_container_base& x)
  {
    for(safe_iterator_base* it0=header.next;it0;it0=it0->next)it0->cont=&x;
    for(safe_iterator_base* it1=x.header.next;it1;it1=it1->next)it1->cont=this;
    std::swap(header.cont,x.header.cont);
    std::swap(header.next,x.header.next);
  }

BOOST_INDEXED_SET_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS:
  friend class safe_iterator_base;

#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
  template<typename Iterator> friend void safe_mode::detach_equivalent_iterators(Iterator&);
#endif

  safe_iterator_base header;
};

void safe_iterator_base::attach(safe_container_base* cont_)
{
  cont=cont_;
  if(cont){
    next=cont->header.next;
    cont->header.next=this;
  }
}

void safe_iterator_base::detach()
{
  if(cont){
    safe_iterator_base *prev_,*next_;
    for(prev_=&cont->header;(next_=prev_->next)!=this;prev_=next_){}
    prev_->next=next;
    cont=0;
  }
}

template<typename Container>
class safe_container;

template<typename Container>
class safe_iterator:public safe_iterator_base
{
public:
  typedef Container container_type;

  safe_iterator():safe_iterator_base(){}
  explicit safe_iterator(safe_container<container_type>* cont_):safe_iterator_base(cont_){}

  const container_type* owner()const
  {
    return
      static_cast<const container_type*>(
        static_cast<const safe_container<container_type>*>(safe_iterator_base::owner()));
  }
};

template<typename Container>
class safe_container:public safe_container_base
{
public:
  void swap(safe_container<Container>& x){safe_container_base::swap(x);}
};

} /* namespace indexed_sets::detail */

namespace safe_mode{

/* checking routines */

template<typename Iterator>
inline bool check_valid_iterator(const Iterator& it)
{
  return it.valid();
}

template<typename Iterator>
inline bool check_dereferenceable_iterator(const Iterator& it)
{
  return it.valid()&&it!=it.owner()->end();
}

template<typename Iterator>
inline bool check_incrementable_iterator(const Iterator& it)
{
  return it.valid()&&it!=it.owner()->end();
}

template<typename Iterator>
inline bool check_decrementable_iterator(const Iterator& it)
{
  return it.valid()&&it!=it.owner()->begin();
}

template<typename Iterator>
inline bool check_is_owner(const Iterator& it,const typename Iterator::container_type& cont)
{
  return it.valid()&&it.owner()==&cont;
}

template<typename Iterator>
inline bool check_same_owner(const Iterator& it0,const Iterator& it1)
{
  return it0.valid()&&it1.valid()&&it0.owner()==it1.owner();
}

template<typename Iterator>
inline bool check_valid_range(const Iterator& it0,const Iterator& it1)
{
  if(!it0.valid()||!it1.valid()||it0.owner()!=it1.owner())return false;

  Iterator last=it0.owner()->end();
  if(it1==last)return true;

  for(Iterator first=it0;first!=last;++first){
    if(first==it1)return true;
  }
  return false;
}

} /* namespace indexed_sets::safe_mode */

} /* namespace indexed_sets */

} /* namespace boost */

#endif /* BOOST_INDEXED_SET_ENABLE_SAFE_MODE */

/* assertion macros */

#if !defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#undef BOOST_INDEXED_SET_SAFE_MODE_ASSERT
#define BOOST_INDEXED_SET_SAFE_MODE_ASSERT(expr,error_code) ((void)0)
#else
#if !defined(BOOST_INDEXED_SET_SAFE_MODE_ASSERT)
#include <boost/assert.hpp>
#define BOOST_INDEXED_SET_SAFE_MODE_ASSERT(expr,error_code) BOOST_ASSERT(expr)
#endif
#endif

#define BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_valid_iterator(it),\
    safe_mode::invalid_iterator);

#define BOOST_INDEXED_SET_CHECK_DEREFERENCEABLE_ITERATOR(it) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_dereferenceable_iterator(it),\
    safe_mode::not_dereferenceable_iterator);

#define BOOST_INDEXED_SET_CHECK_INCREMENTABLE_ITERATOR(it) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_incrementable_iterator(it),\
    safe_mode::not_incrementable_iterator);

#define BOOST_INDEXED_SET_CHECK_DECREMENTABLE_ITERATOR(it) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_decrementable_iterator(it),\
    safe_mode::not_decrementable_iterator);

#define BOOST_INDEXED_SET_CHECK_IS_OWNER(it,cont) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_is_owner(it,cont),\
    safe_mode::not_owner);

#define BOOST_INDEXED_SET_CHECK_SAME_OWNER(it0,it1) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_same_owner(it0,it1),\
    safe_mode::not_same_owner);

#define BOOST_INDEXED_SET_CHECK_VALID_RANGE(it0,it1) \
  BOOST_INDEXED_SET_SAFE_MODE_ASSERT(\
    safe_mode::check_valid_range(it0,it1),\
    safe_mode::invalid_range);

#endif
