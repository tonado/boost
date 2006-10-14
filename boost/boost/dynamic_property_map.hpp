#ifndef DYNAMIC_PROPERTY_MAP_RG09302004_HPP
#define DYNAMIC_PROPERTY_MAP_RG09302004_HPP

// Copyright 2004-5 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  dynamic_property_map.hpp -
//    Support for runtime-polymorphic property maps.  This header is factored
//  out of Doug Gregor's routines for reading GraphML files for use in reading
//  GraphViz graph files.

//  Authors: Doug Gregor
//           Ronald Garcia
//


#include <boost/config.hpp>
#include <boost/property_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
#include <boost/function/function3.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <typeinfo>
#include <boost/mpl/bool.hpp>
#include <stdexcept>
#include <sstream>
#include <map>
#include <boost/type.hpp>

namespace boost {

namespace detail {

  // read_value -
  //   A wrapper around lexical_cast, which does not behave as
  //   desired for std::string types.
  template<typename Value>
  inline Value read_value(const std::string& value)
  { return boost::lexical_cast<Value>(value); }

  template<>
  inline std::string read_value<std::string>(const std::string& value)
  { return value; }

}


// dynamic_property_map -
//  This interface supports polymorphic manipulation of property maps.
class dynamic_property_map
{
public:
  virtual ~dynamic_property_map() { }

  virtual boost::any get(const any& key) = 0;
  virtual std::string get_string(const any& key) = 0;
  virtual void put(const any& key, const any& value) = 0;
  virtual const std::type_info& key() const = 0;
  virtual const std::type_info& value() const = 0;
};


//////////////////////////////////////////////////////////////////////
// Property map exceptions
//////////////////////////////////////////////////////////////////////

struct dynamic_property_exception : public std::exception {
  virtual ~dynamic_property_exception() throw() {}
  virtual const char* what() const throw() = 0;
};

struct property_not_found : public dynamic_property_exception {
  std::string property;
  mutable std::string statement;
  property_not_found(const std::string& property) : property(property) {}
  virtual ~property_not_found() throw() {}

  const char* what() const throw() {
    if(statement.empty())
      statement =
        std::string("Property not found: ") + property + ".";

    return statement.c_str();
  }
};

struct dynamic_get_failure : public dynamic_property_exception {
  std::string property;
  mutable std::string statement;
  dynamic_get_failure(const std::string& property) : property(property) {}
  virtual ~dynamic_get_failure() throw() {}

  const char* what() const throw() {
    if(statement.empty())
      statement =
        std::string(
         "dynamic property get cannot retrieve value for  property: ")
        + property + ".";

    return statement.c_str();
  }
};

struct dynamic_const_put_error  : public dynamic_property_exception {
  virtual ~dynamic_const_put_error() throw() {}

  const char* what() const throw() {
    return "Attempt to put a value into a const property map: ";
  }
};


namespace detail {

//
// dynamic_property_map_adaptor -
//   property-map adaptor to support runtime polymorphism.
template<typename PropertyMap>
class dynamic_property_map_adaptor : public dynamic_property_map
{
  typedef typename property_traits<PropertyMap>::key_type key_type;
  typedef typename property_traits<PropertyMap>::value_type value_type;
  typedef typename property_traits<PropertyMap>::category category;

  // do_put - overloaded dispatches from the put() member function.
  //   Attempts to "put" to a property map that does not model
  //   WritablePropertyMap result in a runtime exception.

  //   in_value must either hold an object of value_type or a string that
  //   can be converted to value_type via iostreams.
  void do_put(const any& in_key, const any& in_value, mpl::bool_<true>)
  {
#if !(defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95))
    using boost::put;
#endif

    key_type key = any_cast<key_type>(in_key);
    if (in_value.type() == typeid(value_type)) {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
      boost::put(property_map, key, any_cast<value_type>(in_value));
#else
      put(property_map, key, any_cast<value_type>(in_value));
#endif
    } else {
      //  if in_value is an empty string, put a default constructed value_type.
      std::string v = any_cast<std::string>(in_value);
      if (v.empty()) {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
        boost::put(property_map, key, value_type());
#else
        put(property_map, key, value_type());
#endif
      } else {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
        boost::put(property_map, key, detail::read_value<value_type>(v));
#else
        put(property_map, key, detail::read_value<value_type>(v));
#endif
      }
    }
  }

  void do_put(const any&, const any&, mpl::bool_<false>)
  {
    throw dynamic_const_put_error();
  }

public:
  explicit dynamic_property_map_adaptor(const PropertyMap& property_map)
    : property_map(property_map) { }

  virtual boost::any get(const any& key)
  {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
    return boost::get(property_map, any_cast<key_type>(key));
#else
    using boost::get;

    return get(property_map, any_cast<key_type>(key));
#endif
  }

  virtual std::string get_string(const any& key)
  {
#if defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
    std::ostringstream out;
    out << boost::get(property_map, any_cast<key_type>(key));
    return out.str();
#else
    using boost::get;

    std::ostringstream out;
    out << get(property_map, any_cast<key_type>(key));
    return out.str();
#endif
  }

  virtual void put(const any& in_key, const any& in_value)
  {
    do_put(in_key, in_value,
           mpl::bool_<(is_convertible<category*,
                                      writable_property_map_tag*>::value)>());
  }

  virtual const std::type_info& key()   const { return typeid(key_type); }
  virtual const std::type_info& value() const { return typeid(value_type); }

  PropertyMap&       base()       { return property_map; }
  const PropertyMap& base() const { return property_map; }

private:
  PropertyMap property_map;
};

} // namespace detail

//
// dynamic_properties -
//   container for dynamic property maps
//
struct dynamic_properties
{
  typedef std::multimap<std::string, dynamic_property_map*>
    property_maps_type;
  typedef boost::function3<std::auto_ptr<dynamic_property_map>,
                           const std::string&,
                           const boost::any&,
                           const boost::any&> generate_fn_type;
public:

  typedef property_maps_type::iterator iterator;
  typedef property_maps_type::const_iterator const_iterator;

  dynamic_properties() : generate_fn() { }
  dynamic_properties(const generate_fn_type& g) : generate_fn(g) {}

  ~dynamic_properties()
  {
    for (property_maps_type::iterator i = property_maps.begin();
         i != property_maps.end(); ++i) {
      delete i->second;
    }
  }

  template<typename PropertyMap>
  dynamic_properties&
  property(const std::string& name, PropertyMap property_map)
  {
    // Tbd: exception safety
    std::auto_ptr<dynamic_property_map> pm(
      new detail::dynamic_property_map_adaptor<PropertyMap>(property_map));
    property_maps_type::iterator i =
      property_maps.insert(property_maps_type::value_type(name, 0));
    i->second = pm.release();

    return *this;
  }

  iterator       begin()       { return property_maps.begin(); }
  const_iterator begin() const { return property_maps.begin(); }
  iterator       end()         { return property_maps.end(); }
  const_iterator end() const   { return property_maps.end(); }

  iterator lower_bound(const std::string& name)
  { return property_maps.lower_bound(name); }

  const_iterator lower_bound(const std::string& name) const
  { return property_maps.lower_bound(name); }

  void
  insert(const std::string& name, std::auto_ptr<dynamic_property_map> pm)
  {
    property_maps.insert(property_maps_type::value_type(name, pm.release()));
  }

  template<typename Key, typename Value>
  std::auto_ptr<dynamic_property_map>
  generate(const std::string& name, const Key& key, const Value& value)
  {
    if(!generate_fn) {
      throw property_not_found(name);
    } else {
      return generate_fn(name,key,value);
    }
  }

private:
  property_maps_type property_maps;
  generate_fn_type generate_fn;
};

template<typename Key, typename Value>
bool
put(const std::string& name, dynamic_properties& dp, const Key& key,
    const Value& value)
{
  for (dynamic_properties::iterator i = dp.lower_bound(name);
       i != dp.end() && i->first == name; ++i) {
    if (i->second->key() == typeid(key)) {
      i->second->put(key, value);
      return true;
    }
  }

  std::auto_ptr<dynamic_property_map> new_map = dp.generate(name, key, value);
  if (new_map.get()) {
    new_map->put(key, value);
    dp.insert(name, new_map);
    return true;
  } else {
    return false;
  }
}

#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS 
template<typename Value, typename Key>
Value
get(const std::string& name, const dynamic_properties& dp, const Key& key)
{
  for (dynamic_properties::const_iterator i = dp.lower_bound(name);
       i != dp.end() && i->first == name; ++i) {
    if (i->second->key() == typeid(key))
      return any_cast<Value>(i->second->get(key));
  }

  throw dynamic_get_failure(name);
}
#endif

template<typename Value, typename Key>
Value
get(const std::string& name, const dynamic_properties& dp, const Key& key, type<Value>)
{
  for (dynamic_properties::const_iterator i = dp.lower_bound(name);
       i != dp.end() && i->first == name; ++i) {
    if (i->second->key() == typeid(key))
      return any_cast<Value>(i->second->get(key));
  }

  throw dynamic_get_failure(name);
}

template<typename Key>
std::string
get(const std::string& name, const dynamic_properties& dp, const Key& key)
{
  for (dynamic_properties::const_iterator i = dp.lower_bound(name);
       i != dp.end() && i->first == name; ++i) {
    if (i->second->key() == typeid(key))
      return i->second->get_string(key);
  }

  throw dynamic_get_failure(name);
}


//
// ref_singleton_map - always returns a reference to
// the same object.
//
template <typename KeyType, typename ValueType>
class ref_property_map :
  public
    boost::put_get_helper<ValueType&,ref_property_map<KeyType,ValueType> >
{ 
  ValueType* value;
public:
  typedef KeyType key_type;
  typedef ValueType value_type;
  typedef ValueType& reference;
  typedef lvalue_property_map_tag category;
  ref_property_map(ValueType& v) : value(&v) {}
  ValueType& operator[](key_type const&) const { return *value; }

};


} // namespace boost

#endif // DYNAMIC_PROPERTY_MAP_RG09302004_HPP
