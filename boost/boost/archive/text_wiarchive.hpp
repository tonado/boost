#ifndef BOOST_ARCHIVE_TEXT_WIARCHIVE_HPP
#define BOOST_ARCHIVE_TEXT_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// text_wiarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>
#ifndef BOOST_NO_STD_WSTREAMBUF

#include <istream>

#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_text_iarchive.hpp>

namespace boost { 
namespace archive {

template<class Archive>
class text_wiarchive_impl : 
    public basic_text_iprimitive<std::wistream>,
    public basic_text_iarchive<Archive>
{
protected:
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class basic_text_iarchive<Archive>;
    friend class load_access;
#endif
    // return a pointer to the most derived class
    Archive * This(){
        return static_cast<Archive *>(this);
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
protected:
    text_wiarchive_impl(std::wistream & is, unsigned int flags = 0) :
        basic_text_iprimitive<std::wistream>(
            is, 
            0 != (flags & no_codecvt)
        )
    {}
};

// do not derive from this class.  If you want to extend this functionality
// via inhertance, derived from text_wiarchive_impl instead.  This will
// preserve correct static polymorphism.
class text_wiarchive : public text_wiarchive_impl<text_wiarchive>
{
public:
    text_wiarchive(std::wistream & is, unsigned int flags = 0) :
        text_wiarchive_impl<text_wiarchive>(is, flags | no_header)
    {
        if(0 == (flags & no_header))
            basic_text_iarchive<text_wiarchive>::init();
    }
};

} // namespace archive
} // namespace boost

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_TEXT_WIARCHIVE_HPP
