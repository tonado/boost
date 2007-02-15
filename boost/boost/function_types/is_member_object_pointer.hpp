
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef BOOST_FT_IS_MEMBER_OBJECT_POINTER_HPP_INCLUDED
#define BOOST_FT_IS_MEMBER_OBJECT_POINTER_HPP_INCLUDED

#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/type_traits/detail/template_arity_spec.hpp>

#include <boost/function_types/components.hpp>

namespace boost 
{ 
  namespace function_types 
  {
    template< typename T, typename Tag = null_tag > 
    struct is_member_object_pointer
      : function_types::represents
        < function_types::components<T>
        , function_types::tag<Tag,detail::member_object_pointer_tag> 
        >
    { 
      BOOST_MPL_AUX_LAMBDA_SUPPORT(2,is_member_object_pointer,(T,Tag))
    };
  }
  BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(2,function_types::is_member_object_pointer)
} 

#endif 

