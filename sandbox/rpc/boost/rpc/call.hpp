// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RPC_CALL_HPP
#define BOOST_RPC_CALL_HPP

#include <boost/rpc/config.hpp>
#include <boost/detail/arity.hpp>
#include <boost/call_traits.hpp>
#include <boost/rpc/detail/archive_out_param.hpp>

namespace boost {
namespace rpc {

/// Base class for all of the call classes
class call_base
{
public:
    /// Returns true if the call has been completed.
    volatile bool has_completed() const {return completed;}
    /// Marks the call as completed.
    void complete() {completed = true;}
    /// Returns the serialized call parameters.
    virtual const std::string &parameters() const = 0;
    /// Processes the serialized call results.
    virtual void result_string(const std::string &str, const call_options &options) = 0;
protected:
    /// Initializes the call.
    call_base()
    {
        reset();
    }
    /// Resets the call.
    void reset()
    {
        completed = false;
    }
private:
    volatile bool completed;
};

template<typename ReturnType>
class returning_call : public call_base
{
public:
    typedef typename boost::detail::storable<ReturnType>::type storable_return_type;

    /// Returns the return value of the completed function call.
    /**
    */
    typename boost::call_traits<storable_return_type>::const_reference
        return_value() const {return return_val;}
protected:
    template<typename Archive>
    void extract_return_val(Archive &archive, const boost::rpc::call_options &options)
    {
        if (options.marshal_option >= call_options::return_only)
            archive & return_val;
    }
    typename boost::detail::storable<ReturnType>::type return_val;
};

template<>
class returning_call<void> : public call_base
{
public:
    typedef void storable_return_type;

    void return_value() const {}
protected:
    template<typename Archive>
    void extract_return_val(Archive &archive, const boost::rpc::call_options &options)
    {}
};

template<typename ReturnType>
class sync_returning_call : public returning_call<typename ReturnType> {};

template<typename Id, typename Signature, typename ArchivePair = binary_archive,
    typename Enable=void, typename Enable2=void>
class call
#ifdef DOXYGEN_DOCS_ONLY
#include <boost/rpc/detail/call_template.hpp>
#endif // DOXYGEN_DOCS_ONLY
;

#define BOOST_ARITY_SEPARATE_VOID_RETURN
#define BOOST_ARITY_ITERATION_PARAMS \
    (3,(0,BOOST_RPC_MAX_ARGS,<boost/rpc/detail/call_template.hpp>))
#include <boost/detail/arity_iterate.hpp>
#undef BOOST_ARITY_ITERATION_PARAMS
#undef BOOST_ARITY_SEPARATE_VOID_RETURN

} // namespace rpc
} // namespace boost

#endif // BOOST_RPC_CALL_HPP
