// ----------------------------------------------------------------------------
// ******
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_JSON_PARSER_WRITE_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_JSON_PARSER_WRITE_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <ostream>
#include <iomanip>

namespace boost { namespace property_tree { namespace json_parser
{

    // Create necessary escape sequences from illegal characters
    template<class Ch>
    std::basic_string<Ch> create_escapes(const std::basic_string<Ch> &s,
                                         const std::locale &loc)
    {
        std::basic_string<Ch> result;
        typename std::basic_string<Ch>::const_iterator b = s.begin();
        typename std::basic_string<Ch>::const_iterator e = s.end();
        while (b != e)
        {
            if (*b == Ch('\0')) result += Ch('\\'), result += Ch('0');
            else if (*b == Ch('\b')) result += Ch('\\'), result += Ch('b');
            else if (*b == Ch('\f')) result += Ch('\\'), result += Ch('f');
            else if (*b == Ch('\n')) result += Ch('\\'), result += Ch('n');
            else if (*b == Ch('\r')) result += Ch('\\'), result += Ch('r');
            else if (*b == Ch('"')) result += Ch('\\'), result += Ch('"');
            else if (*b == Ch('\\')) result += Ch('\\'), result += Ch('\\');
            else
            {
                if (std::isprint(*b, loc))
                    result += *b;
                else
                {   
                    const char *hexdigits = "0123456789ABCDEF";
                    unsigned long u = (std::min)(static_cast<unsigned long>(*b), 0xFFFFul);
                    int d1 = u / 4096; u -= d1 * 4096;
                    int d2 = u / 256; u -= d2 * 256;
                    int d3 = u / 16; u -= d3 * 16;
                    int d4 = u;
                    result += Ch('\\'); result += Ch('u');
                    result += Ch(hexdigits[d1]); result += Ch(hexdigits[d2]);
                    result += Ch(hexdigits[d3]); result += Ch(hexdigits[d4]);
                }
            }
            ++b;
        }
        return result;
    }

    template<class Ptree>
    void write_json_helper(std::basic_ostream<typename Ptree::key_type::value_type> &stream, 
                           const Ptree &pt, 
                           int indent)
    {

        typedef typename Ptree::key_type::value_type Ch;
        typedef typename std::basic_string<Ch> Str;
        
        // Value or object or array
        if (indent > 0 && pt.empty())
        {
            
            // Write value
            Str data = create_escapes(pt.template get_value<Str>(), stream.getloc());
            stream << Ch('"') << data << Ch('"');

        }
        else if (indent > 0 && pt.count(Str()) == pt.size())
        {
                
            // Write array
            stream << Ch('[') << Ch('\n');
            typename Ptree::const_iterator it = pt.begin();
            for (; it != pt.end(); ++it)
            {
                stream << Str(4 * (indent + 1), Ch(' '));
                write_json_helper(stream, it->second, indent + 1);
                if (boost::next(it) != pt.end())
                    stream << Ch(',');
                stream << Ch('\n');
            }
            stream << Str(4 * indent, Ch(' ')) << Ch(']');

        }
        else
        {
        
            // Write object
            stream << Ch('{') << Ch('\n');
            typename Ptree::const_iterator it = pt.begin();
            for (; it != pt.end(); ++it)
            {
                stream << Str(4 * (indent + 1), Ch(' '));
                stream << Ch('"') << create_escapes(it->first, stream.getloc()) << Ch('"') << Ch(':');
                if (it->second.empty())
                    stream << Ch(' ');
                else
                    stream << Ch('\n') << Str(4 * (indent + 1), Ch(' '));
                write_json_helper(stream, it->second, indent + 1);
                if (boost::next(it) != pt.end())
                    stream << Ch(',');
                stream << Ch('\n');
            }
            stream << Str(4 * indent, Ch(' ')) << Ch('}');

        }

    }

    // Verify if ptree does not contain information that cannot be written to json
    template<class Ptree>
    bool verify_json(const Ptree &pt, int depth)
    {

        typedef typename Ptree::key_type::value_type Ch;
        typedef typename std::basic_string<Ch> Str;

        // Root ptree cannot have data
        if (depth == 0 && !pt.template get_value<Str>().empty())
            return false;
        
        // Ptree cannot have both children and data
        if (!pt.template get_value<Str>().empty() && !pt.empty())
            return false;

        // Check children
        typename Ptree::const_iterator it = pt.begin();
        for (; it != pt.end(); ++it)
            if (!verify_json(it->second, depth + 1))
                return false;

        // Success
        return true;

    }
    
    // Write ptree to json stream
    template<class Ptree>
    void write_json_internal(std::basic_ostream<typename Ptree::key_type::value_type> &stream, 
                             const Ptree &pt,
                             const std::string &filename)
    {
        if (!verify_json(pt, 0))
            throw json_parser_error("ptree contains data that cannot be represented in JSON format", filename, 0);
        write_json_helper(stream, pt, 0);
        stream << std::endl;
        if (!stream.good())
            throw json_parser_error("write error", filename, 0);
    }

} } }

#endif
