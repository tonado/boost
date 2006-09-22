/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_PRIOR_05042005_1144)
#define FUSION_PRIOR_05042005_1144

#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    struct array_iterator_tag; // boost::array iterator tag
    struct mpl_iterator_tag; // mpl sequence iterator tag
    struct std_pair_iterator_tag; // std::pair iterator tag

    namespace extension
    {
        template <typename Tag>
        struct prior_impl
        {
            template <typename Iterator>
            struct apply {};
        };

        template <>
        struct prior_impl<array_iterator_tag>;

        template <>
        struct prior_impl<mpl_iterator_tag>;

        template <>
        struct prior_impl<std_pair_iterator_tag>;
    }

    namespace result_of
    {
        template <typename Iterator>
        struct prior
            : extension::prior_impl<typename detail::tag_of<Iterator>::type>::
                template apply<Iterator>
        {};
    }

    template <typename Iterator>
    typename result_of::prior<Iterator>::type
    prior(Iterator const& i)
    {
        return result_of::prior<Iterator>::call(i);
    }
}}

#endif
