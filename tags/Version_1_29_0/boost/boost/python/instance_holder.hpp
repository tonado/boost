// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef INSTANCE_HOLDER_DWA2002517_HPP
# define INSTANCE_HOLDER_DWA2002517_HPP

# include <boost/python/detail/config.hpp>
# include <boost/utility.hpp>
# include <boost/python/type_id.hpp>
# include <cstddef>

namespace boost { namespace python { 

// Base class for all holders
struct BOOST_PYTHON_DECL instance_holder : private noncopyable
{
 public:
    instance_holder();
    virtual ~instance_holder();
    
    // return the next holder in a chain
    instance_holder* next() const;

    virtual void* holds(type_info) = 0;

    void install(PyObject* inst) throw();

    // These functions should probably be located elsewhere.
    
    // Allocate storage for an object of the given size at the given
    // offset in the Python instance<> object if bytes are available
    // there. Otherwise allocate size bytes of heap memory.
    static void* allocate(PyObject*, std::size_t offset, std::size_t size);

    // Deallocate storage from the heap if it was not carved out of
    // the given Python object by allocate(), above.
    static void deallocate(PyObject*, void* storage) throw();
 private:
    instance_holder* m_next;
};

// This macro is needed for implementation of derived holders
# define BOOST_PYTHON_UNFORWARD(N,ignored) (typename unforward<A##N>::type)(a##N)

//
// implementation
//
inline instance_holder* instance_holder::next() const
{
    return m_next;
}

}} // namespace boost::python

#endif // INSTANCE_HOLDER_DWA2002517_HPP
