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

#ifndef BOOST_UBLAS_VECTOR_ASSIGN_H
#define BOOST_UBLAS_VECTOR_ASSIGN_H

#include <boost/numeric/ublas/config.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>

// Iterators based on ideas of Jeremy Siek

namespace boost { namespace numeric { namespace ublas {

    template<class E1, class E2>
    BOOST_UBLAS_INLINE
    bool equals (const vector_expression<E1> &e1, const vector_expression<E2> &e2) {
        typedef BOOST_UBLAS_TYPENAME type_traits<BOOST_UBLAS_TYPENAME promote_traits<BOOST_UBLAS_TYPENAME E1::value_type,
                                                                                     BOOST_UBLAS_TYPENAME E2::value_type>::promote_type>::real_type real_type;
#ifndef __GNUC__
        return norm_inf (e1 - e2) < BOOST_UBLAS_TYPE_CHECK_EPSILON *
               std::max<real_type> (std::max<real_type> (norm_inf (e1),
                                                         norm_inf (e2)),
                                    BOOST_UBLAS_TYPE_CHECK_MIN);
#else
        // GCC 3.1, oops?!
        return norm_inf (e1 - e2) < BOOST_UBLAS_TYPE_CHECK_EPSILON *
               (std::max) (real_type ((std::max) (real_type (norm_inf (e1)), real_type (norm_inf (e2)))),
                         real_type (BOOST_UBLAS_TYPE_CHECK_MIN));
#endif
    }

    // Make sparse proxies conformant
    template<class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void make_conformant (V &v, const vector_expression<E> &e) {
        BOOST_UBLAS_CHECK (v.size () == e ().size (), bad_size ());
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
        // FIXME unbounded_array with push_back maybe better
        std::vector<size_type> index;
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        if (it != it_end && ite != ite_end) {
            size_type it_index = it.index (), ite_index = ite.index ();
            while (true) {
                difference_type compare = it_index - ite_index;
                if (compare == 0) {
                    ++ it, ++ ite;
                    if (it != it_end && ite != ite_end) {
                        it_index = it.index ();
                        ite_index = ite.index ();
                    } else
                        break;
                } else if (compare < 0) {
                    increment (it, it_end, - compare);
                    if (it != it_end)
                        it_index = it.index ();
                    else
                        break;
                } else if (compare > 0) {
                    if (*ite != value_type (0))
                        index.push_back (ite.index ());
                    ++ ite;
                    if (ite != ite_end)
                        ite_index = ite.index ();
                    else
                        break;
                }
            }
        }

        while (ite != ite_end) {
            if (*ite != value_type (0))
                index.push_back (ite.index ());
            ++ ite;
        }
        for (size_type k = 0; k < index.size (); ++ k)
            v (index [k]) = value_type (0);
    }

    // Iterating case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void iterating_vector_assign_scalar (F, V &v, const T &t) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (v.size ());
        typename V::iterator it (v.begin ());
        BOOST_UBLAS_CHECK (v.end () - it == size, bad_size ());
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
        while (-- size >= 0)
            functor_type () (*it, t), ++ it;
#else
        DD (size, 4, r, (functor_type () (*it, t), ++ it));
#endif
    }
    // Indexing case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void indexing_vector_assign_scalar (F, V &v, const T &t) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (v.size ());
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
        for (difference_type i = 0; i < size; ++ i)
            functor_type () (v (i), t);
#else
        difference_type i (0);
        DD (size, 4, r, (functor_type () (v (i), t), ++ i));
#endif
    }

    // Dense (proxy) case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign_scalar (F, V &v, const T &t, dense_proxy_tag) {
        typedef F functor_type;
#ifdef BOOST_UBLAS_USE_INDEXING
        indexing_vector_assign_scalar (functor_type (), v, t);
#elif BOOST_UBLAS_USE_ITERATING
        iterating_vector_assign_scalar (functor_type (), v, t);
#else
        typedef typename V::difference_type difference_type;
        difference_type size (v.size ());
        if (size >= BOOST_UBLAS_ITERATOR_THRESHOLD)
            iterating_vector_assign_scalar (functor_type (), v, t);
        else
            indexing_vector_assign_scalar (functor_type (), v, t);
#endif
    }
    // Packed (proxy) case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign_scalar (F, V &v, const T &t, packed_proxy_tag) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        typename V::iterator it (v.begin ());
        difference_type size (v.end () - it);
        while (-- size >= 0)
            functor_type () (*it, t), ++ it;
    }
    // Sparse (proxy) case
    template<class F, class V, class T>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign_scalar (F, V &v, const T &t, sparse_proxy_tag) {
        typedef F functor_type;
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        while (it != it_end)
            functor_type () (*it, t), ++ it;
    }

    // Dispatcher
    template<class F, class V, class T>
    BOOST_UBLAS_INLINE
    void vector_assign_scalar (F, V &v, const T &t) {
        typedef F functor_type;
        typedef typename V::storage_category storage_category;
        vector_assign_scalar (functor_type (), v, t, storage_category ());
    }

    template<class LS, class A, class RI>
    struct vector_assign_traits {
        typedef LS storage_category;
    };

    template<>
    struct vector_assign_traits<dense_tag, assign_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef packed_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_assign_traits<dense_proxy_tag, assign_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef packed_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<dense_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_assign_traits<packed_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_tag storage_category;
    };
    template<>
    struct vector_assign_traits<packed_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_assign_traits<packed_proxy_tag, assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<packed_proxy_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, dense_random_access_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, packed_random_access_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };
    template<>
    struct vector_assign_traits<sparse_tag, computed_assign_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    // Iterating case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void iterating_vector_assign (F, V &v, const vector_expression<E> &e) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (BOOST_UBLAS_SAME (v.size (), e ().size ()));
        typename V::iterator it (v.begin ());
        BOOST_UBLAS_CHECK (v.end () - it == size, bad_size ());
        typename E::const_iterator ite (e ().begin ());
        BOOST_UBLAS_CHECK (e ().end () - ite == size, bad_size ());
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
        while (-- size >= 0)
            functor_type () (*it, *ite), ++ it, ++ ite;
#else
        DD (size, 2, r, (functor_type () (*it, *ite), ++ it, ++ ite));
#endif
    }
    // Indexing case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void indexing_vector_assign (F, V &v, const vector_expression<E> &e) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (BOOST_UBLAS_SAME (v.size (), e ().size ()));
#ifndef BOOST_UBLAS_USE_DUFF_DEVICE
        for (difference_type i = 0; i < size; ++ i)
            functor_type () (v (i), e () (i));
#else
        difference_type i (0);
        DD (size, 2, r, (functor_type () (v (i), e () (i)), ++ i));
#endif
    }

    // Dense (proxy) case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign (F, V &v, const vector_expression<E> &e, dense_proxy_tag) {
        typedef F functor_type;
#ifdef BOOST_UBLAS_USE_INDEXING
        indexing_vector_assign (functor_type (), v, e);
#elif BOOST_UBLAS_USE_ITERATING
        iterating_vector_assign (functor_type (), v, e);
#else
        typedef typename V::difference_type difference_type;
        difference_type size (BOOST_UBLAS_SAME (v.size (), e ().size ()));
        if (size >= BOOST_UBLAS_ITERATOR_THRESHOLD)
            iterating_vector_assign (functor_type (), v, e);
        else
            indexing_vector_assign (functor_type (), v, e);
#endif
    }
    // Packed (proxy) case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign (F, V &v, const vector_expression<E> &e, packed_proxy_tag) {
        BOOST_UBLAS_CHECK (v.size () == e ().size (), bad_size ());
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
#if BOOST_UBLAS_TYPE_CHECK
        vector<value_type> cv (v.size ());
#ifndef BOOST_UBLAS_NO_ELEMENT_PROXIES
        indexing_vector_assign (scalar_assign<typename vector<value_type>::reference, value_type> (), cv, v);
        indexing_vector_assign (functor_type::template make_debug_functor<typename vector<value_type>::reference, value_type> (), cv, e);
#else
        indexing_vector_assign (scalar_assign<value_type&, value_type> (), cv, v);
        indexing_vector_assign (functor_type (), cv, e);
#endif
#endif
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        difference_type it_size (it_end - it);
        difference_type ite_size (ite_end - ite);
        if (it_size > 0 && ite_size > 0) {
            difference_type size ((std::min) (difference_type (it.index () - ite.index ()), ite_size));
            if (size > 0) {
                ite += size;
                ite_size -= size;
            }
        }
        if (it_size > 0 && ite_size > 0) {
            difference_type size ((std::min) (difference_type (ite.index () - it.index ()), it_size));
            if (size > 0) {
                it_size -= size;
                if (boost::is_same<BOOST_UBLAS_TYPENAME functor_type::assign_category, assign_tag>::value) {
                    while (-- size >= 0)
                        functor_type () (*it, value_type (0)), ++ it;
                } else {
                    it += size;
                }
            }
        }
        difference_type size ((std::min) (it_size, ite_size));
        it_size -= size;
        ite_size -= size;
        while (-- size >= 0)
            functor_type () (*it, *ite), ++ it, ++ ite;
        size = it_size;
        if (boost::is_same<BOOST_UBLAS_TYPENAME functor_type::assign_category, assign_tag>::value) {
            while (-- size >= 0)
                functor_type () (*it, value_type (0)), ++ it;
        } else {
            it += size;
        }
#if BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check<bool>::value)
            BOOST_UBLAS_CHECK (equals (v, cv), external_logic ());
#endif
    }
    // Sparse case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign (F, V &v, const vector_expression<E> &e, sparse_tag) {
        BOOST_UBLAS_CHECK (v.size () == e ().size (), bad_size ());
        typedef F functor_type;
        typedef typename V::value_type value_type;
#if BOOST_UBLAS_TYPE_CHECK
        vector<value_type> cv (v.size ());
#ifndef BOOST_UBLAS_NO_ELEMENT_PROXIES
        indexing_vector_assign (scalar_assign<typename vector<value_type>::reference, value_type> (), cv, v);
        indexing_vector_assign (functor_type::template make_debug_functor<typename vector<value_type>::reference, value_type> (), cv, e);
#else
        indexing_vector_assign (scalar_assign<value_type&, value_type> (), cv, v);
        indexing_vector_assign (functor_type (), cv, e);
#endif
#endif
        v.clear ();
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        while (ite != ite_end) {
            value_type t (*ite);
            if (t != value_type (0))
                v.insert (ite.index (), t);
            ++ ite;
        }
#if BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check<bool>::value)
            BOOST_UBLAS_CHECK (equals (v, cv), external_logic ());
#endif
    }
    // Sparse proxy case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_assign (F, V &v, const vector_expression<E> &e, sparse_proxy_tag) {
        BOOST_UBLAS_CHECK (v.size () == e ().size (), bad_size ());
        typedef F functor_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
        typedef typename V::reference reference;
#if BOOST_UBLAS_TYPE_CHECK
        vector<value_type> cv (v.size ());
#ifndef BOOST_UBLAS_NO_ELEMENT_PROXIES
        indexing_vector_assign (scalar_assign<typename vector<value_type>::reference, value_type> (), cv, v);
        indexing_vector_assign (functor_type::template make_debug_functor<typename vector<value_type>::reference, value_type> (), cv, e);
#else
        indexing_vector_assign (scalar_assign<value_type&, value_type> (), cv, v);
        indexing_vector_assign (functor_type (), cv, e);
#endif
#endif
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
        make_conformant (v, e);
#endif
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::const_iterator ite (e ().begin ());
        typename E::const_iterator ite_end (e ().end ());
        if (it != it_end && ite != ite_end) {
            size_type it_index = it.index (), ite_index = ite.index ();
            while (true) {
                difference_type compare = it_index - ite_index;
                if (compare == 0) {
                    functor_type () (*it, *ite);
                    ++ it, ++ ite;
                    if (it != it_end && ite != ite_end) {
                        it_index = it.index ();
                        ite_index = ite.index ();
                    } else
                        break;
                } else if (compare < 0) {
                    if (boost::is_same<BOOST_UBLAS_TYPENAME functor_type::assign_category, assign_tag>::value) {
                        functor_type () (*it, value_type (0));
                        ++ it;
                    } else
                        increment (it, it_end, - compare);
                    if (it != it_end)
                        it_index = it.index ();
                    else
                        break;
                } else if (compare > 0) {
                    increment (ite, ite_end, compare);
                    if (ite != ite_end)
                        ite_index = ite.index ();
                    else
                        break;
                }
            }
        }

        if (boost::is_same<BOOST_UBLAS_TYPENAME functor_type::assign_category, assign_tag>::value) {
            while (it != it_end) {
                functor_type () (*it, value_type (0));
                ++ it;
            }
        } else {
            it = it_end;
        }
#if BOOST_UBLAS_TYPE_CHECK
        if (! disable_type_check<bool>::value)
            BOOST_UBLAS_CHECK (equals (v, cv), external_logic ());
#endif
    }

    // Dispatcher
    template<class F, class V, class E>
    BOOST_UBLAS_INLINE
    void vector_assign (F, V &v, const vector_expression<E> &e) {
        typedef F functor_type;
        typedef typename vector_assign_traits<BOOST_UBLAS_TYPENAME V::storage_category,
                                              BOOST_UBLAS_TYPENAME F::assign_category,
                                              BOOST_UBLAS_TYPENAME E::const_iterator::iterator_category>::storage_category storage_category;
        vector_assign (functor_type (), v, e, storage_category ());
    }

    template<class LS, class RI>
    struct vector_swap_traits {
        typedef LS storage_category;
    };

    template<>
    struct vector_swap_traits<dense_proxy_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    template<>
    struct vector_swap_traits<packed_proxy_tag, sparse_bidirectional_iterator_tag> {
        typedef sparse_proxy_tag storage_category;
    };

    // Dense (proxy) case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_swap (F, V &v, vector_expression<E> &e, dense_proxy_tag) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        difference_type size (BOOST_UBLAS_SAME (v.size (), e ().size ()));
        typename V::iterator it (v.begin ());
        typename E::iterator ite (e ().begin ());
        while (-- size >= 0)
            functor_type () (*it, *ite), ++ it, ++ ite;
    }
    // Packed (proxy) case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_swap (F, V &v, vector_expression<E> &e, packed_proxy_tag) {
        typedef F functor_type;
        typedef typename V::difference_type difference_type;
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::iterator ite (e ().begin ());
        typename E::iterator ite_end (e ().end ());
        difference_type it_size (it_end - it);
        difference_type ite_size (ite_end - ite);
        if (it_size > 0 && ite_size > 0) {
            difference_type size ((std::min) (difference_type (it.index () - ite.index ()), ite_size));
            if (size > 0) {
                ite += size;
                ite_size -= size;
            }
        }
        if (it_size > 0 && ite_size > 0) {
            difference_type size ((std::min) (difference_type (ite.index () - it.index ()), it_size));
            if (size > 0)
                it_size -= size;
        }
        difference_type size ((std::min) (it_size, ite_size));
        it_size -= size;
        ite_size -= size;
        while (-- size >= 0)
            functor_type () (*it, *ite), ++ it, ++ ite;
    }
    // Sparse proxy case
    template<class F, class V, class E>
    // This function seems to be big. So we do not let the compiler inline it.
    // BOOST_UBLAS_INLINE
    void vector_swap (F, V &v, vector_expression<E> &e, sparse_proxy_tag) {
        BOOST_UBLAS_CHECK (v.size () == e ().size (), bad_size ());
        typedef F functor_type;
        typedef typename V::size_type size_type;
        typedef typename V::difference_type difference_type;
        typedef typename V::value_type value_type;
#ifdef BOOST_UBLAS_NON_CONFORMANT_PROXIES
        make_conformant (v, e);
        make_conformant (e (), v);
#endif
        typename V::iterator it (v.begin ());
        typename V::iterator it_end (v.end ());
        typename E::iterator ite (e ().begin ());
        typename E::iterator ite_end (e ().end ());
        if (it != it_end && ite != ite_end) {
            size_type it_index = it.index (), ite_index = ite.index ();
            while (true) {
                difference_type compare = it_index - ite_index;
                if (compare == 0) {
                    functor_type () (*it, *ite);
                    ++ it, ++ ite;
                    if (it != it_end && ite != ite_end) {
                        it_index = it.index ();
                        ite_index = ite.index ();
                    } else
                        break;
                } else if (compare < 0) {
                    increment (it, it_end, - compare);
                    if (it != it_end)
                        it_index = it.index ();
                    else
                        break;
                } else if (compare > 0) {
                    increment (ite, ite_end, compare);
                    if (ite != ite_end)
                        ite_index = ite.index ();
                    else
                        break;
                }
            }
        }

#if BOOST_UBLAS_TYPE_CHECK
        increment (ite, ite_end);
        increment (it, it_end);
#endif
    }

    // Dispatcher
    template<class F, class V, class E>
    BOOST_UBLAS_INLINE
    void vector_swap (F, V &v, vector_expression<E> &e) {
        typedef F functor_type;
        typedef typename vector_swap_traits<BOOST_UBLAS_TYPENAME V::storage_category,
                                            BOOST_UBLAS_TYPENAME E::const_iterator::iterator_category>::storage_category storage_category;
        vector_swap (functor_type (), v, e, storage_category ());
    }

}}}

#endif
