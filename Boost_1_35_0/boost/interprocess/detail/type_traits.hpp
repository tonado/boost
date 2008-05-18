//////////////////////////////////////////////////////////////////////////////
// (C) Copyright John Maddock 2000.
// (C) Copyright Ion Gaztanaga 2005-2008.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
// The alignment_of implementation comes from John Maddock's boost::alignment_of code
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_DETAIL_TYPE_TRAITS_HPP
#define BOOST_INTERPROCESS_DETAIL_TYPE_TRAITS_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>

namespace boost {
namespace interprocess { 
namespace detail {

struct nat{};

//boost::alignment_of yields to 10K lines of preprocessed code, so we
//need an alternative
template <typename T> struct alignment_of;

template <typename T>
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{
    enum{   value = A < S ? A : S  };
};

template< typename T >
struct alignment_of
{
   enum{ value = alignment_logic
            < sizeof(alignment_of_hack<T>) - sizeof(T)
            , sizeof(T)>::value   };
};

//This is not standard, but should work with all compilers
union max_align
{
   char        char_;
   short       short_;
   int         int_;
   long        long_;
   #ifdef BOOST_HAS_LONG_LONG
   long long   long_long_;
   #endif
   float       float_;
   double      double_;
   long double long_double_;
   void *      void_ptr_;
};

template<class T>
struct remove_reference
{
   typedef T type;
};

template<class T>
struct remove_reference<T&>
{
   typedef T type;
};

template<class T>
struct is_reference
{
   enum {  value = false   };
};

template<class T>
struct is_reference<T&>
{
   enum {  value = true   };
};

template<class T>
struct is_pointer
{
   enum {  value = false   };
};

template<class T>
struct is_pointer<T*>
{
   enum {  value = true   };
};

template <typename T>
struct add_reference
{
    typedef T& type;
};

template<class T>
struct add_reference<T&>
{
    typedef T& type;
};

template<>
struct add_reference<void>
{
    typedef nat& type;
};

template<>
struct add_reference<const void>
{
    typedef const nat& type;
};
template <typename T, typename U>
struct is_same
{
   typedef char yes_type;
   struct no_type
   {
      char padding[8];
   };

   template <typename V>
   static yes_type is_same_tester(V*, V*);
   static no_type is_same_tester(...);

   static T *t;
   static U *u;

   static const bool value = sizeof(yes_type) == sizeof(is_same_tester(t,u));
};
/*
template <template<class P> typename T, template<typename P2> typename U>
struct is_same
{
   typedef char yes_type;
   struct no_type
   {
      char padding[8];
   };

   template <template<class P3> typename V>
   static yes_type is_same_tester(V<P3>*, V<P3>*);
   static no_type is_same_tester(...);

   static T<int> *t;
   static U<int> *u;

   static const bool value = sizeof(yes_type) == sizeof(is_same_tester(t,u));
};*/
/*
template< typename T >
struct is_pointer_impl
{
   static const bool value =
        (::boost::type_traits::ice_and<
        ::boost::detail::is_pointer_helper<typename remove_cv<T>::type>::value
            , ::boost::type_traits::ice_not<
                ::boost::is_member_pointer<T>::value
                >::value
            >::value)
        );
};*/

} // namespace detail
}  //namespace interprocess { 
}  //namespace boost {

#endif   //#ifndef BOOST_INTERPROCESS_DETAIL_TYPE_TRAITS_HPP

#include <boost/interprocess/detail/config_end.hpp>

