#ifndef BOOST_ARCHIVE_XML_WIARCHIVE_HPP
#define BOOST_ARCHIVE_XML_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// xml_wiarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>
#ifndef BOOST_NO_STD_WSTREAMBUF

#include <istream>

//#include <boost/scoped_ptr.hpp>
#include <boost/pfto.hpp>
#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_xml_iarchive.hpp>

namespace boost { 
namespace archive {

template<class CharType>
class basic_xml_grammar;
typedef basic_xml_grammar<wchar_t> xml_wgrammar;

template<class Archive>
class xml_wiarchive_impl : 
    public basic_text_iprimitive<std::wistream>,
    public basic_xml_iarchive<Archive>
{
protected:
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class detail::interface_iarchive<Archive>;
    friend class basic_xml_iarchive<Archive>;
    friend class load_access;
#endif
    // instances of micro xml parser to parse start preambles
    // scoped_ptr doesn't play nice with borland - so use a naked pointer
    // scoped_ptr<xml_wgrammar> gimpl;
    xml_wgrammar *gimpl;
    std::wistream & get_is(){
        return is;
    }
    template<class T>
    void load(T & t){
        basic_text_iprimitive<std::wistream>::load(t);
    }
    void load(char * t);
    #ifndef BOOST_NO_INTRINSIC_WCHAR_T
    void load(wchar_t * t);
    #endif
    void load(std::string &s);
    #ifndef BOOST_NO_STD_WSTRING
    void load(std::wstring &ws);
    #endif
    // return a pointer to the most derived class
    Archive * This(){
        return static_cast<Archive *>(this);
    }
    template<class T>
    void load_override(T & t, BOOST_PFTO int){
        basic_xml_iarchive<Archive>::load_override(t, 0);
    }
    void load_override(class_name_type & t, int);
protected:
    void init();
    xml_wiarchive_impl(std::wistream & is, unsigned int flags = 0) ;
    ~xml_wiarchive_impl();
};

// we use the following because we can't use
// typedef xml_wiarchive_impl<xml_wiarchive_impl<...> > xml_wiarchive;

// do not derive from this class.  If you want to extend this functionality
// via inhertance, derived from xml_wiarchive_impl instead.  This will
// preserve correct static polymorphism.
class xml_wiarchive : 
    public xml_wiarchive_impl<xml_wiarchive>
{
public:
    xml_wiarchive(std::wistream & is, unsigned int flags = 0) :
        xml_wiarchive_impl<xml_wiarchive>(is, flags)
    {}
};

} // namespace archive
} // namespace boost

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_XML_WIARCHIVE_HPP
