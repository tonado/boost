/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library

    Sample: prints out the preprocessed tokens returned by the pp iterator
            Explicit instantiation of the cpp_grammar parsing function

    Copyright (c) 2001-2004 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    See Copyright.txt for full acknowledgements.
=============================================================================*/

#include "cpp_tokens.hpp"          // config data

#if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0

#include <string>

#include <boost/wave/token_ids.hpp>

#include "slex_token.hpp"
#include "slex_iterator.hpp"

#include <boost/wave/grammars/cpp_grammar.hpp>

///////////////////////////////////////////////////////////////////////////////
//  
//  Explicit instantiation of the cpp_grammar_gen template with the correct
//  token type. This instantiates the corresponding pt_parse function, which
//  in turn instantiates the cpp_grammar object 
//  (see wave/grammars/cpp_grammar.hpp)
//
///////////////////////////////////////////////////////////////////////////////

typedef boost::wave::cpp_token_sample::slex_iterator<
        boost::wave::cpp_token_sample::slex_token<> >
    lex_t;
template struct boost::wave::grammars::cpp_grammar_gen<lex_t>;

#endif // #if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0

