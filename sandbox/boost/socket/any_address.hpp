// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef BOOST_SOCKET_ANY_ADDRESS_HPP
#define BOOST_SOCKET_ANY_ADDRESS_HPP 1

#include "boost/socket/config.hpp"
#include "boost/socket/impl/address_storage.hpp"

#include <utility>
#include <string>

namespace boost
{
  namespace socket
  {
    class any_address
    {
    public:
      any_address(family_t family, void* addr, std::size_t size);
      family_t family() const;
      std::string to_string() const;
      std::pair<void*,size_t> representation();
      std::pair<const void*,size_t> representation() const;
      bool operator < (const any_address& addr) const;
      bool operator == (const any_address& addr) const;
      bool operator != (const any_address& addr) const;

    private:
      family_t m_family;
      impl::address_storage m_address;
      std::size_t m_size;
    };
  }
}

#endif
