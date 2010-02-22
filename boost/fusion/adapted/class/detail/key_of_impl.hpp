/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_CLASS_KEY_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_CLASS_KEY_OF_IMPL_HPP

#include <boost/fusion/adapted/struct/detail/key_of_impl.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct key_of_impl;

    template <>
    struct key_of_impl<assoc_class_iterator_tag>
      : key_of_impl<assoc_struct_iterator_tag>
    {};
}}}

#endif
