//  error_code_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//----------------------------------------------------------------------------// 

//  VC++ 8.0 warns on usage of certain Standard Library and API functions that
//  can be cause buffer overruns or no_posix_equivalent possible security issues if misused.
//  See http://msdn.microsoft.com/msdnmag/issues/05/05/SafeCandC/default.aspx
//  But the wording of the warning is misleading and unsettling, there are no
//  portable altersystem functions, and VC++ 8.0's own libraries use the
//  functions in question. So turn off the warnings.
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE

#include <boost/test/minimal.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <sstream>
#include <boost/cerrno.hpp>

//  Although using directives are not the best programming practice, testing
//  with a boost::system using directive increases use scenario coverage.
using namespace boost::system;

# if defined( BOOST_WINDOWS_API )
#   include "winerror.h"
#   define BOOST_ACCESS_ERROR_MACRO ERROR_ACCESS_DENIED
# elif defined( BOOST_POSIX_API )
#   define BOOST_ACCESS_ERROR_MACRO EACCES
# else
#   error "Only supported for POSIX and Windows"
# endif

namespace
{
  void check_ostream( error_code ec, const char * expected )
  {
    std::stringstream ss;
    std::string s;

    ss << ec;
    ss >> s;
    BOOST_CHECK( s == expected );
  }
}

//  test_main  ---------------------------------------------------------------//

// TODO: add hash_value tests

int test_main( int, char ** )
{

  std::cout << "General tests...\n";
  // unit tests:
  error_code ec;
  error_condition dec;
  BOOST_CHECK( !ec );
  BOOST_CHECK( ec.value() == 0 );
  dec = ec.default_error_condition();
  BOOST_CHECK( dec.value() == 0 );
  BOOST_CHECK( dec.category() == posix_category );
  BOOST_CHECK( ec == posix::success );
  BOOST_CHECK( ec.category() == system_category );
  BOOST_CHECK( ec.category().name() == "system" );

  error_code ec_0_system( 0, system_category );
  BOOST_CHECK( !ec_0_system );
  BOOST_CHECK( ec_0_system.value() == 0 );
  dec = ec_0_system.default_error_condition();
  BOOST_CHECK( dec.value() == 0 );
  BOOST_CHECK( dec.category() == posix_category );
  BOOST_CHECK( ec_0_system == posix::success );
  BOOST_CHECK( ec_0_system.category() == system_category );
  BOOST_CHECK( ec_0_system.category().name() == "system" );
  check_ostream( ec_0_system, "system:0" );

  BOOST_CHECK( ec_0_system == ec );

  error_code ec_1_system( 1, system_category );
  BOOST_CHECK( ec_1_system );
  BOOST_CHECK( ec_1_system.value() == 1 );
  BOOST_CHECK( ec_1_system.value() != 0 );
  BOOST_CHECK( ec != ec_1_system );
  BOOST_CHECK( ec_0_system != ec_1_system );
  check_ostream( ec_1_system, "system:1" );

  ec = error_code( BOOST_ACCESS_ERROR_MACRO, system_category );
  BOOST_CHECK( ec );
  BOOST_CHECK( ec.value() == BOOST_ACCESS_ERROR_MACRO );
  dec = ec.default_error_condition();
  BOOST_CHECK( dec.value() == static_cast<int>(posix::permission_denied) );
  BOOST_CHECK( dec.category() == posix_category );
  BOOST_CHECK( dec == error_condition( posix::permission_denied, posix_category ) );
  BOOST_CHECK( dec == posix::permission_denied );
  BOOST_CHECK( posix::permission_denied == dec );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( ec.category() == system_category );
  BOOST_CHECK( ec.category().name() == "system" );

  // test the explicit make_error_code conversion for posix
  ec = make_error_code( posix::bad_message );
  BOOST_CHECK( ec );
  BOOST_CHECK( ec == posix::bad_message );
  BOOST_CHECK( posix::bad_message == ec );
  BOOST_CHECK( ec != posix::permission_denied );
  BOOST_CHECK( posix::permission_denied != ec );
  BOOST_CHECK( ec.category() == posix_category );

  // test the deprecated predefined error_category synonyms
  BOOST_CHECK( &system_category == &native_ecat );
  BOOST_CHECK( &posix_category == &errno_ecat );
  BOOST_CHECK( system_category == native_ecat );
  BOOST_CHECK( posix_category == errno_ecat );

#ifdef BOOST_WINDOWS_API
  std::cout << "Windows tests...\n";
  // these tests probe the Windows posix decoder
  //   test the first entry in the decoder table:
  ec = error_code( ERROR_FILE_NOT_FOUND, system_category );
  BOOST_CHECK( ec.value() == ERROR_FILE_NOT_FOUND );
  BOOST_CHECK( ec == posix::no_such_file_or_directory );
  BOOST_CHECK( ec.default_error_condition().value() == posix::no_such_file_or_directory );

  //   test the second entry in the decoder table:
  ec = error_code( ERROR_PATH_NOT_FOUND, system_category );
  BOOST_CHECK( ec.value() == ERROR_PATH_NOT_FOUND );
  BOOST_CHECK( ec == posix::no_such_file_or_directory );
  BOOST_CHECK( ec.default_error_condition().value() == posix::no_such_file_or_directory );

  //   test the third entry in the decoder table:
  ec = error_code( ERROR_ACCESS_DENIED, system_category );
  BOOST_CHECK( ec.value() == ERROR_ACCESS_DENIED );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( ec.default_error_condition().value() == posix::permission_denied );

  //   test the last regular entry in the decoder table:
  ec = error_code( ERROR_WRITE_PROTECT, system_category );
  BOOST_CHECK( ec.value() == ERROR_WRITE_PROTECT );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( ec.default_error_condition().value() == posix::permission_denied );

  //   test not-in-table condition:
  ec = error_code( 1234567890, system_category );
  BOOST_CHECK( ec.value() == 1234567890 );
  BOOST_CHECK( ec == posix::no_posix_equivalent );
  BOOST_CHECK( ec.default_error_condition().value() == posix::no_posix_equivalent );

#else // POSIX

  std::cout << "POSIX tests...\n";
  ec = error_code( EACCES, system_category );
  BOOST_CHECK( ec == error_code( posix::permission_denied, system_category ) );
  BOOST_CHECK( error_code( posix::permission_denied, system_category ) == ec );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( posix::permission_denied == ec );

# ifdef __CYGWIN__

  std::cout << "Cygwin tests...\n";
  ec = cygwin::no_package;
  BOOST_CHECK( ec == cygwin::no_package );
  BOOST_CHECK( ec == error_code( ENOPKG, system_category ) );
  BOOST_CHECK( ec == error_code( cygwin::no_package, system_category ) );
  BOOST_CHECK( ec == posix::no_posix_equivalent );

# elif defined(linux) || defined(__linux) || defined(__linux__)

  std::cout << "Linux tests...\n";
  ec = lnx::dot_dot_error;
  BOOST_CHECK( ec == lnx::dot_dot_error );
  BOOST_CHECK( ec == error_code( EDOTDOT, system_category ) );
  BOOST_CHECK( ec == error_code( lnx::dot_dot_error, system_category ) );
  BOOST_CHECK( ec == posix::no_posix_equivalent );

# endif

#endif
  
  return 0;
}


