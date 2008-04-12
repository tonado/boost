/*=============================================================================
    Spirit v1.6.1
    Copyright (c) 2003 Martin Wille
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
 =============================================================================*/
#ifndef BOOST_SPIRIT_UTILITY_SCOPED_LOCK_HPP
#define BOOST_SPIRIT_UTILITY_SCOPED_LOCK_HPP

///////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_SPIRIT_COMPOSITE_HPP)
#include "boost/spirit/core/composite.hpp"
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    ///////////////////////////////////////////////////////////////////////////
    //
    // scoped_lock_parser class
    //
    //      implements locking of a mutex during execution of
    //      the parse method of an embedded parser
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename MutexT, typename ParserT>
    struct scoped_lock_parser
        : public unary< ParserT, parser< scoped_lock_parser<MutexT, ParserT> > >
    {
        typedef scoped_lock_parser<MutexT, ParserT> self_t;
        typedef MutexT      mutex_t;
        typedef ParserT     parser_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<parser_t, ScannerT>::type type;
        };

        scoped_lock_parser(mutex_t &m, parser_t const &p)
            : unary< ParserT, parser< scoped_lock_parser<MutexT, ParserT> > >(p)
            , mutex(m)
        {}


        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const &scan) const
        {
            typedef typename  mutex_t::scoped_lock scoped_lock_t;
            scoped_lock_t lock(mutex);
            return subject().parse(scan);
        }

        mutex_t &mutex;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    // scoped_lock_parser_gen
    //
    //      generator for scoped_lock_parser objects
    //      operator[] returns scoped_lock_parser according to its argument
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename MutexT>
    struct scoped_lock_parser_gen
    {
        typedef MutexT mutex_t;
        explicit scoped_lock_parser_gen(mutex_t &m) : mutex(m) {}

        template<typename ParserT>
        scoped_lock_parser
        <
            MutexT,
            typename as_parser<ParserT>::type
        >
        operator[](ParserT const &p) const
        {
            typedef ::boost::spirit::as_parser<ParserT> as_parser_t;
            typedef typename as_parser_t::type parser_t;

            return scoped_lock_parser<mutex_t, parser_t>
                (mutex, as_parser_t::convert(p));
        }

        mutex_t &mutex;
    };


    ///////////////////////////////////////////////////////////////////////////
    //
    // scoped_lock_d parser directive
    //
    //      constructs a scoped_lock_parser generator from its argument
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename MutexT>
    scoped_lock_parser_gen<MutexT>
    scoped_lock_d(MutexT &mutex)
    {
        return scoped_lock_parser_gen<MutexT>(mutex);
    }

}} // namespace boost::spirit
#endif // BOOST_SPIRIT_UTILITY_SCOPED_LOCK_HPP
