//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>

#include <boost/tree/searcher.hpp>
#include <boost/tree/algorithm.hpp>

#include <boost/test/minimal.hpp>

#include "helpers.hpp"

//TODO: Make this a test suite.

using namespace boost::tree;

template <class Tree>
void create_unbalanced_binary_tree(Tree& mytree)
{
	typename Tree::cursor c, c1, c2, c3, c4;
	
	c = mytree.root();

	BOOST_CHECK(c.empty());
	
	c1 = mytree.insert(c, 1);
	BOOST_CHECK(*c1 == 1);
	
	BOOST_CHECK(!c.empty());
	
	BOOST_CHECK(c1.parent() == c);
	
	c2 = mytree.insert(c1, 2);
	BOOST_CHECK(!c.empty());
	BOOST_CHECK(c2.empty());
	BOOST_CHECK(*c1 == 1);
	BOOST_CHECK(*c2 == 2);
	*c1 = 14;
	BOOST_CHECK(*c1 == 14);
	BOOST_CHECK(*c2 == 2);
	BOOST_CHECK(c2.parent() == c1);
	BOOST_CHECK(c1.parent() == c);
	
	c3 = c1.end();
	BOOST_CHECK(c3 == c1.end());
	--c3;
	BOOST_CHECK(c3.parity() == 0);
	BOOST_CHECK(c3.parent() != c3);
	BOOST_CHECK(c3.parent() == c1);
	BOOST_CHECK(c3 == c1.begin());
	
	*c3 = 3;
	*(c1.begin()) = 2;
	
	BOOST_CHECK(*c3 == 2);
	++c3;
	c4 = mytree.insert(c3, 4);
	BOOST_CHECK(*c4 == 4);
	c4 = c4.parent();
	--c4;
	BOOST_CHECK(*c4 == 2);
	BOOST_CHECK(c4.parent() == c1);
	c = boost::tree::lower_bound(mytree.root(), mytree.shoot(), 2, std::less<int>());
	BOOST_CHECK(*c == 2);
	BOOST_CHECK(c4.empty());

	BOOST_CHECK(*c1 == 14);
	
	BOOST_CHECK(c1.begin().empty() || c1.end().empty());
	
	//c1 = mytree.erase(c1);
	//BOOST_CHECK(*c1 == 2);

}

template <class Tree>
void destroy_unbalanced_binary_tree(Tree& mytree)
{
	mytree.clear();	
	BOOST_CHECK(mytree.empty());
}

template <class Tree>
void validate_unbalanced_binary_tree(Tree const& mytree)
{
	typename Tree::const_cursor c, c1, c2, c3, c4;

	c = mytree.root();
	BOOST_CHECK(!c.empty());
	
	c1 = c.begin();
	BOOST_CHECK(c1.parent() == c);
	BOOST_CHECK(*c1 == 14);
	
	c2 = c1.begin();
	BOOST_CHECK(c2.parent() == c1);
}

template <class Tree>
void test_swap_binary_trees(Tree& one, Tree& two)
{
	using std::swap;
	swap(one, two);
}

int test_main(int, char* [])
{
	typedef binary_tree<int> tree_t;
	tree_t tree1, tree2;
	
	create_unbalanced_binary_tree(tree1);
	validate_unbalanced_binary_tree(tree1);

	create_test_data_tree(tree2);
	validate_test_data_tree(tree2);
	
	test_swap_binary_trees(tree1, tree2);
	
	validate_test_data_tree(tree1);
	validate_unbalanced_binary_tree(tree2);
	
	destroy_unbalanced_binary_tree(tree2);
	
	return 0;
}
