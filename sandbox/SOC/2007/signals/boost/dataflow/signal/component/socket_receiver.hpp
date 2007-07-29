// socket_receiver.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SOCKET_RECEIVER_HPP
#define SIGNAL_NETWORK_SOCKET_RECEIVER_HPP

#include <boost/dataflow/signal/component/storage.hpp>
#include <boost/dataflow/signal/component/detail/serialize_fusion_vector.hpp>

#include <sstream>
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace boost { namespace signals {

/** \brief Receives serializable signals through a Boost.Asio socket.
\param Signature Signature of the signal sent (and received through the socket).
\todo socket_sender only works for Signatures of return type void.

*/
template<typename Signature,
    typename OutSignal=SIGNAL_NETWORK_DEFAULT_OUT,
    typename Combiner = boost::last_value<typename boost::function_types::result_type<Signature>::type>,
    typename Group = int,
    typename GroupCompare = std::less<Group> >
class socket_receiver : public storage<Signature, OutSignal, Combiner, Group, GroupCompare>
{
    typedef storage<Signature, OutSignal, Combiner, Group, GroupCompare> base_type;

public:
    /// Initializes the socket_sender to use the provided socket.
    	socket_receiver(asio::ip::tcp::socket &socket) :
		socket_(socket),
		stream(std::ios::in | std::ios::out | std::ios::binary) {}
    /// Sets the receiver to wait for a packet and send its contents via the signal.
	void operator()()
	{
		int received_size = socket_.receive(asio::buffer(buffer, bufferSize));
		stream.str(empty_string);
		stream.write((char *)buffer, received_size);
		boost::archive::binary_iarchive archive(stream);
        archive & base_type::stored();
		base_type::send();
	}

private:
	asio::ip::tcp::socket &socket_;
	std::stringstream stream;
	std::string empty_string;
	boost::archive::binary_iarchive *archive;
	enum { bufferSize = 20000 };
	unsigned char buffer[bufferSize];
};

} } // namespace boost::signals

#endif