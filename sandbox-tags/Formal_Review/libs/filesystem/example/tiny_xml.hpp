//  tiny XML sub-set tools  --------------------------------------------------//

//  (C) Copyright Beman Dawes 2002. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

//  Provides self-contained tools for this XML sub-set:
//
//    element ::= { "<" name { name "=" "\"" value "\"" } ">"
//                  {element} [contents] "</" name ">" }
//
//  The point of "self-contained" is to minimize tool-chain dependencies.

#ifndef BOOST_TINY_XML_H
#define BOOST_TINY_XML_H

#include "boost/smart_ptr.hpp" // for shared_ptr
#include "boost/utility.hpp"   // for noncopyable
#include <list>
#include <iostream>
#include <string>

namespace boost
{
  namespace tiny_xml
  {
    class element;
    struct attribute
    {
      std::string name;
      std::string value;

      attribute(){}
      attribute( const std::string & name, const std::string & value )
        : name(name), value(value) {}
    };
    typedef boost::shared_ptr< element >  element_ptr;
    typedef std::list< element_ptr  >     element_list;
    typedef std::list< attribute >        attribute_list;

    struct element
      : boost::noncopyable  // because of shared_ptr use
    {
      std::string     name;
      attribute_list  attributes;
      element_list    elements;
      std::string     content;

      element() {}
      explicit element( const std::string & name ) : name(name) {}
    };

    element_ptr parse( std::istream & in );
    // Precondition: stream positioned at either the initial "<"
    // or the first character after the initial "<".
    // Postcondition: stream positioned at the first character after final
    //  ">" (or eof).
    // Returns: an element_ptr to an element representing the parsed stream.
    // Throws: std::string on syntax error.

    void write( const element & e, std::ostream & out );

  }
}

#endif  // BOOST_TINY_XML_H



