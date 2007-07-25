//  Copyright (c) 2006 Xiaogang Zhang
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_YN_HPP
#define BOOST_MATH_BESSEL_YN_HPP

#include <boost/math/special_functions/detail/bessel_y0.hpp>
#include <boost/math/special_functions/detail/bessel_y1.hpp>
#include <boost/math/policy/error_handling.hpp>

// Bessel function of the second kind of integer order
// Y_n(z) is the dominant solution, forward recurrence always OK (though unstable)

namespace boost { namespace math { namespace detail{

template <typename T, typename Policy>
T bessel_yn(int n, T x, const Policy& pol)
{
    T value, factor, current, prev;

    using namespace boost::math::tools;

    static const char* function = "boost::math::bessel_yn<%1%>(%1%,%1%)";

    if ((x == 0) && (n == 0))
    {
       return -policy::raise_overflow_error<T>(function, 0, pol);
    }
    if (x <= 0)
    {
       return policy::raise_domain_error<T>(function,
            "Got x = %1%, but x must be > 0, complex result not supported.", x, pol);
    }

    if (n == 0)
    {
        return bessel_y0(x, pol);
    }
    if (n == 1)
    {
        return bessel_y1(x, pol);
    }
    if (n < 0)
    {
        factor = (n & 0x1) ? -1 : 1;  // Y_{-n}(z) = (-1)^n Y_n(z)
        n = -n;
    }
    else
    {
        factor = 1;
    }

    prev = bessel_y0(x, pol);
    current = bessel_y1(x, pol);
    for (int k = 1; k < n; k++)            // n >= 2
    {
        value = 2 * k * current / x - prev;
        prev = current;
        current = value;
    }
    value *= factor;

    return value;
}

}}} // namespaces

#endif // BOOST_MATH_BESSEL_YN_HPP
