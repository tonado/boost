//=============================================================================
// Copyright (C) 2013 Cromwell D. Enage
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef BOOST_CONTAINER_GEN_IS_STACK_SELECTOR_HPP_INCLUDED
#define BOOST_CONTAINER_GEN_IS_STACK_SELECTOR_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/container_gen/selectors.hpp>

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace boost {

    template <typename Selector>
    struct is_stack_selector : ::boost::mpl::false_
    {
        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, is_stack_selector, (Selector))
    };

    template <>
    struct is_stack_selector<stack_selector_base> : ::boost::mpl::true_
    {
    };
}  // namespace boost

#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//[reference__is_stack_selector
namespace boost {

    template <typename Selector>
    struct is_stack_selector : ::boost::mpl::false_
    {
        //<-
        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, is_stack_selector, (Selector))
        //->
    };

    template <typename SequenceSelector>
    struct is_stack_selector<stack_selector<SequenceSelector> >
      : ::boost::mpl::true_
    {
    };
}  // namespace boost
//]

#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif  // BOOST_CONTAINER_GEN_IS_STACK_SELECTOR_HPP_INCLUDED

