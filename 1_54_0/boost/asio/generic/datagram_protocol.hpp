//
// generic/datagram_protocol.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_GENERIC_DATAGRAM_PROTOCOL_HPP
#define BOOST_ASIO_GENERIC_DATAGRAM_PROTOCOL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#include <typeinfo>
#include <boost/asio/basic_datagram_socket.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/detail/throw_exception.hpp>
#include <boost/asio/generic/basic_endpoint.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace generic {

/// Encapsulates the flags needed for a generic datagram-oriented socket.
/**
 * The boost::asio::generic::datagram_protocol class contains flags necessary
 * for datagram-oriented sockets of any address family and protocol.
 *
 * @par Examples
 * Constructing using a native address family and socket protocol:
 * @code datagram_protocol p(AF_INET, IPPROTO_UDP); @endcode
 * Constructing from a specific protocol type:
 * @code datagram_protocol p(boost::asio::ip::udp::v4()); @endcode
 *
 * @par Thread Safety
 * @e Distinct @e objects: Safe.@n
 * @e Shared @e objects: Safe.
 *
 * @par Concepts:
 * Protocol.
 */
class datagram_protocol
{
public:
  /// Construct a protocol object for a specific address family and protocol.
  datagram_protocol(int address_family, int socket_protocol)
    : family_(address_family),
      protocol_(socket_protocol)
  {
  }

  /// Construct a generic protocol object from a specific protocol.
  /**
   * @throws @c bad_cast Thrown if the source protocol is not datagram-oriented.
   */
  template <typename Protocol>
  datagram_protocol(const Protocol& source_protocol)
    : family_(source_protocol.family()),
      protocol_(source_protocol.protocol())
  {
    if (source_protocol.type() != type())
    {
      std::bad_cast ex;
      boost::asio::detail::throw_exception(ex);
    }
  }

  /// Obtain an identifier for the type of the protocol.
  int type() const
  {
    return SOCK_DGRAM;
  }

  /// Obtain an identifier for the protocol.
  int protocol() const
  {
    return protocol_;
  }

  /// Obtain an identifier for the protocol family.
  int family() const
  {
    return family_;
  }

  /// Compare two protocols for equality.
  friend bool operator==(const datagram_protocol& p1,
      const datagram_protocol& p2)
  {
    return p1.family_ == p2.family_ && p1.protocol_ == p2.protocol_;
  }

  /// Compare two protocols for inequality.
  friend bool operator!=(const datagram_protocol& p1,
      const datagram_protocol& p2)
  {
    return !(p1 == p2);
  }

  /// The type of an endpoint.
  typedef basic_endpoint<datagram_protocol> endpoint;

  /// The generic socket type.
  typedef basic_datagram_socket<datagram_protocol> socket;

private:
  int family_;
  int protocol_;
};

} // namespace generic
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_GENERIC_DATAGRAM_PROTOCOL_HPP
