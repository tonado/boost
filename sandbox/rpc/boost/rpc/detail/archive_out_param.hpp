#ifndef BOOST_RPC_ARCHIVE_OUT_PARAM_HPP
#define BOOST_RPC_ARCHIVE_OUT_PARAM_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/future/future.hpp>

#define BOOST_RPC_VAR_arg_storableN_NAME(z,n,data) BOOST_PP_CAT(arg_storable,BOOST_PP_INC(n))
#define BOOST_RPC_VAR_arg_storableN_INIT(z,n,name) \
    BOOST_RPC_VAR_arg_storableN_NAME(z,n,BOOST_PP_EMPTY()) ( name(z,n,BOOST_PP_EMPTY()) )

#define BOOST_RPC_TYPE_ARGUMENT_ARG(z,n,arg) \
    typename boost::rpc::argument<BOOST_ARITY_ARG_TYPE_NAME(n)> arg(z,n,BOOST_PP_EMPTY())
#define BOOST_RPC_TYPE_ARGUMENT_CREF_ARG(z,n,arg) \
    const typename boost::rpc::argument<BOOST_ARITY_ARG_TYPE_NAME(n)> & arg(z,n,BOOST_PP_EMPTY())
#define BOOST_RPC_TYPE_ARGUMENT_VAR(z,n,var) \
    typename boost::rpc::argument<BOOST_ARITY_ARG_TYPE_NAME(n)> var(z,n,BOOST_PP_EMPTY());

#define BOOST_RPC_TYPE_PARGUMENT_ARG(z,n,arg) \
    typename boost::rpc::promising_argument<BOOST_ARITY_ARG_TYPE_NAME(n)> arg(z,n,BOOST_PP_EMPTY())
#define BOOST_RPC_TYPE_PARGUMENT_VAR(z,n,var) \
    typename boost::rpc::promising_argument<BOOST_ARITY_ARG_TYPE_NAME(n)> var(z,n,BOOST_PP_EMPTY());

#define BOOST_RPC_ARCHIVE_OUT_PARAM(z,I,archive) \
    boost::rpc::detail:: \
    archive_out_param<boost::function_traits<Signature>::arg##I##_type>(archive,arg_storable##I);

#define BOOST_RPC_PARAM_TYPE(I) \
    typename boost::rpc::argument<BOOST_ARITY_ARG_TYPE_NAME(I)>::type

#define BOOST_RPC_PARAM_TYPE_ARG(J,I,D) BOOST_RPC_PARAM_TYPE(I) BOOST_PP_CAT(a,BOOST_PP_INC(I))

#define BOOST_RPC_ARGUMENT(z,n,text) \
  boost::rpc:: \
  argument<typename boost::function_traits<Signature>::arg##n##_type> arg_storable##n;

#define BOOST_RPC_INIT_ARGUMENT(z,n,text) \
 BOOST_PP_CAT(arg_storable,n)(BOOST_PP_CAT(a,n))

#define BOOST_RPC_ARGUMENT_ASSIGN_PROMISE(z,I,text) \
    BOOST_PP_CAT(arg_storable,I).assign_promise();
namespace boost {
namespace rpc {

template<typename T, typename Enable=void, typename Enable2=void>
struct is_out_argument
    : public boost::is_void<int>
{};

template<typename T>
struct is_out_argument<T,
    typename boost::enable_if<boost::is_reference<T> >::type,
    typename boost::disable_if<boost::is_const<typename boost::remove_reference<T>::type> >::type>
    : public boost::is_void<void>
{};

/// Wrapper for possibly "out" arguments to a remote call
template<typename T, typename Enable=void>
class argument
{
public:
    typedef T type;
    argument(T){}
};

template<typename T>
class argument<T, typename boost::enable_if<is_out_argument<T> >::type>
{
public:
    typedef argument<T> type;
    typedef typename boost::detail::storable<T>::type storable_type;
    typedef typename boost::remove_reference<T>::type referred_type;

    argument(T ref)
    {
        passed = by_ref;
        referred_ptr = &ref;
    }
    argument(boost::future<referred_type> &future)
    {
        passed = by_future;
        future_ptr = &future;
    }
    operator T()
    {
        switch(passed)
        {
        case by_ref:
            return *referred_ptr;
        case by_future:
            storable = future_ptr->get();
            return storable;
        }
        return storable;
    }
protected:
    union
    {
        referred_type *referred_ptr;
        boost::future<referred_type> *future_ptr;
    };
    enum
    {
        by_ref,
        by_future,
    } passed;
    storable_type storable;
};

template<typename T, typename Enable=void>
class promising_argument : public argument<T>
{
public:
    promising_argument(const argument<T> &a) : argument<T>(a) {}
    void assign_promise(){}
};

template<typename T>
class promising_argument<T, typename boost::enable_if<is_out_argument<T> >::type>
    : public argument<T>
{
public:
    typedef typename boost::detail::storable<T>::type storable_type;
    typedef typename boost::remove_reference<T>::type referred_type;

    promising_argument(const argument<T> &a) : argument<T>(a), ref_future(promise)
    {
        if (passed == by_future)
            *future_ptr = boost::future<referred_type>(promise);
    }
    template<typename Archive>
    void deserialize(Archive &archive)
    {
        if (passed == by_future)
        {
            archive & storable;
            promise.set(storable);
        }
        else // if (passed == by_ref)
        {
            archive & storable;
            promise.set(storable);
        }
    }
    void assign_promise()
    {
        if (passed == by_ref)
            *referred_ptr = ref_future;
    }
private:
    boost::promise<referred_type> promise;
    boost::future<referred_type> ref_future;
};

namespace detail {

#define BOOST_RPC_REF_COPY(z,n,text) \
  boost::rpc::detail:: \
  reference_copy<typename boost::function_traits<Signature>::arg##n##_type> arg_storable##n;

#define BOOST_RPC_INIT_REF_COPY(z,n,text) \
 BOOST_PP_CAT(arg_storable,n)(BOOST_PP_CAT(a,n))

template<typename T, typename Archive>
typename boost::enable_if<typename boost::is_reference<T> >::type
archive_out_param(Archive &archive, T t)
{
    archive & t;
}

template<typename T, typename Archive>
typename boost::disable_if<boost::is_reference<T> >::type
archive_out_param(Archive &archive, T t)
{
}

template<typename T, typename Archive>
typename boost::enable_if<boost::rpc::is_out_argument<T> >::type
archive_out_param (Archive &archive, promising_argument<T> &t)
{
    t.deserialize(archive);
}

template<typename T, typename Archive>
typename boost::disable_if<boost::rpc::is_out_argument<T> >::type
archive_out_param (Archive &, promising_argument<T> &)
{
}

} // namespace detail
} // namespace rpc
} // namespace boost

#endif
