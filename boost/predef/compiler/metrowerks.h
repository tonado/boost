/*
Copyright Redshift Software, Inc. 2008-2013
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BOOST_PREDEF_COMPILER_METROWERKS_H
#define BOOST_PREDEF_COMPILER_METROWERKS_H

#include <boost/predef/version_number.h>
#include <boost/predef/make.h>

/*`
[heading `BOOST_COMP_MWERKS`]

[@http://en.wikipedia.org/wiki/CodeWarrior Metrowerks CodeWarrior] compiler.
Version number available as major, minor, and patch.

[table
    [[__predef_symbol__] [__predef_version__]]

    [[`__MWERKS__`] [__predef_detection__]]
    [[`__CWCC__`] [__predef_detection__]]

    [[`__CWCC__`] [V.R.P]]
    [[`__MWERKS__`] [V.R.P >= 4.2.0]]
    [[`__MWERKS__`] [9.R.0]]
    [[`__MWERKS__`] [8.R.0]]
    ]
 */

#define BOOST_COMP_MWERKS BOOST_VERSION_NUMBER_NOT_AVAILABLE

#if defined(__MWERKS__) || defined(__CWCC__)
#   undef BOOST_COMP_MWERKS
#   if !defined(BOOST_COMP_MWERKS) && defined(__CWCC__)
#       define BOOST_COMP_MWERKS BOOST_PREDEF_MAKE_0X_VRPP(__CWCC__)
#   endif
#   if !defined(BOOST_COMP_MWERKS) && (__MWERKS__ >= 0x4200)
#       define BOOST_COMP_MWERKS BOOST_PREDEF_MAKE_0X_VRPP(__MWERKS__)
#   endif
#   if !defined(BOOST_COMP_MWERKS) && (__MWERKS__ >= 0x3204) // note the "skip": 04->9.3
#       define BOOST_COMP_MWERKS BOOST_VERSION_NUMBER(9,(__MWERKS__)%100-1,0)
#   endif
#   if !defined(BOOST_COMP_MWERKS) && (__MWERKS__ >= 0x3200)
#       define BOOST_COMP_MWERKS BOOST_VERSION_NUMBER(9,(__MWERKS__)%100,0)
#   endif
#   if !defined(BOOST_COMP_MWERKS) && (__MWERKS__ >= 0x3000)
#       define BOOST_COMP_MWERKS BOOST_VERSION_NUMBER(8,(__MWERKS__)%100,0)
#   endif
#   if !defined(BOOST_COMP_MWERKS)
#       define BOOST_COMP_MWERKS BOOST_VERSION_NUMBER_AVAILABLE
#   endif
#endif

#if BOOST_COMP_MWERKS
#   define BOOST_COMP_MWERKS_AVAILABLE
#endif

#define BOOST_COMP_MWERKS_NAME "Metrowerks CodeWarrior"

#include <boost/predef/detail/test.h>
BOOST_PREDEF_DECLARE_TEST(BOOST_COMP_MWERKS,BOOST_COMP_MWERKS_NAME)


#endif
