// voronoi_primal_graph.hpp from Boost.Polygon library
//          Copyright Michael Simbirsky 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history. */

#ifndef BOOST_VORONOI_PRIMAL_GRAPH_H_
#define BOOST_VORONOI_PRIMAL_GRAPH_H_

#include <boost/polygon/voronoi_diagram.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

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
namespace details {

///converts a DCEL half-edge to its twin
template <class edge_ptr> 
class twin_transform
{
public:
    typedef edge_ptr argument_type;
	typedef edge_ptr result_type;

    edge_ptr operator()(edge_ptr e) const { return e->twin(); }
};
} //namespace details


template <class T> //coord type
class voronoi_proper_primal_graph
{
public:
    typedef T coordinate_type;
    typedef voronoi_diagram<T> voronoi_diagram_t;

    typedef typename voronoi_diagram_t::edge_type edge_type;
    typedef typename voronoi_diagram_t::vertex_type vertex_type;

    
    ///Voronoi Primal Graph should also refine the very fundamental concept of Boost::Graph
    ///http://www.boost.org/doc/libs/release/libs/graph/doc/Graph.html with following typedefs
    ///and static function null_vertex()
    typedef const vertex_type*                  vertex_descriptor;
    typedef const edge_type*                    edge_descriptor; 
    typedef boost::bidirectional_tag            directed_category; //typical for DCEL
    typedef boost::disallow_parallel_edge_tag   edge_parallel_category;
    
    struct traversal_category
        : public virtual boost::vertex_list_graph_tag
        //, public virtual boost::edge_list_graph_tag -- we cannot give number of edges as
          	  	  	  	  	  	  	  	  	//some of them are improper (start or end at NULL)
        , public virtual boost::adjacency_graph_tag
        , public virtual boost::bidirectional_graph_tag //means we provide access to in_edges
        												//and out_edges of a given vertex
    {
    };

    //Returns a special boost::graph_traits<G>::vertex_descriptor object which does not refer to any vertex of graph object which type is G. 	
    static vertex_descriptor null_vertex() { return NULL; }

    ///VertexListGraph refines Graph http://www.boost.org/doc/libs/release/libs/graph/doc/VertexListGraph.html
    ///Voronoi diagram also provides access to its vertices, see global functions vertices(g) and num_vertices(g) below
    typedef boost::counting_iterator<vertex_descriptor>         vertex_iterator;
    typedef size_t                                              vertices_size_type;

    ///Several typedefs expected by boost::graph_traits<G> class
    typedef size_t                              degree_size_type;

    //A CCW circulator of edges incident at given "source" vertex
    class basic_out_edge_iterator
        : public boost::iterator_facade<basic_out_edge_iterator,
                                    edge_descriptor, //value
                                    boost::forward_traversal_tag,
                                    edge_descriptor> //reference
    {
    public:    
        basic_out_edge_iterator(); //for copy ctor etc.; "end"

        basic_out_edge_iterator(vertex_descriptor src); //"begin"
      
        //we only compare iterators of the same node
        bool equal(const basic_out_edge_iterator & other) const;
        
        edge_descriptor dereference() const { return m_edge; }
        void increment(); 
    protected:
        edge_descriptor m_edge;
        edge_descriptor m_anchor;
    };

    class has_proper_target: public std::unary_function<edge_descriptor,bool>
    {
    public:
        bool operator()(edge_descriptor e) const { return e->vertex1() != NULL; }
    };

    typedef boost::filter_iterator<has_proper_target, basic_out_edge_iterator> out_edge_iterator;

    ///helper for global function out_edges(v,g)
    std::pair<out_edge_iterator,out_edge_iterator> out_edges(vertex_descriptor v) const;
        
    // AdjacencyGraph requirements: //iterates over vertices adjacent to a given one
    typedef typename boost::adjacency_iterator_generator<voronoi_proper_primal_graph<T>,
    		vertex_descriptor, out_edge_iterator>::type adjacency_iterator;
            
    /// iterator over incoming [half]edges -- almost the same as out_edge_iterator
    typedef boost::transform_iterator<details::twin_transform<edge_descriptor>, out_edge_iterator>
    		in_edge_iterator;

    /////////////////////////// non-graph functions //////////////////
    const edge_type   & getEdge( edge_descriptor e) const { return *e; }
    const vertex_type & getVertex( vertex_descriptor n) const { return *n; }

    explicit voronoi_proper_primal_graph<T>(const voronoi_diagram_t* vd)
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
typename voronoi_proper_primal_graph<T>::vertex_descriptor 
    source(typename voronoi_proper_primal_graph<T>::edge_descriptor e, const voronoi_proper_primal_graph<T> & g) 
{ 
    const typename voronoi_proper_primal_graph<T>::vertex_type* v = g.getEdge(e).vertex0();
    //NULL corresponds to infinity but we ignore it (our clients will always filter out infinity vertices and infinite edges)
    return v;
; 
}

template <class T>
typename voronoi_proper_primal_graph<T>::vertex_descriptor 
    target(typename voronoi_proper_primal_graph<T>::edge_descriptor e, const voronoi_proper_primal_graph<T> & g) 
{ 
    const typename voronoi_proper_primal_graph<T>::vertex_type* v = g.getEdge(e).vertex1();
    //NULL corresponds to infinity but we ignore it (our clients will always filter out infinity vertices and infinite edges)
    return v;
}

///This function is not always O(1)
template <class T>
std::pair<typename voronoi_proper_primal_graph<T>::out_edge_iterator,
		  typename voronoi_proper_primal_graph<T>::out_edge_iterator>
    out_edges(typename voronoi_proper_primal_graph<T>::vertex_descriptor vd, const voronoi_proper_primal_graph<T> & g )
{
    return g.out_edges(vd);
}

template <class T>
std::pair<typename voronoi_proper_primal_graph<T>::out_edge_iterator,
		  typename voronoi_proper_primal_graph<T>::out_edge_iterator>
    voronoi_proper_primal_graph<T>::out_edges(vertex_descriptor vd) const
{
    const vertex_type & v = this->getVertex(vd);

    basic_out_edge_iterator begin(&v);
    basic_out_edge_iterator end;

    return std::make_pair(out_edge_iterator(begin),out_edge_iterator(end));
}

template <class T>
int out_degree(typename voronoi_proper_primal_graph<T>::vertex_descriptor v, const voronoi_proper_primal_graph<T> & g )
{
    std::pair<typename voronoi_proper_primal_graph<T>::out_edge_iterator,
              typename voronoi_proper_primal_graph<T>::out_edge_iterator> status = out_edges(v,g);
    return std::distance(status.first, status.second); 
}

////////////////////////// Graph out_edge_iterator
template <class T> //coord type
voronoi_proper_primal_graph<T>::basic_out_edge_iterator::basic_out_edge_iterator(
            vertex_descriptor src) //"begin"
    : m_edge(src->incident_edge())
    , m_anchor(m_edge)
{
}


template <class T> //coord type
voronoi_proper_primal_graph<T>::basic_out_edge_iterator::basic_out_edge_iterator()
    :m_edge(NULL)
{
}

template <class T> //coord type
bool voronoi_proper_primal_graph<T>::basic_out_edge_iterator::equal(const basic_out_edge_iterator & other) const
{
    return m_edge == other.m_edge;
}


template <class T> //coord type
void voronoi_proper_primal_graph<T>::basic_out_edge_iterator::increment()
{
    m_edge = m_edge->rot_next();
    if (m_edge == m_anchor)
        m_edge= NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// BidirectionalGraph refines IncidenceGraph http://www.boost.org/doc/libs/release/libs/graph/doc/BidirectionalGraph.html
/// with type of in_edge_iterator and global function in_edges(v,g) and in_degre(v,g)

////////////////////////// in_edges
template <class T>
std::pair<typename voronoi_proper_primal_graph<T>::in_edge_iterator,
          typename voronoi_proper_primal_graph<T>::in_edge_iterator>
    in_edges(typename voronoi_proper_primal_graph<T>::vertex_descriptor vd, const voronoi_proper_primal_graph<T> & g )
{
    typedef typename voronoi_proper_primal_graph<T>::out_edge_iterator out_edge_iterator;
    typedef typename voronoi_proper_primal_graph<T>::in_edge_iterator in_edge_iterator;
    
    out_edge_iterator begin, end;
    boost::tie(begin,end) = out_edges(vd, g);
    return std::make_pair( in_edge_iterator(begin), in_edge_iterator(end) );
}

template <class T>
int in_degree(typename voronoi_proper_primal_graph<T>::vertex_descriptor v, const voronoi_proper_primal_graph<T> & g )
{
    return out_degree(v,g);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//VertexListGraph refines Graph http://www.boost.org/doc/libs/release/libs/graph/doc/VertexListGraph.html
//with global function num_vertices(g) and vertices(g)
template <class T>
size_t num_vertices( const voronoi_proper_primal_graph<T> & g)
{
    return g.getDiagram().vertices().size();
}

template <class T>
std::pair<typename voronoi_proper_primal_graph<T>::vertex_iterator,
          typename voronoi_proper_primal_graph<T>::vertex_iterator>
vertices(const voronoi_proper_primal_graph<T> & g)
{ 
    const typename voronoi_proper_primal_graph<T>::vertex_type* v =
    		g.getDiagram().vertices().empty()? NULL :
    		& (g.getDiagram().vertices().front());

    return std::pair<typename voronoi_proper_primal_graph<T>::vertex_iterator,
    		         typename voronoi_proper_primal_graph<T>::vertex_iterator>(v, v + num_vertices(g) );
}

/**
* Returns true for primary edges. This filter is convenient when one wants to define a graph of primary edges
* as follows:
*
*/
template <class T>
class voronoi_primary_edge_filter
    : public std::unary_function<typename voronoi_proper_primal_graph<T>::edge_descriptor, bool>
{
public:
    bool operator()(typename voronoi_proper_primal_graph<T>::edge_descriptor e) const
    {
        return e->is_primary();
    }
};

} //namespace polygon 

namespace details
{

template <class ObPtr>    //can be RA iterator; will compile with any forward iterator
class iterator_offset_map //maps iterator to integer by taking offset from "begin"
{
public:
    typedef readable_property_map_tag category; 
    typedef std::ptrdiff_t value_type;
    typedef value_type reference;
    typedef ObPtr key_type; 
    
    explicit iterator_offset_map(key_type anchor):_anchor(anchor){}
    key_type _anchor;
};



template <class ObjPtr>
iterator_offset_map<ObjPtr>
make_iterator_offset_map( ObjPtr anchor)
{
    return iterator_offset_map<ObjPtr>(anchor);
}

template <class ObPtr>
typename iterator_offset_map<ObPtr>::value_type
    get( iterator_offset_map<ObPtr> pmap, 
         typename iterator_offset_map<ObPtr>::key_type vertex)
{
    return std::distance(pmap._anchor, vertex);
}

} //namespace details

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
struct property_map< polygon::voronoi_proper_primal_graph<T>, vertex_index_t > {
    typedef polygon::voronoi_proper_primal_graph<T> graph_t;
    //iterator_property_map gives offset of given iterator of the iterator it keeps as anchor.
    typedef details::iterator_offset_map<typename graph_t::vertex_descriptor> const_type;
    //typedef const_type type; //-- we do not define type as "vertex_index_t" map is read-only for voronoi graphs
};

template<class T>
typename property_map< polygon::voronoi_proper_primal_graph<T>, vertex_index_t >::const_type
get(vertex_index_t, const polygon::voronoi_proper_primal_graph<T>& g) 
{
    typedef polygon::voronoi_proper_primal_graph<T> graph_t;
    typedef typename graph_t::voronoi_diagram_t::vertex_container_type vertex_container_type;
    typedef typename graph_t::vertex_descriptor vertex_descriptor;

    const vertex_container_type & vertices = g.getDiagram().vertices();
    if (vertices.empty())
        return details::make_iterator_offset_map( g.null_vertex());
    else
        return details::make_iterator_offset_map( & vertices.front() );
}


/****************************************************************************
 * Property maps for Voronoi diagram
 * PropertyGraph refines Graph: http://www.boost.org/doc/libs/release/libs/graph/doc/PropertyGraph.html
 * Here we define the PropertyGraph for color (vertex_color, edge_color)
 * Storage for color is available within voronoi_diagram (aka Interior map)
 */
template<class VoronoiObjectType> //can be voronoi_cell or voronoi_vertex or voronoi_edge
class voronoi_color_map
{    
public:
    typedef typename VoronoiObjectType::color_type color_type;
    
    typedef read_write_property_map_tag category; 
    typedef color_type value_type;
    typedef value_type reference;
    typedef const VoronoiObjectType* key_type; 
    
};

//ReadablePropertyMapConcept
template<class VoronoiObjectType>  //!< VoronoiObjectType can be voronoi_cell or voronoi_vertex or voronoi_edge
typename voronoi_color_map<VoronoiObjectType>::color_type
    get( voronoi_color_map<VoronoiObjectType> /*pmap*/, 
         typename voronoi_color_map<VoronoiObjectType>::key_type vertex)
{
    return vertex->color();
}

//WritablePropertyMapConcept
template<class VoronoiObjectType> //!< VoronoiObjectType can be voronoi_cell or voronoi_vertex or voronoi_edge
void put( voronoi_color_map<VoronoiObjectType> /*pmap*/, 
        typename voronoi_color_map<VoronoiObjectType>::key_type vertex,
        typename voronoi_color_map<VoronoiObjectType>::value_type clr)
{
    vertex->color(clr);
}

//access to color map for vertices of voronoi_proper_primal_graph
template<class T>
struct property_map< polygon::voronoi_proper_primal_graph<T>, vertex_color_t > 
{
    typedef voronoi_color_map< polygon::voronoi_vertex<T> > type;
    typedef type const_type;
};

template<class T>
typename property_map<polygon::voronoi_proper_primal_graph<T>, vertex_color_t>::type
    get(vertex_color_t, const polygon::voronoi_proper_primal_graph<T> & /*g*/)
{
    return voronoi_color_map< polygon::voronoi_vertex<T> >();
}

//access to color map for edges of voronoi_proper_primal_graph
template<class T>
struct property_map< polygon::voronoi_proper_primal_graph<T>, edge_color_t > 
{
    typedef voronoi_color_map< polygon::voronoi_edge<T> > type;
    typedef type const_type;
};

template<class T>
typename property_map<polygon::voronoi_proper_primal_graph<T>, edge_color_t>::type
    get(edge_color_t, const polygon::voronoi_proper_primal_graph<T> & /*g*/)
{
    return voronoi_color_map< polygon::voronoi_edge<T> >();
}

//////////////////////////////////////////////////////////////////////////////////
//Installing custom property for coords
template<class KeyType,  //KeyType is usually "const vertex*" or "const cell*"
         class CoordType,
         int k>  //k==0 corresponds to X, k==1 corresponds to Y; see also polygon::point_data accessors
        
        //Point can be polygon::point_data<T> or geometry::model::d2::point_xy<T>
                         //or simply std::pair<T,T> -- anything with constructor (x,y) and copy semantics
class voronoi_coordinate_map
{    
public:
    typedef readable_property_map_tag category; 
    typedef CoordType  value_type;
    typedef const value_type & reference;
    typedef KeyType key_type; 
    
};


enum vertex_Xcoordinate_t { vertex_Xcoordinate };
enum vertex_Ycoordinate_t { vertex_Ycoordinate };
BOOST_INSTALL_PROPERTY(vertex,Xcoordinate);
BOOST_INSTALL_PROPERTY(vertex,Ycoordinate);

template<class T> 
struct property_map< polygon::voronoi_proper_primal_graph<T>, vertex_Xcoordinate_t >
{
    typedef polygon::voronoi_proper_primal_graph<T> graph_t;
    typedef voronoi_coordinate_map<typename graph_t::vertex_descriptor, T, 0> const_type; //property is read-only
};

template<class T> 
struct property_map< polygon::voronoi_proper_primal_graph<T>, vertex_Ycoordinate_t >
{
    typedef polygon::voronoi_proper_primal_graph<T> graph_t;
    typedef voronoi_coordinate_map<typename graph_t::vertex_descriptor, T, 1> const_type; //property is read-only
};


template<class T>
typename property_map< polygon::voronoi_proper_primal_graph<T>, vertex_Xcoordinate_t >::const_type
get(vertex_Xcoordinate_t, const polygon::voronoi_proper_primal_graph<T>& g) 
{
    typedef  polygon::voronoi_proper_primal_graph<T> graph_t;
    typedef typename graph_t::vertex_descriptor vertex_descriptor;
    return voronoi_coordinate_map< vertex_descriptor, T, 0 >();
}

template<class T>
typename property_map< polygon::voronoi_proper_primal_graph<T>, vertex_Ycoordinate_t >::const_type
get(vertex_Ycoordinate_t, const polygon::voronoi_proper_primal_graph<T>& g) 
{
    typedef  polygon::voronoi_proper_primal_graph<T> graph_t;
    typedef typename graph_t::vertex_descriptor vertex_descriptor;
    return voronoi_coordinate_map< vertex_descriptor, T, 1 >();
}

//ReadablePropertyMapConcept
template<class T, int k>  //can be voronoi_cell or voronoi_vertex or voronoi_edge
const T &
    get( voronoi_coordinate_map< typename polygon::voronoi_proper_primal_graph<T>::vertex_descriptor,  T, k> /*pmap*/, 
         typename polygon::voronoi_proper_primal_graph<T>::vertex_descriptor vertex) //aka const vertex_type* 
{
    if (k == 0) //compile time computation
        return vertex->x();
    else
        return vertex->y();
}


} //namespace boost

#endif //BOOST_VORONOI_PRIMAL_GRAPH_H_
