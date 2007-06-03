// signal_mutex.hpp

// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_MUTEX_HPP
#define SIGNAL_NETWORK_MUTEX_HPP

#include <boost/signal_network/filter.hpp>
#include <boost/signal_network/detail/defines.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/signal_network/detail/unfused_inherited.hpp>

SIGNAL_NETWORK_OPEN_SIGNET_NAMESPACE

template<typename Signature>
class mutex : public fused_filter<Signature>
{
public:
    typedef boost::fusion::unfused_inherited<mutex<Signature>,
        typename mpl::vector<>::type,
        typename boost::function_types::parameter_types<Signature> > unfused;

    template <class Seq>
    struct result
    {
        typedef typename boost::function_traits<Signature>::result_type type;
    };
    typename boost::function_traits<Signature>::result_type 
    operator()(const typename fused_filter<Signature>::parameter_vector &vec_par)
    {
		boost::mutex::scoped_lock lock(mutex_);
        return static_cast<typename boost::function_traits<Signature>::result_type>
                 (fused_out(vec_par));
    }
	private:
		boost::mutex mutex_;
};

SIGNAL_NETWORK_CLOSE_SIGNET_NAMESPACE

#endif // SIGNAL_NETWORK_MUTEX_HPP
