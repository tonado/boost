// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_SELECTOR_COMPARE_FWD_HPP
#define BOOST_TREE_NODE_SELECTOR_COMPARE_FWD_HPP

#include <boost/tree_node/key/data.hpp>

//[reference__compare_selector_fwd
namespace boost { namespace tree_node {

    template <typename Key = data_key>
    struct compare_selector;

    typedef compare_selector<> compareS;
}}  // namespace boost::tree_node
//]

#endif  // BOOST_TREE_NODE_SELECTOR_COMPARE_FWD_HPP

