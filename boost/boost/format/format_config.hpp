// -*- C++ -*-
//  Boost general library 'format'   ---------------------------
//  See http://www.boost.org for updates, documentation, and revision history.

//  (C) Samuel Krempp 2001
//                  krempp@crans.ens-cachan.fr
//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// ideas taken from R�diger Loos's format class
// and Karl Nelson's ofstream (also took its parsing code as basis for printf parsing)

// ------------------------------------------------------------------------------
// format_config.hpp : configuration for the format library
// The contents of this file should be integrated into the boost config system.
// ------------------------------------------------------------------------------

#ifndef BOOST_FORMAT_CONFIG_HPP
#define BOOST_FORMAT_CONFIG_HPP

#include "boost/config.hpp"

// *** This should go to "boost/config/stdlib/stlport.hpp".

// If the streams are not native and there are problems with using templates
// accross namespaces, we define some macros to enable a workaround for this.

// STLport 4.5
#if !defined(_STLP_OWN_IOSTREAMS) && defined(_STLP_USE_NAMESPACES) && defined(BOOST_NO_USING_TEMPLATE)
#  define BOOST_IO_STD 
#  define BOOST_IO_NEEDS_USING_DECLARATION
#endif

// STLport 4.0
#if !defined(__SGI_STL_OWN_IOSTREAMS) && defined(__STL_USE_OWN_NAMESPACE) && defined(BOOST_NO_USING_TEMPLATE)
#  define BOOST_IO_STD 
#  define BOOST_IO_NEEDS_USING_DECLARATION
#endif

// *** This should go to "boost/config/suffix.hpp".

#ifndef BOOST_IO_STD
#  define BOOST_IO_STD std::
#endif

// **** Workaround for io streams, stlport and msvc.
#ifdef BOOST_IO_NEEDS_USING_DECLARATION
namespace boost {
  using std::char_traits;
  using std::basic_ostream;
  using std::basic_ostringstream;
  namespace io {
    using std::basic_ostream;
    namespace detail {
      using std::basic_ios;
      using std::basic_ostream;
      using std::basic_ostringstream;
    }
  }
}
#endif

// ------------------------------------------------------------------------------

#endif // BOOST_FORMAT_CONFIG_HPP
