    /**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_ASSIGN_EXCEPTION_HPP
#define BOOST_ASSIGN_EXCEPTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <exception>

namespace boost
{    
    namespace assignment
    {
        class assignment_exception : public std::exception
        {
            public:
        	assignment_exception( const char* what ) : what_( what )
        	{ }
        
        
        
        	virtual const char* what() const throw()
        	{
        	    return what_;
        	}
        
            private:
                const char* what_;
        };
    }
}
#endif
