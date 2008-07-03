/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2007
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_SLIST_HPP
#define BOOST_INTRUSIVE_SLIST_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/static_assert.hpp>
#include <boost/intrusive/detail/assert.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/slist_hook.hpp>
#include <boost/intrusive/circular_slist_algorithms.hpp>
#include <boost/intrusive/linear_slist_algorithms.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <iterator>
#include <functional>
#include <algorithm>
#include <cstddef>   //std::size_t
#include <utility>   //std::pair

namespace boost {
namespace intrusive {

/// @cond

template <class T>
struct internal_default_slist_hook
{
   template <class U> static detail::one test(...);
   template <class U> static detail::two test(typename U::default_slist_hook* = 0);
   static const bool value = sizeof(test<T>(0)) == sizeof(detail::two);
};

template <class T>
struct get_default_slist_hook
{  typedef typename T::default_slist_hook type; };

template <class ValueTraits, class SizeType, bool ConstantTimeSize, bool Linear, bool CacheLast>
struct slistopt
{
   typedef ValueTraits  value_traits;
   typedef SizeType     size_type;
   static const bool constant_time_size   = ConstantTimeSize;
   static const bool linear               = Linear;
   static const bool cache_last           = CacheLast;
};

template<class Node, class NodePtr, bool>
struct root_plus_last
{
   Node     root_;
   NodePtr  last_;
};

template<class Node, class NodePtr>
struct root_plus_last<Node, NodePtr, false>
{
   Node root_;
};

template <class T>
struct slist_defaults
   :  pack_options
      < none
      , base_hook
         <  typename detail::eval_if_c
               < internal_default_slist_hook<T>::value
               , get_default_slist_hook<T>
               , detail::identity<none>
               >::type
         >
      , constant_time_size<true>
      , linear<false>
      , size_type<std::size_t>
      , cache_last<false>
      >::type
{};

/// @endcond

//! The class template slist is an intrusive container, that encapsulates 
//! a singly-linked list. You can use such a list to squeeze the last bit 
//! of performance from your application. Unfortunately, the little gains 
//! come with some huge drawbacks. A lot of member functions can't be 
//! implemented as efficiently as for standard containers. To overcome 
//! this limitation some other member functions with rather unusual semantics 
//! have to be introduced.
//!
//! The template parameter \c T is the type to be managed by the container.
//! The user can specify additional options and if no options are provided
//! default options are used.
//!
//! The container supports the following options:
//! \c base_hook<>/member_hook<>/value_traits<>,
//! \c constant_time_size<>, \c size_type<>,
//! \c linear<> and \c cache_last<>.
//! 
//! The iterators of slist are forward iterators. slist provides a static 
//! function called "previous" to compute the previous iterator of a given iterator. 
//! This function has linear complexity. To improve the usability esp. with 
//! the '*_after' functions, ++end() == begin() and previous(begin()) == end() 
//! are defined. An new special function "before_begin()" is defined, which returns
//! an iterator that points one less the beginning of the list: ++before_begin() == begin()
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
class slist_impl
{
   //Public typedefs
   public:
   typedef typename Config::value_traits                             value_traits;
   /// @cond
   static const bool external_value_traits =
      detail::external_value_traits_is_true<value_traits>::value;
   typedef typename detail::eval_if_c
      < external_value_traits
      , detail::eval_value_traits<value_traits>
      , detail::identity<value_traits>
      >::type                                                        real_value_traits;
   /// @endcond
   typedef typename real_value_traits::pointer                       pointer;
   typedef typename real_value_traits::const_pointer                 const_pointer;
   typedef typename std::iterator_traits<pointer>::value_type        value_type;
   typedef typename std::iterator_traits<pointer>::reference         reference;
   typedef typename std::iterator_traits<const_pointer>::reference   const_reference;
   typedef typename std::iterator_traits<pointer>::difference_type   difference_type;
   typedef typename Config::size_type                                size_type;
   typedef slist_iterator<slist_impl, false>                         iterator;
   typedef slist_iterator<slist_impl, true>                          const_iterator;
   typedef typename real_value_traits::node_traits                   node_traits;
   typedef typename node_traits::node                                node;
   typedef typename boost::pointer_to_other
      <pointer, node>::type                                          node_ptr;
   typedef typename boost::pointer_to_other
      <pointer, const node>::type                                    const_node_ptr;
   typedef typename detail::if_c
      < Config::linear
      , linear_slist_algorithms<node_traits>
      , circular_slist_algorithms<node_traits>
      >::type                                                        node_algorithms;

   static const bool constant_time_size = Config::constant_time_size;
   static const bool stateful_value_traits = detail::store_cont_ptr_on_it<slist_impl>::value;
   static const bool linear = Config::linear;
   static const bool cache_last = Config::cache_last;

   /// @cond
   private:
   typedef detail::size_holder<constant_time_size, size_type>        size_traits;

   //! This class is
   //! non-copyable
   slist_impl (const slist_impl&);

   //! This class is
   //! non-asignable
   slist_impl &operator =(const slist_impl&);

   enum { safemode_or_autounlink  = 
            (int)real_value_traits::link_mode == (int)auto_unlink   ||
            (int)real_value_traits::link_mode == (int)safe_link     };

   //Constant-time size is incompatible with auto-unlink hooks!
   BOOST_STATIC_ASSERT(!(constant_time_size && ((int)real_value_traits::link_mode == (int)auto_unlink)));
   //Linear singly linked lists are incompatible with auto-unlink hooks!
   BOOST_STATIC_ASSERT(!(linear && ((int)real_value_traits::link_mode == (int)auto_unlink)));
   //A list with cached last node is incompatible with auto-unlink hooks!
   BOOST_STATIC_ASSERT(!(cache_last && ((int)real_value_traits::link_mode == (int)auto_unlink)));

   node_ptr get_end_node()
   {  return node_ptr(linear ? node_ptr(0) : this->get_root_node());  }

   const_node_ptr get_end_node() const
   {
      return const_node_ptr
         (linear ? const_node_ptr(0) : this->get_root_node());  }

   node_ptr get_root_node()
   {  return node_ptr(&data_.root_plus_size_.root_);  }

   const_node_ptr get_root_node() const
   {  return const_node_ptr(&data_.root_plus_size_.root_);  }

   node_ptr get_last_node()
   {  return this->get_last_node(detail::bool_<cache_last>());  }

   const_node_ptr get_last_node() const
   {  return this->get_last_node(detail::bool_<cache_last>());  }

   void set_last_node(node_ptr n)
   {  return this->set_last_node(n, detail::bool_<cache_last>());  }

   static node_ptr get_last_node(detail::bool_<false>)
   {  return node_ptr(0);  }

   static void set_last_node(node_ptr, detail::bool_<false>)
   {}

   node_ptr get_last_node(detail::bool_<true>)
   {  return node_ptr(data_.root_plus_size_.last_);  }

   const_node_ptr get_last_node(detail::bool_<true>) const
   {  return const_node_ptr(data_.root_plus_size_.last_);  }

   void set_last_node(node_ptr n, detail::bool_<true>)
   {  data_.root_plus_size_.last_ = n;  }

   static node_ptr uncast(const_node_ptr ptr)
   {  return node_ptr(const_cast<node*>(detail::get_pointer(ptr)));  }

   void set_default_constructed_state()
   {
      node_algorithms::init_header(this->get_root_node());
      this->priv_size_traits().set_size(size_type(0));
      if(cache_last){
         this->set_last_node(this->get_root_node());
      }
   }

   struct root_plus_size
      :  public size_traits
      ,  public root_plus_last<node, node_ptr, cache_last>
   {};

   struct data_t
      :  public slist_impl::value_traits
   {
      typedef typename slist_impl::value_traits value_traits;
      data_t(const value_traits &val_traits)
         :  value_traits(val_traits)
      {}

      root_plus_size root_plus_size_;
   } data_;

   size_traits &priv_size_traits()
   {  return data_.root_plus_size_;  }

   const size_traits &priv_size_traits() const
   {  return data_.root_plus_size_;  }

   const real_value_traits &get_real_value_traits(detail::bool_<false>) const
   {  return data_;  }

   const real_value_traits &get_real_value_traits(detail::bool_<true>) const
   {  return data_.get_value_traits(*this);  }

   real_value_traits &get_real_value_traits(detail::bool_<false>)
   {  return data_;  }

   real_value_traits &get_real_value_traits(detail::bool_<true>)
   {  return data_.get_value_traits(*this);  }

   /// @endcond

   public:

   const real_value_traits &get_real_value_traits() const
   {  return this->get_real_value_traits(detail::bool_<external_value_traits>());  }

   real_value_traits &get_real_value_traits()
   {  return this->get_real_value_traits(detail::bool_<external_value_traits>());  }

   public:
   //! <b>Effects</b>: constructs an empty list. 
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: If value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks).
   slist_impl(const value_traits &v_traits = value_traits())
      :  data_(v_traits)
   {  this->set_default_constructed_state(); }

   //! <b>Requires</b>: Dereferencing iterator must yield an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Constructs a list equal to [first,last).
   //! 
   //! <b>Complexity</b>: Linear in std::distance(b, e). No copy constructors are called.  
   //! 
   //! <b>Throws</b>: If value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks).
   template<class Iterator>
   slist_impl(Iterator b, Iterator e, const value_traits &v_traits = value_traits())
      :  data_(v_traits)
   {
      this->set_default_constructed_state();
      this->insert_after(this->before_begin(), b, e);
   }

   //! <b>Effects</b>: If it's a safe-mode
   //!   or auto-unlink value, the destructor does nothing
   //!   (ie. no code is generated). Otherwise it detaches all elements from this. 
   //!   In this case the objects in the list are not deleted (i.e. no destructors 
   //!   are called), but the hooks according to the value_traits template parameter
   //!   are set to their default value.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the list, if 
   //!   it's a safe-mode or auto-unlink value. Otherwise constant.
   ~slist_impl()
   {  this->clear(); }

   //! <b>Effects</b>: Erases all the elements of the container.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of the list.
   //!   if it's a safe-mode or auto-unlink value_type. Constant time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased elements.
   void clear()
   {
      if(safemode_or_autounlink){
         this->clear_and_dispose(detail::null_disposer()); 
      }
      else{
         this->set_default_constructed_state();
      }
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements of the container
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of the list.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased elements.
   template <class Disposer>
   void clear_and_dispose(Disposer disposer)
   {
      iterator it(this->begin()), itend(this->end());
      while(it != itend){
         node_ptr to_erase(it.pointed_node());
         ++it;
         if(safemode_or_autounlink)
            node_algorithms::init(to_erase);
         disposer(get_real_value_traits().to_value_ptr(to_erase));
      }
      this->set_default_constructed_state();
   }

   //! <b>Requires</b>: value must be an lvalue.
   //! 
   //! <b>Effects</b>: Inserts the value in the front of the list.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void push_front(reference value) 
   {
      node_ptr to_insert = get_real_value_traits().to_node_ptr(value);
      if(safemode_or_autounlink)
         BOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT(node_algorithms::inited(to_insert));
      if(cache_last){
         if(this->empty()){
            this->set_last_node(to_insert);
         }
      }
      node_algorithms::link_after(this->get_root_node(), to_insert); 
      this->priv_size_traits().increment();
   }

   //! <b>Requires</b>: value must be an lvalue.
   //! 
   //! <b>Effects</b>: Inserts the value in the back of the list.
   //!   No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   This function is only available is cache_last<> is true.
   void push_back(reference value) 
   {
      BOOST_STATIC_ASSERT((cache_last != 0));
      this->insert_after(iterator(this->get_last_node(), this), value);
   }

   //! <b>Effects</b>: Erases the first element of the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the erased element.
   void pop_front() 
   {  return this->pop_front_and_dispose(detail::null_disposer());   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the first element of the list.
   //!   Disposer::operator()(pointer) is called for the removed element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Disposer>
   void pop_front_and_dispose(Disposer disposer)
   {
      node_ptr to_erase = node_traits::get_next(this->get_root_node());
      node_algorithms::unlink_after(this->get_root_node());
      this->priv_size_traits().decrement();
      if(safemode_or_autounlink)
         node_algorithms::init(to_erase);
      disposer(get_real_value_traits().to_value_ptr(to_erase));
      if(cache_last){
         if(this->empty()){
            this->set_last_node(this->get_root_node());
         }
      }
   }

   //! <b>Effects</b>: Returns a reference to the first element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   reference front()
   { return *this->get_real_value_traits().to_value_ptr(node_traits::get_next(this->get_root_node())); }

   //! <b>Effects</b>: Returns a const_reference to the first element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_reference front() const
   { return *this->get_real_value_traits().to_value_ptr(uncast(node_traits::get_next(this->get_root_node()))); }

   //! <b>Effects</b>: Returns a reference to the last element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   This function is only available is cache_last<> is true.
   reference back()
   {
      BOOST_STATIC_ASSERT((cache_last != 0));
      return *this->get_real_value_traits().to_value_ptr(this->get_last_node());
   }

   //! <b>Effects</b>: Returns a const_reference to the last element of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   //!   This function is only available is cache_last<> is true.
   const_reference back() const
   {
      BOOST_STATIC_ASSERT((cache_last != 0));
      return *this->get_real_value_traits().to_value_ptr(this->get_last_node());
   }

   //! <b>Effects</b>: Returns an iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator begin() 
   { return iterator (node_traits::get_next(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator begin() const 
   { return const_iterator (node_traits::get_next(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cbegin() const 
   { return const_iterator(node_traits::get_next(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns an iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator end() 
   { return iterator(this->get_end_node(), this); }

   //! <b>Effects</b>: Returns a const_iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator end() const 
   { return const_iterator(uncast(this->get_end_node()), this); }

   //! <b>Effects</b>: Returns a const_iterator to the end of the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cend() const 
   { return this->end(); }

   //! <b>Effects</b>: Returns an iterator that points to a position
   //!   before the first element. Equivalent to "end()"
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   iterator before_begin() 
   { return iterator(this->get_root_node(), this); }

   //! <b>Effects</b>: Returns an iterator that points to a position
   //!   before the first element. Equivalent to "end()"
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator before_begin() const 
   { return const_iterator(uncast(this->get_root_node()), this); }

   //! <b>Effects</b>: Returns an iterator that points to a position
   //!   before the first element. Equivalent to "end()"
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   const_iterator cbefore_begin() const 
   { return this->before_begin(); }

   //! <b>Precondition</b>: end_iterator must be a valid end iterator
   //!   of slist.
   //! 
   //! <b>Effects</b>: Returns a const reference to the slist associated to the end iterator
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   static slist_impl &container_from_end_iterator(iterator end_iterator)
   {  return slist_impl::priv_container_from_end_iterator(end_iterator);   }

   //! <b>Precondition</b>: end_iterator must be a valid end const_iterator
   //!   of slist.
   //! 
   //! <b>Effects</b>: Returns a const reference to the slist associated to the end iterator
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   static const slist_impl &container_from_end_iterator(const_iterator end_iterator)
   {  return slist_impl::priv_container_from_end_iterator(end_iterator);   }

   //! <b>Effects</b>: Returns the number of the elements contained in the list.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements contained in the list.
   //!   if constant_time_size is false. Constant time otherwise.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   size_type size() const
   {
      if(constant_time_size)
         return this->priv_size_traits().get_size();
      else
         return node_algorithms::count(this->get_root_node()) - 1; 
   }

   //! <b>Effects</b>: Returns true if the list contains no elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   bool empty() const
   { return node_algorithms::unique(this->get_root_node()); }

   //! <b>Effects</b>: Swaps the elements of x and *this.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements of both lists. 
   //!  Constant-time if linear<> and/or cache_last<> options are used.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void swap(slist_impl& other)
   {
      if(cache_last){
         this->priv_swap_cache_last(other);
      }
      else{
         this->priv_swap_lists(this->get_root_node(), other.get_root_node(), detail::bool_<linear>());
      }
      if(constant_time_size){
         size_type backup = this->priv_size_traits().get_size();
         this->priv_size_traits().set_size(other.priv_size_traits().get_size());
         other.priv_size_traits().set_size(backup);
      }
   }

   //! <b>Effects</b>: Moves backwards all the elements, so that the first
   //!   element becomes the second, the second becomes the third...
   //!   the last element becomes the first one.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements plus the number shifts.
   //! 
   //! <b>Note</b>: Iterators Does not affect the validity of iterators and references.
   void shift_backwards(size_type n = 1)
   {  this->priv_shift_backwards(n, detail::bool_<linear>());  }

   //! <b>Effects</b>: Moves forward all the elements, so that the second
   //!   element becomes the first, the third becomes the second...
   //!   the first element becomes the last one.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements plus the number shifts.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   void shift_forward(size_type n = 1)
   {  this->priv_shift_forward(n, detail::bool_<linear>()); }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases all the elements from *this
   //!   calling Disposer::operator()(pointer), clones all the 
   //!   elements from src calling Cloner::operator()(const_reference )
   //!   and inserts them on *this.
   //!
   //!   If cloner throws, all cloned elements are unlinked and disposed
   //!   calling Disposer::operator()(pointer).
   //!   
   //! <b>Complexity</b>: Linear to erased plus inserted elements.
   //! 
   //! <b>Throws</b>: If cloner throws.
   template <class Cloner, class Disposer>
   void clone_from(const slist_impl &src, Cloner cloner, Disposer disposer)
   {
      this->clear_and_dispose(disposer);
      detail::exception_disposer<slist_impl, Disposer>
         rollback(*this, disposer);
      iterator prev(this->before_begin());
      const_iterator b(src.begin()), e(src.end());
      for(; b != e; ++b){
         prev = this->insert_after(prev, *cloner(*b));
      }
      rollback.release();
   }

   //! <b>Requires</b>: value must be an lvalue and prev_p must point to an element
   //!   contained by the list or to end().
   //!
   //! <b>Effects</b>: Inserts the value after the position pointed by prev_p.
   //!    No copy constructor is called.
   //!
   //! <b>Returns</b>: An iterator to the inserted element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   iterator insert_after(iterator prev_p, reference value)
   {
      node_ptr n = get_real_value_traits().to_node_ptr(value);
      if(safemode_or_autounlink)
         BOOST_INTRUSIVE_SAFE_HOOK_DEFAULT_ASSERT(node_algorithms::inited(n));
      node_ptr prev_n(prev_p.pointed_node());
      node_algorithms::link_after(prev_n, n);
      if(cache_last && (this->get_last_node() == prev_n)){
         this->set_last_node(n);
      }
      this->priv_size_traits().increment();
      return iterator (n, this);
   }

   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type and prev_p must point to an element
   //!   contained by the list or to the end node.
   //! 
   //! <b>Effects</b>: Inserts the [first, last)
   //!   after the position prev_p.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted.
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   template<class Iterator>
   void insert_after(iterator prev_p, Iterator first, Iterator last)
   {
      for (; first != last; ++first)
         prev_p = this->insert_after(prev_p, *first);
   }

   //! <b>Requires</b>: value must be an lvalue and p must point to an element
   //!   contained by the list or to end().
   //!
   //! <b>Effects</b>: Inserts the value before the position pointed by p.
   //!   No copy constructor is called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements before p.
   //!  Constant-time if cache_last<> is true and p == end().
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   iterator insert(iterator p, reference value)
   {  return this->insert_after(this->previous(p), value);  }

   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type and p must point to an element 
   //!   contained by the list or to the end node.
   //! 
   //! <b>Effects</b>: Inserts the pointed by b and e
   //!   before the position p. No copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted plus linear
   //!   to the elements before b.
   //!   Linear to the number of elements to insert if cache_last<> option is true and p == end().
   //! 
   //! <b>Note</b>: Does not affect the validity of iterators and references.
   template<class Iterator>
   void insert(iterator p, Iterator b, Iterator e)
   {  return this->insert_after(this->previous(p), b, e);  }

   //! <b>Effects</b>: Erases the element after the element pointed by prev of 
   //!   the list. No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase_after(iterator prev)
   {  return this->erase_after_and_dispose(prev, detail::null_disposer());  }

   //! <b>Effects</b>: Erases the range (before_first, last) from
   //!   the list. No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of erased elements if it's a safe-mode
   //!   , auto-unlink value or constant-time size is activated. Constant time otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase_after(iterator before_first, iterator last)
   {
      if(safemode_or_autounlink || constant_time_size){
         return this->erase_after_and_dispose(before_first, last, detail::null_disposer());
      }
      else{
         node_ptr bfp = before_first.pointed_node();
         node_ptr lp = last.pointed_node();
         if(cache_last){
            if((lp == this->get_end_node())){
               this->set_last_node(bfp);
            }
         }
         node_algorithms::unlink_after(bfp, lp);
         return last;
      }
   }

   //! <b>Effects</b>: Erases the range (before_first, last) from
   //!   the list. n must be std::distance(before_first, last) - 1.
   //!   No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: constant-time if link_mode is normal_link. 
   //!   Linear to the elements (last - before_first) otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase_after(iterator before_first, iterator last, difference_type n)
   {
      BOOST_INTRUSIVE_INVARIANT_ASSERT(std::distance(++iterator(before_first), last) == difference_type(n));
      if(safemode_or_autounlink){
         return this->erase_after(before_first, last);
      }
      else{
         node_ptr bfp = before_first.pointed_node();
         node_ptr lp = last.pointed_node();
         if(cache_last){
            if((lp == this->get_end_node())){
               this->set_last_node(bfp);
            }
         }
         node_algorithms::unlink_after(bfp, lp);
         if(constant_time_size){
            this->priv_size_traits().set_size(this->priv_size_traits().get_size() - n);
         }
         return last;
      }
   }

   //! <b>Effects</b>: Erases the element pointed by i of the list. 
   //!   No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed element,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the elements before i.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase(iterator i)
   {  return this->erase_after(this->previous(i));  }

   //! <b>Requires</b>: first and last must be valid iterator to elements in *this.
   //! 
   //! <b>Effects</b>: Erases the range pointed by b and e.
   //!   No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the elements before last.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased elements.
   iterator erase(iterator first, iterator last)
   {  return this->erase_after(this->previous(first), last);  }

   //! <b>Effects</b>: Erases the range [first, last) from
   //!   the list. n must be std::distance(first, last).
   //!   No destructors are called.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: linear to the elements before first if link_mode is normal_link
   //!   and constant_time_size is activated. Linear to the elements before last otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   iterator erase(iterator first, iterator last, difference_type n)
   {  return this->erase_after(this->previous(first), last, n);  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element after the element pointed by prev of 
   //!   the list.
   //!   Disposer::operator()(pointer) is called for the removed element.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Disposer>
   iterator erase_after_and_dispose(iterator prev, Disposer disposer)
   {
      iterator it(prev);
      ++it;
      node_ptr to_erase(it.pointed_node());
      ++it;
      node_ptr prev_n(prev.pointed_node());
      node_algorithms::unlink_after(prev_n);
      if(cache_last && (to_erase == this->get_last_node())){
         this->set_last_node(prev_n);
      }
      if(safemode_or_autounlink)
         node_algorithms::init(to_erase);
      disposer(get_real_value_traits().to_value_ptr(to_erase));
      this->priv_size_traits().decrement();
      return it;
   }

   /// @cond

   template<class Disposer>
   static iterator s_erase_after_and_dispose(iterator prev, Disposer disposer)
   {
      BOOST_STATIC_ASSERT(((!cache_last)&&(!constant_time_size)&&(!stateful_value_traits)));
      iterator it(prev);
      ++it;
      node_ptr to_erase(it.pointed_node());
      ++it;
      node_ptr prev_n(prev.pointed_node());
      node_algorithms::unlink_after(prev_n);
      if(safemode_or_autounlink)
         node_algorithms::init(to_erase);
      disposer(real_value_traits::to_value_ptr(to_erase));
      return it;
   }

   static iterator s_erase_after(iterator prev)
   {  return s_erase_after_and_dispose(prev, detail::null_disposer());  }

   /// @endcond

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the range (before_first, last) from
   //!   the list.
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Lineal to the elements (last - before_first + 1).
   //! 
   //! <b>Note</b>: Invalidates the iterators to the erased element.
   template<class Disposer>
   iterator erase_after_and_dispose(iterator before_first, iterator last, Disposer disposer)
   {
      node_ptr bfp(before_first.pointed_node()), lp(last.pointed_node());
      node_ptr fp(node_traits::get_next(bfp));
      node_algorithms::unlink_after(bfp, lp);
      while(fp != lp){
         node_ptr to_erase(fp);
         fp = node_traits::get_next(fp);
         if(safemode_or_autounlink)
            node_algorithms::init(to_erase);
         disposer(get_real_value_traits().to_value_ptr(to_erase));
         this->priv_size_traits().decrement();
      }
      if(cache_last && (node_traits::get_next(bfp) == this->get_end_node())){
         this->set_last_node(bfp);
      }
      return last;
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Erases the element pointed by i of the list. 
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed element.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed element,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the elements before i.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased element.
   template<class Disposer>
   iterator erase_and_dispose(iterator i, Disposer disposer)
   {  return this->erase_after_and_dispose(this->previous(i), disposer);  }

   //! <b>Requires</b>: first and last must be valid iterator to elements in *this.
   //!                  Disposer::operator()(pointer) shouldn't throw.
   //! 
   //! <b>Effects</b>: Erases the range pointed by b and e.
   //!   No destructors are called.
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //!
   //! <b>Returns</b>: the first element remaining beyond the removed elements,
   //!   or end() if no such element exists.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of erased elements plus linear
   //!   to the elements before first.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references) to the
   //!   erased elements.
   template<class Disposer>
   iterator erase_and_dispose(iterator first, iterator last, Disposer disposer)
   {  return this->erase_after_and_dispose(this->previous(first), last, disposer);  }

   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Clears the list and inserts the range pointed by b and e.
   //!   No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted plus
   //!   linear to the elements contained in the list if it's a safe-mode
   //!   or auto-unlink value.
   //!   Linear to the number of elements inserted in the list otherwise.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!   to the erased elements.
   template<class Iterator>
   void assign(Iterator b, Iterator e)
   {
      this->clear();
      this->insert_after(this->before_begin(), b, e);
   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Requires</b>: Dereferencing iterator must yield 
   //!   an lvalue of type value_type.
   //! 
   //! <b>Effects</b>: Clears the list and inserts the range pointed by b and e.
   //!   No destructors or copy constructors are called.
   //!   Disposer::operator()(pointer) is called for the removed elements.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements inserted plus
   //!   linear to the elements contained in the list.
   //! 
   //! <b>Note</b>: Invalidates the iterators (but not the references)
   //!   to the erased elements.
   template<class Iterator, class Disposer>
   void dispose_and_assign(Disposer disposer, Iterator b, Iterator e)
   {
      this->clear_and_dispose(disposer);
      this->insert_after(this->before_begin(), b, e, disposer);
   }

   //! <b>Requires</b>: prev is an iterator to an element or x.end()/x.before_begin() in x.
   //! 
   //! <b>Effects</b>: Transfers all the elements of list x to this list, after the
   //! the element pointed by prev. No destructors or copy constructors are called.
   //! 
   //! <b>Returns</b>: The last element inserted of x or prev if x is empty.
   //!   This iterator can be used as new "prev" iterator for a new splice_after call.
   //!   that will splice new values after the previously spliced values.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Linear to the elements contained in x.
   //!   Constant-time if cache_last<> option is true.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //! list. Iterators of this list and all the references are not invalidated.
   iterator splice_after(iterator prev, slist_impl &x)
   {
      if (!x.empty()){
         iterator last_x(x.previous(x.end()));  //<- constant time if cache_last is active
         node_ptr prev_n(prev.pointed_node());
         node_ptr last_x_n(last_x.pointed_node());
         if(cache_last){
            x.set_last_node(x.get_root_node());
            if(node_traits::get_next(prev_n) == this->get_end_node()){
               this->set_last_node(last_x_n);
            }
         }
         node_algorithms::transfer_after( prev_n, x.before_begin().pointed_node(), last_x_n);
         this->priv_size_traits().set_size(this->priv_size_traits().get_size() + x.priv_size_traits().get_size());
         x.priv_size_traits().set_size(size_type(0));
         return last_x;
      }
      else{
         return prev;
      }
   }

   //! <b>Requires</b>: prev must point to an element contained by this list or
   //!   to the before_begin() element. prev_ele must point to an element contained in list
   //!   x or must be x.before_begin().
   //! 
   //! <b>Effects</b>: Transfers the element after prev_ele, from list x to this list, 
   //!   after the element pointed by prev. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //! list. Iterators of this list and all the references are not invalidated.
   void splice_after(iterator prev_pos, slist_impl &x, iterator prev_ele)
   {
      iterator elem = prev_ele;
      this->splice_after(prev_pos, x, prev_ele, ++elem, 1);
   }

   //! <b>Requires</b>: prev_pos must be a dereferenceable iterator in *this or be
   //!   before_begin(), and before_first and before_last belong to x and
   //!   ++before_first != x.end() && before_last != x.end(). 
   //! 
   //! <b>Effects</b>: Transfers the range (before_first, before_last] from list x to this
   //!   list, after the element pointed by prev_pos.
   //!   No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements transferred
   //!   if constant_time_size is true. Constant-time otherwise.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice_after(iterator prev_pos, slist_impl &x, iterator before_first, iterator before_last)
   {
      if(constant_time_size)
         this->splice_after(prev_pos, x, before_first, before_last, std::distance(before_first, before_last));
      else
         this->priv_splice_after
            (prev_pos.pointed_node(), x, before_first.pointed_node(), before_last.pointed_node());
   }

   //! <b>Requires</b>: prev_pos must be a dereferenceable iterator in *this or be
   //!   before_begin(), and before_first and before_last belong to x and
   //!   ++before_first != x.end() && before_last != x.end() and
   //!   n == std::distance(before_first, before_last).
   //! 
   //! <b>Effects</b>: Transfers the range (before_first, before_last] from list x to this
   //!   list, after the element pointed by p. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice_after(iterator prev_pos, slist_impl &x, iterator before_first, iterator before_last, difference_type n)
   {
      if(n){
         BOOST_INTRUSIVE_INVARIANT_ASSERT(std::distance(before_first, before_last) == n);
         this->priv_splice_after
            (prev_pos.pointed_node(), x, before_first.pointed_node(), before_last.pointed_node());
         if(constant_time_size){
            this->priv_size_traits().set_size(this->priv_size_traits().get_size() + n);
            x.priv_size_traits().set_size(x.priv_size_traits().get_size() - n);
         }
      }
   }

   //! <b>Requires</b>: it is an iterator to an element in x.
   //! 
   //! <b>Effects</b>: Transfers all the elements of list x to this list, before the
   //! the element pointed by it. No destructors or copy constructors are called.
   //! 
   //! <b>Returns</b>: The last element inserted of x or the previous element
   //!   of it if x is empty.
   //!   This iterator can be used as new "prev" iterator for a new splice call.
   //!   that will splice new values after the previously spliced values.
   //! 
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Linear to the elements contained in x plus linear to
   //!   the elements before it.
   //!   Linear to the elements before it if cache_last<> option is true.
   //!   Constant-time if cache_last<> option is true and it == end().
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //! list. Iterators of this list and all the references are not invalidated.
   iterator splice(iterator it, slist_impl &x)
   {  return this->splice_after(this->previous(it), x);   }

   //! <b>Requires</b>: it p must be a valid iterator of *this.
   //!   elem must point to an element contained in list
   //!   x.
   //! 
   //! <b>Effects</b>: Transfers the element elem, from list x to this list, 
   //!   before the element pointed by pos. No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the elements before pos and before elem.
   //!   Linear to the elements before elem if cache_last<> option is true and pos == end().
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //! list. Iterators of this list and all the references are not invalidated.
   void splice(iterator pos, slist_impl &x, iterator elem)
   {  return this->splice_after(this->previous(pos), x, x.previous(elem));  }

   //! <b>Requires</b>: pos must be a dereferenceable iterator in *this
   //!   and first and last belong to x and first and last a valid range on x. 
   //! 
   //! <b>Effects</b>: Transfers the range [first, last) from list x to this
   //!   list, before the element pointed by pos.
   //!   No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the sum of elements before pos, first, and last
   //!   plus linear to the number of elements transferred if constant_time_size is true.
   //!   Linear to the sum of elements before first, and last
   //!   plus linear to the number of elements transferred if constant_time_size is true
   //!   if cache_last<> is true and pos == end()
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(iterator pos, slist_impl &x, iterator first, iterator last)
   {  return this->splice_after(this->previous(pos), x, x.previous(first), x.previous(last));  }

   //! <b>Requires</b>: pos must be a dereferenceable iterator in *this
   //!   and first and last belong to x and first and last a valid range on x. 
   //!   n == std::distance(first, last).
   //! 
   //! <b>Effects</b>: Transfers the range [first, last) from list x to this
   //!   list, before the element pointed by pos.
   //!   No destructors or copy constructors are called.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the sum of elements before pos, first, and last.
   //!   Linear to the sum of elements before first and last
   //!   if cache_last<> is true and pos == end().
   //! 
   //! <b>Note</b>: Iterators of values obtained from list x now point to elements of this
   //!   list. Iterators of this list and all the references are not invalidated.
   void splice(iterator pos, slist_impl &x, iterator first, iterator last, difference_type n)
   {  return this->splice_after(this->previous(pos), x, x.previous(first), x.previous(last), n);  }

   //! <b>Effects</b>: This function sorts the list *this according to std::less<value_type>. 
   //!   The sort is stable, that is, the relative order of equivalent elements is preserved.
   //! 
   //! <b>Throws</b>: If value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks)
   //!   or the predicate throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: The number of comparisons is approximately N log N, where N
   //!   is the list's size.
   //!
   //! <b>Note</b>: Iterators and references are not invalidated
   template<class Predicate>
   void sort(Predicate p)
   {
      if (node_traits::get_next(node_traits::get_next(this->get_root_node()))
               != this->get_root_node()) {
         slist_impl carry;
         slist_impl counter[64];
         int fill = 0;
         iterator last_inserted;
         while(!this->empty()){
            last_inserted = this->begin();
            carry.splice_after(carry.before_begin(), *this, this->before_begin());
            int i = 0;
            while(i < fill && !counter[i].empty()) {
               last_inserted = carry.merge(counter[i++], p);
            }
            BOOST_INTRUSIVE_INVARIANT_ASSERT(counter[i].empty());

            node_ptr p = node_algorithms::get_previous_node
               (last_inserted.pointed_node(), carry.end().pointed_node());
            iterator last_element(p, this);
            if(constant_time_size){
               counter[i].splice_after( counter[i].before_begin(), carry
                                      , carry.before_begin(), last_element
                                      , carry.size());
            }
            else{
               counter[i].splice_after( counter[i].before_begin(), carry
                                      , carry.before_begin(), last_element);
            }
            if(i == fill)
               ++fill;
         }

         for (int i = 1; i < fill; ++i)
            last_inserted = counter[i].merge(counter[i-1], p);
         BOOST_INTRUSIVE_INVARIANT_ASSERT(this->empty());

         node_ptr p = node_algorithms::get_previous_node
            (last_inserted.pointed_node(), counter[--fill].end().pointed_node());
         iterator last_element(p, this);
         if(constant_time_size){
            this->splice_after( before_begin(), counter[fill], counter[fill].before_begin()
                              , last_element, counter[fill].size());
         }
         else{
            this->splice_after( before_begin(), counter[fill], counter[fill].before_begin()
                              , last_element);
         }
      }
   }

   //! <b>Requires</b>: p must be a comparison function that induces a strict weak
   //!   ordering and both *this and x must be sorted according to that ordering
   //!   The lists x and *this must be distinct. 
   //! 
   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this. The merge is stable; that is, if an element from *this is 
   //!   equivalent to one from x, then the element from *this will precede the one from x. 
   //! 
   //! <b>Throws</b>: If value_traits::node_traits::node
   //!   constructor throws (this does not happen with predefined Boost.Intrusive hooks)
   //!   or std::less<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   void sort()
   { this->sort(std::less<value_type>()); }

   //! <b>Requires</b>: p must be a comparison function that induces a strict weak
   //!   ordering and both *this and x must be sorted according to that ordering
   //!   The lists x and *this must be distinct. 
   //! 
   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this. The merge is stable; that is, if an element from *this is 
   //!   equivalent to one from x, then the element from *this will precede the one from x. 
   //! 
   //! <b>Returns</b>: An iterator to the last transferred value, end() is x is empty.
   //! 
   //! <b>Throws</b>: If the predicate throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   template<class Predicate>
   iterator merge(slist_impl& x, Predicate p) 
   {
      iterator a(before_begin()), e(end()), ax(x.before_begin()), ex(x.end());
      iterator last_inserted(e);
      iterator a_next;
      while(++(a_next = a) != e && !x.empty()) {
         iterator ix(ax);
         iterator cx;
         size_type n(0);
         while(++(cx = ix) != ex && p(*cx, *a_next)){
            ++ix; ++n;
         }
         if(ax != ix){
            this->splice_after(a, x, ax, ix, n);
            last_inserted = ix;
         }
         a = a_next;
      }  
      if (!x.empty()){
         last_inserted = this->splice_after(a, x);
      }
      return last_inserted;
   }

   //! <b>Effects</b>: This function removes all of x's elements and inserts them
   //!   in order into *this according to std::less<value_type>. The merge is stable; 
   //!   that is, if an element from *this is equivalent to one from x, then the element 
   //!   from *this will precede the one from x. 
   //! 
   //! <b>Throws</b>: if std::less<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: This function is linear time: it performs at most
   //!   size() + x.size() - 1 comparisons.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   void merge(slist_impl& x)
   {  this->merge(x, std::less<value_type>());  }

   //! <b>Effects</b>: Reverses the order of elements in the list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: This function is linear to the contained elements.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   void reverse()
   {
      if(cache_last && !this->empty()){
         this->set_last_node(node_traits::get_next(this->get_root_node()));
      }
      this->priv_reverse(detail::bool_<linear>());
   }

   //! <b>Effects</b>: Removes all the elements that compare equal to value.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: If std::equal_to<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid. This function is 
   //!   linear time: it performs exactly size() comparisons for equality.
   void remove(const_reference value)
   {  this->remove_if(detail::equal_to_value<const_reference>(value));  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes all the elements that compare equal to value.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //!
   //! <b>Throws</b>: If std::equal_to<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Disposer>
   void remove_and_dispose(const_reference value, Disposer disposer)
   {  this->remove_and_dispose_if(detail::equal_to_value<const_reference>(value), disposer);  }

   //! <b>Effects</b>: Removes all the elements for which a specified
   //!   predicate is satisfied. No destructors are called.
   //! 
   //! <b>Throws</b>: If pred throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() calls to the predicate.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Pred>
   void remove_if(Pred pred)
   {  this->remove_and_dispose_if(pred, detail::null_disposer());   }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes all the elements for which a specified
   //!   predicate is satisfied.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //!
   //! <b>Throws</b>: If pred throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time. It performs exactly size() comparisons for equality.
   //!
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Pred, class Disposer>
   void remove_and_dispose_if(Pred pred, Disposer disposer)
   {
      iterator bcur(this->before_begin()), cur(this->begin()), e(this->end());
      
      while(cur != e){
         if (pred(*cur)){
            cur = this->erase_after_and_dispose(bcur, disposer);
         }
         else{
            bcur = cur;
            ++cur;
         }
      }
      if(cache_last){
         this->set_last_node(bcur.pointed_node());
      }
   }

   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that are equal from the list. No destructors are called.
   //! 
   //! <b>Throws</b>: If std::equal_to<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons calls to pred()).
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   void unique()
   {  this->unique_and_dispose(std::equal_to<value_type>(), detail::null_disposer());  }

   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   No destructors are called.
   //! 
   //! <b>Throws</b>: If the predicate throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons equality comparisons.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class BinaryPredicate>
   void unique(BinaryPredicate pred)
   {  this->unique_and_dispose(pred, detail::null_disposer());  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //! 
   //! <b>Throws</b>: If std::equal_to<value_type> throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons equality comparisons.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class Disposer>
   void unique_and_dispose(Disposer disposer)
   {  this->unique(std::equal_to<value_type>(), disposer);  }

   //! <b>Requires</b>: Disposer::operator()(pointer) shouldn't throw.
   //!
   //! <b>Effects</b>: Removes adjacent duplicate elements or adjacent 
   //!   elements that satisfy some binary predicate from the list.
   //!   Disposer::operator()(pointer) is called for every removed element.
   //! 
   //! <b>Throws</b>: If the predicate throws. Basic guarantee.
   //! 
   //! <b>Complexity</b>: Linear time (size()-1) comparisons equality comparisons.
   //! 
   //! <b>Note</b>: The relative order of elements that are not removed is unchanged,
   //!   and iterators to elements that are not removed remain valid.
   template<class BinaryPredicate, class Disposer>
   void unique_and_dispose(BinaryPredicate pred, Disposer disposer)
   {
      iterator end_n(this->end());
      iterator bcur(this->begin());
      if(bcur != end_n){
         iterator cur(bcur);
         ++cur;
         while(cur != end_n) {
            if (pred(*bcur, *cur)){
               cur = this->erase_after_and_dispose(bcur, disposer);
            }
            else{
               bcur = cur;
               ++cur;
            }
         }
         if(cache_last){
            this->set_last_node(bcur.pointed_node());
         }
      }
   }

   //! <b>Requires</b>: value must be a reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns a const_iterator pointing to the element
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   //!   This static function is available only if the <i>value traits</i>
   //!   is stateless.
   static iterator s_iterator_to(reference value) 
   {
      BOOST_STATIC_ASSERT((!stateful_value_traits));
      //BOOST_INTRUSIVE_INVARIANT_ASSERT (!node_algorithms::inited(value_traits::to_node_ptr(value)));
      return iterator (value_traits::to_node_ptr(value), 0);
   }

   //! <b>Requires</b>: value must be a const reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns an iterator pointing to the element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   //!   This static function is available only if the <i>value traits</i>
   //!   is stateless.
   static const_iterator s_iterator_to(const_reference value) 
   {
      BOOST_STATIC_ASSERT((!stateful_value_traits));
      //BOOST_INTRUSIVE_INVARIANT_ASSERT (!node_algorithms::inited(value_traits::to_node_ptr(const_cast<reference> (value))));
      return const_iterator (value_traits::to_node_ptr(const_cast<reference> (value)), 0);
   }

   //! <b>Requires</b>: value must be a reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns a const_iterator pointing to the element
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   iterator iterator_to(reference value) 
   {
      //BOOST_INTRUSIVE_INVARIANT_ASSERT (!node_algorithms::inited(value_traits::to_node_ptr(value)));
      return iterator (value_traits::to_node_ptr(value), this);
   }

   //! <b>Requires</b>: value must be a const reference to a value inserted in a list.
   //! 
   //! <b>Effects</b>: This function returns an iterator pointing to the element.
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Constant time.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated.
   const_iterator iterator_to(const_reference value) const
   {
      //BOOST_INTRUSIVE_INVARIANT_ASSERT (!node_algorithms::inited(value_traits::to_node_ptr(const_cast<reference> (value))));
      return const_iterator (value_traits::to_node_ptr(const_cast<reference> (value)), this);
   }

   //! <b>Returns</b>: The iterator to the element before i in the list. 
   //!   Returns the end-iterator, if either i is the begin-iterator or the 
   //!   list is empty. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements before i.
   //!   Constant if cache_last<> is true and i == end().
   iterator previous(iterator i)
   {
      if(cache_last && (i.pointed_node() == this->get_end_node())){
         return iterator(this->get_last_node(), this);
      }
      return iterator
         (node_algorithms::get_previous_node
            (this->before_begin().pointed_node(), i.pointed_node()), this);
   }

   //! <b>Returns</b>: The const_iterator to the element before i in the list. 
   //!   Returns the end-const_iterator, if either i is the begin-const_iterator or 
   //!   the list is empty. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements before i. 
   //!   Constant if cache_last<> is true and i == end().
   const_iterator previous(const_iterator i) const
   {
      if(cache_last && (i.pointed_node() == this->get_end_node())){
         return iterator(uncast(this->get_last_node()), this);
      }
      return const_iterator
         (node_algorithms::get_previous_node
            (this->before_begin().pointed_node(), i.pointed_node()), this);
   }

   private:
   void priv_splice_after(node_ptr prev_pos_n, slist_impl &x, node_ptr before_first_n, node_ptr before_last_n)
   {
      if (before_first_n != before_last_n && prev_pos_n != before_first_n && prev_pos_n != before_last_n)
      {
         if(cache_last){
            if(node_traits::get_next(prev_pos_n) == this->get_end_node()){
               this->set_last_node(before_last_n);
            }
            if(node_traits::get_next(before_last_n) == x.get_end_node()){
               x.set_last_node(before_first_n);
            }
         }
         node_algorithms::transfer_after(prev_pos_n, before_first_n, before_last_n);
      }
   }

   void priv_reverse(detail::bool_<false>)
   {  node_algorithms::reverse(this->get_root_node());   }

   void priv_reverse(detail::bool_<true>)
   {
      node_ptr new_first = node_algorithms::reverse
         (node_traits::get_next(this->get_root_node()));
      node_traits::set_next(this->get_root_node(), new_first);
   }

   void priv_shift_backwards(size_type n, detail::bool_<false>)
   {
      node_ptr last = node_algorithms::move_forward(this->get_root_node(), (std::size_t)n);
      if(cache_last && last){
         this->set_last_node(last);
      }
   }

   void priv_shift_backwards(size_type n, detail::bool_<true>)
   {
      std::pair<node_ptr, node_ptr> ret(
         node_algorithms::move_first_n_forward
            (node_traits::get_next(this->get_root_node()), (std::size_t)n));
      if(ret.first){
         node_traits::set_next(this->get_root_node(), ret.first);
         if(cache_last){
            this->set_last_node(ret.second);
         }
      }
   }

   void priv_shift_forward(size_type n, detail::bool_<false>)
   {
      node_ptr last = node_algorithms::move_backwards(this->get_root_node(), (std::size_t)n);   
      if(cache_last && last){
         this->set_last_node(last);
      }
   }

   void priv_shift_forward(size_type n, detail::bool_<true>)
   {
      std::pair<node_ptr, node_ptr> ret(
         node_algorithms::move_first_n_backwards
         (node_traits::get_next(this->get_root_node()), (std::size_t)n));
      if(ret.first){
         node_traits::set_next(this->get_root_node(), ret.first);
         if(cache_last){
            this->set_last_node(ret.second);
         }
      }
   }

   void priv_swap_cache_last(slist_impl &other)
   {
      node_ptr other_last(other.get_last_node());
      node_ptr this_last(this->get_last_node());
      node_ptr other_bfirst(other.get_root_node());
      node_ptr this_bfirst(this->get_root_node());
      node_algorithms::transfer_after(this_bfirst, other_bfirst, other_last);
      node_algorithms::transfer_after(other_bfirst, other_last != other_bfirst? other_last : this_bfirst, this_last);
      node_ptr tmp(this->get_last_node());
      this->set_last_node(other.get_last_node());
      other.set_last_node(tmp);
      if(this->get_last_node() == other_bfirst){
         this->set_last_node(this_bfirst);
      }
      if(other.get_last_node() == this_bfirst){
         other.set_last_node(other_bfirst);
      }
   }

   //circular version
   static void priv_swap_lists(node_ptr this_node, node_ptr other_node, detail::bool_<false>)
   {  node_algorithms::swap_nodes(this_node, other_node); }

   //linear version
   static void priv_swap_lists(node_ptr this_node, node_ptr other_node, detail::bool_<true>)
   {  node_algorithms::swap_trailing_nodes(this_node, other_node); }

   static slist_impl &priv_container_from_end_iterator(const const_iterator &end_iterator)
   {
      //Obtaining the container from the end iterator is not possible with linear
      //singly linked lists (because "end" is represented by the null pointer)
      BOOST_STATIC_ASSERT(!linear);
      root_plus_size *r = detail::parent_from_member<root_plus_size, node>
         ( detail::get_pointer(end_iterator.pointed_node()), (&root_plus_size::root_));
      data_t *d = detail::parent_from_member<data_t, root_plus_size>
         ( r, &data_t::root_plus_size_);
      slist_impl *s  = detail::parent_from_member<slist_impl, data_t>(d, &slist_impl::data_);
      return *s;
   }
};

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator<
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(const slist_impl<T, Options...> &x, const slist_impl<T, Options...> &y)
#else
(const slist_impl<Config> &x, const slist_impl<Config> &y)
#endif
{  return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());  }

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
bool operator==
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(const slist_impl<T, Options...> &x, const slist_impl<T, Options...> &y)
#else
(const slist_impl<Config> &x, const slist_impl<Config> &y)
#endif
{
   typedef slist_impl<Config> slist_type;
   typedef typename slist_type::const_iterator const_iterator;
   const bool C = slist_type::constant_time_size;
   if(C && x.size() != y.size()){
      return false;
   }
   const_iterator end1 = x.end();

   const_iterator i1 = x.begin();
   const_iterator i2 = y.begin();
   if(C){
      while (i1 != end1 && *i1 == *i2) {
         ++i1;
         ++i2;
      }
      return i1 == end1;
   }
   else{
      const_iterator end2 = y.end();
      while (i1 != end1 && i2 != end2 && *i1 == *i2) {
         ++i1;
         ++i2;
      }
      return i1 == end1 && i2 == end2;
   }
}

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator!=
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(const slist_impl<T, Options...> &x, const slist_impl<T, Options...> &y)
#else
(const slist_impl<Config> &x, const slist_impl<Config> &y)
#endif
{  return !(x == y); }

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator>
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(const slist_impl<T, Options...> &x, const slist_impl<T, Options...> &y)
#else
(const slist_impl<Config> &x, const slist_impl<Config> &y)
#endif
{  return y < x;  }

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator<=
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(const slist_impl<T, Options...> &x, const slist_impl<T, Options...> &y)
#else
(const slist_impl<Config> &x, const slist_impl<Config> &y)
#endif
{  return !(y < x);  }

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
inline bool operator>=
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(const slist_impl<T, Options...> &x, const slist_impl<T, Options...> &y)
#else
(const slist_impl<Config> &x, const slist_impl<Config> &y)
#endif
{  return !(x < y);  }

#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class Config>
#endif
inline void swap
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
(slist_impl<T, Options...> &x, slist_impl<T, Options...> &y)
#else
(slist_impl<Config> &x, slist_impl<Config> &y)
#endif
{  x.swap(y);  }

//! Helper metafunction to define a \c slist that yields to the same type when the
//! same options (either explicitly or implicitly) are used.
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class ...Options>
#else
template<class T, class O1 = none, class O2 = none, class O3 = none, class O4 = none, class O5 = none>
#endif
struct make_slist
{
   /// @cond
   typedef typename pack_options
      < slist_defaults<T>, O1, O2, O3, O4, O5>::type packed_options;
   typedef typename detail::get_value_traits
      <T, typename packed_options::value_traits>::type value_traits;
   typedef slist_impl
      <
         slistopt
         < value_traits
         , typename packed_options::size_type
         , packed_options::constant_time_size
         , packed_options::linear
         , packed_options::cache_last
         >
      > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};


#ifndef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class T, class O1, class O2, class O3, class O4, class O5>
class slist
   :  public make_slist<T, O1, O2, O3, O4, O5>::type
{
   typedef typename make_slist
      <T, O1, O2, O3, O4, O5>::type   Base;
   typedef typename Base::real_value_traits  real_value_traits;
   //Assert if passed value traits are compatible with the type
   BOOST_STATIC_ASSERT((detail::is_same<typename real_value_traits::value_type, T>::value));
   public:
   typedef typename Base::value_traits       value_traits;
   typedef typename Base::iterator           iterator;
   typedef typename Base::const_iterator     const_iterator;

   slist(const value_traits &v_traits = value_traits())
      :  Base(v_traits)
   {}

   template<class Iterator>
   slist(Iterator b, Iterator e, const value_traits &v_traits = value_traits())
      :  Base(b, e, v_traits)
   {}

   static slist &container_from_end_iterator(iterator end_iterator)
   {  return static_cast<slist &>(Base::container_from_end_iterator(end_iterator));   }

   static const slist &container_from_end_iterator(const_iterator end_iterator)
   {  return static_cast<const slist &>(Base::container_from_end_iterator(end_iterator));   }
};

#endif

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_SLIST_HPP
