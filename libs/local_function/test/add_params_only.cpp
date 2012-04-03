
// Copyright (C) 2009-2012 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Home at http://www.boost.org/libs/local_function

#include <boost/config.hpp>
#ifdef BOOST_NO_VARIADIC_MACROS
#   error "variadic macros required"
#else

#include <boost/local_function.hpp>
#include <boost/detail/lightweight_test.hpp>

int main(void) {
    //[add_params_only
    int BOOST_LOCAL_FUNCTION(int x, int y) { // Local function.
        return x + y;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    
    BOOST_TEST(add(1, 2) == 3); // Local function call.
    //]
    return boost::report_errors();
}

#endif // VARIADIC_MACROS

