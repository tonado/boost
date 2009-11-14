//  Copyright (c) 2001-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <iostream>
#include <map>

namespace client
{
    namespace spirit = boost::spirit;
    namespace qi = boost::spirit::qi;

    template <typename Iterator>
    struct key_value_sequence 
      : qi::grammar<Iterator, std::map<std::string, std::string>()>
    {
        key_value_sequence()
          : key_value_sequence::base_type(query)
        {
            query =  pair >> *((qi::lit(';') | '&') >> pair);
            pair  =  key >> -('=' >> value);
            key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
            value = +qi::char_("a-zA-Z_0-9");
        }

        qi::rule<Iterator, std::map<std::string, std::string>()> query;
        qi::rule<Iterator, std::pair<std::string, std::string>()> pair;
        qi::rule<Iterator, std::string()> key, value;
    };
}

///////////////////////////////////////////////////////////////////////////////
int main()
{
    namespace qi = boost::spirit::qi;

    std::string input("key1=value1;key2;key3=value3");
    std::string::iterator begin = input.begin();
    std::string::iterator end = input.end();

    client::key_value_sequence<std::string::iterator> g;
    std::map<std::string, std::string> m;

    if (!qi::parse(begin, end, g, m))
    {
        std::cout << "-------------------------------- \n";
        std::cout << "Parsing failed\n";
        std::cout << "-------------------------------- \n";
    }
    else
    {
        std::cout << "-------------------------------- \n";
        std::cout << "Parsing succeeded, found entries:\n";
        std::map<std::string, std::string>::iterator end = m.end();
        for (std::map<std::string, std::string>::iterator it = m.begin();
             it != end; ++it)
        {
            std::cout << (*it).first;
            if (!(*it).second.empty())
                std::cout << "=" << (*it).second;
            std::cout << std::endl;
        }
        std::cout << "---------------------------------\n";
    }
    return 0;
}

