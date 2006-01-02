//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <memory>
#include <vector>
#include <deque>
#include <iostream>
#include <functional>

#include <boost/shmem/named_shared_object.hpp>
#include <boost/shmem/containers/deque.hpp>
#include <boost/shmem/indexes/flat_map_index.hpp>
#include "printcontainer.hpp"

/*****************************************************************/
/*                                                               */
/*  This example repeats the same operations with std::deque and */
/*  shmem_deque using the node allocator                         */
/*  and compares the values of both containers                   */
/*                                                               */
/*****************************************************************/

//Explicit instantiation to detect compilation errors
template class boost::shmem::deque<int, std::allocator<int> >;

using namespace boost::shmem;

//Customize named_shared_object class
typedef basic_named_shared_object
   <char,
    simple_seq_fit<shared_mutex_family>,
    flat_map_index
   > my_named_shared_object;

//Alias allocator type
typedef allocator<int, my_named_shared_object::segment_manager>
   shmem_node_allocator_t;

//Alias deque types
typedef deque<int, shmem_node_allocator_t>         MyShmDeque;
typedef std::deque<int>                            MyStdDeque;

//template class deque<int, shmem_node_allocator_t>;

//Function to check if both sets are equal
bool CheckEqual(MyShmDeque *shmdeque, MyStdDeque *stddeque)
{
   if(shmdeque->size() != stddeque->size())
      return false;
   return std::equal(shmdeque->begin(), shmdeque->end(), stddeque->begin());
}

int main ()
{
   const int memsize = 655360;
   const char *const shMemName = "/MySharedMemory";
   const int max = 100;

   //Named new capable shared mem allocator
   my_named_shared_object segment;

   //Create shared memory
   segment.create(shMemName, memsize);

   segment.reserve_named_objects(100);

   //Shared memory allocator must be always be initialized
   //since it has no default constructor
   MyShmDeque *shmdeque = segment.construct<MyShmDeque>("MyShmDeque")
                           (segment.get_segment_manager());

   MyStdDeque *stddeque = new MyStdDeque;

   int i, j;
   for(i = 0; i < max; ++i){
      shmdeque->insert(shmdeque->end(), i);
      stddeque->insert(stddeque->end(), i);
   }
   if(!CheckEqual(shmdeque, stddeque)) return -1;

   MyShmDeque::iterator it;
   MyShmDeque::const_iterator cit = it;

   shmdeque->erase(shmdeque->begin()++);
   stddeque->erase(stddeque->begin()++);
   if(!CheckEqual(shmdeque, stddeque)) return -1;

   shmdeque->erase(shmdeque->begin());
   stddeque->erase(stddeque->begin());
   if(!CheckEqual(shmdeque, stddeque)) return -1;

   std::vector<int> aux_vect;
   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   aux_vect.assign(50, -1);
   shmdeque->insert(shmdeque->end(), aux_vect.begin(), aux_vect.end());
   stddeque->insert(stddeque->end(), aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmdeque, stddeque)) return -1;
   #endif

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   for(i = 0, j = static_cast<int>(shmdeque->size()); i < j; ++i){
      shmdeque->erase(shmdeque->begin());
      stddeque->erase(stddeque->begin());
   }
   if(!CheckEqual(shmdeque, stddeque)) return -1;
   #endif

   #if !BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
   shmdeque->insert(shmdeque->begin(), aux_vect.begin(), aux_vect.end());
   shmdeque->insert(shmdeque->begin(), aux_vect.begin(), aux_vect.end());
   stddeque->insert(stddeque->begin(), aux_vect.begin(), aux_vect.end());
   stddeque->insert(stddeque->begin(), aux_vect.begin(), aux_vect.end());
   if(!CheckEqual(shmdeque, stddeque)) return -1;
   #endif

   shmdeque->erase(shmdeque->begin());
   stddeque->erase(stddeque->begin());

   if(!CheckEqual(shmdeque, stddeque)) return -1;

   for(i = 0; i < max; ++i){
      shmdeque->insert(shmdeque->begin(), i);
      stddeque->insert(stddeque->begin(), i);
   }
   if(!CheckEqual(shmdeque, stddeque)) return -1;

   segment.destroy<MyShmDeque>("MyShmDeque");
   delete stddeque;

   return 0;
}




