// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef FIND_FROM_PYTHON_DWA2002223_HPP
# define FIND_FROM_PYTHON_DWA2002223_HPP

# include <boost/python/detail/config.hpp>
# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/converter/rvalue_from_python_data.hpp>

namespace boost { namespace python { namespace converter { 

struct from_python_registration;
struct rvalue_from_python_chain;

BOOST_PYTHON_DECL void* get_lvalue_from_python(
    PyObject* source, from_python_registration const&);

BOOST_PYTHON_DECL rvalue_from_python_stage1_data rvalue_from_python_stage1(
    PyObject* source, from_python_registration const&);

BOOST_PYTHON_DECL rvalue_from_python_chain const* implicit_conversion_chain(
    PyObject* source, from_python_registration const&);

}}} // namespace boost::python::converter

#endif // FIND_FROM_PYTHON_DWA2002223_HPP
