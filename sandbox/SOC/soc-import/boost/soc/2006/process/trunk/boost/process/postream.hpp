//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
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
//! \remark <b>Blocking remarks</b>: Functions that write data to this
//! stream can block if the associated file handle blocks during the write.
//! As this class is used to communicate with child processes through
//! anonymous pipes, the most typical blocking condition happens when the
//! child is not processing the data in the pipe's system buffer.  When
//! this happens, the buffer eventually fills up and the system blocks
//! until the reader consumes some data, leaving some new room.
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
