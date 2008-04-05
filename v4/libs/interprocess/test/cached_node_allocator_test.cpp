//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/allocators/cached_node_allocator.hpp>
#include "print_container.hpp"
#include "dummy_test_allocator.hpp"
#include "movable_int.hpp"
#include "list_test.hpp"
#include "vector_test.hpp"

using namespace boost::interprocess;

//Alias a integer node allocator type
typedef cached_node_allocator
   <int, managed_shared_memory::segment_manager>
   cached_node_allocator_t;
typedef detail::cached_node_allocator_v1
   <int, managed_shared_memory::segment_manager>
   cached_node_allocator_v1_t;

//Explicit instantiations to catch compilation errors
template class cached_node_allocator<int, managed_shared_memory::segment_manager>;
template class detail::cached_node_allocator_v1<int, managed_shared_memory::segment_manager>;

//Alias list types
typedef list<int, cached_node_allocator_t>    MyShmList;
typedef list<int, cached_node_allocator_v1_t> MyShmListV1;

//Alias vector types
typedef vector<int, cached_node_allocator_t>    MyShmVector;
typedef vector<int, cached_node_allocator_v1_t> MyShmVectorV1;

int main ()
{
   if(test::list_test<managed_shared_memory, MyShmList, true>())
      return 1;
   if(test::list_test<managed_shared_memory, MyShmListV1, true>())
      return 1;
   if(test::vector_test<managed_shared_memory, MyShmVector>())
      return 1;
   if(test::vector_test<managed_shared_memory, MyShmVectorV1>())
      return 1;
   return 0;
}

#include <boost/interprocess/detail/config_end.hpp>
