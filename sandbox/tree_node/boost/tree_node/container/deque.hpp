// Copyright (C) 2013 Cromwell D. Enage
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_NODE_CONTAINER_DEQUE_HPP_INCLUDED
#define BOOST_TREE_NODE_CONTAINER_DEQUE_HPP_INCLUDED

#include <boost/config.hpp>

#if !defined BOOST_NO_CXX11_NULLPTR
#include <cstddef>
#endif

#include <algorithm>
#include <utility>
#include <boost/tr1/type_traits.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/utility/value_init.hpp>

#if !defined BOOST_NO_SFINAE
#include <boost/utility/enable_if.hpp>
#include <boost/detail/metafunction/is_input_iterator.hpp>
#endif

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/move/move.hpp>
#include <boost/container/allocator_traits.hpp>
#endif

#if !defined BOOST_NO_LIMITS
#include <boost/limits.hpp>
#endif

#include <boost/tree_node/preprocessor.hpp>
#include <boost/tree_node/key/data.hpp>
#include <boost/tree_node/key/count.hpp>
#include <boost/tree_node/iterator/breadth_first.hpp>
#include <boost/tree_node/iterator/in_order.hpp>
#include <boost/tree_node/intrinsic/has_key.hpp>
#include <boost/tree_node/intrinsic/at_key.hpp>
#include <boost/tree_node/intrinsic/value_at_key.hpp>
#include <boost/tree_node/algorithm/binary_descendant_at_index.hpp>
#include <boost/tree_node/container/deque_fwd.hpp>
#include <boost/assert.hpp>

namespace boost { namespace tree_node {

    template <typename T, typename NodeGenerator, typename Balancer>
    class deque
    {
        typedef deque<T,NodeGenerator,Balancer> self;

#if !defined BOOST_NO_SFINAE
        struct enabler
        {
        };
#endif

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        BOOST_COPYABLE_AND_MOVABLE(deque)
#endif

     public:
        //[reference__deque__value_type
        typedef T value_type;
        //]

        //[reference__deque__reference
        typedef value_type& reference;
        //]

        //[reference__deque__const_reference
        typedef value_type const& const_reference;
        //]

        //[reference__deque__pointer
        typedef value_type* pointer;
        //]

        //[reference__deque__const_pointer
        typedef value_type const* const_pointer;
        //]

        //[reference__deque__node
        typedef typename ::boost::mpl::apply_wrap1<
                    NodeGenerator
                  , value_type
                >::type
                node;
        //]

        //[reference__deque__allocator_type
        typedef typename node::traits::allocator allocator_type;
        //]

        //[reference__deque__iterator
        typedef ::boost::transform_iterator<
                    at_key<data_key>
                  , in_order_iterator<node>
                >
                iterator;
        //]

        //[reference__deque__const_iterator
        typedef ::boost::transform_iterator<
                    at_key<data_key>
                  , in_order_iterator<node const>
                >
                const_iterator;
        //]

        //[reference__deque__reverse_iterator
        typedef ::boost::transform_iterator<
                    at_key<data_key>
                  , in_order_iterator<node,::boost::mpl::true_>
                >
                reverse_iterator;
        //]

        //[reference__deque__const_reverse_iterator
        typedef ::boost::transform_iterator<
                    at_key<data_key>
                  , in_order_iterator<node const,::boost::mpl::true_>
                >
                const_reverse_iterator;
        //]

        //[reference__deque__size_type
        typedef typename ::boost::mpl::eval_if<
                    result_of::has_key<node,count_key>
                  , result_of::value_at_key<node,count_key>
                  , typename node::size_type
                >::type
                size_type;
        //]

     private:
        allocator_type _allocator;
        typename node::pointer _root_ptr;

     public:
        //[reference__deque__default_fill_ctor
        explicit deque(size_type n = ::boost::initialized_value);
        //]

        //[reference__deque__ctor_w_alloc
        explicit deque(allocator_type const& allocator);
        //]

        //[reference__deque__default_fill_ctor_w_alloc
        deque(size_type n, allocator_type const& allocator);
        //]

        //[reference__deque__fill_ctor
        deque(size_type n, value_type const& t);
        //]

        //[reference__deque__fill_ctor_w_alloc
        deque(
            size_type n
          , value_type const& t
          , allocator_type const& allocator
        );
        //]

#if defined BOOST_NO_SFINAE
        deque(const_iterator itr_begin, const_iterator itr_end);

        deque(
            const_iterator itr_begin
          , const_iterator itr_end
          , allocator_type const& allocator
        );
#else
        //[reference__deque__range_ctor
        template <typename Iterator>
        deque(
            Iterator itr_begin
          , Iterator itr_end
            //<-
          , typename ::boost::enable_if<
                ::boost::detail::metafunction::is_input_iterator<Iterator>
              , enabler
            >::type = enabler()
            //->
        );
        //]

        //[reference__deque__range_ctor_w_alloc
        template <typename Iterator>
        deque(
            Iterator itr_begin
          , Iterator itr_end
          , allocator_type const& allocator
            //<-
          , typename ::boost::enable_if<
                ::boost::detail::metafunction::is_input_iterator<Iterator>
              , enabler
            >::type = enabler()
            //->
        );
        //]
#endif  // BOOST_NO_SFINAE

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        //[reference__deque__copy_ctor
        deque(deque const& copy);
        //]

        //[reference__deque__copy_ctor_w_alloc
        deque(deque const& copy, allocator_type const& allocator);
        //]

        //[reference__deque__copy_assign
        deque& operator=(deque const& copy);
        //]
#else  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        deque(BOOST_COPY_ASSIGN_REF(deque) copy);

        deque(
            BOOST_COPY_ASSIGN_REF(deque) copy
          , allocator_type const& allocator
        );

        deque(BOOST_RV_REF(deque) source);

        deque(
            BOOST_RV_REF(deque) source
          , allocator_type const& allocator
        );

        deque& operator=(BOOST_RV_REF(deque) source);

        deque& operator=(BOOST_COPY_ASSIGN_REF(deque) copy);
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        //[reference__deque__dtor
        ~deque();
        //]

        //[reference__deque__data__const
        typename node::const_pointer data() const;
        //]

        //[reference__deque__data
        typename node::pointer data();
        //]

        //[reference__deque__cbegin
        const_iterator cbegin() const;
        const_iterator begin() const;
        //]

        //[reference__deque__begin
        iterator begin();
        //]

        //[reference__deque__cend
        const_iterator cend() const;
        const_iterator end() const;
        //]

        //[reference__deque__end
        iterator end();
        //]

        //[reference__deque__crbegin
        const_reverse_iterator crbegin() const;
        const_reverse_iterator rbegin() const;
        //]

        //[reference__deque__rbegin
        reverse_iterator rbegin();
        //]

        //[reference__deque__crend
        const_reverse_iterator crend() const;
        const_reverse_iterator rend() const;
        //]

        //[reference__deque__rend
        reverse_iterator rend();
        //]

        //[reference__deque__cback
        const_reference back() const;
        //]

        //[reference__deque__back
        reference back();
        //]

        //[reference__deque__cfront
        const_reference front() const;
        //]

        //[reference__deque__front
        reference front();
        //]

        //[reference__deque__push_back
        void push_back(const_reference t);
        //]

        //[reference__deque__push_front
        void push_front(const_reference t);
        //]

        //[reference__deque__insert
        iterator insert(const_iterator pos, const_reference t);
        //]

        //[reference__deque__insert_fill
        iterator insert(const_iterator pos, size_type n, const_reference t);
        //]

        //[reference__deque__insert_range
        template <typename Iterator>
        iterator
            insert(const_iterator pos, Iterator itr_begin, Iterator itr_end);
        //]

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        void                                                                 \
            emplace_back(                                                    \
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

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        void                                                                 \
            emplace_front(                                                   \
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

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        iterator                                                             \
            emplace(                                                         \
                const_iterator pos                                           \
              , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)   \
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
        void push_back(BOOST_RV_REF(value_type) t);

        void push_front(BOOST_RV_REF(value_type) t);

        iterator insert(const_iterator pos, BOOST_RV_REF(value_type) t);

#if defined BOOST_CONTAINER_PERFECT_FORWARDING
        //[reference__deque__emplace_back
        template <typename ...Args>
        void emplace_back(Args&& ...args);
        //]

        //[reference__deque__emplace_front
        template <typename ...Args>
        void emplace_front(Args&& ...args);
        //]

        //[reference__deque__emplace
        template <typename ...Args>
        iterator emplace(const_iterator pos, Args&& ...args);
        //]
#else  // !defined BOOST_CONTAINER_PERFECT_FORWARDING
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        void                                                                 \
            emplace_back(                                                    \
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

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        void                                                                 \
            emplace_front(                                                   \
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

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        iterator                                                             \
            emplace(                                                         \
                const_iterator pos                                           \
                BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                    \
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

        //[reference__deque__pop_back
        void pop_back();
        //]

        //[reference__deque__pop_front
        void pop_front();
        //]

        //[reference__deque__erase
        iterator erase(const_iterator pos);
        //]

        //[reference__deque__erase_range
        void erase(const_iterator itr_begin, const_iterator itr_end);
        //]

        //[reference__deque__swap_other
        void swap(deque& other);
        //]

        //[reference__deque__empty
        bool empty() const;
        //]

        //[reference__deque__clear
        void clear();
        //]

        //[reference__deque__resize
        void resize(size_type n);
        //]

        //[reference__deque__resize_w_value
        void resize(size_type n, value_type const& t);
        //]

        //[reference__deque__size
        size_type size() const;
        //]

        //[reference__deque__index_operator__const
        const_reference operator[](size_type index) const;
        //]

        //[reference__deque__index_operator
        reference operator[](size_type index);
        //]

#if !defined BOOST_NO_LIMITS
        //[reference__deque__max_size
        static size_type max_size();
        //]
#endif

     private:
        static typename node::pointer
            _construct_from(
                ::std::tr1::true_type
              , allocator_type& allocator
              , value_type const& value
            );

        static typename node::pointer
            _construct_from(
                ::std::tr1::false_type
              , allocator_type& allocator
              , value_type const& value
            );

        static typename node::pointer
            _construct_from(
                ::std::tr1::true_type
              , allocator_type& allocator
              , typename node::pointer p
            );

        static typename node::pointer
            _construct_from(
                ::std::tr1::false_type
              , allocator_type& allocator
              , typename node::pointer p
            );

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        static typename node::pointer                                        \
            _construct_from(                                                 \
                ::std::tr1::true_type                                        \
              , allocator_type& allocator                                    \
              , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)   \
            );                                                               \
//!
        BOOST_PP_FOR(
            (0)
          , BOOST_CONTAINER_GEN_PP_PARAM_PRED
          , BOOST_CONTAINER_GEN_PP_PARAM_INC
          , BOOST_TREE_NODE_EMPLACEMENT_IMPL
        )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                 \
            BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)         \
        BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                          \
        static typename node::pointer                                        \
            _construct_from(                                                 \
                ::std::tr1::false_type                                       \
              , allocator_type& allocator                                    \
              , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)   \
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
        static typename node::pointer
            _construct_from(
                ::std::tr1::true_type
              , allocator_type& allocator
              , Args&& ...args
            );

        template <typename ...Args>
        static typename node::pointer
            _construct_from(
                ::std::tr1::false_type
              , allocator_type& allocator
              , Args&& ...args
            );
#else  // partial template specialization support, imperfect forwarding
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        static typename node::pointer                                        \
            _construct_from(                                                 \
                ::std::tr1::true_type                                        \
              , allocator_type& allocator                                    \
                BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                    \
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

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
        BOOST_PP_EXPR_IF(n, template <)                                      \
            BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                         \
        BOOST_PP_EXPR_IF(n, >)                                               \
        static typename node::pointer                                        \
            _construct_from(                                                 \
                ::std::tr1::false_type                                       \
              , allocator_type& allocator                                    \
                BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                    \
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

        static void
            _fill_construct(
                typename node::pointer const& root_ptr
              , size_type n
            );

        static void
            _fill_construct(
                typename node::pointer const& root_ptr
              , size_type n
              , value_type const& t
            );

        size_type _size(::boost::mpl::true_) const;

        size_type _size(::boost::mpl::false_) const;

        typename node::const_pointer _back() const;

        typename node::pointer _back();
    };

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(allocator_type const& allocator)
      : _allocator(allocator)
#if defined BOOST_NO_CXX11_NULLPTR
      , _root_ptr(0)
#else
      , _root_ptr(nullptr)
#endif
    {
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_construct_from(
            ::std::tr1::true_type
          , allocator_type& allocator
          , value_type const& value
        )
    {
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        typename node::pointer result(allocator.allocate(1));
        allocator.construct(result, value);
#else
        typename node::pointer result(
            ::boost::container::allocator_traits<
                allocator_type
            >::allocate(allocator, 1)
        );
        ::boost::container::allocator_traits<
            allocator_type
        >::construct(allocator, result, value);
#endif
        return result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_construct_from(
            ::std::tr1::false_type
          , allocator_type& allocator
          , value_type const& value
        )
    {
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        typename node::pointer result(allocator.allocate(1));
        allocator.construct(
            result
          , ::boost::container::allocator_arg
          , allocator
          , value
        );
#else
        typename node::pointer result(
            ::boost::container::allocator_traits<
                allocator_type
            >::allocate(allocator, 1)
        );
        ::boost::container::allocator_traits<allocator_type>::construct(
            allocator
          , result
          , ::boost::container::allocator_arg
          , allocator
          , value
        );
#endif
        return result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void
        deque<T,NodeGenerator,Balancer>::_fill_construct(
            typename node::pointer const& root_ptr
          , size_type n
        )
    {
        for (breadth_first_iterator<node> itr(*root_ptr); n && itr; ++itr)
        {
            if (--n)
            {
                itr->emplace_left();

                if (--n)
                {
                    itr->emplace_right();
                }
            }
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void
        deque<T,NodeGenerator,Balancer>::_fill_construct(
            typename node::pointer const& root_ptr
          , size_type n
          , value_type const& t
        )
    {
        for (breadth_first_iterator<node> itr(*root_ptr); n && itr; ++itr)
        {
            if (--n)
            {
                itr->emplace_left(t);

                if (--n)
                {
                    itr->emplace_right(t);
                }
            }
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(size_type n)
      : _allocator()
      , _root_ptr(
            n ? self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
            ) :
#if defined BOOST_NO_CXX11_NULLPTR
            0
#else
            nullptr
#endif
        )
    {
        self::_fill_construct(this->_root_ptr, n);

        if (n)
        {
            Balancer::post_fill(*this->_root_ptr, n);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(
        size_type n
      , allocator_type const& allocator
    ) : _allocator(allocator)
      , _root_ptr(
            n ? self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
            ) :
#if defined BOOST_NO_CXX11_NULLPTR
            0
#else
            nullptr
#endif
        )
    {
        self::_fill_construct(this->_root_ptr, n);

        if (n)
        {
            Balancer::post_fill(*this->_root_ptr, n);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(
        size_type n
      , value_type const& t
    ) : _allocator()
      , _root_ptr(
            n ? self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , t
            ) :
#if defined BOOST_NO_CXX11_NULLPTR
            0
#else
            nullptr
#endif
        )
    {
        self::_fill_construct(this->_root_ptr, n, t);

        if (n)
        {
            Balancer::post_fill(*this->_root_ptr, n);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(
        size_type n
      , value_type const& t
      , allocator_type const& allocator
    ) : _allocator(allocator)
      , _root_ptr(
            n ? self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , t
            ) :
#if defined BOOST_NO_CXX11_NULLPTR
            0
#else
            nullptr
#endif
        )
    {
        self::_fill_construct(this->_root_ptr, n, t);

        if (n)
        {
            Balancer::post_fill(*this->_root_ptr, n);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
#if defined BOOST_NO_SFINAE
    deque<T,NodeGenerator,Balancer>::deque(
        const_iterator itr_begin
      , const_iterator itr_end
    )
#else
    template <typename Iterator>
    deque<T,NodeGenerator,Balancer>::deque(
        Iterator itr_begin
      , Iterator itr_end
      , typename ::boost::enable_if<
            ::boost::detail::metafunction::is_input_iterator<Iterator>
          , enabler
        >::type
    )
#endif
      : _allocator()
      , _root_ptr(
            (itr_begin == itr_end) ?
#if defined BOOST_NO_CXX11_NULLPTR
            0
#else
            nullptr
#endif
            : self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , *itr_begin
            ) 
        )
    {
        size_type const n = static_cast<size_type>(
            ::std::distance(itr_begin, itr_end)
        );

        self::_fill_construct(this->_root_ptr, n, *itr_begin);

        if (n)
        {
            for (in_order_iterator<node> itr(*this->_root_ptr); itr; ++itr)
            {
                put(*itr, data_key(), *itr_begin);
                ++itr_begin;
            }

            Balancer::post_fill(*this->_root_ptr, n);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
#if defined BOOST_NO_SFINAE
    deque<T,NodeGenerator,Balancer>::deque(
        const_iterator itr_begin
      , const_iterator itr_end
      , allocator_type const& allocator
    )
#else
    template <typename Iterator>
    deque<T,NodeGenerator,Balancer>::deque(
        Iterator itr_begin
      , Iterator itr_end
      , allocator_type const& allocator
      , typename ::boost::enable_if<
            ::boost::detail::metafunction::is_input_iterator<Iterator>
          , enabler
        >::type
    )
#endif
      : _allocator(allocator)
      , _root_ptr(
            (itr_begin == itr_end) ?
#if defined BOOST_NO_CXX11_NULLPTR
            0
#else
            nullptr
#endif
            : self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , *itr_begin
            ) 
        )
    {
        size_type const n = static_cast<size_type>(
            ::std::distance(itr_begin, itr_end)
        );

        self::_fill_construct(this->_root_ptr, n, *itr_begin);

        if (n)
        {
            for (in_order_iterator<node> itr(*this->_root_ptr); itr; ++itr)
            {
                put(*itr, data_key(), *itr_begin);
                ++itr_begin;
            }

            Balancer::post_fill(*this->_root_ptr, n);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_construct_from(
            ::std::tr1::true_type
          , allocator_type& allocator
          , typename node::pointer p
        )
    {
        if (p)
        {
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
            typename node::pointer result(allocator.allocate(1));
            allocator.construct(result, *p);
#else
            typename node::pointer result(
                ::boost::container::allocator_traits<
                    allocator_type
                >::allocate(allocator, 1)
            );
            ::boost::container::allocator_traits<
                allocator_type
            >::construct(allocator, result, *p);
#endif
            return result;
        }
        else
        {
            return p;
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_construct_from(
            ::std::tr1::false_type
          , allocator_type& allocator
          , typename node::pointer p
        )
    {
        if (p)
        {
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
            typename node::pointer result(allocator.allocate(1));
            allocator.construct(result, *p, allocator);
#else
            typename node::pointer result(
                ::boost::container::allocator_traits<
                    allocator_type
                >::allocate(allocator, 1)
            );
            ::boost::container::allocator_traits<
                allocator_type
            >::construct(allocator, result, *p, allocator);
#endif
            return result;
        }
        else
        {
            return p;
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        deque const& copy
#else
        BOOST_COPY_ASSIGN_REF(deque) copy
#endif
    ) : _allocator(copy._allocator)
      , _root_ptr(
            self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , copy._root_ptr
            )
        )
    {
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        deque const& copy
#else
        BOOST_COPY_ASSIGN_REF(deque) copy
#endif
      , allocator_type const& allocator
    ) : _allocator(copy._allocator)
      , _root_ptr(
            self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , copy._root_ptr
            )
        )
    {
    }

#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(BOOST_RV_REF(deque) source)
      : _allocator(::boost::move(source._allocator))
      , _root_ptr(source._root_ptr)
    {
#if defined BOOST_NO_CXX11_NULLPTR
        source._root_ptr = 0;
#else
        source._root_ptr = nullptr;
#endif
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::deque(
        BOOST_RV_REF(deque) source
      , allocator_type const& allocator
    ) : _allocator(allocator), _root_ptr(source._root_ptr)
    {
#if defined BOOST_NO_CXX11_NULLPTR
        source._root_ptr = 0;
#else
        source._root_ptr = nullptr;
#endif
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline deque<T,NodeGenerator,Balancer>&
        deque<T,NodeGenerator,Balancer>::operator=(BOOST_RV_REF(deque) source)
    {
        if (this != &static_cast<deque&>(source))
        {
            this->_allocator = ::boost::move(source._allocator);
            this->clear();
            this->_root_ptr = source._root_ptr;
#if defined BOOST_NO_CXX11_NULLPTR
            source._root_ptr = 0;
#else
            source._root_ptr = nullptr;
#endif
        }

        return *this;
    }
#endif  // !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template <typename T, typename NodeGenerator, typename Balancer>
    inline deque<T,NodeGenerator,Balancer>&
        deque<T,NodeGenerator,Balancer>::operator=(
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
            deque const& copy
#else
            BOOST_COPY_ASSIGN_REF(deque) copy
#endif
        )
    {
        if (this != &copy)
        {
            if (copy._root_ptr)
            {
                if (this->_root_ptr)
                {
                    *this->_root_ptr = *copy._root_ptr;
                }
                else
                {
                    this->_root_ptr = self::_construct_from(
                        ::std::tr1::is_const<
                            typename ::std::tr1::remove_reference<
                                typename node::traits::allocator_reference
                            >::type
                        >()
                      , this->_allocator
                      , copy._root_ptr
                    );
                }
            }
            else
            {
                this->clear();
            }
        }

        return *this;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    deque<T,NodeGenerator,Balancer>::~deque()
    {
        this->clear();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::node::const_pointer
        deque<T,NodeGenerator,Balancer>::data() const
    {
        return this->_root_ptr;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::data()
    {
        return this->_root_ptr;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_iterator
        deque<T,NodeGenerator,Balancer>::cbegin() const
    {
        return this->_root_ptr ? const_iterator(
            make_in_order_iterator(*this->_root_ptr)
          , at_key<data_key>()
        ) : this->cend();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_iterator
        deque<T,NodeGenerator,Balancer>::begin() const
    {
        return this->_root_ptr ? const_iterator(
            make_in_order_iterator(*this->_root_ptr)
          , at_key<data_key>()
        ) : this->end();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::begin()
    {
        return this->_root_ptr ? iterator(
            make_in_order_iterator(*this->_root_ptr)
          , at_key<data_key>()
        ) : this->end();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_iterator
        deque<T,NodeGenerator,Balancer>::cend() const
    {
        return const_iterator(
            make_in_order_iterator_end(this->_root_ptr)
          , at_key<data_key>()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_iterator
        deque<T,NodeGenerator,Balancer>::end() const
    {
        return const_iterator(
            make_in_order_iterator_end(this->_root_ptr)
          , at_key<data_key>()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::end()
    {
        return iterator(
            make_in_order_iterator_end(this->_root_ptr)
          , at_key<data_key>()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reverse_iterator
        deque<T,NodeGenerator,Balancer>::crbegin() const
    {
        return this->_root_ptr ? const_reverse_iterator(
            make_in_order_reverse_iterator(*this->_root_ptr)
          , at_key<data_key>()
        ) : this->crend();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reverse_iterator
        deque<T,NodeGenerator,Balancer>::rbegin() const
    {
        return this->_root_ptr ? const_reverse_iterator(
            make_in_order_reverse_iterator(*this->_root_ptr)
          , at_key<data_key>()
        ) : this->rend();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::reverse_iterator
        deque<T,NodeGenerator,Balancer>::rbegin()
    {
        return this->_root_ptr ? reverse_iterator(
            make_in_order_reverse_iterator(*this->_root_ptr)
          , at_key<data_key>()
        ) : this->rend();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reverse_iterator
        deque<T,NodeGenerator,Balancer>::crend() const
    {
        return const_reverse_iterator(
            make_in_order_reverse_iterator_end(this->_root_ptr)
          , at_key<data_key>()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reverse_iterator
        deque<T,NodeGenerator,Balancer>::rend() const
    {
        return const_reverse_iterator(
            make_in_order_reverse_iterator_end(this->_root_ptr)
          , at_key<data_key>()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::reverse_iterator
        deque<T,NodeGenerator,Balancer>::rend()
    {
        return reverse_iterator(
            make_in_order_reverse_iterator_end(this->_root_ptr)
          , at_key<data_key>()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reference
        deque<T,NodeGenerator,Balancer>::front() const
    {
        return *this->cbegin();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::reference
        deque<T,NodeGenerator,Balancer>::front()
    {
        return *this->begin();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reference
        deque<T,NodeGenerator,Balancer>::back() const
    {
        return *this->crbegin();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::reference
        deque<T,NodeGenerator,Balancer>::back()
    {
        return *this->rbegin();
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline void deque<T,NodeGenerator,Balancer>::pop_front()
    {
        this->erase(this->cbegin());
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::node::const_pointer
        deque<T,NodeGenerator,Balancer>::_back() const
    {
        typename node::const_pointer result = this->_root_ptr;

        if (result)
        {
            while (result->get_right_child_ptr())
            {
                result = result->get_right_child_ptr();
            }
        }

        return result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_back()
    {
        typename node::pointer result = this->_root_ptr;

        if (result)
        {
            while (result->get_right_child_ptr())
            {
                result = result->get_right_child_ptr();
            }
        }

        return result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline void deque<T,NodeGenerator,Balancer>::pop_back()
    {
        this->erase(
            iterator(
                make_in_order_iterator_position(*this->_back())
              , at_key<data_key>()
            )
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void deque<T,NodeGenerator,Balancer>::push_front(const_reference t)
    {
        if (this->_root_ptr)
        {
            typename node::pointer p = this->_root_ptr;

            while (p->get_left_child_ptr())
            {
                p = p->get_left_child_ptr();
            }

            p = Balancer::post_insert(p = &*p->emplace_left(t));

            if (!p->get_parent_ptr())
            {
                this->_root_ptr = p;
            }
        }
        else  // if (!this->_root_ptr)
        {
            this->_root_ptr = self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , t
            );
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void deque<T,NodeGenerator,Balancer>::push_back(const_reference t)
    {
        if (this->_root_ptr)
        {
            typename node::pointer p = this->_root_ptr;

            while (p->get_right_child_ptr())
            {
                p = p->get_right_child_ptr();
            }

            p = Balancer::post_insert(p = &*p->emplace_right(t));

            if (!p->get_parent_ptr())
            {
                this->_root_ptr = p;
            }
        }
        else  // if (!this->_root_ptr)
        {
            this->_root_ptr = self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , t
            );
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::insert(
            const_iterator pos
          , const_reference t
        )
    {
        if (pos.base())
        {
            typename node::pointer anc_ptr = const_cast<
                typename node::pointer
            >(&*pos.base());
            typename node::pointer node_ptr = anc_ptr->get_left_child_ptr();

            if (node_ptr)
            {
                while (node_ptr->get_right_child_ptr())
                {
                    node_ptr = node_ptr->get_right_child_ptr();
                }

                node_ptr = &*node_ptr->emplace_right(t);
            }
            else
            {
                node_ptr = &*anc_ptr->emplace_left(t);
            }

            anc_ptr = Balancer::post_insert(node_ptr);

            if (!anc_ptr->get_parent_ptr())
            {
                this->_root_ptr = anc_ptr;
            }

            return iterator(
                make_in_order_iterator_position(*node_ptr)
              , at_key<data_key>()
            );
        }
        else  // if (!pos.base())
        {
            this->push_back(t);
            return --this->end();
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::insert(
            const_iterator pos
          , size_type n
          , value_type const& t
        )
    {
        if (n)
        {
            iterator result = this->insert(pos, t);

            for (iterator pos_itr; --n; pos = ++pos_itr)
            {
                pos_itr = this->insert(pos, t);
            }

            return result;
        }
        else  // if (!n)
        {
            if (pos.base())
            {
                iterator result(
                    make_in_order_iterator_position(
                        const_cast<node&>(*pos.base())
                    )
                  , at_key<data_key>()
                );
                BOOST_ASSERT(result == pos);
                return result;
            }
            else
            {
                BOOST_ASSERT(this->end() == pos);
                return this->end();
            }
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    template <typename Iterator>
    typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::insert(
            const_iterator pos
          , Iterator itr_begin
          , Iterator itr_end
        )
    {
        if (itr_begin == itr_end)
        {
            if (pos.base())
            {
                iterator result(
                    make_in_order_iterator_position(
                        const_cast<node&>(*pos.base())
                    )
                  , at_key<data_key>()
                );
                BOOST_ASSERT(result == pos);
                return result;
            }
            else
            {
                BOOST_ASSERT(this->end() == pos);
                return this->end();
            }
        }
        else
        {
            iterator result = this->insert(pos, *itr_begin);

            for (iterator pos_itr; ++itr_begin != itr_end; pos = ++pos_itr)
            {
                pos_itr = this->insert(pos, *itr_begin);
            }

            return result;
        }
    }

#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(r, seq)                             \
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    typename deque<T,NodeGenerator,Balancer>::node::pointer                  \
        deque<T,NodeGenerator,Balancer>::_construct_from(                    \
            ::std::tr1::true_type                                            \
          , allocator_type& allocator                                        \
          , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        typename node::pointer result(allocator.allocate(1));                \
        allocator.construct(                                                 \
            result                                                           \
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)         \
        );                                                                   \
        return result;                                                       \
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
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    typename deque<T,NodeGenerator,Balancer>::node::pointer                  \
        deque<T,NodeGenerator,Balancer>::_construct_from(                    \
            ::std::tr1::false_type                                           \
          , allocator_type& allocator                                        \
          , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        typename node::pointer result(allocator.allocate(1));                \
        allocator.construct(                                                 \
            result                                                           \
          , ::boost::container::allocator_arg                                \
          , allocator                                                        \
            BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)         \
        );                                                                   \
        return result;                                                       \
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
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    void                                                                     \
        deque<T,NodeGenerator,Balancer>::emplace_front(                      \
            BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        if (this->_root_ptr)                                                 \
        {                                                                    \
            typename node::pointer p = this->_root_ptr;                      \
            while (p->get_left_child_ptr())                                  \
            {                                                                \
                p = p->get_left_child_ptr();                                 \
            }                                                                \
            p = Balancer::post_insert(                                       \
                p = &*p->emplace_left(                                       \
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)          \
                )                                                            \
            );                                                               \
            if (!p->get_parent_ptr())                                        \
            {                                                                \
                this->_root_ptr = p;                                         \
            }                                                                \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            this->_root_ptr = self::_construct_from(                         \
                ::std::tr1::is_const<                                        \
                    typename ::std::tr1::remove_reference<                   \
                        typename node::traits::allocator_reference           \
                    >::type                                                  \
                >()                                                          \
              , this->_allocator                                             \
                BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)     \
            );                                                               \
        }                                                                    \
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
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    void                                                                     \
        deque<T,NodeGenerator,Balancer>::emplace_back(                       \
            BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        if (this->_root_ptr)                                                 \
        {                                                                    \
            typename node::pointer p = this->_root_ptr;                      \
            while (p->get_right_child_ptr())                                 \
            {                                                                \
                p = p->get_right_child_ptr();                                \
            }                                                                \
            p = Balancer::post_insert(                                       \
                p = &*p->emplace_right(                                      \
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)          \
                )                                                            \
            );                                                               \
            if (!p->get_parent_ptr())                                        \
            {                                                                \
                this->_root_ptr = p;                                         \
            }                                                                \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            this->_root_ptr = self::_construct_from(                         \
                ::std::tr1::is_const<                                        \
                    typename ::std::tr1::remove_reference<                   \
                        typename node::traits::allocator_reference           \
                    >::type                                                  \
                >()                                                          \
              , this->_allocator                                             \
                BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)     \
            );                                                               \
        }                                                                    \
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
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), template <)                     \
        BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), typename P)             \
    BOOST_PP_EXPR_IF(BOOST_PP_SEQ_SIZE(seq), >)                              \
    typename deque<T,NodeGenerator,Balancer>::iterator                       \
        deque<T,NodeGenerator,Balancer>::emplace(                            \
            const_iterator pos                                               \
          , BOOST_DETAIL_PP_BINARY_SEQ_TO_PARAMS(seq, P, &, const&, p)       \
        )                                                                    \
    {                                                                        \
        if (pos.base())                                                      \
        {                                                                    \
            typename node::pointer anc_ptr = const_cast<                     \
                typename node::pointer                                       \
            >(&*pos.base());                                                 \
            typename node::pointer node_ptr = (                              \
                anc_ptr->get_left_child_ptr()                                \
            );                                                               \
            if (node_ptr)                                                    \
            {                                                                \
                while (node_ptr->get_right_child_ptr())                      \
                {                                                            \
                    node_ptr = node_ptr->get_right_child_ptr();              \
                }                                                            \
                node_ptr = &*node_ptr->emplace_right(                        \
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)          \
                );                                                           \
            }                                                                \
            else                                                             \
            {                                                                \
                node_ptr = &*anc_ptr->emplace_left(                          \
                    BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)          \
                );                                                           \
            }                                                                \
            anc_ptr = Balancer::post_insert(node_ptr);                       \
            if (!anc_ptr->get_parent_ptr())                                  \
            {                                                                \
                this->_root_ptr = anc_ptr;                                   \
            }                                                                \
            return iterator(                                                 \
                make_in_order_iterator_position(*node_ptr)                   \
              , at_key<data_key>()                                           \
            );                                                               \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            this->emplace_back(                                              \
                BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), p)              \
            );                                                               \
            return this->end();                                              \
        }                                                                    \
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
    template <typename T, typename NodeGenerator, typename Balancer>
    void
        deque<T,NodeGenerator,Balancer>::push_front(BOOST_RV_REF(value_type) t)
    {
        if (this->_root_ptr)
        {
            typename node::pointer p = this->_root_ptr;

            while (p->get_left_child_ptr())
            {
                p = p->get_left_child_ptr();
            }

#if defined BOOST_NO_RVALUE_REFERENCES
            p = Balancer::post_insert(p = &*p->emplace_left(t));
#else
            p = Balancer::post_insert(
                p = &*p->emplace_left(static_cast<value_type&&>(t))
            );
#endif

            if (!p->get_parent_ptr())
            {
                this->_root_ptr = p;
            }
        }
        else  // if (!this->_root_ptr)
        {
            this->_root_ptr = self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
#if defined BOOST_NO_RVALUE_REFERENCES
              , t
#else
              , static_cast<value_type&&>(t)
#endif
            );
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void deque<T,NodeGenerator,Balancer>::push_back(BOOST_RV_REF(value_type) t)
    {
        if (this->_root_ptr)
        {
            typename node::pointer p = this->_root_ptr;

            while (p->get_right_child_ptr())
            {
                p = p->get_right_child_ptr();
            }

#if defined BOOST_NO_RVALUE_REFERENCES
            p = Balancer::post_insert(p = &*p->emplace_right(t));
#else
            p = Balancer::post_insert(
                p = &*p->emplace_right(static_cast<value_type&&>(t))
            );
#endif

            if (!p->get_parent_ptr())
            {
                this->_root_ptr = p;
            }
        }
        else  // if (!this->_root_ptr)
        {
            this->_root_ptr = self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
#if defined BOOST_NO_RVALUE_REFERENCES
              , t
#else
              , static_cast<value_type&&>(t)
#endif
            );
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::insert(
            const_iterator pos
          , BOOST_RV_REF(value_type) t
        )
    {
        if (pos.base())
        {
            typename node::pointer anc_ptr = const_cast<
                typename node::pointer
            >(&*pos.base());
            typename node::pointer node_ptr = anc_ptr->get_left_child_ptr();

            if (node_ptr)
            {
                while (node_ptr->get_right_child_ptr())
                {
                    node_ptr = node_ptr->get_right_child_ptr();
                }

#if defined BOOST_NO_RVALUE_REFERENCES
                node_ptr = &*node_ptr->emplace_right(t);
#else
                node_ptr = &*node_ptr->emplace_right(
                    static_cast<value_type&&>(t)
                );
#endif
            }
            else
            {
#if defined BOOST_NO_RVALUE_REFERENCES
                node_ptr = &*node_ptr->emplace_left(t);
#else
                node_ptr = &*node_ptr->emplace_left(
                    static_cast<value_type&&>(t)
                );
#endif
            }

            anc_ptr = Balancer::post_insert(node_ptr);

            if (!anc_ptr->get_parent_ptr())
            {
                this->_root_ptr = anc_ptr;
            }

            return iterator(
                make_in_order_iterator_position(*node_ptr)
              , at_key<data_key>()
            );
        }
        else  // if (!pos.base())
        {
#if defined BOOST_NO_RVALUE_REFERENCES
            this->push_back(t);
#else
            this->push_back(static_cast<value_type&&>(t));
#endif
            return this->end();
        }
    }

#if defined BOOST_CONTAINER_PERFECT_FORWARDING
    template <typename T, typename NodeGenerator, typename Balancer>
    template <typename ...Args>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_construct_from(
            ::std::tr1::true_type
          , allocator_type& allocator
          , Args&& ...args
        )
    {
        typename node::pointer result(
            ::boost::container::allocator_traits<
                allocator_type
            >::allocate(allocator, 1)
        );
        ::boost::container::allocator_traits<
            allocator_type
        >::construct(allocator, result, ::boost::forward<Args>(args)...);
        return result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    template <typename ...Args>
    typename deque<T,NodeGenerator,Balancer>::node::pointer
        deque<T,NodeGenerator,Balancer>::_construct_from(
            ::std::tr1::false_type
          , allocator_type& allocator
          , Args&& ...args
        )
    {
        typename node::pointer result(
            ::boost::container::allocator_traits<
                allocator_type
            >::allocate(allocator, 1)
        );
        ::boost::container::allocator_traits<allocator_type>::construct(
            allocator
          , result
          , ::boost::container::allocator_arg
          , allocator
          , ::boost::forward<Args>(args)...
        );
        return result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    template <typename ...Args>
    void deque<T,NodeGenerator,Balancer>::emplace_front(Args&& ...args)
    {
        if (this->_root_ptr)
        {
            typename node::pointer p = this->_root_ptr;

            while (p->get_left_child_ptr())
            {
                p = p->get_left_child_ptr();
            }

            p = Balancer::post_insert(
                p = &*p->emplace_left(::boost::forward<Args>(args)...)
            );

            if (!p->get_parent_ptr())
            {
                this->_root_ptr = p;
            }
        }
        else  // if (!this->_root_ptr)
        {
            this->_root_ptr = self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , ::boost::forward<Args>(args)...
            );
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    template <typename ...Args>
    void deque<T,NodeGenerator,Balancer>::emplace_back(Args&& ...args)
    {
        if (this->_root_ptr)
        {
            typename node::pointer p = this->_root_ptr;

            while (p->get_right_child_ptr())
            {
                p = p->get_right_child_ptr();
            }

            p = Balancer::post_insert(
                p = &*p->emplace_right(::boost::forward<Args>(args)...)
            );

            if (!p->get_parent_ptr())
            {
                this->_root_ptr = p;
            }
        }
        else  // if (!this->_root_ptr)
        {
            this->_root_ptr = self::_construct_from(
                ::std::tr1::is_const<
                    typename ::std::tr1::remove_reference<
                        typename node::traits::allocator_reference
                    >::type
                >()
              , this->_allocator
              , ::boost::forward<Args>(args)...
            );
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    template <typename ...Args>
    typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::emplace(
            const_iterator pos
          , Args&& ...args
        )
    {
        if (pos.base())
        {
            typename node::pointer anc_ptr = const_cast<
                typename node::pointer
            >(&*pos.base());
            typename node::pointer node_ptr = anc_ptr->get_left_child_ptr();

            if (node_ptr)
            {
                while (node_ptr->get_right_child_ptr())
                {
                    node_ptr = node_ptr->get_right_child_ptr();
                }

                node_ptr = &*node_ptr->emplace_right(
                    ::boost::forward<Args>(args)...
                );
            }
            else
            {
                node_ptr = &*anc_ptr->emplace_left(
                    ::boost::forward<Args>(args)...
                );
            }

            anc_ptr = Balancer::post_insert(node_ptr);

            if (!anc_ptr->get_parent_ptr())
            {
                this->_root_ptr = anc_ptr;
            }

            return iterator(
                make_in_order_iterator_position(*node_ptr)
              , at_key<data_key>()
            );
        }
        else  // if (!pos.base())
        {
            this->emplace_back(::boost::forward<Args>(args)...);
            return this->end();
        }
    }
#else  // !defined BOOST_CONTAINER_PERFECT_FORWARDING
#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    typename deque<T,NodeGenerator,Balancer>::node::pointer                  \
        deque<T,NodeGenerator,Balancer>::_construct_from(                    \
            ::std::tr1::true_type                                            \
          , allocator_type& allocator                                        \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        typename node::pointer result(                                       \
            ::boost::container::allocator_traits<                            \
                allocator_type                                               \
            >::allocate(allocator, 1)                                        \
        );                                                                   \
        ::boost::container::allocator_traits<allocator_type>::construct(     \
            allocator                                                        \
          , result                                                           \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_FORWARD                             \
              , _                                                            \
            )                                                                \
        );                                                                   \
        return result;                                                       \
     }                                                                       \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    typename deque<T,NodeGenerator,Balancer>::node::pointer                  \
        deque<T,NodeGenerator,Balancer>::_construct_from(                    \
            ::std::tr1::false_type                                           \
          , allocator_type& allocator                                        \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        typename node::pointer result(                                       \
            ::boost::container::allocator_traits<                            \
                allocator_type                                               \
            >::allocate(allocator, 1)                                        \
        );                                                                   \
        ::boost::container::allocator_traits<allocator_type>::construct(     \
            allocator                                                        \
          , result                                                           \
          , ::boost::container::allocator_arg                                \
          , allocator                                                        \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_FORWARD                             \
              , _                                                            \
            )                                                                \
        );                                                                   \
        return result;                                                       \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    void                                                                     \
        deque<T,NodeGenerator,Balancer>::emplace_front(                      \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        if (this->_root_ptr)                                                 \
        {                                                                    \
            typename node::pointer p = this->_root_ptr;                      \
            while (p->get_left_child_ptr())                                  \
            {                                                                \
                p = p->get_left_child_ptr();                                 \
            }                                                                \
            p = Balancer::post_insert(                                       \
                p = &*p->emplace_left(                                       \
                    BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                         \
                        n                                                    \
                      , BOOST_CONTAINER_PP_PARAM_FORWARD                     \
                      , _                                                    \
                    )                                                        \
                )                                                            \
            );                                                               \
            if (!p->get_parent_ptr())                                        \
            {                                                                \
                this->_root_ptr = p;                                         \
            }                                                                \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            this->_root_ptr = self::_construct_from(                         \
                ::std::tr1::is_const<                                        \
                    typename ::std::tr1::remove_reference<                   \
                        typename node::traits::allocator_reference           \
                    >::type                                                  \
                >()                                                          \
              , this->_allocator                                             \
                BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                    \
                    n                                                        \
                  , BOOST_CONTAINER_PP_PARAM_FORWARD                         \
                  , _                                                        \
                )                                                            \
            );                                                               \
        }                                                                    \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    void                                                                     \
        deque<T,NodeGenerator,Balancer>::emplace_back(                       \
            BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                                 \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        if (this->_root_ptr)                                                 \
        {                                                                    \
            typename node::pointer p = this->_root_ptr;                      \
            while (p->get_right_child_ptr())                                 \
            {                                                                \
                p = p->get_right_child_ptr();                                \
            }                                                                \
            p = Balancer::post_insert(                                       \
                p = &*p->emplace_right(                                      \
                    BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                         \
                        n                                                    \
                      , BOOST_CONTAINER_PP_PARAM_FORWARD                     \
                      , _                                                    \
                    )                                                        \
                )                                                            \
            );                                                               \
            if (!p->get_parent_ptr())                                        \
            {                                                                \
                this->_root_ptr = p;                                         \
            }                                                                \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            this->_root_ptr = self::_construct_from(                         \
                ::std::tr1::is_const<                                        \
                    typename ::std::tr1::remove_reference<                   \
                        typename node::traits::allocator_reference           \
                    >::type                                                  \
                >()                                                          \
              , this->_allocator                                             \
                BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                    \
                    n                                                        \
                  , BOOST_CONTAINER_PP_PARAM_FORWARD                         \
                  , _                                                        \
                )                                                            \
            );                                                               \
        }                                                                    \
    }                                                                        \
//!
    BOOST_PP_REPEAT(
        BOOST_CONTAINER_MAX_CONSTRUCTOR_PARAMETERS
      , BOOST_TREE_NODE_EMPLACEMENT_IMPL
      , _
    )
#undef BOOST_TREE_NODE_EMPLACEMENT_IMPL

#define BOOST_TREE_NODE_EMPLACEMENT_IMPL(z, n, _)                            \
    template <typename T, typename NodeGenerator, typename Balancer>         \
    BOOST_PP_EXPR_IF(n, template <)                                          \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename P)                             \
    BOOST_PP_EXPR_IF(n, >)                                                   \
    typename deque<T,NodeGenerator,Balancer>::iterator                       \
        deque<T,NodeGenerator,Balancer>::emplace(                            \
            const_iterator pos                                               \
            BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                        \
                n                                                            \
              , BOOST_CONTAINER_PP_PARAM_LIST                                \
              , _                                                            \
            )                                                                \
        )                                                                    \
    {                                                                        \
        if (pos.base())                                                      \
        {                                                                    \
            typename node::pointer anc_ptr = const_cast<                     \
                typename node::pointer                                       \
            >(&*pos.base());                                                 \
            typename node::pointer node_ptr = (                              \
                anc_ptr->get_left_child_ptr()                                \
            );                                                               \
            if (node_ptr)                                                    \
            {                                                                \
                while (node_ptr->get_right_child_ptr())                      \
                {                                                            \
                    node_ptr = node_ptr->get_right_child_ptr();              \
                }                                                            \
                node_ptr = &*node_ptr->emplace_right(                        \
                    BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                         \
                        n                                                    \
                      , BOOST_CONTAINER_PP_PARAM_FORWARD                     \
                      , _                                                    \
                    )                                                        \
                );                                                           \
            }                                                                \
            else                                                             \
            {                                                                \
                node_ptr = &*anc_ptr->emplace_left(                          \
                    BOOST_PP_CAT(BOOST_PP_ENUM_, z)(                         \
                        n                                                    \
                      , BOOST_CONTAINER_PP_PARAM_FORWARD                     \
                      , _                                                    \
                    )                                                        \
                );                                                           \
            }                                                                \
            anc_ptr = Balancer::post_insert(node_ptr);                       \
            if (!anc_ptr->get_parent_ptr())                                  \
            {                                                                \
                this->_root_ptr = anc_ptr;                                   \
            }                                                                \
            return iterator(                                                 \
                make_in_order_iterator_position(*node_ptr)                   \
              , at_key<data_key>()                                           \
            );                                                               \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            this->emplace_back(                                              \
                BOOST_PP_CAT(BOOST_PP_ENUM_TRAILING_, z)(                    \
                    n                                                        \
                  , BOOST_CONTAINER_PP_PARAM_FORWARD                         \
                  , _                                                        \
                )                                                            \
            );                                                               \
            return this->end();                                              \
        }                                                                    \
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

    template <typename T, typename NodeGenerator, typename Balancer>
    typename deque<T,NodeGenerator,Balancer>::iterator
        deque<T,NodeGenerator,Balancer>::erase(const_iterator pos)
    {
        if (pos.base()->empty() && (this->_root_ptr == &*pos.base()))
        {
            this->clear();
            return this->end();
        }

        typename node::pointer anc_ptr = const_cast<typename node::pointer>(
            &*pos.base()
        );
        iterator result(
            make_in_order_iterator_position(*anc_ptr)
          , at_key<data_key>()
        );
        bool must_erase_begin = (result == this->begin());

        if (!must_erase_begin)
        {
            --result;
        }

        for (typename node::pointer desc_ptr;;)
        {
            if (
                (desc_ptr = anc_ptr->get_left_child_ptr()) && (
                    !anc_ptr->get_right_child_ptr()
                )
            )
            {
                while (desc_ptr->get_right_child_ptr())
                {
                    desc_ptr = desc_ptr->get_right_child_ptr();
                }

                if (desc_ptr->get_parent_ptr() == anc_ptr)
                {
                    if (!anc_ptr->get_right_child_ptr())
                    {
                        put(*anc_ptr, data_key(), get(*desc_ptr, data_key()));

                        bool must_rebalance = Balancer::pre_erase(*desc_ptr);

                        anc_ptr->erase_left();

                        if (must_rebalance)
                        {
                            anc_ptr = Balancer::post_erase_left(anc_ptr);

                            if (!anc_ptr->get_parent_ptr())
                            {
                                this->_root_ptr = anc_ptr;
                            }
                        }

                        break;
                    }
                }
                else  // if (desc_ptr == anc_ptr->get_right_child_ptr())
                {
                    put(*anc_ptr, data_key(), get(*desc_ptr, data_key()));

                    if (desc_ptr->get_left_child_ptr())
                    {
                        anc_ptr = desc_ptr;
                    }
                    else  // if (desc_ptr->empty())
                    {
                        anc_ptr = desc_ptr->get_parent_ptr();

                        if (anc_ptr->get_left_child_ptr())
                        {
                            put(*desc_ptr, data_key(), get(*anc_ptr, data_key()));
                        }
                        else  // desc_ptr is only child of anc_ptr
                        {
                            bool must_rebalance = Balancer::pre_erase(*desc_ptr);

                            anc_ptr->erase_right();

                            if (must_rebalance)
                            {
                                anc_ptr = Balancer::post_erase_right(anc_ptr);

                                if (!anc_ptr->get_parent_ptr())
                                {
                                    this->_root_ptr = anc_ptr;
                                }
                            }

                            break;
                        }
                    }

                    continue;
                }
            }

            if ((desc_ptr = anc_ptr->get_right_child_ptr()))
            {
                while (desc_ptr->get_left_child_ptr())
                {
                    desc_ptr = desc_ptr->get_left_child_ptr();
                }

                put(*anc_ptr, data_key(), get(*desc_ptr, data_key()));

                if (desc_ptr->get_right_child_ptr())
                {
                    if (desc_ptr->get_right_child_ptr()->empty())
                    {
                        bool must_rebalance = Balancer::pre_erase(*desc_ptr);

                        anc_ptr->erase_right();

                        if (must_rebalance)
                        {
                            anc_ptr = Balancer::post_erase_right(anc_ptr);

                            if (!anc_ptr->get_parent_ptr())
                            {
                                this->_root_ptr = anc_ptr;
                            }
                        }
                    }
                    else
                    {
                        anc_ptr = desc_ptr;
                    }
                }
                else if (desc_ptr->get_parent_ptr() == anc_ptr)
                {
                    bool must_rebalance = Balancer::pre_erase(*desc_ptr);

                    anc_ptr->erase_right();

                    if (must_rebalance)
                    {
                        anc_ptr = Balancer::post_erase_right(anc_ptr);

                        if (!anc_ptr->get_parent_ptr())
                        {
                            this->_root_ptr = anc_ptr;
                        }
                    }

                    break;
                }
                else
                {
                    BOOST_ASSERT(desc_ptr->empty());
                    anc_ptr = desc_ptr->get_parent_ptr();
                    BOOST_ASSERT(anc_ptr->get_left_child_ptr() == desc_ptr);

                    if (anc_ptr->get_right_child_ptr())
                    {
                        put(*desc_ptr, data_key(), get(*anc_ptr, data_key()));
                    }
                    else  // desc_ptr is only child of anc_ptr
                    {
                        bool must_rebalance = Balancer::pre_erase(*desc_ptr);

                        anc_ptr->erase_left();

                        if (must_rebalance)
                        {
                            anc_ptr = Balancer::post_erase_left(anc_ptr);

                            if (!anc_ptr->get_parent_ptr())
                            {
                                this->_root_ptr = anc_ptr;
                            }
                        }

                        break;
                    }
                }
            }
            else  // if (anc_ptr->empty())
            {
                desc_ptr = anc_ptr;
                anc_ptr = anc_ptr->get_parent_ptr();

                bool must_rebalance = Balancer::pre_erase(*desc_ptr);

                if (anc_ptr->get_left_child_ptr() == desc_ptr)
                {
                    anc_ptr->erase_left();

                    if (must_rebalance)
                    {
                        anc_ptr = Balancer::post_erase_left(anc_ptr);

                        if (!anc_ptr->get_parent_ptr())
                        {
                            this->_root_ptr = anc_ptr;
                        }
                    }
                }
                else  // if (anc_ptr->get_right_child_ptr() == desc_ptr)
                {
                    anc_ptr->erase_right();

                    if (must_rebalance)
                    {
                        anc_ptr = Balancer::post_erase_right(anc_ptr);

                        if (!anc_ptr->get_parent_ptr())
                        {
                            this->_root_ptr = anc_ptr;
                        }
                    }
                }

                break;
            }
        }

        return must_erase_begin ? this->begin() : ++result;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void
        deque<T,NodeGenerator,Balancer>::erase(
            const_iterator itr_begin
          , const_iterator itr_end
        )
    {
        while (itr_begin != itr_end)
        {
            itr_begin = this->erase(itr_begin);
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline bool deque<T,NodeGenerator,Balancer>::empty() const
    {
        return !this->_root_ptr;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline void deque<T,NodeGenerator,Balancer>::swap(deque& other)
    {
        typename node::pointer p = this->_root_ptr;

        this->_root_ptr = other._root_ptr;
        other._root_ptr = p;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void deque<T,NodeGenerator,Balancer>::clear()
    {
        if (this->_root_ptr)
        {
#if defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
            this->_allocator.destroy(this->_root_ptr);
            this->_allocator.deallocate(this->_root_ptr, 1);
#else
            ::boost::container::allocator_traits<
                allocator_type
            >::destroy(this->_allocator, this->_root_ptr);
            ::boost::container::allocator_traits<
                allocator_type
            >::deallocate(this->_allocator, this->_root_ptr, 1);
#endif

#if defined BOOST_NO_CXX11_NULLPTR
            this->_root_ptr = 0;
#else
            this->_root_ptr = nullptr;
#endif
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void deque<T,NodeGenerator,Balancer>::resize(size_type n)
    {
        if (this->size() < n)
        {
            while (this->size() < n)
            {
                this->emplace_back();
            }
        }
        else if (n < this->size())
        {
            while (n < this->size())
            {
                this->pop_back();
            }
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    void
        deque<T,NodeGenerator,Balancer>::resize(
            size_type n
          , value_type const& t
        )
    {
        if (this->size() < n)
        {
            while (this->size() < n)
            {
                this->push_back(t);
            }
        }
        else if (n < this->size())
        {
            while (n < this->size())
            {
                this->pop_back();
            }
        }
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::size_type
        deque<T,NodeGenerator,Balancer>::_size(::boost::mpl::true_) const
    {
        return this->_root_ptr ? get(*this->_root_ptr, count_key()) : 0;
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::size_type
        deque<T,NodeGenerator,Balancer>::_size(::boost::mpl::false_) const
    {
        return ::std::distance(this->cbegin(), this->cend());
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::size_type
        deque<T,NodeGenerator,Balancer>::size() const
    {
        return this->_size(result_of::has_key<node,count_key>());
    }

#if !defined BOOST_NO_LIMITS
    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::size_type
        deque<T,NodeGenerator,Balancer>::max_size()
    {
        return (::std::numeric_limits<size_type>::max)();
    }
#endif

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::const_reference
        deque<T,NodeGenerator,Balancer>::operator[](size_type index) const
    {
        BOOST_ASSERT_MSG(
            this->_root_ptr && (index < this->size())
          , "index out of bounds"
        );

        typename node::const_pointer node_ptr = this->_root_ptr;

        return get(*binary_descendant_at_index(node_ptr, index), data_key());
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline typename deque<T,NodeGenerator,Balancer>::reference
        deque<T,NodeGenerator,Balancer>::operator[](size_type index)
    {
        BOOST_ASSERT_MSG(
            this->_root_ptr && (index < this->size())
          , "index out of bounds"
        );

        return get(
            *binary_descendant_at_index(this->_root_ptr, index)
          , data_key()
        );
    }

    template <typename T, typename NodeGenerator, typename Balancer>
    inline void
        swap(
            deque<T,NodeGenerator,Balancer>& x
          , deque<T,NodeGenerator,Balancer>& y
        )
    {
        x.swap(y);
    }
}}  // namespace boost::tree_node

#endif  // BOOST_TREE_NODE_CONTAINER_DEQUE_HPP_INCLUDED

