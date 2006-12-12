// (C) Copyright 2005 Matthias Troyer 

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef BOOST_PARALLEL_MPI_TEXT_SKELETON_OARCHIVE_HPP
#define BOOST_PARALLEL_MPI_TEXT_SKELETON_OARCHIVE_HPP

#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/parallel/mpi/detail/forward_skeleton_oarchive.hpp>
#include <boost/parallel/mpi/detail/ignore_oprimitive.hpp>
#include <boost/archive/array/oarchive.hpp>

namespace boost { namespace parallel { namespace mpi {

// an archive that writes a text skeleton into a stream

class text_skeleton_oarchive 
  : public detail::ignore_oprimitive,
    public detail::forward_skeleton_oarchive<text_skeleton_oarchive,boost::archive::text_oarchive>
{
public:
    text_skeleton_oarchive(std::ostream & s, unsigned int flags = 0) 
	 : detail::forward_skeleton_oarchive<text_skeleton_oarchive,boost::archive::text_oarchive>(skeleton_archive_)
     , skeleton_archive_(s,flags)
	{}

private:
    boost::archive::text_oarchive skeleton_archive_;
};

} } } // end namespace boost::parallel::mpi


#endif // BOOST_PARALLEL_MPI_TEXT_SKELETON_OARCHIVE_HPP
