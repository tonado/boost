//
// streambuf.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_STREAMBUF_HPP
#define BOOST_ASIO_STREAMBUF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/push_options.hpp>

#include <boost/asio/basic_streambuf.hpp>

#if !defined(BOOST_NO_IOSTREAM)

namespace boost {
namespace asio {

/// Typedef for the typical usage of basic_streambuf.
typedef basic_streambuf<> streambuf;

} // namespace asio
} // namespace boost

#endif // !defined(BOOST_NO_IOSTREAM)

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_STREAMBUF_HPP
