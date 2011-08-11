
//  (C) Copyright Edward Diener 2011
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(TTI_MF_TEMPLATE_PARAMS_HPP)
#define TTI_MF_TEMPLATE_PARAMS_HPP

#include <boost/config.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/placeholders.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

namespace boost
  {
  namespace tti
    {
  
  /// A metafunction which checks whether a class template with its parameters exists within an enclosing type.
  /**

      This metafunction takes its enclosing type as nullary metafunctions whose typedef 'type' member is the actual type used.
    
      The metafunction types and return:

        HasTemplateCheckParams = a Boost MPL lambda expression using the metafunction generated from either the TTI_HAS_TEMPLATE_CHECK_PARAMS ( TTI_TRAIT_HAS_TEMPLATE_CHECK_PARAMS ) 
                      or TTI_VM_HAS_TEMPLATE_CHECK_PARAMS ( TTI_VM_TRAIT_HAS_TEMPLATE_CHECK_PARAMS ) macros.<br />
                      The easiest way to generate the lambda expression is to use a Boost MPL placeholder expression of the form 'metafunction\<_\>'.
                      You can also use the metafunction class generated by either the TTI_MTFC_HAS_TEMPLATE_CHECK_PARAMS ( TTI_MTFC_TRAIT_HAS_TEMPLATE_CHECK_PARAMS ) macro or the TTI_VM_MTFC_HAS_TEMPLATE_CHECK_PARAMS ( TTI_VM_MTFC_TRAIT_HAS_TEMPLATE_CHECK_PARAMS ) macro.
                      
        T           = The enclosing type as a nullary metafunction.
      
        returns = 'value' is true if the template exists within the enclosing type,
                  otherwise 'value' is false.
                          
  */
    template
      <
      class HasTemplateCheckParams,
      class T
      >
    struct mf_has_template_check_params :
      boost::mpl::apply
        <
        HasTemplateCheckParams,
        typename T::type
        >::type
      {
      };
    }
  }
  
#endif // TTI_MF_TEMPLATE_PARAMS_HPP
