// voronoi_dual_graph.hpp from Boost.Polygon library
//          Copyright Michael Simbirsky 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_VORONOI_DUAL_GRAPH_H_
#define BOOST_VORONOI_DUAL_GRAPH_H_

#include <boost/polygon/voronoi_diagram.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

#include "BoostVoronoiPrimalGraph.hpp"

namespace boost 
{
namespace polygon 
{
/**
* Boost.Polygon provides a data structure for Voronoi Diagram. The latter is represented as DCEL:
* a collection of cells, vertices and edges. As is well known, DCEL structure can be viewed as a
* combination of two graphs:
*
* Primal graph consists of DCEL vertices and [half]-edges. It is a bidirected adjacenecy-list graph
* where each half-edge has a direction and a dual half-edge.
*
* Dual graph consists of DCEL cells. The edges of the Dual Graph are pairs of half-edges. This graph
* can be viewed as adjacency-vertex graph.
*
* Similar ideas are discussed in CGAL, see for example
* http://doc.cgal.org/latest/BGL/index.html#BGLTriangulations
 */


template <class T> //coord type
class voronoi_dual_graph
{
public:
    typedef T coordinate_type;

    typedef voronoi_diagram<T> voronoi_diagram_t;
    typedef voronoi_dual_graph<T> this_graph_type;
    typedef typename voronoi_diagram_t::edge_type edge_type;
    typedef typename voronoi_diagram_t::cell_type cell_type;

    
    ///Voronoi Dual Graph should also refine the very fundamental concepts of Boost::Graph
    ///See details /http://www.boost.org/doc/libs/release/libs/graph/doc/Graph.html
    typedef const cell_type*                  vertex_descriptor;
    typedef const edge_type*                    edge_descriptor; 
    typedef boost::bidirectional_tag            directed_category;
    typedef boost::disallow_parallel_edge_tag   edge_parallel_category;
    
    struct traversal_category
        : public virtual boost::vertex_list_graph_tag
        , public virtual boost::edge_list_graph_tag 
        , public virtual boost::adjacency_graph_tag
        , public virtual boost::bidirectional_graph_tag //means we provide access to in_edges
        												//and out_edges of a given vertex
    {
    };

    //Returns a special boost::graph_traits<G>::vertex_descriptor object which does not refer
    //to any vertex of graph object which type is G.
    static vertex_descriptor null_vertex() { return NULL; }

    ///VertexListGraph refines Graph http://www.boost.org/doc/libs/release/libs/graph/doc/VertexListGraph.html
    typedef boost::counting_iterator<vertex_descriptor>         vertex_iterator;
    typedef size_t                                              vertices_size_type;

    //EdgeListGraph concept
    typedef boost::counting_iterator<edge_descriptor>           edge_iterator;


    ///Several typedefs expected by boost::graph_traits<G> class
    typedef size_t                              degree_size_type;

    //A circulator of half-edges at given "source" cell -- enumerates edges at the given cell
    class out_edge_iterator
        : public boost::iterator_facade<out_edge_iterator,
                                    edge_descriptor, //value
                                    boost::forward_traversal_tag,
                                    edge_descriptor> //reference
    {
    public:    
        out_edge_iterator(); //for copy ctor etc.; "end"

        out_edge_iterator(vertex_descriptor src); //"begin"
      
        //we only compare iterators of the same node
        bool equal(const out_edge_iterator & other) const;
        
        edge_descriptor dereference() const { return m_edge; }
        void increment(); 
    protected:
        edge_descriptor m_edge;
        edge_descriptor m_anchor;
    };


    std::pair<out_edge_iterator,out_edge_iterator> out_edges(vertex_descriptor v) const;
        
    // AdjacencyGraph requirements: //iterates over vertices adjacant to a given one
    typedef typename boost::adjacency_iterator_generator<voronoi_dual_graph<T>,
    		vertex_descriptor, out_edge_iterator>::type adjacency_iterator;

    /////////////////////////////// incoming [half]edges ////////////////////
    typedef boost::transform_iterator<details::twin_transform<edge_descriptor>, out_edge_iterator> in_edge_iterator;

    /////////////////////////// non-graph functions //////////////////
    const edge_type   & getEdge( edge_descriptor e) const { return *e; }
    const cell_type & getCell( vertex_descriptor n) const { return *n; }

    explicit voronoi_dual_graph<T>(const voronoi_diagram_t* vd)
        : m_voronoiDiagram(vd)
    {
    }

    const voronoi_diagram_t & getDiagram() const { return *m_voronoiDiagram; }

protected:
    const voronoi_diagram_t* m_voronoiDiagram;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// IncidenceGraph refines Graph http://www.boost.org/doc/libs/release/libs/graph/doc/IncidenceGraph.html
/// with type of out_edge_iterator and global function source(e,g), target(e,g), out_edges(v,g) and out_degre(v,g)
template <class T>
typename voronoi_dual_graph<T>::vertex_descriptor 
    source(typename voronoi_dual_graph<T>::edge_descriptor e, const voronoi_dual_graph<T> & g) 
{ 
    const typename voronoi_dual_graph<T>::vertex_type* v = e->cell();
    return v;
}

template <class T>
typename voronoi_dual_graph<T>::vertex_descriptor 
    target(typename voronoi_dual_graph<T>::edge_descriptor e, const voronoi_dual_graph<T> & g) 
{ 
    const typename voronoi_dual_graph<T>::cell_type* v = e->twin()->cell();
    return v;
}

///Global functions implementing the concept of Boost.Graph Adjacency List
///This function is not O(1) but 
template <class T>
std::pair<typename voronoi_dual_graph<T>::out_edge_iterator,typename voronoi_dual_graph<T>::out_edge_iterator>
    out_edges(typename voronoi_dual_graph<T>::vertex_descriptor vd, const voronoi_dual_graph<T> & g )
{
    return g.out_edges(vd);
}

template <class T>
std::pair<typename voronoi_dual_graph<T>::out_edge_iterator,typename voronoi_dual_graph<T>::out_edge_iterator>
    voronoi_dual_graph<T>::out_edges(vertex_descriptor vd) const
{
    const cell_type & v = this->getCell(vd);

    out_edge_iterator begin(&v);
    out_edge_iterator end;

    return std::make_pair(out_edge_iterator(begin),out_edge_iterator(end));
}

template <class T>
int out_degree(typename voronoi_dual_graph<T>::vertex_descriptor v, const voronoi_dual_graph<T> & g )
{
    std::pair<typename voronoi_dual_graph<T>::out_edge_iterator,
              typename voronoi_dual_graph<T>::out_edge_iterator> status = out_edges(v,g);
    return std::distance(status.first, status.second); 
}

////////////////////////// Graph out_edge_iterator
template <class T> //coord type
voronoi_dual_graph<T>::out_edge_iterator::out_edge_iterator(
            vertex_descriptor src) //"begin"
    : m_edge(src->incident_edge())
    , m_anchor(m_edge)
{
}


template <class T> //coord type
voronoi_dual_graph<T>::out_edge_iterator::out_edge_iterator()
    :m_edge(NULL)
{
}

template <class T> //coord type
bool voronoi_dual_graph<T>::out_edge_iterator::equal(const out_edge_iterator & other) const
{
    return m_edge == other.m_edge;
}


template <class T> //coord type
void voronoi_dual_graph<T>::out_edge_iterator::increment()
{
    m_edge = m_edge->next(); //compare with primal graph where we use rot_next
    if (m_edge == m_anchor)
        m_edge= NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// BidirectionalGraph refines IncidenceGraph
/// http://www.boost.org/doc/libs/release/libs/graph/doc/BidirectionalGraph.html
/// with type of in_edge_iterator and global function in_edges(v,g) and in_degre(v,g)
template <class T>
std::pair<typename voronoi_dual_graph<T>::in_edge_iterator,typename voronoi_dual_graph<T>::in_edge_iterator>
    in_edges(typename voronoi_dual_graph<T>::vertex_descriptor vd, const voronoi_dual_graph<T> & g )
{
    typedef typename voronoi_dual_graph<T>::out_edge_iterator out_edge_iterator;
    typedef typename voronoi_dual_graph<T>::in_edge_iterator in_edge_iterator;
    
    out_edge_iterator begin, end;
    boost::tie(begin,end) = out_edges(vd, g);
    return std::make_pair( in_edge_iterator(begin), in_edge_iterator(end) );
}

template <class T>
int in_degree(typename voronoi_dual_graph<T>::vertex_descriptor v, const voronoi_dual_graph<T> & g )
{
    return out_degree(v,g);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//VertexListGraph refines Graph http://www.boost.org/doc/libs/release/libs/graph/doc/VertexListGraph.html
//with global function num_vertices(g) and vertices(g)
template <class T>
size_t num_vertices( const voronoi_dual_graph<T> & g)
{
    return g.getDiagram().cells().size();
}

template <class T>
std::pair<typename voronoi_dual_graph<T>::vertex_iterator,
          typename voronoi_dual_graph<T>::vertex_iterator>
vertices(const voronoi_dual_graph<T> & g)
{ 
    const typename voronoi_dual_graph<T>::cell_type* v =
    		g.getDiagram().cells().empty()? NULL :
    		& (g.getDiagram().cells().front());

    return std::pair<typename voronoi_dual_graph<T>::vertex_iterator,
    		         typename voronoi_dual_graph<T>::vertex_iterator>(v, v + num_vertices(g) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//EdgeListGraph refines Graph http://www.boost.org/doc/libs/release/libs/graph/doc/EdgeListGraph.html
//with global function num_edges(g) and edges(g)
template <class T>
size_t num_edges( const voronoi_dual_graph<T> & g)
{
    return g.getDiagram().edges().size();
}

template <class T>
std::pair<typename voronoi_dual_graph<T>::edge_iterator,
          typename voronoi_dual_graph<T>::edge_iterator>
edges(const voronoi_dual_graph<T> & g)
{ 
    const typename voronoi_dual_graph<T>::edge_type* v =
    		g.getDiagram().edges().empty()? NULL :
    		& (g.getDiagram().edges().front());

    return std::pair<typename voronoi_dual_graph<T>::edge_iterator,
    		         typename voronoi_dual_graph<T>::edge_iterator>(v, v + num_edges(g) );
}

} //namespace polygon 

/****************************************************************************
 * Property maps for Voronoi diagram
 * PropertyGraph refines Graph: http://www.boost.org/doc/libs/release/libs/graph/doc/PropertyGraph.html
 *
 * In Voronoi diagram graph, the vertex_descriptor also gives information about the index of vertex
 * in some hidden array. It means this type can work as property_map for vertex_index_t.
 * The class boost::property_map< typename voronoi_proper_primal_graph<T>, vertex_index_t >
 * is extremely important for all DFS- and BFS-based algorithms as all these algorithms
 * need to know how to convert vertex_descriptor into a natural number and vise versa.
 */
template<class T>
struct property_map< polygon::voronoi_dual_graph<T>, vertex_index_t > {
    typedef polygon::voronoi_dual_graph<T> graph_t;

    //iterator_property_map gives offset of given iterator of the iterator it keeps as anchor.
    typedef details::iterator_offset_map<typename graph_t::vertex_descriptor> const_type;
    //typedef const_type type; -- we do not define type as "vertex_index_t" map is read-only for voronoi graphs
};

template<class T>
typename property_map< polygon::voronoi_dual_graph<T>, vertex_index_t >::const_type
get(vertex_index_t, const polygon::voronoi_dual_graph<T>& g) 
{
    typedef polygon::voronoi_dual_graph<T> graph_t;
    typedef typename graph_t::voronoi_diagram_t::cell_container_type container_type;

    const container_type & vertices = g.getDiagram().cells();
    if (vertices.empty())
        return details::make_iterator_offset_map( g.null_vertex() );
    else
        return details::make_iterator_offset_map( & vertices.front() );
}

//The property map for edge_index_t looks the same as the one for vertex_index_t, 
//However, the edge indexing is not used much in algorithms
template<class T>
struct property_map< polygon::voronoi_dual_graph<T>, edge_index_t > {
    typedef polygon::voronoi_dual_graph<T> graph_t;
    //iterator_property_map gives offset of given iterator of the iterator it keeps as anchor.
    typedef details::iterator_offset_map<typename graph_t::edge_descriptor> const_type;
    //typedef const_type type; -- we do not define type as "edge_index_t" map is read-only for voronoi graphs
};

template<class T>
typename property_map< polygon::voronoi_dual_graph<T>, edge_index_t >::type
get(edge_index_t, const polygon::voronoi_dual_graph<T>& g) 
{
    typedef polygon::voronoi_dual_graph<T> graph_t;
    typedef typename graph_t::voronoi_diagram_t::edge_container_type container_type;

    const container_type & edges = g.getDiagram().edges();
    if (edges.empty())
        return details::make_iterator_offset_map((typename graph_t::edge_descriptor)NULL);
    else
        return details::make_iterator_offset_map( & edges.front() );
}


//access to color map for vertices of voronoi_dual_graph
template<class T>
struct property_map< polygon::voronoi_dual_graph<T>, vertex_color_t > 
{
    typedef voronoi_color_map< polygon::voronoi_vertex<T> > type;
    typedef type const_type;
};

template<class T>
typename property_map<polygon::voronoi_dual_graph<T>, vertex_color_t>::type
    get(vertex_color_t, const polygon::voronoi_dual_graph<T> & /*g*/)
{
    return voronoi_color_map< polygon::voronoi_vertex<T> >();
}

//access to color map for edges of voronoi_dual_graph
template<class T>
struct property_map< polygon::voronoi_dual_graph<T>, edge_color_t > 
{
    typedef voronoi_color_map< polygon::voronoi_edge<T> > type;
    typedef type const_type;
};

template<class T>
typename property_map<polygon::voronoi_dual_graph<T>, edge_color_t>::type
    get(edge_color_t, const polygon::voronoi_dual_graph<T> & /*g*/)
{
    return voronoi_color_map< polygon::voronoi_edge<T> >();
}

//TODO: for dual graph we could provide custom tags for properties such as source_index and source_category.


} //namespace boost

#endif //BOOST_VORONOI_DUAL_GRAPH_H_
