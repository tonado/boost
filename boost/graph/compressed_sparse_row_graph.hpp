// Copyright 2005-2009 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Jeremiah Willcock
//           Douglas Gregor
//           Andrew Lumsdaine

// Compressed sparse row graph type

#ifndef BOOST_GRAPH_COMPRESSED_SPARSE_ROW_GRAPH_HPP
#define BOOST_GRAPH_COMPRESSED_SPARSE_ROW_GRAPH_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <climits>
#include <cassert>
#include <iterator>
#if 0
#include <iostream> // For some debugging code below
#endif
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/filtered_graph.hpp> // For keep_all
#include <boost/graph/detail/indexed_properties.hpp>
#include <boost/graph/detail/compressed_sparse_row_struct.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/integer.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/mpl/if.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <boost/static_assert.hpp>
#include <boost/functional/hash.hpp>
#include <boost/utility.hpp>

#ifdef BOOST_GRAPH_NO_BUNDLED_PROPERTIES
#  error The Compressed Sparse Row graph only supports bundled properties.
#  error You will need a compiler that conforms better to the C++ standard.
#endif

#ifndef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
#warning "Using deprecated BGL compressed sparse row graph interface --"
#warning "please see the documentation for the new interface and then"
#warning "#define BOOST_GRAPH_USE_NEW_CSR_INTERFACE before including"
#warning "<boost/graph/compressed_sparse_row_graph.hpp>"
#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE

#ifndef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
#define BOOST_GRAPH_USE_OLD_CSR_INTERFACE
#endif

namespace boost {

// A tag type indicating that the graph in question is a compressed
// sparse row graph. This is an internal detail of the BGL.
struct csr_graph_tag;

// A type (edges_are_sorted_t) and a value (edges_are_sorted) used to indicate
// that the edge list passed into the CSR graph is already sorted by source
// vertex.
enum edges_are_sorted_t {edges_are_sorted};

#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
// A type (edges_are_sorted_global_t) and a value (edges_are_sorted_global)
// used to indicate that the edge list passed into the CSR graph is already
// sorted by source vertex.
enum edges_are_sorted_global_t {edges_are_sorted_global};

// A type (edges_are_unsorted_t) and a value (edges_are_unsorted) used to
// indicate that the edge list passed into the CSR graph is not sorted by
// source vertex.  This version caches the edge information in memory, and thus
// requires only a single pass over the input data.
enum edges_are_unsorted_t {edges_are_unsorted};

// A type (edges_are_unsorted_multi_pass_t) and a value
// (edges_are_unsorted_multi_pass) used to indicate that the edge list passed
// into the CSR graph is not sorted by source vertex.  This version uses less
// memory but requires multi-pass capability on the iterators.
enum edges_are_unsorted_multi_pass_t {edges_are_unsorted_multi_pass};

// A type (edges_are_unsorted_multi_pass_global_t) and a value
// (edges_are_unsorted_multi_pass_global) used to indicate that the edge list
// passed into the CSR graph is not sorted by source vertex.  This version uses
// less memory but requires multi-pass capability on the iterators.  The
// global mapping and filtering is done here because it is often faster and it
// greatly simplifies handling of edge properties.
enum edges_are_unsorted_multi_pass_global_t {edges_are_unsorted_multi_pass_global};

// A type (construct_inplace_from_sources_and_targets_t) and a value
// (construct_inplace_from_sources_and_targets) used to indicate that mutable
// vectors of sources and targets (and possibly edge properties) are being used
// to construct the CSR graph.  These vectors are sorted in-place and then the
// targets and properties are swapped into the graph data structure.
enum construct_inplace_from_sources_and_targets_t {construct_inplace_from_sources_and_targets};

// A type (construct_inplace_from_sources_and_targets_global_t) and a value
// (construct_inplace_from_sources_and_targets_global) used to indicate that
// mutable vectors of sources and targets (and possibly edge properties) are
// being used to construct the CSR graph.  These vectors are sorted in-place
// and then the targets and properties are swapped into the graph data
// structure.  It is assumed that global indices (for distributed CSR) are
// used, and a map is required to convert those to local indices.  This
// constructor is intended for internal use by the various CSR graphs
// (sequential and distributed).
enum construct_inplace_from_sources_and_targets_global_t {construct_inplace_from_sources_and_targets_global};

// A type (edges_are_unsorted_global_t) and a value (edges_are_unsorted_global)
// used to indicate that the edge list passed into the CSR graph is not sorted
// by source vertex.  The data is also stored using global vertex indices, and
// must be filtered to choose only local vertices.  This constructor caches the
// edge information in memory, and thus requires only a single pass over the
// input data.  This constructor is intended for internal use by the
// distributed CSR constructors.
enum edges_are_unsorted_global_t {edges_are_unsorted_global};

#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE

/****************************************************************************
 * Local helper macros to reduce typing and clutter later on.               *
 ****************************************************************************/
#define BOOST_CSR_GRAPH_TEMPLATE_PARMS                                  \
  typename Directed, typename VertexProperty, typename EdgeProperty,    \
  typename GraphProperty, typename Vertex, typename EdgeIndex
#define BOOST_CSR_GRAPH_TYPE                                            \
   compressed_sparse_row_graph<Directed, VertexProperty, EdgeProperty,  \
                               GraphProperty, Vertex, EdgeIndex>

#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
namespace detail {
  template <typename T>
  struct default_construct_iterator: public boost::iterator_facade<default_construct_iterator<T>, T, boost::random_access_traversal_tag, const T&> {
    typedef boost::iterator_facade<default_construct_iterator<T>, T, std::random_access_iterator_tag, const T&> base_type;
    T saved_value;
    const T& dereference() const {return saved_value;}
    bool equal(default_construct_iterator i) const {return true;}
    void increment() {}
    void decrement() {}
    void advance(typename base_type::difference_type) {}
    typename base_type::difference_type distance_to(default_construct_iterator) const {return 0;}
  };

  template <typename Less>
  struct compare_first {
    Less less;
    compare_first(Less less = Less()): less(less) {}
    template <typename Tuple>
    bool operator()(const Tuple& a, const Tuple& b) const {
      return less(a.template get<0>(), b.template get<0>());
    }
  };

  template <int N, typename Result>
  struct my_tuple_get_class {
    typedef const Result& result_type;
    template <typename Tuple>
    result_type operator()(const Tuple& t) const {
      return t.template get<N>();
    }
  };
}
#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE

/** Compressed sparse row graph.
 *
 * Vertex and EdgeIndex should be unsigned integral types and should
 * specialize numeric_limits.
 */
template<typename Directed = directedS, 
         typename VertexProperty = no_property,
         typename EdgeProperty = no_property,
         typename GraphProperty = no_property,
         typename Vertex = std::size_t,
         typename EdgeIndex = Vertex>
class compressed_sparse_row_graph
   : public detail::indexed_vertex_properties<BOOST_CSR_GRAPH_TYPE, VertexProperty,
                                              Vertex>

{
 public:
  typedef detail::indexed_vertex_properties<compressed_sparse_row_graph,
                                            VertexProperty, Vertex>
    inherited_vertex_properties;

 public:
  // For Property Graph
  typedef GraphProperty graph_property_type;

  typedef detail::compressed_sparse_row_structure<EdgeProperty, Vertex, EdgeIndex> forward_type;

 public:
  /* At this time, the compressed sparse row graph can only be used to
   * create a directed graph. In the future, bidirectional and
   * undirected CSR graphs will also be supported.
   */
  BOOST_STATIC_ASSERT((is_same<Directed, directedS>::value));

  // Concept requirements:
  // For Graph
  typedef Vertex vertex_descriptor;
  typedef csr_edge_descriptor<Vertex, EdgeIndex> edge_descriptor;
  typedef directed_tag directed_category;
  typedef allow_parallel_edge_tag edge_parallel_category;

  class traversal_category: public incidence_graph_tag,
                            public adjacency_graph_tag,
                            public vertex_list_graph_tag,
                            public edge_list_graph_tag {};

  static vertex_descriptor null_vertex() { return vertex_descriptor(-1); }

  // For VertexListGraph
  typedef counting_iterator<Vertex> vertex_iterator;
  typedef Vertex vertices_size_type;

  // For EdgeListGraph
  typedef EdgeIndex edges_size_type;

  // For IncidenceGraph
  class out_edge_iterator;
  typedef EdgeIndex degree_size_type;

  // For AdjacencyGraph
  typedef typename std::vector<Vertex>::const_iterator adjacency_iterator;

  // For EdgeListGraph
  class edge_iterator;

  // For BidirectionalGraph (not implemented)
  typedef void in_edge_iterator;

  // For internal use
  typedef csr_graph_tag graph_tag;

  typedef typename forward_type::inherited_edge_properties::edge_bundled edge_bundled;
  typedef typename forward_type::inherited_edge_properties::edge_push_back_type edge_push_back_type;
  typedef typename forward_type::inherited_edge_properties::edge_property_type edge_property_type;

  // Constructors

  // Default constructor: an empty graph.
  compressed_sparse_row_graph(): m_property() {}

  //  With numverts vertices
  compressed_sparse_row_graph(vertices_size_type numverts)
    : inherited_vertex_properties(numverts), m_forward(numverts) {}

#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  //  Rebuild graph from number of vertices and multi-pass unsorted list of
  //  edges (filtered using source_pred and mapped using global_to_local)
  template <typename MultiPassInputIterator, typename GlobalToLocal, typename SourcePred>
  void
  assign_unsorted_multi_pass_edges(MultiPassInputIterator edge_begin,
                                   MultiPassInputIterator edge_end,
                                   vertices_size_type numlocalverts,
                                   const GlobalToLocal& global_to_local,
                                   const SourcePred& source_pred) {
    m_forward.assign_unsorted_multi_pass_edges(edge_begin, edge_end, numlocalverts, global_to_local, source_pred);
  }

  //  Rebuild graph from number of vertices and multi-pass unsorted list of
  //  edges and their properties (filtered using source_pred and mapped using
  //  global_to_local)
  template <typename MultiPassInputIterator, typename EdgePropertyIterator, typename GlobalToLocal, typename SourcePred>
  void
  assign_unsorted_multi_pass_edges(MultiPassInputIterator edge_begin,
                                   MultiPassInputIterator edge_end,
                                   EdgePropertyIterator ep_iter,
                                   vertices_size_type numlocalverts,
                                   const GlobalToLocal& global_to_local,
                                   const SourcePred& source_pred) {
    m_forward.assign_unsorted_multi_pass_edges(edge_begin, edge_end, ep_iter, numlocalverts, global_to_local, source_pred);
  }

  //  From number of vertices and unsorted list of edges
  template <typename MultiPassInputIterator>
  compressed_sparse_row_graph(edges_are_unsorted_multi_pass_t,
                              MultiPassInputIterator edge_begin,
                              MultiPassInputIterator edge_end,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numverts), m_property(prop)
  {
    m_forward.assign_unsorted_multi_pass_edges(edge_begin, edge_end, numverts, identity_property_map(), keep_all());
  }

  //  From number of vertices and unsorted list of edges, plus edge properties
  template <typename MultiPassInputIterator, typename EdgePropertyIterator>
  compressed_sparse_row_graph(edges_are_unsorted_multi_pass_t,
                              MultiPassInputIterator edge_begin,
                              MultiPassInputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numverts), m_forward(), m_property(prop)
  {
    m_forward.assign_unsorted_multi_pass_edges(edge_begin, edge_end, ep_iter, numverts, identity_property_map(), keep_all());
  }

  //  From number of vertices and unsorted list of edges, with filter and
  //  global-to-local map
  template <typename MultiPassInputIterator, typename GlobalToLocal, typename SourcePred>
  compressed_sparse_row_graph(edges_are_unsorted_multi_pass_global_t,
                              MultiPassInputIterator edge_begin,
                              MultiPassInputIterator edge_end,
                              vertices_size_type numlocalverts,
                              const GlobalToLocal& global_to_local,
                              const SourcePred& source_pred,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numlocalverts), m_forward(), m_property(prop)
  {
    m_forward.assign_unsorted_multi_pass_edges(edge_begin, edge_end, numlocalverts, global_to_local, source_pred);
  }

  //  From number of vertices and unsorted list of edges, plus edge properties,
  //  with filter and global-to-local map
  template <typename MultiPassInputIterator, typename EdgePropertyIterator, typename GlobalToLocal, typename SourcePred>
  compressed_sparse_row_graph(edges_are_unsorted_multi_pass_global_t,
                              MultiPassInputIterator edge_begin,
                              MultiPassInputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              vertices_size_type numlocalverts,
                              const GlobalToLocal& global_to_local,
                              const SourcePred& source_pred,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numlocalverts), m_forward(), m_property(prop)
  {
    m_forward.assign_unsorted_multi_pass_edges(edge_begin, edge_end, ep_iter, numlocalverts, global_to_local, source_pred);
  }
#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE

  //  Assign from number of vertices and sorted list of edges
  template<typename InputIterator, typename GlobalToLocal, typename SourcePred>
  void assign_from_sorted_edges(
         InputIterator edge_begin, InputIterator edge_end,
         const GlobalToLocal& global_to_local,
         const SourcePred& source_pred,
         vertices_size_type numlocalverts,
         edges_size_type numedges_or_zero) {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, global_to_local, source_pred, numlocalverts, numedges_or_zero);
    inherited_vertex_properties::resize(numlocalverts);
  }

  //  Assign from number of vertices and sorted list of edges
  template<typename InputIterator, typename EdgePropertyIterator, typename GlobalToLocal, typename SourcePred>
  void assign_from_sorted_edges(
         InputIterator edge_begin, InputIterator edge_end,
         EdgePropertyIterator ep_iter,
         const GlobalToLocal& global_to_local,
         const SourcePred& source_pred,
         vertices_size_type numlocalverts,
         edges_size_type numedges_or_zero) {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, ep_iter, global_to_local, source_pred, numlocalverts, numedges_or_zero);
    inherited_vertex_properties::resize(numlocalverts);
  }

#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE

  //  From number of vertices and sorted list of edges (deprecated
  //  interface)
  template<typename InputIterator>
  compressed_sparse_row_graph(InputIterator edge_begin, InputIterator edge_end,
                              vertices_size_type numverts,
                              edges_size_type numedges = 0,
                              const GraphProperty& prop = GraphProperty())
    : m_property(prop)
  {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, identity_property_map(), keep_all(), numverts, numedges);
    inherited_vertex_properties::resize(numlocalverts);
  }

  //  From number of vertices and sorted list of edges (deprecated
  //  interface)
  template<typename InputIterator, typename EdgePropertyIterator>
  compressed_sparse_row_graph(InputIterator edge_begin, InputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              vertices_size_type numverts,
                              edges_size_type numedges = 0,
                              const GraphProperty& prop = GraphProperty())
    : m_property(prop)
  {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, ep_iter, identity_property_map(), keep_all(), numverts, numedges);
    inherited_vertex_properties::resize(numlocalverts);
  }

#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE

  //  From number of vertices and sorted list of edges (new interface)
  template<typename InputIterator>
  compressed_sparse_row_graph(edges_are_sorted_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              vertices_size_type numverts,
                              edges_size_type numedges = 0,
                              const GraphProperty& prop = GraphProperty())
    : m_property(prop)
  {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, identity_property_map(), keep_all(), numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

  //  From number of vertices and sorted list of edges (new interface)
  template<typename InputIterator, typename EdgePropertyIterator>
  compressed_sparse_row_graph(edges_are_sorted_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              vertices_size_type numverts,
                              edges_size_type numedges = 0,
                              const GraphProperty& prop = GraphProperty())
    : m_property(prop)
  {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, ep_iter, identity_property_map(), keep_all(), numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  //  From number of vertices and sorted list of edges, filtered and global (new interface)
  template<typename InputIterator, typename GlobalToLocal, typename SourcePred>
  compressed_sparse_row_graph(edges_are_sorted_global_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              const GlobalToLocal& global_to_local,
                              const SourcePred& source_pred,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : m_property(prop)
  {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, global_to_local, source_pred, numverts, 0);
    inherited_vertex_properties::resize(numverts);
  }

  //  From number of vertices and sorted list of edges (new interface)
  template<typename InputIterator, typename EdgePropertyIterator, typename GlobalToLocal, typename SourcePred>
  compressed_sparse_row_graph(edges_are_sorted_global_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              const GlobalToLocal& global_to_local,
                              const SourcePred& source_pred,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : m_property(prop)
  {
    m_forward.assign_from_sorted_edges(edge_begin, edge_end, ep_iter, global_to_local, source_pred, numverts, 0);
    inherited_vertex_properties::resize(numverts);
  }

  //  From number of vertices and mutable vectors of sources and targets;
  //  vectors are returned with unspecified contents but are guaranteed not to
  //  share storage with the constructed graph.
  compressed_sparse_row_graph(construct_inplace_from_sources_and_targets_t,
                              std::vector<vertex_descriptor>& sources,
                              std::vector<vertex_descriptor>& targets,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numverts), m_property(prop)
  {
    m_forward.assign_sources_and_targets_global(sources, targets, numverts, boost::identity_property_map());
  }

  //  From number of vertices and mutable vectors of sources and targets,
  //  expressed with global vertex indices; vectors are returned with
  //  unspecified contents but are guaranteed not to share storage with the
  //  constructed graph.  This constructor should only be used by the
  //  distributed CSR graph.
  template <typename GlobalToLocal>
  compressed_sparse_row_graph(construct_inplace_from_sources_and_targets_global_t,
                              std::vector<vertex_descriptor>& sources,
                              std::vector<vertex_descriptor>& targets,
                              vertices_size_type numlocalverts,
                              GlobalToLocal global_to_local,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numlocalverts), m_property(prop)
  {
    m_forward.assign_sources_and_targets_global(sources, targets, numlocalverts, global_to_local);
  }

  //  From number of vertices and mutable vectors of sources, targets, and edge
  //  properties; vectors are returned with unspecified contents but are
  //  guaranteed not to share storage with the constructed graph.
  compressed_sparse_row_graph(construct_inplace_from_sources_and_targets_t,
                              std::vector<vertex_descriptor>& sources,
                              std::vector<vertex_descriptor>& targets,
                              std::vector<typename forward_type::inherited_edge_properties::edge_bundled>& edge_props,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numverts), m_property(prop)
  {
    m_forward.assign_sources_and_targets_global(sources, targets, edge_props, numverts, boost::identity_property_map());
  }

  //  From number of vertices and mutable vectors of sources and targets and
  //  edge properties, expressed with global vertex indices; vectors are
  //  returned with unspecified contents but are guaranteed not to share
  //  storage with the constructed graph.  This constructor should only be used
  //  by the distributed CSR graph.
  template <typename GlobalToLocal>
  compressed_sparse_row_graph(construct_inplace_from_sources_and_targets_global_t,
                              std::vector<vertex_descriptor>& sources,
                              std::vector<vertex_descriptor>& targets,
                              std::vector<typename forward_type::inherited_edge_properties::edge_bundled>& edge_props,
                              vertices_size_type numlocalverts,
                              GlobalToLocal global_to_local,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numlocalverts), m_property(prop)
  {
    m_forward.assign_sources_and_targets_global(sources, targets, edge_props, numlocalverts, global_to_local);
  }

  //  From number of vertices and single-pass range of unsorted edges.  Data is
  //  cached in coordinate form before creating the actual graph.
  template<typename InputIterator>
  compressed_sparse_row_graph(edges_are_unsorted_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numverts), m_property(prop)
  {
    std::vector<vertex_descriptor> sources, targets;
    boost::graph::detail::split_into_separate_coords
      (edge_begin, edge_end, sources, targets);
    assign_sources_and_targets_global(sources, targets, numverts, boost::identity_property_map());
  }

  //  From number of vertices and single-pass range of unsorted edges and
  //  single-pass range of edge properties.  Data is cached in coordinate form
  //  before creating the actual graph.
  template<typename InputIterator, typename EdgePropertyIterator>
  compressed_sparse_row_graph(edges_are_unsorted_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              vertices_size_type numverts,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numverts), m_property(prop)
  {
    std::vector<vertex_descriptor> sources, targets;
    boost::graph::detail::split_into_separate_coords
      (edge_begin, edge_end, sources, targets);
    size_t numedges = sources.size();
    std::vector<typename forward_type::inherited_edge_properties::edge_bundled> edge_props(numedges);
    for (size_t i = 0; i < numedges; ++i) {
      edge_props[i] = *ep_iter++;
    }
    m_forward.assign_sources_and_targets_global(sources, targets, edge_props, numverts, boost::identity_property_map());
  }

  //  From number of vertices and single-pass range of unsorted edges.  Data is
  //  cached in coordinate form before creating the actual graph.  Edges are
  //  filtered and transformed for use in a distributed graph.
  template<typename InputIterator, typename GlobalToLocal, typename SourcePred>
  compressed_sparse_row_graph(edges_are_unsorted_global_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              vertices_size_type numlocalverts,
                              GlobalToLocal global_to_local,
                              const SourcePred& source_pred,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numlocalverts), m_property(prop)
  {
    std::vector<vertex_descriptor> sources, targets;
    boost::graph::detail::split_into_separate_coords_filtered
      (edge_begin, edge_end, sources, targets, source_pred);
    m_forward.assign_sources_and_targets_global(sources, targets, numlocalverts, global_to_local);
  }

  //  From number of vertices and single-pass range of unsorted edges and
  //  single-pass range of edge properties.  Data is cached in coordinate form
  //  before creating the actual graph.  Edges are filtered and transformed for
  //  use in a distributed graph.
  template<typename InputIterator, typename EdgePropertyIterator,
           typename GlobalToLocal, typename SourcePred>
  compressed_sparse_row_graph(edges_are_unsorted_global_t,
                              InputIterator edge_begin, InputIterator edge_end,
                              EdgePropertyIterator ep_iter,
                              vertices_size_type numlocalverts,
                              GlobalToLocal global_to_local,
                              const SourcePred& source_pred,
                              const GraphProperty& prop = GraphProperty())
    : inherited_vertex_properties(numlocalverts), m_property(prop)
  {
    std::vector<vertex_descriptor> sources, targets;
    std::vector<edge_bundled> edge_props;
    boost::graph::detail::split_into_separate_coords_filtered
      (edge_begin, edge_end, ep_iter, sources, targets, edge_props, source_pred);
    m_forward.assign_sources_and_targets_global(sources, targets, edge_props, numlocalverts, global_to_local);
  }

  // Replace graph with sources and targets given, sorting them in-place, and
  // using the given global-to-local property map to get local indices from
  // global ones in the two arrays.
  template <typename GlobalToLocal>
  void assign_sources_and_targets_global(std::vector<vertex_descriptor>& sources,
                                         std::vector<vertex_descriptor>& targets,
                                         vertices_size_type numverts,
                                         GlobalToLocal global_to_local) {
    m_forward.assign_sources_and_targets_global(sources, targets, numverts, global_to_local);
  }

  // Replace graph with sources and targets and edge properties given, sorting
  // them in-place, and using the given global-to-local property map to get
  // local indices from global ones in the two arrays.
  template <typename GlobalToLocal>
  void assign_sources_and_targets_global(std::vector<vertex_descriptor>& sources,
                                         std::vector<vertex_descriptor>& targets,
                                         std::vector<edge_bundled>& edge_props,
                                         vertices_size_type numverts,
                                         GlobalToLocal global_to_local) {
    m_forward.assign_sources_and_targets_global(sources, targets, edge_props, numverts, global_to_local);
  }

#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE


  //   Requires IncidenceGraph and a vertex index map
  template<typename Graph, typename VertexIndexMap>
  compressed_sparse_row_graph(const Graph& g, const VertexIndexMap& vi,
                              vertices_size_type numverts,
                              edges_size_type numedges)
    : m_property()
  {
    assign(g, vi, numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

  //   Requires VertexListGraph and EdgeListGraph
  template<typename Graph, typename VertexIndexMap>
  compressed_sparse_row_graph(const Graph& g, const VertexIndexMap& vi)
    : m_property()
  {
    typename graph_traits<Graph>::edges_size_type numedges = num_edges(g);
    if (is_same<typename graph_traits<Graph>::directed_category, undirectedS>::value) {
      numedges *= 2; // Double each edge (actual doubling done by out_edges function)
    }
    vertices_size_type numverts = num_vertices(g);
    assign(g, vi, numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

  // Requires vertex index map plus requirements of previous constructor
  template<typename Graph>
  explicit compressed_sparse_row_graph(const Graph& g)
    : m_property()
  {
    typename graph_traits<Graph>::edges_size_type numedges = num_edges(g);
    if (is_same<typename graph_traits<Graph>::directed_category, undirectedS>::value) {
      numedges *= 2; // Double each edge (actual doubling done by out_edges function)
    }
    assign(g, get(vertex_index, g), num_vertices(g), numedges);
  }

  // From any graph (slow and uses a lot of memory)
  //   Requires IncidenceGraph and a vertex index map
  //   Internal helper function
  //   Note that numedges must be doubled for undirected source graphs
  template<typename Graph, typename VertexIndexMap>
  void
  assign(const Graph& g, const VertexIndexMap& vi,
         vertices_size_type numverts, edges_size_type numedges)
  {
    m_forward.assign(g, vi, numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

  // Requires the above, plus VertexListGraph and EdgeListGraph
  template<typename Graph, typename VertexIndexMap>
  void assign(const Graph& g, const VertexIndexMap& vi)
  {
    typename graph_traits<Graph>::edges_size_type numedges = num_edges(g);
    if (is_same<typename graph_traits<Graph>::directed_category, undirectedS>::value) {
      numedges *= 2; // Double each edge (actual doubling done by out_edges function)
    }
    vertices_size_type numverts = num_vertices(g);
    m_forward.assign(g, vi, numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

  // Requires the above, plus a vertex_index map.
  template<typename Graph>
  void assign(const Graph& g)
  {
    typename graph_traits<Graph>::edges_size_type numedges = num_edges(g);
    if (is_same<typename graph_traits<Graph>::directed_category, undirectedS>::value) {
      numedges *= 2; // Double each edge (actual doubling done by out_edges function)
    }
    vertices_size_type numverts = num_vertices(g);
    m_forward.assign(g, get(vertex_index, g), numverts, numedges);
    inherited_vertex_properties::resize(numverts);
  }

#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  // Add edges from a sorted (smallest sources first) range of pairs and edge
  // properties
  template <typename BidirectionalIteratorOrig, typename EPIterOrig,
            typename GlobalToLocal>
  void
  add_edges_sorted_internal(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      EPIterOrig ep_iter_sorted,
      const GlobalToLocal& global_to_local) {
    m_forward.add_edges_sorted_internal(first_sorted, last_sorted, ep_iter_sorted, global_to_local);
  }

  template <typename BidirectionalIteratorOrig, typename EPIterOrig>
  void
  add_edges_sorted_internal(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      EPIterOrig ep_iter_sorted)  {
    m_forward.add_edges_sorted_internal(first_sorted, last_sorted, ep_iter_sorted, identity_property_map());
  }

  // Add edges from a sorted (smallest sources first) range of pairs
  template <typename BidirectionalIteratorOrig>
  void
  add_edges_sorted_internal(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted) {
    m_forward.add_edges_sorted_internal(first_sorted, last_sorted, detail::default_construct_iterator<edge_bundled>());
  }

  template <typename BidirectionalIteratorOrig, typename GlobalToLocal>
  void
  add_edges_sorted_internal_global(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      const GlobalToLocal& global_to_local) {
    m_forward.add_edges_sorted_internal(first_sorted, last_sorted, detail::default_construct_iterator<edge_bundled>(), global_to_local);
  }

  template <typename BidirectionalIteratorOrig, typename EPIterOrig, 
            typename GlobalToLocal>
  void
  add_edges_sorted_internal_global(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      EPIterOrig ep_iter_sorted,
      const GlobalToLocal& global_to_local) {
    m_forward.add_edges_sorted_internal(first_sorted, last_sorted, ep_iter_sorted, global_to_local);
  }

  // Add edges from a range of (source, target) pairs that are unsorted
  template <typename InputIterator, typename GlobalToLocal>
  inline void
  add_edges_internal(InputIterator first, InputIterator last, 
                     const GlobalToLocal& global_to_local) {
    typedef compressed_sparse_row_graph Graph;
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_num;
    typedef typename boost::graph_traits<Graph>::edges_size_type edge_num;
    typedef std::vector<std::pair<vertex_t, vertex_t> > edge_vector_t;
    edge_vector_t new_edges(first, last);
    if (new_edges.empty()) return;
    std::sort(new_edges.begin(), new_edges.end());
    this->add_edges_sorted_internal_global(new_edges.begin(), new_edges.end(), global_to_local);
  }

  template <typename InputIterator>
  inline void
  add_edges_internal(InputIterator first, InputIterator last) {
    this->add_edges_internal(first, last, identity_property_map());
  }

  // Add edges from a range of (source, target) pairs and edge properties that
  // are unsorted
  template <typename InputIterator, typename EPIterator, typename GlobalToLocal>
  inline void
  add_edges_internal(InputIterator first, InputIterator last,
                     EPIterator ep_iter, EPIterator ep_iter_end,
                     const GlobalToLocal& global_to_local) {
    typedef compressed_sparse_row_graph Graph;
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_t;
    typedef typename boost::graph_traits<Graph>::vertices_size_type vertex_num;
    typedef typename boost::graph_traits<Graph>::edges_size_type edge_num;
    typedef std::pair<vertex_t, vertex_t> vertex_pair;
    typedef std::vector<
              boost::tuple<vertex_pair,
                           edge_bundled> >
      edge_vector_t;
    edge_vector_t new_edges
      (boost::make_zip_iterator(boost::make_tuple(first, ep_iter)),
       boost::make_zip_iterator(boost::make_tuple(last, ep_iter_end)));
    if (new_edges.empty()) return;
    std::sort(new_edges.begin(), new_edges.end(),
              boost::detail::compare_first<
                std::less<vertex_pair> >());
    m_forward.add_edges_sorted_internal
      (boost::make_transform_iterator(
         new_edges.begin(),
         boost::detail::my_tuple_get_class<0, vertex_pair>()),
       boost::make_transform_iterator(
         new_edges.end(),
         boost::detail::my_tuple_get_class<0, vertex_pair>()),
       boost::make_transform_iterator(
         new_edges.begin(),
         boost::detail::my_tuple_get_class
           <1, edge_bundled>()),
       global_to_local);
  }

  // Add edges from a range of (source, target) pairs and edge properties that
  // are unsorted
  template <typename InputIterator, typename EPIterator>
  inline void
  add_edges_internal(InputIterator first, InputIterator last,
                     EPIterator ep_iter, EPIterator ep_iter_end) {
    this->add_edges_internal(first, last, ep_iter, ep_iter_end, identity_property_map());
  }
#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE

  using inherited_vertex_properties::operator[];

  // Directly access a edge or edge bundle
  edge_push_back_type& operator[](const edge_descriptor& v)
  { return m_forward.m_edge_properties[get(edge_index, *this, v)]; }

  const edge_push_back_type& operator[](const edge_descriptor& v) const
  { return m_forward.m_edge_properties[get(edge_index, *this, v)]; }

  // private: non-portable, requires friend templates
  inherited_vertex_properties&       vertex_properties()       {return *this;}
  const inherited_vertex_properties& vertex_properties() const {return *this;}
  typename forward_type::inherited_edge_properties&       edge_properties()       { return m_forward; }
  const typename forward_type::inherited_edge_properties& edge_properties() const { return m_forward; }

  forward_type m_forward;
  GraphProperty m_property;
};

template<typename Vertex, typename EdgeIndex>
class csr_edge_descriptor
{
 public:
  Vertex src;
  EdgeIndex idx;

  csr_edge_descriptor(Vertex src, EdgeIndex idx): src(src), idx(idx) {}
  csr_edge_descriptor(): src(0), idx(0) {}

  bool operator==(const csr_edge_descriptor& e) const {return idx == e.idx;}
  bool operator!=(const csr_edge_descriptor& e) const {return idx != e.idx;}
  bool operator<(const csr_edge_descriptor& e) const {return idx < e.idx;}
  bool operator>(const csr_edge_descriptor& e) const {return idx > e.idx;}
  bool operator<=(const csr_edge_descriptor& e) const {return idx <= e.idx;}
  bool operator>=(const csr_edge_descriptor& e) const {return idx >= e.idx;}

  template<typename Archiver>
  void serialize(Archiver& ar, const unsigned int /*version*/)
  {
    ar & src & idx;
  }
};

template<typename Vertex, typename EdgeIndex>
struct hash<csr_edge_descriptor<Vertex, EdgeIndex> >
{
  std::size_t operator()(csr_edge_descriptor<Vertex, EdgeIndex> const& x) const
  {
    std::size_t hash = hash_value(x.src);
    hash_combine(hash, x.idx);
    return hash;
  }
};

// Construction functions
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
add_vertex(BOOST_CSR_GRAPH_TYPE& g) {
  Vertex old_num_verts_plus_one = g.m_forward.m_rowstart.size();
  EdgeIndex numedges = g.m_forward.m_rowstart.back();
  g.m_forward.m_rowstart.push_back(numedges);
  g.vertex_properties().resize(num_vertices(g));
  return old_num_verts_plus_one - 1;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
add_vertex(BOOST_CSR_GRAPH_TYPE& g, 
           typename BOOST_CSR_GRAPH_TYPE::vertex_bundled const& p) {
  Vertex old_num_verts_plus_one = g.m_forward.m_rowstart.size();
  g.m_forward.m_rowstart.push_back(EdgeIndex(0));
  g.vertex_properties().push_back(p);
  return old_num_verts_plus_one - 1;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
add_vertices(typename BOOST_CSR_GRAPH_TYPE::vertices_size_type count, BOOST_CSR_GRAPH_TYPE& g) {
  Vertex old_num_verts_plus_one = g.m_forward.m_rowstart.size();
  EdgeIndex numedges = g.m_forward.m_rowstart.back();
  g.m_forward.m_rowstart.resize(old_num_verts_plus_one + count, numedges);
  g.vertex_properties().resize(num_vertices(g));
  return old_num_verts_plus_one - 1;
}

#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
// This function requires that (src, tgt) be lexicographically at least as
// large as the largest edge in the graph so far
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline typename BOOST_CSR_GRAPH_TYPE::edge_descriptor
add_edge(Vertex src, Vertex tgt, BOOST_CSR_GRAPH_TYPE& g) {
  assert ((g.m_last_source == 0 || src >= g.m_last_source - 1) &&
          src < num_vertices(g));
  EdgeIndex num_edges_orig = g.m_forward.m_column.size();
  for (; g.m_last_source <= src; ++g.m_last_source)
    g.m_forward.m_rowstart[g.m_last_source] = num_edges_orig;
  g.m_forward.m_rowstart[src + 1] = num_edges_orig + 1;
  g.m_forward.m_column.push_back(tgt);
  typedef typename BOOST_CSR_GRAPH_TYPE::edge_push_back_type push_back_type;
  g.edge_properties().push_back(push_back_type());
  return typename BOOST_CSR_GRAPH_TYPE::edge_descriptor(src, num_edges_orig);
}

// This function requires that src be at least as large as the largest source
// in the graph so far
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline typename BOOST_CSR_GRAPH_TYPE::edge_descriptor
add_edge(Vertex src, Vertex tgt,
         typename BOOST_CSR_GRAPH_TYPE::edge_bundled const& p,
         BOOST_CSR_GRAPH_TYPE& g) {
  assert ((g.m_last_source == 0 || src >= g.m_last_source - 1) &&
          src < num_vertices(g));
  EdgeIndex num_edges_orig = g.m_forward.m_column.size();
  for (; g.m_last_source <= src; ++g.m_last_source)
    g.m_forward.m_rowstart[g.m_last_source] = num_edges_orig;
  g.m_forward.m_rowstart[src + 1] = num_edges_orig + 1;
  g.m_forward.m_column.push_back(tgt);
  g.edge_properties().push_back(p);
  return typename BOOST_CSR_GRAPH_TYPE::edge_descriptor(src, num_edges_orig);
}
#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE

#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  // Add edges from a sorted (smallest sources first) range of pairs and edge
  // properties
  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename BidirectionalIteratorOrig,
            typename EPIterOrig>
  void
  add_edges_sorted(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      EPIterOrig ep_iter_sorted,
      BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_sorted_internal(first_sorted, last_sorted, ep_iter_sorted);
  }

  // Add edges from a sorted (smallest sources first) range of pairs
  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename BidirectionalIteratorOrig>
  void
  add_edges_sorted(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_sorted_internal(first_sorted, last_sorted);
  }

  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename BidirectionalIteratorOrig,
            typename EPIterOrig, typename GlobalToLocal>
  void
  add_edges_sorted_global(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      EPIterOrig ep_iter_sorted,
      const GlobalToLocal& global_to_local,
      BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_sorted_internal_global(first_sorted, last_sorted, ep_iter_sorted, 
                                       global_to_local);
  }

  // Add edges from a sorted (smallest sources first) range of pairs
  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename BidirectionalIteratorOrig,
            typename GlobalToLocal>
  void
  add_edges_sorted_global(
      BidirectionalIteratorOrig first_sorted,
      BidirectionalIteratorOrig last_sorted,
      const GlobalToLocal& global_to_local,
      BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_sorted_internal_global(first_sorted, last_sorted, global_to_local);
  }

  // Add edges from a range of (source, target) pairs that are unsorted
  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename InputIterator,
            typename GlobalToLocal>
  inline void
  add_edges_global(InputIterator first, InputIterator last, 
                   const GlobalToLocal& global_to_local, BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_internal(first, last, global_to_local);
  }

  // Add edges from a range of (source, target) pairs that are unsorted
  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename InputIterator>
  inline void
  add_edges(InputIterator first, InputIterator last, BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_internal(first, last);
  }

  // Add edges from a range of (source, target) pairs and edge properties that
  // are unsorted
  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS,
            typename InputIterator, typename EPIterator>
  inline void
  add_edges(InputIterator first, InputIterator last,
            EPIterator ep_iter, EPIterator ep_iter_end,
            BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_internal(first, last, ep_iter, ep_iter_end);
  }

  template <BOOST_CSR_GRAPH_TEMPLATE_PARMS,
            typename InputIterator, typename EPIterator, typename GlobalToLocal>
  inline void
  add_edges_global(InputIterator first, InputIterator last,
            EPIterator ep_iter, EPIterator ep_iter_end,
            const GlobalToLocal& global_to_local,
            BOOST_CSR_GRAPH_TYPE& g) {
    g.add_edges_internal(first, last, ep_iter, ep_iter_end, global_to_local);
  }
#endif // BOOST_GRAPH_USE_NEW_CSR_INTERFACE

// From VertexListGraph
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
num_vertices(const BOOST_CSR_GRAPH_TYPE& g) {
  return g.m_forward.m_rowstart.size() - 1;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
std::pair<counting_iterator<Vertex>, counting_iterator<Vertex> >
inline vertices(const BOOST_CSR_GRAPH_TYPE& g) {
  return std::make_pair(counting_iterator<Vertex>(0),
                        counting_iterator<Vertex>(num_vertices(g)));
}

// From IncidenceGraph
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
class BOOST_CSR_GRAPH_TYPE::out_edge_iterator
  : public iterator_facade<typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator,
                           typename BOOST_CSR_GRAPH_TYPE::edge_descriptor,
                           std::random_access_iterator_tag,
                           const typename BOOST_CSR_GRAPH_TYPE::edge_descriptor&,
                           typename int_t<CHAR_BIT * sizeof(EdgeIndex)>::fast>
{
 public:
  typedef typename int_t<CHAR_BIT * sizeof(EdgeIndex)>::fast difference_type;

  out_edge_iterator() {}
  // Implicit copy constructor OK
  explicit out_edge_iterator(edge_descriptor edge) : m_edge(edge) { }

 private:
  // iterator_facade requirements
  const edge_descriptor& dereference() const { return m_edge; }

  bool equal(const out_edge_iterator& other) const
  { return m_edge == other.m_edge; }

  void increment() { ++m_edge.idx; }
  void decrement() { --m_edge.idx; }
  void advance(difference_type n) { m_edge.idx += n; }

  difference_type distance_to(const out_edge_iterator& other) const
  { return other.m_edge.idx - m_edge.idx; }

  edge_descriptor m_edge;

  friend class iterator_core_access;
};

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
source(typename BOOST_CSR_GRAPH_TYPE::edge_descriptor e,
       const BOOST_CSR_GRAPH_TYPE&)
{
  return e.src;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
target(typename BOOST_CSR_GRAPH_TYPE::edge_descriptor e,
       const BOOST_CSR_GRAPH_TYPE& g)
{
  return g.m_forward.m_column[e.idx];
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline std::pair<typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator,
                 typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator>
out_edges(Vertex v, const BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename BOOST_CSR_GRAPH_TYPE::edge_descriptor ed;
  typedef typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator it;
  EdgeIndex v_row_start = g.m_forward.m_rowstart[v];
  EdgeIndex next_row_start = g.m_forward.m_rowstart[v + 1];
#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  return std::make_pair(it(ed(v, v_row_start)),
                        it(ed(v, next_row_start)));
#else
  // Special case to allow incremental construction
  return std::make_pair(it(ed(v, v_row_start)),
                        it(ed(v, (std::max)(v_row_start, next_row_start))));
#endif
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline EdgeIndex
out_degree(Vertex v, const BOOST_CSR_GRAPH_TYPE& g)
{
  EdgeIndex v_row_start = g.m_forward.m_rowstart[v];
  EdgeIndex next_row_start = g.m_forward.m_rowstart[v + 1];
#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  return next_row_start - v_row_start;
#else
  // Special case to allow incremental construction
  return (std::max)(v_row_start, next_row_start) - v_row_start;
#endif
}

// From AdjacencyGraph
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline std::pair<typename BOOST_CSR_GRAPH_TYPE::adjacency_iterator,
                 typename BOOST_CSR_GRAPH_TYPE::adjacency_iterator>
adjacent_vertices(Vertex v, const BOOST_CSR_GRAPH_TYPE& g)
{
  EdgeIndex v_row_start = g.m_forward.m_rowstart[v];
  EdgeIndex next_row_start = g.m_forward.m_rowstart[v + 1];
#ifdef BOOST_GRAPH_USE_NEW_CSR_INTERFACE
  return std::make_pair(g.m_forward.m_column.begin() + v_row_start,
                        g.m_forward.m_column.begin() + next_row_start);
#else
  // Special case to allow incremental construction
  return std::make_pair(g.m_forward.m_column.begin() + v_row_start,
                        g.m_forward.m_column.begin() +
                                (std::max)(v_row_start, next_row_start));
#endif
}

// Extra, common functions
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline typename graph_traits<BOOST_CSR_GRAPH_TYPE>::vertex_descriptor
vertex(typename graph_traits<BOOST_CSR_GRAPH_TYPE>::vertex_descriptor i, 
       const BOOST_CSR_GRAPH_TYPE&)
{
  return i;
}

#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
// These require that the out edges from a vertex are sorted, which is only
// guaranteed by the old interface

// Unlike for an adjacency_matrix, edge_range and edge take lg(out_degree(i))
// time
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline std::pair<typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator,
                 typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator>
edge_range(Vertex i, Vertex j, const BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename std::vector<Vertex>::const_iterator adj_iter;
  typedef typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator out_edge_iter;
  typedef typename BOOST_CSR_GRAPH_TYPE::edge_descriptor edge_desc;
  std::pair<adj_iter, adj_iter> raw_adjacencies = adjacent_vertices(i, g);
  std::pair<adj_iter, adj_iter> adjacencies =
    std::equal_range(raw_adjacencies.first, raw_adjacencies.second, j);
  EdgeIndex idx_begin = adjacencies.first - g.m_forward.m_column.begin();
  EdgeIndex idx_end = adjacencies.second - g.m_forward.m_column.begin();
  return std::make_pair(out_edge_iter(edge_desc(i, idx_begin)),
                        out_edge_iter(edge_desc(i, idx_end)));
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline std::pair<typename BOOST_CSR_GRAPH_TYPE::edge_descriptor, bool>
edge(Vertex i, Vertex j, const BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator out_edge_iter;
  std::pair<out_edge_iter, out_edge_iter> range = edge_range(i, j, g);
  if (range.first == range.second)
    return std::make_pair(typename BOOST_CSR_GRAPH_TYPE::edge_descriptor(),
                          false);
  else
    return std::make_pair(*range.first, true);
}

#else // !BOOST_GRAPH_USE_OLD_CSR_INTERFACE
// edge() can be provided in linear time for the new interface

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline std::pair<typename BOOST_CSR_GRAPH_TYPE::edge_descriptor, bool>
edge(Vertex i, Vertex j, const BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename BOOST_CSR_GRAPH_TYPE::out_edge_iterator out_edge_iter;
  std::pair<out_edge_iter, out_edge_iter> range = out_edges(i, g);
  for (; range.first != range.second; ++range.first) {
    if (target(*range.first) == j)
      return std::make_pair(*range.first, true);
  }
  return std::make_pair(typename BOOST_CSR_GRAPH_TYPE::edge_descriptor(),
                        false);
}

#endif // !BOOST_GRAPH_USE_OLD_CSR_INTERFACE

// Find an edge given its index in the graph
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline typename BOOST_CSR_GRAPH_TYPE::edge_descriptor
edge_from_index(EdgeIndex idx, const BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename std::vector<EdgeIndex>::const_iterator row_start_iter;
  assert (idx < num_edges(g));
  row_start_iter src_plus_1 =
    std::upper_bound(g.m_forward.m_rowstart.begin(),
#ifdef BOOST_GRAPH_USE_OLD_CSR_INTERFACE
                     // This handles the case where there are some vertices
                     // with rowstart 0 after the last provided vertex; this
                     // case does not happen with the new interface
                     g.m_forward.m_rowstart.begin() + g.m_last_source + 1,
#else // !BOOST_GRAPH_USE_OLD_CSR_INTERFACE
                     g.m_forward.m_rowstart.end(),
#endif // BOOST_GRAPH_USE_OLD_CSR_INTERFACE
                     idx);
    // Get last source whose rowstart is at most idx
    // upper_bound returns this position plus 1
  Vertex src = (src_plus_1 - g.m_forward.m_rowstart.begin()) - 1;
  return typename BOOST_CSR_GRAPH_TYPE::edge_descriptor(src, idx);
}

// From EdgeListGraph
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
class BOOST_CSR_GRAPH_TYPE::edge_iterator
{
 public:
  typedef std::forward_iterator_tag iterator_category;
  typedef edge_descriptor value_type;

  typedef const edge_descriptor* pointer;

  typedef edge_descriptor reference;
  typedef typename int_t<CHAR_BIT * sizeof(EdgeIndex)>::fast difference_type;

  edge_iterator() : rowstart_array(0), current_edge(), end_of_this_vertex(0) {}

  edge_iterator(const compressed_sparse_row_graph& graph,
                edge_descriptor current_edge,
                EdgeIndex end_of_this_vertex)
    : rowstart_array(&graph.m_forward.m_rowstart[0]), current_edge(current_edge),
      end_of_this_vertex(end_of_this_vertex) {}

  // From InputIterator
  reference operator*() const { return current_edge; }
  pointer operator->() const { return &current_edge; }

  bool operator==(const edge_iterator& o) const {
    return current_edge == o.current_edge;
  }
  bool operator!=(const edge_iterator& o) const {
    return current_edge != o.current_edge;
  }

  edge_iterator& operator++() {
    ++current_edge.idx;
    while (current_edge.idx == end_of_this_vertex) {
      ++current_edge.src;
      end_of_this_vertex = rowstart_array[current_edge.src + 1];
    }
    return *this;
  }

  edge_iterator operator++(int) {
    edge_iterator temp = *this;
    ++*this;
    return temp;
  }

 private:
  const EdgeIndex* rowstart_array;
  edge_descriptor current_edge;
  EdgeIndex end_of_this_vertex;
};

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline EdgeIndex
num_edges(const BOOST_CSR_GRAPH_TYPE& g)
{
  return g.m_forward.m_column.size();
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
std::pair<typename BOOST_CSR_GRAPH_TYPE::edge_iterator,
          typename BOOST_CSR_GRAPH_TYPE::edge_iterator>
edges(const BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename BOOST_CSR_GRAPH_TYPE::edge_iterator ei;
  typedef typename BOOST_CSR_GRAPH_TYPE::edge_descriptor edgedesc;
  if (g.m_forward.m_rowstart.size() == 1 || g.m_forward.m_column.empty()) {
    return std::make_pair(ei(), ei());
  } else {
    // Find the first vertex that has outgoing edges
    Vertex src = 0;
    while (g.m_forward.m_rowstart[src + 1] == 0) ++src;
    return std::make_pair(ei(g, edgedesc(src, 0), g.m_forward.m_rowstart[src + 1]),
                          ei(g, edgedesc(num_vertices(g), g.m_forward.m_column.size()), 0));
  }
}

// For Property Graph

// Graph properties
template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, class Tag, class Value>
inline void
set_property(BOOST_CSR_GRAPH_TYPE& g, Tag, const Value& value)
{
  get_property_value(g.m_property, Tag()) = value;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, class Tag>
inline
typename graph_property<BOOST_CSR_GRAPH_TYPE, Tag>::type&
get_property(BOOST_CSR_GRAPH_TYPE& g, Tag)
{
  return get_property_value(g.m_property, Tag());
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, class Tag>
inline
const
typename graph_property<BOOST_CSR_GRAPH_TYPE, Tag>::type&
get_property(const BOOST_CSR_GRAPH_TYPE& g, Tag)
{
  return get_property_value(g.m_property, Tag());
}

// Add edge_index property map
template<typename Index, typename Descriptor>
struct csr_edge_index_map
{
  typedef Index                     value_type;
  typedef Index                     reference;
  typedef Descriptor                key_type;
  typedef readable_property_map_tag category;
};

template<typename Index, typename Descriptor>
inline Index
get(const csr_edge_index_map<Index, Descriptor>&,
    const typename csr_edge_index_map<Index, Descriptor>::key_type& key)
{
  return key.idx;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
struct property_map<BOOST_CSR_GRAPH_TYPE, vertex_index_t>
{
  typedef identity_property_map type;
  typedef type const_type;
};

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
struct property_map<BOOST_CSR_GRAPH_TYPE, edge_index_t>
{
private:
  typedef typename graph_traits<BOOST_CSR_GRAPH_TYPE>::edge_descriptor
    edge_descriptor;
  typedef csr_edge_index_map<EdgeIndex, edge_descriptor> edge_index_type;

public:
  typedef edge_index_type type;
  typedef type const_type;
};

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline identity_property_map
get(vertex_index_t, const BOOST_CSR_GRAPH_TYPE&)
{
  return identity_property_map();
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline Vertex
get(vertex_index_t,
    const BOOST_CSR_GRAPH_TYPE&, Vertex v)
{
  return v;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline typename property_map<BOOST_CSR_GRAPH_TYPE, edge_index_t>::const_type
get(edge_index_t, const BOOST_CSR_GRAPH_TYPE&)
{
  typedef typename property_map<BOOST_CSR_GRAPH_TYPE, edge_index_t>::const_type
    result_type;
  return result_type();
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS>
inline EdgeIndex
get(edge_index_t, const BOOST_CSR_GRAPH_TYPE&,
    typename BOOST_CSR_GRAPH_TYPE::edge_descriptor e)
{
  return e.idx;
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename T, typename Bundle>
inline
typename property_map<BOOST_CSR_GRAPH_TYPE, T Bundle::*>::type
get(T Bundle::* p, BOOST_CSR_GRAPH_TYPE& g)
{
  typedef typename property_map<BOOST_CSR_GRAPH_TYPE,
                                T Bundle::*>::type
    result_type;
  return result_type(&g, p);
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename T, typename Bundle>
inline
typename property_map<BOOST_CSR_GRAPH_TYPE, T Bundle::*>::const_type
get(T Bundle::* p, BOOST_CSR_GRAPH_TYPE const & g)
{
  typedef typename property_map<BOOST_CSR_GRAPH_TYPE,
                                T Bundle::*>::const_type
    result_type;
  return result_type(&g, p);
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename T, typename Bundle,
         typename Key>
inline T
get(T Bundle::* p, BOOST_CSR_GRAPH_TYPE const & g,
    const Key& key)
{
  return get(get(p, g), key);
}

template<BOOST_CSR_GRAPH_TEMPLATE_PARMS, typename T, typename Bundle,
         typename Key>
inline void
put(T Bundle::* p, BOOST_CSR_GRAPH_TYPE& g,
    const Key& key, const T& value)
{
  put(get(p, g), key, value);
}

#undef BOOST_CSR_GRAPH_TYPE
#undef BOOST_CSR_GRAPH_TEMPLATE_PARMS

} // end namespace boost

#endif // BOOST_GRAPH_COMPRESSED_SPARSE_ROW_GRAPH_HPP
