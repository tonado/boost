//  Boost string_algo library example file  ---------------------------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

/*
    RLE compression using replace framwork. Goal is to compress a sequence of
    repeating characters into 3 bytes ( repeat mark, character and repetition count ).
    For simplification, it works only on numeric-value sequences.
*/

#include <string>
#include <iostream>
#include <limits>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/replace2.hpp>
#include <boost/string_algo/find2.hpp>

using namespace std;
using namespace boost;
namespace sa=boost::string_algo;

// replace mark specification, specialize for a specific element type
template< typename T > T repeat_mark() { return std::numeric_limits<T>::max(); };

// Compression  -----------------------------------------------------------------------


// compress finder -rle
/*
    Find a sequence which can be compressed. It has to be at least 3-character long
    sequence of repetitive characters 
*/
struct find_compressF 
{
    // Construction
    find_compressF() {}

    // Operation
    template<typename ForwardIteratorT>
    iterator_range<ForwardIteratorT> operator()( 
        ForwardIteratorT Begin, 
        ForwardIteratorT End ) const
    {
        typedef ForwardIteratorT input_iterator_type;
        typedef typename boost::detail::iterator_traits<input_iterator_type>::value_type value_type;
        typedef iterator_range<input_iterator_type> result_type;

        // begin of matchin segment
        input_iterator_type MStart=End;
        // Repetition counter
        value_type Cnt=0;

        // Search for a sequence of repetitive characters
        for(input_iterator_type It=Begin; It!=End;)
        {
            input_iterator_type It2=It++;

            if ( It==End || Cnt>=std::numeric_limits<value_type>::max() )
            {
                return result_type( MStart, It );
            }

            if ( *It==*It2 )
            {
                if ( MStart==End )
                {
                    // Mark the start
                    MStart=It2;
                }

                // Increate repetition counter
                Cnt++;
            }
            else
            {
                if ( MStart!=End )
                {
                    if ( Cnt>2 )
                        return result_type( MStart, It );
                    else
                    {
                        MStart=End;
                        Cnt=0;
                    }
                }
            }
        }

        return result_type( End, End );
    }
};

// rle compress format
/*
    Tranform a sequence into repeat mark, characted and count 
*/
template< typename SeqT >
struct format_compressF
{
private:
    typedef SeqT result_type;
    typedef typename SeqT::value_type value_type;

public:
    // Construction
    format_compressF() {};

    // Operation
    template< typename ReplaceT >
    result_type operator()( const ReplaceT& Replace ) const
    {
        SeqT r;
        r.push_back( repeat_mark<value_type>() );
        r.push_back( *(Replace.begin()) );
        r.push_back( value_type( Replace.size() ) );
        
        return r;
    }
};

// Decompression  -----------------------------------------------------------------------


// find decompress-rle functor
/*
    find a repetition block
*/
struct find_decompressF
{
    // Construction
    find_decompressF() {}

    // Operation
    template<typename ForwardIteratorT>
    iterator_range<ForwardIteratorT> operator()( 
        ForwardIteratorT Begin, 
        ForwardIteratorT End ) const
    {
        typedef ForwardIteratorT input_iterator_type;
        typedef typename boost::detail::iterator_traits<input_iterator_type>::value_type value_type;
        typedef iterator_range<input_iterator_type> result_type;

        for(input_iterator_type It=Begin; It!=End; It++)
        {
            if( *It==repeat_mark<value_type>() )
            {
                // Repeat mark found, extract body
                input_iterator_type It2=It++; 
                
                if ( It==End ) break;
                    It++; 
                if ( It==End ) break;
                    It++;
                
                return result_type( It2, It );
            }
        }

        return result_type( End, End );
    }
};

// rle decompress format
/*
    transform a repetition block into a seuqence of characters
*/
template< typename SeqT >
struct format_decompressF
{
private:
    typedef SeqT result_type;
    typedef typename SeqT::value_type value_type;

public:
    // Construction
    format_decompressF() {};

    // Operation
    template< typename ReplaceT >
    result_type operator()( const ReplaceT& Replace ) const
    {
        // extract info
        typename ReplaceT::const_iterator It=Replace.begin();

        value_type Value=*(++It);
        value_type Repeat=*(++It);
        
        SeqT r;
        for( value_type Index=0; Index<Repeat; Index++ ) r.push_back( Value );

        return r;
    }
};


int main()
{
    cout << "* RLE Compression Example *" << endl << endl;

    string original("123_AA_*ZZZZZZZZZZZZZZ*34");

    // copy compression
    string compress=sa::replace_all_copy( 
        original, 
        find_compressF(), 
        format_compressF<string>() );

    cout << "Compressed string: " << compress << endl;

    // Copy decompression
    string decompress=sa::replace_all_copy( 
        compress, 
        find_decompressF(), 
        format_decompressF<string>() );

    cout << "Decompressed string: " << decompress << endl;

    // in-place compression
    sa::replace_all( 
        original, 
        find_compressF(), 
        format_compressF<string>() );
    
    cout << "Compressed string: " << original << endl;

    // in-place decompression
    sa::replace_all( 
        original, 
        find_decompressF(), 
        format_decompressF<string>() );

    cout << "Decompressed string: " << original << endl;

    cout << endl;

    return 0;
}
