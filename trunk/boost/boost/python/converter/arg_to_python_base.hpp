// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef ARG_TO_PYTHON_BASE_DWA200237_HPP
# define ARG_TO_PYTHON_BASE_DWA200237_HPP
# include <boost/python/converter/to_python_function_type.hpp>
# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/reference.hpp>

namespace boost { namespace python { namespace converter {

namespace detail
{
  struct arg_to_python_holder
  {
      arg_to_python_holder(PyObject* obj);
      PyObject* get() const;
      PyObject* get_incref() const;
   private:
      ref m_held;
  };
  
  struct BOOST_PYTHON_DECL arg_to_python_base : arg_to_python_holder
  {
      arg_to_python_base(void const volatile* source, to_python_function_t);
  };

  //
  // implmentation
  //
  inline arg_to_python_holder::arg_to_python_holder(PyObject* obj)
      : m_held(obj)
  {
  }
  
  inline PyObject* arg_to_python_holder::get() const
  {
      return m_held.get();
  }

  inline PyObject* arg_to_python_holder::get_incref() const
  {
      PyObject* result = m_held.get();
      Py_XINCREF(result);
      return result;
  }
}

}}} // namespace boost::python::converter

#endif // ARG_TO_PYTHON_BASE_DWA200237_HPP
