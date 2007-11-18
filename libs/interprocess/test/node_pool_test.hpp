//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/interprocess/smart_ptr/deleter.hpp>
#include <boost/interprocess/detail/type_traits.hpp>
#include <vector>
#include <cstddef>
#include <string>
#include "get_process_id_name.hpp"

namespace boost {
namespace interprocess {
namespace test {

template <class NodePool>
struct test_node_pool
{
   static bool allocate_then_deallocate(NodePool &pool);
   static bool deallocate_free_chunks(NodePool &pool);
};

template <class NodePool>
bool test_node_pool<NodePool>::allocate_then_deallocate(NodePool &pool)
{
   const std::size_t num_alloc = 1 + 3*pool.get_real_num_node();

   std::vector<void*> nodes;

   //Precondition, the pool must be empty
   if(0 != pool.num_free_nodes()){
      return false;
   }

   //First allocate nodes
   for(std::size_t i = 0; i < num_alloc; ++i){
      nodes.push_back(pool.allocate(1));
   }

   //Check that the free count is correct
   if((pool.get_real_num_node() - 1) != pool.num_free_nodes()){
      return false;
   }
   
   //Now deallocate all and check again
   for(std::size_t i = 0; i < num_alloc; ++i){
       pool.deallocate(nodes[i], 1);
   }

   //Check that the free count is correct
   if(4*pool.get_real_num_node() != pool.num_free_nodes()){
      return false;
   }
   
   pool.deallocate_free_chunks();

   if(0 != pool.num_free_nodes()){
      return false;
   }

   return true;
}

template <class NodePool>
bool test_node_pool<NodePool>::deallocate_free_chunks(NodePool &pool)
{
   const std::size_t max_chunks        = 10;
   const std::size_t max_nodes         = max_chunks*pool.get_real_num_node();
   const std::size_t nodes_per_chunk   = pool.get_real_num_node();

   std::vector<void*> nodes;

   //Precondition, the pool must be empty
   if(0 != pool.num_free_nodes()){
      return false;
   }

   //First allocate nodes
   for(std::size_t i = 0; i < max_nodes; ++i){
      nodes.push_back(pool.allocate(1));
   }

   //Check that the free count is correct
   if(0 != pool.num_free_nodes()){
      return false;
   }
   
   //Now deallocate one of each chunk per iteration
   for(std::size_t node_i = 0; node_i < nodes_per_chunk; ++node_i){
      //Deallocate a node per chunk
      for(std::size_t i = 0; i < max_chunks; ++i){
         pool.deallocate(nodes[i*nodes_per_chunk + node_i], 1);
      }

      //Check that the free count is correct
      if(max_chunks*(node_i+1) != pool.num_free_nodes()){
         return false;
      }
      
      //Now try to deallocate free chunks
      pool.deallocate_free_chunks();

      //Until we don't deallocate the last node of every chunk
      //no node should be deallocated
      if(node_i != (nodes_per_chunk - 1)){
         if(max_chunks*(node_i+1) != pool.num_free_nodes()){
            return false;
         }
      }
      else{
         //If this is the last iteration, all the memory should
         //have been deallocated.
         if(0 != pool.num_free_nodes()){
            return false;
         }
      }
   }

   return true;
}

template<class node_pool_t>
bool test_all_node_pool()
{
   using namespace boost::interprocess;
   typedef managed_shared_memory::segment_manager segment_manager;

   typedef boost::interprocess::test::test_node_pool<node_pool_t> test_node_pool_t;
   shared_memory_object::remove(test::get_process_id_name());
   {
      managed_shared_memory shm(create_only, test::get_process_id_name(), 16*1024);

      typedef deleter<node_pool_t, segment_manager> deleter_t;
      typedef unique_ptr<node_pool_t, deleter_t> unique_ptr_t;

      //Delete the pool when the tests end
      unique_ptr_t p
         (shm.construct<node_pool_t>(anonymous_instance)(shm.get_segment_manager())
         ,deleter_t(shm.get_segment_manager()));

      //Now call each test
      if(!test_node_pool_t::allocate_then_deallocate(*p))
         return false;
      if(!test_node_pool_t::deallocate_free_chunks(*p))
         return false;
   }
   shared_memory_object::remove(test::get_process_id_name());
   return true;
}

}  //namespace test {
}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>
