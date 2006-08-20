//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file preorder.hpp
 * Preorder traversal algorithms for cursors
 */

#ifndef BOOST_TREE_PREORDER_HPP
#define BOOST_TREE_PREORDER_HPP

// TODO: Concept checks: Non-MultiwayTree or forest, parent?

namespace boost {
namespace tree {

namespace preorder {
	
/** \addtogroup traversal */
/*\@{*/

/**
 * @brief	Preorder successor
 * @param c	Cursor to be set to its preorder successor
 */
template <class Cursor>
inline void forward(Cursor& c)
{
	if (c.has_child()) { // Left.
		c = c.begin();
		return;
	}
	if ((++c).has_child()) { // Right.
		c = c.begin();
		return;
	}

	while (c.parity())
		c = c.parent();
	if ((++c).has_child()) {
		c = c.begin();
		return;
	}
	
	--c;
	while (c.has_child()) {
		c = c.end();
		if (!c.has_child())
			--c;
	}
	if (!c.parity()) 
		++c;
	return;
}

/**
 * @brief	Preorder successor
 * @param c	A cursor
 * @return	Preorder successor of @a c
 */
template <class Cursor>
inline Cursor next(Cursor c)
{
	forward(c);
	return c;
}

/**
 * @brief	Preorder predecessor
 * @param c	Cursor to be set to its preorder predecessor
 */
template <class Cursor>
inline void back(Cursor& c)
{
	c = c.parent();
	if (c.parity()) {
		--c;
		while (c.has_child())
			c = c.end();
		return;
	}
	return;
}

/**
 * @brief	Preorder predecessor
 * @param c	A cursor
 * @return	Preorder predecessor of @a c
 */
template <class Cursor>
inline Cursor prior(Cursor c)
{
	back(c);
	return c;
}

/**
 * @brief	cursor to the first element of a tree in preorder traversal
 * @param t	A tree
 * @return	Preorder begin of @a t
 */
template <class Tree>
typename Tree::cursor begin(Tree& t)
{
	return t.root().begin();
}

/**
 * @brief	const_cursor to the first element of a tree in preorder traversal
 * 			(Alias of cbegin())
 * @param t	A tree
 * @return	Preorder begin of @a t
 */
template <class Tree>
typename Tree::const_cursor begin(Tree const& t)
{
	return t.croot().cbegin();
}

/**
 * @brief	const_cursor to the first element of a tree in preorder traversal
 * @param t	A tree
 * @return	Preorder begin of @a t
 */
template <class Tree>
typename Tree::const_cursor cbegin(Tree const& t)
{
	return t.croot().cbegin();
}

/**
 * @brief	cursor to one position past the last element of a tree in preorder 
 * 			traversal
 * @param t	A tree
 * @return	Preorder end of @a t
 */
template <class Tree>
typename Tree::cursor end(Tree& t)
{
	typename Tree::cursor c = t.shoot();
	--c;	
	while (c.has_child()) {
		c = c.end();
		if (!c.has_child())
			--c;
	}
	return ++c;
}

/**
 * @brief	const_cursor to one position past the last element of a tree in 
 * 			preorder traversal (Alias of cend())
 * @param t	A tree
 * @return	Preorder end of @a t
 */
template <class Tree>
typename Tree::const_cursor end(Tree const& t)
{
	typename Tree::const_cursor c = t.cshoot();
	--c;	
	while (c.has_child()) {
		c = c.end();
		if (!c.has_child())
			--c;
	}
	return ++c;
}

/**
 * @brief	const_cursor to one position past the last element of a tree in 
 * 			preorder traversal
 * @param t	A tree
 * @return	Preorder end of @a t
 */
template <class Tree>
typename Tree::const_cursor cend(Tree const& t)
{
	typename Tree::const_cursor c = t.cshoot();
	--c;	
	while (c.has_child()) {
		c = c.end();
		if (!c.has_child())
			--c;
	}
	return ++c;
}

/*\@}*/

} // namespace preorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_PREORDER_HPP
