// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef OBJECT_PROTOCOL_CORE_DWA2002615_HPP
# define OBJECT_PROTOCOL_CORE_DWA2002615_HPP

# include <boost/python/detail/prefix.hpp>

# include <boost/python/detail/config.hpp>
# include <boost/python/handle_fwd.hpp>

namespace boost { namespace python { 

namespace api
{
  class object;

  BOOST_PYTHON_DECL object getattr(object const& target, object const& key);
  BOOST_PYTHON_DECL object getattr(object const& target, object const& key, object const& default_);
  BOOST_PYTHON_DECL void setattr(object const& target, object const& key, object const& value);
  BOOST_PYTHON_DECL void delattr(object const& target, object const& key);

  // These are defined for efficiency, since attributes are commonly
  // accessed through literal strings.
  BOOST_PYTHON_DECL object getattr(object const& target, char const* key);
  BOOST_PYTHON_DECL object getattr(object const& target, char const* key, object const& default_);
  BOOST_PYTHON_DECL void setattr(object const& target, char const* key, object const& value);
  BOOST_PYTHON_DECL void delattr(object const& target, char const* key);
  
  BOOST_PYTHON_DECL object getitem(object const& target, object const& key);
  BOOST_PYTHON_DECL void setitem(object const& target, object const& key, object const& value);
  BOOST_PYTHON_DECL void delitem(object const& target, object const& key);

  BOOST_PYTHON_DECL object getslice(object const& target, handle<> const& begin, handle<> const& end);
  BOOST_PYTHON_DECL void setslice(object const& target, handle<> const& begin, handle<> const& end, object const& value);
  BOOST_PYTHON_DECL void delslice(object const& target, handle<> const& begin, handle<> const& end);
}

using api::getattr;
using api::setattr;
using api::delattr;

using api::getitem;
using api::setitem;
using api::delitem;

using api::getslice;
using api::setslice;
using api::delslice;

}} // namespace boost::python

#endif // OBJECT_PROTOCOL_CORE_DWA2002615_HPP
