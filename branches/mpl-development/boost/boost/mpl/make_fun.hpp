//-----------------------------------------------------------------------------
// boost mpl/make_fun.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_MAKE_FUN_HPP
#define BOOST_MPL_MAKE_FUN_HPP

namespace boost {
namespace mpl {

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)

template< template<typename> class UnaryFunction >
struct make_f_x
{
    template<typename U>
    struct apply
        : UnaryFunction<U>
    {
    };
};

template< template<typename, typename> class BinaryFunction >
struct make_f_xy
{
    template<typename U1, typename U2>
    struct apply
        : BinaryFunction<U1,U2>
    {
    };
};

template< template<typename, typename> class BinaryFunction >
struct make_f_yx
{
    template<typename U1, typename U2>
    struct apply
        : BinaryFunction<U2,U1>
    {
    };
};


#   define BOOST_MPL_MAKE_F_X(name, f) typedef boost::mpl::make_f_x<f> name
#   define BOOST_MPL_MAKE_F_XY(name, f) typedef boost::mpl::make_f_xy<f> name
#   define BOOST_MPL_MAKE_F_YX(name, f) typedef boost::mpl::make_f_yx<f> name

#else

#   define BOOST_MPL_MAKE_F_X(name, f)                                        \
    struct name { template<typename U> struct apply : f<U> {}; }              \
/**/

#   define BOOST_MPL_MAKE_F_XY(name, f) struct name                           \
    { template<typename U1, typename U2> struct apply : f<U1,U2> {}; }        \
/**/

#   define BOOST_MPL_MAKE_F_YX(name, f) struct name                           \
    { template<typename U1, typename U2> struct apply : f<U2,U1> {}; }        \
/**/

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_MAKE_FUN_HPP
