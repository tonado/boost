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

#ifndef TEST5_H
#define TEST5_H

namespace ublas = boost::numeric::ublas;

template<class V>
void initialize_vector (V &v) {
    int size = v.size ();
    for (int i = 0; i < size; ++ i)
        v [i] = i + 1.f;
}

template<class M>
void initialize_matrix (M &m, ublas::lower_tag) {
    int size1 = m.size1 ();
//    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) {
        int j = 0;
        for (; j <= i; ++ j)
            m (i, j) = i * size1 + j + 1.f;
//        for (; j < size2; ++ j)
//            m (i, j) = 0.f;
    }
}
template<class M>
void initialize_matrix (M &m, ublas::upper_tag) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) {
        int j = 0;
//        for (; j < i; ++ j)
//            m (i, j) = 0.f;
        for (; j < size2; ++ j)
            m (i, j) = i * size1 + j + 1.f;
    }
}
template<class M>
void initialize_matrix (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i)
        for (int j = 0; j < size2; ++ j) 
            m (i, j) = i * size1 + j + 1;
}


void test_vector ();
void test_matrix_vector ();
void test_matrix ();


// Disable some tests for truly broken compilers
	// MSVC Version 6.0 & 7.0 have problems compiling with std::complex
#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1300)
#undef USE_STD_COMPLEX
#endif

	// Intel for Windows fails to link when a std::complex is returned!
#if defined(BOOST_INTEL_CXX_VERSION) && (BOOST_INTEL_CXX_VERSION <= 800) && defined(__ICL)
#undef USE_STD_COMPLEX
#endif

#endif
