/*
Copyright Redshift Software, Inc. 2008-2013
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_COMPILER_IAR_H
#define BOOST_PREDEF_COMPILER_IAR_H

#include <boost/predef/version_number.h>
#include <boost/predef/make.h>

/*`
[heading `BOOST_COMP_IAR`]

IAR C/C++ compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__IAR_SYSTEMS_ICC__`] [__predef_detection__]]

    [[`__VER__`] [V.R.P]]
    ]
 */

#define BOOST_COMP_IAR BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__IAR_SYSTEMS_ICC__)
#   undef BOOST_COMP_IAR
#   define BOOST_COMP_IAR BOOST_PREDEF_MAKE_10_VVRR(__VER__)
#endif

#if BOOST_COMP_IAR
#   define BOOST_COMP_IAR_AVAILABLE
#endif

#define BOOST_COMP_IAR_NAME "IAR C/C++"

#include <boost/predef/detail/test.h>
BOOST_PREDEF_DECLARE_TEST(BOOST_COMP_IAR,BOOST_COMP_IAR_NAME)


#endif
