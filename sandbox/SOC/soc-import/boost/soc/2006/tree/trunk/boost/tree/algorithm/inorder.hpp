//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file inorder.hpp
 * Non-modifying hierarchy inorder range algorithms
 */

// TODO: concept checks.

#ifndef BOOST_TREE_ALGORITHM_INORDER_HPP
#define BOOST_TREE_ALGORITHM_INORDER_HPP

#include <boost/tree/inorder_iterator.hpp>

namespace boost {
namespace tree {

namespace inorder {

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor, forward_traversal_tag> 
begin(MultiwayTree& t, forward_traversal_tag)
{
	typedef typename MultiwayTree::cursor cursor;
	std::stack<cursor> s;
 	typename cursor_size<cursor>::type branch = 0;
 	typename cursor_size<cursor>::type i = 0;
	s.push(t.root());
	while (!s.top().empty()) {
		++i;
		if (!branch && !s.top().end().empty())
			branch = i;
		s.push(s.top().begin());
	}
	return iterator<cursor, forward_traversal_tag>(s, branch);
}

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> 
begin(MultiwayTree& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::cursor>(first(t));
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::begin())
 * @param t	A MultiwayTree
 * @return	Mutable inorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> begin(MultiwayTree& t)
{
	typedef typename cursor_category<typename MultiwayTree::cursor>::type
		traversal;
	return begin(t, traversal());
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(Alias of cbegin(); equivalent to postorder::begin())
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> begin(MultiwayTree const& t)
{
	return cbegin(t);
}


template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor>
cbegin(MultiwayTree const& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::const_cursor>(cfirst(t));
}

/**
 * @brief	First element of a MultiwayTree in inorder traversal
 * 			(equivalent to postorder::begin())
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator to the first element of @a t
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cbegin(MultiwayTree const& t)
{
	typedef typename cursor_category<
		typename MultiwayTree::const_cursor>::type traversal;
	return cbegin(t, traversal());
}

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor, forward_traversal_tag> 
end(MultiwayTree& t, forward_traversal_tag)
{
	typedef typename MultiwayTree::cursor cursor;
	std::stack<cursor> s;
	s.push(t.root());
	while (!s.top().empty())
		s.push(s.top().end());
	return iterator<cursor, forward_traversal_tag>(s, s.size());
}

template <class MultiwayTree>
iterator<typename MultiwayTree::cursor, bidirectional_traversal_tag>
end(MultiwayTree& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::cursor>(last(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Mutable inorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::cursor> end(MultiwayTree& t)
{
	typedef typename cursor_category<typename MultiwayTree::cursor>::type
		traversal;
	return end(t, traversal());
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal (Alias of cend())
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> end(MultiwayTree const& t)
{
	return cend(t);
}

template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor>
cend(MultiwayTree const& t, bidirectional_traversal_tag)
{
	return iterator<typename MultiwayTree::const_cursor>(clast(t));
}

/**
 * @brief	One position past the last element of a MultiwayTree 
 * 			in inorder traversal
 * @param t	A MultiwayTree
 * @return	Read-only inorder iterator one position past the last element of @a t 
 */
template <class MultiwayTree>
iterator<typename MultiwayTree::const_cursor> cend(MultiwayTree const& t)
{
	typedef typename cursor_category<
		typename MultiwayTree::const_cursor>::type traversal;
	return cend(t, traversal());
}

} // namespace inorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_ALGORITHM_INORDER_HPP
