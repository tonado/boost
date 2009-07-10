//  Copyright (c) 2001-2009 Hartmut Kaiser
//  Copyright (c) 2001-2009 Joel de Guzman
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_GENERATOR_JANUARY_13_2009_1002AM)
#define BOOST_SPIRIT_GENERATOR_JANUARY_13_2009_1002AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/mpl/has_xxx.hpp>
#include <boost/spirit/home/karma/domain.hpp>

namespace boost { namespace spirit { namespace karma
{
    template <typename Derived>
    struct generator
    {
        struct generator_id;
        typedef Derived derived_type;
        typedef karma::domain domain;

        // Requirement: g.generate(o, context, delimiter, attr) -> bool
        //
        //  g:          a generator
        //  o:          output iterator
        //  context:    enclosing rule context (can be unused_type)
        //  delimit:    delimiter (can be unused_type)
        //  attr:       attribute (can be unused_type)

        // Requirement: g.what(context) -> info
        //
        //  g:          a generator
        //  context:    enclosing rule context (can be unused_type)

        // Requirement: G::template attribute<Ctx, Iter>::type
        //
        //  G:          a generator type
        //  Ctx:        A context type (can be unused_type)
        //  Iter:       An iterator type (always unused_type)

        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }
    };

    template <typename Derived>
    struct primitive_generator : generator<Derived>
    {
        struct primitive_generator_id;
    };

    template <typename Derived>
    struct nary_generator : generator<Derived>
    {
        struct nary_generator_id;

        // Requirement: g.elements -> fusion sequence
        //
        // g:   a composite generator

        // Requirement: G::elements_type -> fusion sequence
        //
        // G:   a composite generator type
    };

    template <typename Derived>
    struct unary_generator : generator<Derived>
    {
        struct unary_generator_id;

        // Requirement: g.subject -> subject generator
        //
        // g:   a unary generator

        // Requirement: G::subject_type -> subject generator type
        //
        // G:   a unary generator type
    };

    template <typename Derived>
    struct binary_generator : generator<Derived>
    {
        struct binary_generator_id;

        // Requirement: g.left -> left generator
        //
        // g:   a binary generator

        // Requirement: G::left_type -> left generator type
        //
        // G:   a binary generator type

        // Requirement: g.right -> right generator
        //
        // g:   a binary generator

        // Requirement: G::right_type -> right generator type
        //
        // G:   a binary generator type
    };

}}}

namespace boost { namespace spirit { namespace traits // classification
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(generator_id)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(primitive_generator_id)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(nary_generator_id)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(unary_generator_id)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(binary_generator_id)
    }

    template <typename T>
    struct is_generator : detail::has_generator_id<T> {};

    template <typename T>
    struct is_primitive_generator : detail::has_primitive_generator_id<T> {};

    template <typename T>
    struct is_nary_generator : detail::has_nary_generator_id<T> {};

    template <typename T>
    struct is_unary_generator : detail::has_unary_generator_id<T> {};

    template <typename T>
    struct is_binary_generator : detail::has_binary_generator_id<T> {};

}}}

#endif
