// Copyright David Abrahams 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef NULLARY_FUNCTION_ADAPTOR_DWA2003824_HPP
# define NULLARY_FUNCTION_ADAPTOR_DWA2003824_HPP

namespace boost { namespace python { namespace detail { 

// nullary_function_adaptor -- a class template which ignores its
// arguments and calls a nullary function instead.  Used for building
// error-reporting functions, c.f. pure_virtual
template <class NullaryFunction>
struct nullary_function_adaptor
{
    nullary_function_adaptor(NullaryFunction fn)
      : m_fn(fn)
    {}

    void operator()() const { m_fn(); }

# define BOOST_PP_LOCAL_MACRO(i)                                            \
    template <BOOST_PP_ENUM_PARAMS_Z(1, i, class A)>                        \
    void operator()(                                                        \
        BOOST_PP_ENUM_BINARY_PARAMS_Z(1, i, A, const& BOOST_PP_INTERCEPT)   \
    ) const                                                                 \
    {                                                                       \
        m_fn();                                                             \
    }

# define BOOST_PP_LOCAL_LIMITS (1, BOOST_PYTHON_MAX_ARITY)
# include BOOST_PP_LOCAL_ITERATE()
    
 private:
    NullaryFunction m_fn;
};

}}} // namespace boost::python::detail

#endif // NULLARY_FUNCTION_ADAPTOR_DWA2003824_HPP
