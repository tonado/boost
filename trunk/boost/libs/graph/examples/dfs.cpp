//=======================================================================
// Copyright 1997, 1998, 1999, 2000 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, University of Notre Dame, Notre
// Dame, IN 46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
#include <assert.h>
#include <iostream>

#include <vector>
#include <algorithm>
#include <utility>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>

/*
  This calculates the discover finishing time.

  Sample Output

  Tree edge: 0 --> 2
  Tree edge: 2 --> 1
  Back edge: 1 --> 1
  Tree edge: 1 --> 3
  Back edge: 3 --> 1
  Tree edge: 3 --> 4
  Back edge: 4 --> 0
  Back edge: 4 --> 1
  Forward or cross edge: 2 --> 3
  1 10
  3 8
  2 9
  4 7
  5 6

 */

using namespace boost;
using namespace std;


template <class VisitorList>
struct edge_categorizer : public dfs_visitor<VisitorList> {
  typedef dfs_visitor<VisitorList> Base;

  edge_categorizer(const VisitorList& v = null_visitor()) : Base(v) { }

  template <class Edge, class Graph>
  void tree_edge(Edge e, Graph& G) {
    cout << "Tree edge: " << source(e, G) <<
      " --> " <<  target(e, G) << endl;
    Base::tree_edge(e, G);
  }
  template <class Edge, class Graph>
  void back_edge(Edge e, Graph& G) {
    cout << "Back edge: " << source(e, G)
	 << " --> " <<  target(e, G) << endl;
    Base::back_edge(e, G);
  }
  template <class Edge, class Graph>
  void forward_or_cross_edge(Edge e, Graph& G) {
    cout << "Forward or cross edge: " << source(e, G)
	 << " --> " <<  target(e, G) << endl;
    Base::forward_or_cross_edge(e, G);
  }
};
template <class VisitorList>
edge_categorizer<VisitorList>
categorize_edges(const VisitorList& v) {
  return edge_categorizer<VisitorList>(v);
}

int 
main(int argc, char* argv[])
{

  using namespace boost;
  
  typedef adjacency_list<> Graph;
  
  Graph G(5);
  add_edge(G, 0, 2);
  add_edge(G, 1, 1);
  add_edge(G, 1, 3);
  add_edge(G, 2, 1);
  add_edge(G, 2, 3);
  add_edge(G, 3, 1);
  add_edge(G, 3, 4);
  add_edge(G, 4, 0);
  add_edge(G, 4, 1);

  typedef graph_traits<Graph>::vertex_descriptor Vertex;
  typedef graph_traits<Graph>::vertices_size_type size_type;

  std::vector<default_color_type> c(num_vertices(G));
  std::vector<size_type> d(num_vertices(G));  
  std::vector<size_type> f(num_vertices(G));
  int t = 0;
  depth_first_search(G, categorize_edges(
                     make_pair(stamp_times(&d[0], t, on_discover_vertex()),
			       stamp_times(&f[0], t, on_finish_vertex()))),
                     &c[0]);


  std::vector<size_type>::iterator i, j;
  for (i = d.begin(), j = f.begin(); i != d.end(); ++i, ++j) {
    cout << *i << " " << *j << endl;
  }

  return 0;
}

