#ifdef BOOST_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "test6.hpp"

// Test matrix & vector expression templates 
template<class V, class M, int N>
struct test_my_matrix_vector {
    typedef typename V::value_type value_type;

    template<class VP, class MP>
    void operator () (VP &v1, VP &v2, MP &m1) const {
        try {
            // Rows and columns
            initialize_matrix (m1, ublas::lower_tag ());
            for (int i = 0; i < N; ++ i) {
                v2 = ublas::row (m1, i);
                std::cout << "row (m, " << i << ") = " << v2 << std::endl;
                v2 = ublas::column (m1, i);
                std::cout << "column (m, " << i << ") = " << v2 << std::endl;
            }

            // Outer product
            initialize_vector (v1);
            initialize_vector (v2);
            v1 (0) = 0;
            v1 (N - 1) = 0;
            v2 (0) = 0;
            v2 (N - 1) = 0;
            m1 = ublas::outer_prod (v1, v2);
            std::cout << "outer_prod (v1, v2) = " << m1 << std::endl;

            // Matrix vector product 
            initialize_matrix (m1, ublas::lower_tag ());
            initialize_vector (v1);
            v2 = ublas::prod (m1, v1);
            std::cout << "prod (m1, v1) = " << v2 << std::endl;
            v2 = ublas::prod (v1, m1);
            std::cout << "prod (v1, m1) = " << v2 << std::endl;
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
    void operator () () const {
        try {
            V v1 (N), v2 (N);
            M m1 (N, N);
            (*this) (v1, v2, m1);

            ublas::matrix_row<M> mr1 (m1, N - 1), mr2 (m1, N - 1);
            (*this) (mr1, mr2, m1);

            ublas::matrix_column<M> mc1 (m1, 0), mc2 (m1, 0);
            (*this) (mc1, mc2, m1);

#ifdef BOOST_UBLAS_ENABLE_INDEX_SET_ALL
#ifdef USE_RANGE_AND_SLICE
            ublas::matrix_vector_range<M> mvr1 (m1, ublas::range<> (0, N), ublas::range<> (0, N)),
                                          mvr2 (m1, ublas::range<> (0, N), ublas::range<> (0, N));
            (*this) (mvr1, mvr2, m1);

            ublas::matrix_vector_slice<M> mvs1 (m1, ublas::slice<> (0, 1, N), ublas::slice<> (0, 1, N)),
                                          mvs2 (m1, ublas::slice<> (0, 1, N), ublas::slice<> (0, 1, N));
            (*this) (mvs1, mvs2, m1);
#endif
#else
#ifdef USE_RANGE_AND_SLICE
            ublas::matrix_vector_range<M> mvr1 (m1, ublas::range (0, N), ublas::range (0, N)),
                                          mvr2 (m1, ublas::range (0, N), ublas::range (0, N));
            (*this) (mvr1, mvr2, m1);

            ublas::matrix_vector_slice<M> mvs1 (m1, ublas::slice (0, 1, N), ublas::slice (0, 1, N)),
                                          mvs2 (m1, ublas::slice (0, 1, N), ublas::slice (0, 1, N));
            (*this) (mvs1, mvs2, m1);
#endif
#endif
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
    void operator () (int) const {
#ifdef USE_ADAPTOR
        try {
            V v1 (N), v2 (N);
            M m1 (N, N);
            ublas::symmetric_adaptor<M> tam1 (m1);
            (*this) (v1, v2, tam1);

            ublas::matrix_row<ublas::symmetric_adaptor<M> > mr1 (tam1, N - 1), mr2 (tam1, N - 1);
            (*this) (mr1, mr2, tam1);

            ublas::matrix_column<ublas::symmetric_adaptor<M> > mc1 (tam1, 0), mc2 (tam1, 0);
            (*this) (mc1, mc2, tam1);

#ifdef BOOST_UBLAS_ENABLE_INDEX_SET_ALL
#ifdef USE_RANGE_AND_SLICE
            ublas::matrix_vector_range<ublas::symmetric_adaptor<M> > mvr1 (tam1, ublas::range<> (0, N), ublas::range<> (0, N)),
                                                                     mvr2 (tam1, ublas::range<> (0, N), ublas::range<> (0, N));
            (*this) (mvr1, mvr2, tam1);

            ublas::matrix_vector_slice<ublas::symmetric_adaptor<M> > mvs1 (tam1, ublas::slice<> (0, 1, N), ublas::slice<> (0, 1, N)),
                                                                     mvs2 (tam1, ublas::slice<> (0, 1, N), ublas::slice<> (0, 1, N));
            (*this) (mvs1, mvs2, tam1);
#endif
#else
#ifdef USE_RANGE_AND_SLICE
            ublas::matrix_vector_range<ublas::symmetric_adaptor<M> > mvr1 (tam1, ublas::range (0, N), ublas::range (0, N)),
                                                                     mvr2 (tam1, ublas::range (0, N), ublas::range (0, N));
            (*this) (mvr1, mvr2, tam1);

            ublas::matrix_vector_slice<ublas::symmetric_adaptor<M> > mvs1 (tam1, ublas::slice (0, 1, N), ublas::slice (0, 1, N)),
                                                                     mvs2 (tam1, ublas::slice (0, 1, N), ublas::slice (0, 1, N));
            (*this) (mvs1, mvs2, tam1);
#endif
#endif
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
#endif
    }
};

// Test matrix & vector
void test_matrix_vector () {
    std::cout << "test_matrix_vector" << std::endl;

#ifdef USE_BOUNDED_ARRAY
    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<float, ublas::bounded_array<float, 3> >,
                          ublas::symmetric_matrix<float, ublas::lower, ublas::row_major, ublas::bounded_array<float, 3 * 3> >, 3> () ();
    test_my_matrix_vector<ublas::vector<float, ublas::bounded_array<float, 3> >,
                          ublas::symmetric_matrix<float, ublas::lower, ublas::row_major, ublas::bounded_array<float, 3 * 3> >, 3> () (0);

    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<double, ublas::bounded_array<double, 3> >,
                          ublas::symmetric_matrix<double, ublas::lower, ublas::row_major, ublas::bounded_array<double, 3 * 3> >, 3> () ();
    test_my_matrix_vector<ublas::vector<double, ublas::bounded_array<double, 3> >,
                          ublas::symmetric_matrix<double, ublas::lower, ublas::row_major, ublas::bounded_array<double, 3 * 3> >, 3> () (0);

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<std::complex<float>, ublas::bounded_array<std::complex<float>, 3> >,
                          ublas::symmetric_matrix<std::complex<float>, ublas::lower, ublas::row_major, ublas::bounded_array<std::complex<float>, 3 * 3> >, 3> () ();
    test_my_matrix_vector<ublas::vector<std::complex<float>, ublas::bounded_array<std::complex<float>, 3> >,
                          ublas::symmetric_matrix<std::complex<float>, ublas::lower, ublas::row_major, ublas::bounded_array<std::complex<float>, 3 * 3> >, 3> () (0);

    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<std::complex<double>, ublas::bounded_array<std::complex<double>, 3> >,
                          ublas::symmetric_matrix<std::complex<double>, ublas::lower, ublas::row_major, ublas::bounded_array<std::complex<double>, 3 * 3> >, 3> () ();
    test_my_matrix_vector<ublas::vector<std::complex<double>, ublas::bounded_array<std::complex<double>, 3> >,
                          ublas::symmetric_matrix<std::complex<double>, ublas::lower, ublas::row_major, ublas::bounded_array<std::complex<double>, 3 * 3> >, 3> () (0);
#endif

#ifdef USE_UNBOUNDED_ARRAY
    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<float, ublas::unbounded_array<float> >,
                          ublas::symmetric_matrix<float, ublas::lower, ublas::row_major, ublas::unbounded_array<float> >, 3> () ();
    test_my_matrix_vector<ublas::vector<float, ublas::unbounded_array<float> >,
                          ublas::symmetric_matrix<float, ublas::lower, ublas::row_major, ublas::unbounded_array<float> >, 3> () (0);

    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<double, ublas::unbounded_array<double> >,
                          ublas::symmetric_matrix<double, ublas::lower, ublas::row_major, ublas::unbounded_array<double> >, 3> () ();
    test_my_matrix_vector<ublas::vector<double, ublas::unbounded_array<double> >,
                          ublas::symmetric_matrix<double, ublas::lower, ublas::row_major, ublas::unbounded_array<double> >, 3> () (0);

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<std::complex<float>, ublas::unbounded_array<std::complex<float> > >,
                          ublas::symmetric_matrix<std::complex<float>, ublas::lower, ublas::row_major, ublas::unbounded_array<std::complex<float> > >, 3> () ();
    test_my_matrix_vector<ublas::vector<std::complex<float>, ublas::unbounded_array<std::complex<float> > >,
                          ublas::symmetric_matrix<std::complex<float>, ublas::lower, ublas::row_major, ublas::unbounded_array<std::complex<float> > >, 3> () (0);

    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_matrix_vector<ublas::vector<std::complex<double>, ublas::unbounded_array<std::complex<double> > >,
                          ublas::symmetric_matrix<std::complex<double>, ublas::lower, ublas::row_major, ublas::unbounded_array<std::complex<double> > >, 3> () ();
    test_my_matrix_vector<ublas::vector<std::complex<double>, ublas::unbounded_array<std::complex<double> > >,
                          ublas::symmetric_matrix<std::complex<double>, ublas::lower, ublas::row_major, ublas::unbounded_array<std::complex<double> > >, 3> () (0);
#endif

#ifdef USE_STD_VECTOR
    std::cout << "float, std::vector" << std::endl;
    test_my_matrix_vector<ublas::vector<float, std::vector<float> >,
                          ublas::symmetric_matrix<float, ublas::lower, ublas::row_major, std::vector<float> >, 3> () ();
    test_my_matrix_vector<ublas::vector<float, std::vector<float> >,
                          ublas::symmetric_matrix<float, ublas::lower, ublas::row_major, std::vector<float> >, 3> () (0);

    std::cout << "double, std::vector" << std::endl;
    test_my_matrix_vector<ublas::vector<double, std::vector<double> >,
                          ublas::symmetric_matrix<double, ublas::lower, ublas::row_major, std::vector<double> >, 3> () ();
    test_my_matrix_vector<ublas::vector<double, std::vector<double> >,
                          ublas::symmetric_matrix<double, ublas::lower, ublas::row_major, std::vector<double> >, 3> () (0);

    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_matrix_vector<ublas::vector<std::complex<float>, std::vector<std::complex<float> > >,
                          ublas::symmetric_matrix<std::complex<float>, ublas::lower, ublas::row_major, std::vector<std::complex<float> > >, 3> () ();
    test_my_matrix_vector<ublas::vector<std::complex<float>, std::vector<std::complex<float> > >,
                          ublas::symmetric_matrix<std::complex<float>, ublas::lower, ublas::row_major, std::vector<std::complex<float> > >, 3> () (0);

    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_matrix_vector<ublas::vector<std::complex<double>, std::vector<std::complex<double> > >,
                          ublas::symmetric_matrix<std::complex<double>, ublas::lower, ublas::row_major, std::vector<std::complex<double> > >, 3> () ();
    test_my_matrix_vector<ublas::vector<std::complex<double>, std::vector<std::complex<double> > >,
                          ublas::symmetric_matrix<std::complex<double>, ublas::lower, ublas::row_major, std::vector<std::complex<double> > >, 3> () (0);
#endif
}

