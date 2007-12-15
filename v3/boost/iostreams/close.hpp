// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_CLOSE_HPP_INCLUDED
#define BOOST_IOSTREAMS_CLOSE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>  // DEDUCED_TYPENAME, MSVC.
#include <boost/detail/workaround.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/flush.hpp>
#include <boost/iostreams/detail/adapter/non_blocking_adapter.hpp>
#include <boost/iostreams/detail/ios.hpp> // BOOST_IOS
#include <boost/iostreams/detail/wrap_unwrap.hpp>
#include <boost/iostreams/operations_fwd.hpp>
#include <boost/iostreams/traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300) //-----------------------------------//
# include <boost/iostreams/detail/vc6/close.hpp>
#else // #if BOOST_WORKAROUND(BOOST_MSVC, < 1300) //--------------------------//

namespace boost { namespace iostreams {

namespace detail {

template<typename T>
struct close_impl;

} // End namespace detail.

template<typename T>
void close(T& t);

template<typename T>
void close(T& t, BOOST_IOS::openmode which)
{ 
    assert(which == BOOST_IOS::in || which == BOOST_IOS::out);
    detail::close_impl<T>::close(detail::unwrap(t), which); 
}

//template<typename T, typename Sink>
//void close( T& t, Sink& snk, 
//            typename 
//            boost::disable_if< 
//                boost::is_convertible<Sink, BOOST_IOS::openmode> 
//            >::type* = 0 )
//{ 
//    detail::close_all(t, snk);
//}

template<typename T, typename Sink>
void close(T& t, Sink& snk, BOOST_IOS::openmode which)
{ 
    assert(which == BOOST_IOS::in || which == BOOST_IOS::out);
    detail::close_impl<T>::close(detail::unwrap(t), snk, which); 
}

namespace detail {

//------------------Definition of close_impl----------------------------------//

template<typename T>
struct close_tag {
    typedef typename category_of<T>::type category;
    typedef typename
            mpl::eval_if<
                is_convertible<category, closable_tag>,
                mpl::if_<
                    mpl::or_<
                        is_convertible<category, two_sequence>,
                        is_convertible<category, dual_use>
                    >,
                    two_sequence,
                    closable_tag
                >,
                mpl::identity<any_tag>
            >::type type;
};

template<typename T>
struct close_impl
    : mpl::if_<
          is_custom<T>,
          operations<T>,
          close_impl<BOOST_DEDUCED_TYPENAME close_tag<T>::type>
      >::type
    { };

template<>
struct close_impl<any_tag> {
    template<typename T>
    static void close(T& t, BOOST_IOS::openmode which)
    {
        if (which == BOOST_IOS::out)
            iostreams::flush(t);
    }

    template<typename T, typename Sink>
    static void close(T& t, Sink& snk, BOOST_IOS::openmode which)
    {
        if (which == BOOST_IOS::out) {
            non_blocking_adapter<Sink> nb(snk);
            iostreams::flush(t, nb);
        }
    }
};

#include <boost/iostreams/detail/config/disable_warnings.hpp> // Borland.
template<>
struct close_impl<closable_tag> {
    template<typename T>
    static void close(T& t, BOOST_IOS::openmode which)
    {
        typedef typename category_of<T>::type category;
        const bool in =  is_convertible<category, input>::value &&
                        !is_convertible<category, output>::value;
        if (in == (which == BOOST_IOS::in))
            t.close();
    }
    template<typename T, typename Sink>
    static void close(T& t, Sink& snk, BOOST_IOS::openmode which)
    {
        typedef typename category_of<T>::type category;
        const bool in =  is_convertible<category, input>::value &&
                        !is_convertible<category, output>::value;
        if (in == (which == BOOST_IOS::in)) {
            non_blocking_adapter<Sink> nb(snk);
            t.close(nb);
        }
    }
};

template<>
struct close_impl<two_sequence> {
    template<typename T>
    static void close(T& t, BOOST_IOS::openmode which) { t.close(which); }
    template<typename T, typename Sink>
    static void close(T& t, Sink& snk, BOOST_IOS::openmode which)
    {
        non_blocking_adapter<Sink> nb(snk);
        t.close(nb, which);
    }
};

} // End namespace detail.

} } // End namespaces iostreams, boost.

#endif // #if BOOST_WORKAROUND(BOOST_MSVC, < 1300) //-------------------------//

namespace boost { namespace iostreams { 
    
namespace detail {

template<typename T>
void close_all(T& t)
{ 
    try {
        boost::iostreams::close(t, BOOST_IOS::in);
    } catch (...) {
        try {
            boost::iostreams::close(t, BOOST_IOS::out);
        } catch (...) { }
        throw;
    }
    boost::iostreams::close(t, BOOST_IOS::out);
}

template<typename T, typename Sink>
void close_all(T& t, Sink& snk)
{ 
    try {
        boost::iostreams::close(t, snk, BOOST_IOS::in);
    } catch (...) {
        try {
            boost::iostreams::close(t, snk, BOOST_IOS::out);
        } catch (...) { }
        throw;
    }
    boost::iostreams::close(t, snk, BOOST_IOS::out);
}

} // End namespaces detail. 

template<typename T>
void close(T& t) { detail::close_all(t); }

} } // End iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp>

#endif // #ifndef BOOST_IOSTREAMS_CLOSE_HPP_INCLUDED
