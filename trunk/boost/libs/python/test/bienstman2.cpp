#include <boost/python/module.hpp>
#include <boost/python/class.hpp>

struct C {};

struct D {};

struct E
{
   const D fe (const C&)           {return D();}
   const D fe2(const C&, const C&) {return D();}
};

BOOST_PYTHON_MODULE_INIT(m)
{
  using namespace boost::python;

  module m("m");
  
  m
    .add(class_<C>("C"))
    .add(class_<D>("D"))
    .add(
      class_<E>("E")
      .def("fe",  &E::fe)  // this compiles.
      .def("fe2", &E::fe2) // this doesn't.
      )
    ;
}
