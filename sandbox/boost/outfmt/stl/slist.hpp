// (C) Copyright 2004: Reece H. Dunn

#ifndef BOOST_IOFM_STL_SList_HPP
#define BOOST_IOFM_STL_SList_HPP
#  include <boost/outfmt/formatob.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  if !defined(BOOST_HAS_SLIST)
#     error STL implementation does not support the <slist> extension.
#  endif

#  include <slist>

   // type deduction:

   BOOST_IO_CLASSIFY_TYPE( 2, BOOST_IOFM_STDEXT::slist, boost::io::seq_container_type )

   // I/O support:

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_ostream< CharT, TraitsT > & operator<<
      (
         std::basic_ostream< CharT, TraitsT > & os,
         const BOOST_IOFM_STDEXT::slist< T, Allocator >  & sl
      )
      {
         return( os << boost::io::formatob( sl, boost::io::containerfmt()));
      }

      template< typename CharT, class TraitsT, typename T, class Allocator >
      inline std::basic_istream< CharT, TraitsT > & operator>>
      (
         std::basic_istream< CharT, TraitsT > & is,
         BOOST_IOFM_STDEXT::slist< T, Allocator > & sl
      )
      {
         return( is >> boost::io::formatob( sl, boost::io::containerfmt()));
      }
#  else
      template< typename T, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const BOOST_IOFM_STDEXT::slist< T, Allocator > & sl
      )
      {
         return( os << boost::io::formatob( sl, boost::io::containerfmt()));
      }

      template< typename T, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         BOOST_IOFM_STDEXT::slist< T, Allocator > & sl
      )
      {
         return( is >> boost::io::formatob( sl, boost::io::containerfmt()));
      }
#  endif
#endif