// (C) Copyright Jonathan Turkanis 2005.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_COUNTER_HPP_INCLUDED
#define BOOST_IOSTREAMS_COUNTER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif         

#include <algorithm>  // count.
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/newline.hpp>
#include <boost/iostreams/operations.hpp>

#include <boost/iostreams/detail/config/disable_warnings.hpp> // VC7.1 C4244.

namespace boost { namespace iostreams {

//
// Template name: basic_counter.
// Template paramters:
//      Ch - The character type.
// Description: Filter which counts lines and characters.
//
template<typename Ch>
class basic_counter  {
public:
    typedef Ch char_type;
    struct io_category
        : dual_use,
          filter_tag,
          multichar_tag,
          optimally_buffered_tag
        { };
    basic_counter(int first_line = 0, int first_char = 0) 
        : lines_(first_line), chars_(first_char) 
        { }
    int lines() const { return lines_; }
    int characters() const { return chars_; }
    std::streamsize optimal_buffer_size() const { return 1; }

    template<typename Source>
    std::streamsize read(Source& src, char_type* s, std::streamsize n)
    {
        std::streamsize result = iostreams::read(src, s, n);
        lines_ += std::count(s, s + result, newline());
        chars_ += result;
        return result;
    }

    template<typename Sink>
    void write(Sink& snk, const char_type* s, std::streamsize n)
    {
        iostreams::write(snk, s, n);
        lines_ += std::count(s, s + n, newline());
        chars_ += n;
    }
private:
    Ch newline() const { return detail::newline<Ch>::value; }
    int lines_;
    int chars_;
};

typedef basic_counter<char>     counter;
typedef basic_counter<wchar_t>  wcounter;

} } // End namespaces iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp>

#endif // #ifndef BOOST_IOSTREAMS_COUNTER_HPP_INCLUDED
