//  (C) Copyright John Maddock 2001. 
// (C) Copyright Bryce Lelbach 2010
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  BeOS specific config options:

#define BOOST_PLATFORM "BeOS"

#define BOOST_BEOS 1

#define BOOST_NIX            1 
//#define BOOST_GENETIC_NIX    1
//#define BOOST_TRADEMARK_NIX  1
#define BOOST_FUNCTIONAL_NIX 1

#define BOOST_NO_CWCHAR
#define BOOST_NO_CWCTYPE
#define BOOST_HAS_UNISTD_H

#define BOOST_HAS_BETHREADS

#ifndef BOOST_DISABLE_THREADS
#  define BOOST_HAS_THREADS
#endif

// boilerplate code:
#include <boost/config/posix_features.hpp>
 


