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

#ifndef BOOST_INTRUSIVE_UNORDERED_SET_HOOK_HPP
#define BOOST_INTRUSIVE_UNORDERED_SET_HOOK_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/slist_hook.hpp>
#include <boost/intrusive/options.hpp>
#include <boost/intrusive/detail/generic_hook.hpp>

namespace boost {
namespace intrusive {

/// @cond

template<class VoidPointer>
struct slist_node_plus_hash
{
   typedef typename boost::pointer_to_other
      <VoidPointer, slist_node_plus_hash>::type   node_ptr;
   node_ptr    next_;
   std::size_t hash_;
};

// slist_node_traits can be used with circular_slist_algorithms and supplies
// a slist_node holding the pointers needed for a singly-linked list
// it is used by slist_base_hook and slist_member_hook
template<class VoidPointer>
struct slist_node_traits_plus_hash
{
   typedef slist_node_plus_hash<VoidPointer> node;
   typedef typename boost::pointer_to_other
      <VoidPointer, node>::type          node_ptr;
   typedef typename boost::pointer_to_other
      <VoidPointer, const node>::type    const_node_ptr;

   static const bool store_hash = true;

   static node_ptr get_next(const_node_ptr n)
   {  return n->next_;  }

   static void set_next(node_ptr n, node_ptr next)
   {  n->next_ = next;  }

   static std::size_t get_hash(const_node_ptr n)
   {  return n->hash_;  }  

   static void set_hash(node_ptr n, std::size_t h)
   {  n->hash_ = h;  }  
};

template<class VoidPointer, bool StoreHash>
struct get_uset_node_algo
{
   typedef typename detail::if_c
      < StoreHash
      , slist_node_traits_plus_hash<VoidPointer> 
      , slist_node_traits<VoidPointer> 
      >::type node_traits_type;
   typedef circular_slist_algorithms<node_traits_type> type;
};
/// @endcond

//! Helper metafunction to define a \c unordered_set_base_hook that yields to the same
//! type when the same options (either explicitly or implicitly) are used.
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class ...Options>
#else
template<class O1 = none, class O2 = none, class O3 = none, class O4 = none>
#endif
struct make_unordered_set_base_hook
{
   /// @cond
   typedef typename pack_options
      < hook_defaults, O1, O2, O3, O4>::type packed_options;

   typedef detail::generic_hook
   < get_uset_node_algo<typename packed_options::void_pointer
                       , packed_options::store_hash
                       >
   , typename packed_options::tag
   , packed_options::link_mode
   , detail::UsetBaseHook
   > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};

//! Derive a class from unordered_set_base_hook in order to store objects in 
//! in an unordered_set/unordered_multi_set. unordered_set_base_hook holds the data necessary to maintain 
//! the unordered_set/unordered_multi_set and provides an appropriate value_traits class for unordered_set/unordered_multi_set.
//! 
//! The hook admits the following options: \c tag<>, \c void_pointer<>,
//! \c link_mode<> and \c store_hash<>.
//!
//! \c tag<> defines a tag to identify the node. 
//! The same tag value can be used in different classes, but if a class is 
//! derived from more than one \c list_base_hook, then each \c list_base_hook needs its 
//! unique tag.
//!
//! \c void_pointer<> is the pointer type that will be used internally in the hook
//! and the the container configured to use this hook.
//!
//! \c link_mode<> will specify the linking mode of the hook (\c normal_link,
//! \c auto_unlink or \c safe_link).
//!
//! \c store_hash<> will tell the hook to store the hash of the value
//! to speed up rehashings.
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class ...Options>
#else
template<class O1, class O2, class O3, class O4>
#endif
class unordered_set_base_hook
   :  public make_unordered_set_base_hook<O1, O2, O3, O4>::type
{
   #ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state.
   //! 
   //! <b>Throws</b>: Nothing. 
   unordered_set_base_hook();

   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   unordered_set_base_hook(const unordered_set_base_hook& );

   //! <b>Effects</b>: Empty function. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   unordered_set_base_hook& operator=(const unordered_set_base_hook& );

   //! <b>Effects</b>: If link_mode is \c normal_link, the destructor does
   //!   nothing (ie. no code is generated). If link_mode is \c safe_link and the
   //!   object is stored in an unordered_set an assertion is raised. If link_mode is
   //!   \c auto_unlink and \c is_linked() is true, the node is unlinked.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~unordered_set_base_hook();

   //! <b>Effects</b>: Swapping two nodes swaps the position of the elements 
   //!   related to those nodes in one or two containers. That is, if the node 
   //!   this is part of the element e1, the node x is part of the element e2 
   //!   and both elements are included in the containers s1 and s2, then after 
   //!   the swap-operation e1 is in s2 at the position of e2 and e2 is in s1 
   //!   at the position of e1. If one element is not in a container, then 
   //!   after the swap-operation the other element is not in a container. 
   //!   Iterators to e1 and e2 related to those nodes are invalidated. 
   //!
   //! <b>Complexity</b>: Constant 
   //!
   //! <b>Throws</b>: Nothing. 
   void swap_nodes(unordered_set_base_hook &other);

   //! <b>Precondition</b>: link_mode must be \c safe_link or \c auto_unlink.
   //!
   //! <b>Returns</b>: true, if the node belongs to a container, false
   //!   otherwise. This function can be used to test whether \c unordered_set::iterator_to 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool is_linked() const;

   //! <b>Effects</b>: Removes the node if it's inserted in a container.
   //!   This function is only allowed if link_mode is \c auto_unlink.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink();
   #endif
};


//! Helper metafunction to define a \c unordered_set_member_hook that yields to the same
//! type when the same options (either explicitly or implicitly) are used.
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class ...Options>
#else
template<class O1 = none, class O2 = none, class O3 = none, class O4 = none>
#endif
struct make_unordered_set_member_hook
{
   /// @cond
   typedef typename pack_options
      < hook_defaults, O1, O2, O3, O4>::type packed_options;

   typedef detail::generic_hook
   < get_uset_node_algo< typename packed_options::void_pointer
                       , packed_options::store_hash
                       >
   , member_tag
   , packed_options::link_mode
   , detail::NoBaseHook
   > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};

//! Put a public data member unordered_set_member_hook in order to store objects of this class in
//! an unordered_set/unordered_multi_set. unordered_set_member_hook holds the data necessary for maintaining the
//! unordered_set/unordered_multi_set and provides an appropriate value_traits class for unordered_set/unordered_multi_set.
//! 
//! The hook admits the following options: \c void_pointer<>,
//! \c link_mode<> and \c store_hash<>.
//!
//! \c void_pointer<> is the pointer type that will be used internally in the hook
//! and the the container configured to use this hook.
//!
//! \c link_mode<> will specify the linking mode of the hook (\c normal_link,
//! \c auto_unlink or \c safe_link).
//!
//! \c store_hash<> will tell the hook to store the hash of the value
//! to speed up rehashings.
#ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
template<class ...Options>
#else
template<class O1, class O2, class O3, class O4>
#endif
class unordered_set_member_hook
   :  public make_unordered_set_member_hook<O1, O2, O3, O4>::type
{
   #ifdef BOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state.
   //! 
   //! <b>Throws</b>: Nothing. 
   unordered_set_member_hook();

   //! <b>Effects</b>: If link_mode is \c auto_unlink or \c safe_link
   //!   initializes the node to an unlinked state. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing a copy-constructor
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   unordered_set_member_hook(const unordered_set_member_hook& );

   //! <b>Effects</b>: Empty function. The argument is ignored.
   //! 
   //! <b>Throws</b>: Nothing. 
   //! 
   //! <b>Rationale</b>: Providing an assignment operator 
   //!   makes classes using the hook STL-compliant without forcing the 
   //!   user to do some additional work. \c swap can be used to emulate
   //!   move-semantics.
   unordered_set_member_hook& operator=(const unordered_set_member_hook& );

   //! <b>Effects</b>: If link_mode is \c normal_link, the destructor does
   //!   nothing (ie. no code is generated). If link_mode is \c safe_link and the
   //!   object is stored in an unordered_set an assertion is raised. If link_mode is
   //!   \c auto_unlink and \c is_linked() is true, the node is unlinked.
   //! 
   //! <b>Throws</b>: Nothing. 
   ~unordered_set_member_hook();

   //! <b>Effects</b>: Swapping two nodes swaps the position of the elements 
   //!   related to those nodes in one or two containers. That is, if the node 
   //!   this is part of the element e1, the node x is part of the element e2 
   //!   and both elements are included in the containers s1 and s2, then after 
   //!   the swap-operation e1 is in s2 at the position of e2 and e2 is in s1 
   //!   at the position of e1. If one element is not in a container, then 
   //!   after the swap-operation the other element is not in a container. 
   //!   Iterators to e1 and e2 related to those nodes are invalidated. 
   //!
   //! <b>Complexity</b>: Constant 
   //!
   //! <b>Throws</b>: Nothing. 
   void swap_nodes(unordered_set_member_hook &other);

   //! <b>Precondition</b>: link_mode must be \c safe_link or \c auto_unlink.
   //!
   //! <b>Returns</b>: true, if the node belongs to a container, false
   //!   otherwise. This function can be used to test whether \c unordered_set::iterator_to 
   //!   will return a valid iterator. 
   //!
   //! <b>Complexity</b>: Constant 
   bool is_linked() const;

   //! <b>Effects</b>: Removes the node if it's inserted in a container.
   //!   This function is only allowed if link_mode is \c auto_unlink.
   //! 
   //! <b>Throws</b>: Nothing. 
   void unlink();
   #endif
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_UNORDERED_SET_HOOK_HPP
