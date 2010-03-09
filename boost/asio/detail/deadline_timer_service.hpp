//
// deadline_timer_service.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_DEADLINE_TIMER_SERVICE_HPP
#define BOOST_ASIO_DETAIL_DEADLINE_TIMER_SERVICE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/push_options.hpp>

#include <boost/asio/detail/push_options.hpp>
#include <cstddef>
#include <boost/config.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/asio/detail/pop_options.hpp>

#include <boost/asio/error.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/detail/bind_handler.hpp>
#include <boost/asio/detail/fenced_block.hpp>
#include <boost/asio/detail/noncopyable.hpp>
#include <boost/asio/detail/socket_ops.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/detail/timer_op.hpp>
#include <boost/asio/detail/timer_queue.hpp>
#include <boost/asio/detail/timer_scheduler.hpp>

namespace boost {
namespace asio {
namespace detail {

template <typename Time_Traits>
class deadline_timer_service
{
public:
  // The time type.
  typedef typename Time_Traits::time_type time_type;

  // The duration type.
  typedef typename Time_Traits::duration_type duration_type;

  // The implementation type of the timer. This type is dependent on the
  // underlying implementation of the timer service.
  struct implementation_type
    : private boost::asio::detail::noncopyable
  {
    time_type expiry;
    bool might_have_pending_waits;
  };

  // Constructor.
  deadline_timer_service(boost::asio::io_service& io_service)
    : scheduler_(boost::asio::use_service<timer_scheduler>(io_service))
  {
    scheduler_.init_task();
    scheduler_.add_timer_queue(timer_queue_);
  }

  // Destructor.
  ~deadline_timer_service()
  {
    scheduler_.remove_timer_queue(timer_queue_);
  }

  // Destroy all user-defined handler objects owned by the service.
  void shutdown_service()
  {
  }

  // Construct a new timer implementation.
  void construct(implementation_type& impl)
  {
    impl.expiry = time_type();
    impl.might_have_pending_waits = false;
  }

  // Destroy a timer implementation.
  void destroy(implementation_type& impl)
  {
    boost::system::error_code ec;
    cancel(impl, ec);
  }

  // Cancel any asynchronous wait operations associated with the timer.
  std::size_t cancel(implementation_type& impl, boost::system::error_code& ec)
  {
    if (!impl.might_have_pending_waits)
    {
      ec = boost::system::error_code();
      return 0;
    }
    std::size_t count = scheduler_.cancel_timer(timer_queue_, &impl);
    impl.might_have_pending_waits = false;
    ec = boost::system::error_code();
    return count;
  }

  // Get the expiry time for the timer as an absolute time.
  time_type expires_at(const implementation_type& impl) const
  {
    return impl.expiry;
  }

  // Set the expiry time for the timer as an absolute time.
  std::size_t expires_at(implementation_type& impl,
      const time_type& expiry_time, boost::system::error_code& ec)
  {
    std::size_t count = cancel(impl, ec);
    impl.expiry = expiry_time;
    ec = boost::system::error_code();
    return count;
  }

  // Get the expiry time for the timer relative to now.
  duration_type expires_from_now(const implementation_type& impl) const
  {
    return Time_Traits::subtract(expires_at(impl), Time_Traits::now());
  }

  // Set the expiry time for the timer relative to now.
  std::size_t expires_from_now(implementation_type& impl,
      const duration_type& expiry_time, boost::system::error_code& ec)
  {
    return expires_at(impl,
        Time_Traits::add(Time_Traits::now(), expiry_time), ec);
  }

  // Perform a blocking wait on the timer.
  void wait(implementation_type& impl, boost::system::error_code& ec)
  {
    time_type now = Time_Traits::now();
    while (Time_Traits::less_than(now, impl.expiry))
    {
      boost::posix_time::time_duration timeout =
        Time_Traits::to_posix_duration(Time_Traits::subtract(impl.expiry, now));
      ::timeval tv;
      tv.tv_sec = timeout.total_seconds();
      tv.tv_usec = timeout.total_microseconds() % 1000000;
      boost::system::error_code ec;
      socket_ops::select(0, 0, 0, 0, &tv, ec);
      now = Time_Traits::now();
    }
    ec = boost::system::error_code();
  }

  template <typename Handler>
  class wait_handler : public timer_op
  {
  public:
    wait_handler(Handler handler)
      : timer_op(&wait_handler::do_complete),
        handler_(handler)
    {
    }

    static void do_complete(io_service_impl* owner, operation* base,
        boost::system::error_code /*ec*/, std::size_t /*bytes_transferred*/)
    {
      // Take ownership of the handler object.
      wait_handler* h(static_cast<wait_handler*>(base));
      typedef handler_alloc_traits<Handler, wait_handler> alloc_traits;
      handler_ptr<alloc_traits> ptr(h->handler_, h);

      // Make the upcall if required.
      if (owner)
      {
        // Make a copy of the handler so that the memory can be deallocated
        // before the upcall is made. Even if we're not about to make an
        // upcall, a sub-object of the handler may be the true owner of the
        // memory associated with the handler. Consequently, a local copy of
        // the handler is required to ensure that any owning sub-object remains
        // valid until after we have deallocated the memory here.
        detail::binder1<Handler, boost::system::error_code>
          handler(h->handler_, h->ec_);
        ptr.reset();
        boost::asio::detail::fenced_block b;
        boost_asio_handler_invoke_helpers::invoke(handler, handler);
      }
    }

  private:
    Handler handler_;
  };

  // Start an asynchronous wait on the timer.
  template <typename Handler>
  void async_wait(implementation_type& impl, Handler handler)
  {
    // Allocate and construct an operation to wrap the handler.
    typedef wait_handler<Handler> value_type;
    typedef handler_alloc_traits<Handler, value_type> alloc_traits;
    raw_handler_ptr<alloc_traits> raw_ptr(handler);
    handler_ptr<alloc_traits> ptr(raw_ptr, handler);

    impl.might_have_pending_waits = true;

    scheduler_.schedule_timer(timer_queue_, impl.expiry, ptr.get(), &impl);
    ptr.release();
  }

private:
  // The queue of timers.
  timer_queue<Time_Traits> timer_queue_;

  // The object that schedules and executes timers. Usually a reactor.
  timer_scheduler& scheduler_;
};

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_DEADLINE_TIMER_SERVICE_HPP
