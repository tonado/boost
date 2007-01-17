//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ALGORITHM_BASIC_FOR_BASIC_FOR_BASE_HPP
#define BOOST_ACT_ALGORITHM_BASIC_FOR_BASIC_FOR_BASE_HPP

#include "basic_for_fwd.hpp"

namespace boost
{
namespace act
{

struct basic_for_base
{
  template< typename ExtendedParamsType
          , typename AssignType
          , typename ConditionType
          , typename StepType
          >
  static detail::bound_basic_for< ExtendedParamsType
                                , AssignType
                                , ConditionType
                                , StepType
                                >
  execute( ExtendedParamsType const& extended_params
         , AssignType assign
         , ConditionType condition
         , StepType step
         )
  {
    return detail::make_bound_basic_for
             ( extended_params, assign, condition, step );
  }
};

}
}

#endif
