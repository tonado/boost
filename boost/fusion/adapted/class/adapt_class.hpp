/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_CLASS_ADAPT_CLASS_HPP
#define BOOST_FUSION_ADAPTED_CLASS_ADAPT_CLASS_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>

#include <boost/fusion/adapted/class/detail/extension.hpp>
#include <boost/fusion/adapted/class/detail/adapt_base.hpp>
#include <boost/fusion/adapted/class/detail/at_impl.hpp>
#include <boost/fusion/adapted/class/detail/is_view_impl.hpp>
#include <boost/fusion/adapted/class/detail/is_sequence_impl.hpp>
#include <boost/fusion/adapted/class/detail/value_at_impl.hpp>
#include <boost/fusion/adapted/class/detail/category_of_impl.hpp>
#include <boost/fusion/adapted/class/detail/size_impl.hpp>
#include <boost/fusion/adapted/class/detail/begin_impl.hpp>
#include <boost/fusion/adapted/class/detail/end_impl.hpp>
#include <boost/fusion/adapted/class/detail/value_of_impl.hpp>
#include <boost/fusion/adapted/class/detail/deref_impl.hpp>

#define BOOST_FUSION_ADAPT_CLASS_FILLER_0(A, B, C, D)\
    ((A, B, C, D)) BOOST_FUSION_ADAPT_CLASS_FILLER_1
#define BOOST_FUSION_ADAPT_CLASS_FILLER_1(A, B, C, D)\
    ((A, B, C, D)) BOOST_FUSION_ADAPT_CLASS_FILLER_0
#define BOOST_FUSION_ADAPT_CLASS_FILLER_0_END
#define BOOST_FUSION_ADAPT_CLASS_FILLER_1_END

#define BOOST_FUSION_ADAPT_CLASS_C(TEMPLATE_PARAMS_SEQ, NAME_SEQ, I, ATTRIBUTE) \
    BOOST_FUSION_ADAPT_CLASS_C_BASE(                                            \
        TEMPLATE_PARAMS_SEQ, NAME_SEQ, I, ATTRIBUTE, 4)

#define BOOST_FUSION_ADAPT_TPL_CLASS(TEMPLATE_PARAMS_SEQ, NAME_SEQ , ATTRIBUTES)\
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (1)TEMPLATE_PARAMS_SEQ,                                                 \
        (1)NAME_SEQ,                                                            \
        class_tag,                                                              \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_CLASS_FILLER_0 ATTRIBUTES,_END),        \
        BOOST_FUSION_ADAPT_CLASS_C)

#define BOOST_FUSION_ADAPT_CLASS(NAME, ATTRIBUTES)                              \
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (0),                                                                    \
        (0)(NAME),                                                              \
        class_tag,                                                              \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_CLASS_FILLER_0 ATTRIBUTES,_END),        \
        BOOST_FUSION_ADAPT_CLASS_C)


#endif
