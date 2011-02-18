/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A Calculator example demonstrating generation of AST
//
//  [ JDG April 28, 2008 ]
//  [ JDG February 18, 2011 : Pure attributes. No semantic actions. ]
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
#define BOOST_SPIRIT_ALTERNATIVES_ALLOW_ATTR_COMPAT

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <string>

namespace client { namespace ast
{
    struct nil {};
    struct signed_;
    struct program;

    typedef boost::variant<
            nil
          , unsigned int
          , boost::recursive_wrapper<signed_>
          , boost::recursive_wrapper<program>
        >
    operand;

    struct signed_
    {
        char sign;
        operand operand_;
    };

    struct operation
    {
        char operator_;
        operand operand_;
    };

    struct program
    {
        operand first;
        std::list<operation> rest;
    };
}}

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::signed_,
    (char, sign)
    (client::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::operation,
    (char, operator_)
    (client::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    client::ast::program,
    (client::ast::operand, first)
    (std::list<client::ast::operation>, rest)
)

namespace client { namespace ast
{
    struct printer
    {
        typedef void result_type;

        void operator()(nil) const {}
        void operator()(unsigned int n) const { std::cout << n; }

        void operator()(operation const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.operator_)
            {
                case '+': std::cout << " add"; break;
                case '-': std::cout << " subt"; break;
                case '*': std::cout << " mult"; break;
                case '/': std::cout << " div"; break;
            }
        }

        void operator()(signed_ const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': std::cout << " neg"; break;
                case '+': std::cout << " pos"; break;
            }
        }

        void operator()(program const& x) const
        {
            boost::apply_visitor(*this, x.first);
            BOOST_FOREACH(operation const& oper, x.rest)
            {
                std::cout << ' ';
                (*this)(oper);
            }
        }
    };

    struct eval
    {
        typedef int result_type;

        int operator()(nil) const { BOOST_ASSERT(0); return 0; }
        int operator()(unsigned int n) const { return n; }

        int operator()(operation const& x, int state) const
        {
            int rhs = boost::apply_visitor(*this, x.operand_);
            switch (x.operator_)
            {
                case '+': return state + rhs;
                case '-': return state - rhs;
                case '*': return state * rhs;
                case '/': return state / rhs;
            }
            BOOST_ASSERT(0);
            return 0;
        }

        int operator()(signed_ const& x) const
        {
            int rhs = boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': return -rhs;
                case '+': return +rhs;
            }
            BOOST_ASSERT(0);
            return 0;
        }

        int operator()(program const& x) const
        {
            int state = boost::apply_visitor(*this, x.first);
            BOOST_FOREACH(operation const& oper, x.rest)
            {
                state = (*this)(oper, state);
            }
            return state;
        }
    };
}}

namespace client
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    ///////////////////////////////////////////////////////////////////////////////
    //  Our calculator grammar
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct calculator : qi::grammar<Iterator, ast::program(), ascii::space_type>
    {
        calculator() : calculator::base_type(expression)
        {
            qi::uint_type uint_;
            qi::char_type char_;

            expression =
                term
                >> *(   (char_('+') >> term)
                    |   (char_('-') >> term)
                    )
                ;

            term =
                factor
                >> *(   (char_('*') >> factor)
                    |   (char_('/') >> factor)
                    )
                ;

            factor =
                uint_
                |   '(' >> expression >> ')'
                |   (char_('-') >> factor)
                |   (char_('+') >> factor)
                ;
        }

        qi::rule<Iterator, ast::program(), ascii::space_type> expression;
        qi::rule<Iterator, ast::program(), ascii::space_type> term;
        qi::rule<Iterator, ast::operand(), ascii::space_type> factor;
    };
}

///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
int
main()
{
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Expression parser...\n\n";
    std::cout << "/////////////////////////////////////////////////////////\n\n";
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

    boost::spirit::ascii::space_type space;
    typedef std::string::const_iterator iterator_type;
    typedef client::calculator<iterator_type> calculator;
    typedef client::ast::program program;
    typedef client::ast::printer ast_print;
    typedef client::ast::eval ast_eval;

    std::string str;
    while (std::getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        calculator calc;    // Our grammar
        program ast;        // Our AST
        ast_print printer;  // Prints the AST
        ast_eval eval;      // Evaluates the AST

        std::string::const_iterator iter = str.begin();
        std::string::const_iterator end = str.end();
        bool r = phrase_parse(iter, end, calc, space, ast);

        if (r && iter == end)
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
            printer(ast);
            std::cout << "\nResult: " << eval(ast) << std::endl;
            std::cout << "-------------------------\n";
        }
        else
        {
            std::string rest(iter, end);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \": " << rest << "\"\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... :-) \n\n";
    return 0;
}


