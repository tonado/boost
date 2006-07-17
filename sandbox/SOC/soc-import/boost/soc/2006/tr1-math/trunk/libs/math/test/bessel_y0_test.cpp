//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/bessel_y0.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_y0(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 2>, 10> data = {
        SC_(0), -std::numeric_limits<T>::infinity(),
        SC_(1), SC_(0.08825696421567695798292676602351516282782),
        SC_(2), SC_(0.5103756726497451195966065927271578732681),
        SC_(4), SC_(-0.01694073932506499190363513444715321824049),
        SC_(8), SC_(0.2235214893875662205273234004986203592748),
        SC_(1e-05), SC_(-7.403160283701970132596760507467590720710),
        SC_(1e-10), SC_(-14.73251627269724204269166964262091448888),
        SC_(1e-20), SC_(-29.39122825028579689509806935415682997534),
        SC_(1e+03), SC_(0.004715917977622813399773261465665255009859),
        SC_(1e+05), SC_(0.001846766158865064104340741024315461258849),
    };
    #undef SC_

    boost::math::tools::test_univariate
        (data, type_name, "boost::math::bessel_y0", boost::math::bessel_y0);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_y0(0.0F, "float");
    test_bessel_y0(0.0, "double");
    test_bessel_y0(0.0L, "long double");

    return 0;
}
