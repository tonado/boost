#ifndef BOOST_PREPROCESSOR_LIST_FILTER_HPP
#define BOOST_PREPROCESSOR_LIST_FILTER_HPP

/* Copyright (C) 2001
 * Housemarque Oy
 * http://www.housemarque.com
 *
 * Permission to copy, use, modify, sell and distribute this software is
 * granted provided this copyright notice appears in all copies. This
 * software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 * See http://www.boost.org for most recent version.
 */

#include <boost/preprocessor/list/fold_right.hpp>

/** <p>Expands to a list containing all the elements <code>X</code> of the list
for which <code>PRED(D,DATA,X)</code> is true.</p>

<p>For example,</p>

<pre>
BOOST_PP_LIST_FILTER(BOOST_PP_NOT_EQUAL_D,2,BOOST_PP_TUPLE_TO_LIST(3,(1,2,3)))
</pre>

<p>expands to the same as:</p>

<pre>
BOOST_PP_TUPLE_TO_LIST(2,(1,3))
</pre>

<h3>Test</h3>
<ul>
  <li><a href="../../test/list_test.cpp">list_test.cpp</a></li>
</ul>
*/
#define BOOST_PP_LIST_FILTER(PRED,DATA,LIST) BOOST_PP_LIST_FILTER_D(0,PRED,DATA,LIST)

/** <p>Can be used inside BOOST_PP_WHILE() (see for an explanation of the D parameter).</p> */
#define BOOST_PP_LIST_FILTER_D(D,PRED,DATA,LIST) BOOST_PP_TUPLE_ELEM(3,2,BOOST_PP_LIST_FOLD_RIGHT_D(D,BOOST_PP_LIST_FILTER_F,LIST,(PRED,DATA,(_,_,0))))

#define BOOST_PP_LIST_FILTER_F(D,X,PDR) (BOOST_PP_TUPLE_ELEM(3,0,PDR),BOOST_PP_TUPLE_ELEM(3,1,PDR),BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(3,0,PDR)(D,BOOST_PP_TUPLE_ELEM(3,1,PDR),X),BOOST_PP_LIST_CONS,BOOST_PP_IF1)(X,BOOST_PP_TUPLE_ELEM(3,2,PDR)))
#endif
