// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#include <boost/python/detail/config.hpp>
#include <boost/python/detail/wrap_python.hpp>
#include <boost/python/object/class.hpp>
#include <boost/bind.hpp>
#include <functional>

namespace boost { namespace python { namespace objects { 

instance_holder::instance_holder()
    : m_next(0)
{
}

instance_holder::~instance_holder()
{
}

PyTypeObject class_metatype_object = {
    PyObject_HEAD_INIT(0)//&PyType_Type)
        0,
        "Boost.Python.class",
        PyType_Type.tp_basicsize,
        0,
        0,                                      /* tp_dealloc */
        0,                                      /* tp_print */
        0,                                      /* tp_getattr */
        0,                                      /* tp_setattr */
        0,                                      /* tp_compare */
        0,                                      /* tp_repr */
        0,                                      /* tp_as_number */
        0,                                      /* tp_as_sequence */
        0,                                      /* tp_as_mapping */
        0,                                      /* tp_hash */
        0,                                      /* tp_call */
        0,                                      /* tp_str */
        0,                                      /* tp_getattro */
        0,                                      /* tp_setattro */
        0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | // Py_TPFLAGS_HAVE_GC |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
        0,                                      /* tp_doc */
        0,                                      /* tp_traverse */
        0,                                      /* tp_clear */
        0,                                      /* tp_richcompare */
        0,                                      /* tp_weaklistoffset */
        0,                                      /* tp_iter */
        0,                                      /* tp_iternext */
        0,                                      /* tp_methods */
        0,                                      /* tp_members */
        0,                                      /* tp_getset */
        0, //&PyType_Type,                           /* tp_base */
        0,                                      /* tp_dict */
        0,                                      /* tp_descr_get */
        0,                                      /* tp_descr_set */
        0,                                      /* tp_dictoffset */
        0,                                      /* tp_init */
        0,                                      /* tp_alloc */
        0,
        // PyType_GenericNew                       /* tp_new */
};

PyTypeObject class_type_object = {
    PyObject_HEAD_INIT(0) //&class_metatype_object)
        0,
        "Boost.Python.instance",
        sizeof(instance),
        0,
        0,                                      /* tp_dealloc */
        0,                                      /* tp_print */
        0,                                      /* tp_getattr */
        0,                                      /* tp_setattr */
        0,                                      /* tp_compare */
        0,                                      /* tp_repr */
        0,                                      /* tp_as_number */
        0,                                      /* tp_as_sequence */
        0,                                      /* tp_as_mapping */
        0,                                      /* tp_hash */
        0,                                      /* tp_call */
        0,                                      /* tp_str */
        0,                                      /* tp_getattro */
        0,                                      /* tp_setattro */
        0,                                      /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
        0,                                      /* tp_doc */
        0,                                      /* tp_traverse */
        0,                                      /* tp_clear */
        0,                                      /* tp_richcompare */
        0,                                      /* tp_weaklistoffset */
        0,                                      /* tp_iter */
        0,                                      /* tp_iternext */
        0,                                      /* tp_methods */
        0,                                      /* tp_members */
        0,                                      /* tp_getset */
    0, //&PyBaseObject_Type,                     /* tp_base */
        0,                                      /* tp_dict */
        0,                                      /* tp_descr_get */
        0,                                      /* tp_descr_set */
        0,                                      /* tp_dictoffset */
        0,                                      /* tp_init */
        0,                                      /* tp_alloc */
        PyType_GenericNew
};

BOOST_PYTHON_DECL PyTypeObject* class_metatype()
{
    if (class_metatype_object.tp_dict == 0)
    {
        class_metatype_object.ob_type = &PyType_Type;
        class_metatype_object.tp_base = &PyType_Type;
        if (PyType_Ready(&class_metatype_object))
            return 0;
    }
    Py_INCREF(&class_metatype_object);
    return &class_metatype_object;
}

BOOST_PYTHON_DECL PyTypeObject* class_type()
{
    if (class_type_object.tp_dict == 0)
    {
        class_type_object.ob_type = class_metatype();
        class_type_object.tp_base = &PyBaseObject_Type;
        if (PyType_Ready(&class_type_object))
            return 0;
    }
    Py_INCREF(&class_type_object);
    return &class_type_object;
}

void instance_holder::install(PyObject* self)
{
    assert(self->ob_type->ob_type == &class_metatype_object);
    m_next = ((instance*)self)->objects;
    ((instance*)self)->objects = this;
}

BOOST_PYTHON_DECL void*
find_instance_impl(PyObject* inst, converter::type_id_t type)
{
    if (inst->ob_type->ob_type != &class_metatype_object)
        return 0;
    
    instance* self = reinterpret_cast<instance*>(inst);

    for (instance_holder::iterator match(self->objects), end(0); match != end; ++match)
    {
        void* const found = (*match).holds(type);
        if (found)
            return found;
    }
    return 0;
}

}}} // namespace boost::python::objects
