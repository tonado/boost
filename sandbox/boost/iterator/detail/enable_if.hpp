// (C) Copyright David Abrahams 2002.
// (C) Copyright Jeremy Siek    2002.
// (C) Copyright Thomas Witt    2002.
// Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BOOST_ENABLE_IF_23022003THW_HPP
#define BOOST_ENABLE_IF_23022003THW_HPP

#include <boost/detail/workaround.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/iterator/detail/config_def.hpp>

//
// Boost iterators uses its own enable_if cause we need
// special semantics for deficient compilers.
// 23/02/03 thw
//

namespace boost
{

  namespace detail
  {
    //
    // Base machinery for all kinds of enable if
    //
    template<bool>
    struct enabled
    {
      template<typename T>
      struct base
      {
        typedef T type;
      };
    };
    
    //
    // For compilers that don't support "Substitution Failure Is Not An Error"
    // enable_if falls back to always enabled. See comments
    // on operator implementation for consequences.
    //
    template<>
    struct enabled<false>
    {
      template<typename T>
      struct base
      {
#ifdef BOOST_NO_SFINAE

        typedef T type;

        // This way to do it would give a nice error message containing
        // invalid overload, but has the big disadvantage that
        // there is no reference to user code in the error message.
        //
        // struct invalid_overload;
        // typedef invalid_overload type;
        //
#endif
      };
    };


    template <class Cond,
              class Return>
    struct enable_if
# if !defined(BOOST_NO_SFINAE) && !defined(BOOST_NO_IS_CONVERTIBLE)
      : enabled<Cond::value>::template base<Return>
# else
      : mpl::identity<Return>
# endif 
    {
# if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)
        typedef Return type;
# endif 
    };

  } // namespace detail

} // namespace boost

#include <boost/iterator/detail/config_undef.hpp>

#endif // BOOST_ENABLE_IF_23022003THW_HPP
