/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_SEQUENCE_DETAIL_SEQUENCE_BASE_HPP)
#define FUSION_SEQUENCE_DETAIL_SEQUENCE_BASE_HPP

namespace boost { namespace fusion
{
    struct sequence_root {};

    template <typename Sequence>
    struct sequence_base : sequence_root {};
}}

#endif
