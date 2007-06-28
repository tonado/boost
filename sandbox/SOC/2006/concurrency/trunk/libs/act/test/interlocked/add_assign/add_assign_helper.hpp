/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_ADD_ASSIGN_ADD_ASSIGN_HELPER
#define BOOST_ACT_TEST_INTERLOCKED_ADD_ASSIGN_ADD_ASSIGN_HELPER

#include <boost/test/minimal.hpp>
#include "../integral_additive_helper.hpp"
#include "../operation_result_checker.hpp"
#include <boost/act/interlocked/add_assign.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/preprocessor/cat.hpp>

struct BOOST_PP_CAT( interlocked_add_assign_
                   , BOOST_ACT_TEST_INTERLOCKED_SEMANTICS
                   )
{
  template< typename LeftType, typename RightType >
  typename boost::remove_cv< LeftType >::type
  operator ()( LeftType& left, RightType const& right ) const
  {
    return boost::act::interlocked::add_assign
           < boost::act::interlocked::BOOST_ACT_TEST_INTERLOCKED_SEMANTICS >
           ( left, right );
  }
};

int test_main( int, char *[] )
{
  brute_operation_result_checker( basic_add()
                                , BOOST_PP_CAT( interlocked_add_assign_
                                              , BOOST_ACT_TEST_INTERLOCKED_SEMANTICS
                                              )
                                              ()
                                );

  brute_additive_assign_test( basic_add_assign()
                            , BOOST_PP_CAT( interlocked_add_assign_
                                          , BOOST_ACT_TEST_INTERLOCKED_SEMANTICS
                                          )
                                          ()
                            );

  return 0;
}

#endif
