// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_VALUE_TYPE_HPP
#define BOOST_CONTAINER_TRAITS_VALUE_TYPE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/collection_traits/config.hpp>

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/collection_traits/detail/value_type.hpp>
#else

#include <boost/detail/iterator.hpp>
#include <iterator>
#include <cstddef>
#include <utility>


namespace boost
{
    //////////////////////////////////////////////////////////////////////////
    // default
    //////////////////////////////////////////////////////////////////////////
    
    template< typename C >
    struct value_type_of
    {
        typedef BOOST_DEDUCED_TYPENAME C::value_type type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////

    template< typename Iterator >
    struct value_type_of< std::pair<Iterator,Iterator> >
    {
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<Iterator>::value_type type;
    };
    
    
    template< typename Iterator >
    struct value_type_of< const std::pair<Iterator,Iterator> >
    {
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<Iterator>::value_type type;
    };

    //////////////////////////////////////////////////////////////////////////
    // array
    //////////////////////////////////////////////////////////////////////////

    template< typename T, std::size_t sz >
    struct value_type_of< T[sz] >
    {
        typedef T type;
    };

    template< typename T, std::size_t sz >
    struct value_type_of< const T[sz] >
    {
        typedef const T type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct value_type_of< char* >
    {
        typedef char type;
    };

    template<>
    struct value_type_of< wchar_t* >
    {
        typedef wchar_t type;
    };

    template<>
    struct value_type_of< const char* >
    {
        typedef const char type;
    };

    template<>
    struct value_type_of< const wchar_t* >
    {
        typedef const wchar_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // iterator
    //////////////////////////////////////////////////////////////////////////

    template< typename C, typename T, typename D, typename P>
    struct value_type_of< std::istream_iterator<C,T,D,P> >
    {
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<std::istream_iterator<C,T,D,P> >::value_type type;
    };


    template< typename C, typename T, typename D, typename P>
    struct value_type_of< const std::istream_iterator<C,T,D,P> >
    {
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<std::istream_iterator<C,T,D,P> >::value_type type;
    };


} // namespace boost

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif
