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
#ifndef BOOST_GRAPH_MATRIX2GRAPH_HPP
#define BOOST_GRAPH_MATRIX2GRAPH_HPP

#include <utility>
#include <boost/config.hpp>
#include <boost/operators.hpp>
#include <boost/int_iterator.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost {

  template <class Iter, class Vertex>
  class matrix_adj_iterator;

  template <class Iter, class Vertex>
  class matrix_incidence_iterator;

}

#define BOOST_GRAPH_ADAPT_MATRIX_TO_GRAPH(Matrix)  \
namespace boost { \
  template <> \
  struct graph_traits< Matrix > { \
    typedef Matrix::OneD::const_iterator Iter; \
    typedef Matrix::size_type V; \
    typedef V vertex_descriptor; \
    typedef Iter E; \
    typedef E edge_descriptor; \
    typedef boost::matrix_incidence_iterator<Iter, V> out_edge_iterator; \
    typedef boost::matrix_adj_iterator<Iter, V> adjacency_iterator; \
    typedef Matrix::size_type size_type; \
    typedef boost::int_iterator<size_type> vertex_iterator; \
    \
    friend std::pair<vertex_iterator, vertex_iterator> \
    vertices(const Matrix& g) { \
      typedef vertex_iterator VIter; \
      return std::make_pair(VIter(0), VIter(g.nrows())); \
    } \
    \
    friend std::pair<out_edge_iterator, out_edge_iterator> \
    out_edges(V v, const Matrix& g) { \
      typedef out_edge_iterator IncIter; \
      return std::make_pair(IncIter(g[v].begin()), \
                            IncIter(g[v].end())); \
    } \
    friend std::pair<adjacency_iterator, adjacency_iterator> \
    adjacent_vertices(V v, const Matrix& g) { \
      typedef adjacency_iterator AdjIter; \
      return std::make_pair(AdjIter(g[v].begin()), \
                            AdjIter(g[v].end())); \
    } \
    friend vertex_descriptor \
    source(E e, const Matrix& g) {  \
      return e.row();  \
    } \
    friend vertex_descriptor \
    target(E e, const Matrix& g) { \
      return e.column();  \
    } \
    friend size_type \
    num_vertices(const Matrix& g) { \
      return g.nrows();  \
    } \
    friend size_type \
    num_edges(const Matrix& g) { \
      return g.nnz(); \
    } \
    friend size_type \
    out_degree(V i, const Matrix& g) { \
      return g[i].nnz(); \
    } \
  }; \
}

namespace boost {

  template <class Iter, class Vertex>
  class matrix_adj_iterator
    : public std::iterator<std::input_iterator_tag, Vertex >
  {
    typedef matrix_adj_iterator self;
  public:
    matrix_adj_iterator() { }
    matrix_adj_iterator(Iter i) : _iter(i) { }
    matrix_adj_iterator(const self& x) : _iter(x._iter) { }
    self& operator=(const self& x) { _iter = x._iter; return *this; }
    Vertex operator*() { return _iter.column(); }
    self& operator++() { ++_iter; return *this; }
    self operator++(int) { self t = *this; ++_iter; return t; }
    bool operator==(const self& x) const { return _iter == x._iter; }
    bool operator!=(const self& x) const { return _iter != x._iter; }
  protected:
    Iter _iter;
  };

  template <class Iter, class Vertex>
  class matrix_incidence_iterator
    : public std::iterator<std::input_iterator_tag, Iter >
  {
    typedef matrix_incidence_iterator self;
  public:
    matrix_incidence_iterator() { }
    matrix_incidence_iterator(Iter i) : _iter(i) { }
    matrix_incidence_iterator(const self& x) : _iter(x._iter) { }
    self& operator=(const self& x) { _iter = x._iter; return *this; }
    Iter operator*() { return _iter; }
    self& operator++() { ++_iter; return *this; }
    self operator++(int) { self t = *this; ++_iter; return t; }
    bool operator==(const self& x) const { return _iter == x._iter; }
    bool operator!=(const self& x) const { return _iter != x._iter; }
  protected:
    Iter _iter;
  };
  
} /* namespace boost */

#endif /* BOOST_GRAPH_MATRIX2GRAPH_HPP*/
