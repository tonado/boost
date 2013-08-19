/*==============================================================================
    Copyright (c) 2012 Matt Calabrese

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#include <boost/utility/enable_if_macros.hpp>

#include <boost/mpl/bool.hpp>
#include <utility>

boost::mpl::false_ has_nested_result_type( ... );

template< class T
        , BOOST_ENABLE_IF_VALID_TYPE( typename T::result_type )
        >
boost::mpl::true_ has_nested_result_type( T );

struct nested_result_type_t
{
  typedef void result_type;
};

static_assert( !decltype( has_nested_result_type( 1 ) )::value
             , "int incorrectly detected as having a nested result type."
             );

static_assert
( decltype( has_nested_result_type( nested_result_type_t() ) )::value
, "nested_result_type_t incorrectly detected as not having a "
  "nested result type."
);
