// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_CYCLE_HXX
#define BOOST_GRAPH_CYCLE_HXX

#include <vector>
#include <limits>

#include <boost/utility.hpp>
#include <boost/graph/graph_traits.hpp>

namespace boost
{

    // The implementation of this algorithm is a reproduction of the Teirnan
    // approach for directed graphs: bibtex follows
    //
    //     @article{362819,
    //         author = {James C. Tiernan},
    //         title = {An efficient search algorithm to find the elementary circuits of a graph},
    //         journal = {Commun. ACM},
    //         volume = {13},
    //         number = {12},
    //         year = {1970},
    //         issn = {0001-0782},
    //         pages = {722--726},
    //         doi = {http://doi.acm.org/10.1145/362814.362819},
    //             publisher = {ACM Press},
    //             address = {New York, NY, USA},
    //         }
    //
    // It should be pointed out that the author does not provide a complete analysis for
    // either time or space. This is in part, due to the fact that it's a fairly input
    // sensitive problem related to the density and construction of the graph, not just
    // its size.
    //
    // I've also taken some liberties with the interpretation of the algorithm - I've
    // basically modernized it to use real data structures (no more arrays and matrices).
    // Oh... and there's explicit control structures - not just gotos.
    //
    // The problem is definitely NP-complete, an an unbounded implementation of this
    // will probably run for quite a while (i.e.) on a large graph. The conclusions
    // of this paper alkso reference a Paton algorithm for undirected graphs as being
    // much more efficient (apparently based on spanning trees). Although not implemented,
    // it can be found here:
    //
    //     @article{363232,
    //         author = {Keith Paton},
    //         title = {An algorithm for finding a fundamental set of cycles of a graph},
    //         journal = {Commun. ACM},
    //         volume = {12},
    //         number = {9},
    //         year = {1969},
    //         issn = {0001-0782},
    //         pages = {514--518},
    //         doi = {http://doi.acm.org/10.1145/363219.363232},
    //             publisher = {ACM Press},
    //             address = {New York, NY, USA},
    //         }

    // TODO: The algorithm, as presented, suffers from a number of disappoinments (not
    // just being slow. It only really works on the most simple directed and undirected
    // graphs. Because of the requirement on increasingly large vertex indices, the
    // algorithm will never follow bidirected or reflexive edges. This failure causes
    // the algorithm to miss a number cycles that end up bridging those reflexive
    // humps.
    //
    // An interesting tradeoff would be to remove the test for increasing indices
    // in detail::ignore_vertex(), causing the algorithm to visit every permutation
    // of each cycle - that actually hits them all. Some extra work could be done
    // mapping permuations into combinations - if you really want to.
    //
    // However, I should point out that this never miscomputes triangles - which
    // is what I'd intended it for.

    struct cycle_visitor
    {
        template <class Vertex, class Graph>
        inline void start_vertex(Vertex v, Graph& g)
        { }

        template <class Vertex, class Graph>
        inline void finish_vertex(Vertex v, Graph& g)
        { }

        template <class Path, class Graph>
        inline void cycle(const Path& p, Graph& g)
        { }
    };

    struct cycle_counter
        : public cycle_visitor
    {
        cycle_counter(std::size_t& num)
            : m_num(num)
        { }

        template <class Path, class Graph>
        inline void cycle(const Path& p, Graph& g)
        { ++m_num; }

        size_t& m_num;
    };

    namespace detail
    {
        template <typename Graph, typename Path>
        inline bool
        is_in_path(const Graph&,
                   typename graph_traits<Graph>::vertex_descriptor v,
                   const Path& p)
        {
            return (std::find(p.begin(), p.end(), v) != p.end());
        }

        template <typename Graph, typename ClosedMatrix>
        inline bool
        is_path_closed(const Graph& g,
                       typename graph_traits<Graph>::vertex_descriptor u,
                       typename graph_traits<Graph>::vertex_descriptor v,
                       const ClosedMatrix& closed)
        {
            // the path from u to v is closed if v can be found in the list
            // of closed vertices associated with u.
            typedef typename ClosedMatrix::const_reference Row;
            Row r = closed[get(vertex_index, g, u)];
            if(find(r.begin(), r.end(), v) != r.end()) {
                return true;
            }
            return false;
        }

        template <typename Graph, typename Path, typename ClosedMatrix>
        inline bool
        ignore_vertex(const Graph& g,
                      typename graph_traits<Graph>::vertex_descriptor u,
                      typename graph_traits<Graph>::vertex_descriptor v,
                      const Path& p,
                      const ClosedMatrix& m)
        {
            // for undirected graphs, we're actually going to follow the
            // original algorithm and disallow back-tracking over the
            // undirected edge.
            return get(vertex_index, g, v) < get(vertex_index, g, u) ||
                   is_in_path(g, v, p) ||
                   is_path_closed(g, u, v, m);
        }

        template <typename Graph, typename Path, typename ClosedMatrix>
        inline typename graph_traits<Graph>::vertex_descriptor
        extend_path(const Graph& g, Path& p, ClosedMatrix& closed)
        {
            typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
            typedef typename graph_traits<Graph>::adjacency_iterator AdjacencyIterator;

            // basically, p is a sequence of vertex descriptors.
            // we want to find an adjacent vertex that the path
            // can extend over.

            // get the current vertex
            Vertex u = p.back();
            Vertex ret = graph_traits<Graph>::null_vertex();

            AdjacencyIterator i, end;
            for(tie(i, end) = adjacent_vertices(u, g); i != end; ++i) {
                Vertex v = *i;
                if(ignore_vertex(g, u, v, p, closed)) {
                    continue;
                }

                // if we get here, we've actually satisfied the loop
                // so we can just break and return
                p.push_back(v);
                ret = v;
                break;
            }
            return ret;
        }

        template <typename Graph, typename Path, typename ClosedMatrix>
        inline bool
        exhaust_paths(const Graph& g, Path& p, ClosedMatrix& closed)
        {
            typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

            // if there's more than one vertex in the path, this closes
            // of some possible routes and returns true. otherwise, if there's
            // only one vertex left, the vertex has been used up
            if(p.size() > 1) {
                // get the last and second to last vertices, popping the last
                // vertex off the path
                Vertex last, prev;
                last = p.back();
                p.pop_back();
                prev = p.back();

                // reset the closure for the last vertex of the path and
                // indicate that the last vertex in p is now closed to
                // the next-to-last vertex in p
                closed[get(vertex_index, g, last)].clear();
                closed[get(vertex_index, g, prev)].push_back(last);
                return true;
            }
            else {
                return false;
            }
        }
    }

    // TODO: I may need to templatize the path type - it would add a little extra
    // flexibility, but I'm not certain its a great idea.

    template <typename Graph, typename Visitor>
    inline void
    visit_cycles(const Graph& g,
                 Visitor vis,
                 std::size_t maxlen = std::numeric_limits<std::size_t>::max(),
                 std::size_t minlen = 3)
    {
        typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
        typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;
        typedef std::vector<Vertex> Path;
        typedef std::vector<Vertex> VertexList;
        typedef std::vector<VertexList> ClosedMatrix;

        // closed contains the list of vertices that are "closed" to a vertex. this
        // is a kind of strange half-mapped matrix. rows are indexed by vertex, but
        // the columns are not - they simply store the list of vertices that are
        // closed to the vertex represented by the row.

        const Vertex null = graph_traits<Graph>::null_vertex();

        VertexIterator i, end;
        for(tie(i, end) = vertices(g); i != end; ++i) {
            Path p;
            ClosedMatrix closed(num_vertices(g), VertexList());
            p.push_back(*i);

            vis.start_vertex(*i, g);

            while(1) {
                // extend the path until we've reached the end or the maximum
                // sized cycle
                Vertex j = null;
                while(((j = detail::extend_path(g, p, closed)) != null)
                       && (p.size() < maxlen))
                    ; // empty loop

                // at this point, we need to see if there's a cycle
                if(edge(p.back(), p.front(), g).second) {
                    if(p.size() >= minlen) {
                        vis.cycle(p, g);
                    }
                }

                if(!detail::exhaust_paths(g, p, closed)) {
                    break;
                }
            }

            vis.finish_vertex(*i, g);
        }
    }

    template <typename Graph>
    inline size_t
    count_cycles(const Graph& g,
                 std::size_t maxlen = std::numeric_limits<std::size_t>::max(),
                 std::size_t minlen = 3)
    {
        size_t ret = 0;
        visit_triangles(g, cycle_counter(ret), maxlen, minlen);
        return ret;
    }

    template <typename Graph, typename Visitor>
    inline void
    visit_triangles(const Graph& g, Visitor vis)
    { visit_cycles(g, vis, 3, 3); }

    template <typename Graph>
    inline size_t
    count_triangles(const Graph& g)
    {
        size_t ret = 0;
        visit_triangles(g, cycle_counter(ret));
        return ret;
    }
}

#endif
