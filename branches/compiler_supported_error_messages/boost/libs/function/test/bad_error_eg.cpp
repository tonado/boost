// Boost.Function library

// Copyright (C) 2001 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include <boost/function.hpp>

using namespace boost;

class X {};

int foo(X*);
X* bar(double);

int test_main(int, char* [])
{
  function<int, float> f(&foo);
  f = &bar;
  

  BOOST_CRITICAL_ERROR("This should not have compiled.");

  return 0;
}
