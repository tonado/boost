/* Copyright Joaqu�n M L�pez Mu�oz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_SEQUENCED_INDEX_HPP
#define BOOST_INDEXED_SET_SEQUENCED_INDEX_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/call_traits.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/indexed_set/access_specifier.hpp>
#include <boost/indexed_set/index_iterator.hpp>
#include <boost/indexed_set/seq_index_node.hpp>
#include <boost/indexed_set/seq_index_ops.hpp>
#include <boost/indexed_set/safe_mode.hpp>
#include <boost/indexed_set/scope_guard.hpp>
#include <boost/indexed_set/try_catch.hpp>
#include <boost/indexed_set/tag.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/tuple/tuple.hpp>
#include <cstddef>
#include <functional>
#include <utility>

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
#define BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT \
  detail::scope_guard BOOST_JOIN(check_invariant_,__LINE__)=\
    detail::make_obj_guard(*this,&sequenced_index::check_invariant_);\
  BOOST_JOIN(check_invariant_,__LINE__).touch();
#else
#define BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT
#endif

namespace boost{

namespace indexed_sets{

namespace detail{

/* sequenced_index adds a layer of sequenced indexing to a given Super */

template<typename Super,typename TagList>

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
class sequenced_index:
  BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super,
  public index_proxy<sequenced_index_node<typename Super::node_type> >
#else
class sequenced_index:
  BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super,
  public safe_container<sequenced_index<Super,TagList> >
#endif
#else
class sequenced_index:BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super
#endif

{ 
protected:
  typedef sequenced_index_node<typename Super::node_type> node_type;

public:
  /* types */

  typedef typename node_type::value_type             value_type;
  typedef tuples::null_type                          ctor_args;
  typedef typename Super::final_allocator_type       allocator_type;
  typedef typename allocator_type::reference         reference;
  typedef typename allocator_type::const_reference   const_reference;

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  typedef index_iterator<node_type>                  iterator;
  typedef index_iterator<node_type>                  const_iterator;
#else
  typedef index_iterator<node_type,sequenced_index>  iterator;
  typedef index_iterator<node_type,sequenced_index>  const_iterator;
#endif
#else
  typedef index_iterator<node_type>                  iterator;
  typedef index_iterator<node_type>                  const_iterator;
#endif

  typedef std::size_t                                size_type;      
  typedef std::ptrdiff_t                             difference_type;
  typedef typename allocator_type::pointer           pointer;
  typedef typename allocator_type::const_pointer     const_pointer;
  typedef typename
    boost::reverse_iterator<iterator>                reverse_iterator;
  typedef typename
    boost::reverse_iterator<const_iterator>          const_reverse_iterator;
  typedef typename TagList::type                     tag_list;

protected:
  typedef typename Super::final_node_type               final_node_type;
  typedef tuples::cons<
    ctor_args, 
    typename Super::ctor_args_list>                     ctor_args_list;
  typedef typename mpl::push_front<
    typename Super::index_type_list,
    sequenced_index>::type                              index_type_list;
  typedef typename mpl::push_front<
    typename Super::iterator_type_list,iterator>::type  iterator_type_list;
  typedef typename mpl::push_front<
    typename Super::const_iterator_type_list,
    const_iterator>::type                               const_iterator_type_list;
  typedef typename Super::copy_map_type                 copy_map_type;

private:
#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
  typedef index_proxy<sequenced_index_node<
      typename Super::node_type> >           safe_super;
#else
  typedef safe_container<sequenced_index>    safe_super;
#endif
#endif

  typedef typename call_traits<value_type>::param_type value_param_type;

public:

  /* construct/copy/destroy
   * Default and copy ctors are in the protected section as indices are
   * not supposed to be created on their own. No range ctor either.
   */

  sequenced_index<Super,TagList>& operator=(const sequenced_index<Super,TagList>& x)
  {
    final()=x.final();
    return *this;
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template <class InputIterator>
  void assign(InputIterator first,InputIterator last)
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    erase(begin(),end());
    for(;first!=last;++first)push_back(*first);
  }
#endif

  void assign(size_type n,value_param_type value)
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    erase(begin(),end());
    for(size_type i=0;i<n;++i)push_back(value);
  }
    
  allocator_type get_allocator()const
  {
    return final().get_allocator();
  }

  /* iterators */

  iterator               begin()
                           {return make_iterator(node_type::from_impl(header()->next()));}
  const_iterator         begin()const
                           {return make_iterator(node_type::from_impl(header()->next()));}
  iterator               end(){return make_iterator(header());}
  const_iterator         end()const{return make_iterator(header());}
  reverse_iterator       rbegin(){return make_reverse_iterator(end());}
  const_reverse_iterator rbegin()const{return make_reverse_iterator(end());}
  reverse_iterator       rend(){return make_reverse_iterator(begin());}
  const_reverse_iterator rend()const{return make_reverse_iterator(begin());}

  /* capacity */

  bool      empty()const{return final_empty_();}
  size_type size()const{return final_size_();}
  size_type max_size()const{return final_max_size_();}

  /* access: no need to provide non-const versions as
   * sequenced_index::iterator==index::const_iterator
   */

  const_reference front()const{return *begin();}
  const_reference back()const{return *--end();}

  /* modifiers */

  std::pair<iterator,bool> push_front(value_param_type x){return insert(begin(),x);}
  void                     pop_front(){erase(begin());}
  std::pair<iterator,bool> push_back(value_param_type x){return insert(end(),x);}
  void                     pop_back(){erase(--end());}

  std::pair<iterator,bool> insert(iterator position,value_param_type x)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=final_insert_(x);
    if(p.second)relink(position.get_node(),p.first);
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename InputIterator>
  void insert(InputIterator first,InputIterator last)
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    for(;first!=last;++first)push_back(*first);
  }
#endif

  void erase(iterator position)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    final_erase_(static_cast<final_node_type*>(position.get_node()));
  }
  
  void erase(iterator first,iterator last)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(first);
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(last);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(first,*this);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(last,*this);
    BOOST_INDEXED_SET_CHECK_VALID_RANGE(first,last);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    while(first!=last){
      erase(first++);
    }
  }

  bool update(iterator position,value_param_type x)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    return final_update_(x,static_cast<final_node_type*>(position.get_node()));
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename Modifier>
  bool modify(iterator position,Modifier mod)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_DEREFERENCEABLE_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    return final_modify_(mod,static_cast<final_node_type*>(position.get_node()));
  }
#endif

  void swap(sequenced_index<Super,TagList>& x)
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    final_swap_(x.final());
  }

  void clear()
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    erase(begin(),end());
  }

  /* list operations */

  void splice(iterator position,sequenced_index<Super,TagList>& x)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_CHECK_DIFFERENT_CONTAINER(*this,x);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    iterator first=x.begin(),last=x.end();
    while(first!=last){
      if(insert(position,*first).second)x.erase(first++);
      else ++first;
    }
  }

  void splice(iterator position,sequenced_index<Super,TagList>& x,iterator i)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(i);
    BOOST_INDEXED_SET_CHECK_DEREFERENCEABLE_ITERATOR(i);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(i,x);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    if(x==*this){
      if(position!=i)relink(position.get_node(),i.get_node());
    }
    else{
      if(insert(position,*i).second){

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer has a hard time with safe mode, and the following
     * workaround is needed. Left it for all compilers as it does no
     * harm.
     */
        i.detach();
        x.erase(x.make_iterator(i.get_node()));
#else
        x.erase(i);
#endif

      }
    }
  }

  void splice(
    iterator position,sequenced_index<Super,TagList>& x,
    iterator first,iterator last)
  {
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(position);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(position,*this);
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(first);
    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(last);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(first,x);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(last,x);
    BOOST_INDEXED_SET_CHECK_VALID_RANGE(first,last);
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    if(x==*this){
      BOOST_INDEXED_SET_CHECK_OUTSIDE_RANGE(position,first,last);
      if(position!=last)relink(position.get_node(),first.get_node(),last.get_node());
    }
    else{
      while(first!=last){
        if(insert(position,*first).second)x.erase(first++);
        else ++first;
      }
    }
  }

  void remove(value_param_type value)
  {
    sequenced_index_remove(*this,std::bind2nd(std::equal_to<value_type>(),value));
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename Predicate>
  void remove_if(Predicate pred)
  {
    sequenced_index_remove(*this,pred);
  }
#endif

  void unique()
  {
    sequenced_index_unique(*this,std::equal_to<value_type>());
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred)
  {
    sequenced_index_unique(*this,binary_pred);
  }
#endif

  void merge(sequenced_index<Super,TagList>& x)
  {
    sequenced_index_merge(*this,x,std::less<value_type>());
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template <typename Compare>
  void merge(sequenced_index<Super,TagList>& x,Compare comp)
  {
    sequenced_index_merge(*this,x,comp);
  }
#endif

  void sort()
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_sort(header(),std::less<value_type>(),get_allocator());
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template <typename Compare>
  void sort(Compare comp)
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_sort(header(),comp,get_allocator());
  }
#endif

  void reverse()
  {
    BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_node_impl::reverse(header()->impl());
  }
    
BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:
  sequenced_index(const ctor_args_list& args_list,const allocator_type& al):
    Super(args_list.get_tail(),al)

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)&&BOOST_WORKAROUND(BOOST_MSVC,<1300)
    ,safe_super(final_header())
#endif

  {
    header()->prior()=header()->next()=header()->impl();
  }

  sequenced_index(const sequenced_index<Super,TagList>& x):
    Super(x)

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)&&BOOST_WORKAROUND(BOOST_MSVC,<1300)
    ,safe_super(final_header())
#endif

  {
    /* Copy ctor just takes the compare objects from x. The rest is done in
     * subsequent call to copy_().
     */
  }

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
  iterator       make_iterator(node_type* node){return iterator(node,this);}
  const_iterator make_iterator(node_type* node)const
    {return const_iterator(node,const_cast<sequenced_index*>(this));}
#else
  iterator       make_iterator(node_type* node){return iterator(node);}
  const_iterator make_iterator(node_type* node)const{return const_iterator(node);}
#endif

  void copy_(const sequenced_index<Super,TagList>& x,const copy_map_type& map)
  {
    node_type* org=x.header();
    node_type* cpy=header();
    do{
      node_type* next_org=node_type::from_impl(org->next());
      node_type* next_cpy=map.find(static_cast<final_node_type*>(next_org));
      cpy->next()=next_cpy->impl();
      next_cpy->prior()=cpy->impl();
      org=next_org;
      cpy=next_cpy;
    }while(org!=x.header());

    Super::copy_(x,map);
  }

  node_type* insert_(value_param_type v,node_type* x)
  {
    node_type* res=static_cast<node_type*>(Super::insert_(v,x));
    if(res==x)link(x);
    return res;
  }

  node_type* insert_(value_param_type v,node_type* position,node_type* x)
  {
    node_type* res=static_cast<node_type*>(Super::insert_(v,position,x));
    if(res==x)link(x);
    return res;
  }

  void erase_(node_type* x)
  {
    unlink(x);
    Super::erase_(x);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
    detach_iterators(x);
#endif
  }

  void swap_(sequenced_index<Super,TagList>& x)
  {
#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
    safe_super::swap(x);
#endif

    Super::swap_(x);
  }

  bool update_(value_param_type v,node_type* x)
  {
    return Super::update_(v,x);
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  bool modify_(node_type* x)
  {
    BOOST_INDEXED_SET_TRY{
      if(!Super::modify_(x)){
        unlink(x);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
        detach_iterators(x);
#endif

        return false;
      }
      else return true;
    }
    BOOST_INDEXED_SET_CATCH(...){
      unlink(x);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
      detach_iterators(x);
#endif

      BOOST_INDEXED_SET_RETHROW;
    }

    return true;
  }
#endif

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const
  {
    if(size()==0||begin()==end()){
      if(size()!=0||begin()!=end()||
         header()->next()!=header()->impl()||
         header()->prior()!=header()->impl())return false;
    }
    else{
      size_type s=0;
      for(const_iterator it=begin();it!=end();++it,++s){
        if(it.get_node()->next()->prior()!=it.get_node()->impl())return false;
        if(it.get_node()->prior()->next()!=it.get_node()->impl())return false;
      }
      if(s!=size())return false;
    }

    return Super::invariant_();
  }

  /* This forwarding function eases things for the boost::mem_fn construct
   * in BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT. Actually, final_check_invariant
   * is already an inherited member function of index.
   */
  void check_invariant_()const{final_check_invariant_();}
#endif

private:
  node_type* header()const{return final_header();}

  void link(node_type* x)
  {
    sequenced_index_node_impl::link(x->impl(),header()->impl());
  };

  static void unlink(node_type* x)
  {
    sequenced_index_node_impl::unlink(x->impl());
  }

  static void relink(node_type* position,node_type* x)
  {
    sequenced_index_node_impl::relink(position->impl(),x->impl());
  }

  static void relink(node_type* position,node_type* first,node_type* last)
  {
    sequenced_index_node_impl::relink(position->impl(),first->impl(),last->impl());
  }

  static void swap(node_type* x,node_type* y)
  {
    sequenced_index_node_impl::swap(x->impl(),y->impl());
  }

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
  void detach_iterators(node_type* x)
  {
    iterator it=make_iterator(x);
    safe_mode::detach_equivalent_iterators(it);
  }
#endif
};

/* comparison */

template<typename Super,typename TagList>
bool operator==(
  const sequenced_index<Super,TagList>& x,
  const sequenced_index<Super,TagList>& y)
{
  return x.size()==y.size()&&std::equal(x.begin(),x.end(),y.begin());
}

template<typename Super,typename TagList>
bool operator<(
  const sequenced_index<Super,TagList>& x,
  const sequenced_index<Super,TagList>& y)
{
  return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}

template<typename Super,typename TagList>
bool operator!=(
  const sequenced_index<Super,TagList>& x,
  const sequenced_index<Super,TagList>& y)
{
  return !(x==y);
}

template<typename Super,typename TagList>
bool operator>(
  const sequenced_index<Super,TagList>& x,
  const sequenced_index<Super,TagList>& y)
{
  return y<x;
}

template<typename Super,typename TagList>
bool operator>=(
  const sequenced_index<Super,TagList>& x,
  const sequenced_index<Super,TagList>& y)
{
  return !(x<y);
}

template<typename Super,typename TagList>
bool operator<=(
  const sequenced_index<Super,TagList>& x,
  const sequenced_index<Super,TagList>& y)
{
  return !(x>y);
}

/*  specialized algorithms */

template<typename Super,typename TagList>
void swap(
  sequenced_index<Super,TagList>& x,
  sequenced_index<Super,TagList>& y)
{
  x.swap(y);
}

} /* namespace indexed_sets::detail */

/* sequenced index specifier */

template <typename TagList=tag<> >
struct sequenced
{
  BOOST_STATIC_ASSERT(detail::is_tag<TagList>::value);

  template<typename Super>
  struct node_type
  {
    typedef detail::sequenced_index_node<Super> type;
  };

  template<typename Super>
  struct index_class_type
  {
    typedef detail::sequenced_index<Super,TagList> type;
  };
};

} /* namespace indexed_sets */

} /* namespace boost */

#undef BOOST_INDEXED_SET_SEQ_INDEX_CHECK_INVARIANT

#endif
