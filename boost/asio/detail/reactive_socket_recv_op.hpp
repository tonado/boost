//
// detail/reactive_socket_recv_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_REACTIVE_SOCKET_RECV_OP_HPP
#define BOOST_ASIO_DETAIL_REACTIVE_SOCKET_RECV_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/asio/detail/bind_handler.hpp>
#include <boost/asio/detail/buffer_sequence_adapter.hpp>
#include <boost/asio/detail/fenced_block.hpp>
#include <boost/asio/detail/reactor_op.hpp>
#include <boost/asio/detail/socket_ops.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace detail {

template <typename MutableBufferSequence>
class reactive_socket_recv_op_base : public reactor_op
{
public:
  reactive_socket_recv_op_base(socket_type socket,
      socket_ops::state_type state, const MutableBufferSequence& buffers,
      socket_base::message_flags flags, func_type complete_func)
    : reactor_op(&reactive_socket_recv_op_base::do_perform, complete_func),
      socket_(socket),
      state_(state),
      buffers_(buffers),
      flags_(flags)
  {
  }

  static bool do_perform(reactor_op* base)
  {
    reactive_socket_recv_op_base* o(
        static_cast<reactive_socket_recv_op_base*>(base));

    buffer_sequence_adapter<boost::asio::mutable_buffer,
        MutableBufferSequence> bufs(o->buffers_);

    return socket_ops::non_blocking_recv(o->socket_,
        bufs.buffers(), bufs.count(), o->flags_,
        (o->state_ & socket_ops::stream_oriented),
        o->ec_, o->bytes_transferred_);
  }

private:
  socket_type socket_;
  socket_ops::state_type state_;
  MutableBufferSequence buffers_;
  socket_base::message_flags flags_;
};

template <typename MutableBufferSequence, typename Handler>
class reactive_socket_recv_op :
  public reactive_socket_recv_op_base<MutableBufferSequence>
{
public:
  BOOST_ASIO_DEFINE_HANDLER_PTR(reactive_socket_recv_op);

  reactive_socket_recv_op(socket_type socket,
      socket_ops::state_type state, const MutableBufferSequence& buffers,
      socket_base::message_flags flags, Handler handler)
    : reactive_socket_recv_op_base<MutableBufferSequence>(socket, state,
        buffers, flags, &reactive_socket_recv_op::do_complete),
      handler_(handler)
  {
  }

  static void do_complete(io_service_impl* owner, operation* base,
      boost::system::error_code /*ec*/, std::size_t /*bytes_transferred*/)
  {
    // Take ownership of the handler object.
    reactive_socket_recv_op* o(static_cast<reactive_socket_recv_op*>(base));
    ptr p = { boost::addressof(o->handler_), o, o };

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder2<Handler, boost::system::error_code, std::size_t>
      handler(o->handler_, o->ec_, o->bytes_transferred_);
    p.h = boost::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      boost::asio::detail::fenced_block b;
      boost_asio_handler_invoke_helpers::invoke(handler, handler.handler_);
    }
  }

private:
  Handler handler_;
};

} // namespace detail
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_REACTIVE_SOCKET_RECV_OP_HPP
