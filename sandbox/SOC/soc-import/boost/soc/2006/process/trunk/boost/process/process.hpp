//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/process.hpp
//!
//! Includes the declaration of the process class.
//!

#if !defined(BOOST_PROCESS_PROCESS_HPP)
/** \cond */
#define BOOST_PROCESS_PROCESS_HPP
/** \endcond */

#include <boost/process/basic_process.hpp>
#include <boost/process/command_line.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Generic instantiation of the basic_process template.
//!
//! Generic instantiation of the basic_process template.  This relies on
//! the command_line implementation of the Command_Line concept provided
//! by the library.
//!
typedef basic_process< command_line > process;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_PROCESS_HPP)
