/*=============================================================================
    Spirit v1.6.2
    Copyright (c) 2002 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at 
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "quickdoc.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
using namespace std;
using namespace quickdoc;
using namespace boost::spirit;

void navigator(ostream& out, char const* prev, char const* next)
{
    out << "<table border=\"0\">\n"
        << "  <tr>\n"
        << "    <td width=\"30\"><a href=\"../index.html\"><img src=\"theme/u_arr.gif\" border=\"0\"></a></td>\n"
        ;

    if (prev)
        out << "    <td width=\"30\"><a href=\"" << prev << "\"><img src=\"theme/l_arr.gif\" border=\"0\"></a></td>\n";
    else
        out << "    <td width=\"30\"><img src=\"theme/l_arr_disabled.gif\" border=\"0\"></td>\n";

    if (next)
        out << "    <td width=\"20\"><a href=\"" << next << "\"><img src=\"theme/r_arr.gif\" border=\"0\"></a></td>\n";
    else
        out << "    <td width=\"20\"><img src=\"theme/r_arr_disabled.gif\" border=\"0\"></td>\n";

    out << "   </tr>\n"
        << "</table>\n"
        ;
}

void title_bar(ostream& out, char const* title, char const* prev, char const* next, bool is_index = false)
{
    out << "<html>\n" << "<head>\n"
        << "<!-- Generated by the Spirit (http://spirit.sf.net) QuickDoc -->\n"
        << "<title>" << title << "</title>\n"
        << "<link rel=\"stylesheet\" href=\"" << (is_index ? "doc/" : "")
        << "theme/style.css\" type=\"text/css\">\n"
        ;

    if (prev)
        out << "<link rel=\"prev\" href=\"" << prev << "\">\n";
    if (next)
        out << "<link rel=\"next\" href=\"" << next << "\">\n";

    out << "</head>\n"
        << "<body>\n"
        << "<table width=\"100%\" height=\"48\" border=\"0\" background=\"" << (is_index ? "doc/" : "")
        << "theme/bkd2.gif\" cellspacing=\"2\">\n"
        << "  <tr>\n"
        << "    <td width=\"10\">\n"
        << "    </td>\n"
        << "    <td width=\"85%\">\n"
        << "      <font size=\"6\" face=\"Verdana, Arial, Helvetica, sans-serif\"><b>" << title << "</b></font>\n"
        << "    </td>\n"
        << "    <td width=\"112\"><a href=\"http://spirit.sf.net\">"
        << "<img src=\"" << (is_index ? "doc/" : "") << "theme/spirit.gif\""
        << " align=\"right\" border=\"0\"></a></td>\n"
        << "  </tr>\n"
        << "</table>\n"
        << "<br>\n"
        ;
}

void basic_footer(ostream& out)
{
    out << "<br>\n"
        << "<hr size=\"1\">"
        << "<p class=\"copyright\">Copyright &copy; 2001-2002 Joel de Guzman<br><br>\n"
        << "<font size=\"2\">Distributed under the Boost Software License, Version 1.0.\n"
        << " (See accompanying file LICENSE_1_0.txt or copy at\n"
        << " <a href=\"http://www.boost.org/LICENSE_1_0.txt\">\n"
        << "http://www.boost.org/LICENSE_1_0.txt</a>\n"
        << "</body>\n"
        << "</html>\n"
        ;
}

void start_page(ostream& out, char const* title, char const* prev, char const* next)
{
    title_bar(out, title, prev, next);
    navigator(out, prev, next);
}

void end_page(ostream& out, char const* prev, char const* next)
{
    navigator(out, prev, next);
    basic_footer(out);
}

namespace
{
    template <typename CharT>
    void print_char(CharT ch, ostream& out)
    {
        switch (ch)
        {
            case '<': out << "&lt;";    break;
            case '>': out << "&gt;";    break;
            case '&': out << "&amp;";   break;
            case '"': out << "&quot;";  break;
            default:  out << ch;        break;
        }
    }

    struct filter_identifier_char
    {
        template <typename CharT>
        CharT operator()(CharT ch) const
        {
            if (!isalnum(ch))
                ch = '_';
            return tolower(ch);
        }
    };

    template <typename IteratorT>
    string make_identifier(IteratorT const& first, IteratorT const& last)
    {
        string out_name;
#if (defined(BOOST_INTEL_CXX_VERSION) && (BOOST_INTEL_CXX_VERSION <= 500)) \
    || defined(__GNUC__) && (__GNUC__ == 2)
        filter_identifier_char filter;
        for (IteratorT i = first; i != last; ++i)
            out_name += filter(*i);
#else
        transform(first, last,
            back_insert_iterator<string>(out_name), filter_identifier_char());
#endif
        return out_name;
    }

    template <typename IteratorT>
    string make_filename(IteratorT const& first, IteratorT const& last, char const* ext)
    {
        return make_identifier(first, last) + ext;
    }
}

struct wrap_action
{
    wrap_action(
        ostream&        out_,
        stringstream&   phrase_,
        string const&   pre_,
        string const&   post_,
        bool            anchor_ = false)
    : out(out_), phrase(phrase_), pre(pre_), post(post_), anchor(anchor_) {}

    template <typename IteratorT>
    void operator()(IteratorT const& /*first*/, IteratorT const& /*last*/) const
    {
        if (out)
        {
            string  str = phrase.str();
            if (anchor)
                out << "<a name=\"" << make_identifier(str.begin(), str.end()) << "\"></a>";
            phrase.str(string());
            out << pre << str << post;
        }
    }

    ostream&        out;
    stringstream&   phrase;
    string          pre;
    string          post;
    bool            anchor;
};

struct process
{
    process(char const* name_, ostream& out_)
    : name(name_), out(out_) {}

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
        if (out)
        {
            out << "<span class=" << name << ">";
            while (first != last)
                print_char(*first++, out);
            out << "</span>";
        }
    }

    char const* name;
    ostream&    out;
};

struct unexpected_char
{
    unexpected_char(ostream& out_)
    : out(out_) {}

    template <typename CharT>
    void operator()(CharT) const
    {
        if (out)
            out << '#'; // print out an unexpected character
    }

    ostream& out;
};

struct code_action
{
    code_action(ostream& out_)
    : out(out_), code_p(out_) {}

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
        if (out)
        {
            out << "<code><pre>\n";
            while (first != last && isspace(*first))    //  print leading spaces
                print_char(*first++, out);
            parse(first, last, code_p);
            out << "</pre></code>\n";
        }
    }

    ostream& out;
    code_to_html<process, unexpected_char, ostream> code_p;
};

struct actions; // forward

struct page_action
{
    page_action(actions& actor_)
    : actor(actor_) {}

    ~page_action()
    {
        build_page();
    }

    template <typename IteratorT>
    void operator()(IteratorT const& first, IteratorT const& last) const;
    void build_page(char const* next = 0) const;

    actions& actor;
};

namespace
{
    const char* paragraph_pre   = "<p>\n";
    const char* paragraph_post  = "</p>\n";
    const char* h1_pre          = "<h1>";
    const char* h1_post         = "</h1>";
    const char* h2_pre          = "<h2>";
    const char* h2_post         = "</h2>";
    const char* h3_pre          = "<h3>";
    const char* h3_post         = "</h3>";
    const char* h4_pre          = "<h4>";
    const char* h4_post         = "</h4>";
    const char* h5_pre          = "<h5>";
    const char* h5_post         = "</h5>";
    const char* h6_pre          = "<h6>";
    const char* h6_post         = "</h6>";
    const char* hr_             = "<hr>";

    const char* blurb_pre =
        "<table width=\"80%\" border=\"0\" align=\"center\">\n"
        "  <tr>\n"
        "    <td class=\"note_box\">\n"
        ;

    const char* blurb_post =
        "    </td>\n"
        "  </tr>\n"
        "</table>\n"
        ;

    const char* blockquote_pre      = "<blockquote><p>";
    const char* blockquote_post     = "</p></blockquote>";
    const char* preformatted_pre    = "<code><pre>";
    const char* preformatted_post   = "</pre></code>";
    const char* list_item_pre       = "<li>";
    const char* list_item_post      = "</li>";
    const char* unordered_list_pre  = "<ul>";
    const char* unordered_list_post = "</ul>";
    const char* ordered_list_pre    = "<ol>";
    const char* ordered_list_post   = "</ol>";
    const char* bold_pre_           = "<b>";
    const char* bold_post_          = "</b>";
    const char* italic_pre_         = "<i>";
    const char* italic_post_        = "</i>";
    const char* underline_pre_      = "<u>";
    const char* underline_post_     = "</u>";
    const char* teletype_pre_       = "<tt>";
    const char* teletype_post_      = "</tt>";
    const char* break_mark          = "<br>";
    const char* link_post_          = "</a>";
    const char* start_row_          = "<tr>";
    const char* end_row_            = "</tr>";
    const char* start_cell_         = "<td class=\"table_cells\">";
    const char* end_cell_           = "</td>";
}

struct plain_char_action
{
    plain_char_action(stringstream& phrase_)
    : phrase(phrase_) {}

    template <typename IteratorT>
    void operator()(IteratorT const& first, IteratorT const& /*last*/) const
    {
        print_char(*first, phrase);
    }

    stringstream& phrase;
};

struct image_action
{
    image_action(stringstream& phrase_)
    : phrase(phrase_) {}

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
        phrase << "<img src=\"";
        while (first != last)
            print_char(*first++, phrase);
        phrase << "\"></img>";
    }

    stringstream& phrase;
};

struct markup_action
{
    markup_action(stringstream& phrase_, string const& str_)
    : phrase(phrase_), str(str_) {}

    template <typename T>
    void operator()(T const&) const
    { phrase << str; }

    template <typename T>
    void operator()(T const&, T const&) const
    { phrase << str; }

    stringstream& phrase;
    string str;
};

struct page_level_action
{
    page_level_action(unsigned& page_level_)
    : page_level(page_level_) {}

    void operator()(unsigned page_level_) const
    { page_level = page_level_; }

    unsigned& page_level;
};

template <typename T>
class assign_action_
{
public:

    explicit
    assign_action_(T& ref_)
    : ref(ref_) {}

    template <typename T2>
    void operator()(T2 const& val) const
    { ref = val; }

    template <typename IteratorT>
    void operator()(IteratorT const& first, IteratorT const& last) const
    {
#ifdef __BORLANDC__ //  Should have used boost::spirit::assign_actor but
                    //  Borland has a linker error BC it couldn't find
                    //  the string::replace code when IteratorT is a
                    //  file_iterator
        ref.clear();
        copy(first, last, back_insert_iterator<T>(ref));
#else
        ref.assign(first, last);
#endif
    }

private:

    T& ref;
};

namespace
{
    char const*
    rightmost_slash(char const* first, char const* last)
    {
        //  This is a hack, should use boost::file_system perhaps
        if (first != last)
        {
            --last;
            while ((first != last) && (*last != '\\') && (*last != '/'))
                --last;
            ++last;
        }
        return last;
    }

    char const*
    actual_filename(char const* filename)
    {
        return rightmost_slash(filename, filename + strlen(filename));
    }
}

struct indentifier_action
{
    indentifier_action(actions& actor_)
    : actor(actor_) {}

    template <typename IteratorT>
    void operator()(IteratorT const& first, IteratorT const& last) const;

    actions& actor;
};

struct macro_def_action
{
    macro_def_action(actions& actor_)
    : actor(actor_) {}

    template <typename IteratorT>
    void operator()(IteratorT const&, IteratorT const&) const;

    actions& actor;
};

struct do_macro_action
{
    do_macro_action(stringstream& phrase_)
    : phrase(phrase_) {}

    void operator()(string const& str) const
    {
        phrase << str;
    }

    stringstream& phrase;
};

struct link_action
{
    link_action(stringstream& phrase_)
    : phrase(phrase_) {}

    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
        phrase << "<a href=\"";
        while (first != last)
            print_char(*first++, phrase);
        phrase << "\">\n";
    }

    stringstream& phrase;
};

struct table_action
{
    table_action(actions& actor_)
    : actor(actor_) {}

    template <typename IteratorT>
    void operator()(IteratorT, IteratorT) const;

    actions& actor;
};

struct start_col_action
{
    start_col_action(stringstream& phrase_, unsigned& span_)
    : phrase(phrase_), span(span_) {}

    template <typename T>
    void operator()(T const&) const
    { phrase << start_cell_; ++span; }

    stringstream& phrase;
    unsigned& span;
};

struct unexpected_action
{
    template <typename IteratorT>
    void operator()(IteratorT first, IteratorT last) const
    {
        cerr << "Unexpected markup: [";
        while (first != last)
            cerr << *first++;
        cerr << "]\n";
    }
};

struct actions
{
    actions(char const* filename_)
    : filename(filename_)
    , page_level_n(0)
    , table_span(0)
    , page(*this)
    , page_level(page_level_n)
    , code(buffer)
    , paragraph(buffer, phrase, paragraph_pre, paragraph_post)
    , h1(buffer, phrase, h1_pre, h1_post, true)
    , h2(buffer, phrase, h2_pre, h2_post, true)
    , h3(buffer, phrase, h3_pre, h3_post, true)
    , h4(buffer, phrase, h4_pre, h4_post, true)
    , h5(buffer, phrase, h5_pre, h5_post, true)
    , h6(buffer, phrase, h6_pre, h6_post, true)
    , hr(buffer, hr_)
    , blurb(buffer, phrase, blurb_pre, blurb_post)
    , blockquote(buffer, phrase, blockquote_pre, blockquote_post)
    , preformatted(buffer, phrase, preformatted_pre, preformatted_post)
    , plain_char(phrase)
    , image(phrase)
    , list_item(list_buffer, phrase, list_item_pre, list_item_post)
    , unordered_list(buffer, list_buffer, unordered_list_pre, unordered_list_post)
    , ordered_list(buffer, list_buffer, ordered_list_pre, ordered_list_post)
    , bold_pre(phrase, bold_pre_)
    , bold_post(phrase, bold_post_)
    , italic_pre(phrase, italic_pre_)
    , italic_post(phrase, italic_post_)
    , underline_pre(phrase, underline_pre_)
    , underline_post(phrase, underline_post_)
    , teletype_pre(phrase, teletype_pre_)
    , teletype_post(phrase, teletype_post_)
    , break_(phrase, break_mark)
    , doc_title(doc_title_str)
    , identifier(*this)
    , macro_def(*this)
    , do_macro(phrase)
    , link_pre(phrase)
    , link_post(phrase, link_post_)
    , table_title(table_title_str)
    , table(*this)
    , start_row(phrase, start_row_)
    , end_row(phrase, end_row_)
    , start_cell(phrase, table_span)
    , end_cell(phrase, end_cell_)
    {
        //  This is a hack, should use boost::file_system perhaps
        char const* last = actual_filename(filename);
        directory = string(filename, last);
        cout << "Output directory: " << directory << endl;
    }

    char const*             filename;
    string                  directory;
    string                  macro_id;
    string                  phrase_save;
    string                  table_title_str;
    vector<string>          files;
    vector<string>          titles;
    vector<unsigned>        page_levels;
    stringstream            buffer;
    ofstream                out;
    string                  doc_title_str;
    string                  title;
    string                  previous;
    stringstream            phrase;
    stringstream            list_buffer;
    unsigned                page_level_n;
    unsigned                table_span;

    page_action             page;
    page_level_action       page_level;
    code_action             code;
    wrap_action             paragraph, h1, h2, h3, h4, h5, h6;
    markup_action           hr;
    wrap_action             blurb, blockquote, preformatted;
    plain_char_action       plain_char;
    image_action            image;
    wrap_action             list_item;
    wrap_action             unordered_list;
    wrap_action             ordered_list;
    markup_action           bold_pre;
    markup_action           bold_post;
    markup_action           italic_pre;
    markup_action           italic_post;
    markup_action           underline_pre;
    markup_action           underline_post;
    markup_action           teletype_pre;
    markup_action           teletype_post;
    markup_action           break_;
    assign_action_<string>  doc_title;
    symbols<string>         macro;
    indentifier_action      identifier;
    macro_def_action        macro_def;
    do_macro_action         do_macro;
    link_action             link_pre;
    markup_action           link_post;
    assign_action_<string>  table_title;
    table_action            table;
    markup_action           start_row;
    markup_action           end_row;
    start_col_action        start_cell;
    markup_action           end_cell;
    unexpected_action       unexpected;
};

template <typename IteratorT>
void table_action::operator()(IteratorT, IteratorT) const
{
    if (!!actor.buffer)
    {
        actor.buffer << "<table width=\"90%\" border=\"0\" align=\"center\">";
        actor.buffer << "  <tr>\n";
        actor.buffer << "  <td class=\"table_title\" colspan=\"";
        actor.buffer << actor.table_span << "\">\n";

        string::iterator first = actor.table_title_str.begin();
        string::iterator last = actor.table_title_str.end();
        while (first != last)
            print_char(*first++, actor.buffer);

        actor.buffer << "  </td>\n";
        actor.buffer << "  </tr>\n";

        string  str = actor.phrase.str();
        actor.phrase.str(string());
        actor.buffer << str;

        actor.buffer << "</table>\n";
        actor.table_span = 0;
    }
}

template <typename IteratorT>
void macro_def_action::operator()(IteratorT const&, IteratorT const&) const
{
    actor.macro.add(actor.macro_id.begin(), actor.macro_id.end(), actor.phrase.str());
    actor.phrase.str(actor.phrase_save);
}

template <typename IteratorT>
void indentifier_action::operator()(IteratorT const& first, IteratorT const& last) const
{
#ifdef __BORLANDC__ //  Should have used boost::spirit::assign_actor but
                //  Borland has a linker error BC it couldn't find
                //  the string::replace code when IteratorT is a
                //  file_iterator
    actor.macro_id.clear();
    copy(first, last, back_insert_iterator<string>(actor.macro_id));
#else
    actor.macro_id.assign(first, last);
#endif

    actor.phrase_save = actor.phrase.str();
    actor.phrase.str(string());
}

template <typename IteratorT>
void page_action::operator()(IteratorT const& first, IteratorT const& last) const
{
    actor.page_levels.push_back(actor.page_level_n);
    actor.page_level_n = 0;

    string out_name = make_filename(first, last, ".html");
    build_page(out_name.c_str());
    cout << "Building " << out_name << endl;
    out_name = actor.directory + out_name;
    actor.out.open(out_name.c_str());

    if (!actor.out)
    {
        cerr << "Could not open output file: " << out_name << endl;
    }
    else
    {
        if (!actor.title.empty())
            actor.previous = make_filename(actor.title.begin(), actor.title.end(), ".html");
#ifdef __BORLANDC__
        actor.title.clear();
        copy(first, last, back_insert_iterator<string>(actor.title));
#else
        actor.title.assign(first, last);
#endif
        actor.titles.push_back(actor.title);
        actor.buffer.str(string());
    }
}

void page_action::build_page(char const* next) const
{
    string buffer = actor.buffer.str();

    if (actor.out.is_open())
    {
        start_page(
            actor.out, actor.title.c_str(),
            actor.previous.empty() ? 0 : actor.previous.c_str(),
            next);

        actor.out << buffer;

        end_page(
            actor.out,
            actor.previous.empty() ? 0 : actor.previous.c_str(),
            next);

        actor.out.close();
    }

    else if (!buffer.empty() && next)
    {
        cerr    << "Some contents are lost before file \"" << next
                << "\". Missing page directive." << endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Parse a file
//
///////////////////////////////////////////////////////////////////////////////
void toc_start(ostream& out)
{
    out << "<table width=\"80%\" border=\"0\" align=\"center\">\n"
        << "  <tr>\n"
        << "    <td class=\"toc_title\">Table of contents</td>\n"
        << "  </tr>\n"
        ;
}

void toc_end(ostream& out)
{
    out << "</table>\n";
}

struct build_toc_entries
{
    build_toc_entries(ostream& out_, unsigned const* levels_)
    : out(out_), levels(levels_) {}

    void operator()(string const& str) const
    {
        string filename =
            make_filename(str.begin(), str.end(), ".html");

        out << "  <tr>\n"
            << "    <td class=\"toc_cells_L" << *levels << "\">\n"
            << "        <a href=\"doc/" << filename << "\">" << str << "</a>\n"
            << "    </td>\n"
            << "  </tr>\n"
            ;
        ++levels;
    }

    ostream& out;
    mutable unsigned const* levels;
};

static int
parse(char const* filename)
{
    ifstream in(filename);

    if (!in)
    {
        cerr << "Could not open input file: " << filename << endl;
        return 1;
    }

    in.unsetf(ios::skipws); //  Turn of white space skipping on the stream

    vector<char> vec;
    std::copy(
        istream_iterator<char>(in),
        istream_iterator<char>(),
        std::back_inserter(vec));

    //  ensure that we have enough trailing newlines to eliminate
    //  the need to check for end of file in the grammar.
    vec.push_back('\n');
    vec.push_back('\n');

    vector<char>::const_iterator first = vec.begin();
    vector<char>::const_iterator last = vec.end();

    actions actor(filename);
    quickdoc_grammar<actions> g(actor);

    parse_info<vector<char>::const_iterator> info = parse(first, last, g);
    if (info.full)
    {
        if (!actor.directory.empty() && !actor.titles.empty())
        {
            //  Build the index
            char const* dir_begin = &*actor.directory.begin();
            char const* dir_end = &*(actor.directory.end()-1);
            string name(dir_begin, rightmost_slash(dir_begin, dir_end));
            name += "index.html";
            ofstream index(name.c_str());
            if (index)
            {
                cout << "Building Index " << "../index.html" << endl;
                string filename =
                    make_filename(
                        actor.titles[0].begin(),
                        actor.titles[0].end(), ".html");
                title_bar(index, actor.doc_title_str.c_str(), 0, filename.c_str(), true);
                toc_start(index);
                for_each(actor.titles.begin(), actor.titles.end(),
                    build_toc_entries(index, &*actor.page_levels.begin()));
                toc_end(index);
                basic_footer(index);
            }
        }
    }
    else
    {
        cerr << "Parse failed!\n";
        for (int i = 0; i < 255; ++i)
            if (info.stop == last)
                break;
            else
                cerr << *info.stop++;
        return -1;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Main program
//
///////////////////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    if (argc > 1)
        return parse(argv[1]);
    else
        cerr << "---NO FILENAME GIVEN---" << endl;

    return 0;
}

