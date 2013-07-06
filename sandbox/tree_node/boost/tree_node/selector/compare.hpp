// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_SELECTOR_COMPARE_HPP
#define BOOST_TREE_NODE_SELECTOR_COMPARE_HPP

#include <functional>
#include <boost/tr1/type_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/function.hpp>
#include <boost/tree_node/selector/compare_fwd.hpp>
#include <boost/tree_node/intrinsic/value_at_key.hpp>
#include <boost/tree_node/iterator/dereference.hpp>
#include <boost/detail/metafunction/is_input_iterator.hpp>
#include <boost/detail/metafunction/has_super_t.hpp>

//[reference__compare_selector
namespace boost { namespace tree_node {

    template <typename Key>
    struct compare_selector
    {
        template <typename NodeIterator>
        struct apply
        {
            //<-
         private:
            typedef typename ::std::tr1::remove_const<
                        typename ::boost::mpl::eval_if<
                            ::std::tr1::is_pointer<NodeIterator>
                          , ::std::tr1::remove_pointer<NodeIterator>
                          , ::boost::mpl::eval_if<
                                ::boost::detail::metafunction
                                ::is_input_iterator<NodeIterator>
                              , ::boost::iterator_value<NodeIterator>
                              , ::boost::mpl::identity<NodeIterator>
                            >
                        >::type
                    >::type
                    MaybeNode;
            typedef typename ::boost::mpl::eval_if<
                        typename ::boost::detail::metafunction::has_super_t<
                            MaybeNode
                        >::type
                      , result_of::value_at_key<MaybeNode,Key>
                      , ::boost::mpl::identity<MaybeNode>
                    >::type
                    MaybeValue;

         public:
            //->
            class type
            {
                //<-
                ::boost::function2<bool,MaybeValue const&,MaybeValue const&>
                    _compare;
                //->

             public:
                typedef NodeIterator first_argument_type;
                typedef NodeIterator second_argument_type;
                typedef bool result_type;

                type();

                template <typename Function>
                type(Function compare);

                result_type
                    operator()(
                        first_argument_type const& lhs_node_itr
                      , second_argument_type const& rhs_node_itr
                    ) const;

                //<-
             private:
                result_type
                    _evaluate(
                        first_argument_type const& lhs_node_itr
                      , second_argument_type const& rhs_node_itr
                      , ::std::tr1::true_type
                    ) const;

                result_type
                    _evaluate(
                        first_argument_type const& lhs_node_itr
                      , second_argument_type const& rhs_node_itr
                      , ::std::tr1::false_type
                    ) const;
                //->
            };
        };
    };
}}  // namespace boost::tree_node
//]

namespace boost { namespace tree_node {

    template <typename Key>
    template <typename NodeIterator>
    compare_selector<Key>::apply<NodeIterator>::type::type()
      : _compare(::std::less<MaybeValue>())
    {
    }

    template <typename Key>
    template <typename NodeIterator>
    template <typename Function>
    compare_selector<Key>::apply<NodeIterator>::type::type(Function compare)
      : _compare(compare)
    {
    }

    template <typename Key>
    template <typename NodeIterator>
    inline typename compare_selector<Key>::BOOST_NESTED_TEMPLATE apply<
        NodeIterator
    >::type::result_type
        compare_selector<Key>::apply<NodeIterator>::type::operator()(
            first_argument_type const& lhs_node_itr
          , second_argument_type const& rhs_node_itr
        ) const
    {
        return this->_evaluate(
            lhs_node_itr
          , rhs_node_itr
          , ::std::tr1::is_pointer<NodeIterator>()
        );
    }

    template <typename Key>
    template <typename NodeIterator>
    inline typename compare_selector<Key>::BOOST_NESTED_TEMPLATE apply<
        NodeIterator
    >::type::result_type
        compare_selector<Key>::apply<NodeIterator>::type::_evaluate(
            first_argument_type const& lhs_node_itr
          , second_argument_type const& rhs_node_itr
          , ::std::tr1::true_type
        ) const
    {
        return this->_compare(
            get(*lhs_node_itr, Key())
          , get(*rhs_node_itr, Key())
        );
    }

    template <typename Key>
    template <typename NodeIterator>
    inline typename compare_selector<Key>::BOOST_NESTED_TEMPLATE apply<
        NodeIterator
    >::type::result_type
        compare_selector<Key>::apply<NodeIterator>::type::_evaluate(
            first_argument_type const& lhs_node_itr
          , second_argument_type const& rhs_node_itr
          , ::std::tr1::false_type
        ) const
    {
        return this->_compare(
            get(dereference_iterator(lhs_node_itr), Key())
          , get(dereference_iterator(rhs_node_itr), Key())
        );
    }
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_SELECTOR_COMPARE_HPP

