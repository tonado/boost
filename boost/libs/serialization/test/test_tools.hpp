#ifndef BOOST_SERIALIZATION_TEST_TOOLS_HPP
#define BOOST_SERIALIZATION_TEST_TOOLS_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_tools.hpp
//
// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>

#if defined(BOOST_MSVC)

// VC has a really nice free memory leak detector
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

struct leak_reporter {
    static leak_reporter instance;
    leak_reporter(){
        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    }
    ~leak_reporter(){
//		_CrtDumpMemoryLeaks(); // replaced by the above
    }
};
leak_reporter leak_reporter::instance;

#endif

// these compilers have brain-dead tmpnam functions.
// regardless of environmental settings
// VC which leaves temp files in root directory 
// and BORLAND leaves them in the current directory
#if defined(BOOST_MSVC) || defined(__BORLANDC__)

#include <cstdlib>
#include <cstring>
#include <cstdio>

#if defined(__BORLANDC__) 
namespace { 
    using std::_tempnam;
    using std::getenv;
	// borland defines strcpy as a macro in release mode
    #if ! defined(NDEBUG)
    using std::strcpy;
    #endif
}
#endif

char * tmpnam(char * buffer){
    static char ibuffer [256];
    char *libvar;
    libvar = getenv("TMP");
    if(NULL == libvar)
        libvar = getenv("TMPDIR");
    if(NULL == libvar)
        libvar = getenv("TEMP");
    if(NULL == libvar)
        libvar = "C:/TMP";
    char * tfilename = _tempnam(libvar, "ser");
    if(NULL == buffer){
        strcpy(ibuffer, tfilename);
        delete tfilename;
        return ibuffer;
    }
    else{
        strcpy(buffer, tfilename);
        delete tfilename;
        return buffer;
    }
}

#endif

// invoke header for a custom archive test.
#if ! defined(BOOST_ARCHIVE_TEST)
#define BOOST_ARCHIVE_TEST text_archive.hpp
#endif

#include <boost/preprocessor/stringize.hpp>
#include BOOST_PP_STRINGIZE(BOOST_ARCHIVE_TEST)

#include <boost/test/test_tools.hpp>

#endif // BOOST_SERIALIZATION_TEST_TOOLS_HPP
