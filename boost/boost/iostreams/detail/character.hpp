// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_DETAIL_CHARACTER_HPP_INCLUDED
#define BOOST_IOSTREAMS_DETAIL_CHARACTER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <string>              // char_traits.
#include <utility>             // pair.
#include <boost/optional.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace iostreams { namespace detail {

template<bool IsSame>
struct translate_char_impl;

//
// Template name: translate_char.
// Description: Translates a character or an end-of-file indicator from the 
//      int_type of one character traits type to the int_type of another.
//
template<typename SourceTr, typename TargetTr>
typename TargetTr::int_type 
translate_char(typename SourceTr::int_type c) 
{ 
    typedef translate_char_impl<is_same<SourceTr, TargetTr>::value> impl;
    return impl::template inner<SourceTr, TargetTr>::translate_char(c);
}

//----------------------------------------------------------------------------//

template<bool IsSame>
struct translate_char_impl;

template<>
struct translate_char_impl<true> {
    template<typename SourceTr, typename TargetTr>
    struct inner {
        static typename TargetTr::int_type 
        translate_char(typename SourceTr::int_type c) 
        { return c; }
    };
};

template<>
struct translate_char_impl<false> {
    template<typename SourceTr, typename TargetTr>
    struct inner {
        static typename TargetTr::int_type 
        translate_char(typename SourceTr::int_type c)
            { 
                return SourceTr::eq_int_type(SourceTr::eof()) ?
                            TargetTr::eof() :
                            TargetTr::to_int_type(SourceTr::to_char_type(c));
            }
    };
};

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_DETAIL_CHARACTER_HPP_INCLUDED
