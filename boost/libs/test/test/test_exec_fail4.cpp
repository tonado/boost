//  (C) Copyright Gennadiy Rozental 2001-2002.
//  (C) Copyright Beman Dawes 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description : test failures reported by thgrowing the exception.
//  Should fail during run.
// ***************************************************************************

// Boost.Test
#include <boost/test/test_tools.hpp>

int test_main( int, char* [] )  // note the name
{
    BOOST_ERROR( "Msg" );

    throw "Test error by throwing C-style string exception";

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.6  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
