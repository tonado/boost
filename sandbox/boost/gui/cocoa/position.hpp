// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GUI_COCOA_POSITION_HPP
#define BOOST_GUI_COCOA_POSITION_HPP

#if(( defined(_MSC_VER) && (_MSC_VER >= 1020 )) || defined(__WAVE__))
#  pragma once
#endif

#include <boost/gui/cocoa/cocoa.hpp>
#include <boost/property/scalar_property.hpp>
#include <boost/property/aliased_property.hpp>

namespace boost { namespace gui { namespace cocoa
{

class position: public NSPoint
{
   public:
      typedef float   value_type;
      typedef NSPoint native_type;
      typedef boost::scalar_property< boost::aliased_property< value_type > >
         property_type;
   public:
      property_type xpos;
      property_type ypos;
   public:
      inline position & operator=( const position & p );
      inline position & operator=( const native_type & p );
   public:
      inline position( const native_type & p );
      inline position( const value_type & _x, const value_type & _y );
};

position & position::operator=( const position & p )
{
   x = p.x;
   y = p.y;
   return *this;
}

position & position::operator=( const native_type & p )
{
   x = p.x;
   y = p.y;
   return *this;
}

position::position( const native_type & p ):
   xpos( x ), ypos( y )
{
   x = p.x;
   y = p.y;
}

position::position( const value_type & _x, const value_type & _y ):
   xpos( x ), ypos( y )
{
   x = _x;
   y = _y;
}

}}}

#endif
