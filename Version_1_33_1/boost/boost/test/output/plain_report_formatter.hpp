//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : plain report formatter implementation
// ***************************************************************************

#ifndef BOOST_TEST_PLAIN_REPORT_FORMATTER_HPP_020105GER
#define BOOST_TEST_PLAIN_REPORT_FORMATTER_HPP_020105GER

// Boost.Test
#include <boost/test/detail/global_typedef.hpp>
#include <boost/test/results_reporter.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

namespace output {

// ************************************************************************** //
// **************             plain_report_formatter           ************** //
// ************************************************************************** //

class plain_report_formatter : public results_reporter::format {
public:
    // Formatter interface
    void    results_report_start( std::ostream& ostr );
    void    results_report_finish( std::ostream& ostr );

    void    test_unit_report_start( test_unit const&, std::ostream& ostr );
    void    test_unit_report_finish( test_unit const&, std::ostream& ostr );

    void    do_confirmation_report( test_unit const&, std::ostream& ostr );

private:
    // Data members
    counter_t m_indent;
};

} // namespace output

} // namespace unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2005/02/20 08:27:08  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

#endif // BOOST_TEST_PLAIN_REPORT_FORMATTER_HPP_020105GER
