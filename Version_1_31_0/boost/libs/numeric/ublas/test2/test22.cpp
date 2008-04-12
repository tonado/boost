//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

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
#include <boost/numeric/ublas/io.hpp>

#include <boost/numeric/ublas/blas.hpp>

#include "test2.hpp"

template<class V, class M, int N>
void test_blas_2<V, M, N>::operator () () {
    try {
        V v1 (N), v2 (N);
        M m (N, N);

        // _t_mv
        initialize_vector (v1);
        initialize_matrix (m);
        ublas::blas_2::tmv (v1, m);
        std::cout << "tmv (v1, m) = " << v1 << std::endl;
        initialize_vector (v1);
        initialize_matrix (m);
        ublas::blas_2::tmv (v1, ublas::trans (m));
        std::cout << "tmv (v1, trans (m)) = " << v1 << std::endl;
#ifdef USE_STD_COMPLEX
        initialize_vector (v1);
        initialize_matrix (m);
        ublas::blas_2::tmv (v1, ublas::herm (m));
        std::cout << "tmv (v1, herm (m)) = " << v1 << std::endl;
#endif

        // _t_sv
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m, ublas::lower_tag ());
        ublas::blas_2::tsv (v1, m, ublas::lower_tag ());
        std::cout << "tsv (v1, m) = " << v1 << " " << ublas::prod (m, v1) - v2 << std::endl;
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m, ublas::upper_tag ());
        ublas::blas_2::tsv (v1, ublas::trans (m), ublas::lower_tag ());
        std::cout << "tsv (v1, trans (m)) = " << v1 << " " << ublas::prod (ublas::trans (m), v1) - v2 << std::endl;
#ifdef USE_STD_COMPLEX
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m, ublas::upper_tag ());
        ublas::blas_2::tsv (v1, ublas::herm (m), ublas::lower_tag ());
        std::cout << "tsv (v1, herm (m)) = " << v1 << " " << ublas::prod (ublas::herm (m), v1) - v2 << std::endl;
#endif
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m, ublas::upper_tag ());
        ublas::blas_2::tsv (v1, m, ublas::upper_tag ());
        std::cout << "tsv (v1, m) = " << v1 << " " << ublas::prod (m, v1) - v2 << std::endl;
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m, ublas::lower_tag ());
        ublas::blas_2::tsv (v1, ublas::trans (m), ublas::upper_tag ());
        std::cout << "tsv (v1, trans (m)) = " << v1 << " " << ublas::prod (ublas::trans (m), v1) - v2 << std::endl;
#ifdef USE_STD_COMPLEX
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m, ublas::lower_tag ());
        ublas::blas_2::tsv (v1, ublas::herm (m), ublas::upper_tag ());
        std::cout << "tsv (v1, herm (m)) = " << v1 << " " << ublas::prod (ublas::herm (m), v1) - v2 << std::endl;
#endif

        // _g_mv
        // _s_mv
        // _h_mv
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m);
        ublas::blas_2::gmv (v1, value_type (1), value_type (1), m, v2);
        std::cout << "gmv (v1, 1, 1, m, v2) = " << v1 << std::endl;
        ublas::blas_2::gmv (v1, value_type (1), value_type (1), ublas::trans (m), v2);
        std::cout << "gmv (v1, 1, 1, trans (m), v2) = " << v1 << std::endl;
#ifdef USE_STD_COMPLEX
        ublas::blas_2::gmv (v1, value_type (1), value_type (1), ublas::herm (m), v2);
        std::cout << "gmv (v1, 1, 1, herm (m), v2) = " << v1 << std::endl;
#endif

        // _g_r
        // _g_ru
        // _g_rc
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m);
        ublas::blas_2::gr (m, value_type (1), v1, v2);
        std::cout << "gr (m, 1, v1, v2) = " << m << std::endl;
        ublas::blas_2::gr (m, value_type (1), v1, ublas::conj (v2));
        std::cout << "gr (m, 1, v1, conj (v2)) = " << m << std::endl;

        // _s_r
        initialize_vector (v1);
        initialize_matrix (m);
        ublas::blas_2::sr (m, value_type (1), v1);
        std::cout << "sr (m, 1, v1) = " << m << std::endl;

#ifdef USE_STD_COMPLEX
        // _h_r
        initialize_vector (v1);
        initialize_matrix (m);
        ublas::blas_2::hr (m, value_type (1), v1);
        std::cout << "hr (m, 1, v1) = " << m << std::endl;
#endif

        // _s_r2
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m);
        ublas::blas_2::sr2 (m, value_type (1), v1, v2);
        std::cout << "sr2 (m, 1, v1, v2) = " << m << std::endl;

#ifdef USE_STD_COMPLEX
        // _h_r2
        initialize_vector (v1);
        initialize_vector (v2);
        initialize_matrix (m);
        ublas::blas_2::hr2 (m, value_type (1), v1, v2);
        std::cout << "hr2 (m, 1, v1, v2) = " << m << std::endl;
#endif
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

#ifdef USE_FLOAT
template struct test_blas_2<ublas::vector<float>, ublas::matrix<float>, 3>;
#endif

#ifdef USE_DOUBLE
template struct test_blas_2<ublas::vector<double>, ublas::matrix<double>, 3>;
#endif

#ifdef USE_STD_COMPLEX
#ifdef USE_FLOAT
template struct test_blas_2<ublas::vector<std::complex<float> >, ublas::matrix<std::complex<float> >, 3>;
#endif

#ifdef USE_DOUBLE
template struct test_blas_2<ublas::vector<std::complex<double> >, ublas::matrix<std::complex<double> >, 3>;
#endif
#endif

