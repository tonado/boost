//  (C) Copyright Gennadiy Rozental 2001-2002.
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
//  Description : defines class unit_test_result that is responsible for 
//  gathering test results and presenting this information to end-user
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_RESULT_HPP
#define BOOST_UNIT_TEST_RESULT_HPP

// Boost.Test
#include <boost/test/detail/unit_test_config.hpp>

// BOOST
#include <boost/shared_ptr.hpp>

// STL
#include <iosfwd>   // for std::ostream&

namespace boost {

namespace unit_test_framework {

// ************************************************************************** //
// **************               unit_test_result               ************** //
// ************************************************************************** //

class unit_test_result {
    friend struct unit_test_result_saver;
public:
    // Destructor
    ~unit_test_result();

    // current test results access and management
    static unit_test_result& instance();
    static void     test_case_start( c_string_literal name_, unit_test_counter expected_failures_ = 0 );
    static void     test_case_end();
    
    // use to dynamically change amount of errors expected in current test case
    void            increase_expected_failures( unit_test_counter amount = 1 );

    // reporting
    void            confirmation_report( std::ostream& where_to_ );                 // shortest
    void            short_report( std::ostream& where_to_, size_t indent_ = 0 );    // short
    void            detailed_report( std::ostream& where_to_, size_t indent_ = 0 ); // long
    int             result_code();                                                  // to be returned from main

    // to be used by tool box implementation
    void            inc_failed_assertions();
    void            inc_passed_assertions(); 

    // to be used by monitor to notify that test case thrown exception
    void            caught_exception();

    // access method; to be used by unit_test_log
    c_string_literal     test_case_name();

    // used mostly by the Boost.Test unit testing
    void            failures_details( unit_test_counter& num_of_failures, bool& exception_caught );

private:
    // used to temporarely introduce new results set without polluting current one
    static void     reset_current_result_set();

    // Constructor
    unit_test_result( unit_test_result* parent_, c_string_literal test_case_name_, unit_test_counter expected_failures_ = 0 );
   
    struct Impl;
    boost::shared_ptr<Impl> m_pimpl;
};

// ************************************************************************** //
// **************            unit_test_result_saver            ************** //
// ************************************************************************** //

struct unit_test_result_saver
{
    unit_test_result_saver()  { unit_test_result::reset_current_result_set(); }
    ~unit_test_result_saver() { unit_test_result::reset_current_result_set(); }
};

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.11  2002/12/08 17:47:31  rogeeff
//  switched to use c_string_literal
//  unit_test_result_saver introduced to properly managed reset_current_test_set calls
//  in case of exceptions
//
//  Revision 1.10  2002/11/02 19:31:04  rogeeff
//  merged into the main trank
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_RESULT_HPP

