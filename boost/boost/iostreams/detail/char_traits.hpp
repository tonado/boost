// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_DETAIL_CHAR_TRAITS_HPP_INCLUDED
#define BOOST_IOSTREAMS_DETAIL_CHAR_TRAITS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              
                
#include <iosfwd> 
#include <boost/iostreams/detail/config/wide_streams.hpp>
#ifdef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES
# include <boost/config.hpp> // Make sure size_t is in std.
# include <cstddef>
# include <cstring>
# include <cstdio>
#endif 

#ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //--------------------------------//
# define BOOST_IOSTREAMS_CHAR_TRAITS(ch) std::char_traits< ch >
#else
# define BOOST_IOSTREAMS_CHAR_TRAITS(ch) boost::iostreams::detail::char_traits

namespace boost { namespace iostreams { namespace detail {

struct char_traits {
    typedef char            char_type;
    typedef int             int_type;
    typedef std::streampos  pos_type;
    typedef std::streamoff  off_type;

    // Note: this may not be not conforming, since it treats chars as unsigned, 
    // but is only used to test for equality.
    static int compare(const char* lhs, const char* rhs, std::size_t n)
    { return std::strncmp(lhs, rhs, n); }
    static char* copy(char *dest, const char *src, std::size_t n)
    { return static_cast<char*>(std::memcpy(dest, src, n)); }
    static char* move(char *dest, const char *src, std::size_t n)
    { return static_cast<char*>(std::memmove(dest, src, n)); }
    static char to_char_type(const int& c) { return c; }
    static int to_int_type(const char& c) { return c; }
    static bool eq_int_type(const int& lhs, const int& rhs) 
    { return lhs == rhs; }
    static int eof() { return EOF; }
    static int not_eof(const int& c) { return c != EOF ? c : '\n'; }
};

} } } // End namespaces detail, iostreams, boost.

#endif // #ifdef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //-----------------------//

#endif // #ifndef BOOST_IOSTREAMS_DETAIL_CHAR_TRAITS_HPP_INCLUDED
