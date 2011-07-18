/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  Not a calculator anymore, right? :-)
//
//  [ JDG April 10, 2007 ]      spirit2
//  [ JDG February 18, 2011 ]   Pure attributes. No semantic actions.
//  [ HK June 3, 2011 ]         Adding lexer
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Define this to enable debugging
//#define BOOST_SPIRIT_QI_DEBUG

#include "config.hpp"
#include "function.hpp"
#include "vm.hpp"
#include "compiler.hpp"
#include "lexer.hpp"
#include <boost/lexical_cast.hpp>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << filename << std::endl;
        return 1;
    }

    std::string source_code; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(source_code));

    typedef std::string::const_iterator base_iterator_type;
    typedef client::lexer::conjure_tokens<base_iterator_type> lexer_type;
    typedef lexer_type::iterator_type iterator_type;

    lexer_type lexer;                           // Our lexer

    base_iterator_type first = source_code.begin();
    base_iterator_type last = source_code.end();

    iterator_type iter = lexer.begin(first, last);
    iterator_type end = lexer.end();

    client::vmachine vm;                        // Our virtual machine
    client::ast::function_list ast;             // Our AST

    client::error_handler<base_iterator_type, iterator_type>
        error_handler(first, last);             // Our error handler
    client::parser::function<iterator_type, lexer_type>
        function(error_handler, lexer);         // Our parser
    client::code_gen::compiler
        compiler(vm, error_handler);            // Our compiler

    // note: we don't need a skipper
    bool success = parse(iter, end, +function, ast);

    std::cout << "-------------------------\n";

    if (success && iter == end)
    {
        if (compiler(ast))
        {
            // JIT the main function
            client::function main_function = vm.get_function("main");
            if (!main_function)
            {
                std::cerr << "function main not found" << std::endl;
                return 1;
            }

            int nargs = argc-2;
            if (main_function.arity() != nargs)
            {
                std::cerr << "Error: main function requires "
                    << main_function.arity() << " arguments." << std::endl;
                std::cerr << nargs << " supplied." << std::endl;
                return 1;
            }

            std::cout << "Success\n";
            std::cout << "-------------------------\n";
            std::cout << "Assembler----------------\n\n";
            vm.print_assembler();

            // Call the main function
            int r;
            char** args = argv + 2;
            switch (nargs)
            {
                case 0: r = main_function(); break;

                case 1: r = main_function(
                    boost::lexical_cast<int>(args[0]));
                    break;

                case 2: r = main_function(
                    boost::lexical_cast<int>(args[0]),
                    boost::lexical_cast<int>(args[1]));
                    break;

                case 3: r = main_function(
                    boost::lexical_cast<int>(args[0]),
                    boost::lexical_cast<int>(args[1]),
                    boost::lexical_cast<int>(args[2]));
                    break;

                default:
                    std::cerr << "Function calls with more " <<
                        "than 3 arguments not supported" << std::endl;
                    return 1;
            }

            std::cout << "-------------------------\n";
            std::cout << "Result: " << r << std::endl;
            std::cout << "-------------------------\n\n";
        }
        else
        {
            std::cout << "Compile failure\n";
        }
    }
    else
    {
        std::cout << "Parse failure\n";
    }
    return 0;
}


