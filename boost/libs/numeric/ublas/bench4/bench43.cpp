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
#include <string>

#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <boost/timer.hpp>

#include "bench4.hpp"

template<class T, int N>
struct bench_c_matrix_prod {
    typedef T value_type;

    void operator () (int runs) const {
        try {
            static typename c_matrix_traits<T, N, N>::type m1, m2, m3;
            initialize_c_matrix<T, N, N> () (m1);
            initialize_c_matrix<T, N, N> () (m2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                for (int j = 0; j < N; ++ j) {
                    for (int k = 0; k < N; ++ k) {
                        m3 [j] [k] = 0;
                        for (int l = 0; l < N; ++ l) {
                            m3 [j] [k] += m1 [j] [l] * m2 [l] [k];
                        }
                    }
                }
//                sink_c_matrix<T, N, N> () (m3);
            }
            footer<value_type> () (N * N * N, N * N * (N - 1), runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class M, int N>
struct bench_my_matrix_prod {
    typedef typename M::value_type value_type;

    void operator () (int runs, safe_tag) const {
        try {
            static M m1 (N, N), m2 (N, N), m3 (N, N);
#ifdef BOOST_UBLAS_ENABLE_INDEX_SET_ALL
            ublas::matrix_range<M> mr1 (m1, ublas::range<> (0, N), ublas::range<> (0, N)),
                                   mr2 (m2, ublas::range<> (0, N), ublas::range<> (0, N)),
                                   mr3 (m3, ublas::range<> (0, N), ublas::range<> (0, N));
#else
            ublas::matrix_range<M> mr1 (m1, ublas::range (0, N), ublas::range (0, N)),
                                   mr2 (m2, ublas::range (0, N), ublas::range (0, N)),
                                   mr3 (m3, ublas::range (0, N), ublas::range (0, N));
#endif
            initialize_matrix (mr1);
            initialize_matrix (mr2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                mr3 = ublas::prod (mr1, mr2);
//                sink_matrix (mr3);
            }
            footer<value_type> () (N * N * N, N * N * (N - 1), runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
    void operator () (int runs, fast_tag) const {
        try {
            static M m1 (N, N), m2 (N, N), m3 (N, N);
#ifdef BOOST_UBLAS_ENABLE_INDEX_SET_ALL
            ublas::matrix_range<M> mr1 (m1, ublas::range<> (0, N), ublas::range<> (0, N)),
                                   mr2 (m2, ublas::range<> (0, N), ublas::range<> (0, N)),
                                   mr3 (m3, ublas::range<> (0, N), ublas::range<> (0, N));
#else
            ublas::matrix_range<M> mr1 (m1, ublas::range (0, N), ublas::range (0, N)),
                                   mr2 (m2, ublas::range (0, N), ublas::range (0, N)),
                                   mr3 (m3, ublas::range (0, N), ublas::range (0, N));
#endif
            initialize_matrix (mr1);
            initialize_matrix (mr2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                mr3.assign (ublas::prod (mr1, mr2));
//                sink_matrix (mr3);
            }
            footer<value_type> () (N * N * N, N * N * (N - 1), runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class M, int N>
struct bench_cpp_matrix_prod {
    typedef typename M::value_type value_type;

    void operator () (int runs) const {
        try {
            static M m1 (N * N), m2 (N * N), m3 (N * N);
            initialize_vector (m1);
            initialize_vector (m2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                for (int j = 0; j < N; ++ j) {
                    std::valarray<value_type> row (m1 [std::slice (N * j, N, 1)]);
                    for (int k = 0; k < N; ++ k) {
                        std::valarray<value_type> column (m2 [std::slice (k, N, N)]);
                        m3 [N * j + k] = (row * column).sum ();
                    }
                }
//                sink_vector (m3);
            }
            footer<value_type> () (N * N * N, N * N * (N - 1), runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

// Benchmark O (n ^ 3)
template<class T, int N>
void bench_3<T, N>::operator () (int runs) {
    header ("bench_3");

    header ("prod (matrix, matrix)");

    header ("C array");
    bench_c_matrix_prod<T, N> () (runs);

#ifdef USE_C_ARRAY
    header ("c_matrix safe");
    bench_my_matrix_prod<ublas::c_matrix<T, N, N>, N> () (runs, safe_tag ());

    header ("c_matrix fast");
    bench_my_matrix_prod<ublas::c_matrix<T, N, N>, N> () (runs, fast_tag ());
#endif

#ifdef USE_BOUNDED_ARRAY
    header ("matrix<bounded_array> safe");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, ublas::bounded_array<T, N * N> >, N> () (runs, safe_tag ());

    header ("matrix<bounded_array> fast");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, ublas::bounded_array<T, N * N> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_UNBOUNDED_ARRAY
    header ("matrix<unbounded_array> safe");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, ublas::unbounded_array<T> >, N> () (runs, safe_tag ());

    header ("matrix<unbounded_array> fast");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, ublas::unbounded_array<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("matrix<std::valarray> safe");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, std::valarray<T> >, N> () (runs, safe_tag ());

    header ("matrix<std::valarray> fast");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, std::valarray<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VECTOR
    header ("matrix<std::vector> safe");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, std::vector<T> >, N> () (runs, safe_tag ());

    header ("matrix<std::vector> fast");
    bench_my_matrix_prod<ublas::matrix<T, ublas::row_major, std::vector<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
    bench_cpp_matrix_prod<std::valarray<T>, N> () (runs);
#endif
}

#ifdef USE_FLOAT
template struct bench_3<boost::numeric::interval<float>, 3>;
template struct bench_3<boost::numeric::interval<float>, 10>;
template struct bench_3<boost::numeric::interval<float>, 30>;
template struct bench_3<boost::numeric::interval<float>, 100>;
#endif

#ifdef USE_DOUBLE
template struct bench_3<boost::numeric::interval<double>, 3>;
template struct bench_3<boost::numeric::interval<double>, 10>;
template struct bench_3<boost::numeric::interval<double>, 30>;
template struct bench_3<boost::numeric::interval<double>, 100>;
#endif

#ifdef USE_BOOST_COMPLEX
#ifdef USE_FLOAT
template struct bench_3<boost::complex<boost::numeric::interval<float> >, 3>;
template struct bench_3<boost::complex<boost::numeric::interval<float> >, 10>;
template struct bench_3<boost::complex<boost::numeric::interval<float> >, 30>;
template struct bench_3<boost::complex<boost::numeric::interval<float> >, 100>;
#endif

#ifdef USE_DOUBLE
template struct bench_3<boost::complex<boost::numeric::interval<double> >, 3>;
template struct bench_3<boost::complex<boost::numeric::interval<double> >, 10>;
template struct bench_3<boost::complex<boost::numeric::interval<double> >, 30>;
template struct bench_3<boost::complex<boost::numeric::interval<double> >, 100>;
#endif
#endif


