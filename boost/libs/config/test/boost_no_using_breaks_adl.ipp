//  (C) Copyright John Maddock 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/config for most recent version.

//  MACRO:         BOOST_FUNCTION_SCOPE_USING_DECLARATION_BREAKS_ADL
//  TITLE:         broken ADL
//  DESCRIPTION:   Using declarations break argument dependent lookup
//                 (probably Borland specific), the fix is to use
//                 using namespace whatever; rather than
//                 using whatever::symbol;.


namespace boost
{
   template <class T>
   T* get_pointer(T* p)
   { return p; }

   namespace inner2
   {
      template <class T>
      struct X {};

      template <class T>
      T* get_pointer(X<T>)
      { return 0; }
   }
}

namespace user
{
   template <class T>
   struct Y{};

   template <class T>
   T* get_pointer(user::Y<T>)
   { return 0; }

   template <class T>
   int f(T x) 
   {
      // use this as a workaround:
      //using namespace boost;
      // this statement breaks ADL:
      using boost::get_pointer;    // conforming compilers require
                                   // this one to find the auto_ptr
                                   // and T* overloads
      return get_pointer(x) == 0;
   }
}

namespace boost_function_scope_using_declaration_breaks_adl{

int test()
{
   int i;
   typedef void* pv;
   i = user::f(pv());
   i = user::f(boost::inner2::X<int>());
   return 0;
}

}






