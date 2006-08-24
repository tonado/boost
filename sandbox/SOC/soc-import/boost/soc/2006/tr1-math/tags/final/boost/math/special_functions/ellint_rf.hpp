//  Copyright (c) 2006 Xiaogang Zhang
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_ELLINT_RF_HPP
#define BOOST_MATH_ELLINT_RF_HPP

#define MAX_ITERATION 100000
#define MAX3(a, b, c) ( (a)>(b) ? (a)>(c)?(a):(c) : (b)>(c)?(b):(c) )

#include <boost/math/tools/error_handling.hpp>

// Carlson's elliptic integral of the first kind
// R_F(x, y, z) = 0.5 * \int_{0}^{\infty} [(t+x)(t+y)(t+z)]^{-1/2} dt
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

template <typename T>
T ellint_rf(T x, T y, T z)
{
    T value, X, Y, Z, E2, E3, u, lambda, tolerance;
    int k;

    using namespace std;
    using namespace boost::math::tools;

    if (x < 0 || y < 0 || z < 0)
    {
        return domain_error<T>("boost::math::ellint_rf(x, y, z)",
            "domain error, all arguments must be non-negative");
    }
    if (x + y == 0 || y + z == 0 || z + x == 0)
    {
        return domain_error<T>("boost::math::ellint_rf(x, y, z)",
            "domain error, at most one argument can be zero");
    }

    // error scales as the 6th power of tolerance
    tolerance = pow(4.0L*std::numeric_limits<T>::epsilon(), 1.0L/6.0L);

    // duplication
    for (k = 1; k < MAX_ITERATION; k++)
    {
        u = (x + y + z) / 3.0L;
        X = 1.0L - x / u;
        Y = 1.0L - y / u;
        Z = 1.0L - z / u;
        if (MAX3(abs(X), abs(Y), abs(Z)) < tolerance) { break; }
        lambda = sqrt(x * y) + sqrt(y * z) + sqrt(z * x);
        x = 0.25L * (x + lambda);
        y = 0.25L * (y + lambda);
        z = 0.25L * (z + lambda);
    }
    if (k >= MAX_ITERATION)             // virtually impossible
    {
        cout << "Warning: boost::math::ellint_rf failed to converge" << endl;
    }

    // Taylor series expansion to the 5th order
    E2 = X * Y - Z * Z;
    E3 = X * Y * Z;
    value = (1.0L + E2*(E2/24.0L - E3*3.0L/44.0L - 0.1L) + E3/14.0L) / sqrt(u);

    return value;
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_RF_HPP
