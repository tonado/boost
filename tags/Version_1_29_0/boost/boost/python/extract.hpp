// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef EXTRACT_DWA200265_HPP
# define EXTRACT_DWA200265_HPP

# include <boost/python/converter/object_manager.hpp>
# include <boost/python/converter/from_python.hpp>
# include <boost/python/converter/rvalue_from_python_data.hpp>
# include <boost/python/converter/registered.hpp>
# include <boost/python/converter/registered_pointee.hpp>
# include <boost/python/detail/void_ptr.hpp>
# include <boost/call_traits.hpp>
# include <boost/python/detail/void_return.hpp>
# include <boost/python/object_core.hpp>
# include <boost/python/refcount.hpp>
# include <boost/utility.hpp>

namespace boost { namespace python {

namespace converter
{
  template <class Ptr>
  struct extract_pointer
  {
      typedef Ptr result_type;
      extract_pointer(PyObject*);
      
      bool check() const;
      Ptr operator()() const;
      
   private:
      PyObject* m_source;
      void* m_result;
  };
  
  template <class Ref>
  struct extract_reference
  {
      typedef Ref result_type;
      extract_reference(PyObject*);
      
      bool check() const;
      Ref operator()() const;
      
   private:
      PyObject* m_source;
      void* m_result;
  };
  
  template <class T>
  struct extract_rvalue : private noncopyable
  {
      typedef typename call_traits<T>::param_type result_type;
      extract_rvalue(PyObject*);

      bool check() const;
      result_type operator()() const;
   private:
      PyObject* m_source;
      mutable rvalue_from_python_data<T> m_data;
  };
  
  template <class T>
  struct extract_object_manager
  {
      typedef T result_type;
      extract_object_manager(PyObject*);

      bool check() const;
      result_type operator()() const;
   private:
      PyObject* m_source;
  };
  
  template <class T>
  struct select_extract
  {
      BOOST_STATIC_CONSTANT(
          bool, obj_mgr = is_object_manager<T>::value);

      BOOST_STATIC_CONSTANT(
          bool, ptr = is_pointer<T>::value);
    
      BOOST_STATIC_CONSTANT(
          bool, ref = is_reference<T>::value);

      typedef typename mpl::if_c<
          obj_mgr
          , extract_object_manager<T>
          , typename mpl::if_c<
              ptr
              , extract_pointer<T>
              , typename mpl::if_c<
                  ref
                  , extract_reference<T>
                  , extract_rvalue<T>
                >::type
            >::type
         >::type type;
  };
}

template <class T>
struct extract
    : converter::select_extract<T>::type
{
 private:
    typedef typename converter::select_extract<T>::type base;
 public:
    typedef typename base::result_type result_type;
    
    operator result_type() const
    {
        return (*this)();
    }
    
    extract(PyObject*);
    extract(object const&);
};

//
// Implementations
//
template <class T>
inline extract<T>::extract(PyObject* o)
    : base(o)
{
}

template <class T>
inline extract<T>::extract(object const& o)
    : base(o.ptr())
{
}

namespace converter
{
  template <class T>
  inline extract_rvalue<T>::extract_rvalue(PyObject* x)
      : m_source(x)
      , m_data(
          (rvalue_from_python_stage1)(x, registered<T>::converters)
          )
  {
  }
  
  template <class T>
  inline bool
  extract_rvalue<T>::check() const
  {
      return m_data.stage1.convertible;
  }

  template <class T>
  inline typename extract_rvalue<T>::result_type
  extract_rvalue<T>::operator()() const
  {
      return *(T*)(
          // Only do the stage2 conversion once
          m_data.stage1.convertible ==  m_data.storage.bytes
             ? m_data.storage.bytes
             : (rvalue_from_python_stage2)(m_source, m_data.stage1, registered<T>::converters)
          );
  }

  template <class Ref>
  inline extract_reference<Ref>::extract_reference(PyObject* obj)
      : m_source(obj)
      , m_result(
          (get_lvalue_from_python)(obj, registered<Ref>::converters)
          )
  {
  }

  template <class Ref>
  inline bool extract_reference<Ref>::check() const
  {
      return m_result != 0;
  }

  template <class Ref>
  inline Ref extract_reference<Ref>::operator()() const
  {
      if (m_result == 0)
          (throw_no_reference_from_python)(m_source, registered<Ref>::converters);
      
      return python::detail::void_ptr_to_reference(m_result, (Ref(*)())0);
  }

  template <class Ptr>
  inline extract_pointer<Ptr>::extract_pointer(PyObject* obj)
      : m_source(obj)
      , m_result(
          obj == Py_None ? 0 : (get_lvalue_from_python)(obj, registered_pointee<Ptr>::converters)
          )
  {
  }

  template <class Ptr>
  inline bool extract_pointer<Ptr>::check() const
  {
      return m_source == Py_None || m_result != 0;
  }

  template <class Ptr>
  inline Ptr extract_pointer<Ptr>::operator()() const
  {
      if (m_result == 0 && m_source != Py_None)
          (throw_no_pointer_from_python)(m_source, registered_pointee<Ptr>::converters);
      
      return Ptr(m_result);
  }

  template <class T>
  inline extract_object_manager<T>::extract_object_manager(PyObject* obj)
      : m_source(obj)
  {
  }

  template <class T>
  inline bool extract_object_manager<T>::check() const
  {
      return object_manager_traits<T>::check(m_source);
  }

  template <class T>
  inline T extract_object_manager<T>::operator()() const
  {
      return T(
          object_manager_traits<T>::adopt(python::incref(m_source))
          );
  }
}
  
}} // namespace boost::python::converter

#endif // EXTRACT_DWA200265_HPP
