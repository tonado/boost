//  (C) Copyright Gennadiy Rozental 2001-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Runtime.Param
#include <boost/test/utils/runtime/cla/named_parameter.hpp>
#include <boost/test/utils/runtime/cla/char_parameter.hpp>
#include <boost/test/utils/runtime/cla/parser.hpp>

namespace rt  = boost::runtime;
namespace cla = boost::runtime::cla;

// STL
#include <iostream>

int main() {
    char* argv[] = { "basic", "-abcd", "25" };
    int argc = sizeof(argv)/sizeof(char*);


    try {
        cla::parser P;

        P << cla::named_parameter<int>( "abcd" )
          << cla::named_parameter<float>( "abcd" );

        P.parse( argc, argv );
    }
    catch( rt::logic_error const& ex ) {
        std::cout << "Logic error: " << ex.msg() << std::endl;
    }

    try {
        cla::parser P;

        P << cla::named_parameter<int>( "abcd" ) - cla::guess_name
          << cla::named_parameter<float>( "ab" );

        P.parse( argc, argv );
    }
    catch( rt::logic_error const& ex ) {
        std::cout << "Logic error: " << ex.msg() << std::endl;
    }
    try {
        cla::parser P;

        P << cla::named_parameter<int>( "abcd" ) - cla::guess_name
          << cla::named_parameter<float>( "abdf" ) - cla::guess_name;

        P.parse( argc, argv );
    }
    catch( rt::logic_error const& ex ) {
        std::cout << "Logic error: " << ex.msg() << std::endl;
    }
    try {
        cla::parser P;

        P << cla::named_parameter<int>( "abcd" ) - cla::guess_name
          << cla::char_parameter<float>( 'a' );

        P.parse( argc, argv );
    }
    catch( rt::logic_error const& ex ) {
        std::cout << "Logic error: " << ex.msg() << std::endl;
    }

    return 0;
}
