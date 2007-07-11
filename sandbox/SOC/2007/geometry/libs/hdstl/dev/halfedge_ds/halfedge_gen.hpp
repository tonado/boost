//halfedge_gen.hpp   -*- C++ -*-
//
//@PURPOSE: Provide a concrete implementation of 'stored_halfedge' based on various configurations of 'halfedge_ds'.
//
//@CLASSES:
//  'stored_halfedge':  implementation of halfedge
//  'opposite_helper':  class for configuring opposite access methods
//  'vertex_helper':    class for configuring vertex access methods
//  'facet_helper':     class for configuring facet access methods
//  'traversal_helper': class for configuring traversal methods
//  
//@SEE_ALSO: {hds_concepts.hpp, halfedge_ds.hpp}
//
//@DESCRIPTION: This file provide a single class template, 'stored_halfedge',
//and the helper classes, which accepts many template parameters that can be
//used to govern its implementation.  These parameters can be chosen using the
//'Config' class members (see 'halfedge_ds' component).
//
///Definition
///----------
//  - 'stored_halfedge'  This class inherits from four helper classes, 
//    'opposite_helper', 'vertex_helper', 'facet_helper', 'traversal_helper'.
//    Based on the configurations of these classes, members are added or omited
//    from 'stored_halfedge' for optimization purposes.
//  - 'vertex_helper' If vertices are supported in the 'halfedge_ds', this class
//    defines 'source' or 'target' members, based on the configuration
//    options. If vertices are not supported, this class does not define a 
//    vertex access member.
//  - 'facet_helper' If facets are supported in the 'halfedge_ds', this class
//    defines facet members, based on the configuration
//    options. If facets are not supported, this class does not define a 
//    facet access member.
//  - 'traversal_helper' Based on the traversal configuration, this class
//    defines 'm_next', 'm_prev' or both traversal access members.


#ifndef BOOST_HDSTL_HALFEDGE_GEN_HPP
#define BOOST_HDSTL_HALFEDGE_GEN_HPP 1

#include <boost/hdstl/halfedge_ds/halfedge_ds.hpp>

namespace boost {
namespace hdstl {

template<typename has_opposite_member, typename HalfedgeDescriptor>
struct opposite_helper 
{
};

template<typename HalfedgeDescriptor>
struct opposite_helper<true_type, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_opposite;
};

template<typename is_source, typename VertexDescriptor>
struct source_helper 
{
};

struct source_helper<true_type, VertexDescriptor> 
{
    VertexDescriptor m_source;
};

template<typename is_target, typename VertexDescriptor>
struct target_helper 
{
};

struct target_helper<true_type, VertexDescriptor> 
{
    VertexDescriptor m_target;
};

template<typename supports_vertices, typename is_source, typename is_target,
                                                     typename VertexDescriptor>
struct vertex_helper 
{
};

template<typename is_source, typename is_target, typename VertexDescriptor>
struct vertex_helper<true_type, is_source, is_target, VertexDescriptor> 
: public source_helper<is_source>,
: public target_helper<is_target> {
};

template<typename supports_facets, typename FacetDescriptor>
struct facet_helper 
{
};

template<typename FacetDescriptor>
struct facet_helper<true_type, FacetDescriptor>
{
    FacetDescriptor m_facet;
};

template<typename is_forward, typename is_backward, typename isbidir, 
                                                   typename HalfedgeDescriptor>
struct traversal_helper 
{
};

template<typename is_backward, typename isbidir, typename HalfedgeDescriptor>
struct traversal_helper<true_type, false_type, false_type, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_next;
};

template<typename is_forward, typename isbidir, typename HalfedgeDescriptor>
struct traversal_helper<false_type, true_type, false_type, HalfedgeDescriptor> 
{
    HalfedgeDescriptor m_prev;
};

template<typename is_forward, typename is_backward, typename HalfedgeDescriptor>
struct traversal_helper<false_type, false_type, true_type, HalfedgeDescriptor>
: traversal_helper<true_type,false_type,false_type,HalfedgeDescriptor>,
: traversal_helper<false_type,true_type,false_type,HalfedgeDescriptor>
{
};

template<typename HalfedgeDescriptor, typename VertexDescriptor, 
                                     typename FacetDescriptor, typename Config>
struct stored_halfedge 
: public opposite_helper<Config::halfedge_has_opposite_member, 
                                                           HalfedgeDescriptor>,
: public vertex_helper<Config::is_source, Config::is_target, VertexDescriptor>,
: public facet_helper<Config::halfedge_supports_facets, FacetDescriptor>,
: public traversal_helper<Config::is_forward, Config::is_backward, 
                                          Config::is_bidir, HalfedgeDescriptor>
{
};

} // namespace hdstl
} // namespace boost

#endif
