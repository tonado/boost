// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef PARSERS_VP_2003_05_19
#define PARSERS_VP_2003_05_19

#include <boost/program_options/config.hpp>
#include <boost/program_options/option.hpp>

#define DECL BOOST_PROGRAM_OPTIONS_DECL

#include <boost/function/function1.hpp>

#include <iosfwd>
#include <vector>
#include <utility>

namespace boost { namespace program_options {

    class options_description;
    class positional_options_description;


    /** Results of parsing an input source. 
        The primary use of this class is passing information from parsers 
        component to value storage component. This class does not makes
        much sense itself.        
    */
    template<class charT>
    class basic_parsed_options {
    public:
        explicit basic_parsed_options(const options_description* description) 
        : description(description) {}
        /** Options found in the source. */
        std::vector< basic_option<charT> > options;
        /** Options description that was used for parsing. 
            Parsers should return pointer to the instance of 
            option_description passed to them, and issues of lifetime are
            up to the caller. Can be NULL.
         */
        const options_description* description;
    };

    /** Specialization of basic_parsed_options which:
        - provides convenient conversion from basic_parsed_options<char>
        - stores the passed char-based options for later use.
    */
    template<>
    class DECL basic_parsed_options<wchar_t> {
    public:
        /** Constructs wraped_options from options in UTF8 encoding. */
        explicit basic_parsed_options(const basic_parsed_options<char>& po);

        std::vector< basic_option<wchar_t> > options;
        const options_description* description;

        /** Stores UTF8 encoded options that were passed to constructor,
            to avoid reverse conversion in some cases. */
        basic_parsed_options<char> utf8_encoded_options;        
    };

    typedef basic_parsed_options<char> parsed_options;
    typedef basic_parsed_options<wchar_t> wparsed_options;

    /** Augments basic_parsed_options<wchar_t> with conversion from
        'parsed_options' */


    typedef function1<std::pair<std::string, std::string>, const std::string&> ext_parser;

    /** Character-type independent command line parser. */
    class DECL common_command_line_parser {
    public:
        /// Creates the parsers. The arguments must be in internal encoding.
        common_command_line_parser(const std::vector<std::string>& args);
        /// Parses the command line and returns parsed options in internal
        /// encoding.
        parsed_options run() const;
    protected:
        int m_style;
        const options_description* m_desc;
        const positional_options_description* m_positional;
        function1<std::pair<std::string, std::string>, const std::string&> m_ext;

        // Intentionally independent from charT
        std::vector<std::string> m_args;
    };

    /** Comamnd line parser.

        The class allows to specify all the information needed for parsing and
        the parse the command line. It is primarily needed to emulate named 
        function parameters -- a regular function with 5 parameters will be hard
        to use and creating overloads with smaller nuber of parameters will be 
        confusing.

        For the most common case, the function parse_command_line is a better 
        alternative.        
    */
    template<class charT>
    class basic_command_line_parser : private common_command_line_parser {
    public:
        /** Creates a command line parser for the specified arguments
            list. The 'args' parameter should not include program name.
        */
        basic_command_line_parser(const std::vector<
                                  std::basic_string<charT> >& args);
        /** Creates a command line parser for the specified arguments
            list. The parameter should be the same as passes to 'main'.
        */
        basic_command_line_parser(int argc, charT* argv[]);

        basic_command_line_parser& options(const options_description& desc);
        basic_command_line_parser& positional(
            const positional_options_description& desc);

        basic_command_line_parser& style(int);
        basic_command_line_parser& extra_parser(ext_parser);
        
        basic_parsed_options<charT> run() const;
    private:
    };

    typedef basic_command_line_parser<char> command_line_parser;
    typedef basic_command_line_parser<wchar_t> wcommand_line_parser;

    /** Creates instance of 'command_line_parser', passes parameters to it,
        and returns the result of calling the 'run' method.        
     */
    template<class charT>
    basic_parsed_options<charT>
    parse_command_line(int argc, charT* argv[],
                       const options_description&,
                       int style = 0,
                       function1<std::pair<std::string, std::string>, 
                                 const std::string&> ext
                       = ext_parser());

    /** Parse a config file. 
    */
    template<class charT>
    basic_parsed_options<charT>
    parse_config_file(std::basic_istream<charT>&, const options_description&);

    /** Parse environment. 

        For each environment variable, the 'name_mapper' function is called to
        obtain the option name. If it returns empty string, the variable is 
        ignored. 

        This is done since naming of environment variables is typically 
        different from the naming of command line options.        
    */
    DECL parsed_options
    parse_environment(const options_description&, 
                      const function1<std::string, std::string>& name_mapper);

    /** Parse environment.

        Takes all environment variables which start with 'prefix'. The option
        name is obtained from variable name by removing the prefix and 
        converting the remaining string into lower case.
    */
    DECL parsed_options
    parse_environment(const options_description&, const std::string& prefix);

    /** @overload
        This function exists for resolve ambiguity between the two above 
        functions when second argument is of 'char*' type. There's implicit
        convension to both function1 and string.
    */
    DECL parsed_options
    parse_environment(const options_description&, const char* prefix);

}}

#undef DECL

#include "detail/parsers.hpp"

#endif
