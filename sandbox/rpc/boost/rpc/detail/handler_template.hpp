// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DOXYGEN_DOCS_ONLY
template<typename Signature, typename ArchivePair>
class handler<Signature, ArchivePair
    , typename enable_if_c<boost::function_traits<Signature>::arity==BOOST_ARITY_NUM_ARGS>::type,
   BOOST_ARITY_ENABLE_DISABLE_VOID
   >
   : public async_returning_handler<typename boost::function_traits<Signature>::result_type>
#endif // DOXYGEN_DOCS_ONLY
{
#ifdef DOXYGEN_DOCS_ONLY
public:
    /// Prepares the call with provided id and parameters.
    call(typename boost::call_traits<Id>::param_type id, Args..);
    /// Prepares the call with provided id and parameters.
    void set(typename boost::call_traits<Id>::param_type id, Agrs...);
    void reset();
protected:
    virtual const std::string &parameters() const;
    virtual void result_string(const std::string &str, const call_options &options);
};
#else

    BOOST_ARITY_STORABLE_TYPES(Signature)
    BOOST_ARITY_TYPES(Signature)

    BOOST_ARITY_REPEAT(BOOST_RPC_TYPE_PARGUMENT_VAR,BOOST_RPC_VAR_arg_storableN_NAME)
public:
    handler(BOOST_ARITY_ENUM(BOOST_RPC_TYPE_ARGUMENT_CREF_ARG,BOOST_RPC_VAR_arg_storableN_NAME))
        BOOST_ARITY_COLON
        BOOST_ARITY_ENUM(BOOST_RPC_VAR_arg_storableN_INIT,BOOST_RPC_VAR_arg_storableN_NAME)
    {}
    void assign_promises()
    {
        BOOST_PP_REPEAT_FROM_TO(1,BOOST_ARITY_NUM_ARGS_INC,BOOST_RPC_ARGUMENT_ASSIGN_PROMISE,BOOST_PP_EMPTY)
    }
    virtual void result_string(const std::string &str, const call_options &options)
    {
        std::stringstream stream(str, std::ios::in | std::ios::out | std::ios::binary);
        typename ArchivePair::iarchive_type archive(stream);
        extract_return_val(archive, options);
#ifdef BOOST_ARITY_NON_VOID_RETURN_TYPE
        return_prom.set(return_val);
#else
        return_prom.set();
#endif
        if (options.marshal_option >= boost::rpc::call_options::all_out_parameters)
        {
            BOOST_PP_REPEAT_FROM_TO(1, BOOST_ARITY_NUM_ARGS_INC, BOOST_RPC_ARCHIVE_OUT_PARAM, archive)
        }
    };
#endif
};