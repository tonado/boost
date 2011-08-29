
//  (C) Copyright Edward Diener 2011
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(TTI_MF_MEMBER_TYPE_HPP)
#define TTI_MF_MEMBER_TYPE_HPP
  
#include <boost/config.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/tti/gen/namespace_gen.hpp>
#include <boost/tti/detail/dnotype.hpp>

/*

  The succeeding comments in this file are in doxygen format.

*/

/** \file
*/

namespace boost
  {
  namespace tti
    {
  
    /// A metafunction which checks whether the member 'type' returned from invoking the macro metafunction generated by BOOST_TTI_MEMBER_TYPE ( BOOST_TTI_TRAIT_MEMBER_TYPE ) or from invoking boost::tti::mf_member_type is a valid type.
    /**

        template<class TTI_T>
        struct mf_valid_member_type
          {
          static const value = unspecified;
          typedef mpl::bool_<true-or-false> type;
          };

        The metafunction types and return:

          TTI_T   = the nullary metafunction from invoking the macro metafunction generated by BOOST_TTI_MEMBER_TYPE ( BOOST_TTI_TRAIT_MEMBER_TYPE ) or from invoking boost::tti::mf_member_type.
      
          returns = 'value' is true if the type is valid, otherwise 'value' is false.
                    Validity means that the type being tested is not equal to an unspecified marker type.
                          
    */
    template
      <
      class TTI_T
      >
    struct mf_valid_member_type :
      boost::mpl::not_
        <
        boost::is_same
          <
          typename TTI_T::type,
          BOOST_TTI_NAMESPACE::detail::notype
          >
        >
      {
      };
      
    /// A metafunction whose typedef 'type' is either the internal type or an unspecified type.
    /**

        This metafunction takes its enclosing type as a nullary metafunction whose typedef 'type' member is the actual type used.
    
        template<class TTI_MemberType,class TTI_T>
        struct mf_member_type
          {
          typedef unspecified type;
          };

        The metafunction types and return:

          TTI_MemberType = a Boost MPL lambda expression using the metafunction generated from the BOOST_TTI_MEMBER_TYPE ( or BOOST_TTI_TRAIT_MEMBER_TYPE ) macro.<br />
                           The easiest way to generate the lambda expression is to use a Boost MPL placeholder expression of the form 'metafunction\<_\>'.
                           You can also use a metafunction class generated by boost::mpl::quote1.
                       
          TTI_T          = the enclosing type as a nullary metafunction.
      
          returns = 'type' is the inner type of the 'name' in BOOST_TTI_MEMBER_TYPE ( or BOOST_TTI_TRAIT_MEMBER_TYPE ) 
                    if the inner type exists within the enclosing type,
                    else 'type' is an unspecified type.
                          
        The purpose of this metafunction is to encapsulate the 'name' type
        in BOOST_TTI_MEMBER_TYPE ( or BOOST_TTI_TRAIT_MEMBER_TYPE ) as the typedef 'type'
        of a metafunction, but only if it exists within the enclosing type. This allows for
        a evaluation of inner type existence, without generating a compiler error,
        which can be used by other metafunctions in this library.
    
        Furthermore this metafunction allows the enclosing type to be return type from either the 
        metafunction generated from BOOST_TTI_MEMBER_TYPE ( or BOOST_TTI_TRAIT_MEMBER_TYPE ) or from this 
        metafunction itself.
    
    */
    template
      <
      class TTI_MemberType,
      class TTI_T
      >
    struct mf_member_type :
      boost::mpl::apply
        <
        TTI_MemberType,
        typename TTI_T::type
        > 
      {
      };
    
    }
  }
  
#endif // TTI_MF_MEMBER_TYPE_HPP
