//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file balanced_tree.hpp
 * Balanced tree implementation
 */


#ifndef BOOST_TREE_BALANCED_TREE_HPP
#define BOOST_TREE_BALANCED_TREE_HPP

#include <boost/tree/cursor.hpp>
#include <boost/tree/iterator.hpp>

#include <boost/tree/balancers/unbalanced.hpp>
#include <boost/tree/augmentors/unaugmented.hpp>

#include <boost/tree/search.hpp>

#include <boost/tree/detail/iterator/augmented.hpp>

#include <boost/bind.hpp>
#include <boost/multi_index/member.hpp>

#include <iterator>
#include <memory>

namespace boost {
namespace tree {

using detail::node;
using detail::const_nary_tree_cursor;
using detail::nary_tree_cursor;

using detail::augmented_iterator;

using boost::multi_index::member;

template <class Val, class Meta>
struct augmented_type {
	typedef Val value_type;
	typedef Meta metadata_type;
	
	value_type data;
	metadata_type meta;
	
	augmented_type(value_type const& d, metadata_type const& m = metadata_type())
	: data(d), meta(m) {}
	
	augmented_type(augmented_type const& x)
	: data(x.data), meta(x.meta) {}
	
	metadata_type& metadata()
	{
		return meta;
	}
	
	metadata_type const& metadata() const
	{
		return meta;
	}

	typedef member<augmented_type,value_type,&augmented_type::data> extract_data;
	typedef member<augmented_type,metadata_type,&augmented_type::meta> extract_meta;
};

/** 
 * @brief A %balanced_tree.
 * This class models the hierarchy concept, the container concept and the
 * sequence concept. TODO: complete this...
 *
*/
template <class Hierarchy, class Balance>
class balanced_tree {
 public:
	typedef typename Hierarchy::value_type value_type;
	typedef Balance balancer_type;
	
	typedef typename Balance::metadata_type metadata_type;
	
	typedef augmented_type<value_type, metadata_type> data_type;
	typedef typename Hierarchy::template rebind<data_type>::other hierarchy_type;

 protected:
    hierarchy_type h;

 private:
	typedef balanced_tree<Hierarchy, Balance> self_type;
	
	typedef typename hierarchy_type::cursor cursor;
	typedef typename hierarchy_type::const_cursor const_cursor;

 public:	
	typedef augmented_iterator<inorder::iterator<cursor>, typename data_type::extract_data, bidirectional_traversal_tag> iterator;
	typedef augmented_iterator<inorder::iterator<const_cursor>, typename data_type::extract_data, bidirectional_traversal_tag> const_iterator;
	
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef typename hierarchy_type::allocator_type allocator_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;

	explicit balanced_tree (hierarchy_type const& hier = hierarchy_type())
	: h(hier)
	{}
	
	// TODO: 3rd arg...?
	explicit balanced_tree (size_type n, value_type const& value = value_type(), 
		hierarchy_type const& hier = hierarchy_type())
	: h(hier)
	{}

	template <class InputIterator>
		balanced_tree (InputIterator first, InputIterator last, 
			hierarchy_type const& hier = hierarchy_type())
			: h(hier)
	{
		while (first++ != last)
			this->insert(this->end(), *first);
	}
	
	balanced_tree (self_type const& other)
	: h(other.h)
	{}
	
	self_type& operator=(
		self_type const& x);
	template <class InputIterator>
		void assign(InputIterator first, InputIterator last);
	template <class Size, class T>
		void assign(Size n, const T& t = T());
//	allocator_type get_allocator() const
//	{
//		return h.get_allocator();
//	}
		
	/// Functions returning (inorder) iterators (as required by the Sequence
	/// concept)
	
	/**
	 * Returns a read/write ("mutable") iterator to the first (inorder) value.
	 */ 	 
	iterator begin()
	{
		return iterator(h.inorder_first());

	}
	
	/**
	 * Returns a read-only const_iterator to the first (inorder) value.
	 */ 	 
	const_iterator begin() const
	{
		return cbegin();
	}
	
	/**
	 * Returns a read-only const_iterator to the first (inorder) value.
	 */ 	 
	const_iterator cbegin() const
	{
		return const_iterator(h.inorder_cfirst());
	}

	/**
	 * Returns a read/write ("mutable") inorder iterator to the position one 
	 * past the last (inorder) value in the %balanced_tree.  
	 */
	iterator end()
	{
		return iterator(h.shoot());
	}

	/**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %balanced_tree.
	 */	
	const_iterator end() const
	{
		return cend();
	}
	
	/**
	 * Returns a read-only inorder const_iterator to the position one past the 
	 * last (inorder) value in the %balanced_tree. 
	 */	
	const_iterator cend() const
	{
		return const_iterator(h.cshoot());
	}

	/**
	 * @brief		Finds the first position in the @balanced_tree in which @a k
	 * 				could be inserted without changing the ordering, using <
	 * 				(less than) for comparisons.
	 * @param k		The search term
	 * @return		An iterator pointing to the first element not less than 
	 *				@a k, or end() if every element in the @balanced_tree is
	 * 				less than @a k.
	 */
	 iterator lower_bound(value_type const& k)
	 {
	 	return lower_bound(k, std::less<value_type>());
	 }

	/**
	 * @brief		Finds the first position in the @balanced_tree in which @a k
	 * 				could be inserted without changing the ordering, using <
	 * 				(less than) for comparisons.
	 * @param k		The search term
	 * @return		A const_iterator pointing to the first element not less than 
	 *				@a k, or end() if every element in the @balanced_tree is
	 * 				less than @a k.
	 */
	 const_iterator lower_bound(value_type const& k) const
	 {
	 	return lower_bound(k, std::less<value_type>());
	 }

	/**
	 * @brief		Finds the first position in the @balanced_tree in which @a k
	 * 				could be inserted without changing the ordering, using cmp
	 * 				for comparisons.
	 * @param k		The search term
	 * @param cmp	The comparison functor
	 * @return		An iterator pointing to the first element not less than 
	 *				@a k, or end() if every element in the @balanced_tree is
	 * 				less than @a k.
	 */
	 template <class Cmp>
	 iterator lower_bound(value_type const& k, Cmp cmp)
	 {
	 	return iterator(boost::tree::lower_bound(h.root(), h.shoot(), k, 
	 		bind<bool>(cmp, bind(typename data_type::extract_data(), _1), _2)));
	 }

	/**
	 * @brief		Finds the first position in the @balanced_tree in which @a k
	 * 				could be inserted without changing the ordering, using cmp
	 * 				for comparisons.
	 * @param k		The search term
	 * @param cmp	The comparison functor
	 * @return		A const_iterator pointing to the first element not less than 
	 *				@a k, or end() if every element in the @balanced_tree is
	 * 				less than @a k.
	 */
	 template <class Cmp>
	 const_iterator lower_bound(value_type const& k, Cmp cmp) const
	 {
	 	return const_iterator(boost::tree::lower_bound(h.croot(), h.cshoot(), k,
		 	bind<bool>(cmp, bind(typename data_type::extract_data(), _1), _2)));
	 }
	 	 
	/**
	 * @brief  Add data to the front of the %balanced_tree.
	 * @param  x  Data to be added.
	 * 
	 * This is a typical stack operation.  The function creates an
	 * element at the front of the %balanced_tree and assigns the given data to
	 * it.  Due to the nature of a %balanced_tree this operation can be done
	 * in constant time, and does not invalidate iterators and references.
	 */
    void push_front(value_type const& x)
    {
		insert(begin(), x);
    }

	/**
	 * @brief  Removes first element.
	 * 
	 * This is a typical stack operation.  It shrinks the %balance_tree by
	 * one.  Due to the nature of a %balanced_tree this operation can be done
	 * in constant time, and only invalidates iterators/references to
	 * the element being removed.
	 * 
	 * Note that no data is returned, and if the first element's data
	 * is needed, it should be retrieved before pop_front() is called.
	 */
	void pop_front()
	{
		erase(begin());
	}

	/**
	 * @brief  Add data to the end of the %balanced_tree.
	 * @param  x  Data to be added.
	 * 
	 * This is a typical stack operation.  The function creates an
	 * element at the end of the %balanced_tree and assigns the given data to
	 * it.  Due to the nature of a %balanced_tree this operation can be done
	 * in constant time, and does not invalidate iterators and references.
	 */
    void push_back(value_type const& x)
    {
    		insert(end(), x);
    }

	/**
	 * @brief  Removes last element.
	 * 
	 * This is a typical stack operation.  It shrinks the %balance_tree by
	 * one.  Due to the nature of a %balanced_tree this operation can be done
	 * in constant time, and only invalidates iterators/references to
	 * the element being removed.
	 * 
	 * Note that no data is returned, and if the last element's data
	 * is needed, it should be retrieved before pop_back() is called.
	 */
	void pop_back()
	{
	}
      
	/**
	 * @brief		Inserts val in front of @a pos.
	 * @param pos	The %balanced_tree iterator in front of which to insert.
	 * @param val	The value to insert.
	 * @return		An inorder iterator that points to the inserted data.
	 */
	iterator insert(iterator pos, value_type const& val)
	{
		//TODO: we might need to go down in the hierarchy first
		// That means some redundant work if pos was yielded by lower_bound,
		// so maybe saving two cursors in the iterator would make sense
		// (one for deref, one for insert)
		
		cursor c = pos.base().base();
		while (!c.empty())
			c = c.end();
		
		c = h.insert(c, data_type(val));
		
//		c = c.parent();
		balancer_type::add(h, c);
//		c = c.begin();
		return iterator(c);
	}

	/**
	 * @brief  Remove element at given position.
	 * @param  position  Iterator pointing to element to be erased.
	 * @return  An iterator pointing to the next element (or end()).
	 * 
	 * This function will erase the element at the given position and thus
	 * shorten the %balanced_tree by one.
	 * 
	 * Due to the nature of a %balanced_tree this operation can be done in
	 * constant time, and only invalidates iterators/references to
	 * the element being removed.  The user is also cautioned that
	 * this function only erases the element, and that if the element
	 * is itself a pointer, the pointed-to memory is not touched in
	 * any way.  Managing the pointer is the user's responsibilty.
	 */
	void erase (iterator position)
 	{
 		//TODO - we can only call h.clear, not h.erase!
 		//cursor c = position.base().base(); //.parent();
 		//h.erase(balancer_type::remove(*this, c.parent()));
 	}

	/**
	 * @brief  Remove a range of elements.
	 * @param  first  Iterator pointing to the first element to be erased.
	 * @param  last  Iterator pointing to one past the last element to be
	 *               erased.
	 * @return  An iterator pointing to the element pointed to by @a last
	 *          prior to erasing (or end()).
	 * 
	 * This function will erase the elements in the range @a
	 * [first,last) and shorten the %balanced_tree accordingly.
	 * Due to the nature of a %balanced_tree this operation can be done in
	 * constant time, and only invalidates iterators/references to
	 * the elements being removed.  The user is also cautioned that
	 * this function only erases the elements, and that if the
	 * elements themselves are pointers, the pointed-to memory is not
	 * touched in any way.  Managing the pointers is the user's
	 * responsibilty.
	 */
 	void erase (iterator a, iterator b)
 	{
 	}
 	
	/**
	 * @brief Clears all data from the tree.
	 */
 	void clear()
 	{
 		h.clear();
 	}

	/**
	 * Returns true if the %balanced_tree is empty.  (Thus begin() would
	 * equal end().)
	 */
	bool empty() const
	{
		return h.empty();
	}
	
	void rotate(iterator& i)
	{
		cursor c = cursor(inorder::iterator<cursor>(i));
		h.rotate(c);
	}
};


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BALANCED_TREE_HPP
