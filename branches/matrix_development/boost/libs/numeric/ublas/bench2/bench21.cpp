#ifdef BOOST_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>
#include <string>

#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/vector_sp.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/matrix_sp.h>

#include <boost/timer.hpp>

#include "bench2.h"

template<class T, int N>
struct bench_c_inner_prod {
    typedef T value_type;

    void operator () (int runs) const {
        try {
            static typename c_vector_traits<T, N>::type v1, v2;
            initialize_c_vector<T, N> () (v1);
            initialize_c_vector<T, N> () (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                static value_type s (0);
                for (int j = 0; j < N; ++ j) {
                    s += v1 [j] * v2 [j];
                }
//                sink_scalar (s);
            }
            footer<value_type> () (N, N - 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_my_inner_prod {
    typedef typename V::value_type value_type;

    void operator () (int runs) const {
        try {
            static V v1 (N, N), v2 (N, N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                static value_type s (0);
                s = numerics::inner_prod (v1, v2);
//                sink_scalar (s);
            }
            footer<value_type> () (N, N - 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_cpp_inner_prod {
    typedef typename V::value_type value_type;

    void operator () (int runs) const {
        try {
            static V v1 (N), v2 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                static value_type s (0);
                s = (v1 * v2).sum ();
//                sink_scalar (s);
            }
            footer<value_type> () (N, N - 1, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

template<class T, int N>
struct bench_c_vector_add {
    typedef T value_type;

    void operator () (int runs) const {
        try {
            static typename c_vector_traits<T, N>::type v1, v2, v3;
            initialize_c_vector<T, N> () (v1);
            initialize_c_vector<T, N> () (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                for (int j = 0; j < N; ++ j) {
                    v3 [j] = - (v1 [j] + v2 [j]);
                }
//                sink_c_vector<T, N> () (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_my_vector_add {
    typedef typename V::value_type value_type;

    void operator () (int runs, safe_tag) const {
        try {
            static V v1 (N, N), v2 (N, N), v3 (N, N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                v3 = - (v1 + v2);
//                sink_vector (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
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
            static V v1 (N, N), v2 (N, N), v3 (N, N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                v3.assign (- (v1 + v2));
//                sink_vector (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};
template<class V, int N>
struct bench_cpp_vector_add {
    typedef typename V::value_type value_type;

    void operator () (int runs) const {
        try {
            static V v1 (N), v2 (N), v3 (N);
            initialize_vector (v1);
            initialize_vector (v2);
            boost::timer t;
            for (int i = 0; i < runs; ++ i) {
                v3 = - (v1 + v2);
//                sink_vector (v3);
            }
            footer<value_type> () (0, 2 * N, runs, t.elapsed ());
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

// Benchmark O (n)
template<class T, int N>
void bench_1<T, N>::operator () (int runs) {
    header ("bench_1");

    header ("inner_prod");

    header ("C array");
    bench_c_inner_prod<T, N> () (runs);

#ifdef USE_MAP_ARRAY
    header ("sparse_vector<map_array>");
    bench_my_inner_prod<numerics::sparse_vector<T, numerics::map_array<std::size_t, T> >, N> () (runs);
#endif

#ifdef USE_STD_MAP
    header ("sparse_vector<std::map>");
    bench_my_inner_prod<numerics::sparse_vector<T, std::map<std::size_t, T> >, N> () (runs);
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
    bench_cpp_inner_prod<std::valarray<T>, N> () (runs);
#endif

    header ("sparse_vector + sparse_vector");

    header ("C array");
    bench_c_vector_add<T, N> () (runs);

#ifdef USE_MAP_ARRAY
    header ("sparse_vector<map_array> safe");
    bench_my_vector_add<numerics::sparse_vector<T, numerics::map_array<std::size_t, T> >, N> () (runs, safe_tag ());

    header ("sparse_vector<map_array> fast");
    bench_my_vector_add<numerics::sparse_vector<T, numerics::map_array<std::size_t, T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_MAP
    header ("sparse_vector<std::map> safe");
    bench_my_vector_add<numerics::sparse_vector<T, std::map<std::size_t, T> >, N> () (runs, safe_tag ());

    header ("sparse_vector<std::map> fast");
    bench_my_vector_add<numerics::sparse_vector<T, std::map<std::size_t, T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
    bench_cpp_vector_add<std::valarray<T>, N> () (runs);
#endif
}

template struct bench_1<float, 3>;
template struct bench_1<float, 10>;
template struct bench_1<float, 30>;
template struct bench_1<float, 100>;

template struct bench_1<double, 3>;
template struct bench_1<double, 10>;
template struct bench_1<double, 30>;
template struct bench_1<double, 100>;

#ifdef USE_STD_COMPLEX

template struct bench_1<std::complex<float>, 3>;
template struct bench_1<std::complex<float>, 10>;
template struct bench_1<std::complex<float>, 30>;
template struct bench_1<std::complex<float>, 100>;

template struct bench_1<std::complex<double>, 3>;
template struct bench_1<std::complex<double>, 10>;
template struct bench_1<std::complex<double>, 30>;
template struct bench_1<std::complex<double>, 100>;

#endif


