//  Boost string_algo library find2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND2_HPP
#define BOOST_STRING_FIND2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/compare.hpp>

/*! \file
	Defines finder generators. Finder is a functor which is able to 
	find a subsequence matching a specific criteria in an input
	sequence. 
	Finders are used by as a plugable components for replace, find 
	and split facilities. This header contains generator functions 
	for finders provided in this library.

	In addtion there is a finder_adaptor class. It is a wrapper which
	simplifies the design of user defined finders
*/

namespace boost {
    namespace string_algo {

//  Finder adaptor -----------------------------------------------------------//
        
		//! Finder adaptor		
		/*!
			This class is provided to simplify writting of custom finders.
			Finder is required to have two () operators. 
			- Variant 1 with two iterators as parameters
			- Variant 2 with a container as a parameter
			Finder adaptor requires the base to have only the first type.
			Second one is added by this class.
		*/
		template<typename BaseT>
		struct finder_adaptor 
		{
		// Construction
			//!	Constructor
			/* 
				Constructs a finder adaptor
			*/
			explicit finder_adaptor( const BaseT& Base ) : m_Base(Base) {}
		
		// Operators

			//! Function operator variant 1
			/*!
				Function operator, performs find operation
			*/
			template<typename IteratorT>
            iterator_range<IteratorT> operator()( 
				IteratorT Begin, 
                IteratorT End ) const
			{
				return m_Base(Begin,End);
			}

			//! Function operator variant 2
			/*!
				Function operator, performs find operation
			*/
			template<typename InputT>
			iterator_range< BOOST_STRING_TYPENAME 
				string_algo::container_traits<InputT>::result_iterator >
			operator()( InputT& Input ) const
			{
	            return m_Base( begin(Input), end(Input) );
			}

		private:
			BaseT m_Base;
		};

		//! Finder adaptor construction helper
		/*!
			Construct a finder adaptor for a giver finder
			\param Base Base finder
			\return Finder adaptor encapsulating the base finder
		*/
		template<class BaseT>
		inline finder_adaptor<BaseT> make_finder_adaptor( const BaseT& Base )
		{
			return finder_adaptor<BaseT>(Base);
		}

//  Finder generators ------------------------------------------//
        
        //! "First" finder 
        /*!
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\return first_finder functor
        */
        template<typename ContainerT>
        inline finder_adaptor< detail::first_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator,
            is_equal> >
        first_finder( const ContainerT& Search )
        {
            return 
				make_finder_adaptor( 
					detail::first_finderF<
						BOOST_STRING_TYPENAME 
		                    container_traits<ContainerT>::const_iterator,
						is_equal>( Search, is_equal() ) );
        }

        //! "First" finder
        /*!
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Comp An element comparison predicate
			\return first_finder functor
        */
        template<typename ContainerT,typename PredicateT>
        inline finder_adaptor< detail::first_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator,
            PredicateT> >
        first_finder( 
            const ContainerT& Search, PredicateT Comp )
        {
            return 
				make_finder_adaptor( 
					detail::first_finderF<
						BOOST_STRING_TYPENAME 
							container_traits<ContainerT>::const_iterator,
						PredicateT>( Search, Comp ) );
        }

        //! "Last" finder
        /*!
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\return last_finder functor
        */
        template<typename ContainerT>
        inline finder_adaptor< detail::last_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator,
            is_equal> >
        last_finder( const ContainerT& Search )
        {
			return 
				make_finder_adaptor( 
					detail::last_finderF<
					BOOST_STRING_TYPENAME 
						container_traits<ContainerT>::const_iterator,
					is_equal>( Search, is_equal() ) );
        }
        //! "Last" finder
        /*!
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Comp An element comparison predicate
			\return last_finder functor
        */
        template<typename ContainerT, typename PredicateT>
        inline finder_adaptor< detail::last_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator,
            PredicateT> >
        last_finder( const ContainerT& Search, PredicateT Comp )
        {
            return 
				make_finder_adaptor(
					detail::last_finderF<
					BOOST_STRING_TYPENAME 
						container_traits<ContainerT>::const_iterator,
					PredicateT>( Search, Comp ) );
        }

        //! "Nth" finder
        /*!
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\param Nth An index of the match to be find
			\return nth_finder functor
        */
        template<typename ContainerT>
        inline finder_adaptor< detail::nth_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator,
            is_equal> >
        nth_finder( 
            const ContainerT& Search, 
            unsigned int Nth)
        {
            return 
				make_finder_adaptor(
					detail::nth_finderF<
					BOOST_STRING_TYPENAME 
						container_traits<ContainerT>::const_iterator,
					is_equal>( Search, Nth, is_equal() ) );
        }
        //! "Nth" finder
        /*!
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Nth An index of the match to be find
			\param Comp An element comparison predicate
			\return nth_finder functor
        */
        template<typename ContainerT, typename PredicateT>
        inline finder_adaptor< detail::nth_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator,
            PredicateT> >
        nth_finder( 
            const ContainerT& Search, 
            unsigned int Nth, 
            PredicateT Comp )
        {
            return 
				make_finder_adaptor(
					detail::nth_finderF<
					BOOST_STRING_TYPENAME 
	                    container_traits<ContainerT>::const_iterator,
					PredicateT>( Search, Nth, Comp ) );
        }

        //! "Head" finder
        /*!
            Construct head_finder. The finder returns a head of a given
            input. Head is a prefix of a sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.

			\param N The size of the head
			\return head_finder functor
        */
        inline finder_adaptor< detail::head_finderF >
        head_finder( unsigned int N )
        {
            return make_finder_adaptor( detail::head_finderF( N ) );
        }
        
        //! "Tail" finder
        /*!
            Construct tail_finder. The finder returns a tail of a given
            input. Tail is a suffix of the sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.

			\param N The size of the head
			\return tail_finder functor
        */
        inline finder_adaptor< detail::tail_finderF >
        tail_finder( unsigned int N )
        {
            return make_finder_adaptor( detail::tail_finderF( N ) );
        }

        //! "Token" finder
        /*!
            Construct token_finder. The finder searches for a token 
            specified by a predicate. It is similar to std::find_if 
            algorith, with an exception that it return a range of
            instead of a single iterator.

            If bCompress is set to true, adjacent matching tokens are 
            concatenated into one match. Thus the finder can be used to 
            search for continous segments of elements satisfying the 
            given predicate.

			\param Pred An element selection predicate
			\param bCompress Compress flag
			\return token_finder functor
        */
        template< typename PredicateT >
        inline finder_adaptor< detail::token_finderF<PredicateT> >
        token_finder( PredicateT Pred, bool bCompress=true )
        {
            return make_finder_adaptor( 
				detail::token_finderF<PredicateT>( Pred, bCompress ) );
        }

        //! "Range" finder
        /*!
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

			\param Begin Beginning of the range
			\param End End of the range
			\return range_finger functor
        */
        template< typename ForwardIteratorT >
        inline finder_adaptor< detail::range_finderF<ForwardIteratorT> >
        range_finder(
            ForwardIteratorT Begin,
            ForwardIteratorT End )
        {
            return make_finder_adaptor(
				detail::range_finderF<ForwardIteratorT>( Begin, End ) );
        }

        //! "Range" finder
        /*!
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

			\param Range The range.
			\return range_finger functor
        */
        template< typename ForwardIteratorT >
        inline finder_adaptor< detail::range_finderF<ForwardIteratorT> >
        range_finder( iterator_range<ForwardIteratorT> Range )
        {
            return 	make_finder_adaptor(
				detail::range_finderF<ForwardIteratorT>( Range ) );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_FIND2_HPP
