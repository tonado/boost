//  Copyright (c) 2006 Xiaogang Zhang
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_KN_HPP
#define BOOST_MATH_BESSEL_KN_HPP

#include <boost/math/special_functions/detail/bessel_k0.hpp>
#include <boost/math/special_functions/detail/bessel_k1.hpp>
#include <boost/math/policy/error_handling.hpp>

// Modified Bessel function of the second kind of integer order
// K_n(z) is the dominant solution, forward recurrence always OK (though unstable)

namespace boost { namespace math { namespace detail{

template <typename T, typename Policy>
T bessel_kn(int n, T x, const Policy& pol)
{
    T value, current, prev;

    using namespace boost::math::tools;

    static const char* function = "boost::math::bessel_kn<%1%>(%1%,%1%)";

    if (x < 0)
    {
       return policy::raise_domain_error<T>(function,
            "Got x = %1%, but argument x must be non-negative, complex number result not supported.", x, pol);
    }
    if (x == 0)
    {
       return policy::raise_overflow_error<T>(function, 0, pol);
    }

    if (n == 0)
    {
        return bessel_k0(x, pol);
    }
    if (n == 1)
    {
        return bessel_k1(x, pol);
    }
    if (n < 0)
    {
        n = -n;                             // K_{-n}(z) = K_n(z)
    }

    prev = bessel_k0(x, pol);
    current = bessel_k1(x, pol);
    for (int k = 1; k < n; k++)            // n >= 2
    {
        value = 2 * k * current / x + prev;
        prev = current;
        current = value;
    }

    return value;
}

}}} // namespaces

#endif // BOOST_MATH_BESSEL_KN_HPP
