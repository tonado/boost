// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef CLASS_DWA20011214_HPP
# define CLASS_DWA20011214_HPP

# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/detail/config.hpp>
# include <boost/utility.hpp>
# include <boost/python/type_id.hpp>
# include <boost/python/reference.hpp>
# include <boost/python/instance_holder.hpp>
# include <cstddef>

namespace boost { namespace python {

class module;

namespace objects { 

// To identify a class, we don't need cv/reference decorations
typedef type_info class_id;

struct BOOST_PYTHON_DECL class_base : private noncopyable
{
    // constructor
    class_base(
        char const* name              // The name of the class
        
        , std::size_t num_types         // A list of class_ids. The first is the type
        , class_id const*const types    // this is wrapping. The rest are the types of
                                        // any bases.
        );

    // Retrieve the underlying object
    ref object() const { return m_object; }
    void add_property(char const* name, ref const& fget);
    void add_property(char const* name, ref const& fget, ref const& fset);
    void setattr(char const* name, ref const&);
    void enable_pickle_support();
 private:
    ref m_object;
};

BOOST_PYTHON_DECL ref registered_class_object(class_id id);

// Each extension instance will be one of these
struct instance
{
    PyObject_HEAD
    instance_holder* objects;
};

BOOST_PYTHON_DECL ref class_metatype();
BOOST_PYTHON_DECL ref class_type();

}}} // namespace boost::python::objects

#endif // CLASS_DWA20011214_HPP
