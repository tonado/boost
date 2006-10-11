/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <iostream>
#include <boost/spirit/phoenix/core.hpp>

using namespace boost::phoenix;
using namespace std;

int
main()
{
    cout << val(3)() << endl;
    cout << val("Hello World")() << endl;
    return 0;
}
