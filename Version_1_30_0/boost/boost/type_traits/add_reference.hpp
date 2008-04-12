
// (C) Copyright Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock 2000.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_ADD_REFERENCE_HPP_INCLUDED
#define BOOST_TT_ADD_REFERENCE_HPP_INCLUDED

#include "boost/type_traits/is_reference.hpp"
#include "boost/detail/workaround.hpp"
#include "boost/config.hpp"

// should be the last #include
#include "boost/type_traits/detail/type_trait_def.hpp"

namespace boost {

namespace detail {

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && defined(BOOST_MSVC6_MEMBER_TEMPLATES)

template <bool x>
struct reference_adder
{
    template <typename T> struct result_
    {
        typedef T& type;
    };
};

template <>
struct reference_adder<true>
{
    template <typename T> struct result_
    {
        typedef T type;
    };
};

template <typename T>
struct add_reference_impl
{
    typedef typename reference_adder<
          ::boost::is_reference<T>::value
        >::template result_<T> result;

    typedef typename result::type type;
};

#else

template <typename T>
struct add_reference_impl
{
    typedef T& type;
};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
BOOST_TT_AUX_TYPE_TRAIT_IMPL_PARTIAL_SPEC1_1(typename T,add_reference,T&,T&)
#endif

#endif

// these full specialisations are always required:
BOOST_TT_AUX_TYPE_TRAIT_IMPL_SPEC1(add_reference,void,void)
#ifndef BOOST_NO_CV_VOID_SPECIALIZATIONS
BOOST_TT_AUX_TYPE_TRAIT_IMPL_SPEC1(add_reference,void const,void const)
BOOST_TT_AUX_TYPE_TRAIT_IMPL_SPEC1(add_reference,void volatile,void volatile)
BOOST_TT_AUX_TYPE_TRAIT_IMPL_SPEC1(add_reference,void const volatile,void const volatile)
#endif

} // namespace detail

BOOST_TT_AUX_TYPE_TRAIT_DEF1(add_reference,T,typename detail::add_reference_impl<T>::type)

// agurt, 07/mar/03: workaround Borland's ill-formed sensitivity to an additional 
// level of indirection, here
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x561))
BOOST_TT_AUX_TYPE_TRAIT_PARTIAL_SPEC1_1(typename T,add_reference,T&,T&)
#endif

} // namespace boost

#include "boost/type_traits/detail/type_trait_undef.hpp"

#endif // BOOST_TT_ADD_REFERENCE_HPP_INCLUDED
