//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_MANAGED_SHARED_MEMORY_HPP
#define BOOST_INTERPROCESS_MANAGED_SHARED_MEMORY_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/detail/managed_memory_impl.hpp>
#include <boost/interprocess/detail/managed_open_or_create_impl.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/creation_tags.hpp>

//!\file
//!Describes a named shared memory object allocation user class. 

namespace boost {

namespace interprocess {

//!A basic shared memory named object creation class. Initializes the 
//!shared memory segment. Inherits all basic functionality from 
//!basic_managed_memory_impl<CharType, AllocationAlgorithm, IndexType>*/
template
      <
         class CharType, 
         class AllocationAlgorithm, 
         template<class IndexConfig> class IndexType
      >
class basic_managed_shared_memory 
   : public detail::basic_managed_memory_impl
      <CharType, AllocationAlgorithm, IndexType
      ,detail::managed_open_or_create_impl<shared_memory_object>::ManagedOpenOrCreateUserOffset>
   , private detail::managed_open_or_create_impl<shared_memory_object>
{
   /// @cond
   public:
   typedef shared_memory_object                       device_type;

   private:
   typedef detail::basic_managed_memory_impl 
      <CharType, AllocationAlgorithm, IndexType,
      detail::managed_open_or_create_impl<shared_memory_object>::ManagedOpenOrCreateUserOffset>   base_t;
   typedef detail::managed_open_or_create_impl
      <shared_memory_object>                       base2_t;

   typedef detail::create_open_func<base_t>        create_open_func_t;

   basic_managed_shared_memory();

   basic_managed_shared_memory *get_this_pointer()
   {  return this;   }
   /// @endcond

   public: //functions

   //!Destroys *this and indicates that the calling process is finished using
   //!the resource. The destructor function will deallocate
   //!any system resources allocated by the system for use by this process for
   //!this resource. The resource can still be opened again calling
   //!the open constructor overload. To erase the resource from the system
   //!use remove().
   ~basic_managed_shared_memory()
   {}

   //!Creates shared memory and creates and places the segment manager. 
   //!This can throw.
   basic_managed_shared_memory(create_only_t create_only, const char *name,
                             std::size_t size, const void *addr = 0)
      : base_t()
      , base2_t(create_only, name, size, read_write, addr, 
                create_open_func_t(get_this_pointer(), detail::DoCreate))
   {}

   //!Creates shared memory and creates and places the segment manager if
   //!segment was not created. If segment was created it connects to the
   //!segment.
   //!This can throw.
   basic_managed_shared_memory (open_or_create_t open_or_create,
                              const char *name, std::size_t size, 
                              const void *addr = 0)
      : base_t()
      , base2_t(open_or_create, name, size, read_write, addr, 
                create_open_func_t(get_this_pointer(), 
                detail::DoOpenOrCreate))
   {}

   //!Connects to a created shared memory and it's the segment manager.
   //!Never throws.
   basic_managed_shared_memory (open_only_t open_only, const char* name, 
                              const void *addr = 0)
      : base_t()
      , base2_t(open_only, name, read_write, addr, 
                create_open_func_t(get_this_pointer(), 
                detail::DoOpen))
   {}

   //!Moves the ownership of "moved"'s managed memory to *this.
   //!Does not throw
   #ifndef BOOST_INTERPROCESS_RVALUE_REFERENCE
   basic_managed_shared_memory
      (detail::moved_object<basic_managed_shared_memory> &moved)
   {  this->swap(moved.get());   }
   #else
   basic_managed_shared_memory(basic_managed_shared_memory &&moved)
   {  this->swap(moved);   }
   #endif

   //!Moves the ownership of "moved"'s managed memory to *this.
   //!Does not throw
   #ifndef BOOST_INTERPROCESS_RVALUE_REFERENCE
   basic_managed_shared_memory &operator=
      (detail::moved_object<basic_managed_shared_memory> &moved)
   {  this->swap(moved.get());   return *this;  }
   #else
   basic_managed_shared_memory &operator=(basic_managed_shared_memory &&moved)
   {  this->swap(moved);   return *this;  }
   #endif

   //!Swaps the ownership of the managed shared memories managed by *this and other.
   //!Never throws.
   void swap(basic_managed_shared_memory &other)
   {
      base_t::swap(other);
      base2_t::swap(other);
   }

   //!Tries to resize the managed shared memory object so that we have
   //!room for more objects. 
   //!
   //!This function is not synchronized so no other thread or process should
   //!be reading or writing the file
   static bool grow(const char *filename, std::size_t extra_bytes)
   {
      return base_t::template grow
         <basic_managed_shared_memory>(filename, extra_bytes);
   }

   //!Tries to resize the managed shared memory to minimized the size of the file.
   //!
   //!This function is not synchronized so no other thread or process should
   //!be reading or writing the file
   static bool shrink_to_fit(const char *filename)
   {
      return base_t::template shrink_to_fit
         <basic_managed_shared_memory>(filename);
   }
};

}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_MANAGED_SHARED_MEMORY_HPP

