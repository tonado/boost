// Copyright David Abrahams 2001. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/python/converter/type_id.hpp>
#if !defined(__GNUC__) || __GNUC__ >= 3 || __SGI_STL_PORT
# include <ostream>
#else 
# include <ostream.h>
#endif 

namespace boost { namespace python { namespace converter { 

BOOST_PYTHON_DECL std::ostream& operator<<(std::ostream& os, undecorated_type_id_t const& x)
{
    return os << x.name();
}

BOOST_PYTHON_DECL std::ostream& operator<<(std::ostream& os, type_id_t const& x)
{
    os << x.m_base_type;
    // VC6 mistakenly distinguishes typeid(X) from typeid(X const)
    // from typeid(X&)...  so the name is already correct. I have it
    // from Jason Shirk that VC7.0 has the same bug but it will be
    // fixed in 7.1
# if !defined(BOOST_MSVC) || BOOST_MSVC > 1300
    if (x.m_decoration & type_id_t::const_)
        os << " const";
    if (x.m_decoration & type_id_t::volatile_)
        os << " volatile";
    if (x.m_decoration & type_id_t::reference)
        os << "&";
# endif 
    return os;
}

}}} // namespace boost::python::converter
