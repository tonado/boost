//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, Indiana University,
// Bloomington, IN 47405.
//
// Permission to modify the code and to distribute the code is
// granted, provided the text of this NOTICE is retained, a notice if
// the code was modified is included with the above COPYRIGHT NOTICE
// and with the COPYRIGHT NOTICE in the LICENSE file, and that the
// LICENSE file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
#include <boost/config.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

// can't do using namespace boost because then
// we get conflict with boost::default_dfs_visitor.
using namespace boost;

namespace std {
  template <typename T >
  std::istream& operator >> (std::istream & in, std::pair < T, T > &p)
  {
    in >> p.first >> p.second;
    return
      in;
  }
}

typedef adjacency_list<
  listS,                        // Store out-edges of each vertex in a std::list
  vecS,                         // Store vertex set in a std::vector
  directedS                     // The file dependency graph is directed
  > file_dep_graph;

typedef graph_traits<file_dep_graph>::vertex_descriptor vertex_t;
typedef graph_traits<file_dep_graph>::edge_descriptor edge_t;

template < typename Visitor > void
dfs_v1(const file_dep_graph & g, vertex_t u, default_color_type * color,
       Visitor vis)
{
  color[u] = gray_color;
  vis.discover_vertex(u, g);
  graph_traits < file_dep_graph >::out_edge_iterator ei, ei_end;
  for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
    if (color[target(*ei, g)] == white_color) {
      vis.tree_edge(*ei, g);
      dfs_v1(g, target(*ei, g), color, vis);
    } else if (color[target(*ei, g)] == gray_color)
      vis.back_edge(*ei, g);
    else
      vis.forward_or_cross_edge(*ei, g);
  }
  color[u] = black_color;
  vis.finish_vertex(u, g);
}

template < typename Visitor > void
generic_dfs_v1(const file_dep_graph & g, Visitor vis)
{
  std::vector < default_color_type > color(num_vertices(g), white_color);
  graph_traits < file_dep_graph >::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    if (color[*vi] == white_color)
      dfs_v1(g, *vi, &color[0], vis);
  }
}

struct dfs_visitor_default
{
  template <typename V, typename G > void
  discover_vertex(V, const G &)
  {
  }

  template <typename E, typename G > void
  tree_edge(E, const G &)
  {
  }

  template < typename E, typename G > void
  back_edge(E, const G &)
  {
  }

  template < typename E, typename G > void
  forward_or_cross_edge(E, const G &)
  {
  }

  template < typename V, typename G > void
  finish_vertex(V, const G &)
  {
  }
};

struct cycle_detector : public dfs_visitor_default
{
  cycle_detector(bool & cycle):
  has_cycle(cycle)
  {
  }
  void
  back_edge(edge_t, const file_dep_graph &)
  {
    has_cycle = true;
  }
  bool & has_cycle;
};

bool
has_cycle(const file_dep_graph & g)
{
  bool has_cycle = false;
  cycle_detector vis(has_cycle);
  generic_dfs_v1(g, vis);
  return has_cycle;
}


int
main()
{
  std::ifstream file_in("makefile-dependencies.dat");
  typedef graph_traits <file_dep_graph >::vertices_size_type size_type;
  size_type n_vertices;
  file_in >> n_vertices;        // read in number of vertices
  std::istream_iterator < std::pair < size_type,
    size_type > >input_begin(file_in), input_end;
#ifdef BOOST_MSVC
  // VC++ has trouble with the edge iterator constructor
  file_dep_graph g(n_vertices);
  while (input_begin != input_end) {
    size_type i, j;
    tie(i, j) = *input_begin++;
    add_edge(i, j, g);
  }
#else
  file_dep_graph g(input_begin, input_end, n_vertices);
#endif

  std::vector < std::string > name(num_vertices(g));
  std::ifstream name_in("makefile-target-names.dat");
  graph_traits < file_dep_graph >::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    name_in >> name[*vi];

  assert(has_cycle(g) == false);
  return 0;
}
