//  
//  Copyright (c) 2000-2001
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
//	GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_ITERATOR_H
#define NUMERICS_ITERATOR_H

#include <iterator>

#include "config.h"

#ifdef USE_GCC
// FIXME: could we use BOOST_NO_STD_ITERATOR and <boost/iterator.h> here?
#if __GNUC__ <= 2

namespace std {

    template <class I, class T, class D = std::ptrdiff_t, class P = T *, class R = T &>
    struct iterator {
        typedef I derived_iterator_category;
        typedef T derived_value_type;
        typedef D derived_difference_type;
        typedef P derived_pointer;
        typedef R derived_reference;
    };

}

#endif
#endif

namespace numerics {

    template<class C>
    class container_const_reference {
    public:
        typedef C container_type;

        NUMERICS_INLINE
        container_const_reference ():
            c_ (nil_) {}
        NUMERICS_INLINE
        container_const_reference (const container_type &c):
            c_ (c) {}

        NUMERICS_INLINE
        const container_type &operator () () const {
            return c_; 
        }

    private:
        const container_type &c_;
        static container_type nil_;

        container_const_reference operator = (container_const_reference &) {
            return *this;
        }
    };

    template<class C>
    container_const_reference<C>::container_type container_const_reference<C>::nil_;

    template<class C>
    class container_reference {
    public:
        typedef C container_type;

        NUMERICS_INLINE
        container_reference ():
            c_ (nil_) {}
        NUMERICS_INLINE
        container_reference (container_type &c):
            c_ (c) {}

        NUMERICS_INLINE
        container_type &operator () () const {
            return c_; 
        }
        NUMERICS_INLINE
        container_type &operator () () {
            return c_; 
        }

    private:
        container_type &c_;
        static container_type nil_;

        container_reference operator = (container_reference &) {
            return *this;
        }
    };

    template<class C>
    container_reference<C>::container_type container_reference<C>::nil_;

    template<class I, class T>
    struct forward_iterator_base: 
        public std::iterator<std::forward_iterator_tag, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference_type;
        typedef T &derived_reference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#endif

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

    template<class I, class T>
    struct bidirectional_iterator_base: 
        public std::iterator<std::bidirectional_iterator_tag, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference_type;
        typedef T &derived_reference_type;

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator -- (int) {
            derived_iterator_type &d (*static_cast<const derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator -- (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#endif

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

    template<class I, class T, class D = std::ptrdiff_t>
    struct random_access_iterator_base:
        public std::iterator<std::random_access_iterator_tag, T> {
        typedef I derived_iterator_type;
        typedef T derived_value_type;
        typedef const T &derived_const_reference_type;
        typedef T &derived_reference_type;
        typedef D derived_difference_type;
#ifdef USE_MSVC
        typedef D difference_type;
#endif

        // Arithmetic
        NUMERICS_INLINE
        derived_iterator_type operator ++ (int) {
            derived_iterator_type &d (*static_cast<derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator ++ (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            ++ d;
            return tmp;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator -- (int) {
            derived_iterator_type &d (*static_cast<derived_iterator_type *> (this));
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator -- (derived_iterator_type &d, int) {
            derived_iterator_type tmp (d);
            -- d;
            return tmp;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator + (derived_difference_type n) const {
            derived_iterator_type d (*static_cast<const derived_iterator_type *> (this));
            return d += n;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator + (const derived_iterator_type &d, derived_difference_type n) {
            derived_iterator_type tmp (d);
            return tmp += n;
        }
#endif
        NUMERICS_INLINE
        derived_iterator_type operator - (derived_difference_type n) const {
            derived_iterator_type d (*static_cast<const derived_iterator_type *> (this));
            return d -= n;
        }
#ifndef USE_GCC
        NUMERICS_INLINE
        friend derived_iterator_type operator - (const derived_iterator_type &d, derived_difference_type n) {
            derived_iterator_type tmp (d);
            return tmp -= n;
        }
#endif

        // Comparison
        NUMERICS_INLINE
        bool operator != (const derived_iterator_type &it) const {
            const derived_iterator_type *d = static_cast<const derived_iterator_type *> (this);
            return ! (*d == it);
        }
    };

#if defined (USE_GCC)
    template<class I>
    NUMERICS_INLINE
    I operator ++ (I &d, int) {
        I tmp (d);
        ++ d;
        return tmp;
    }
    template<class I>
    NUMERICS_INLINE
    I operator -- (I &d, int) {
        I tmp (d);
        -- d;
        return tmp;
    }
    template<class I>
    NUMERICS_INLINE
    I operator - (const I &d, std::ptrdiff_t n) {
        I tmp (d);
        return tmp -= n;
    }
#endif

    template<class I1, class I2>
    struct restrict_traits {
        typedef I1 iterator_category;
    };

    template<>
    struct restrict_traits<std::bidirectional_iterator_tag, std::random_access_iterator_tag> {
        typedef std::bidirectional_iterator_tag iterator_category;
    };
    template<>
    struct restrict_traits<std::random_access_iterator_tag, std::bidirectional_iterator_tag> {
        typedef std::bidirectional_iterator_tag iterator_category;
    };

}

#endif


