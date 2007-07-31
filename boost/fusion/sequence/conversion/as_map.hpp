/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_AS_MAP_09232005_1340)
#define FUSION_AS_MAP_09232005_1340

#include <boost/fusion/sequence/conversion/detail/as_map.hpp>
#include <boost/fusion/sequence/container/map/map.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>

namespace boost { namespace fusion 
{
    namespace result_of
    {
        template <typename Sequence>
        struct as_map
        {
            typedef typename detail::as_map<result_of::size<Sequence>::value> gen;
            typedef typename gen::
                template apply<typename result_of::begin<Sequence>::type>::type
            type;
        };
    }

    template <typename Sequence>
    inline typename result_of::as_map<Sequence>::type
    as_map(Sequence& seq)
    {
        typedef typename result_of::as_map<Sequence>::gen gen;
        return gen::call(fusion::begin(seq));
    }

    template <typename Sequence>
    inline typename result_of::as_map<Sequence const>::type
    as_map(Sequence const& seq)
    {
        typedef typename result_of::as_map<Sequence const>::gen gen;
        return gen::call(fusion::begin(seq));
    }
}}

#endif
