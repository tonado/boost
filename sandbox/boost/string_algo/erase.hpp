//  Boost string_algo library erase.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_HPP
#define BOOST_STRING_ERASE_HPP

#include <boost/detail/iterator.hpp>
#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {

//  erase_range -------------------------------------------------------//

    // erase_range iterator version
    template< typename ForwardIterator1T, typename OutputIteratorT >
    inline OutputIteratorT erase_range_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator1T SearchBegin,
        ForwardIterator1T SearchEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_range( 
                        Range,
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::empty_formatF< 
                typename boost::detail::iterator_traits<ForwardIterator1T>::value_type >() );
    }

    template< typename InputT, typename OutputIteratorT >
    inline OutputIteratorT erase_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_range(
                    Input,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_range sequence version
    template< typename InputT >
    inline InputT erase_range_copy( 
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_range(
                    Input,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // replace_first in-place sequence version
    template< typename InputT >
    inline InputT& erase_range( 
        InputT& Input,
        typename InputT::iterator SearchBegin,
        typename InputT::iterator SearchEnd )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_range(
                    Input,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

//  erase_first  --------------------------------------------------------//

    // erase_first iterator version
    template< 
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_first_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_first( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::empty_formatF< 
                typename boost::detail::iterator_traits<ForwardIterator1T>::value_type >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_first sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_first_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_first in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_first( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

//  erase_last  --------------------------------------------------------//

    // erase_last iterator version
    template< 
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_last_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_last( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::empty_formatF< 
                typename boost::detail::iterator_traits<ForwardIterator1T>::value_type >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_last( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_last sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_last_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::detail::
                create_find_last( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_last in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_last( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_last( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

//  erase_nth --------------------------------------------------------------------//

    // erase_nth iterator version
    template< 
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_nth_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd,
        unsigned int Nth )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_copy(
            Output,
            Range, 
            string_algo::detail::
                create_find_last( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ), Nth ),
            string_algo::detail::empty_formatF< 
                typename boost::detail::iterator_traits<ForwardIterator1T>::value_type >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_last( Input, Search, Nth ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_nth sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::detail::
                create_find_last( Input, Search, Nth ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_nth in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_last( Input, Search, Nth ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }


//  erase_all  --------------------------------------------------------//

    // erase_all iterator version
    template< 
        typename ForwardIterator1T, 
        typename ForwardIterator2T, 
        typename OutputIteratorT >
    inline OutputIteratorT erase_all_copy(
        OutputIteratorT Output,
        ForwardIterator1T Begin,
        ForwardIterator1T End,
        ForwardIterator2T SearchBegin,
        ForwardIterator2T SearchEnd )
    {
        iterator_range<ForwardIterator1T> Range( Begin, End );

        return string_algo::replace_all_copy(
            Output,
            Range,
            string_algo::detail::
                create_find_first( 
                    Range,
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::empty_formatF< 
                typename boost::detail::iterator_traits<ForwardIterator1T>::value_type >() );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename OutputIteratorT >
    inline OutputIteratorT erase_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_all sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_all_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }

    // erase_all in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_all( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::detail::
                create_find_first( Input, Search ),
            string_algo::detail::
                empty_formatF<typename InputT::value_type>() );
    }
    
} // namespace boost


#endif  // BOOST_ERASE_HPP
