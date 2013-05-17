//
// spawn.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_SPAWN_HPP
#define BOOST_ASIO_SPAWN_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <boost/asio/detail/weak_ptr.hpp>
#include <boost/asio/detail/wrapped_handler.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {

template <typename Handler>
class basic_yield_context
{
public:
  basic_yield_context(
      const detail::weak_ptr<boost::coroutines::coroutine<void()> >& coro,
      boost::coroutines::coroutine<void()>::caller_type& ca, Handler& handler)
    : coro_(coro),
      ca_(ca),
      handler_(handler),
      ec_(0)
  {
  }

  basic_yield_context operator[](boost::system::error_code& ec)
  {
    basic_yield_context tmp(*this);
    tmp.ec_ = &ec;
    return tmp;
  }

#if defined(GENERATING_DOCUMENTATION)
private:
#endif // defined(GENERATING_DOCUMENTATION)
  detail::weak_ptr<boost::coroutines::coroutine<void()> > coro_;
  boost::coroutines::coroutine<void()>::caller_type& ca_;
  Handler& handler_;
  boost::system::error_code* ec_;
};

#if defined(GENERATING_DOCUMENTATION)
typedef basic_yield_context<unspecified> yield_context;
#else // defined(GENERATING_DOCUMENTATION)
typedef basic_yield_context<
  detail::wrapped_handler<
    io_service::strand, void(*)(),
    detail::is_continuation_if_running> > yield_context;
#endif // defined(GENERATING_DOCUMENTATION)

template <typename Handler, typename Function>
void spawn(BOOST_ASIO_MOVE_ARG(Handler) handler,
    BOOST_ASIO_MOVE_ARG(Function) function,
    const boost::coroutines::attributes& attributes
      = boost::coroutines::attributes());

template <typename Handler, typename Function>
void spawn(basic_yield_context<Handler> ctx,
    BOOST_ASIO_MOVE_ARG(Function) function,
    const boost::coroutines::attributes& attributes
      = boost::coroutines::attributes());

template <typename Function>
void spawn(boost::asio::io_service::strand strand,
    BOOST_ASIO_MOVE_ARG(Function) function,
    const boost::coroutines::attributes& attributes
      = boost::coroutines::attributes());

template <typename Function>
void spawn(boost::asio::io_service& io_service,
    BOOST_ASIO_MOVE_ARG(Function) function,
    const boost::coroutines::attributes& attributes
      = boost::coroutines::attributes());

} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#include <boost/asio/impl/spawn.hpp>

#endif // BOOST_ASIO_SPAWN_HPP
