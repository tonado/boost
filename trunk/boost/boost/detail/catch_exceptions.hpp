//  boost/catch_exceptions.hpp -----------------------------------------------//

//  (C) Copyright Beman Dawes 1995-2001. Permission to copy, use, modify, sell
//  and distribute this software is granted provided this copyright notice
//  appears in all copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

//  Revision History
//   26 Feb 01 Numerous changes suggested during formal review. (Beman)
//   25 Jan 01 catch_exceptions.hpp code factored out of cpp_main.cpp.
//   22 Jan 01 Remove test_tools dependencies to reduce coupling.
//    5 Nov 00 Initial boost version (Beman Dawes)

#ifndef BOOST_CATCH_EXCEPTIONS_HPP
#define BOOST_CATCH_EXCEPTIONS_HPP

//  header dependencies are deliberately restricted to the standard library
//  to reduce coupling to other boost libraries.
#include <string>             // for string
#include <new>                // for bad_alloc
#include <typeinfo>           // for bad_cast, bad_typeid
#include <exception>          // for exception, bad_exception
#include <stdexcept>          // for std exception hierarchy
#include <boost/cstdlib.hpp>  // for exit codes
# if __GNUC__ != 2 || __GNUC_MINOR__ > 96
#   include <ostream>         // for ostream
# else
#   include <iostream> // workaround GNU missing ostream header
# endif


namespace boost
{

  namespace detail
  {
    //  A separate reporting function was requested during formal review.
    void report_exception( std::ostream & os, 
                           const char * name, const char * info )
      { os << "\n** uncaught exception: " << name << " " << info << std::endl; }
  }

  //  catch_exceptions  ------------------------------------------------------//

  template< class Generator >  // Generator is function object returning int
  int catch_exceptions( Generator function_object,
                        std::ostream & out, std::ostream & err )
  {
    int result = 0;               // quiet compiler warnings
    bool exception_thrown = true; // avoid setting result for each excptn type

    try
    {
      result = function_object();
      exception_thrown = false;
    }

    //  As a result of hard experience with strangely interleaved output
    //  under some compilers, there is a lot of use of endl in the code below
    //  where a simple '\n' might appear to do.

    //  The rules for catch & arguments are a bit different from function 
    //  arguments (ISO 15.3 paragraphs 18 & 19). Apparently const isn't
    //  required, but it doesn't hurt and some programmers ask for it.

    catch ( const char * ex )
      { detail::report_exception( out, "", ex ); }
    catch ( const std::string & ex )
      { detail::report_exception( out, "", ex.c_str() ); }

    //  std:: exceptions
    catch ( const std::bad_alloc & ex )
      { detail::report_exception( out, "std::bad_alloc:", ex.what() ); }

# if !defined(__BORLANDC__) || __BORLANDC__ > 0x0551
    catch ( const std::bad_cast & ex )
      { detail::report_exception( out, "std::bad_cast:", ex.what() ); }
    catch ( const std::bad_typeid & ex )
      { detail::report_exception( out, "std::bad_typeid:", ex.what() ); }
# else
    catch ( const std::bad_cast & ex )
      { detail::report_exception( out, "std::bad_cast", "" ); }
    catch ( const std::bad_typeid & ex )
      { detail::report_exception( out, "std::bad_typeid", "" ); }
# endif

    catch ( const std::bad_exception & ex )
      { detail::report_exception( out, "std::bad_exception:", ex.what() ); }
    catch ( const std::domain_error & ex )
      { detail::report_exception( out, "std::domain_error:", ex.what() ); }
    catch ( const std::invalid_argument & ex )
      { detail::report_exception( out, "std::invalid_argument:", ex.what() ); }
    catch ( const std::length_error & ex )
      { detail::report_exception( out, "std::length_error:", ex.what() ); }
    catch ( const std::out_of_range & ex )
      { detail::report_exception( out, "std::out_of_range:", ex.what() ); }
    catch ( const std::range_error & ex )
      { detail::report_exception( out, "std::range_error:", ex.what() ); }
    catch ( const std::overflow_error & ex )
      { detail::report_exception( out, "std::overflow_error:", ex.what() ); }
    catch ( const std::underflow_error & ex )
      { detail::report_exception( out, "std::underflow_error:", ex.what() ); }
    catch ( const std::logic_error & ex )
      { detail::report_exception( out, "std::logic_error:", ex.what() ); }
    catch ( const std::runtime_error & ex )
      { detail::report_exception( out, "std::runtime_error:", ex.what() ); }
    catch ( const std::exception & ex )
      { detail::report_exception( out, "std::exception:", ex.what() ); }

    catch ( ... )
      { detail::report_exception( out, "unknown exception", "" ); }

    if ( exception_thrown ) result = boost::exit_exception_failure;

    if ( result != 0 && result != exit_success )
    {
      out << std::endl << "**** returning with error code "
                << result << std::endl;
      err
        << "**********  errors detected; see stdout for details  ***********"
        << std::endl;
    }
    else { out << std::flush << "no errors detected" << std::endl; }

    return result;
  } // catch_exceptions

} // boost

#endif  // BOOST_CATCH_EXCEPTIONS_HPP

