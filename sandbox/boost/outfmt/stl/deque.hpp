// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_Deque_HPP
#define BOOST_IOFM_STL_Deque_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <deque>

   // type deduction:

   namespace boost { namespace io { namespace detail
   {
      BOOST_IO_CLASSIFY_TYPE_2( std::deque, boost::io::seq_container_type );
   }}}

   // I/O support:

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const std::deque< T, Allocator >     & dq
      )
      {
         return( os << boost::io::formatob( dq, boost::io::containerfmt()));
      }

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         std::deque< T, Allocator >           & dq
      )
      {
         return( is >> boost::io::formatob( dq, boost::io::containerfmt()));
      }
#  else
      template< typename T, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::deque< T, Allocator > & dq
      )
      {
         return( os << boost::io::formatob( dq, boost::io::containerfmt()));
      }

      template< typename T, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::deque< T, Allocator > & dq
      )
      {
         return( is >> boost::io::formatob( dq, boost::io::containerfmt()));
      }
#  endif
#endif
