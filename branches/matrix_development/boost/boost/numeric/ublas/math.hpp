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

#ifndef BOOST_UBLAS_MATH_H
#define BOOST_UBLAS_MATH_H

#ifdef BOOST_UBLAS_DEPRECATED

#include <algorithm>

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/traits.hpp>

namespace boost { namespace numeric { namespace ublas {

    // GNU does not honour std, so we have to use namespace detail?
    namespace detail {

        template<class T>
        BOOST_UBLAS_INLINE
        typename type_traits<T>::real_type real (const T &t) {
            return type_traits<T>::real (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        typename type_traits<T>::real_type imag (const T &t) {
            return type_traits<T>::imag (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        T conj (const T &t) {
            return type_traits<T>::conj (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        typename type_traits<T>::real_type abs (const T &t) {
            return type_traits<T>::abs (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        T sqrt (const T &t) {
            return type_traits<T>::sqrt (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        typename type_traits<T>::real_type norm_1 (const T &t) {
            return type_traits<T>::norm_1 (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        typename type_traits<T>::real_type norm_2 (const T &t) {
            return type_traits<T>::norm_2 (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        typename type_traits<T>::real_type norm_inf (const T &t) {
            return type_traits<T>::norm_inf (t);
        }

        template<class T>
        BOOST_UBLAS_INLINE
        bool equals (const T &t1, const T &t2) {
            return type_traits<T>::equals (t1, t2);
        }

    }

}}}

#endif

#endif


