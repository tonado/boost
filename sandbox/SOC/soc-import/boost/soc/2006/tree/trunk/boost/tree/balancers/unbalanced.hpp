//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_BALANCERS_UNBALANCED_HPP
#define BOOST_TREE_BALANCERS_UNBALANCED_HPP

#include <boost/tree/detail/cursor/nary.hpp>
#include <boost/tree/inorder.hpp>

namespace boost {
namespace tree {
namespace balancers {

class unbalanced {
 public:
	unbalanced() {}

	struct metadata_type {};
	//metadata_type metadata;

// protected:	
	template <class Tree>
	static void add(Tree&, typename Tree::cursor& c)
	{ }
	  
	template <class Tree>
	static typename Tree::cursor remove(Tree& t, typename Tree::cursor& x)
	{
		//typename Tree::cursor y = x;
		if (x.begin().empty() || x.end().empty())
 			return x;
 		//inorder::forward(x);
 		return inorder::next(x);
	}
		
	template <class Tree>
	static void touch(Tree&, typename Tree::cursor&)
	{ }
};

} // namespace balancers
} // namespace tree
} // namespace boost

#endif // BOOST_TREE_BALANCERS_UNBALANCED_HPP


