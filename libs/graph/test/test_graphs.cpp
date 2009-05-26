// (C) Copyright 2009 Andrew Sutton
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#define BOOST_NO_HASH

#include "typestr.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/subgraph.hpp>

#include "test_graph.hpp"

// This test module is a testing ground to determine if graphs and graph
// adaptors actually implement the graph concepts correctly.

using namespace boost;

int main()
{
    // Bootstrap all of the tests by declaring a kind graph and  asserting some
    // basic properties about it.
    {
        typedef undirected_graph<VertexBundle, EdgeBundle> Graph;
        BOOST_META_ASSERT(is_undirected_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_mutable_graph<Graph>);
        BOOST_META_ASSERT(is_mutable_property_graph<Graph>);
        Graph g;
        test_graph(g);
    }
    {
        typedef directed_graph<VertexBundle, EdgeBundle> Graph;
        BOOST_META_ASSERT(is_directed_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(is_directed_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_mutable_graph<Graph>);
        BOOST_META_ASSERT(is_mutable_property_graph<Graph>);
        Graph g;
        test_graph(g);
    }
    {
        typedef adjacency_list<vecS, vecS, undirectedS, VertexBundle, EdgeBundle> Graph;
        BOOST_META_ASSERT(is_undirected_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_add_only_property_graph<Graph>);
        Graph g;
        test_graph(g);
    }
    {
        typedef adjacency_list<vecS, vecS, directedS, VertexBundle, EdgeBundle> Graph;
        Graph g;
        BOOST_META_ASSERT(is_directed_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(!is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(is_directed_unidirectional_graph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_add_only_property_graph<Graph>);
        test_graph(g);
    }
    {
        // Common bidi adjlist
        typedef adjacency_list<vecS, vecS, bidirectionalS, VertexBundle, EdgeBundle> Graph;
        BOOST_META_ASSERT(is_directed_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(is_directed_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_add_only_property_graph<Graph>);
        Graph g;
        test_graph(g);
    }
    {
        // Same as above, but testing VL==listS
        typedef adjacency_list<vecS, listS, bidirectionalS, VertexBundle, EdgeBundle> Graph;
        BOOST_META_ASSERT(is_directed_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(is_directed_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_mutable_property_graph<Graph>);
        Graph g;
        test_graph(g);
    }
    {
        // TODO: What other kinds of graphs do we have here...
        typedef adjacency_matrix<directedS, VertexBundle, EdgeBundle> Graph;
        BOOST_META_ASSERT(is_directed_graph<Graph>);
        BOOST_META_ASSERT(!is_multigraph<Graph>);
        BOOST_META_ASSERT(has_vertex_property<Graph>);
        BOOST_META_ASSERT(has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(has_edge_property<Graph>);
        BOOST_META_ASSERT(has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_mutable_edge_graph<Graph>);
        BOOST_META_ASSERT(is_mutable_edge_property_graph<Graph>);
        Graph g(N);
        test_graph(g);
    }
    {
        typedef labeled_graph<directed_graph<>, unsigned> Graph;
        BOOST_META_ASSERT(is_directed_graph<Graph>);
        BOOST_META_ASSERT(is_multigraph<Graph>);
        BOOST_META_ASSERT(is_incidence_graph<Graph>);
        BOOST_META_ASSERT(is_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(is_directed_bidirectional_graph<Graph>);
        BOOST_META_ASSERT(is_labeled_mutable_property_graph<Graph>);
        BOOST_META_ASSERT(is_labeled_graph<Graph>);
        BOOST_META_ASSERT(!has_vertex_property<Graph>);
        BOOST_META_ASSERT(!has_bundled_vertex_property<Graph>);
        BOOST_META_ASSERT(!has_edge_property<Graph>);
        BOOST_META_ASSERT(!has_bundled_edge_property<Graph>);
        BOOST_META_ASSERT(is_labeled_mutable_graph<Graph>);
        Graph g;
        test_graph(g);
    }
}

