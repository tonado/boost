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

#ifndef BOOST_GRAPH_NAMED_FUNCTION_PARAMS_HPP
#define BOOST_GRAPH_NAMED_FUNCTION_PARAMS_HPP

#include <boost/graph/properties.hpp>

namespace boost {

  struct distance_compare_t { enum { num = detail::distance_compare_num }; };
  struct distance_combine_t { enum { num = detail::distance_combine_num};  };
  struct distance_inf_t { enum { num = detail::distance_inf_num }; };
  struct distance_zero_t { enum { num = detail::distance_zero_num }; };

  template <typename T, typename Tag, typename Base = no_property>
  struct bgl_named_params : public Base
  {
    typedef bgl_named_params self;
    typedef Base next_type;
    typedef Tag tag_type;
    typedef T value_type;
    bgl_named_params(T v) : m_value(v) { }
    bgl_named_params(T v, const Base& b) : Base(b), m_value(v) { }
    T m_value;

    template <typename WeightMap>
    bgl_named_params<WeightMap, edge_weight_t, self>
    weight_map(const WeightMap& pmap) {
      typedef bgl_named_params<WeightMap, edge_weight_t, self> Params;
      return Params(pmap, *this);
    }

    template <typename DistanceMap>
    bgl_named_params<DistanceMap, vertex_distance_t, self>
    distance_map(const DistanceMap& pmap) {
      typedef bgl_named_params<DistanceMap, vertex_distance_t, self> Params;
      return Params(pmap, *this);
    }

    template <typename ColorMap>
    bgl_named_params<ColorMap, vertex_color_t, self>
    color_map(const ColorMap& pmap) {
      typedef bgl_named_params<ColorMap, vertex_color_t, self> Params;
      return Params(pmap, *this);
    }

    template <typename IndexMap>
    bgl_named_params<IndexMap, vertex_index_t, self>
    vertex_index_map(const IndexMap& pmap) {
      typedef bgl_named_params<IndexMap, vertex_index_t, self> Params;
      return Params(pmap, *this);
    }

    template <typename Visitor>
    bgl_named_params<Visitor, graph_visitor_t, self>
    visitor(const Visitor& vis) {
      typedef bgl_named_params<Visitor, graph_visitor_t, self> Params;
      return Params(vis, *this);
    }

    template <typename Compare>
    bgl_named_params<Compare, distance_compare_t, self>
    distance_compare(const Compare& cmp) {
      typedef bgl_named_params<Compare, distance_compare_t, self> Params;
      return Params(cmp, *this);
    }

    template <typename Combine>
    bgl_named_params<Combine, distance_combine_t, self>
    distance_combine(const Combine& cmb) {
      typedef bgl_named_params<Combine, distance_combine_t, self> Params;
      return Params(cmb, *this);
    }

    template <typename Init>
    bgl_named_params<Init, distance_inf_t, self>
    distance_inf(const Init& init) {
      typedef bgl_named_params<Init, distance_inf_t, self> Params;
      return Params(init, *this);
    }

    template <typename Init>
    bgl_named_params<Init, distance_zero_t, self>
    distance_zero(const Init& init) {
      typedef bgl_named_params<Init, distance_zero_t, self> Params;
      return Params(init, *this);
    }
  };

  template <typename WeightMap>
  bgl_named_params<WeightMap, edge_weight_t>
  weight_map(WeightMap pmap) {
    typedef bgl_named_params<WeightMap, edge_weight_t> Params;
    return Params(pmap);
  }

  template <typename DistanceMap>
  bgl_named_params<DistanceMap, vertex_distance_t>
  distance_map(DistanceMap pmap) {
    typedef bgl_named_params<DistanceMap, vertex_distance_t> Params;
    return Params(pmap);
  }

  template <typename ColorMap>
  bgl_named_params<ColorMap, vertex_color_t>
  color_map(ColorMap pmap) {
    typedef bgl_named_params<ColorMap, vertex_color_t> Params;
    return Params(pmap);
  }

  template <typename IndexMap>
  bgl_named_params<IndexMap, vertex_index_t>
  vertex_index_map(IndexMap pmap) {
    typedef bgl_named_params<IndexMap, vertex_index_t> Params;
    return Params(pmap);
  }

  template <typename Visitor>
  bgl_named_params<Visitor, graph_visitor_t>
  visitor(const Visitor& vis) {
    typedef bgl_named_params<Visitor, graph_visitor_t> Params;
    return Params(vis);
  }

  template <typename Compare>
  bgl_named_params<Compare, distance_compare_t>
  distance_compare(const Compare& cmp) {
    typedef bgl_named_params<Compare, distance_compare_t> Params;
    return Params(cmp);
  }

  template <typename Combine>
  bgl_named_params<Combine, distance_combine_t>
  distance_combine(const Combine& cmb) {
    typedef bgl_named_params<Combine, distance_combine_t> Params;
    return Params(cmb);
  }

  template <typename Init>
  bgl_named_params<Init, distance_inf_t>
  distance_inf(const Init& init) {
    typedef bgl_named_params<Init, distance_inf_t> Params;
    return Params(init);
  }

  template <typename Init>
  bgl_named_params<Init, distance_zero_t>
  distance_zero(const Init& init) {
    typedef bgl_named_params<Init, distance_zero_t> Params;
    return Params(init);
  }

  //===========================================================================
  // Functions for extracting parameters from bgl_named_params

  template <class Tag1, class Tag2, class T1, class Base>
  inline
  typename property_value< bgl_named_params<T1,Tag1,Base>, Tag2>::type
  get_param(const bgl_named_params<T1,Tag1,Base>& p, Tag2 tag2)
  {
    enum { match = detail::same_property<Tag1,Tag2>::value };
    typedef typename
      property_value< bgl_named_params<T1,Tag1,Base>, Tag2>::type T2;
    T2* t2 = 0;
    typedef detail::property_value_dispatch<match> Dispatcher;
    return Dispatcher::const_get_value(p, t2, tag2);
  }

  template <typename Default>
  const Default& 
  choose_param(const detail::error_property_not_found&, const Default& d)
    { return d; }

  template <class P, class Default> 
  const P&
  choose_param(const P& param, const Default&) { return param; }

  template <typename T>
  inline bool is_default_param(const T&) { return false; }

  inline bool is_default_param(const detail::error_property_not_found&)
    { return true; }

  namespace detail {

    struct choose_parameter {
      template <class P, class Graph, class Tag>
      struct bind {
	typedef const P& const_result_type;
	typedef P& result_type;
	static const_result_type apply(const P& p, const Graph&, Tag&)
	{ return p; }
	static result_type apply(P& p, Graph&, Tag&)
	{ return p; }
      };
    };
    struct choose_default_param {
      template <class P, class Graph, class Tag>
      struct bind {
	typedef typename property_map<Graph, Tag>::type 
	  result_type;
	typedef typename property_map<Graph, Tag>::const_type 
	  const_result_type;

	static const_result_type
	apply(const P& p, const Graph& g, Tag tag) { 
	  return get(tag, g); 
	}

	static result_type
	apply(P& p, Graph& g, Tag tag) { 
	  return get(tag, g); 
	}
      };
    };

    template <class Param>
    struct choose_property_map {
      typedef choose_parameter type;
    };
    template <>
    struct choose_property_map<detail::error_property_not_found> {
      typedef choose_default_param type;
    };

    template <class Param, class Graph, class Tag>
    struct choose_pmap_helper {
      typedef typename choose_property_map<Param>::type Selector;
      typedef typename Selector:: template bind<Param, Graph, Tag> type;
      typedef typename type::result_type result_type;
      typedef typename type::const_result_type const_result_type;
    };

  } // namespace detail
  

  // Use this function instead of choose_param() when you want
  // to avoid requiring get(tag, g) when it is not used. 
  template <typename Param, typename Graph, typename PropertyTag>
  typename
    detail::choose_pmap_helper<Param,Graph,PropertyTag>::const_result_type
  choose_pmap(const Param& p, const Graph& g, PropertyTag tag)
  { 
    typedef typename 
      detail::choose_pmap_helper<Param,Graph,PropertyTag>::type Choice;
    return Choice::apply(p, g, tag);
  }

  template <typename Param, typename Graph, typename PropertyTag>
  typename detail::choose_pmap_helper<Param,Graph,PropertyTag>::result_type
  choose_pmap(Param& p, Graph& g, PropertyTag tag)
  { 
    typedef typename 
      detail::choose_pmap_helper<Param,Graph,PropertyTag>::type Choice;
    return Choice::apply(p, g, tag);
  }

} // namespace boost

#endif // BOOST_GRAPH_NAMED_FUNCTION_PARAMS_HPP
