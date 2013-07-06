// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UTILITY_EQUIVALENCE_FUNCTION_HPP_INCLUDED
#define BOOST_UTILITY_EQUIVALENCE_FUNCTION_HPP_INCLUDED

//[reference__equivalence_function_gen
namespace boost {

    template <typename StrictWeakOrdering>
    struct equivalence_function_gen;
}  // namespace boost
//]

//[reference__make_equivalence_function
namespace boost {

    template <typename StrictWeakOrdering>
    typename equivalence_function_gen<StrictWeakOrdering>::type
        make_equivalence_function(StrictWeakOrdering const& predicate);
}  // namespace boost
//]

namespace boost { namespace detail {

    template <typename StrictWeakOrdering>
    class adaptable_equivalence_function
    {
        StrictWeakOrdering _predicate;

     public:
        typedef bool
                result_type;
        typedef typename StrictWeakOrdering::first_argument_type
                first_argument_type;
        typedef typename StrictWeakOrdering::second_argument_type
                second_argument_type;

        adaptable_equivalence_function();

        template <typename Predicate>
        adaptable_equivalence_function(Predicate const& predicate);

        result_type
            operator()(
                first_argument_type const& arg1
              , second_argument_type const& arg2
            ) const;
    };

    template <typename StrictWeakOrdering>
    adaptable_equivalence_function<
        StrictWeakOrdering
    >::adaptable_equivalence_function() : _predicate()
    {
    }

    template <typename StrictWeakOrdering>
    template <typename Predicate>
    adaptable_equivalence_function<
        StrictWeakOrdering
    >::adaptable_equivalence_function(Predicate const& predicate)
      : _predicate(predicate)
    {
    }

    template <typename StrictWeakOrdering>
    inline typename adaptable_equivalence_function<
        StrictWeakOrdering
    >::result_type
        adaptable_equivalence_function<StrictWeakOrdering>::operator()(
            first_argument_type const& arg1
          , second_argument_type const& arg2
        ) const
    {
        return !this->_predicate(arg1, arg2) && !this->_predicate(arg2, arg1);
    }

    template <typename StrictWeakOrdering>
    class plain_equivalence_function
    {
        StrictWeakOrdering _predicate;

     public:
        typedef bool result_type;

        plain_equivalence_function();

        template <typename Predicate>
        plain_equivalence_function(Predicate const& predicate);

        template <typename T1, typename T2>
        result_type operator()(T1 const& t1, T2 const& t2) const;
    };

    template <typename StrictWeakOrdering>
    plain_equivalence_function<
        StrictWeakOrdering
    >::plain_equivalence_function() : _predicate()
    {
    }

    template <typename StrictWeakOrdering>
    template <typename Predicate>
    plain_equivalence_function<StrictWeakOrdering>::plain_equivalence_function(
        Predicate const& predicate
    ) : _predicate(predicate)
    {
    }

    template <typename StrictWeakOrdering>
    template <typename T1, typename T2>
    inline typename plain_equivalence_function<StrictWeakOrdering>::result_type
        plain_equivalence_function<StrictWeakOrdering>::operator()(
            T1 const& t1
          , T2 const& t2
        ) const
    {
        return !this->_predicate(t1, t2) && !this->_predicate(t2, t1);
    }
}}  // namespace boost::detail

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/detail/metafunction/is_adaptable_binary_func.hpp>
#include <boost/detail/metafunction/is_function_wrt.hpp>

namespace boost {

    template <typename StrictWeakOrdering>
    struct equivalence_function_gen
      : ::boost::mpl::if_<
            typename ::boost::mpl::if_<
                ::boost::detail::metafunction::is_function_with_result_type<
                    StrictWeakOrdering
                  , bool
                >
              , ::boost::detail::metafunction::is_adaptable_binary_function<
                    StrictWeakOrdering
                >
              , ::boost::mpl::false_
            >::type
          , detail::adaptable_equivalence_function<StrictWeakOrdering>
          , detail::plain_equivalence_function<StrictWeakOrdering>
        >
    {
        BOOST_MPL_AUX_LAMBDA_SUPPORT(
            1
          , equivalence_function_gen
          , (StrictWeakOrdering)
        )
    };

    template <typename StrictWeakOrdering>
    inline typename equivalence_function_gen<StrictWeakOrdering>::type
        make_equivalence_function(StrictWeakOrdering const& predicate)
    {
        return typename equivalence_function_gen<StrictWeakOrdering>::type(
            predicate
        );
    }
}  // namespace boost

#endif  // BOOST_UTILITY_EQUIVALENCE_FUNCTION_HPP_INCLUDED

