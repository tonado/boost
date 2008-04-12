//
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
//
#ifndef BOOST_GRAPH_CUTHILL_MCKEE_HPP
#define BOOST_GRAPH_CUTHILL_MCKEE_HPP

#include <boost/config.hpp>
#include <vector>
#include <queue>
#include <boost/pending/queue.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/property_map.hpp>

/*
  (Reverse) Cuthill-McKee Algorithm for matrix reordering
 */

namespace boost {

  namespace detail {

    // rcm_queue
    //
    // This is a custom queue type used in the
    // reverse_cuthill_mckee_ordering algorithm.
    // In addition to the normal queue operations, the
    // rcm_queue provides:
    // 
    //   int eccentricity() const;
    //   value_type spouse() const;
    // 
    template < class Vertex, class DegreeMap,
	       class Container = std::deque<Vertex> >
    class rcm_queue : public std::queue<Vertex, Container> {
      typedef std::queue<Vertex> base;
    public:
      typedef typename base::value_type value_type;
      typedef typename base::size_type size_type;

      /* SGI queue has not had a contructor queue(const Container&) */
      inline rcm_queue(DegreeMap deg)
	: _size(0), Qsize(1), eccen(-1), degree(deg) { }

      inline void pop() {
	if ( !_size ) 
	  Qsize = base::size();

	base::pop();
	if ( _size == Qsize-1 ) {
	  _size = 0;
	  ++eccen;
	} else 
	  ++_size;

      }

      inline value_type& front() {
	value_type& u =  base::front();
	if ( _size == 0 ) 
	  w = u;
	else if (get(degree,u) < get(degree,w) )
	  w = u;
	return u;
      }
      inline const value_type& front() const {
	const value_type& u =  base::front();
	if ( _size == 0 ) 
	  w = u;
	else if (get(degree,u) < get(degree,w) )
	  w = u;
	return u;
      }

      inline value_type& top() { return front(); }
      inline const value_type& top() const { return front(); }

      inline size_type size() const { return base::size(); }

      inline size_type eccentricity() const { return eccen; }
      inline value_type spouse() const { return w; }

    protected:
      size_type _size;
      size_type Qsize;
      int eccen;
      mutable value_type w;
      DegreeMap degree;
    };

  } // namespace detail  

  // Compute Pseudo peripheral
  //
  // To compute an approximated peripheral for a given vertex. 
  // Used in <tt>reverse_cuthill_mckee_ordering</tt> algorithm.
  //
  template <class Graph, class Vertex, class ColorMap, class DegreeMap>
  Vertex 
  pseudo_peripheral_pair(Graph& G, const Vertex& u, int& ecc,
			 ColorMap color, DegreeMap degree)
  {
    typedef typename property_traits<ColorMap>::value_type ColorValue;
    typedef color_traits<ColorValue> Color;

    detail::rcm_queue<Vertex, DegreeMap> Q(degree);

    typename boost::graph_traits<Graph>::vertex_iterator ui, ui_end;
    for (tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
      put(color, *ui, Color::white());
    breadth_first_search(G, u, buffer(Q).color_map(color));

    ecc = Q.eccentricity(); 
    return Q.spouse();
  }

  // Find a good starting node
  //
  // This is to find a good starting node for the
  // reverse_cuthill_mckee_ordering algorithm. "good" is in the sense
  // of the ordering generated by RCM.
  //
  template <class Graph, class Vertex, class Color, class Degree> 
  Vertex find_starting_node(Graph& G, Vertex r, Color color, Degree degree)
  {
    Vertex x, y;
    int eccen_r, eccen_x;

    x = pseudo_peripheral_pair(G, r, eccen_r, color, degree);
    y = pseudo_peripheral_pair(G, x, eccen_x, color, degree);

    while (eccen_x > eccen_r) {
      r = x;
      eccen_r = eccen_x;
      x = y;
      y = pseudo_peripheral_pair(G, x, eccen_x, color, degree);
    }
    return x;
  }


  // Reverse Cuthill-McKee algorithm with a given starting Vertex.
  //
  // This algorithm requires user to provide a starting vertex to
  // compute RCM ordering.

  template <class Graph, class OutputIterator,
            class ColorMap, class DegreeMap>
  OutputIterator
  cuthill_mckee_ordering(Graph& g,
                         typename graph_traits<Graph>::vertex_descriptor s,
                         OutputIterator inverse_permutation, 
                         ColorMap color, DegreeMap degree)
  {
    typedef typename property_traits<DegreeMap>::value_type DS;
    typedef typename property_traits<ColorMap>::value_type ColorValue;
    typedef color_traits<ColorValue> Color;
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

    typename graph_traits<Graph>::vertex_iterator ui, ui_end;
    for (tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui)
      put(color, *ui, Color::white());

    typedef indirect_cmp<DegreeMap, std::greater<DS> > Compare;
    Compare comp(degree);

    boost::queue<Vertex> bfs_queue;
    std::priority_queue<Vertex, std::vector<Vertex>, Compare> 
      degree_queue(comp);
    Vertex u, v;

    // Like BFS, except the adjacent vertices are visited
    // in increasing order of degree.
    
    put(color, s, Color::gray());
    bfs_queue.push(s);
    while (! bfs_queue.empty()) {
      u = bfs_queue.top(); bfs_queue.pop();
      *inverse_permutation++ = u;
      typename graph_traits<Graph>::out_edge_iterator ei, ei_end;
      for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei) {
	v = target(*ei, g);
        if (get(color, v) == Color::white()) {
          put(color, v, Color::gray());
	  degree_queue.push(v);
	}
      }
      while (!degree_queue.empty()) {
	v = degree_queue.top(); degree_queue.pop();
	bfs_queue.push(v);
      }
      put(color, u, Color::black());
    } // while
    return inverse_permutation;
  }  
  
  template < class Graph, class OutputIterator, 
             class Color, class Degree >
  inline OutputIterator 
  cuthill_mckee_ordering(Graph& G, OutputIterator inverse_permutation, 
                         Color color, Degree degree)
  {
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename boost::graph_traits<Graph>::vertex_iterator   VerIter;
    VerIter ri = vertices(G).first;
    Vertex r = *ri;

    //if G has several forests, how to let is cover all. ??

    Vertex s = find_starting_node(G, r, color, degree);
    return cuthill_mckee_ordering(G, s, inverse_permutation, color, degree);
  }

} // namespace boost


#endif // BOOST_GRAPH_CUTHILL_MCKEE_HPP
