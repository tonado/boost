// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_STL_IO_OSTREAM_HPP
#define BOOST_IOFM_STL_IO_OSTREAM_HPP
   // std::vector

   template< typename T, class Allocator >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::vector< T, Allocator > & v
   )
   {
      return( os << boost::io::formatob( v, boost::io::containerfmt()));
   }

   template< typename T, class Allocator >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::vector< T, Allocator > & v
   )
   {
      return( is >> boost::io::formatob( v, boost::io::containerfmt()));
   }

   // std::list

   template< typename T, class Allocator >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::list< T, Allocator > & l
   )
   {
      return( os << boost::io::formatob( l, boost::io::containerfmt()));
   }

   template< typename T, class Allocator >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::list< T, Allocator > & l
   )
   {
      return( is >> boost::io::formatob( l, boost::io::containerfmt()));
   }

   // std::deque

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

   // std::map

   template< typename KeyT, typename T, class Cmp, class Allocator >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::map< KeyT, T, Cmp, Allocator > & m
   )
   {
      return
      (
         os << boost::io::formatob
         (
            m,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   template< typename KeyT, typename T, class Cmp, class Allocator >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::map< KeyT, T, Cmp, Allocator > & m
   )
   {
      return
      (
         is >> boost::io::formatob
         (
            m,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   // std::multimap

   template< typename KeyT, typename T, class Cmp, class Allocator >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::multimap< KeyT, T, Cmp, Allocator > & mm
   )
   {
      return
      (
         os << boost::io::formatob
         (
            mm,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   template< typename KeyT, typename T, class Cmp, class Allocator >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::multimap< KeyT, T, Cmp, Allocator > & mm
   )
   {
      return
      (
         is >> boost::io::formatob
         (
            mm,
            boost::io::containerfmt( boost::io::pairfmt())
         )
      );
   }

   // std::set

   template< typename KeyT, class Cmp, class Allocator >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::set< KeyT, Cmp, Allocator > & s
   )
   {
      return( os << boost::io::formatob( s, boost::io::containerfmt()));
   }

   template< typename KeyT, class Cmp, class Allocator >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::set< KeyT, Cmp, Allocator > & s
   )
   {
      return( is >> boost::io::formatob( s, boost::io::containerfmt()));
   }

   // std::multiset

   template< typename KeyT, class Cmp, class Allocator >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::multiset< KeyT, Cmp, Allocator > & ms
   )
   {
      return( os << boost::io::formatob( ms, boost::io::containerfmt()));
   }

   template< typename KeyT, class Cmp, class Allocator >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::multiset< KeyT, Cmp, Allocator > & ms
   )
   {
      return( is >> boost::io::formatob( ms, boost::io::containerfmt()));
   }

   // std::pair

   template< typename T1, typename T2 >
   inline std::ostream & operator<<
   (
      std::ostream & os,
      const std::pair< T1, T2 > & p
   )
   {
      return( os << boost::io::formatob( p, boost::io::pairfmt()));
   }

   template< typename T1, typename T2 >
   inline std::istream & operator>>
   (
      std::istream & is,
      std::pair< T1, T2 > & p
   )
   {
      return( is >> boost::io::formatob( p, boost::io::pairfmt()));
   }

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
      // std::hash_map

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return
         (
            os << boost::io::formatob
            (
               m,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::hash_map< BOOST_IOFM_HASH_MAP_ARG > & m
      )
      {
         return
         (
            is >> boost::io::formatob
            (
               m,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      // std::hash_multimap 

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return
         (
            os << boost::io::formatob
            (
               mm,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      template< BOOST_IOFM_HASH_MAP_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::hash_multimap< BOOST_IOFM_HASH_MAP_ARG > & mm
      )
      {
         return
         (
            is >> boost::io::formatob
            (
               mm,
               boost::io::containerfmt( boost::io::pairfmt())
            )
         );
      }

      // std::hash_set

      template< BOOST_IOFM_HASH_SET_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( os << boost::io::formatob( s, boost::io::containerfmt()));
      }

      template< BOOST_IOFM_HASH_SET_T >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::hash_set< BOOST_IOFM_HASH_SET_ARG > & s
      )
      {
         return( is >> boost::io::formatob( s, boost::io::containerfmt()));
      }

      // std::hash_multiset

      template< BOOST_IOFM_HASH_SET_T >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( os << boost::io::formatob( ms, boost::io::containerfmt()));
      }

      template< BOOST_IOFM_HASH_SET_T >
      inline std::istream & operator>>
      (
         std::istream & os,
         std::hash_multiset< BOOST_IOFM_HASH_SET_ARG > & ms
      )
      {
         return( is >> boost::io::formatob( ms, boost::io::containerfmt()));
      }
#  endif

#  if defined(BOOST_HAS_SLIST) // std::slist
      template< typename T, class Allocator >
      inline std::ostream & operator<<
      (
         std::ostream & os,
         const std::slist< T, Allocator > & sl
      )
      {
         return( os << boost::io::formatob( sl, boost::io::containerfmt()));
      }

      template< typename T, class Allocator >
      inline std::istream & operator>>
      (
         std::istream & is,
         std::slist< T, Allocator > & sl
      )
      {
         return( is >> boost::io::formatob( sl, boost::io::containerfmt()));
      }
#  endif
#endif
