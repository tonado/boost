//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Copyright (C) 2011 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <boost/thread/thread.hpp>

// class thread

// void detach();

#include <boost/thread/thread.hpp>
#include <new>
#include <cstdlib>
#include <cassert>
#include <boost/detail/lightweight_test.hpp>

class G
{
  int alive_;
public:
  static int n_alive;
  static bool op_run;

  G() :
    alive_(1)
  {
    ++n_alive;
  }
  G(const G& g) :
    alive_(g.alive_)
  {
    ++n_alive;
  }
  ~G()
  {
    alive_ = 0;
    --n_alive;
  }

  void operator()()
  {
    BOOST_TEST(alive_ == 1);
    BOOST_TEST(n_alive == 1);
    op_run = true;
  }
};

int G::n_alive = 0;
bool G::op_run = false;

int main()
{
  {
    boost::thread t0( (G()));
    BOOST_TEST(t0.joinable());
    t0.detach();
    BOOST_TEST(!t0.joinable());
#if defined BOOST_THREAD_USES_CHRONO
    boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
#endif
    BOOST_TEST(G::op_run);
    BOOST_TEST(G::n_alive == 0);
  }
  return boost::report_errors();
}

