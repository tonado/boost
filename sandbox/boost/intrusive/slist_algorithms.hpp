/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2007.
// (C) Copyright Ion Gazta�aga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_SLIST_ALGORITHMS_HPP
#define BOOST_INTRUSIVE_SLIST_ALGORITHMS_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/get_pointer.hpp>
#include <cstddef>

namespace boost {
namespace intrusive {

//! slist_algorithms provides basic algorithms to manipulate a sequence of nodes
//! forming a circular singly linked list. An empty sequence is formed by a node
//! whose pointer to the next node point to itself.
//!
//! slist_algorithms is configured with a NodeTraits class, which capsulates the
//! information about the node to be manipulated. NodeTraits must support the
//! following interface:
//!
//! <b>Typedefs</b>:
//!
//! <tt>node</tt>: The type of the node that forms the circular list
//!
//! <tt>void_pointer</tt>: The type of pointer of the node (usually void*)
//!
//! <b>Static functions</b>:
//!
//! <tt>static node_ptr get_next(const_node_ptr n);</tt>
//! 
//! <tt>static void set_next(node_ptr n, node_ptr next);</tt>
template<class NodeTraits>
struct slist_algorithms
{
   //! A pointer to NodeTraits::node of the same type as NodeTraits::void_pointer
   typedef typename detail::pointer_to_other
      <typename NodeTraits::void_pointer
      ,typename NodeTraits::node>::type            node_ptr;

   //! A pointer to const NodeTraits::node of the same type as NodeTraits::void_pointer
   typedef typename detail::pointer_to_other
      <typename NodeTraits::void_pointer
      ,const typename NodeTraits::node>::type      const_node_ptr;

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Returns the previous node of this_node in the sequence.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the sequence.
   //! 
   //! <b>Throws</b>: Nothing.
   static node_ptr get_previous_node(node_ptr this_node)
   {
      node_ptr p = this_node;
      while (this_node != NodeTraits::get_next(p))
         p = NodeTraits::get_next(p);
      return p;
   }

   //! <b>Requires</b>: this_node and prev_init_node must be in the same sequence.
   //! 
   //! <b>Effects</b>: Returns the previous node of this_node in the sequence starting.
   //!   the search from prev_init_node. The first node checked for equality
   //!   is NodeTraits::get_next(prev_init_node).
   //! 
   //! <b>Complexity</b>: Linear to the number of elements between prev_init_node and this_node.
   //! 
   //! <b>Throws</b>: Nothing.
   static node_ptr get_previous_node(node_ptr prev_init_node, node_ptr this_node)
   {
      node_ptr p = prev_init_node;
      while (this_node != NodeTraits::get_next(p))
         p = NodeTraits::get_next(p);
      return p;
   }

   //! <b>Effects</b>: Constructs an empty list, making this_node the only
   //!   node of the sequence:
   //!  <tt>NodeTraits::get_next(this_node) == NodeTraits::get_previous(this_node)
   //!  == this_node</tt>.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void init(node_ptr this_node)  
   {  NodeTraits::set_next(this_node, this_node);  }  

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Returns true is "this_node" is the only node of a sequence:
   //!  <tt>return NodeTraits::get_next(this_node) == this_node</tt>
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static bool unique(const_node_ptr this_node)  
   {  return NodeTraits::get_next(this_node) == this_node; }

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Returns the number of nodes in a sequence. If the sequence
   //!  is empty, returns 1.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static std::size_t count(const_node_ptr this_node) 
   {
      std::size_t result = 0;
      const_node_ptr p = this_node;
      do{
         p = NodeTraits::get_next(p);
         ++result;
      } while (p != this_node);
      return result;
   }

   //! <b>Requires</b>: prev_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Unlinks the next node of prev_node from the sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void unlink_after(node_ptr prev_node)
   {
      node_ptr this_node(NodeTraits::get_next(prev_node));
      NodeTraits::set_next(prev_node, NodeTraits::get_next(this_node));
      NodeTraits::set_next(this_node, this_node);
   }

   //! <b>Requires</b>: this_node must be in a sequence or be an empty sequence.
   //! 
   //! <b>Effects</b>: Unlinks the node from the sequence.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the sequence 
   //! 
   //! <b>Throws</b>: Nothing.
   static void unlink(node_ptr this_node)
   {  unlink_after(get_previous_node(this_node)); }

   //! <b>Requires</b>: prev_node must be a node of a sequence.
   //! 
   //! <b>Effects</b>: Links this_node after prev_node in the sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void link_after(node_ptr prev_node, node_ptr this_node)
   {
      NodeTraits::set_next(this_node, NodeTraits::get_next(prev_node));
      NodeTraits::set_next(prev_node, this_node);
   }

   //! <b>Requires</b>: nxt_node must be a node of a sequence.
   //! 
   //! <b>Effects</b>: Links this_node before nxt_node in the sequence.
   //! 
   //! <b>Complexity</b>: Linear to the number of elements in the sequence. 
   //! 
   //! <b>Throws</b>: Nothing.
   static void link_before (node_ptr nxt_node, node_ptr this_node)
   {  link_after(get_previous_node(nxt_node), this_node);   }

   //! <b>Requires</b>: this_node and other_node must be nodes inserted
   //!  in sequences or empty sequences.
   //! 
   //! <b>Effects</b>: Swaps the position of the nodes: this_node is inserted in
   //!   other_nodes position in seconds sequence and the other_node is inserted
   //!   in this_node's position in the second sequence.
   //! 
   //! <b>Complexity</b>: Linear to number of elements of both sequences 
   //! 
   //! <b>Throws</b>: Nothing.
   static void swap_nodes(node_ptr this_node, node_ptr other_node)
   {
      if (other_node == this_node)
         return;
      bool empty1 = unique(this_node);
      bool empty2 = unique(other_node);
      node_ptr prev_this (get_previous_node(this_node));
      node_ptr prev_other(get_previous_node(other_node));

      node_ptr this_next (NodeTraits::get_next(this_node));
      node_ptr other_next(NodeTraits::get_next(other_node));
      NodeTraits::set_next(this_node, other_next);
      NodeTraits::set_next(other_node, this_next);
      NodeTraits::set_next(empty1 ? other_node : prev_this, other_node);
      NodeTraits::set_next(empty2 ? this_node  : prev_other, this_node);
   }

   //! <b>Requires</b>: b and e must be nodes of the same sequence or an empty range.
   //!   and p must be a node of a different sequence.
   //! 
   //! <b>Effects</b>: Removes the nodes from (b, e] range from their sequence and inserts
   //!   them after p in p's sequence.
   //! 
   //! <b>Complexity</b>: Constant 
   //! 
   //! <b>Throws</b>: Nothing.
   static void transfer_after(node_ptr p, node_ptr b, node_ptr e)
   {
      if (p != b && p != e) {
         node_ptr next_b = NodeTraits::get_next(b);
         node_ptr next_e = NodeTraits::get_next(e);
         node_ptr next_p = NodeTraits::get_next(p);
         NodeTraits::set_next(b, next_e);
         NodeTraits::set_next(e, next_p);
         NodeTraits::set_next(p, next_b);
      }
   }

   //! <b>Effects</b>: Reverses the order of elements in the list. 
   //! 
   //! <b>Throws</b>: Nothing.
   //! 
   //! <b>Complexity</b>: This function is linear to the contained elements.
   //! 
   //! <b>Note</b>: Iterators and references are not invalidated
   static void reverse(node_ptr p)
   {
      node_ptr i = NodeTraits::get_next(p), e(p); 
      for (;;) {
         node_ptr nxt(NodeTraits::get_next(i));
         if (nxt == e)
            break;
         transfer_after(e, i, nxt);
      }
   }
};

} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_SLIST_ALGORITHMS_HPP
