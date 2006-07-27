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
//! \file boost/process/postream.hpp
//!
//! Includes the declaration of the postream class.
//!

#if !defined(BOOST_PROCESS_POSTREAM_HPP)
/** \cond */
#define BOOST_PROCESS_POSTREAM_HPP
/** \endcond */

#include <ostream>

#include <boost/noncopyable.hpp>
#include <boost/process/detail/file_handle.hpp>
#include <boost/process/detail/systembuf.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Child process' input stream.
//!
//! The postream class represents an input communication channel with the
//! child process.  The child process reads data from this stream and the
//! parent process can write to it through the postream object.  In other
//! words, from the child's point of view, the communication channel is an
//! input one, but from the parent's point of view it is an output one;
//! hence the confusing postream name.
//!
//! postream objects cannot be copied because they own the file handle
//! they use to communicate with the child and because they buffer data
//! that flows through the communication channel.
//!
//! A postream object behaves as a std::ostream stream in all senses.
//! The class is only provided because it must provide a method to let
//! the caller explicitly close the communication channel.
//!
class postream :
    public std::ostream, boost::noncopyable
{
    //!
    //! \brief The file handle managed by this stream.
    //!
    detail::file_handle m_handle;

    //!
    //! \brief The systembuf object used to manage this stream's data.
    //!
    detail::systembuf m_systembuf;

public:
    //!
    //! \brief Creates a new process' input stream.
    //!
    //! Given a file handle, this constructor creates a new postream
    //! object that owns the given file handle \a fh.  Ownership of
    //! \a fh is transferred to the created postream object.
    //!
    //! \pre \a fh is valid.
    //! \post \a fh is invalid.
    //! \post The new postream object owns \a fh.
    //!
    explicit postream(detail::file_handle& fh);

    //!
    //! \brief Closes the file handle managed by this stream.
    //!
    //! Explicitly closes the file handle managed by this stream.  This
    //! function can be used by the user to tell the child process there
    //! is no more data to send.
    //!
    void close(void);
};

// ------------------------------------------------------------------------

inline
postream::postream(detail::file_handle& fh) :
    std::ostream(NULL),
    m_handle(fh),
    m_systembuf(m_handle.get())
{
    rdbuf(&m_systembuf);
}

// ------------------------------------------------------------------------

inline
void
postream::close(void)
{
    m_handle.close();
}

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_POSTREAM_HPP)
