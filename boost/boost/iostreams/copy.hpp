// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// Contains: The function template copy, which reads data from a Source 
// and writes it to a Sink until the end of the sequence is reached, returning 
// the number of characters transfered.

// The implementation is complicated by the need to handle smart adapters
// and direct devices.

#ifndef BOOST_IOSTREAMS_COPY_HPP_INCLUDED
#define BOOST_IOSTREAMS_COPY_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <algorithm>                        // copy.
#include <ios>                              // failure.
#include <utility>                          // pair.
#include <boost/detail/workaround.hpp>
#include <boost/iostreams/constants.hpp>
#include <boost/iostreams/detail/buffer.hpp>       
#include <boost/iostreams/detail/closer.hpp>    
#include <boost/iostreams/detail/enable_if_stream.hpp>                      
#include <boost/iostreams/detail/resolve.hpp>                   
#include <boost/iostreams/detail/wrap_unwrap.hpp>
#include <boost/iostreams/is_direct.hpp>
#include <boost/iostreams/operations.hpp>          // read, write, close.
#include <boost/static_assert.hpp>  
#include <boost/type_traits/is_same.hpp>  

namespace boost { namespace iostreams {

namespace detail {

template<typename Source, typename Sink>
std::streamsize copy_impl( Source& src, Sink& snk, 
                           std::streamsize /* buffer_size */,
                           mpl::true_, mpl::true_ )
{   // Copy from a direct Source to a direct Sink.
    using namespace std;
    typedef typename io_char<Source>::type  char_type;
    typedef pair<char_type*, char_type*>    pair_type;
    pair_type p1 = iostreams::input_sequence(src);
    pair_type p2 = iostreams::output_sequence(snk);
    if (p1.second - p1.first < p2.second - p2.first) {
        std::copy(p1.first, p1.second, p2.first);
        return static_cast<streamsize>(p1.second - p1.first);
    } else {
        throw ios::failure("destination too small");
    }
}

template<typename Source, typename Sink>
std::streamsize copy_impl( Source& src, Sink& snk, 
                           std::streamsize /* buffer_size */,
                           mpl::true_, mpl::false_ )
{   // Copy from a direct Source to an indirect Sink.
    using namespace std;
    typedef typename io_char<Source>::type  char_type;
    typedef pair<char_type*, char_type*>    pair_type;
    pair_type p = iostreams::input_sequence(src);
    iostreams::write(snk, p.first, static_cast<streamsize>(p.second - p.first));
    return static_cast<streamsize>(p.second - p.first);
}

template<typename Source, typename Sink>
std::streamsize copy_impl( Source& src, Sink& snk, 
                           std::streamsize buffer_size,
                           mpl::false_, mpl::true_ )
{   // Copy from an indirect Source to a direct Sink.
    using namespace std;
    typedef typename io_char<Source>::type  char_type;
    typedef pair<char_type*, char_type*>    pair_type;
    detail::basic_buffer<char_type> buf(buffer_size);
    pair_type p = snk.output_sequence();
    streamsize total = 0;
    bool done  = false;
    while (!done) {
        streamsize amt;
        done = (amt = iostreams::read(src, buf.data(), buffer_size)) 
                    != 
                buffer_size;
        std::copy(buf.data(), buf.data() + amt, p.first + total);
        total += amt;
    }
    return total;
}

template<typename Source, typename Sink>
std::streamsize copy_impl( Source& src, Sink& snk, 
                           std::streamsize buffer_size,
                           mpl::false_, mpl::false_ )
{   // Copy from an indirect Source to a indirect Sink.
    typedef typename io_char<Source>::type char_type;
    detail::basic_buffer<char_type> buf(buffer_size);
    std::streamsize total = 0;
    bool done  = false;
    while (!done) {
        std::streamsize amt;
        done = (amt = iostreams::read(src, buf.data(), buffer_size)) 
                    != 
                buffer_size;
        iostreams::write(snk, buf.data(), amt);
        total += amt;
    }
    return total;
}

template<typename Source, typename Sink>
std::streamsize copy_impl(Source src, Sink snk, std::streamsize buffer_size)
{
    using namespace std;
    typedef typename io_char<Source>::type  src_char;
    typedef typename io_char<Sink>::type    snk_char;
    BOOST_STATIC_ASSERT((is_same<src_char, snk_char>::value));
    external_closer<Source> close_source(src, ios::in | ios::out);
    external_closer<Sink> close_sink(snk, ios::in | ios::out);
    streamsize result =
        copy_impl( src, snk, buffer_size, 
                    is_direct<Source>(), is_direct<Sink>() );
    return result; 
}

} // End namespace detail.
                    
//------------------Definition of copy----------------------------------------//

template<typename Source, typename Sink>
std::streamsize
copy( const Source& src, const Sink& snk,
      std::streamsize buffer_size = default_buffer_size
      BOOST_IOSTREAMS_DISABLE_IF_STREAM(Source)
      BOOST_IOSTREAMS_DISABLE_IF_STREAM(Sink) )
{ 
    using namespace detail;
    typedef typename io_char<Source>::type char_type;
    return copy_impl( resolve<input, char_type>(src), 
                      resolve<output, char_type>(snk), 
                      buffer_size ); 
}

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300) //---------------------------------//
template<typename Source, typename Sink>
std::streamsize
copy( Source& src, const Sink& snk,
      std::streamsize buffer_size = default_buffer_size
      BOOST_IOSTREAMS_ENABLE_IF_STREAM(Source)
      BOOST_IOSTREAMS_DISABLE_IF_STREAM(Sink) ) 
{ 
    using namespace detail;
    typedef typename io_char<Source>::type char_type;
    return copy_impl(wrap(src), resolve<output, char_type>(snk), buffer_size);
}

template<typename Source, typename Sink>
std::streamsize
copy( const Source& src, Sink& snk,
      std::streamsize buffer_size = default_buffer_size
      BOOST_IOSTREAMS_DISABLE_IF_STREAM(Source)
      BOOST_IOSTREAMS_ENABLE_IF_STREAM(Sink) ) 
{ 
    using namespace detail;
    typedef typename io_char<Source>::type char_type;
    return copy_impl(resolve<input, char_type>(src), wrap(snk), buffer_size);
}

template<typename Source, typename Sink>
std::streamsize
copy( Source& src, Sink& snk,
      std::streamsize buffer_size = default_buffer_size
      BOOST_IOSTREAMS_ENABLE_IF_STREAM(Source)
      BOOST_IOSTREAMS_ENABLE_IF_STREAM(Sink) ) 
{ 
    using namespace detail;
    return copy_impl(wrap(src), wrap(snk), buffer_size);
}
#endif // #if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300) //-----------------------//

} } // End namespaces iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_COPY_HPP_INCLUDED
