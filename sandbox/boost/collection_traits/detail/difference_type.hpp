//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_DIFFERENCE_TYPE_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_DIFFERENCE_TYPE_HPP

#include <boost/collection_traits/detail/common.hpp>
#include <boost/detail/iterator.hpp>

//////////////////////////////////////////////////////////////////////////////
// missing partial specialization  workaround.
//////////////////////////////////////////////////////////////////////////////

namespace boost 
{
    namespace collection_traits_detail 
    {        
        template< typename T >
        struct collection_difference_type_;

        template<>
        struct collection_difference_type_<std_container_>
        {
            template< typename C >
            struct pts
            {
                typedef BOOST_DEDUCED_TYPENAME C::difference_type type;
            };
        };

        template<>
        struct collection_difference_type_<std_pair_>
        {
            template< typename P >
            struct pts
            {
                typedef BOOST_CT_DEDUCED_TYPENAME detail::iterator_traits< BOOST_DEDUCED_TYPENAME P::first_type>::difference_type type;                
            };
        };

        template<>
        struct collection_difference_type_<array_>
        {
            template< typename A >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };
        };

        template<>
        struct collection_difference_type_<char_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct collection_difference_type_<const_char_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct collection_difference_type_<wchar_t_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct collection_difference_type_<const_wchar_t_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef std::ptrdiff_t type;
            };         
        };
        
        template<>
        struct collection_difference_type_<iterator_>
        {
            template< typename I >
            struct pts
            {
                typedef BOOST_CT_DEDUCED_TYPENAME detail::iterator_traits<I>::difference_type type;
            };         
        };
    } 
    
    template< typename C >
    class difference_type_of
    {
        typedef BOOST_CT_DEDUCED_TYPENAME collection_traits_detail::collection<C>::type c_type;
    public:
        typedef BOOST_CT_DEDUCED_TYPENAME collection_traits_detail::collection_difference_type_<c_type>::BOOST_NESTED_TEMPLATE pts<C>::type type; 
    };

}

#endif

