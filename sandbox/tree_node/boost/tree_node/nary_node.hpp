// Copyright (C) 2011-2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_NARY_NODE_HPP_INCLUDED
#define BOOST_TREE_NODE_NARY_NODE_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined BOOST_NO_CXX11_NULLPTR
#include <cstddef>
#endif

#include <utility>
#include <deque>
#include <algorithm>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/noncopyable.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range.hpp>
#include <boost/container_gen/container_gen.hpp>
#include <boost/container_gen/emplace_function_gen.hpp>
#include <boost/container_gen/splice_function_gen.hpp>
#include <boost/container_gen/insert_range_result_gen.hpp>
#include <boost/container_gen/is_recursive_selector.hpp>
#include <boost/container_gen/is_reversible_selector.hpp>
#include <boost/container_gen/is_random_access_selector.hpp>
#include <boost/container_gen/is_ptr_selector.hpp>
#include <boost/container_gen/has_stable_iters_selector.hpp>
#include <boost/tree_node/preprocessor.hpp>
#include <boost/tree_node/base.hpp>
#include <boost/tree_node/nary_node_fwd.hpp>
#include <boost/tree_node/intrinsic/has_key.hpp>
#include <boost/tree_node/intrinsic/get_keys.hpp>
#include <boost/tree_node/iterator/depth_first_descendant.hpp>
#include <boost/tree_node/iterator/post_order.hpp>
#include <boost/detail/metafunction/container_reverse_iterator.hpp>
#include <boost/assert.hpp>

#include <boost/tree_node/_detail/config_begin.hpp>

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/container_gen/insert_range_function_gen.hpp>
#else
#include <boost/type_traits/is_pod.hpp>
#include <boost/move/move.hpp>
#include <boost/container_gen/selectors.hpp>

namespace boost {

    template <typename Derived, typename T, typename Selector>
    struct is_POD<
        ::boost::tree_node::nary_node_base<Derived,T,Selector>
    > : ::boost::false_type
    {
    };

    template <typename Derived, typename T, typename Selector>
    struct is_pod<
        ::boost::tree_node::nary_node_base<Derived,T,Selector>
    > : ::boost::false_type
    {
    };
}  // namespace boost
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/iterator/key_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/category_of.hpp>

namespace boost { namespace tree_node { namespace result_of {

    template <
        typename Derived
      , typename T
      , typename Selector
      , typename FusionKey
    >
    struct has_key_impl<nary_node_base<Derived,T,Selector>,FusionKey>
      : ::boost::mpl::eval_if<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , ::boost::fusion::result_of::has_key<T,FusionKey>
          , ::std::tr1::is_same<FusionKey,data_key>
        >::type
    {
    };

    template <typename Derived, typename T, typename Selector>
    struct get_keys_impl<nary_node_base<Derived,T,Selector> >
      : ::boost::mpl::eval_if<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , ::boost::mpl::transform_view<
                T
              , ::boost::fusion::result_of::key_of< ::boost::mpl::_>
            >
          , ::boost::mpl::vector1<data_key>
        >
    {
    };
}}}  // namespace boost::tree_node::result_of
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

namespace boost { namespace tree_node {

    template <typename Derived, typename T, typename Selector>
    class nary_node_base
      : public
        //[reference__nary_node_base__bases
        tree_node_base<Derived>
        //]
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
      , public data_key
#endif
      , private ::boost::noncopyable
    {
        BOOST_MPL_ASSERT((::boost::is_recursive_selector<Selector>));

        typedef nary_node_base<Derived,T,Selector> self;

        //[reference__nary_node_base__children
        typedef typename ::boost::container_gen<Selector,Derived>::type
                children;
        //]

     public:
        //[reference__nary_node_base__super_t
        typedef tree_node_base<Derived> super_t;
        //]

        //[reference__nary_node_base__traits
        struct traits
        {
            typedef T data_type;
            typedef typename children::allocator_type allocator;
            typedef allocator const& allocator_reference;
        };
        //]

        //[reference__nary_node_base__pointer
        typedef typename super_t::pointer pointer;
        //]

        //[reference__nary_node_base__const_pointer
        typedef typename super_t::const_pointer const_pointer;
        //]

        //[reference__nary_node_base__iterator
        typedef typename children::iterator iterator;
        //]

        //[reference__nary_node_base__const_iterator
        typedef typename children::const_iterator const_iterator;
        //]

        typedef typename ::boost::detail::metafunction
                ::container_reverse_iterator<children>::type
                reverse_iterator;
        typedef typename ::boost::detail::metafunction
                ::container_reverse_iterator<children const>::type
                const_reverse_iterator;

        //[reference__nary_node_base__size_type
        typedef typename children::size_type size_type;
        //]

     private:
        children                   _children;
        typename traits::data_type _data;
        pointer                    _parent;

     protected:
        //[reference__nary_node_base__derived_copy_ctor
        nary_node_base(Derived const& copy);
        //]

        //[reference__nary_node_base__derived_copy_ctor_w_allocator
        nary_node_base(
            Derived const& copy
          , typename traits::allocator_reference allocator
        );
        //]

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        BOOST_PP_FOR(
            ((0), nary_node_base)
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED_WITH_DATA
          , BOOST_CONTAINER_GEN_PP_PARAM_INC_WITH_DATA
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_FWD_DECL
        )

        BOOST_PP_FOR(
            ((0), nary_node_base)
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED_WITH_DATA
          , BOOST_CONTAINER_GEN_PP_PARAM_INC_WITH_DATA
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_W_ALLOC_FWD_DECL
        )
#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        nary_node_base(BOOST_RV_REF(Derived) source);

        nary_node_base(
            BOOST_RV_REF(Derived) source
          , typename traits::allocator_reference allocator
        );

#if defined BOOST_CONTAINER_PERFECT_FORWARDING
        //[reference__nary_node_base__emplacement_ctor
        template <typename ...Args>
        explicit nary_node_base(Args&& ...args);
        //]

        //[reference__nary_node_base__emplacement_ctor_w_allocator
        template <typename ...Args>
        explicit nary_node_base(
            ::boost::container::allocator_arg_t
          , typename traits::allocator_reference allocator
          , Args&& ...args
        );
        //]
#else  // !defined BOOST_CONTAINER_PERFECT_FORWARDING
        BOOST_PP_REPEAT(
            BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_FWD_DECL
          , nary_node_base
        )

        BOOST_PP_REPEAT(
            BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_W_ALLOC_FWD_DECL
          , nary_node_base
        )
#endif  // BOOST_CONTAINER_PERFECT_FORWARDING
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        ~nary_node_base();

        //[reference__nary_node_base__clone_descendants
        void clone_descendants(Derived const& copy);
        //]

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        //[reference__nary_node_base__copy_assign
        void copy_assign(Derived const& copy);
        //]
#else
        void move_descendants(BOOST_RV_REF(Derived) source);

        void copy_assign(BOOST_COPY_ASSIGN_REF(Derived) copy);

        void move_assign(BOOST_RV_REF(Derived) source);
#endif

        //[reference__nary_node_base__on_post_assign
        void on_post_assign();
        //]

     public:
        //[reference__nary_node_base__data_key_value_operator__const
        typename traits::data_type const& operator[](data_key const&) const;
        //]

        //[reference__nary_node_base__data_key_value_operator
        typename traits::data_type& operator[](data_key const&);
        //]

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        //[reference__nary_node_base__key_value_operator__const
        template <typename FusionKey>
        typename ::boost::lazy_enable_if<
            typename ::boost::mpl::if_<
                typename ::boost::mpl::eval_if<
                    typename ::boost::fusion::traits::is_sequence<T>::type
                  , ::boost::fusion::traits::is_associative<T>
                  , ::boost::mpl::false_
                >::type
              , result_of::has_key<
                    nary_node_base<Derived,T,Selector>
                  , FusionKey
                >
              , ::boost::mpl::false_
            >::type
          , ::boost::fusion::result_of::at_key<T const,FusionKey>
        >::type
            operator[](FusionKey const&) const;
        //]

        //[reference__nary_node_base__key_value_operator
        template <typename FusionKey>
        typename ::boost::lazy_enable_if<
            typename ::boost::mpl::if_<
                typename ::boost::mpl::eval_if<
                    typename ::boost::fusion::traits::is_sequence<T>::type
                  , ::boost::fusion::traits::is_associative<T>
                  , ::boost::mpl::false_
                >::type
              , result_of::has_key<
                    nary_node_base<Derived,T,Selector>
                  , FusionKey
                >
              , ::boost::mpl::false_
            >::type
          , ::boost::fusion::result_of::at_key<T,FusionKey>
        >::type
            operator[](FusionKey const&);
        //]
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

        //[reference__nary_node_base__get_parent_ptr__const
        const_pointer get_parent_ptr() const;
        //]

        //[reference__nary_node_base__get_parent_ptr
        pointer get_parent_ptr();
        //]

        //[reference__nary_node_base__cbegin
        const_iterator cbegin() const;
        const_iterator begin() const;
        //]

        //[reference__nary_node_base__begin
        iterator begin();
        //]

        //[reference__nary_node_base__cend
        const_iterator cend() const;
        const_iterator end() const;
        //]

        //[reference__nary_node_base__end
        iterator end();
        //]

        //[reference__nary_node_base__crbegin
        const_reverse_iterator crbegin() const;
        const_reverse_iterator rbegin() const;
        //]

        //[reference__nary_node_base__rbegin
        reverse_iterator rbegin();
        //]

        //[reference__nary_node_base__crend
        const_reverse_iterator crend() const;
        const_reverse_iterator rend() const;
        //]

        //[reference__nary_node_base__rend
        reverse_iterator rend();
        //]

        //[reference__nary_node_base__size
        size_type size() const;
        //]

        //[reference__nary_node_base__empty
        bool empty() const;
        //]

        //[reference__nary_node_base__clear
        void clear();
        //]

        //[reference__nary_node_base__insert
        iterator insert(Derived const& child);
        //]

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        iterator                                                             \
            emplace(                                                         \
                BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)   \
            );                                                               \
//!
        BOOST_PP_FOR(
            (0)
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED
          , BOOST_CONTAINER_GEN_PP_PARAM_INC
          , BOOST_TREE_NODE_EMPLACEMENT_IMPL
        )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        iterator insert(BOOST_RV_REF(Derived) child);

#if defined BOOST_CONTAINER_PERFECT_FORWARDING
        //[reference__nary_node_base__emplace
        template <typename ...Args>
        iterator emplace(Args&& ...args);
        //]
#else  // !defined BOOST_CONTAINER_PERFECT_FORWARDING
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        iterator                                                             \
            emplace(                                                         \
                BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                             \
                    n                                                        \
                  , BOOST_CONTAINER_PP_PARAM_LIST                            \
                  , _                                                        \
                )                                                            \
            );                                                               \
//!
        BOOST_PP_REPEAT(
            BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
          , BOOST_TREE_NODE_EMPLACEMENT_IMPL
          , _
        )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#endif  // BOOST_CONTAINER_PERFECT_FORWARDING
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        //[reference__nary_node_base__splice
        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            splice(iterator pos, Derived& node);
        //]

        //[reference__nary_node_base__splice_one
        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            splice(iterator pos, Derived& node, iterator itr);
        //]

        //[reference__nary_node_base__splice_range
        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            splice(
                iterator pos
              , Derived& node
              , iterator itr
              , iterator itr_end
            );
        //]

     private:
        static void
            _link_children_to_parent(
                pointer const& p
              , iterator itr
              , iterator itr_end
            );

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        iterator                                                             \
            _add_child(                                                      \
                BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)   \
            );                                                               \
//!
        BOOST_PP_FOR(
            (0)
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED
          , BOOST_CONTAINER_GEN_PP_PARAM_INC
          , BOOST_TREE_NODE_EMPLACEMENT_IMPL
        )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#elif defined BOOST_CONTAINER_PERFECT_FORWARDING
        template <typename ...Args>
        iterator _add_child(Args&& ...args);
#else  // partial template specialization support, imperfect forwarding
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        iterator                                                             \
            _add_child(                                                      \
                BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                             \
                    n                                                        \
                  , BOOST_CONTAINER_PP_PARAM_LIST                            \
                  , _                                                        \
                )                                                            \
            );                                                               \
//!
        BOOST_PP_REPEAT(
            BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
          , BOOST_TREE_NODE_EMPLACEMENT_IMPL
          , _
        )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#endif  // compiler defect handling

        void _clone_descendants(Derived const& copy);

        void _clone_descendants(Derived const& copy, ::boost::mpl::true_);

        void _clone_descendants(Derived const& copy, ::boost::mpl::false_);

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        void _move_descendants(BOOST_RV_REF(Derived) source);

        void _move_descendants(Derived& source, ::boost::mpl::true_);

        void _move_descendants(Derived& source, ::boost::mpl::false_);

        void _move_descendants_non_ptr(Derived& source, ::boost::mpl::true_);

        void _move_descendants_non_ptr(Derived& source, ::boost::mpl::false_);

        template <typename A>
        void
            _on_post_insert(
                iterator to_child
              , ::boost::vector_selector< ::boost::mpl::true_,A>
            );

        template <typename A>
        void _on_post_resize(::boost::vector_selector< ::boost::mpl::true_,A>);
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        template <typename S>
        void _on_post_insert(iterator to_child, S);

        template <typename S>
        void _on_post_resize(S);

        void _on_post_clone_or_move(::boost::mpl::true_);

        void _on_post_clone_or_move(::boost::mpl::false_);

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        template <typename A>
        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            _splice_ra(
                iterator pos
              , Derived& node
              , iterator itr
              , iterator itr_end
              , ::boost::vector_selector< ::boost::mpl::true_,A>
            );

        template <typename A>
        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            _splice_ra(
                iterator pos
              , Derived& node
              , iterator itr
              , iterator itr_end
              , ::boost::deque_selector< ::boost::mpl::true_,A>
            );
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        template <typename S>
        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            _splice_ra(
                iterator pos
              , Derived& node
              , iterator itr
              , iterator itr_end
              , S
            );

        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            _splice(
                iterator pos
              , Derived& node
              , iterator itr
              , iterator itr_end
              , ::boost::mpl::true_
            );

        typename ::boost::insert_range_result_gen<Selector,Derived>::type
            _splice(
                iterator pos
              , Derived& node
              , iterator itr
              , iterator itr_end
              , ::boost::mpl::false_
            );

        template <typename D, typename T0, typename S, typename V>
        friend void
            put(
                nary_node_base<D,T0,S>& node
              , data_key const& key
              , V const& value
            );

        void _on_post_modify_value(data_key const& key);

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
        template <typename FusionKey>
        void _on_post_modify_value(FusionKey const& key);

        template <
            typename D
          , typename T0
          , typename S
          , typename FusionKey
          , typename V
        >
        friend void
          put(
            nary_node_base<D,T0,S>& node
          , FusionKey const& key
          , V const& value
          , typename ::boost::enable_if<
              typename ::boost::mpl::if_<
                typename ::boost::mpl::eval_if<
                  typename ::boost::fusion::traits::is_sequence<T0>::type
                , ::boost::fusion::traits::is_associative<T0>
                , ::boost::mpl::false_
                >::type
              , result_of::has_key<nary_node_base<D,T0,S>,FusionKey>
              , ::boost::mpl::false_
              >::type
            , ::boost::mpl::true_
            >::type
          );
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION
    };

    template <typename Derived, typename T, typename Selector>
    nary_node_base<Derived,T,Selector>::nary_node_base(Derived const& copy)
      : _children(), _data(copy._data), _parent()
    {
    }

    template <typename Derived, typename T, typename Selector>
    nary_node_base<Derived,T,Selector>::nary_node_base(
        Derived const& copy
      , typename traits::allocator_reference allocator
    ) : _children(allocator), _data(copy._data), _parent()
    {
    }

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    nary_node_base<Derived,T,Selector>::nary_node_base(                      \
        BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)           \
    ) : _children()                                                          \
      , _data(BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p))               \
      , _parent()                                                            \
    {                                                                        \
    }                                                                        \
//!
    BOOST_PP_FOR(
        (0)
      , BOOST_CONTAINER_GEN_PP_PARAM_PRED
      , BOOST_CONTAINER_GEN_PP_PARAM_INC
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    nary_node_base<Derived,T,Selector>::nary_node_base(                      \
        ::boost::container::allocator_arg_t                                  \
      , typename traits::allocator_reference allocator                       \
      , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)           \
    ) : _children()                                                          \
      , _data(BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p))               \
      , _parent()                                                            \
    {                                                                        \
    }                                                                        \
//!
    BOOST_PP_FOR(
        (0)
      , BOOST_CONTAINER_GEN_PP_PARAM_PRED
      , BOOST_CONTAINER_GEN_PP_PARAM_INC
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename Derived, typename T, typename Selector>
    nary_node_base<Derived,T,Selector>::nary_node_base(
        BOOST_RV_REF(Derived) source
    ) : _children(), _data(::boost::move(source._data)), _parent()
    {
    }

    template <typename Derived, typename T, typename Selector>
    nary_node_base<Derived,T,Selector>::nary_node_base(
        BOOST_RV_REF(Derived) source
      , typename traits::allocator_reference allocator
    ) : _children(allocator), _data(::boost::move(source._data)), _parent()
    {
    }

#if defined BOOST_CONTAINER_PERFECT_FORWARDING
    template <typename Derived, typename T, typename Selector>
    template <typename ...Args>
    nary_node_base<Derived,T,Selector>::nary_node_base(Args&& ...args)
      : _children(), _data(::boost::forward<Args>(args)...), _parent()
    {
    }

    template <typename Derived, typename T, typename Selector>
    template <typename ...Args>
    nary_node_base<Derived,T,Selector>::nary_node_base(
        ::boost::container::allocator_arg_t
      , typename traits::allocator_reference allocator
      , Args&& ...args
    ) : _children(allocator), _data(::boost::forward<Args>(args)...), _parent()
    {
    }
#else  // !defined BOOST_CONTAINER_PERFECT_FORWARDING
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    nary_node_base<Derived,T,Selector>::nary_node_base(                      \
        BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                     \
            n                                                                \
          , BOOST_CONTAINER_PP_PARAM_LIST                                    \
          , _                                                                \
        )                                                                    \
    ) : _children()                                                          \
      , _data(                                                               \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_FORWARD                             \
              , _                                                            \
            )                                                                \
        )                                                                    \
      , _parent()                                                            \
    {                                                                        \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    nary_node_base<Derived,T,Selector>::nary_node_base(                      \
        ::boost::container::allocator_arg_t                                  \
      , typename traits::allocator_reference allocator                       \
        BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                            \
            n                                                                \
          , BOOST_CONTAINER_PP_PARAM_LIST                                    \
          , _                                                                \
        )                                                                    \
    ) : _children(allocator)                                                 \
      , _data(                                                               \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_FORWARD                             \
              , _                                                            \
            )                                                                \
        )                                                                    \
      , _parent()                                                            \
    {                                                                        \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#endif  // BOOST_CONTAINER_PERFECT_FORWARDING
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template <typename Derived, typename T, typename Selector>
    nary_node_base<Derived,T,Selector>::~nary_node_base()
    {
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::clone_descendants(
            Derived const& copy
        )
    {
        this->_clone_descendants(copy);
        this->_on_post_clone_or_move(
            ::boost::is_reversible_selector<Selector>()
        );
    }

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::copy_assign(Derived const& copy)
    {
        Derived twin(copy._data);

        twin._clone_descendants(copy);
        twin.clone_metadata(copy);
        this->_children.clear();
        this->_data = twin._data;
        this->_clone_descendants(twin);
        this->clone_metadata(twin);
    }
#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::move_descendants(
            BOOST_RV_REF(Derived) source
        )
    {
#if defined BOOST_NO_RVALUE_REFERENCES
        this->_move_descendants(source);
#else
        this->_move_descendants(static_cast<Derived&&>(source));
#endif
        this->_on_post_clone_or_move(
            ::boost::is_reversible_selector<Selector>()
        );
        source.on_post_clear();
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::copy_assign(
            BOOST_COPY_ASSIGN_REF(Derived) copy
        )
    {
        Derived twin(copy._data);

        twin._clone_descendants(static_cast<Derived const&>(copy));
        twin.clone_metadata(static_cast<Derived const&>(copy));
        this->_children.clear();
        this->_data = ::boost::move(twin._data);
        this->_move_descendants(::boost::move(twin));
        this->move_metadata(::boost::move(twin));
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::move_assign(
            BOOST_RV_REF(Derived) source
        )
    {
        this->_children.clear();
        this->_data = ::boost::move(source._data);
#if defined BOOST_NO_RVALUE_REFERENCES
        this->_move_descendants(source);
        this->move_metadata(source);
#else
        this->_move_descendants(static_cast<Derived&&>(source));
        this->move_metadata(static_cast<Derived&&>(source));
#endif
        this->_on_post_clone_or_move(
            ::boost::is_reversible_selector<Selector>()
        );
        source.on_post_clear();
    }
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template <typename Derived, typename T, typename Selector>
    inline void nary_node_base<Derived,T,Selector>::on_post_assign()
    {
        if (this->_parent)
        {
            this->_parent->on_post_propagate_value(data_key());
        }
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<
        Derived
      , T
      , Selector
    >::traits::data_type const&
        nary_node_base<Derived,T,Selector>::operator[](data_key const&) const
    {
        return this->_data;
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::traits::data_type&
        nary_node_base<Derived,T,Selector>::operator[](data_key const&)
    {
        return this->_data;
    }

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
    template <typename Derived, typename T, typename Selector>
    template <typename FusionKey>
    inline typename ::boost::lazy_enable_if<
        typename ::boost::mpl::if_<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , result_of::has_key<
                nary_node_base<Derived,T,Selector>
              , FusionKey
            >
          , ::boost::mpl::false_
        >::type
      , ::boost::fusion::result_of::at_key<T const,FusionKey>
    >::type
        nary_node_base<Derived,T,Selector>::operator[](FusionKey const&) const
    {
        return ::boost::fusion::at_key<FusionKey>(this->_data);
    }

    template <typename Derived, typename T, typename Selector>
    template <typename FusionKey>
    inline typename ::boost::lazy_enable_if<
        typename ::boost::mpl::if_<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , result_of::has_key<
                nary_node_base<Derived,T,Selector>
              , FusionKey
            >
          , ::boost::mpl::false_
        >::type
      , ::boost::fusion::result_of::at_key<T,FusionKey>
    >::type
        nary_node_base<Derived,T,Selector>::operator[](FusionKey const&)
    {
        return ::boost::fusion::at_key<FusionKey>(this->_data);
    }
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_pointer
        nary_node_base<Derived,T,Selector>::get_parent_ptr() const
    {
        return this->_parent;
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::pointer
        nary_node_base<Derived,T,Selector>::get_parent_ptr()
    {
        return this->_parent;
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_iterator
        nary_node_base<Derived,T,Selector>::cbegin() const
    {
        return this->_children.cbegin();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_iterator
        nary_node_base<Derived,T,Selector>::begin() const
    {
        return this->_children.begin();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::iterator
        nary_node_base<Derived,T,Selector>::begin()
    {
        return this->_children.begin();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_iterator
        nary_node_base<Derived,T,Selector>::cend() const
    {
        return this->_children.cend();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_iterator
        nary_node_base<Derived,T,Selector>::end() const
    {
        return this->_children.end();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::iterator
        nary_node_base<Derived,T,Selector>::end()
    {
        return this->_children.end();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_reverse_iterator
        nary_node_base<Derived,T,Selector>::crbegin() const
    {
        return this->_children.crbegin();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_reverse_iterator
        nary_node_base<Derived,T,Selector>::rbegin() const
    {
        return this->_children.rbegin();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::reverse_iterator
        nary_node_base<Derived,T,Selector>::rbegin()
    {
        return this->_children.rbegin();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_reverse_iterator
        nary_node_base<Derived,T,Selector>::crend() const
    {
        return this->_children.crend();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::const_reverse_iterator
        nary_node_base<Derived,T,Selector>::rend() const
    {
        return this->_children.rend();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::reverse_iterator
        nary_node_base<Derived,T,Selector>::rend()
    {
        return this->_children.rend();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::size_type
        nary_node_base<Derived,T,Selector>::size() const
    {
        return this->_children.size();
    }

    template <typename Derived, typename T, typename Selector>
    inline bool nary_node_base<Derived,T,Selector>::empty() const
    {
        return this->_children.empty();
    }

    template <typename Derived, typename T, typename Selector>
    inline void nary_node_base<Derived,T,Selector>::clear()
    {
        this->_children.clear();
        this->on_post_clear();
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::iterator
        nary_node_base<Derived,T,Selector>::insert(Derived const& child)
    {
        Derived twin(child._data);

        twin._clone_descendants(child);
        twin.clone_metadata(child);

        typename ::boost::emplace_function_gen<Selector>::type emplacer;
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        ::std::pair<iterator,bool> p = emplacer(this->_children, twin._data);
#else
        ::std::pair<iterator,bool> p = emplacer(
            this->_children
          , ::boost::move(twin._data)
        );
#endif

        if (p.second)
        {
            this->_on_post_insert(p.first, Selector());
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
            (*p.first)._clone_descendants(twin);
            (*p.first).move_metadata(twin);
#else
            (*p.first)._move_descendants(::boost::move(twin));
            (*p.first).move_metadata(::boost::move(twin));
#endif
            (*p.first).on_post_inserted(
                p.first
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }
        else
        {
            BOOST_ASSERT_MSG(
                false
              , "This type does not yet handle associative selectors."
            );
        }

        return p.first;
    }

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    inline typename nary_node_base<Derived,T,Selector>::iterator             \
        nary_node_base<Derived,T,Selector>::emplace(                         \
            BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        typename ::boost::emplace_function_gen<Selector>::type emplacer;     \
        ::std::pair<iterator,bool> p = emplacer(                             \
            this->_children                                                  \
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)         \
        );                                                                   \
        if (p.second)                                                        \
        {                                                                    \
            this->_on_post_insert(p.first, Selector());                      \
            (*p.first).on_post_inserted(                                     \
                p.first                                                      \
              , ::boost::has_stable_iterators_selector<Selector>()           \
            );                                                               \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            BOOST_ASSERT_MSG(                                                \
                false                                                        \
              , "This type does not yet handle associative selectors."       \
            );                                                               \
        }                                                                    \
        return p.first;                                                      \
    }                                                                        \
//!
    BOOST_PP_FOR(
        (0)
      , BOOST_CONTAINER_GEN_PP_PARAM_PRED
      , BOOST_CONTAINER_GEN_PP_PARAM_INC
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::iterator
        nary_node_base<Derived,T,Selector>::insert(BOOST_RV_REF(Derived) child)
    {
        typename ::boost::emplace_function_gen<Selector>::type emplacer;
        ::std::pair<iterator,bool> p = emplacer(
            this->_children
          , ::boost::move(child._data)
        );

        if (p.second)
        {
            this->_on_post_insert(p.first, Selector());
#if defined BOOST_NO_RVALUE_REFERENCES
            (*p.first)._move_descendants(child);
            (*p.first).move_metadata(child);
#else
            (*p.first)._move_descendants(static_cast<Derived&&>(child));
            (*p.first).move_metadata(static_cast<Derived&&>(child));
#endif
            (*p.first).on_post_inserted(
                p.first
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }
        else
        {
            BOOST_ASSERT_MSG(
                false
              , "This type does not yet handle associative selectors."
            );
        }

        return p.first;
    }

#if defined BOOST_CONTAINER_PERFECT_FORWARDING
    template <typename Derived, typename T, typename Selector>
    template <typename ...Args>
    inline typename nary_node_base<Derived,T,Selector>::iterator
        nary_node_base<Derived,T,Selector>::emplace(Args&& ...args)
    {
        typename ::boost::emplace_function_gen<Selector>::type emplacer;
        ::std::pair<iterator,bool> p = emplacer(
            this->_children
          , ::boost::forward<Args>(args)...
        );

        if (p.second)
        {
            this->_on_post_insert(p.first, Selector());
            (*p.first).on_post_inserted(
                p.first
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }
        else
        {
            BOOST_ASSERT_MSG(
                false
              , "This type does not yet handle associative selectors."
            );
        }

        return p.first;
    }
#else  // !defined BOOST_CONTAINER_PERFECT_FORWARDING
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    inline typename nary_node_base<Derived,T,Selector>::iterator             \
        nary_node_base<Derived,T,Selector>::emplace(                         \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        typename ::boost::emplace_function_gen<Selector>::type emplacer;     \
        ::std::pair<iterator,bool> p = emplacer(                             \
            this->_children                                                  \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_FORWARD                             \
              , _                                                            \
            )                                                                \
        );                                                                   \
        if (p.second)                                                        \
        {                                                                    \
            this->_on_post_insert(p.first, Selector());                      \
            (*p.first).on_post_inserted(                                     \
                p.first                                                      \
              , ::boost::has_stable_iterators_selector<Selector>()           \
            );                                                               \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            BOOST_ASSERT_MSG(                                                \
                false                                                        \
              , "This type does not yet handle associative selectors."       \
            );                                                               \
        }                                                                    \
        return p.first;                                                      \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#endif  // BOOST_CONTAINER_PERFECT_FORWARDING
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template <typename Derived, typename T, typename Selector>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::splice(iterator pos, Derived& node)
    {
#if 0
        typename ::boost::insert_range_result_gen<
            Selector
          , Derived
        >::type result = this->_splice(
            pos
          , node
          , ::boost::begin(node._children)
          , ::boost::end(node._children)
          , ::boost::is_random_access_selector<Selector>()
        );

        node.on_post_clear();
        this->on_post_insert(
            ::boost::begin(result)
          , ::boost::end(result)
          , ::boost::has_stable_iterators_selector<Selector>()
        );
        return result;
#endif
        return this->splice(
            pos
          , node
          , ::boost::begin(node._children)
          , ::boost::end(node._children)
        );
    }

    template <typename Derived, typename T, typename Selector>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::splice(
            iterator pos
          , Derived& node
          , iterator itr
        )
    {
        iterator itr_end = itr;
        return this->splice(
            pos
          , node
          , itr
          , ++itr_end
        );
    }

    template <typename Derived, typename T, typename Selector>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::splice(
            iterator pos
          , Derived& node
          , iterator itr
          , iterator itr_end
        )
    {
        if ((pos == itr) || (pos == itr_end))
        {
            BOOST_ASSERT(this->get_derived() == node.get_derived());
            return ::std::pair<iterator,iterator>(itr, itr_end);
        }
        else
        {
            return this->_splice(
                pos
              , node
              , itr
              , itr_end
              , typename ::boost::mpl::if_<
                    ::boost::is_ptr_selector<Selector>
                  , ::boost::mpl::false_
                  , ::boost::is_random_access_selector<Selector>
                >::type()
            );
        }
    }

    template <typename Derived, typename T, typename Selector>
    void
        nary_node_base<Derived,T,Selector>::_link_children_to_parent(
            pointer const& p
          , iterator itr
          , iterator itr_end
        )
    {
        for (; itr != itr_end; ++itr)
        {
            (*itr)._parent = p;
        }
    }

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    inline typename nary_node_base<Derived,T,Selector>::iterator             \
        nary_node_base<Derived,T,Selector>::_add_child(                      \
            BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        typename ::boost::emplace_function_gen<Selector>::type emplacer;     \
        ::std::pair<iterator,bool> p = emplacer(                             \
            this->_children                                                  \
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)         \
        );                                                                   \
        if (p.second)                                                        \
        {                                                                    \
            (*p.first)._parent = this->get_derived();                        \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            BOOST_ASSERT_MSG(                                                \
                false                                                        \
              , "This type does not yet handle associative selectors."       \
            );                                                               \
        }                                                                    \
        return p.first;                                                      \
    }                                                                        \
//!
    BOOST_PP_FOR(
        (0)
      , BOOST_CONTAINER_GEN_PP_PARAM_PRED
      , BOOST_CONTAINER_GEN_PP_PARAM_INC
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#elif defined BOOST_CONTAINER_PERFECT_FORWARDING
    template <typename Derived, typename T, typename Selector>
    template <typename ...Args>
    inline typename nary_node_base<Derived,T,Selector>::iterator
        nary_node_base<Derived,T,Selector>::_add_child(Args&& ...args)
    {
        typename ::boost::emplace_function_gen<Selector>::type emplacer;
        ::std::pair<iterator,bool> p = emplacer(
            this->_children
          , ::boost::forward<Args>(args)...
        );

        if (p.second)
        {
            (*p.first)._parent = this->get_derived();
        }
        else
        {
            BOOST_ASSERT_MSG(
                false
              , "This type does not yet handle associative selectors."
            );
        }

        return p.first;
    }
#else  // partial template specialization support, imperfect forwarding
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename Derived, typename T, typename Selector>               \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    inline typename nary_node_base<Derived,T,Selector>::iterator             \
        nary_node_base<Derived,T,Selector>::_add_child(                      \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        typename ::boost::emplace_function_gen<Selector>::type emplacer;     \
        ::std::pair<iterator,bool> p = emplacer(                             \
            this->_children                                                  \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_FORWARD                             \
              , _                                                            \
            )                                                                \
        );                                                                   \
        if (p.second)                                                        \
        {                                                                    \
            (*p.first)._parent = this->get_derived();                        \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            BOOST_ASSERT_MSG(                                                \
                false                                                        \
              , "This type does not yet handle associative selectors."       \
            );                                                               \
        }                                                                    \
        return p.first;                                                      \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL
#endif  // compiler defect handling

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::_clone_descendants(
            Derived const& copy
        )
    {
        this->_clone_descendants(
            copy
          , typename ::boost::mpl::if_<
                ::boost::is_ptr_selector<Selector>
              , ::boost::mpl::false_
              , ::boost::is_random_access_selector<Selector>
            >::type()
        );
        this->get_derived()->on_post_copy_or_move();
    }

    template <typename Derived, typename T, typename Selector>
    void
        nary_node_base<Derived,T,Selector>::_clone_descendants(
            Derived const& copy
          , ::boost::mpl::true_
        )
    {
        pointer p = this->get_derived();
        ::std::deque<size_type> indices;
        size_type current_index = ::boost::initialized_value;

        p->_children.resize(copy._children.size(), Derived(copy._data));

        for (
            depth_first_descendant_iterator<Derived const> itr(copy);
            itr;
            ++itr
        )
        {
            switch (::boost::tree_node::traversal_state(itr))
            {
                case ::boost::tree_node::pre_order_traversal:
                {
                    p->_children[current_index]._parent = p;
                    p = &p->_children[current_index];
                    indices.push_back(current_index);
                    p->_children.clear();
                    p->_children.resize(
                        (*itr)._children.size()
                      , Derived((*itr)._data)
                    );
                    p->_data = (*itr)._data;
                    p->clone_metadata(*itr);
                    current_index = ::boost::initialized_value;
                    break;
                }

                case ::boost::tree_node::post_order_traversal:
                {
                    p->on_post_copy_or_move();
                    p = p->get_parent_ptr();
                    ++(current_index = indices.back());
                    indices.pop_back();
                    break;
                }

                default:
                {
                    BOOST_ASSERT_MSG(
                        false
                      , "traversal_state must be pre- or post-order!"
                    );
                }
            }
        }

        BOOST_ASSERT_MSG(p == this->get_derived(), "itr not at-the-end");
    }

    template <typename Derived, typename T, typename Selector>
    void
        nary_node_base<Derived,T,Selector>::_clone_descendants(
            Derived const& copy
          , ::boost::mpl::false_
        )
    {
        pointer p = this->get_derived();

        for (
            depth_first_descendant_iterator<Derived const> itr(copy);
            itr;
            ++itr
        )
        {
            switch (::boost::tree_node::traversal_state(itr))
            {
                case ::boost::tree_node::pre_order_traversal:
                {
                    p = &*p->_add_child((*itr)._data);
                    p->clone_metadata(*itr);
                    break;
                }

                case ::boost::tree_node::post_order_traversal:
                {
                    p->on_post_copy_or_move();
                    p = p->get_parent_ptr();
                    break;
                }

                default:
                {
                    BOOST_ASSERT_MSG(
                        false
                      , "traversal_state must be pre- or post-order!"
                    );
                }
            }
        }

        BOOST_ASSERT_MSG(p == this->get_derived(), "itr not at-the-end");
    }

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::_move_descendants(
            BOOST_RV_REF(Derived) source
        )
    {
        this->_move_descendants(
            static_cast<Derived&>(source)
          , ::boost::is_ptr_selector<Selector>()
        );
        this->get_derived()->on_post_copy_or_move();
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::_move_descendants(
            Derived& source
          , ::boost::mpl::true_
        )
    {
        this->_children.transfer(this->_children.begin(), source._children);
        self::_link_children_to_parent(
            this->get_derived()
          , this->_children.begin()
          , this->_children.end()
        );
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::_move_descendants(
            Derived& source
          , ::boost::mpl::false_
        )
    {
        this->_move_descendants_non_ptr(
            source
          , ::boost::is_random_access_selector<Selector>()
        );
        source._children.clear();
    }

    template <typename Derived, typename T, typename Selector>
    void
        nary_node_base<Derived,T,Selector>::_move_descendants_non_ptr(
            Derived& source
          , ::boost::mpl::true_
        )
    {
        pointer p = this->get_derived();
        ::std::deque<size_type> indices;
        size_type current_index = ::boost::initialized_value;

        p->_children.resize(source._children.size(), Derived(this->_data));
        p->_on_post_resize(Selector());

        for (depth_first_descendant_iterator<Derived> itr(source); itr; ++itr)
        {
            switch (::boost::tree_node::traversal_state(itr))
            {
                case ::boost::tree_node::pre_order_traversal:
                {
                    p->_children[current_index]._parent = p;
                    p = &p->_children[current_index];
                    indices.push_back(current_index);
                    p->_children.clear();
                    p->_children.resize(
                        (*itr)._children.size()
                      , Derived((*itr)._data)
                    );
                    p->_on_post_resize(Selector());
                    p->_data = ::boost::move((*itr)._data);
                    p->move_metadata(::boost::move(*itr));
                    current_index = ::boost::initialized_value;
                    break;
                }

                case ::boost::tree_node::post_order_traversal:
                {
                    (*itr)._children.clear();
                    p->on_post_copy_or_move();
                    p = p->get_parent_ptr();
                    ++(current_index = indices.back());
                    indices.pop_back();
                    break;
                }

                default:
                {
                    BOOST_ASSERT_MSG(
                        false
                      , "traversal_state must be pre- or post-order!"
                    );
                }
            }
        }

        BOOST_ASSERT_MSG(p == this->get_derived(), "itr not at-the-end");
    }

    template <typename Derived, typename T, typename Selector>
    void
        nary_node_base<Derived,T,Selector>::_move_descendants_non_ptr(
            Derived& source
          , ::boost::mpl::false_
        )
    {
        pointer p = this->get_derived();

        for (depth_first_descendant_iterator<Derived> itr(source); itr; ++itr)
        {
            switch (::boost::tree_node::traversal_state(itr))
            {
                case ::boost::tree_node::pre_order_traversal:
                {
                    p = &*p->_add_child(::boost::move((*itr)._data));
                    p->move_metadata(::boost::move(*itr));
                    break;
                }

                case ::boost::tree_node::post_order_traversal:
                {
                    p->on_post_copy_or_move();
                    p = p->get_parent_ptr();
                    break;
                }

                default:
                {
                    BOOST_ASSERT_MSG(
                        false
                      , "traversal_state must be pre- or post-order!"
                    );
                }
            }
        }

        BOOST_ASSERT_MSG(p == this->get_derived(), "itr not at-the-end");
    }

    template <typename Derived, typename T, typename Selector>
    template <typename A>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_insert(
            iterator to_child
          , ::boost::vector_selector< ::boost::mpl::true_,A>
        )
    {
        self::_link_children_to_parent(
            this->get_derived()
          , this->_children.begin()
          , this->_children.end()
        );
    }

    template <typename Derived, typename T, typename Selector>
    template <typename A>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_resize(
            ::boost::vector_selector< ::boost::mpl::true_,A>
        )
    {
        self::_link_children_to_parent(
            this->get_derived()
          , this->_children.begin()
          , this->_children.end()
        );
    }
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template <typename Derived, typename T, typename Selector>
    template <typename S>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_insert(
            iterator to_child
          , S
        )
    {
        (*to_child)._parent = this->get_derived();
    }

    template <typename Derived, typename T, typename Selector>
    template <typename S>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_resize(S)
    {
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_clone_or_move(
            ::boost::mpl::true_
        )
    {
    }

    template <typename Derived, typename T, typename Selector>
    void
        nary_node_base<Derived,T,Selector>::_on_post_clone_or_move(
            ::boost::mpl::false_
        )
    {
        for (
            post_order_iterator<Derived> itr(*this->get_derived());
            itr;
            ++itr
        )
        {
            (*itr)._children.reverse();
        }
    }

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename Derived, typename T, typename Selector>
    template <typename A>
    typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::_splice_ra(
            iterator pos
          , Derived& node
          , iterator itr
          , iterator itr_end
          , ::boost::vector_selector< ::boost::mpl::true_,A>
        )
    {
        children splice_buffer(itr, itr_end);
        children move_buffer(itr_end, node._children.end());
        size_type const result_size = splice_buffer.size();
        size_type insert_index = ::std::distance(this->_children.begin(), pos);

        if ((this->get_derived() == node.get_derived()) && (itr < pos))
        {
            BOOST_ASSERT_MSG(
                !(pos < itr_end)
              , "Insertion point lies inside subrange to splice."
            );
            insert_index -= result_size;
        }

        for (
            itr_end = move_buffer.begin();
            itr_end != move_buffer.end();
            ++itr_end
        )
        {
            (*itr)._data = ::boost::move((*itr_end)._data);
            (*itr)._move_descendants(::boost::move(*itr_end));
            (*itr).move_metadata(::boost::move(*itr_end));
            ++itr;
        }

        move_buffer.clear();

        for (size_type erase_count = result_size; erase_count; --erase_count)
        {
            node._children.pop_back();
        }

        if (result_size)
        {
            node.on_post_erase(
                ::boost::has_stable_iterators_selector<Selector>()
            );
        }

        if (insert_index < this->_children.size())
        {
            for (
                pos = this->_children.begin() + insert_index;
                pos != this->_children.end();
                ++pos
            )
            {
                move_buffer.emplace_back((*pos)._data);
                move_buffer.back()._clone_descendants(*pos);
                move_buffer.back().clone_metadata(*pos);
            }

            for (
                ;
                insert_index < this->_children.size();
                this->_children.pop_back()
            )
            {
            }

            this->on_post_erase(
                ::boost::has_stable_iterators_selector<Selector>()
            );
        }

        for (itr = splice_buffer.begin(); itr != splice_buffer.end(); ++itr)
        {
            this->insert(::boost::move(*itr));
        }

        for (itr = move_buffer.begin(); itr != move_buffer.end(); ++itr)
        {
            this->insert(::boost::move(*itr));
        }

        pos = this->_children.begin() + insert_index;
        return ::std::pair<iterator,iterator>(pos, pos + result_size);
    }

    template <typename Derived, typename T, typename Selector>
    template <typename A>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::_splice_ra(
            iterator pos
          , Derived& node
          , iterator itr
          , iterator itr_end
          , ::boost::deque_selector< ::boost::mpl::true_,A>
        )
    {
        typedef ::boost::move_iterator<iterator> MoveIterator;

        MoveIterator _itr(itr);
        MoveIterator _itr_end(itr_end);
        children splice_buffer(_itr, _itr_end);
        size_type const result_size = splice_buffer.size();
        size_type insert_index = ::std::distance(this->_children.begin(), pos);

        if (this->get_derived() == node.get_derived())
        {
            if (itr < pos)
            {
                BOOST_ASSERT_MSG(
                    !(pos < itr_end)
                  , "Insertion point lies inside subrange to splice."
                );
                insert_index -= result_size;
            }

            node._children.erase(itr, itr_end);
            self::_link_children_to_parent(
#if defined BOOST_NO_CXX11_NULLPTR
                0
#else
                nullptr
#endif
              , this->_children.begin()
              , this->_children.end()
            );
            node.on_post_clear();
            pos = this->_children.insert(
                this->_children.begin() + insert_index
              , MoveIterator(splice_buffer.begin())
              , MoveIterator(splice_buffer.end())
            );
            self::_link_children_to_parent(
                this->get_derived()
              , this->_children.begin()
              , this->_children.end()
            );
            this->on_post_insert(
                this->_children.begin()
              , this->_children.end()
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }
        else
        {
            node._children.erase(itr, itr_end);
            node.on_post_erase(
                ::boost::has_stable_iterators_selector<Selector>()
            );
            pos = this->_children.insert(
                this->_children.begin() + insert_index
              , MoveIterator(splice_buffer.begin())
              , MoveIterator(splice_buffer.end())
            );
            self::_link_children_to_parent(
                this->get_derived()
              , this->_children.begin()
              , this->_children.end()
            );
            this->get_derived()->on_post_copy_or_move();
        }

        return ::std::pair<iterator,iterator>(pos, pos + result_size);
    }
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template <typename Derived, typename T, typename Selector>
    template <typename S>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::_splice_ra(
            iterator pos
          , Derived& node
          , iterator itr
          , iterator itr_end
          , S
        )
    {
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        children splice_buffer(itr, itr_end);
#else
        typedef ::boost::move_iterator<iterator> MoveIterator;

        MoveIterator _itr(itr);
        MoveIterator _itr_end(itr_end);
        children splice_buffer(_itr, _itr_end);
#endif
        size_type const result_size = splice_buffer.size();
        size_type insert_index = ::std::distance(this->_children.begin(), pos);

        if (this->get_derived() == node.get_derived())
        {
            if (itr < pos)
            {
                BOOST_ASSERT_MSG(
                    !(pos < itr_end)
                  , "Insertion point lies inside subrange to splice."
                );
                insert_index -= result_size;
            }

            node._children.erase(itr, itr_end);
            self::_link_children_to_parent(
#if defined BOOST_NO_CXX11_NULLPTR
                0
#else
                nullptr
#endif
              , this->_children.begin()
              , this->_children.end()
            );
            node.on_post_clear();
            pos = this->_children.insert(
                this->_children.begin() + insert_index
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
              , splice_buffer.begin()
              , splice_buffer.end()
#else
              , MoveIterator(splice_buffer.begin())
              , MoveIterator(splice_buffer.end())
#endif
            );
            self::_link_children_to_parent(
                this->get_derived()
              , this->_children.begin()
              , this->_children.end()
            );
            this->on_post_insert(
                this->_children.begin()
              , this->_children.end()
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }
        else
        {
            node._children.erase(itr, itr_end);
            node.on_post_erase(
                ::boost::has_stable_iterators_selector<Selector>()
            );
            pos = this->_children.insert(
                this->_children.begin() + insert_index
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
              , splice_buffer.begin()
              , splice_buffer.end()
#else
              , MoveIterator(splice_buffer.begin())
              , MoveIterator(splice_buffer.end())
#endif
            );
            self::_link_children_to_parent(
                this->get_derived()
              , this->_children.begin()
              , this->_children.end()
            );
            this->on_post_insert(
                pos
              , pos + result_size
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }

        return ::std::pair<iterator,iterator>(pos, pos + result_size);
    }

    template <typename Derived, typename T, typename Selector>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::_splice(
            iterator pos
          , Derived& node
          , iterator itr
          , iterator itr_end
          , ::boost::mpl::true_
        )
    {
        return this->_splice_ra(pos, node, itr, itr_end, Selector());
    }

    template <typename Derived, typename T, typename Selector>
    inline typename ::boost::insert_range_result_gen<Selector,Derived>::type
        nary_node_base<Derived,T,Selector>::_splice(
            iterator pos
          , Derived& node
          , iterator itr
          , iterator itr_end
          , ::boost::mpl::false_
        )
    {
        typename ::boost::splice_function_gen<Selector>::type splicer;
        typename ::boost::insert_range_result_gen<
            Selector
          , Derived
        >::type result = splicer(
            this->_children
          , pos
          , node._children
          , itr
          , itr_end
        );

        if (this->get_derived() == node.get_derived())
        {
            self::_link_children_to_parent(
#if defined BOOST_NO_CXX11_NULLPTR
                0
#else
                nullptr
#endif
              , this->_children.begin()
              , this->_children.end()
            );
            node.on_post_clear();
            self::_link_children_to_parent(
                this->get_derived()
              , this->_children.begin()
              , this->_children.end()
            );
            this->on_post_insert(
                this->_children.begin()
              , this->_children.end()
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }
        else
        {
            self::_link_children_to_parent(
                this->get_derived()
              , ::boost::begin(result)
              , ::boost::end(result)
            );
            node.on_post_erase(
                ::boost::has_stable_iterators_selector<Selector>()
            );
            this->on_post_insert(
                ::boost::begin(result)
              , ::boost::end(result)
              , ::boost::has_stable_iterators_selector<Selector>()
            );
        }

        return result;
    }

    template <typename Derived, typename T, typename Selector>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_modify_value(
            data_key const& key
        )
    {
        this->on_post_propagate_value(key);
    }

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
    template <typename Derived, typename T, typename Selector>
    template <typename FusionKey>
    inline void
        nary_node_base<Derived,T,Selector>::_on_post_modify_value(
            FusionKey const& key
        )
    {
        this->on_post_propagate_value(key);
    }
#endif
}}  // namespace boost::tree_node

namespace boost { namespace tree_node {

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<
        Derived
      , T
      , Selector
    >::traits::data_type const&
        get(
            nary_node_base<Derived,T,Selector> const& node
          , data_key const& key
        )
    {
        return node[key];
    }

    template <typename Derived, typename T, typename Selector>
    inline typename nary_node_base<Derived,T,Selector>::traits::data_type&
        get(nary_node_base<Derived,T,Selector>& node, data_key const& key)
    {
        return node[key];
    }

#if !defined BOOST_NO_SFINAE
    template <typename Key, typename Derived, typename T, typename Selector>
    inline typename ::boost::enable_if<
        ::std::tr1::is_same<Key,data_key>
      , typename nary_node_base<
            Derived
          , T
          , Selector
        >::traits::data_type const&
    >::type
        get(nary_node_base<Derived,T,Selector> const& node)
    {
        return node[data_key()];
    }

    template <typename Key, typename Derived, typename T, typename Selector>
    inline typename ::boost::enable_if<
        ::std::tr1::is_same<Key,data_key>
      , typename nary_node_base<
            Derived
          , T
          , Selector
        >::traits::data_type&
    >::type
        get(nary_node_base<Derived,T,Selector>& node)
    {
        return node[data_key()];
    }
#endif  // !defined BOOST_NO_SFINAE

    template <typename Derived, typename T, typename Selector, typename V>
    inline void
        put(
            nary_node_base<Derived,T,Selector>& node
          , data_key const& key
          , V const& value
        )
    {
        node[key] = value;
        node._on_post_modify_value(key);
    }

#if defined BOOST_TREE_NODE_CAN_USE_FUSION
    template <
        typename Derived
      , typename T
      , typename Selector
      , typename FusionKey
    >
    inline typename ::boost::lazy_enable_if<
        typename ::boost::mpl::if_<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , result_of::has_key<
                nary_node_base<Derived,T,Selector>
              , FusionKey
            >
          , ::boost::mpl::false_
        >::type
      , ::boost::fusion::result_of::at_key<T const,FusionKey>
    >::type
        get(
            nary_node_base<Derived,T,Selector> const& node
          , FusionKey const& key
        )
    {
        return node[key];
    }

    template <
        typename Derived
      , typename T
      , typename Selector
      , typename FusionKey
    >
    inline typename ::boost::lazy_enable_if<
        typename ::boost::mpl::if_<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , result_of::has_key<
                nary_node_base<Derived,T,Selector>
              , FusionKey
            >
          , ::boost::mpl::false_
        >::type
      , ::boost::fusion::result_of::at_key<T,FusionKey>
    >::type
        get(nary_node_base<Derived,T,Selector>& node, FusionKey const& key)
    {
        return node[key];
    }

    template <
        typename FusionKey
      , typename Derived
      , typename T
      , typename Selector
    >
    inline typename ::boost::lazy_enable_if<
        typename ::boost::mpl::if_<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , result_of::has_key<
                nary_node_base<Derived,T,Selector>
              , FusionKey
            >
          , ::boost::mpl::false_
        >::type
      , ::boost::fusion::result_of::at_key<T const,FusionKey>
    >::type
        get(nary_node_base<Derived,T,Selector> const& node)
    {
        return node[FusionKey()];
    }

    template <
        typename FusionKey
      , typename Derived
      , typename T
      , typename Selector
    >
    inline typename ::boost::lazy_enable_if<
        typename ::boost::mpl::if_<
            typename ::boost::mpl::eval_if<
                typename ::boost::fusion::traits::is_sequence<T>::type
              , ::boost::fusion::traits::is_associative<T>
              , ::boost::mpl::false_
            >::type
          , result_of::has_key<
                nary_node_base<Derived,T,Selector>
              , FusionKey
            >
          , ::boost::mpl::false_
        >::type
      , ::boost::fusion::result_of::at_key<T,FusionKey>
    >::type
        get(nary_node_base<Derived,T,Selector>& node)
    {
        return node[FusionKey()];
    }

    template <
        typename Derived
      , typename T
      , typename Selector
      , typename FusionKey
      , typename V
    >
    inline void
        put(
            nary_node_base<Derived,T,Selector>& node
          , FusionKey const& key
          , V const& value
          , typename ::boost::enable_if<
              typename ::boost::mpl::if_<
                typename ::boost::mpl::eval_if<
                  typename ::boost::fusion::traits::is_sequence<T>::type
                , ::boost::fusion::traits::is_associative<T>
                , ::boost::mpl::false_
                >::type
              , result_of::has_key<
                  nary_node_base<Derived,T,Selector>
                , FusionKey
                >
              , ::boost::mpl::false_
              >::type
            , ::boost::mpl::true_
            >::type
        )
    {
        node[key] = value;
        node._on_post_modify_value(key);
    }
#endif  // defined BOOST_TREE_NODE_CAN_USE_FUSION
}}  // namespace boost::tree_node

//[reference__nary_node_base_gen
namespace boost { namespace tree_node {

    template <typename Selector = ::boost::ptr_dequeS>
    struct nary_node_base_gen
    {
        template <typename Derived, typename T>
        struct apply
        {
            typedef nary_node_base<Derived,T,Selector> type;
        };
    };

    typedef nary_node_base_gen<> nary_node_base_default_gen;
}}  // namespace boost::tree_node
//]

namespace boost { namespace tree_node {

    template <typename T, typename Selector>
    struct nary_node
      : public
        //[reference__nary_node__bases
        nary_node_base<nary_node<T,Selector>,T,Selector>
        //]
    {
        //[reference__nary_node__super_t
        typedef nary_node_base<nary_node,T,Selector>
                super_t;
        //]

        //[reference__nary_node__traits
        typedef typename super_t::traits
                traits;
        //]

        //[reference__nary_node__pointer
        typedef typename super_t::pointer
                pointer;
        //]

        //[reference__nary_node__const_pointer
        typedef typename super_t::const_pointer
                const_pointer;
        //]

        //[reference__nary_node__iterator
        typedef typename super_t::iterator
                iterator;
        //]

        //[reference__nary_node__const_iterator
        typedef typename super_t::const_iterator
                const_iterator;
        //]

        //[reference__nary_node__reverse_iterator
        typedef typename super_t::reverse_iterator
                reverse_iterator;
        //]

        //[reference__nary_node__const_reverse_iterator
        typedef typename super_t::const_reverse_iterator
                const_reverse_iterator;
        //]

        //[reference__nary_node__size_type
        typedef typename super_t::size_type
                size_type;
        //]

        BOOST_TREE_NODE_COPYABLE_AND_MOVABLE(nary_node, super_t)

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        BOOST_PP_FOR(
            ((0), (nary_node, super_t))
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED_WITH_DATA
          , BOOST_CONTAINER_GEN_PP_PARAM_INC_WITH_DATA
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_INLINE_DEF
        )

        BOOST_PP_FOR(
            ((0), (nary_node, super_t))
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED_WITH_DATA
          , BOOST_CONTAINER_GEN_PP_PARAM_INC_WITH_DATA
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_W_ALLOC_INLINE_DEF
        )
#elif defined BOOST_CONTAINER_PERFECT_FORWARDING
        //[reference__nary_node__emplacement_ctor
        template <typename ...Args>
        explicit nary_node(Args&& ...args);
        //]

        //[reference__nary_node__emplacement_ctor_w_allocator
        template <typename ...Args>
        explicit nary_node(
            ::boost::container::allocator_arg_t
          , typename traits::allocator_reference allocator
          , Args&& ...args
        );
        //]
#else  // partial template specialization support, imperfect forwarding
        BOOST_PP_REPEAT(
            BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_INLINE_DEF
          , (nary_node, super_t)
        )

        BOOST_PP_REPEAT(
            BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
          , BOOST_TREE_NODE_EMPLACEMENT_CTOR_W_ALLOC_INLINE_DEF
          , (nary_node, super_t)
        )
#endif  // compiler defect handling
    };

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION && \
    defined BOOST_CONTAINER_PERFECT_FORWARDING
    template <typename T, typename Selector>
    template <typename ...Args>
    inline nary_node<T,Selector>::nary_node(Args&& ...args)
      : super_t(::boost::forward<Args>(args)...)
    {
    }

    template <typename T, typename Selector>
    template <typename ...Args>
    inline nary_node<T,Selector>::nary_node(
        ::boost::container::allocator_arg_t
      , typename traits::allocator_reference allocator
      , Args&& ...args
    ) : super_t(
            ::boost::container::allocator_arg
          , allocator
          , ::boost::forward<Args>(args)...
        )
    {
    }
#endif  // partial template specialization support, perfect forwarding
}}  // namespace boost::tree_node

//[reference__nary_node_gen
namespace boost { namespace tree_node {

    template <typename Selector = ::boost::ptr_dequeS>
    struct nary_node_gen
    {
        template <typename T>
        struct apply
        {
            typedef nary_node<T,Selector> type;
        };
    };

    typedef nary_node_gen<> nary_node_default_gen;
}}  // namespace boost::tree_node
//]

#include <boost/tree_node/_detail/config_end.hpp>

#endif  // BOOST_TREE_NODE_NARY_NODE_HPP_INCLUDED

