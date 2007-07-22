//halfedge_functions.hpp   -*- C++ -*-
//
//@PURPOSE: Provide free functions for accessing and manipulating halfedges.
//
//@DESCRIPTION: This file provides free functions for accessing and
// manipulating halfedges. The list of these functions are as follows:
//
//  - halfedges_begin: returns an iterator to the first halfedge in the 'hds' 
//  -   halfedges_end: returns an iterator after the last halfedge in the 'hds'
//  -   num_halfedges: returns the number of halfedges in the 'hds'
//  -        opposite: returns the opposite of the given halfedge in the 'hds'
//  -   next_in_facet: returns a halfedge descriptor to the halfedge succeeding 
//                     the given halfedge in the adjacent facet cycle, when facet 
//                     cycles are oriented in counter-clockwise order.
//  -  next_at_source: returns a halfedge descriptor to the halfedge succeeding 
//                     the given halfedge around the source vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  -  next_at_target: returns a halfedge descriptor to the halfedge succeeding 
//                     the given halfedge around the target vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  -   prev_in_facet: returns a halfedge descriptor to the halfedge preceeding 
//                     the given halfedge in the adjacent facet cycle, when facet 
//                     cycles are oriented in counter-clockwise order.
//  -  prev_at_source: returns a halfedge descriptor to the halfedge preceeding 
//                     the given halfedge around the source vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  -  prev_at_target: returns a halfedge descriptor to the halfedge preceeding 
//                     the given halfedge around the target vertex, when halfedges 
//                     are ordered around a given vertex in clockwise order
//  
///Definitions
//------------
// All next_... and prev_... methods take a forward traversal tag, which defines how the
// next or previous halfedges are stored in the data structure. Using these tags, all three
// methods can be called so that the specializations can take care of converting 
// one access method to another.
//

#ifndef BOOST_HDSTL_HALFEDGE_FUNCTIONS_HPP
#define BOOST_HDSTL_HALFEDGE_FUNCTIONS_HPP 1

#include <boost/hdstl/hds_traits.hpp>
#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>

namespace boost {
namespace hdstl {

// FREE FUNCTIONS
template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>::halfedge_iterator
halfedges_begin(halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>
                ::ContainerGen ContainerGen;
    return ContainerGen::container_begin(hds.m_container);
}

template <typename HalfedgeS,  typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>::halfedge_iterator
halfedges_end(halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>
                ::ContainerGen ContainerGen;
    return ContainerGen::container_end(hds.m_container);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>::size_type
num_halfedges(halfedge_gen<HalfedgeS,  VertexDescriptor, FacetDescriptor, Config>& hds)
{
    return hds.m_container.size();
}

// \cond
template<typename HalfedgeS, typename HalfedgeDescriptor>
struct opposite_function_helper {
    static
    HalfedgeDescriptor
    opposite(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(h->m_opposite);
    }
};

template<typename HalfedgeDescriptor, typename TraversalS>
struct opposite_function_helper<halfedgeS<vecS,TraversalS>, HalfedgeDescriptor> {
    static
    HalfedgeDescriptor
    opposite(HalfedgeDescriptor h)
    {
        return h^1;
    }
};
// \endcond

template <typename HalfedgeS,  typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
opposite(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
         const halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>&)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_selector halfedge_selector;
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    return opposite_function_helper<halfedge_selector,
                                    halfedge_descriptor
                                   >::opposite(h);
}

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = next_in_facet_tag>
struct next_in_facet_helper{
    static
    HalfedgeDescriptor
    next_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_in_facet_helper<HalfedgeGen, HalfedgeDescriptor, next_at_source_tag>{
    static
    HalfedgeDescriptor
    next_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(
                   static_cast<HalfedgeDescriptor>(opposite(h, hds))
                         ,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_in_facet_helper<HalfedgeGen, HalfedgeDescriptor, next_at_target_tag>{
    static
    HalfedgeDescriptor
    next_in_facet(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                   static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next)
                         , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = next_at_source_tag>
struct next_at_source_helper{
    static
    HalfedgeDescriptor
    next_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_source_helper<HalfedgeGen, HalfedgeDescriptor, next_in_facet_tag>{
    static
    HalfedgeDescriptor
    next_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(
                   static_cast<HalfedgeDescriptor>(opposite(h, hds))
                         ,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_source_helper<HalfedgeGen, HalfedgeDescriptor, next_at_target_tag>{
    static
    HalfedgeDescriptor
    next_at_source(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                    static_cast<HalfedgeDescriptor>(containerGen::value(
                        static_cast<HalfedgeDescriptor>(opposite(h, hds))
                            ,hds.m_container).m_next)
                                , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor, typename Tag = next_at_target_tag>
struct next_at_target_helper{
    static
    HalfedgeDescriptor
    next_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next);
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_target_helper<HalfedgeGen, HalfedgeDescriptor, next_in_facet_tag>{
    static
    HalfedgeDescriptor
    next_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                   static_cast<HalfedgeDescriptor>(containerGen::value(h,hds.m_container).m_next)
                         , hds));
    }
};

template<typename HalfedgeGen, typename HalfedgeDescriptor>
struct next_at_target_helper<HalfedgeGen, HalfedgeDescriptor, next_at_source_tag>{
    static
    HalfedgeDescriptor
    next_at_target(HalfedgeDescriptor h, HalfedgeGen& hds)
    {
        typedef typename HalfedgeGen::ContainerGen containerGen;
        return static_cast<HalfedgeDescriptor>(opposite(
                    static_cast<HalfedgeDescriptor>(containerGen::value(
                        static_cast<HalfedgeDescriptor>(opposite(h, hds))
                            ,hds.m_container).m_next)
                                , hds));
    }
};

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
next_in_facet(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return next_in_facet_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::next_tag>::next_in_facet(h, hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
next_at_source(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return next_at_source_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::next_tag>::next_at_source(h,hds);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
next_at_target(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>& hds)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    typedef  halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config> halfedgeGen;

    return next_at_target_helper<halfedgeGen, halfedge_descriptor, typename HalfedgeS::next_tag>::next_at_target(h,hds);
}

template<typename HalfedgeDescriptor, typename Tag = prev_in_facet_tag>
struct prev_in_facet_helper{
    static
    HalfedgeDescriptor
    prev_in_facet(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(h->m_prev);
    }
};

template<typename HalfedgeDescriptor>
struct prev_in_facet_helper<HalfedgeDescriptor, prev_at_source_tag>{
    static
    HalfedgeDescriptor
    prev_in_facet(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(static_cast<HalfedgeDescriptor>(h->m_prev)->m_opposite);
    }
};

template<typename HalfedgeDescriptor>
struct prev_in_facet_helper<HalfedgeDescriptor, prev_at_target_tag>{
    static
    HalfedgeDescriptor
    prev_in_facet(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(static_cast<HalfedgeDescriptor>(h->m_opposite)->m_prev);
    }
};

template<typename HalfedgeDescriptor, typename Tag = prev_at_source_tag>
struct prev_at_source_helper{
    static
    HalfedgeDescriptor
    prev_at_source(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(h->m_prev);
    }
};

template<typename HalfedgeDescriptor>
struct prev_at_source_helper<HalfedgeDescriptor, prev_in_facet_tag>{
    static
    HalfedgeDescriptor
    prev_at_source(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(static_cast<HalfedgeDescriptor>(h->m_prev)->m_opposite);
    }
};

template<typename HalfedgeDescriptor>
struct prev_at_source_helper<HalfedgeDescriptor, prev_at_target_tag>{
    static
    HalfedgeDescriptor
    prev_at_source(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(
                   static_cast<HalfedgeDescriptor>(
                       static_cast<HalfedgeDescriptor>(h->m_opposite)->m_prev)->m_opposite);
    }
};

template<typename HalfedgeDescriptor, typename Tag = prev_at_target_tag>
struct prev_at_target_helper{
    static
    HalfedgeDescriptor
    prev_at_target(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(h->m_prev);
    }
};

template<typename HalfedgeDescriptor>
struct prev_at_target_helper<HalfedgeDescriptor, prev_in_facet_tag>{
    static
    HalfedgeDescriptor
    prev_at_target(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(static_cast<HalfedgeDescriptor>(h->m_opposite)->m_prev);
    }
};

template<typename HalfedgeDescriptor>
struct prev_at_target_helper<HalfedgeDescriptor, prev_at_source_tag>{
    static
    HalfedgeDescriptor
    prev_at_target(HalfedgeDescriptor h)
    {
        return static_cast<HalfedgeDescriptor>(
                  static_cast<HalfedgeDescriptor>(
                     static_cast<HalfedgeDescriptor>(h->m_opposite)->m_prev)->m_opposite);
    }
};

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
prev_in_facet(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
              halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>&)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    return prev_in_facet_helper<halfedge_descriptor, typename HalfedgeS::prev_tag>::prev_in_facet(h);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
prev_at_source(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>&)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    return prev_at_source_helper<halfedge_descriptor, typename HalfedgeS::prev_tag>::prev_at_source(h);
}

template <typename HalfedgeS, typename VertexDescriptor, typename FacetDescriptor, typename Config>
typename Config::halfedge_descriptor
prev_at_target(typename halfedge_gen<HalfedgeS, VertexDescriptor,FacetDescriptor, Config>::halfedge_descriptor h,
               halfedge_gen<HalfedgeS, VertexDescriptor, FacetDescriptor, Config>&)
{
    typedef typename halfedge_gen<HalfedgeS, 
                                  VertexDescriptor,FacetDescriptor, Config
                                 >::halfedge_descriptor halfedge_descriptor;
    return prev_at_target_helper<halfedge_descriptor, typename HalfedgeS::prev_tag>::prev_at_target(h);
}


} // namespace hdstl
} // namespace boost

#endif
