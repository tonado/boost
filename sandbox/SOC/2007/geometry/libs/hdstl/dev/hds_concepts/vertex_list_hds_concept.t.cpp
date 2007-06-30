//vertex_list_hds_concept.t.cpp   -*- C++ -*-
//
//@OVERVIEW:  The component under test is a concept-checking class.  We
// proceed with the standard test plan for such a class.
//
//@TEST_PLAN:  Create a (loose) archetype of the concept, and verify
// (using the Boost.ConceptCheck library macros) that it satisfies all
// the concept requirements of the concept.  Note that the purpose of
// this test driver is to verify the validity of the concept checking
// class, not of the archetype.  This is why it suffices to take the
// 'halfedge_descriptor' as an 'int', although a real archetype would make
// this into a custom-made class with the tailored minimal requirements.

#include <boost/hdstl/hds_concepts/vertex_list_hds_concept.hpp>
#include <boost/test/minimal.hpp>
#include <boost/concept_check.hpp>

namespace hdstl1 {

    struct vertex_list_hds_archetype {
        // This 'struct', intentionally defined in a namespace different from
        // 'hdstl', the 'hds_traits' specialization defined in the namespace
        // 'hdstl', and the supporting function 'opposite', defined in the same
        // namespace as this 'struct' to take advantage of ADL
        // (argument-dependent lookup) provide an archetype for the 'HDS'
        // concept.

        typedef int* vertex_iterator;
        typedef int halfedge_descriptor;
        typedef int vertex_descriptor;
        typedef int size_type;
    };

    vertex_list_hds_archetype::halfedge_descriptor
        opposite(vertex_list_hds_archetype::halfedge_descriptor h,  
                const vertex_list_hds_archetype&)
    {
        return h;
    }
    
    vertex_list_hds_archetype::vertex_iterator
        vertices_begin(const vertex_list_hds_archetype&)
    {
        return vertex_list_hds_archetype::vertex_iterator();
    }
    
    vertex_list_hds_archetype::vertex_iterator
        vertices_end(const vertex_list_hds_archetype&)
    {
        return vertex_list_hds_archetype::vertex_iterator();
    }
    
    vertex_list_hds_archetype::size_type
        num_vertices(const vertex_list_hds_archetype&)
    {
        return vertex_list_hds_archetype::size_type();
    }
    
    vertex_list_hds_archetype::vertex_descriptor
    source(vertex_list_hds_archetype::halfedge_descriptor,
             const vertex_list_hds_archetype&)
    {
        return vertex_list_hds_archetype::vertex_descriptor();
    }

    vertex_list_hds_archetype::vertex_descriptor
    target(vertex_list_hds_archetype::halfedge_descriptor,
             const vertex_list_hds_archetype&)
    {
        return vertex_list_hds_archetype::vertex_descriptor();
    }


}  // namespace hdstl

namespace boost {
namespace hdstl {
    
    template <>
    struct hds_traits<hdstl1::vertex_list_hds_archetype>
    {
        typedef hdstl1::vertex_list_hds_archetype::halfedge_descriptor
                halfedge_descriptor;
        typedef hdstl1::vertex_list_hds_archetype::vertex_descriptor
                vertex_descriptor;
        typedef hdstl1::vertex_list_hds_archetype::vertex_iterator
                vertex_iterator;
        typedef hdstl1::vertex_list_hds_archetype::size_type size_type;
        enum { supports_vertices = true};
        static const int supports_facets = false;
    };
    
}  // namespace hdstl
}  // namespace boost

// ===========================================================================
//                              BOOST TEST APPARATUS
// ===========================================================================

template <class HDS>
struct class_concept_requirements
{
    BOOST_CLASS_REQUIRE(HDS, boost::hdstl::concepts, VertexListHDSConcept);
};

template <class HDS>
bool concept_requirements()
{
    boost::function_requires<boost::hdstl::concepts::VertexListHDSConcept<HDS> >();
    class_concept_requirements<HDS>(); // force instantiation
    return true;
}

int test_main(int, char **)
{
    BOOST_CHECK(( concept_requirements<hdstl1::vertex_list_hds_archetype>() ));
    return 0;
}
