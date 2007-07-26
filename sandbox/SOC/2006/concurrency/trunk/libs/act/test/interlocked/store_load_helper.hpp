/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_STORE_LOAD_HELPER_HPP
#define BOOST_ACT_TEST_INTERLOCKED_STORE_LOAD_HELPER_HPP

#include <boost/test/minimal.hpp>
#include <boost/act/interlocked/load.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/act/interlocked/store.hpp>
#include <boost/act/interlocked/integer/types.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/foreach.hpp>

#include "random_uint8.hpp"

// Note: Logic of how test are performed:
//
//       Start with a non-volatile variable.
//
//       Create a separate thread, in new thread, set value with
//       interlocked::store and approach barrier (thresh 2).
//       
//       In primary thread, approach barrier, then check value. 
//
//       Repeat similar tests with different threads.
//
//       Test will potentially have both false positives and false negatives,
//       though improper results should hopefully keep to a minimum,
//       particularly in a multicore environment.

template< typename GetType, typename SetType >
void test_store_load_no_threads( GetType get, SetType set )
{
  ::boost::act::interlocked::uint_least8_t test_var = 0;

  BOOST_FOREACH( ::boost::act::interlocked::uint_least8_t val, random_uint8 )
  {
    set( test_var, val );
    BOOST_CHECK( get( test_var ) == val );
  }
}

struct single_thread_basic_set
{
  template< typename VariableType, typename SourceType >
  void operator ()( VariableType& var, SourceType new_val ) const
  {
    var = new_val;
  }
};

struct single_thread_basic_get
{
  template< typename VariableType >
  typename boost::remove_cv< VariableType >::type
  operator ()( VariableType& var ) const
  {
    return var;
  }
};

struct interlocked_store_set
{
  template< typename VariableType, typename SourceType >
  void
  operator ()( VariableType& var, SourceType new_val ) const
  {
    boost::act::interlocked::store( var, new_val );
  }
};

struct interlocked_load_get
{
  template< typename VariableType >
  typename boost::remove_cv< VariableType >::type
  operator ()( VariableType& var ) const
  {
    return boost::act::interlocked::load( var );
  }
};

template< typename VariableType, typename SourceType
        , typename BarrierType, typename SetType
        >
class store_thread_fun
{
public:
  store_thread_fun( VariableType& var_init, SourceType new_val_init
                   , BarrierType& barrier_init, SetType set_init
                   )
    : var_m( var_init ), new_val_m( new_val_init )
    , barrier_m( barrier_init ), set_m( set_init ) {}
public:
  void operator ()() const
  {
    set_m( var_m, new_val_m );
    barrier_m.wait();
  }
private:
  VariableType& var_m,
                new_val_m;
  BarrierType& barrier_m;
  SetType set_m;
};

template< typename VariableType, typename SourceType
        , typename BarrierType, typename SetType
        >
store_thread_fun< VariableType, SourceType, BarrierType, SetType >
make_store_thread_fun( VariableType& var_init, SourceType new_val_init
                      , BarrierType& barrier_init, SetType set_init
                      )
{
  return store_thread_fun< VariableType, SourceType, BarrierType, SetType >
         ( var_init, new_val_init
         , barrier_init, set_init
         );
}

template< typename VariableType, typename SourceType
        , typename BarrierType, typename GetType, typename SetType
        >
void store_in_new_thread( VariableType& var, SourceType new_val
                         , BarrierType& barrier, GetType get, SetType set
                         )
{
  boost::thread thread( make_store_thread_fun( var, new_val, barrier, set ) );
  barrier.wait();
  BOOST_CHECK( get( var ) == new_val );
  thread.join();
}

template< typename BarrierType, typename GetType, typename SetType >
void test_store_load_with_threads( BarrierType& barrier
                                      , GetType get, SetType set
                                      )
{
  typedef boost::act::interlocked::uint_least8_t uint_least8_t;

  uint_least8_t test_var = 0;

  BOOST_FOREACH( uint_least8_t val, random_uint8 )
  {
    store_in_new_thread( test_var, val, barrier, get, set );
  }
}

#endif
