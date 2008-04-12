// Copyright (C) 2000, 2001 Stephen Cleary (shammah@voyager.net)
//
// This file can be redistributed and/or modified under the terms found
//  in "copyright.html"
// This software and its documentation is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SINGLETON_POOL_HPP
#define BOOST_SINGLETON_POOL_HPP

#include <boost/pool/singleton_pool_fwd.hpp>

// boost::pool
#include <boost/pool/pool.hpp>
// boost::details::pool::singleton_default
#include <boost/pool/detail/singleton.hpp>
// boost::details::pool::guard
#include <boost/pool/detail/guard.hpp>

namespace boost {

//
// The singleton_pool class allows other pool interfaces for types of the same
//   size to share the same pool
//
template <typename Tag, unsigned RequestedSize,
    typename UserAllocator,
    typename Mutex,
    unsigned NextSize>
struct singleton_pool
{
  public:
    typedef Tag tag;
    typedef Mutex mutex;
    typedef UserAllocator user_allocator;
    typedef typename pool<UserAllocator>::size_type size_type;
    typedef typename pool<UserAllocator>::difference_type difference_type;

    static const unsigned requested_size = RequestedSize;
    static const unsigned next_size = NextSize;

  private:
    struct pool_type: Mutex
    {
      pool<UserAllocator> p;
      pool_type():p(RequestedSize, NextSize) { }
    };

    typedef details::pool::singleton_default<pool_type> singleton;

    singleton_pool();

  public:
    static void * malloc()
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.malloc();
    }
    static void * ordered_malloc()
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.ordered_malloc();
    }
    static void * ordered_malloc(const size_type n)
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.ordered_malloc(n);
    }
    static bool is_from(void * const ptr)
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.is_from(ptr);
    }
    static void free(void * const ptr)
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      p.p.free(ptr);
    }
    static void ordered_free(void * const ptr)
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      p.p.ordered_free(ptr);
    }
    static void free(void * const ptr, const std::size_t n)
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      p.p.free(ptr, n);
    }
    static void ordered_free(void * const ptr, const size_type n)
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      p.p.ordered_free(ptr, n);
    }
    static bool release_memory()
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.release_memory();
    }
    static bool purge_memory()
    {
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.purge_memory();
    }
};

} // namespace boost

#endif
