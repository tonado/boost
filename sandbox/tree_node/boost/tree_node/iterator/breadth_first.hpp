// Copyright (C) 2011-2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_ITERATOR_BREADTH_FIRST_HPP
#define BOOST_TREE_NODE_ITERATOR_BREADTH_FIRST_HPP

#include <boost/config.hpp>

#if defined BOOST_NO_SFINAE
#include <iterator>
#include <boost/range/algorithm/copy.hpp>
#else
#include <boost/utility/enable_if.hpp>
#include <boost/tr1/type_traits.hpp>
#endif

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/heap/heap_merge.hpp>
#include <boost/container_gen/selectors.hpp>
#include <boost/container_gen/container_gen.hpp>
#include <boost/container_gen/is_std_heap_selector.hpp>
#include <boost/container_gen/is_heap_selector.hpp>
#include <boost/container_gen/is_mergeable_heap_selector.hpp>
#include <boost/tree_node/traversal_state.hpp>
#include <boost/tree_node/iterator/dereference.hpp>
#include <boost/detail/metafunction/container_pointer.hpp>
#include <boost/detail/metafunction/container_iterator.hpp>

//[reference__breadth_first_iterator
namespace boost { namespace tree_node {

    template <typename Node, typename Selector = ::boost::dequeS>
    class breadth_first_iterator
      : public ::boost::iterator_adaptor<
            breadth_first_iterator<Node,Selector>
//<-
#if 0
//->
          , Node*
//<-
#endif
          , typename ::boost::detail::metafunction::container_pointer<
                Node
            >::type
//->
          , ::boost::use_default
          , ::boost::forward_traversal_tag
        >
    {
        //<-
        typedef ::boost::iterator_adaptor<
                    breadth_first_iterator<Node,Selector>
                  , typename ::boost::detail::metafunction::container_pointer<
                        Node
                    >::type
                  , ::boost::use_default
                  , ::boost::forward_traversal_tag
                >
                super_t;
        typedef typename ::boost::container_gen<
                    Selector
                  , typename ::boost::detail::metafunction::container_pointer<
                        Node
                    >::type
                >::type
                Queue;

#if !defined BOOST_NO_SFINAE
        //->
        struct enabler
        {
        };
        //<-
#endif

     public:  // Should be private, but conversion ctor won't work.
        Queue                               _queue;
        ::boost::tree_node::traversal_state _state;
        //->

     public:
        breadth_first_iterator();

        template <typename Compare>
        explicit breadth_first_iterator(Compare const& compare);

        explicit breadth_first_iterator(Node& node);

        template <typename Compare>
        breadth_first_iterator(Node& node, Compare const& compare);

//<-
#if defined BOOST_NO_SFINAE
        template <typename N, typename S>
        breadth_first_iterator(breadth_first_iterator<N,S> const& other);
#else
//->
        template <typename N, typename S>
        breadth_first_iterator(
            breadth_first_iterator<N,S> const& other
          , typename ::boost::enable_if<
                typename ::boost::mpl::eval_if<
                    typename ::boost::mpl::if_<
                        ::std::tr1::is_same<S,Selector>
                      , ::std::tr1::is_convertible<N,Node>
                      , ::boost::mpl::false_
                    >::type
                  , ::boost::mpl::eval_if<
                        ::boost::is_heap_selector<Selector>
                      , ::boost::mpl::false_
                      , ::boost::mpl::if_<
                            ::boost::is_std_heap_selector<Selector>
                          , ::boost::mpl::false_
                          , ::boost::mpl::true_
                        >
                    >
                  , ::boost::mpl::false_
                >::type
              , enabler
            >::type = enabler()
        );

        template <typename N, typename S>
        breadth_first_iterator(
            breadth_first_iterator<N,S> const& other
          , typename ::boost::enable_if<
                typename ::boost::mpl::eval_if<
                    typename ::boost::mpl::if_<
                        ::std::tr1::is_same<S,Selector>
                      , ::std::tr1::is_convertible<N,Node>
                      , ::boost::mpl::false_
                    >::type
                  , ::boost::mpl::if_<
                        ::boost::is_heap_selector<Selector>
                      , ::boost::mpl::true_
                      , ::boost::is_std_heap_selector<Selector>
                    >
                  , ::boost::mpl::false_
                >::type
              , enabler
            >::type = enabler()
        );
//<-
#endif  // BOOST_NO_SFINAE
//->

        operator ::boost::tree_node::traversal_state() const;

        //<-
#if !BOOST_WORKAROUND(__GNUC__, == 2)
     private:
        friend class ::boost::iterator_core_access;
#endif

        void increment();

        template <typename N, typename S>
        bool equal(breadth_first_iterator<N,S> const& other) const;

     private:
        void _increment(::boost::mpl::true_);

        void _increment(::boost::mpl::false_);

        static void
            _merge_heap(
                Queue& this_queue
              , Queue& other_queue
              , ::boost::mpl::true_
            );

        template <typename OtherQueue, typename BooleanConstant>
        static void
            _merge_heap(
                Queue& this_queue
              , OtherQueue& other_queue
              , BooleanConstant
            );

        template <typename OtherQueue>
        static void
            _merge(
                Queue& this_queue
              , OtherQueue& other_queue
              , ::boost::mpl::true_
            );

        template <typename OtherQueue>
        static void
            _merge(
                Queue& this_queue
              , OtherQueue& other_queue
              , ::boost::mpl::false_
            );

#if defined BOOST_NO_SFINAE
        template <typename CopyQueue>
        static void
            _clone(
                Queue& this_queue
              , CopyQueue const& copy_queue
              , ::boost::mpl::true_
            );

        template <typename CopyQueue>
        static void
            _clone(
                Queue& this_queue
              , CopyQueue const& copy_queue
              , ::boost::mpl::false_
            );
#endif
        //->
    };
}}  // namespace boost::tree_node
//]

namespace boost { namespace tree_node {

    template <typename Node, typename Selector>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator()
      : super_t(), _queue(), _state(::boost::tree_node::no_traversal)
    {
    }

    template <typename Node, typename Selector>
    template <typename Compare>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator(
        Compare const& compare
    ) : super_t(), _queue(compare), _state(::boost::tree_node::no_traversal)
    {
    }

    template <typename Node, typename Selector>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator(Node& node)
      : super_t(&node)
      , _queue()
      , _state(::boost::tree_node::breadth_first_traversal)
    {
    }

    template <typename Node, typename Selector>
    template <typename Compare>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator(
        Node& node
      , Compare const& compare
    ) : super_t(&node)
      , _queue(compare)
      , _state(::boost::tree_node::breadth_first_traversal)
    {
    }

    template <typename Node, typename Selector>
    inline void
        breadth_first_iterator<Node,Selector>::_merge_heap(
            Queue& this_queue
          , Queue& other_queue
          , ::boost::mpl::true_
        )
    {
        this_queue.merge(other_queue);
    }

    template <typename Node, typename Selector>
    template <typename OtherQueue, typename BooleanConstant>
    inline void
        breadth_first_iterator<Node,Selector>::_merge_heap(
            Queue& this_queue
          , OtherQueue& other_queue
          , BooleanConstant
        )
    {
        ::boost::heap::heap_merge(this_queue, other_queue);
    }

    template <typename Node, typename Selector>
    template <typename OtherQueue>
    inline void
        breadth_first_iterator<Node,Selector>::_merge(
            Queue& this_queue
          , OtherQueue& other_queue
          , ::boost::mpl::true_
        )
    {
        breadth_first_iterator<Node,Selector>::_merge_heap(
            this_queue
          , other_queue
          , ::boost::is_mergeable_heap_selector<Selector>()
        );
    }

    template <typename Node, typename Selector>
    template <typename OtherQueue>
    void
        breadth_first_iterator<Node,Selector>::_merge(
            Queue& this_queue
          , OtherQueue& other_queue
          , ::boost::mpl::false_
        )
    {
        for (; !other_queue.empty(); other_queue.pop())
        {
            this_queue.push(other_queue.top());
        }
    }

#if defined BOOST_NO_SFINAE
    template <typename Node, typename Selector>
    template <typename CopyQueue>
    inline void
        breadth_first_iterator<Node,Selector>::_clone(
            Queue& this_queue
          , CopyQueue const& copy_queue
          , ::boost::mpl::true_
        )
    {
        CopyQueue other_queue(copy_queue);

        breadth_first_iterator<Node,Selector>::_merge(
            this_queue
          , other_queue
          , ::boost::is_heap_selector<Selector>()
        );
    }

    template <typename Node, typename Selector>
    template <typename CopyQueue>
    inline void
        breadth_first_iterator<Node,Selector>::_clone(
            Queue& this_queue
          , CopyQueue const& copy_queue
          , ::boost::mpl::false_
        )
    {
        ::boost::range::copy(copy_queue, ::std::back_inserter(this_queue));
    }

    template <typename Node, typename Selector>
    template <typename N, typename S>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator(
        breadth_first_iterator<N,S> const& other
    ) : super_t(other.base()), _queue(), _state(other._state)
    {
        breadth_first_iterator<Node,Selector>::_clone(
            this->_queue
          , other._queue
          , typename ::boost::mpl::if_<
                ::boost::is_std_heap_selector<Selector>
              , ::boost::mpl::true_
              , ::boost::is_heap_selector<Selector>
            >::type()
        );
    }
#else  // !defined BOOST_NO_SFINAE
    template <typename Node, typename Selector>
    template <typename N, typename S>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator(
        breadth_first_iterator<N,S> const& other
      , typename ::boost::enable_if<
            typename ::boost::mpl::eval_if<
                typename ::boost::mpl::if_<
                    ::std::tr1::is_same<S,Selector>
                  , ::std::tr1::is_convertible<N,Node>
                  , ::boost::mpl::false_
                >::type
              , ::boost::mpl::eval_if<
                    ::boost::is_heap_selector<Selector>
                  , ::boost::mpl::false_
                  , ::boost::mpl::if_<
                        ::boost::is_std_heap_selector<Selector>
                      , ::boost::mpl::false_
                      , ::boost::mpl::true_
                    >
                >
              , ::boost::mpl::false_
            >::type
          , enabler
        >::type
    ) : super_t(other.base())
      , _queue(other._queue.begin(), other._queue.end())
      , _state(other._state)
    {
    }

    template <typename Node, typename Selector>
    template <typename N, typename S>
    breadth_first_iterator<Node,Selector>::breadth_first_iterator(
        breadth_first_iterator<N,S> const& other
      , typename ::boost::enable_if<
            typename ::boost::mpl::eval_if<
                typename ::boost::mpl::if_<
                    ::std::tr1::is_same<S,Selector>
                  , ::std::tr1::is_convertible<N,Node>
                  , ::boost::mpl::false_
                >::type
              , ::boost::mpl::if_<
                    ::boost::is_heap_selector<Selector>
                  , ::boost::mpl::true_
                  , ::boost::is_std_heap_selector<Selector>
                >
              , ::boost::mpl::false_
            >::type
          , enabler
        >::type
    ) : super_t(other.base()), _queue(), _state(other._state)
    {
        typename breadth_first_iterator<N,S>::Queue other_queue(other._queue);

        breadth_first_iterator<Node,Selector>::_merge(
            this->_queue
          , other_queue
          , ::boost::is_heap_selector<Selector>()
        );
    }
#endif  // BOOST_NO_SFINAE

    template <typename Node, typename Selector>
    inline breadth_first_iterator<Node,Selector>::operator
        ::boost::tree_node::traversal_state() const
    {
        return this->_state;
    }

    template <typename Node, typename Selector>
    void breadth_first_iterator<Node,Selector>::_increment(::boost::mpl::true_)
    {
        typedef typename ::boost::detail::metafunction::container_iterator<
                    Node
                >::type
                child_itr;

        child_itr itr_end = this->base()->end();

        for (child_itr itr = this->base()->begin(); itr != itr_end; ++itr)
        {
            this->_queue.push(&dereference_iterator(itr));
        }

        if (this->_queue.empty())
        {
            this->_state = ::boost::tree_node::no_traversal;
        }
        else
        {
            this->base_reference() = this->_queue.top();
            this->_queue.pop();
        }
    }

    template <typename Node, typename Selector>
    void
        breadth_first_iterator<Node,Selector>::_increment(::boost::mpl::false_)
    {
        typedef typename ::boost::detail::metafunction::container_iterator<
                    Node
                >::type
                child_itr;

        child_itr itr_end = this->base()->end();

        for (child_itr itr = this->base()->begin(); itr != itr_end; ++itr)
        {
            this->_queue.push_back(&dereference_iterator(itr));
        }

        if (this->_queue.empty())
        {
            this->_state = ::boost::tree_node::no_traversal;
        }
        else
        {
            this->base_reference() = this->_queue.front();
            this->_queue.pop_front();
        }
    }

    template <typename Node, typename Selector>
    inline void breadth_first_iterator<Node,Selector>::increment()
    {
        this->_increment(
            typename ::boost::mpl::if_<
                ::boost::is_std_heap_selector<Selector>
              , ::boost::mpl::true_
              , ::boost::is_heap_selector<Selector>
            >::type()
        );
    }

    template <typename Node, typename Selector>
    template <typename N, typename S>
    inline bool
        breadth_first_iterator<Node,Selector>::equal(
            breadth_first_iterator<N,S> const& other
        ) const
    {
        if (this->_state == other._state)
        {
            return this->_state ? (
                this->base() == other.base()
            ) : !other._state;
        }
        else
        {
            return false;
        }
    }
}}  // namespace boost::tree_node

//[reference__make_breadth_first_iterator
namespace boost { namespace tree_node {

    template <typename Node>
    breadth_first_iterator<Node,::boost::dequeS>
        make_breadth_first_iterator(Node& node);
}}  // namespace boost::tree_node
//]

//[reference__make_breadth_first_iterator__with_selector
namespace boost { namespace tree_node {

    template <typename Selector, typename Node>
    breadth_first_iterator<Node,Selector>
        make_breadth_first_iterator(Node& node);
}}  // namespace boost::tree_node
//]

namespace boost { namespace tree_node {

    template <typename Node>
    inline breadth_first_iterator<Node,::boost::dequeS>
        make_breadth_first_iterator(Node& node)
    {
        return breadth_first_iterator<Node,::boost::dequeS>(node);
    }

    template <typename Selector, typename Node>
    inline breadth_first_iterator<Node,Selector>
        make_breadth_first_iterator(Node& node)
    {
        return breadth_first_iterator<Node,Selector>(node);
    }
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_ITERATOR_BREADTH_FIRST_HPP

