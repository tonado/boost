//  Copyright (c) 2006 Xiaogang Zhang
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_I1_HPP
#define BOOST_MATH_BESSEL_I1_HPP

#include <boost/math/tools/rational.hpp>
#include <boost/assert.hpp>

// Modified Bessel function of the first kind of order one
// minimax rational approximations on intervals, see
// Blair and Edwards, Chalk River Report AECL-4928, 1974

namespace boost { namespace math { namespace detail{

template <typename T>
T bessel_i1(T x)
{
    static const T P1[] = {
        static_cast<T>(-1.4577180278143463643e+15L),
        static_cast<T>(-1.7732037840791591320e+14L),
        static_cast<T>(-6.9876779648010090070e+12L),
        static_cast<T>(-1.3357437682275493024e+11L),
        static_cast<T>(-1.4828267606612366099e+09L),
        static_cast<T>(-1.0588550724769347106e+07L),
        static_cast<T>(-5.1894091982308017540e+04L),
        static_cast<T>(-1.8225946631657315931e+02L),
        static_cast<T>(-4.7207090827310162436e-01L),
        static_cast<T>(-9.1746443287817501309e-04L),
        static_cast<T>(-1.3466829827635152875e-06L),
        static_cast<T>(-1.4831904935994647675e-09L),
        static_cast<T>(-1.1928788903603238754e-12L),
        static_cast<T>(-6.5245515583151902910e-16L),
        static_cast<T>(-1.9705291802535139930e-19L),
    };
    static const T Q1[] = {
        static_cast<T>(-2.9154360556286927285e+15L),
        static_cast<T>(9.7887501377547640438e+12L),
        static_cast<T>(-1.4386907088588283434e+10L),
        static_cast<T>(1.1594225856856884006e+07L),
        static_cast<T>(-5.1326864679904189920e+03L),
        static_cast<T>(1.0L),
    };
    static const T P2[] = {
        static_cast<T>(1.4582087408985668208e-05L),
        static_cast<T>(-8.9359825138577646443e-04L),
        static_cast<T>(2.9204895411257790122e-02L),
        static_cast<T>(-3.4198728018058047439e-01L),
        static_cast<T>(1.3960118277609544334e+00L),
        static_cast<T>(-1.9746376087200685843e+00L),
        static_cast<T>(8.5591872901933459000e-01L),
        static_cast<T>(-6.0437159056137599999e-02L),
    };
    static const T Q2[] = {
        static_cast<T>(3.7510433111922824643e-05L),
        static_cast<T>(-2.2835624489492512649e-03L),
        static_cast<T>(7.4212010813186530069e-02L),
        static_cast<T>(-8.5017476463217924408e-01L),
        static_cast<T>(3.2593714889036996297e+00L),
        static_cast<T>(-3.8806586721556593450e+00L),
        static_cast<T>(1.0L),
    };
    T value, factor, r, w;

    using namespace std;
    using namespace boost::math::tools;

    w = abs(x);
    if (x == 0)
    {
        return static_cast<T>(0);
    }
    if (w <= 15)                        // w in (0, 15]
    {
        T y = x * x;
        r = evaluate_polynomial(P1, y) / evaluate_polynomial(Q1, y);
        factor = w;
        value = factor * r;
    }
    else                                // w in (15, \infty)
    {
        T y = 1 / w - T(1) / 15;
        r = evaluate_polynomial(P2, y) / evaluate_polynomial(Q2, y);
        factor = exp(w) / sqrt(w);
        value = factor * r;
    }

    if (x < 0)
    {
        value *= -value;                 // odd function
    }
    return value;
}

}}} // namespaces

#endif // BOOST_MATH_BESSEL_I1_HPP
