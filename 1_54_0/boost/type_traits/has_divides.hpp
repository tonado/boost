//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_DIVIDES_HPP_INCLUDED
#define BOOST_TT_HAS_DIVIDES_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_divides
#define BOOST_TT_TRAIT_OP /
#define BOOST_TT_FORBIDDEN_IF\
   /* pointer with pointer or fundamental */\
   ::boost::type_traits::ice_or<\
      ::boost::type_traits::ice_and<\
         ::boost::is_pointer< Lhs_noref >::value,\
         ::boost::type_traits::ice_or<\
            ::boost::is_fundamental< Rhs_nocv >::value,\
            ::boost::is_pointer< Rhs_noref >::value\
         >::value\
      >::value,\
      ::boost::type_traits::ice_and<\
         ::boost::is_pointer< Rhs_noref >::value,\
         ::boost::type_traits::ice_or<\
            ::boost::is_fundamental< Lhs_nocv >::value,\
            ::boost::is_pointer< Lhs_noref >::value\
         >::value\
      >::value\
   >::value


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif
