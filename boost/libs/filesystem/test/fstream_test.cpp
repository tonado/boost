//  fstream_test.cpp  --------------------------------------------------------//

//  Copyright Beman Dawes 2002.
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <string>
#include <iostream>
#include <cstdio> // for std::remove

#ifndef BOOST_FILESYSTEM_NARROW_ONLY
#  include "lpath.hpp"
#endif

namespace fs = boost::filesystem;

#include <boost/config.hpp>
#ifdef BOOST_NO_STDC_NAMESPACE
  namespace std { using ::remove; }
#endif

#include <boost/test/minimal.hpp>

namespace
{
  template< class Path >
  void test( const Path & p )
  {
#  if !BOOST_WORKAROUND( BOOST_MSVC, <= 1200 ) // VC++ 6.0 can't handle open
    { 
      std::cout << " in test 1\n";
      fs::filebuf fb;
      fb.open( p, std::ios_base::in );
      BOOST_CHECK( fb.is_open() == fs::exists( p ) );
    }
    {
      std::cout << " in test 2\n";
      fs::filebuf fb1;
      fb1.open( p, std::ios_base::out );
      BOOST_CHECK( fb1.is_open() );
    }
    {
      std::cout << " in test 3\n";
      fs::filebuf fb2;
      fb2.open( p, std::ios_base::in );
      BOOST_CHECK( fb2.is_open() );
    }
#  else
    std::cout << "<note>\n";
    std::cout <<
      "VC++6.0 does not support boost::filesystem open()\n";
#  endif
    {
      std::cout << " in test 4\n";
      fs::ifstream tfs( p );
      BOOST_CHECK( tfs.is_open() );
    }
    {
      std::cout << " in test 5\n";
      fs::ifstream tfs( p, std::ios_base::in );
      BOOST_CHECK( tfs.is_open() );
    }
#  if !BOOST_WORKAROUND( BOOST_MSVC, <= 1200 ) // VC++ 6.0 can't handle open
    {
      std::cout << " in test 6\n";
      fs::ifstream tfs;
      tfs.open( p );
      BOOST_CHECK( tfs.is_open() );
    }
    {
      std::cout << " in test 7\n";
      fs::ifstream tfs;
      tfs.open( p, std::ios_base::in );
      BOOST_CHECK( tfs.is_open() );
    }
#  endif
    {
      std::cout << " in test 8\n";
      fs::ofstream tfs( p );
      BOOST_CHECK( tfs.is_open() );
    }
    {
      std::cout << " in test 9\n";
      fs::ofstream tfs( p, std::ios_base::out );
      BOOST_CHECK( tfs.is_open() );
    }
#  if !BOOST_WORKAROUND( BOOST_MSVC, <= 1200 ) // VC++ 6.0 can't handle open
    {
      std::cout << " in test 10\n";
      fs::ofstream tfs;
      tfs.open( p );
      BOOST_CHECK( tfs.is_open() );
    }
    {
      std::cout << " in test 11\n";
      fs::ofstream tfs;
      tfs.open( p, std::ios_base::out );
      BOOST_CHECK( tfs.is_open() );
    }
# endif
    {
      std::cout << " in test 12\n";
      fs::fstream tfs( p );
      BOOST_CHECK( tfs.is_open() );
    }
    {
      std::cout << " in test 13\n";
      fs::fstream tfs( p, std::ios_base::in|std::ios_base::out );
      BOOST_CHECK( tfs.is_open() );
    }
#  if !BOOST_WORKAROUND( BOOST_MSVC, <= 1200 ) // VC++ 6.0 can't handle open
    {
      std::cout << " in test 14\n";
      fs::fstream tfs;
      tfs.open( p );
      BOOST_CHECK( tfs.is_open() );
    }
    {
      std::cout << " in test 15\n";
      fs::fstream tfs;
      tfs.open( p, std::ios_base::in|std::ios_base::out );
      BOOST_CHECK( tfs.is_open() );
    }
#  endif
  } // test
} // unnamed namespace

int test_main( int, char*[] )
{
  std::cout << "path tests:\n";
  test( fs::path( "fstream_test_foo" ) );

#ifndef BOOST_FILESYSTEM_NARROW_ONLY

  std::cout << "\nwpath tests:\n";
  test( fs::wpath( L"fstream_test_\x2780" ) );
  const long lname[] = { 'f', 's', 'r', 'e', 'a', 'm', '_', 't', 'e', 's',
    't', '_', 'l', 'p', 'a', 't', 'h', 0 };
  std::cout << "\nlpath tests:\n";
  test( user::lpath( lname ) );

#endif

  return 0;
}
