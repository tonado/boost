/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library

    Explicit instantiation of the lex_functor generation function
    
    Copyright (c) 2001-2004 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/wave/wave_config.hpp>          // configuration data

#if BOOST_WAVE_SEPARATE_LEXER_INSTANTIATION != 0

#include <string>

#include <boost/wave/token_ids.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The following file needs to be included only once throughout the whole
//  program.
#include <boost/wave/cpplexer/re2clex/cpp_re2c_lexer.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  If you've used another iterator type as std::string::iterator, you have to
//  instantiate the new_lexer_gen<> template for this iterator type too.
//  The reason is, that the library internally uses the new_lexer_gen<> 
//  template with a std::string::iterator. (You just have to undefine the 
//  following line.)
//
//  This is moved into a separate compilation unit to decouple the compilation
//  of the C++ lexer from the compilation of the other modules, which helps to 
//  reduce compilation time.
//
//  The template parameter(s) supplied should be identical to the first 
//  parameter supplied while instantiating the boost::wave::context<> template 
//  (see the file cpp.cpp).
//
///////////////////////////////////////////////////////////////////////////////

template struct boost::wave::cpplexer::new_lexer_gen<std::string::iterator>;

#endif // BOOST_WAVE_SEPARATE_LEXER_INSTANTIATION != 0
