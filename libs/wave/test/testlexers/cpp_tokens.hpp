/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2007 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/wave/wave_config.hpp>
#include <boost/wave/token_ids.hpp>

///////////////////////////////////////////////////////////////////////////////
struct lexem
{
    char const*token;
    boost::wave::token_id id;
};

///////////////////////////////////////////////////////////////////////////////
lexem const lexems[] = 
{
    { "bitand", boost::wave::T_AND_ALT },
    { "and", boost::wave::T_ANDAND_ALT },
    { "and_eq", boost::wave::T_ANDASSIGN_ALT },
    { "bitor", boost::wave::T_OR_ALT },
    { "or_eq", boost::wave::T_ORASSIGN_ALT },
    { "or", boost::wave::T_OROR_ALT },
    { "xor", boost::wave::T_XOR_ALT },
    { "xor_eq", boost::wave::T_XORASSIGN_ALT },
    { "not", boost::wave::T_NOT_ALT },
    { "not_eq", boost::wave::T_NOTEQUAL_ALT },
    { "compl", boost::wave::T_COMPL_ALT },
    { "->*", boost::wave::T_ARROWSTAR },
    { ".*", boost::wave::T_DOTSTAR },
    { "::", boost::wave::T_COLON_COLON },
    
    { "/* some C comment */", boost::wave::T_CCOMMENT },
    { "/* some multiline \n  C comment \n */", boost::wave::T_CCOMMENT },
    { "// some c++ comment\n", boost::wave::T_CPPCOMMENT },
    { " ", boost::wave::T_SPACE },
    { "\t\v\f", boost::wave::T_SPACE },
    { "'c'", boost::wave::T_CHARLIT },
    { "'\\n'", boost::wave::T_CHARLIT },
    { "'\\u1234'", boost::wave::T_CHARLIT },
    { "'\\U12345678'", boost::wave::T_CHARLIT },
    { "\"string literal\"", boost::wave::T_STRINGLIT },
    { "\"string literal \\n \\u1234 \\U12345678\"", boost::wave::T_STRINGLIT },
    { "&&", boost::wave::T_ANDAND },
    { "&=", boost::wave::T_ANDASSIGN },
    { "==", boost::wave::T_EQUAL },
    { "=", boost::wave::T_ASSIGN },
    { "|=", boost::wave::T_ORASSIGN },
    { "\?\?!=", boost::wave::T_ORASSIGN_TRIGRAPH },
    { "||", boost::wave::T_OROR },
    { "|\?\?!", boost::wave::T_OROR_TRIGRAPH },
    { "\?\?!|", boost::wave::T_OROR_TRIGRAPH },
    { "\?\?!\?\?!", boost::wave::T_OROR_TRIGRAPH },
    { "|", boost::wave::T_OR },
    { "\?\?!", boost::wave::T_OR_TRIGRAPH },
    { "^=", boost::wave::T_XORASSIGN },
    { "\?\?'=", boost::wave::T_XORASSIGN_TRIGRAPH },
    { "^", boost::wave::T_XOR },
    { "\?\?'", boost::wave::T_XOR_TRIGRAPH },
    { ",", boost::wave::T_COMMA },
    { ":>", boost::wave::T_RIGHTBRACKET_ALT },
    { ":", boost::wave::T_COLON },
    { "/=", boost::wave::T_DIVIDEASSIGN },
    { "/", boost::wave::T_DIVIDE },
    { "...", boost::wave::T_ELLIPSIS },
    { ".", boost::wave::T_DOT },
    { ">>=", boost::wave::T_SHIFTRIGHTASSIGN },
    { ">>", boost::wave::T_SHIFTRIGHT },
    { ">=", boost::wave::T_GREATEREQUAL },
    { ">", boost::wave::T_GREATER },
    { "{", boost::wave::T_LEFTBRACE },
    { "<<=", boost::wave::T_SHIFTLEFTASSIGN },
    { "<<", boost::wave::T_SHIFTLEFT },
    { "<=", boost::wave::T_LESSEQUAL },
    { "<%", boost::wave::T_LEFTBRACE_ALT },
    { "<:", boost::wave::T_LEFTBRACKET_ALT },
    { "<", boost::wave::T_LESS },
    { "\?\?<", boost::wave::T_LEFTBRACE_TRIGRAPH },
    { "(", boost::wave::T_LEFTPAREN },
    { "[", boost::wave::T_LEFTBRACKET },
    { "\?\?(", boost::wave::T_LEFTBRACKET_TRIGRAPH },
    { "--", boost::wave::T_MINUSMINUS },
    { "-=", boost::wave::T_MINUSASSIGN },
    { "->", boost::wave::T_ARROW },
    { "-", boost::wave::T_MINUS },
    { "%:%:", boost::wave::T_POUND_POUND_ALT },
    { "%=", boost::wave::T_PERCENTASSIGN },
    { "%>", boost::wave::T_RIGHTBRACE_ALT },
    { "%:", boost::wave::T_POUND_ALT },
    { "%", boost::wave::T_PERCENT },
    { "!=", boost::wave::T_NOTEQUAL },
    { "!", boost::wave::T_NOT },
    { "+=", boost::wave::T_PLUSASSIGN },
    { "++", boost::wave::T_PLUSPLUS },
    { "+", boost::wave::T_PLUS },
    { "}", boost::wave::T_RIGHTBRACE },
    { "\?\?>", boost::wave::T_RIGHTBRACE_TRIGRAPH },
    { ")", boost::wave::T_RIGHTPAREN },
    { "]", boost::wave::T_RIGHTBRACKET },
    { "\?\?)", boost::wave::T_RIGHTBRACKET_TRIGRAPH },
    { ";", boost::wave::T_SEMICOLON },
    { "*=", boost::wave::T_STARASSIGN },
    { "*", boost::wave::T_STAR },
    { "~", boost::wave::T_COMPL },
    { "\?\?-", boost::wave::T_COMPL_TRIGRAPH },
    { "asm", boost::wave::T_ASM },
    { "auto", boost::wave::T_AUTO },
    { "bool", boost::wave::T_BOOL },
    { "false", boost::wave::T_FALSE },
    { "true", boost::wave::T_TRUE },
    { "break", boost::wave::T_BREAK },
    { "case", boost::wave::T_CASE },
    { "catch", boost::wave::T_CATCH },
    { "char", boost::wave::T_CHAR },
    { "class", boost::wave::T_CLASS },
    { "const_cast", boost::wave::T_CONSTCAST },
    { "const", boost::wave::T_CONST },
    { "continue", boost::wave::T_CONTINUE },
    { "default", boost::wave::T_DEFAULT },
    { "delete", boost::wave::T_DELETE },
    { "do", boost::wave::T_DO },
    { "double", boost::wave::T_DOUBLE },
    { "dynamic_cast", boost::wave::T_DYNAMICCAST },
    { "else", boost::wave::T_ELSE },
    { "enum", boost::wave::T_ENUM },
    { "explicit", boost::wave::T_EXPLICIT },
    { "export", boost::wave::T_EXPORT },
    { "extern", boost::wave::T_EXTERN },
    { "float", boost::wave::T_FLOAT },
    { "for", boost::wave::T_FOR },
    { "friend", boost::wave::T_FRIEND },
    { "goto", boost::wave::T_GOTO },
    { "if", boost::wave::T_IF },
#if BOOST_WAVE_SUPPORT_IMPORT_KEYWORD != 0
    { "import", boost::wave::T_IMPORT},
#endif
    { "inline", boost::wave::T_INLINE },
    { "int", boost::wave::T_INT },
    { "long", boost::wave::T_LONG },
    { "mutable", boost::wave::T_MUTABLE },
    { "namespace", boost::wave::T_NAMESPACE },
    { "new", boost::wave::T_NEW },
    { "operator", boost::wave::T_OPERATOR },
    { "private", boost::wave::T_PRIVATE },
    { "protected", boost::wave::T_PROTECTED },
    { "public", boost::wave::T_PUBLIC },
    { "register", boost::wave::T_REGISTER },
    { "reinterpret_cast", boost::wave::T_REINTERPRETCAST },
    { "return", boost::wave::T_RETURN },
    { "short", boost::wave::T_SHORT },
    { "signed", boost::wave::T_SIGNED },
    { "sizeof", boost::wave::T_SIZEOF },
    { "static_cast", boost::wave::T_STATICCAST },
    { "static", boost::wave::T_STATIC },
    { "struct", boost::wave::T_STRUCT },
    { "switch", boost::wave::T_SWITCH },
    { "template", boost::wave::T_TEMPLATE },
    { "this", boost::wave::T_THIS },
    { "throw", boost::wave::T_THROW },
    { "try", boost::wave::T_TRY },
    { "typedef", boost::wave::T_TYPEDEF },
    { "typeid", boost::wave::T_TYPEID },
    { "typename", boost::wave::T_TYPENAME },
    { "union", boost::wave::T_UNION },
    { "unsigned", boost::wave::T_UNSIGNED },
    { "using", boost::wave::T_USING },
    { "virtual", boost::wave::T_VIRTUAL },
    { "void", boost::wave::T_VOID },
    { "volatile", boost::wave::T_VOLATILE },
    { "wchar_t", boost::wave::T_WCHART },
    { "while", boost::wave::T_WHILE },
#if BOOST_WAVE_SUPPORT_MS_EXTENSIONS != 0
    { "__int8", boost::wave::T_MSEXT_INT8 },
    { "__int16", boost::wave::T_MSEXT_INT16 },
    { "__int32", boost::wave::T_MSEXT_INT32 },
    { "__int64", boost::wave::T_MSEXT_INT64 },
    { "__based", boost::wave::T_MSEXT_BASED },
    { "_based", boost::wave::T_MSEXT_BASED },
    { "__declspec", boost::wave::T_MSEXT_DECLSPEC },
    { "_declspec", boost::wave::T_MSEXT_DECLSPEC },
    { "__fastcall", boost::wave::T_MSEXT_FASTCALL },
    { "_fastcall", boost::wave::T_MSEXT_FASTCALL },
    { "__stdcall", boost::wave::T_MSEXT_STDCALL },
    { "_stdcall", boost::wave::T_MSEXT_STDCALL },
    { "__try", boost::wave::T_MSEXT_TRY },
    { "__except", boost::wave::T_MSEXT_EXCEPT },
    { "__finally", boost::wave::T_MSEXT_FINALLY },
    { "__leave", boost::wave::T_MSEXT_LEAVE },
    { "__inline", boost::wave::T_MSEXT_INLINE },
    { "_inline", boost::wave::T_MSEXT_INLINE },
    { "__asm", boost::wave::T_MSEXT_ASM },
    { "_asm", boost::wave::T_MSEXT_ASM },
    { "#region", boost::wave::T_MSEXT_PP_REGION },
    { "#endregion", boost::wave::T_MSEXT_PP_ENDREGION },
#endif // BOOST_WAVE_SUPPORT_MS_EXTENSIONS != 0
    { "#define", boost::wave::T_PP_DEFINE },
    { "#ifdef", boost::wave::T_PP_IFDEF },
    { "#ifndef", boost::wave::T_PP_IFNDEF },
    { "#if", boost::wave::T_PP_IF },
    { "#else", boost::wave::T_PP_ELSE },
    { "#elif", boost::wave::T_PP_ELIF },
    { "#endif", boost::wave::T_PP_ENDIF },
    { "#error", boost::wave::T_PP_ERROR },
    { "#include \"some_header\"", boost::wave::T_PP_QHEADER },
    { "#include <some_header>", boost::wave::T_PP_HHEADER },
//    { "#include SOME_HEADER", boost::wave::T_PP_INCLUDE },
    { "#line", boost::wave::T_PP_LINE },
    { "#pragma", boost::wave::T_PP_PRAGMA },
    { "#undef", boost::wave::T_PP_UNDEF },
    { "#warning", boost::wave::T_PP_WARNING },
    { "some_identifier_1234", boost::wave::T_IDENTIFIER },
    { "some_identifier_\\u00d4", boost::wave::T_IDENTIFIER },
    { "some_identifier_\\U000000d4", boost::wave::T_IDENTIFIER },
    { ".1", boost::wave::T_FLOATLIT },
    { "1.1", boost::wave::T_FLOATLIT },
    { "1.", boost::wave::T_FLOATLIT },
    { ".1e+1", boost::wave::T_FLOATLIT },
    { ".1E-1", boost::wave::T_FLOATLIT },
    { ".1e1", boost::wave::T_FLOATLIT },
    { "1e1", boost::wave::T_FLOATLIT },
    { "1e1f", boost::wave::T_FLOATLIT },
    { "1e1L", boost::wave::T_FLOATLIT },
    { "1.1e1fl", boost::wave::T_FLOATLIT },
    { "1.e1LF", boost::wave::T_FLOATLIT },
    { "1234LL", boost::wave::T_LONGINTLIT },
    { "1234Ll", boost::wave::T_LONGINTLIT },
    { "1234ll", boost::wave::T_LONGINTLIT },
    { "1234llu", boost::wave::T_LONGINTLIT },
    { "1234ull", boost::wave::T_LONGINTLIT },
#if BOOST_WAVE_SUPPORT_MS_EXTENSIONS != 0
    { "1234i64", boost::wave::T_LONGINTLIT },
#endif
    { "1234L", boost::wave::T_INTLIT },
    { "1234l", boost::wave::T_INTLIT },
    { "1234ul", boost::wave::T_INTLIT },
    { "1234LU", boost::wave::T_INTLIT },
    { "1234", boost::wave::T_INTLIT },
    { " ", boost::wave::T_SPACE },
    { "\t", boost::wave::T_SPACE },
    { "\n", boost::wave::T_NEWLINE },
    { "##", boost::wave::T_POUND_POUND },
    { "\?\?=\?\?=", boost::wave::T_POUND_POUND_TRIGRAPH },
    { "#", boost::wave::T_POUND },
    { "\?\?=", boost::wave::T_POUND_TRIGRAPH },
    { "\?\?/", boost::wave::T_ANY_TRIGRAPH },
    { "?", boost::wave::T_QUESTION_MARK },
    { NULL, boost::wave::token_id(0) }
};

