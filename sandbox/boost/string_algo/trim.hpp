//  Boost string_algo library trim.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRIM_HPP
#define BOOST_STRING_TRIM_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/trim2.hpp>
#include <boost/string_algo/classification.hpp>
#include <locale>

/*! \file
    Defines trim algorithms.
    Trim algorithms are used to remove trailing and leading spaces from a 
    sequence (string). Space is recognized using given locales.
*/

namespace boost {

    //! Left trim
    /*!
        Remove all leading spaces from the input. 
        The result is a trimmed copy if the input

        \param Input An input container
        \param Loc a locale used for 'space' classification
        \return A trimmed copy if the input
    */
    template< typename ContainerT >
    inline ContainerT trim_left_copy( const ContainerT& Input, const std::locale& Loc=std::locale() )
    {
        return            
            string_algo::trim_left_copy(
                Input, 
                is_space<BOOST_STRING_TYPENAME ContainerT::value_type>( Loc ) );
    }

    //! Left trim
    /*!
        Remove all leading spaces from the input.
        The Input sequence is modified in-place.

        \param Input An input container
        \param Loc A locale used for 'space' classification
        \return A reference to the modified input
    */
    template< typename ContainerT >
    inline ContainerT& trim_left( ContainerT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            string_algo::trim_left( 
                Input, 
                is_space<BOOST_STRING_TYPENAME ContainerT::value_type>( Loc ) );
    }

    //! Right trim
    /*!
        Remove all trailing spaces from the input. 
        The result is a trimmed copy if the input

        \param Input An input container
        \param Loc A locale used for 'space' classification
        \return A trimmed copy if the input
    */  
    template< typename ContainerT >
    inline ContainerT trim_right_copy( const ContainerT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            string_algo::trim_right_copy( 
                Input, 
                is_space<BOOST_STRING_TYPENAME ContainerT::value_type>( Loc ) );
    }

    //! Right trim
    /*!
        Remove all trailing spaces from the input. 
        The input sequence is modified in-place.

        \param Input An input container
        \param Loc A locale used for 'space' classification
        \return A reference to the modified input
    */
    template< typename ContainerT >
    inline ContainerT& trim_right( ContainerT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            string_algo::trim_right(
                Input, 
                is_space<BOOST_STRING_TYPENAME ContainerT::value_type>( Loc ) );
    }

    //! Trim
    /*!
        Remove all leading and trailing spaces from the input. 
        The result is a trimmed copy if the input

        \param Input An input container
        \param Loc A locale used for 'space' classification
        \return A trimmed copy if the input
    */
    template< typename ContainerT >
    inline ContainerT trim_copy( const ContainerT& Input, const std::locale& Loc=std::locale() )
    {
        return
            string_algo::trim_copy(
                Input, 
                is_space<BOOST_STRING_TYPENAME ContainerT::value_type>( Loc ) );
    }
    
    //! Trim
    /*!
        Remove all leading and trailing spaces from the input. 
        The input sequence is modified in-place.

        \param Input An input container
        \param Loc A locale used for 'space' classification
        \return A reference to the modified input
    */
    template< typename ContainerT >
    inline ContainerT& trim( ContainerT& Input, const std::locale& Loc=std::locale() )
    {
        return
            string_algo::trim(
                Input, 
                is_space<BOOST_STRING_TYPENAME ContainerT::value_type>( Loc ) );
    }


} // namespace boost

#endif  // BOOST_STRING_TRIM_HPP
