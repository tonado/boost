//  Boost io_fwd.hpp header file  --------------------------------------------//

//  (C) Copyright boost.org 2002-2003.  Permission to copy, use, modify, sell
//  and distribute this software is granted provided this copyright notice
//  appears in all copies.  This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_IO_FWD_HPP
#define BOOST_IO_FWD_HPP

#include <cstddef>  // for std::size_t
#include <iosfwd>   // for std::char_traits (declaration)


namespace boost
{
namespace io
{


//  From <boost/io/ios_state.hpp>  -------------------------------------------//

class ios_flags_saver;
class ios_precision_saver;
class ios_width_saver;
class ios_base_all_saver;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_iostate_saver;
template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_exception_saver;
template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_tie_saver;
template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_rdbuf_saver;
template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_fill_saver;
template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_locale_saver;
template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ios_all_saver;

typedef basic_ios_iostate_saver<char>        ios_iostate_saver;
typedef basic_ios_iostate_saver<wchar_t>    wios_iostate_saver;
typedef basic_ios_exception_saver<char>      ios_exception_saver;
typedef basic_ios_exception_saver<wchar_t>  wios_exception_saver;
typedef basic_ios_tie_saver<char>            ios_tie_saver;
typedef basic_ios_tie_saver<wchar_t>        wios_tie_saver;
typedef basic_ios_rdbuf_saver<char>          ios_rdbuf_saver;
typedef basic_ios_rdbuf_saver<wchar_t>      wios_rdbuf_saver;
typedef basic_ios_fill_saver<char>           ios_fill_saver;
typedef basic_ios_fill_saver<wchar_t>       wios_fill_saver;
typedef basic_ios_locale_saver<char>         ios_locale_saver;
typedef basic_ios_locale_saver<wchar_t>     wios_locale_saver;
typedef basic_ios_all_saver<char>            ios_all_saver;
typedef basic_ios_all_saver<wchar_t>        wios_all_saver;

class ios_iword_saver;
class ios_pword_saver;
class ios_all_word_saver;


//  From <boost/io/iomanip.hpp>  ---------------------------------------------//

// Only has function templates


//  From <boost/io/streambuf_wrapping.hpp>  ----------------------------------//

template < class StreamBuf >
    class basic_wrapping_istream;

template < class StreamBuf >
    class basic_wrapping_ostream;

template < class StreamBuf >
    class basic_wrapping_iostream;


//  From <boost/io/array_stream.hpp>  ----------------------------------------//

template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_streambuf;

template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_istream;
template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_ostream;
template < std::size_t N, typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_array_stream;


//  From <boost/io/pointer_stream.hpp>  --------------------------------------//

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_pointerbuf;
typedef basic_pointerbuf<char>      pointerbuf;
typedef basic_pointerbuf<wchar_t>  wpointerbuf;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_ipointerstream;
typedef basic_ipointerstream<char>      ipointerstream;
typedef basic_ipointerstream<wchar_t>  wipointerstream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_opointerstream;
typedef basic_opointerstream<char>      opointerstream;
typedef basic_opointerstream<wchar_t>  wopointerstream;

template < typename Ch, class Tr = ::std::char_traits<Ch> >
    class basic_pointerstream;
typedef basic_pointerstream<char>      pointerstream;
typedef basic_pointerstream<wchar_t>  wpointerstream;


}  // namespace io
}  // namespace boost


#endif  // BOOST_IO_FWD_HPP
