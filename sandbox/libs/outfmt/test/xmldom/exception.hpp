// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XMLDOM_LITE_EXCEPTION_HPP
#define BOOST_XMLDOM_LITE_EXCEPTION_HPP
#  include "exception.hpp" // xmldom::exception
#  include <string>

   namespace xmldom
   {
      class exception
      {
         public:
            std::string                message;
            std::string                file;
            long                       line;
         public:
            inline           exception( const std::string & m, const std::string & fn, long l ):
               message( m ),
               file( fn ),
               line( l )
            {
            }
      };

      inline std::ostream & operator<<( std::ostream & os, const exception & e )
      {
         return( os << e.file << "(line: " << e.line << "): " << e.message );
      }
   }
#endif
