//  (C) Copyright Joel de Guzman 2003.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty, and
//  with no claim as to its suitability for any purpose.

#ifndef INDEXING_SUITE_JDG20036_HPP
# define INDEXING_SUITE_JDG20036_HPP

# include <boost/python/class.hpp>
# include <boost/python/register_ptr_to_python.hpp>
# include <boost/python/suite/indexing/detail/indexing_suite_detail.hpp>
# include <boost/python/suite/indexing/container_utils.hpp>
# include <boost/python/return_internal_reference.hpp>
# include <boost/python/iterator.hpp>
# include <boost/mpl/or.hpp>
# include <boost/mpl/not.hpp>

namespace boost { namespace python {
                   
    // indexing_suite class. This class is the protocol class for
    // the management of C++ containers intended to be integrated
    // to Python. The objective is make a C++ container look and
    // feel and behave exactly as we'd expect a Python container.
    // By default indexed elements are returned by proxy. This can be
    // disabled by supplying *true* in the NoProxy template parameter.
    // 
    // Derived classes provide the hooks needed by the indexing_suite
    // to do its job:
    //
    //      static element_type& 
    //      get_item(Container& container, index_type i);
    //
    //      static object 
    //      get_slice(Container& container, index_type from, index_type to);
    //
    //      static void 
    //      set_item(Container& container, index_type i, element_type const& v);
    //
    //      static void 
    //      set_slice(
    //         Container& container, index_type from, 
    //         index_type to, element_type const& v
    //      );
    //
    //      template <class Iter>
    //      static void 
    //      set_slice(Container& container, index_type from, 
    //          index_type to, Iter first, Iter last
    //      );
    //
    //      static void 
    //      delete_item(Container& container, index_type i);
    //        
    //      static void 
    //      delete_slice(Container& container, index_type from, index_type to);
    //        
    //      static size_t
    //      size(Container& container);
    //
    //      template <class T>
    //      static bool
    //      contains(Container& container, T const& val);
    //        
    //      static index_type
    //      convert_index(Container& container, PyObject* i);
    //        
    //      static index_type
    //      adjust_index(index_type current, index_type from, 
    //          index_type to, size_type len
    //      );
    //
    // Most of these policies are self explanatory. convert_index and 
    // adjust_index, however, deserves some explanation. 
    //
    // convert_index converts an Python index into a C++ index that the 
    // container can handle. For instance, negative indexes in Python, by 
    // convention, indexes from the right (e.g. C[-1] indexes the rightmost 
    // element in C). convert_index should handle the necessary conversion 
    // for the C++ container (e.g. convert -1 to C.size()-1). convert_index
    // should also be able to convert the type of the index (A dynamic Python
    // type) to the actual type that the C++ container expects.
    //
    // When a container expands or contracts, held indexes to its elements
    // must be adjusted to follow the movement of data. For instance, if
    // we erase 3 elements, starting from index 0 from a 5 element vector, 
    // what used to be at index 4 will now be at index 1:
    //
    //      [a][b][c][d][e] ---> [d][e]
    //                   ^           ^
    //                   4           1
    //
    // adjust_index takes care of the adjustment. Given a current index,
    // the function should return the adjusted index when data in the
    // container at index from..to is replaced by *len* elements.
    //

    template <
          class Container
        , class DerivedPolicies
        , bool NoProxy = false
        , bool NoSlice = false
        , class Element = typename Container::value_type
        , class Key = typename Container::value_type
        , class Index = typename Container::size_type
    >
    class indexing_suite 
        : public def_arg<
            indexing_suite<
              Container
            , DerivedPolicies
            , NoProxy
            , NoSlice
            , Element
            , Key
            , Index
        > >
    {
    private:
        
        typedef mpl::or_<
            mpl::bool_<NoProxy>
          , mpl::not_<is_class<Element> > > 
        no_proxy;
                    
        typedef detail::container_element<Container, Index, DerivedPolicies>
            container_element_t;
       
        typedef typename mpl::if_<
            no_proxy
          , iterator<Container>
          , iterator<Container, return_internal_reference<> > >::type
        def_iterator;
        
        typedef typename mpl::if_<
            no_proxy
          , detail::no_proxy_helper<
                Container
              , DerivedPolicies
              , container_element_t
              , Index>
          , detail::proxy_helper<
                Container
              , DerivedPolicies
              , container_element_t
              , Index> >::type
        proxy_handler;

        typedef typename mpl::if_<
            mpl::bool_<NoSlice>
          , detail::slice_helper<
                Container
              , DerivedPolicies
              , proxy_handler
              , Element
              , Index>
          , detail::slice_helper<
                Container
              , DerivedPolicies
              , proxy_handler
              , Element
              , Index> >::type
        slice_handler;
  
    public:
      
        template <class Class>
        void visit(Class& cl) const
        {
            // Hook into the class_ generic visitation .def function
            proxy_handler::register_container_element();
            
            cl
                .def("__len__", base_size)
                .def("__setitem__", &base_set_item)
                .def("__delitem__", &base_delete_item)
                .def("__getitem__", &base_get_item)
                .def("__contains__", &base_contains)
                .def("__iter__", def_iterator())

                .def("append", &base_append)
                .def("extend", &base_extend)
            ;         
        }        
        
    private:
                   
        static object
        base_get_item(back_reference<Container&> container, PyObject* i)
        { 
            if (PySlice_Check(i))
                return slice_handler::base_get_slice(
                    container.get(), reinterpret_cast<PySliceObject*>(i));
            
            return proxy_handler::base_get_item_(container, i);
        }
        
        static void 
        base_set_item(Container& container, PyObject* i, PyObject* v)
        {
            if (PySlice_Check(i))
            {
                 slice_handler::base_set_slice(container, 
                     reinterpret_cast<PySliceObject*>(i), v);
            }
            else
            {
                extract<Element&> elem(v);
                // try if elem is an exact Element
                if (elem.check())
                {
                    DerivedPolicies::
                        set_item(container, 
                            DerivedPolicies::
                                convert_index(container, i), elem());
                }
                else
                {
                    //  try to convert elem to Element
                    extract<Element> elem(v);
                    if (elem.check())
                    {
                        DerivedPolicies::
                            set_item(container, 
                                DerivedPolicies::
                                    convert_index(container, i), elem());
                    }
                    else
                    {
                        PyErr_SetString(PyExc_TypeError, "Invalid assignment");
                        throw_error_already_set();
                    }
                }
            }
        }

        static void 
        base_delete_item(Container& container, PyObject* i)
        {
            if (PySlice_Check(i))
            {
                slice_handler::base_delete_slice(
                    container, reinterpret_cast<PySliceObject*>(i));
                return;
            }
            
            Index index = DerivedPolicies::convert_index(container, i);
            proxy_handler::base_erase_indexes(container, index, index+1);
            DerivedPolicies::delete_item(container, index);
        } 

        static size_t
        base_size(Container& container)
        {
            return DerivedPolicies::size(container);
        }

        static bool
        base_contains(Container& container, PyObject* key)
        {
            extract<Key const&> x(key);
            //  try if key is an exact Key type
            if (x.check())
            {
                return DerivedPolicies::contains(container, x());
            }
            else
            {
                //  try to convert key to Key type
                extract<Key> x(key);
                if (x.check())
                    return DerivedPolicies::contains(container, x());
                else
                    return false;
            }            
        }
        
        static void
        base_append(Container& container, PyObject* v)
        {
            extract<Element&> elem(v);
            // try if elem is an exact Element
            if (elem.check())
            {
                DerivedPolicies::append(container, elem());
            }
            else
            {
                //  try to convert elem to Element
                extract<Element> elem(v);
                if (elem.check())
                {
                    DerivedPolicies::append(container, elem());
                }
                else
                {
                    PyErr_SetString(PyExc_TypeError, 
                        "Attempting to append an invalid type");
                    throw_error_already_set();
                }
            }
        }
        
        static void
        base_extend(Container& container, PyObject* v)
        {
            std::vector<Element> temp;
            handle<> l_(borrowed(v));
            object l(l_);
            container_utils::extend_container(temp, l);
            DerivedPolicies::extend(container, temp.begin(), temp.end());
        }
    };
    
}} // namespace boost::python 

#endif // INDEXING_SUITE_JDG20036_HPP
