// Copyright (c) 2008-2009 Ben Hanson
// Copyright (c) 2008-2011 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Auto-generated by boost::lexer, do not edit

#if !defined(BOOST_SPIRIT_LEXER_NEXT_TOKEN_CONJURE_STATIC_SWITCH_JUL_24_2011_23_59_45)
#define BOOST_SPIRIT_LEXER_NEXT_TOKEN_CONJURE_STATIC_SWITCH_JUL_24_2011_23_59_45

#include <boost/detail/iterator.hpp>
#include <boost/spirit/home/support/detail/lexer/char_traits.hpp>

////////////////////////////////////////////////////////////////////////////////
// the generated table of state names and the tokenizer have to be
// defined in the boost::spirit::lex::lexertl::static_ namespace
namespace boost { namespace spirit { namespace lex { namespace lexertl { namespace static_ {

////////////////////////////////////////////////////////////////////////////////
// this table defines the names of the lexer states
char const* const lexer_state_names_conjure_static_switch[1] = 
{
    "INITIAL"
};

////////////////////////////////////////////////////////////////////////////////
// this variable defines the number of lexer states
std::size_t const lexer_state_count_conjure_static_switch = 1;

////////////////////////////////////////////////////////////////////////////////
// this function returns the next matched token
template<typename Iterator>
std::size_t next_token_conjure_static_switch (std::size_t& /*start_state_*/, bool& /*bol_*/, 
    Iterator &start_token_, Iterator const& end_, std::size_t& unique_id_)
{
    static std::size_t const npos = static_cast<std::size_t>(~0);

    if (start_token_ == end_)
    {
        unique_id_ = npos;
        return 0;
    }

    Iterator curr_ = start_token_;
    bool end_state_ = false;
    std::size_t id_ = npos;
    std::size_t uid_ = npos;
    Iterator end_token_ = start_token_;

    char ch_ = 0;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9')) goto state0_1;

    if (ch_ == 't') goto state0_2;

    if (ch_ == 'f') goto state0_3;

    if (ch_ == 'v') goto state0_4;

    if (ch_ == 'i') goto state0_5;

    if (ch_ == 'e') goto state0_6;

    if (ch_ == 'w') goto state0_7;

    if (ch_ == 'r') goto state0_8;

    if (ch_ == '|') goto state0_9;

    if (ch_ == '&') goto state0_10;

    if (ch_ == '=') goto state0_11;

    if (ch_ == '!') goto state0_12;

    if (ch_ == '<') goto state0_13;

    if (ch_ == '>') goto state0_14;

    if (ch_ == '+') goto state0_15;

    if (ch_ == '-') goto state0_16;

    if (ch_ == '*') goto state0_17;

    if (ch_ == '/') goto state0_18;

    if (ch_ == '(') goto state0_19;

    if (ch_ == ')') goto state0_20;

    if (ch_ == '{') goto state0_21;

    if (ch_ == '}') goto state0_22;

    if (ch_ == ',') goto state0_23;

    if (ch_ == ';') goto state0_24;

    if ((ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'd') || (ch_ >= 'g' && ch_ <= 'h') || (ch_ >= 'j' && ch_ <= 'q') || ch_ == 's' || ch_ == 'u' || (ch_ >= 'x' && ch_ <= 'z')) goto state0_25;

    if ((ch_ >= '\t' && ch_ <= '\n') || ch_ == '\r' || ch_ == ' ') goto state0_26;
    goto end;

state0_1:
    end_state_ = true;
    id_ = 41;
    uid_ = 0;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9')) goto state0_1;
    goto end;

state0_2:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'q') || (ch_ >= 's' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'r') goto state0_27;
    goto end;

state0_3:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'b' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'a') goto state0_28;
    goto end;

state0_4:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'n') || (ch_ >= 'p' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'o') goto state0_29;
    goto end;

state0_5:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'e') || (ch_ >= 'g' && ch_ <= 'm') || (ch_ >= 'o' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'n') goto state0_30;

    if (ch_ == 'f') goto state0_31;
    goto end;

state0_6:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'k') || (ch_ >= 'm' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'l') goto state0_32;
    goto end;

state0_7:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'g') || (ch_ >= 'i' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'h') goto state0_33;
    goto end;

state0_8:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'd') || (ch_ >= 'f' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'e') goto state0_34;
    goto end;

state0_9:
    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '|') goto state0_35;
    goto end;

state0_10:
    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '&') goto state0_36;
    goto end;

state0_11:
    end_state_ = true;
    id_ = 61;
    uid_ = 26;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '=') goto state0_37;
    goto end;

state0_12:
    end_state_ = true;
    id_ = 7077923;
    uid_ = 20;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '=') goto state0_38;
    goto end;

state0_13:
    end_state_ = true;
    id_ = 4849683;
    uid_ = 12;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '=') goto state0_39;
    goto end;

state0_14:
    end_state_ = true;
    id_ = 4849685;
    uid_ = 14;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '=') goto state0_40;
    goto end;

state0_15:
    end_state_ = true;
    id_ = 5898265;
    uid_ = 16;
    end_token_ = curr_;
    goto end;

state0_16:
    end_state_ = true;
    id_ = 5898266;
    uid_ = 17;
    end_token_ = curr_;
    goto end;

state0_17:
    end_state_ = true;
    id_ = 6422555;
    uid_ = 18;
    end_token_ = curr_;
    goto end;

state0_18:
    end_state_ = true;
    id_ = 6422556;
    uid_ = 19;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '*') goto state0_41;
    goto end;

state0_19:
    end_state_ = true;
    id_ = 40;
    uid_ = 21;
    end_token_ = curr_;
    goto end;

state0_20:
    end_state_ = true;
    id_ = 41;
    uid_ = 22;
    end_token_ = curr_;
    goto end;

state0_21:
    end_state_ = true;
    id_ = 123;
    uid_ = 23;
    end_token_ = curr_;
    goto end;

state0_22:
    end_state_ = true;
    id_ = 125;
    uid_ = 24;
    end_token_ = curr_;
    goto end;

state0_23:
    end_state_ = true;
    id_ = 44;
    uid_ = 25;
    end_token_ = curr_;
    goto end;

state0_24:
    end_state_ = true;
    id_ = 59;
    uid_ = 27;
    end_token_ = curr_;
    goto end;

state0_25:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_26:
    end_state_ = true;
    id_ = 40;
    uid_ = 30;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '\t' && ch_ <= '\n') || ch_ == '\r' || ch_ == ' ') goto state0_26;
    goto end;

state0_27:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 't') || (ch_ >= 'v' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'u') goto state0_42;
    goto end;

state0_28:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'k') || (ch_ >= 'm' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'l') goto state0_43;
    goto end;

state0_29:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'h') || (ch_ >= 'j' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'i') goto state0_44;
    goto end;

state0_30:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 's') || (ch_ >= 'u' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 't') goto state0_45;
    goto end;

state0_31:
    end_state_ = true;
    id_ = 65538;
    uid_ = 4;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_32:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'r') || (ch_ >= 't' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 's') goto state0_46;
    goto end;

state0_33:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'h') || (ch_ >= 'j' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'i') goto state0_47;
    goto end;

state0_34:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 's') || (ch_ >= 'u' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 't') goto state0_48;
    goto end;

state0_35:
    end_state_ = true;
    id_ = 1703948;
    uid_ = 8;
    end_token_ = curr_;
    goto end;

state0_36:
    end_state_ = true;
    id_ = 2228237;
    uid_ = 9;
    end_token_ = curr_;
    goto end;

state0_37:
    end_state_ = true;
    id_ = 4325393;
    uid_ = 10;
    end_token_ = curr_;
    goto end;

state0_38:
    end_state_ = true;
    id_ = 4325394;
    uid_ = 11;
    end_token_ = curr_;
    goto end;

state0_39:
    end_state_ = true;
    id_ = 4849684;
    uid_ = 13;
    end_token_ = curr_;
    goto end;

state0_40:
    end_state_ = true;
    id_ = 4849686;
    uid_ = 15;
    end_token_ = curr_;
    goto end;

state0_41:
    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ != '*') goto state0_41;

    if (ch_ == '*') goto state0_49;
    goto end;

state0_42:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'd') || (ch_ >= 'f' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'e') goto state0_50;
    goto end;

state0_43:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'r') || (ch_ >= 't' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 's') goto state0_42;
    goto end;

state0_44:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'c') || (ch_ >= 'e' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'd') goto state0_51;
    goto end;

state0_45:
    end_state_ = true;
    id_ = 65537;
    uid_ = 3;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_46:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'd') || (ch_ >= 'f' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'e') goto state0_52;
    goto end;

state0_47:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'k') || (ch_ >= 'm' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'l') goto state0_53;
    goto end;

state0_48:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 't') || (ch_ >= 'v' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'u') goto state0_54;
    goto end;

state0_49:
    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ == '*') goto state0_49;

    if (ch_ != '*' && ch_ != '/') goto state0_55;

    if (ch_ == '/') goto state0_56;
    goto end;

state0_50:
    end_state_ = true;
    id_ = 42;
    uid_ = 1;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_51:
    end_state_ = true;
    id_ = 65536;
    uid_ = 2;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_52:
    end_state_ = true;
    id_ = 65539;
    uid_ = 5;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_53:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'd') || (ch_ >= 'f' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'e') goto state0_57;
    goto end;

state0_54:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'q') || (ch_ >= 's' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'r') goto state0_58;
    goto end;

state0_55:
    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ != '*') goto state0_55;

    if (ch_ == '*') goto state0_59;
    goto end;

state0_56:
    end_state_ = true;
    id_ = 39;
    uid_ = 29;
    end_token_ = curr_;
    goto end;

state0_57:
    end_state_ = true;
    id_ = 65540;
    uid_ = 6;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;
    goto end;

state0_58:
    end_state_ = true;
    id_ = 38;
    uid_ = 28;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'm') || (ch_ >= 'o' && ch_ <= 'z')) goto state0_25;

    if (ch_ == 'n') goto state0_60;
    goto end;

state0_59:
    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if (ch_ != '*' && ch_ != '/') goto state0_55;

    if (ch_ == '/') goto state0_56;

    if (ch_ == '*') goto state0_59;
    goto end;

state0_60:
    end_state_ = true;
    id_ = 65541;
    uid_ = 7;
    end_token_ = curr_;

    if (curr_ == end_) goto end;
    ch_ = *curr_;
    ++curr_;

    if ((ch_ >= '0' && ch_ <= '9') || (ch_ >= 'A' && ch_ <= 'Z') || ch_ == '_' || (ch_ >= 'a' && ch_ <= 'z')) goto state0_25;

end:
    if (end_state_)
    {
        // return longest match
        start_token_ = end_token_;
    }
    else
    {
        id_ = npos;
        uid_ = npos;
    }

    unique_id_ = uid_;
    return id_;
}

////////////////////////////////////////////////////////////////////////////////
// this defines a generic accessors for the information above
struct lexer_conjure_static_switch
{
    // version number and feature-set of compatible static lexer engine
    enum
    {
        static_version = 65536,
        supports_bol = false,
        supports_eol = false
    };

    // return the number of lexer states
    static std::size_t state_count()
    {
        return lexer_state_count_conjure_static_switch; 
    }

    // return the name of the lexer state as given by 'idx'
    static char const* state_name(std::size_t idx)
    {
        return lexer_state_names_conjure_static_switch[idx]; 
    }

    // return the next matched token
    template<typename Iterator>
    static std::size_t next(std::size_t &start_state_, bool& bol_
      , Iterator &start_token_, Iterator const& end_, std::size_t& unique_id_)
    {
        return next_token_conjure_static_switch(start_state_, bol_, start_token_, end_, unique_id_);
    }
};

}}}}}  // namespace boost::spirit::lex::lexertl::static_

#endif
