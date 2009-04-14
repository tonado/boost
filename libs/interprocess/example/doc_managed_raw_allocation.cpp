//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
//[doc_managed_raw_allocation
#include <boost/interprocess/managed_shared_memory.hpp>

int main()
{
   using namespace boost::interprocess;

   //Remove shared memory on construction and destruction
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;

   //Managed memory segment that allocates portions of a shared memory
   //segment with the default management algorithm
   managed_shared_memory managed_shm(create_only, "MySharedMemory", 65536);

   //Allocate 100 bytes of memory from segment, throwing version
   void *ptr = managed_shm.allocate(100);

   //Deallocate it
   managed_shm.deallocate(ptr);

   //Non throwing version
   ptr = managed_shm.allocate(100, std::nothrow);

   //Deallocate it
   managed_shm.deallocate(ptr);
   return 0;
}
//]
#include <boost/interprocess/detail/config_end.hpp>
