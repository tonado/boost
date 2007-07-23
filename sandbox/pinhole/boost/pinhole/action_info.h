// MscProperty system action_info.h file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DETAILS_ACTION_INFO
#define BOOST_DETAILS_ACTION_INFO

#pragma warning(push)
#pragma warning( disable: 4272 4394 )
#include "Editor.h"
#pragma warning(pop)

#pragma warning(push)
#pragma warning( disable: 4561 4793 )
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning(pop)

namespace boost { namespace pinhole { namespace detail
{
    struct action_info
    {
    private:
        template<typename type> struct Internal;

    public:
        typedef boost::function<void ()> action_type;

        action_type m_action;
        std::string m_name;
        std::string m_description;

        /**
         * Calls the appropriate action function for this parameter.
         *
         * @throw boost::bad_function_call There isn't a set function associated with this property.
         */
        virtual void trigger()
        {
            m_action();
        }
    };
}}}

#endif // include guard