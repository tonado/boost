
// (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
// Hinnant & John Maddock 2000.  Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_IS_VOLATILE_HPP_INCLUDED
#define BOOST_TT_IS_VOLATILE_HPP_INCLUDED

#include "boost/config.hpp"

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#   include "boost/type_traits/detail/cv_traits_impl.hpp"
#else
#   include "boost/type_traits/is_reference.hpp"
#   include "boost/type_traits/is_array.hpp"
#   include "boost/type_traits/detail/yes_no_type.hpp"
#   include "boost/type_traits/detail/false_result.hpp"
#endif

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//* is a type T declared volatile - is_volatile<T>
BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_volatile,T,::boost::detail::cv_traits_imp<T*>::is_volatile)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T&,false)

#if defined(__BORLANDC__) && (__BORLANDC__ < 0x600)
// these are illegal specialisations; cv-qualifies applied to
// references have no effect according to [8.3.2p1],
// C++ Builder requires them though as it treats cv-qualified
// references as distinct types...
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T& const,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T& volatile,false)
BOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_1(typename T,is_volatile,T& const volatile,false)
#endif

#else

namespace detail {

using ::boost::type_traits::yes_type;
using ::boost::type_traits::no_type;

yes_type is_volatile_tester(void const volatile*);
no_type is_volatile_tester(void const*);

template <bool is_ref, bool array>
struct is_volatile_helper
    : ::boost::type_traits::false_result
{
};

template <>
struct is_volatile_helper<false,false>
{
    template <typename T> struct result_
    {
        static T* t;
        BOOST_STATIC_CONSTANT(bool, value = (
            sizeof(detail::yes_type) == sizeof(detail::is_volatile_tester(t))
            ));
    };
};

template <>
struct is_volatile_helper<false,true>
{
    template <typename T> struct result_
    {
        static T t;
        BOOST_STATIC_CONSTANT(bool, value = (
            sizeof(detail::yes_type) == sizeof(detail::is_volatile_tester(&t))
            ));
    };
};

template <typename T>
struct is_volatile_impl
    : is_volatile_helper<
          is_reference<T>::value
        , is_array<T>::value
        >::template result_<T>
{
};

BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void,false)
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void const,false)
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void volatile,true)
BOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_volatile,void const volatile,true)
#endif

} // namespace detail

//* is a type T declared volatile - is_volatile<T>
BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_volatile,T,::boost::detail::is_volatile_impl<T>::value)

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_IS_VOLATILE_HPP_INCLUDED
