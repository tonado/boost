#if defined(_MSC_VER) && !defined(__ICL)
#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#endif

//
//  bind_test_3.cpp - tests bind.hpp with member function pointers
//
//  Version 1.00.0003 (2001-07-13)
//
//  Copyright (c) 2001 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <iostream>

struct X
{
    mutable unsigned int hash;

    X(): hash(0) {}

    int f0() { f1(17); return 0; }
    int g0() const { g1(17); return 0; }

    int f1(int a1) { hash = (hash * 17041 + a1) % 32768; return 0; }
    int g1(int a1) const { hash = (hash * 17041 + a1 * 2) % 32768; return 0; }

    int f2(int a1, int a2) { f1(a1); f1(a2); return 0; }
    int g2(int a1, int a2) const { g1(a1); g1(a2); return 0; }

    int f3(int a1, int a2, int a3) { f2(a1, a2); f1(a3); return 0; }
    int g3(int a1, int a2, int a3) const { g2(a1, a2); g1(a3); return 0; }

    int f4(int a1, int a2, int a3, int a4) { f3(a1, a2, a3); f1(a4); return 0; }
    int g4(int a1, int a2, int a3, int a4) const { g3(a1, a2, a3); g1(a4); return 0; }

    int f5(int a1, int a2, int a3, int a4, int a5) { f4(a1, a2, a3, a4); f1(a5); return 0; }
    int g5(int a1, int a2, int a3, int a4, int a5) const { g4(a1, a2, a3, a4); g1(a5); return 0; }

    int f6(int a1, int a2, int a3, int a4, int a5, int a6) { f5(a1, a2, a3, a4, a5); f1(a6); return 0; }
    int g6(int a1, int a2, int a3, int a4, int a5, int a6) const { g5(a1, a2, a3, a4, a5); g1(a6); return 0; }

    int f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) { f6(a1, a2, a3, a4, a5, a6); f1(a7); return 0; }
    int g7(int a1, int a2, int a3, int a4, int a5, int a6, int a7) const { g6(a1, a2, a3, a4, a5, a6); g1(a7); return 0; }

    int f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) { f7(a1, a2, a3, a4, a5, a6, a7); f1(a8); return 0; }
    int g8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) const { g7(a1, a2, a3, a4, a5, a6, a7); g1(a8); return 0; }
};

int detect_errors(bool x)
{
    if(x)
    {
        std::cerr << "no errors detected.\n";
        return 0;
    }
    else
    {
        std::cerr << "test failed.\n";
        return 1;
    }
}

int main()
{
    using namespace boost;

    X x;

    // 0

    bind(&X::f0, &x)();
    bind(&X::f0, ref(x))();

    bind(&X::g0, &x)();
    bind(&X::g0, x)();
    bind(&X::g0, ref(x))();

    // 1

    bind(&X::f1, &x, 1)();
    bind(&X::f1, ref(x), 1)();

    bind(&X::g1, &x, 1)();
    bind(&X::g1, x, 1)();
    bind(&X::g1, ref(x), 1)();

    // 2

    bind(&X::f2, &x, 1, 2)();
    bind(&X::f2, ref(x), 1, 2)();

    bind(&X::g2, &x, 1, 2)();
    bind(&X::g2, x, 1, 2)();
    bind(&X::g2, ref(x), 1, 2)();

    // 3

    bind(&X::f3, &x, 1, 2, 3)();
    bind(&X::f3, ref(x), 1, 2, 3)();

    bind(&X::g3, &x, 1, 2, 3)();
    bind(&X::g3, x, 1, 2, 3)();
    bind(&X::g3, ref(x), 1, 2, 3)();

    // 4

    bind(&X::f4, &x, 1, 2, 3, 4)();
    bind(&X::f4, ref(x), 1, 2, 3, 4)();

    bind(&X::g4, &x, 1, 2, 3, 4)();
    bind(&X::g4, x, 1, 2, 3, 4)();
    bind(&X::g4, ref(x), 1, 2, 3, 4)();

    // 5

    bind(&X::f5, &x, 1, 2, 3, 4, 5)();
    bind(&X::f5, ref(x), 1, 2, 3, 4, 5)();

    bind(&X::g5, &x, 1, 2, 3, 4, 5)();
    bind(&X::g5, x, 1, 2, 3, 4, 5)();
    bind(&X::g5, ref(x), 1, 2, 3, 4, 5)();

    // 6

    bind(&X::f6, &x, 1, 2, 3, 4, 5, 6)();
    bind(&X::f6, ref(x), 1, 2, 3, 4, 5, 6)();

    bind(&X::g6, &x, 1, 2, 3, 4, 5, 6)();
    bind(&X::g6, x, 1, 2, 3, 4, 5, 6)();
    bind(&X::g6, ref(x), 1, 2, 3, 4, 5, 6)();

    // 7

    bind(&X::f7, &x, 1, 2, 3, 4, 5, 6, 7)();
    bind(&X::f7, ref(x), 1, 2, 3, 4, 5, 6, 7)();

    bind(&X::g7, &x, 1, 2, 3, 4, 5, 6, 7)();
    bind(&X::g7, x, 1, 2, 3, 4, 5, 6, 7)();
    bind(&X::g7, ref(x), 1, 2, 3, 4, 5, 6, 7)();

    // 8

    bind(&X::f8, &x, 1, 2, 3, 4, 5, 6, 7, 8)();
    bind(&X::f8, ref(x), 1, 2, 3, 4, 5, 6, 7, 8)();

    bind(&X::g8, &x, 1, 2, 3, 4, 5, 6, 7, 8)();
    bind(&X::g8, x, 1, 2, 3, 4, 5, 6, 7, 8)();
    bind(&X::g8, ref(x), 1, 2, 3, 4, 5, 6, 7, 8)();

    return detect_errors(x.hash == 23558);
}
