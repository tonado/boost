// (C) Copyright 2005 Matthias Troyer

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/impl/archive_pointer_oserializer.ipp>
#include <boost/mpi/skeleton_and_content.hpp>

namespace boost { namespace archive { namespace detail {
// explicitly instantiate all required template functions

template class archive_pointer_oserializer<mpi::detail::content_oarchive> ;
template class archive_pointer_oserializer<boost::mpi::detail::ignore_skeleton_oarchive<boost::mpi::detail::content_oarchive> >;
template class archive_pointer_oserializer<boost::mpi::detail::ignore_skeleton_oarchive<boost::mpi::detail::mpi_datatype_oarchive> >;
} } }
