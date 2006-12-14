// Copyright (C) 2005-2006 Douglas Gregor <doug.gregor -at- gmail.com>.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Message Passing Interface 1.1 -- Section 4.7. Gather-to-all
#ifndef BOOST_PARALLEL_MPI_ALL_GATHER_HPP
#define BOOST_PARALLEL_MPI_ALL_GATHER_HPP

#include <boost/parallel/mpi/exception.hpp>
#include <boost/parallel/mpi/datatype.hpp>
#include <vector>
#include <boost/serialization/vector.hpp>

// all_gather falls back to gather+broadcast in some cases
#include <boost/parallel/mpi/collectives/broadcast.hpp>
#include <boost/parallel/mpi/collectives/gather.hpp>

namespace boost { namespace parallel { namespace mpi {

namespace detail {
  // We're all-gathering for a type that has an associated MPI
  // datatype, so we'll use MPI_Gather to do all of the work.
  template<typename T>
  void
  all_gather_impl(const communicator& comm, const T* in_values, int n, 
                  T* out_values, mpl::true_)
  {
    MPI_Datatype type = boost::parallel::mpi::get_mpi_datatype<T>();
    BOOST_MPI_CHECK_RESULT(MPI_Allgather,
                           (const_cast<T*>(in_values), n, type,
                            out_values, n, type, comm));
  }

  // We're all-gathering for a type that has no associated MPI
  // type. So, we'll do a manual gather followed by a broadcast.
  template<typename T>
  void
  all_gather_impl(const communicator& comm, const T* in_values, int n, 
                  T* out_values, mpl::false_)
  {
    std::cerr << comm.rank() << ": gathering " << n << " values to root 0..." << std::endl;
    std::cerr << in_values << ", " << out_values << std::endl;
    gather(comm, in_values, n, out_values, 0);
    std::cerr << comm.rank() << ":  broadcasting from root 0..." << std::endl;
    broadcast(comm, out_values, comm.size() * n, 0);
  }
} // end namespace detail

template<typename T>
inline void
all_gather(const communicator& comm, const T& in_value, T* out_values)
{
  detail::all_gather_impl(comm, &in_value, 1, out_values, is_mpi_datatype<T>());
}

template<typename T>
void
all_gather(const communicator& comm, const T& in_value, 
           std::vector<T>& out_values)
{
  out_values.resize(comm.size());
  ::boost::parallel::mpi::all_gather(comm, &in_value, 1, &out_values[0]);
}

template<typename T>
inline void
all_gather(const communicator& comm, const T* in_values, int n, T* out_values)
{
  detail::all_gather_impl(comm, in_values, n, out_values, is_mpi_datatype<T>());
}

template<typename T>
void
all_gather(const communicator& comm, const T* in_values, int n,
           std::vector<T>& out_values)
{
  out_values.resize(comm.size() * n);
  ::boost::parallel::mpi::all_gather(comm, in_values, n, &out_values[0]);
}

} } } // end namespace boost::parallel::mpi

#endif // BOOST_PARALLEL_MPI_ALL_GATHER_HPP
