// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/python/converter/from_python.hpp>
#include <boost/python/converter/registrations.hpp>
#include <boost/python/converter/rvalue_from_python_data.hpp>
#include <boost/python/handle.hpp>
#include <boost/python/detail/raw_pyobject.hpp>
#include <boost/python/cast.hpp>
#include <vector>
#include <algorithm>

namespace boost { namespace python { namespace converter { 

// rvalue_from_python_stage1 -- do the first stage of a conversion
// from a Python object to a C++ rvalue.
//
//    source     - the Python object to be converted
//    converters - the registry entry for the target type T
//
// Postcondition: where x is the result, one of:
//
//   1. x.convertible == 0, indicating failure
//
//   2. x.construct == 0, x.convertible is the address of an object of
//      type T. Indicates a successful lvalue conversion
//
//   3. where y is of type rvalue_from_python_data<T>,
//      x.construct(source, y) attempts to construct an object of type T
//      in y. Indicates an rvalue converter was found
BOOST_PYTHON_DECL rvalue_from_python_stage1_data rvalue_from_python_stage1(
    PyObject* source
    , registration const& converters)
{
    rvalue_from_python_chain const* chain = converters.rvalue_chain;
    
    rvalue_from_python_stage1_data data;
    data.convertible = 0;
    for (;chain != 0; chain = chain->next)
    {
        void* r = chain->convertible(source);
        if (r != 0)
        {
            data.convertible = r;
            data.construct = chain->construct;
            break;
        }
    }
    return data;
}

// rvalue_result_from_python -- return the address of a C++ object which
// can be used as the result of calling a Python function.
//
//      src  - the Python object to be converted
//
//      data - a reference to the base part of a
//             rvalue_from_python_data<T> object, where T is the
//             target type of the conversion.
//
// Requires: data.convertible == &registered<T>::converters
//
BOOST_PYTHON_DECL void* rvalue_result_from_python(
    PyObject* src, rvalue_from_python_stage1_data& data)
{
    // Take possession of the source object.
    handle<> holder(src);

    // Retrieve the registration
    // Cast in two steps for less-capable compilers
    void const* converters_ = data.convertible;
    registration const& converters = *static_cast<registration const*>(converters_);

    // Look for an eligible converter
    data = rvalue_from_python_stage1(src, converters);
      
    if (!data.convertible)
    {
        handle<> msg(
            ::PyString_FromFormat(
                "No registered converter was able to produce a C++ rvalue of type %s from this Python object of type %s"
                , converters.target_type.name()
                , src->ob_type->tp_name
                ));
              
        PyErr_SetObject(PyExc_TypeError, msg.get());
        throw_error_already_set();
    }

    // If a construct function was registered (i.e. we found an
    // rvalue conversion), call it now.
    if (data.construct != 0)
        data.construct(src, &data);

    // Return the address of the resulting C++ object
    return data.convertible;
}

BOOST_PYTHON_DECL void* get_lvalue_from_python(
    PyObject* source
    , registration const& converters)
{
    lvalue_from_python_chain const* chain = converters.lvalue_chain;
    
    for (;chain != 0; chain = chain->next)
    {
        void* r = chain->convert(source);
        if (r != 0)
            return r;
    }
    return 0;
}

namespace
{
  // Prevent looping in implicit conversions. This could/should be
  // much more efficient, but will work for now.
  typedef std::vector<rvalue_from_python_chain const*> visited_t;
  static visited_t visited;

  inline bool visit(rvalue_from_python_chain const* chain)
  {
      visited_t::iterator const p = std::lower_bound(visited.begin(), visited.end(), chain);
      if (p != visited.end() && *p == chain)
          return false;
      visited.insert(p, chain);
      return true;
  }

  void unvisit(rvalue_from_python_chain const* chain)
  {
      visited_t::iterator const p = std::lower_bound(visited.begin(), visited.end(), chain);
      assert(p != visited.end());
      visited.erase(p);
  }
}

BOOST_PYTHON_DECL rvalue_from_python_chain const* implicit_conversion_chain(
    PyObject* source
    , registration const& converters)
{    
    rvalue_from_python_chain const* chain = converters.rvalue_chain;
    
    if (!visit(chain))
        return 0;
    
    try
    {
        for (;chain != 0; chain = chain->next)
        {
            if (chain->convertible(source))
                break;
        }
    }
    catch(...)
    {
        unvisit(chain);
        throw;
    }
    unvisit(chain);
    return chain;
}

BOOST_PYTHON_DECL void* reference_result_from_python(
    PyObject* source
    , registration const& converters)
{
    handle<> holder(source);
    if (source->ob_refcnt <= 2)
    {
        handle<> msg(
            ::PyString_FromFormat(
                "Attempt to return dangling pointer/reference to object of type: %s"
                , converters.target_type.name()));
          
        PyErr_SetObject(PyExc_ReferenceError, msg.get());

        throw_error_already_set();
    }
    void* result = get_lvalue_from_python(source, converters);
    if (!result)
    {
        handle<> msg(
            ::PyString_FromFormat(
                "No registered converter was able to extract a a C++ lvalue of type %s from this Python object of type %s"
                , converters.target_type.name()
                , source->ob_type->tp_name
                ));
              
        PyErr_SetObject(PyExc_TypeError, msg.get());

        throw_error_already_set();
    }
    return result;
}
  
BOOST_PYTHON_DECL void* pointer_result_from_python(
    PyObject* source
    , registration const& converters)
{
    if (source == Py_None)
    {
        Py_DECREF(source);
        return 0;
    }
    return reference_result_from_python(source, converters);
}
  
BOOST_PYTHON_DECL void throw_no_class_registered()
{
    PyErr_SetString(
        PyExc_TypeError
        , const_cast<char*>("class not registered for to_python type"));
    throw_error_already_set();
}
  
BOOST_PYTHON_DECL void void_result_from_python(PyObject* o)
{
    Py_DECREF(expect_non_null(o));
}

BOOST_PYTHON_DECL python::detail::new_reference
pytype_result_from_python(PyTypeObject* type_, PyObject* source)
{
    if (!PyObject_IsInstance(source, python::upcast<PyObject>(type_)))
    {
        handle<> keeper(source);
        handle<> msg(
            ::PyString_FromFormat(
                "Expecting a Python %s return type, but got an object of type %s instead"
                , type_
                , source->ob_type->tp_name
                ));
        PyErr_SetObject(PyExc_TypeError, msg.get());
        throw_error_already_set();
    }
    return python::detail::new_reference(source);
}

}}} // namespace boost::python::converter
