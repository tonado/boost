//  Copyright (c) 2001-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_RIGHT_ALIGNMENT_FEB_27_2007_1216PM)
#define BOOST_SPIRIT_KARMA_RIGHT_ALIGNMENT_FEB_27_2007_1216PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include <boost/spirit/home/support/component.hpp>
#include <boost/spirit/home/support/attribute_of.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/directive/detail/right_alignment_generate.hpp>
#include <boost/lexical_cast.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  The BOOST_KARMA_DEFAULT_FIELD_LENGTH specifies the default field length 
//  to be used for padding.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_KARMA_DEFAULT_FIELD_LENGTH)
#define BOOST_KARMA_DEFAULT_FIELD_LENGTH 10
#endif

namespace boost { namespace spirit { namespace karma 
{
    ///////////////////////////////////////////////////////////////////////////
    //  The simple right alignment directive is used for right_align[...] 
    //  generators. It uses default values for the generated width (defined via
    //  the BOOST_KARMA_DEFAULT_FIELD_LENGTH constant) and for the padding
    //  generator (always spaces).
    ///////////////////////////////////////////////////////////////////////////
    struct simple_right_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::argument1<Component>::type, 
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator, 
            typename Context, typename Delimiter, typename Parameter>
        static bool 
        generate(Component const& component, OutputIterator& sink, 
            Context& ctx, Delimiter const& d, Parameter const& param) 
        {
            return detail::right_align_generate(sink, ctx, d, param, 
                argument1(component), BOOST_KARMA_DEFAULT_FIELD_LENGTH, 
                spirit::as_component(karma::domain(), ' '));
        }

        template <typename Component>
        static std::string what(Component const& component)
        {
            std::string result = "right_align[";

            typedef typename
                spirit::result_of::argument1<Component>::type::director
            director;

            result += director::what(spirit::argument1(component));
            result += "]";
            return result;
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    //  The right alignment with width directive, is used for generators
    //  like right_align(width)[...]. It uses a default value for the padding
    //  generator (always spaces).
    ///////////////////////////////////////////////////////////////////////////
    struct width_right_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::subject<Component>::type, 
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator, 
            typename Context, typename Delimiter, typename Parameter>
        static bool 
        generate(Component const& component, OutputIterator& sink, 
            Context& ctx, Delimiter const& d, Parameter const& param) 
        {
            return detail::right_align_generate(sink, ctx, d, param, 
                subject(component), proto::arg_c<0>(argument1(component)), ' ');
        }

        template <typename Component>
        static std::string what(Component const& component)
        {
            std::string result = "right_align(";
            
            result += boost::lexical_cast<std::string>(
                proto::arg_c<0>(argument1(component)));
            result += ")[";

            typedef typename
                spirit::result_of::subject<Component>::type::director
            director;

            result += director::what(spirit::subject(component));
            result += "]";
            return result;
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    //  The right alignment directive with padding, is used for generators like
    //  right_align(padding)[...], where padding is a arbitrary generator 
    //  expression. It uses a default value for the generated width (defined 
    //  via the BOOST_KARMA_DEFAULT_FIELD_LENGTH constant).
    ///////////////////////////////////////////////////////////////////////////
    struct padding_right_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::subject<Component>::type, 
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator, 
            typename Context, typename Delimiter, typename Parameter>
        static bool 
        generate(Component const& component, OutputIterator& sink, 
            Context& ctx, Delimiter const& d, Parameter const& param) 
        {
            return detail::right_align_generate(sink, ctx, d, param, 
                subject(component), BOOST_KARMA_DEFAULT_FIELD_LENGTH, 
                argument1(component));
        }

        template <typename Component>
        static std::string what(Component const& component)
        {
            std::string result = "right_align(";
            
            typedef typename
                spirit::result_of::argument1<Component>::type::director
            padding;

            result += padding::what(spirit::argument1(component));
            result += ")[";

            typedef typename
                spirit::result_of::subject<Component>::type::director
            director;

            result += director::what(spirit::subject(component));
            result += "]";
            return result;
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    //  The full right alignment directive, is used for generators like 
    //  right_align(width, padding)[...], where width is a integer value to be 
    //  used as the field width and padding is a arbitrary generator 
    //  expression.
    ///////////////////////////////////////////////////////////////////////////
    struct full_right_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::subject<Component>::type, 
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator, 
            typename Context, typename Delimiter, typename Parameter>
        static bool 
        generate(Component const& component, OutputIterator& sink, 
            Context& ctx, Delimiter const& d, Parameter const& param) 
        {
            return detail::right_align_generate(sink, ctx, d, param, 
                subject(component), proto::arg_c<0>(argument1(component)), 
                argument2(component));
        }

        template <typename Component>
        static std::string what(Component const& component)
        {
            std::string result = "right_align(";
            
            result += boost::lexical_cast<std::string>(
                proto::arg_c<0>(argument1(component)));
            result += ", ";

            typedef typename
                spirit::result_of::argument2<Component>::type::director
            padding;

            result += padding::what(spirit::argument2(component));
            result += ")[";

            typedef typename
                spirit::result_of::subject<Component>::type::director
            director;

            result += director::what(spirit::subject(component));
            result += "]";
            return result;
        }
    };
    
}}} // namespace boost::spirit::karma

#endif


