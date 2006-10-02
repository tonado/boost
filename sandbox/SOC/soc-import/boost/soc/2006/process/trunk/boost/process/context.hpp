//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/context.hpp
//!
//! Includes the declaration of the context class and several accessory
//! base classes.
//!

#if !defined(BOOST_PROCESS_CONTEXT_HPP)
/** \cond */
#define BOOST_PROCESS_CONTEXT_HPP
/** \endcond */

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
#   include <errno.h>
#elif defined(BOOST_PROCESS_WIN32_API)
#   include <tchar.h>
#   include <windows.h>
#else
#   error "Unsupported platform."
#endif

#include <boost/assert.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/exceptions.hpp>
#include <boost/process/stream_behavior.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Base context class that defines the child's work directory.
//!
//! Base context class that defines the necessary fields to configure a
//! child's work directory.  This class is useless on its own because no
//! function in the library will accept it as a valid Context
//! implementation.
//!
template< class Path >
class basic_work_directory_context
{
public:
    //!
    //! \brief Constructs a new work directory context.
    //!
    //! Constructs a new work directory context making the work directory
    //! described by the new object point to the caller's current working
    //! directory.
    //!
    basic_work_directory_context(void);

    //!
    //! \brief The process' initial work directory.
    //!
    //! The work directory is the directory in which the process starts
    //! execution.
    //!
    Path m_work_directory;
};

// ------------------------------------------------------------------------

template< class Path >
inline
basic_work_directory_context< Path >::basic_work_directory_context(void)
{
#if defined(BOOST_PROCESS_POSIX_API)
    const char* buf = ::getcwd(NULL, 0);
    if (buf == NULL)
        boost::throw_exception
            (system_error
             ("boost::process::context::context",
              "getcwd(2) failed", errno));
    m_work_directory = buf;
#elif defined(BOOST_PROCESS_WIN32_API)
    DWORD length = ::GetCurrentDirectory(0, NULL);
    TCHAR* buf = new TCHAR[length * sizeof(TCHAR)];
    if (::GetCurrentDirectory(length, buf) == 0) {
        delete buf;
        boost::throw_exception
            (system_error
             ("boost::process::context::context",
              "GetCurrentDirectory failed", ::GetLastError()));
    }
    m_work_directory = buf;
    delete buf;
#endif
    BOOST_ASSERT(!m_work_directory.empty());
}

// ------------------------------------------------------------------------

//!
//! \brief Base context class that defines the child's environment.
//!
//! Base context class that defines the necessary fields to configure a
//! child's environment variables.  This class is useless on its own
//! because no function in the library will accept it as a valid Context
//! implementation.
//!
class environment_context
{
public:
    //!
    //! \brief The process' environment.
    //!
    //! Contains the list of environment variables, alongside with their
    //! values, that will be passed to the spawned child process.
    //!
    environment m_environment;
};

// ------------------------------------------------------------------------

//!
//! \brief Process startup execution context.
//!
//! The context class groups all the parameters needed to configure a
//! process' environment during its creation.
//!
template< class Path >
class basic_context :
    public basic_work_directory_context< Path >,
    public environment_context
{
public:
    //!
    //! \brief Child's stdin behavior.
    //!
    stream_behavior m_stdin_behavior;

    //!
    //! \brief Child's stdout behavior.
    //!
    stream_behavior m_stdout_behavior;

    //!
    //! \brief Child's stderr behavior.
    //!
    stream_behavior m_stderr_behavior;
};

typedef basic_context< std::string > context;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_CONTEXT_HPP)
