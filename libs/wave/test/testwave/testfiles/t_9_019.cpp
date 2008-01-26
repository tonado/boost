/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2008 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

//  tests, whether regression causing #include_next to infinitely recurse is
//  fixed

//E t_9_019.cpp(14): error: could not find include file: t_9_019.hpp
#include_next "t_9_019.hpp"

