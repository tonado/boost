// Copyright Daniel Wallin, David Abrahams 2005. Use, modification and
// distribution is subject to the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DEFAULT_050329_HPP
#define DEFAULT_050329_HPP

namespace boost { namespace named_params { namespace aux {

// A wrapper for the default value passed by the user when resolving
// the value of the parameter with the given Keyword
template <class Keyword, class Value>
struct default_
{
    default_(Value& x)
      : value(x)
    {}

    Value& value;
};

//
// lazy_default -- 
//
//    A wrapper for the default value computation function passed by
//    the user when resolving the value of the parameter with the
//    given keyword
//
#if BOOST_WORKAROUND(__EDG_VERSION__, <= 300)
// These compilers need a little extra help with overload
// resolution; we have empty_arg_list's operator[] accept a base
// class to make that overload less preferable.
template <class KW, class DefaultComputer>
struct lazy_default_base
{
    lazy_default_base(DefaultComputer const& x)
      : default_(x)
    {}
    DefaultComputer const& compute_default;
};

template <class KW, class DefaultComputer>
struct lazy_default
  : lazy_default_base<KW,DefaultComputer>
  {
      lazy_default(DefaultComputer const & x)
        : lazy_default_base<KW,DefaultComputer>(x)
        {}
  };
# define BOOST_NAMED_PARAMS_lazy_default_fallback lazy_default_base
#else 
template <class KW, class DefaultComputer>
struct lazy_default
{
    lazy_default(const DefaultComputer& x)
      : compute_default(x)
    {}
    DefaultComputer const& compute_default;
};
# define BOOST_NAMED_PARAMS_lazy_default_fallback lazy_default
#endif 

}}} // namespace boost::named_params::aux

#endif // DEFAULT_050329_HPP

