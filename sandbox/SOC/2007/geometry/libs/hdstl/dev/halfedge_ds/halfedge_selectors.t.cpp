//halfedge_selectors.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a selector class.  We
// must make sure that all selectors are suitably defined and that the
// selection is done properly.
//
//@TEST_PLAN: First create an instance of all the selectors (to make sure they
// exist, and verify that there are no more than tested).  Then instantiate the
// 'container_gen<HalfedgeS,ValueType>' for a given value type and all
// selectors, and verify that its members have the expected types and
// signatures.  Finally, verify that the usage example compiles and executes
// without errors, when assert is replaced by BOOST_CHECK.

#include <boost/hdstl/halfedge_ds/halfedge_selectors.hpp>

#include <boost/hdstl/halfedge_ds/facet_selectors.hpp>
#include <boost/hdstl/halfedge_ds/vertex_selectors.hpp>
#include <boost/hdstl/halfedge_ds/meta_functions.hpp>
#include <boost/pending/ct_if.hpp>

#include <boost/test/minimal.hpp>

#include <set>
#include <string>
#include <vector>

using namespace boost::hdstl;
using namespace std;

// ===========================================================================
//                              SELECTION CLASSES
// ===========================================================================

template <typename HalfedgeS>
bool selection_requirements(HalfedgeS const&) {
    return false;
}

template <typename Selector, typename ForwardCategory>
bool selection_requirements(halfedgeS<Selector, 
                            forwardS<ForwardCategory> > const&) {
    return true;
}

template <typename Selector, typename BackwardCategory>
bool selection_requirements(halfedgeS<Selector, 
                            backwardS<BackwardCategory> > const&) {
    return true;
}

template <typename Selector, typename ForwardCategory,
                             typename BackwardCategory>
bool selection_requirements(halfedgeS<Selector, 
                            bidirS<ForwardCategory, 
                            BackwardCategory> > const&) {
    return true;
}

// ===========================================================================
//                              CLASS HALFEDGE_GEN
// ===========================================================================

typedef void* halfedge_ptr;

template <typename HalfedgeS, typename VertexS, typename FacetS>
struct halfedge_config {
    // This halfedge_config to identically replace the halfedge_ds_gen::config
    // and reproduced here for test purposes only. Note that this tests are 
    // not for the config class, but the halfedge_selectors, so config will
    // be tested in its own package.

    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = !meta_is_same<VertexS,noVertexS>::value,
        is_source = VertexS::is_source,
        halfedge_supports_facets = !meta_is_same<FacetS,noFacetS>::value
    };
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
};

template <typename HalfedgeS>
struct halfedge_config<HalfedgeS, noVertexS, noFacetS> {
    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = false,
        is_source = false,
        halfedge_supports_facets = false
    };
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
};

template <typename HalfedgeS, typename FacetS>
struct halfedge_config<HalfedgeS, noVertexS, FacetS> {
    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = false,
        is_source = false,
        halfedge_supports_facets = !meta_is_same<FacetS,noFacetS>::value
    };
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
};

template <typename HalfedgeS, typename VertexS>
struct halfedge_config<HalfedgeS, VertexS, noFacetS> {
    enum {
        halfedge_has_opposite_member = !meta_is_same<
                        typename HalfedgeS::container_selector, vecS>::value,
        is_forward = HalfedgeS::is_forward,
        is_backward = HalfedgeS::is_backward,
        halfedge_supports_vertices = !meta_is_same<VertexS,noVertexS>::value,
        is_source = VertexS::is_source,
        halfedge_supports_facets = false
    };
    typedef typename boost::ct_if<halfedge_has_opposite_member,
                  halfedge_ptr, std::size_t>::type halfedge_descriptor;
};


template <typename HalfedgeGen>
bool halfedge_gen_requirements() {
    
    // Types must exist.
    typedef typename HalfedgeGen::halfedge_selector   halfedge_selector;
    typedef typename HalfedgeGen::halfedge_iterator   halfedge_iterator;
    typedef typename HalfedgeGen::halfedge_descriptor halfedge_descriptor;
    typedef typename HalfedgeGen::halfedge_type       halfedge_type;
    typedef typename HalfedgeGen::container_type      container_type;
    typedef typename HalfedgeGen::ContainerGen        container_gen;
    typedef typename HalfedgeGen::config              config;

    halfedge_type fa;
    halfedge_type fb;
    halfedge_type fc;
    halfedge_type fd;
    halfedge_type array[] = { fa, fb, fc, fd };  
    
    // Construct a halfedge_gen object whose container contains array.

    container_type halfedges(array, array+4);
    HalfedgeGen halfedgeGen;
    halfedgeGen.m_container = halfedges;

    BOOST_CHECK(( halfedgeGen.m_container.size() == 4 ));

    halfedge_iterator   halfedges_begin     = container_gen::container_begin(halfedgeGen.m_container); 
    halfedge_descriptor first_halfedge      = *halfedges_begin;
    halfedge_type&      first_halfedge_ref  = container_gen::value(first_halfedge, halfedgeGen.m_container);
    
    BOOST_CHECK(( &first_halfedge_ref == &(*halfedgeGen.m_container.begin())));

    return true;
}

// ===========================================================================
//                              USAGE EXAMPLE
// ===========================================================================

// The usage example is the component itself.

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <typename ContainerS>
bool test_container_selector()
{
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                            forwardS<next_in_facet_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                           forwardS<next_at_source_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                           forwardS<next_at_target_tag> >()) ));
    
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                           backwardS<prev_in_facet_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                          backwardS<prev_at_source_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                                          backwardS<prev_at_target_tag> >()) ));
    
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                           bidirS<next_in_facet_tag, prev_in_facet_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                         bidirS<next_at_source_tag, prev_at_source_tag> >()) ));
    BOOST_CHECK(( selection_requirements(halfedgeS<ContainerS,
                         bidirS<next_at_target_tag, prev_at_target_tag> >()) ));

    // Check 'forwardS'.
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'backwardS'.
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, backwardS<prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'bidirS'.
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                   prev_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_in_facet_tag,
                                                     prev_in_facet_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                   prev_at_source_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_source_tag,
                                                       prev_at_source_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                   prev_at_target_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, bidirS<next_at_target_tag,
                                                     prev_at_target_tag> >, 
                        noVertexS, noFacetS> > 
                  >() ));
                    
    // Check 'vertexS'.
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,false,sourceS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,false,targetS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,true,sourceS>, noFacetS> > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        vertexS<ContainerS,true,targetS>, noFacetS> > 
                  >() ));
    
    // Check 'facetS'.
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, facetS<ContainerS,true> > > 
                  >() ));
    BOOST_CHECK(( halfedge_gen_requirements<
                    halfedge_gen<
                      halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                      int, int, 
                      halfedge_config<
                        halfedgeS<ContainerS, forwardS<next_in_facet_tag> >, 
                        noVertexS, facetS<ContainerS,false> > > 
                  >() ));

    return true;
}

int test_main(int, char**)
{
    BOOST_CHECK(( test_container_selector<listS>() ));
    BOOST_CHECK(( test_container_selector<vecS>() ));
    // BOOST_CHECK(( test_container_selector() ));
    return 0;
}
