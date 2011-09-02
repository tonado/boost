
// Copyright (C) 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright (C) 2005-2011 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/unordered for documentation

#ifndef BOOST_UNORDERED_UNORDERED_SET_HPP_INCLUDED
#define BOOST_UNORDERED_UNORDERED_SET_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/unordered/unordered_set_fwd.hpp>
#include <boost/functional/hash.hpp>
#include <boost/unordered/detail/allocator_helpers.hpp>
#include <boost/unordered/detail/equivalent.hpp>
#include <boost/unordered/detail/unique.hpp>

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
#include <initializer_list>
#endif

#if defined(BOOST_MSVC)
#pragma warning(push)
#if BOOST_MSVC >= 1400
#pragma warning(disable:4396) //the inline specifier cannot be used when a
                              // friend declaration refers to a specialization
                              // of a function template
#endif
#endif

namespace boost
{
namespace unordered
{
    template <class T, class H, class P, class A>
    class unordered_set
    {
        BOOST_COPYABLE_AND_MOVABLE(unordered_set)
    public:

        typedef T key_type;
        typedef T value_type;
        typedef H hasher;
        typedef P key_equal;
        typedef A allocator_type;

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
    private:
#endif

        typedef typename ::boost::unordered::detail::rebind_wrap<
                allocator_type, value_type>::type
            value_allocator;
        typedef ::boost::unordered::detail::allocator_traits<value_allocator>
            allocator_traits;

        typedef ::boost::unordered::detail::set<H, P,
            value_allocator> types;
        typedef typename types::impl table;

        typedef typename types::node_ptr node_ptr;

    public:

        typedef typename allocator_traits::pointer pointer;
        typedef typename allocator_traits::const_pointer const_pointer;

        typedef value_type& reference;
        typedef value_type const& const_reference;

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef ::boost::unordered::iterator_detail::cl_iterator<
            value_allocator, true> const_local_iterator;
        typedef ::boost::unordered::iterator_detail::c_iterator<
            value_allocator, true> const_iterator;
        typedef const_local_iterator local_iterator;
        typedef const_iterator iterator;

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
    private:
#endif

        table table_;

    public:

        // construct/destroy/copy

        explicit unordered_set(
                size_type = ::boost::unordered::detail::default_bucket_count,
                const hasher& = hasher(),
                const key_equal& = key_equal(),
                const allocator_type& = allocator_type());

        explicit unordered_set(allocator_type const&);

        unordered_set(unordered_set const&, allocator_type const&);

        template <class InputIt>
        unordered_set(InputIt f, InputIt l);

        template <class InputIt>
        unordered_set(
                InputIt, InputIt,
                size_type,
                const hasher& = hasher(),
                const key_equal& = key_equal());        

        template <class InputIt>
        unordered_set(
                InputIt, InputIt,
                size_type,
                const hasher&,
                const key_equal&,
                const allocator_type&);
        
        ~unordered_set();

        unordered_set& operator=(
                BOOST_COPY_ASSIGN_REF(unordered_set) x)
        {
            table_.assign(x.table_);
            return *this;
        }

        unordered_set(unordered_set const&);

        unordered_set& operator=(BOOST_RV_REF(unordered_set) x)
        {
            table_.move_assign(x.table_);
            return *this;
        }

        unordered_set(BOOST_RV_REF(unordered_set) other)
            : table_(other.table_, ::boost::unordered::detail::move_tag())
        {
        }

#if !defined(BOOST_NO_RVALUE_REFERENCES)
        unordered_set(unordered_set&&, allocator_type const&);
#endif

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
        unordered_set(
                std::initializer_list<value_type>,
                size_type = ::boost::unordered::detail::default_bucket_count,
                const hasher& = hasher(),
                const key_equal&l = key_equal(),
                const allocator_type& = allocator_type());

        unordered_set& operator=(std::initializer_list<value_type>);
#endif

        allocator_type get_allocator() const
        {
            return table_.node_alloc();
        }

        // size and capacity

        bool empty() const
        {
            return table_.size_ == 0;
        }

        size_type size() const
        {
            return table_.size_;
        }

        size_type max_size() const;

        // iterators

        iterator begin()
        {
            return iterator(table_.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(table_.begin());
        }

        iterator end()
        {
            return iterator();
        }

        const_iterator end() const
        {
            return const_iterator();
        }

        const_iterator cbegin() const
        {
            return const_iterator(table_.begin());
        }

        const_iterator cend() const
        {
            return const_iterator();
        }

        // modifiers

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
        template <class... Args>
        std::pair<iterator, bool> emplace(Args&&...);
        template <class... Args>
        iterator emplace_hint(const_iterator, Args&&...);
#else

        std::pair<iterator, bool> emplace(
                boost::unordered::detail::empty_emplace
                    = boost::unordered::detail::empty_emplace(),
                value_type v = value_type()
            );
        iterator emplace_hint(const_iterator,
                boost::unordered::detail::empty_emplace
                    = boost::unordered::detail::empty_emplace(),
                value_type v = value_type()
            );

#define BOOST_UNORDERED_EMPLACE(z, n, _)                                       \
            template <                                                         \
                BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                            \
            >                                                                  \
            std::pair<iterator, bool> emplace(                                 \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n)                          \
            );                                                                 \
                                                                               \
            template <                                                         \
                BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                            \
            >                                                                  \
            iterator emplace_hint(const_iterator,                              \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n)                          \
            );

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_UNORDERED_EMPLACE_LIMIT,
            BOOST_UNORDERED_EMPLACE, _)

#undef BOOST_UNORDERED_EMPLACE

#endif

        std::pair<iterator, bool> insert(value_type const&);
        std::pair<iterator, bool> insert(BOOST_UNORDERED_RV_REF(value_type));
        iterator insert(const_iterator, value_type const&);
        iterator insert(const_iterator, BOOST_UNORDERED_RV_REF(value_type));
        template <class InputIt> void insert(InputIt, InputIt);

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
        void insert(std::initializer_list<value_type>);
#endif

        iterator erase(const_iterator);
        size_type erase(const key_type&);
        iterator erase(const_iterator, const_iterator);
        void quick_erase(const_iterator it) { erase(it); }
        void erase_return_void(const_iterator it) { erase(it); }

        void clear();
        void swap(unordered_set&);

        // observers

        hasher hash_function() const;
        key_equal key_eq() const;

        // lookup

        const_iterator find(const key_type&) const;

        template <class CompatibleKey, class CompatibleHash,
            class CompatiblePredicate>
        const_iterator find(
                CompatibleKey const&,
                CompatibleHash const&,
                CompatiblePredicate const&) const;

        size_type count(const key_type&) const;
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type&) const;

        // bucket interface

        size_type bucket_count() const
        {
            return table_.bucket_count_;
        }

        size_type max_bucket_count() const
        {
            return table_.max_bucket_count();
        }

        size_type bucket_size(size_type n) const;

        size_type bucket(const key_type& k) const
        {
            return table_.hash_function()(k) % table_.bucket_count_;
        }

        local_iterator begin(size_type n)
        {
            return local_iterator(
                table_.bucket_begin(n), n, table_.bucket_count_);
        }

        const_local_iterator begin(size_type n) const
        {
            return const_local_iterator(
                table_.bucket_begin(n), n, table_.bucket_count_);
        }

        local_iterator end(size_type)
        {
            return local_iterator();
        }

        const_local_iterator end(size_type) const
        {
            return const_local_iterator();
        }

        const_local_iterator cbegin(size_type n) const
        {
            return const_local_iterator(
                table_.bucket_begin(n), n, table_.bucket_count_);
        }

        const_local_iterator cend(size_type) const
        {
            return const_local_iterator();
        }

        // hash policy

        float max_load_factor() const
        {
            return table_.mlf_;
        }

        float load_factor() const;
        void max_load_factor(float);
        void rehash(size_type n);

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
        friend bool operator==<T,H,P,A>(
                unordered_set const&, unordered_set const&);
        friend bool operator!=<T,H,P,A>(
                unordered_set const&, unordered_set const&);
#endif
    }; // class template unordered_set

    template <class T, class H, class P, class A>
    class unordered_multiset
    {
        BOOST_COPYABLE_AND_MOVABLE(unordered_multiset)
    public:

        typedef T key_type;
        typedef T value_type;
        typedef H hasher;
        typedef P key_equal;
        typedef A allocator_type;

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
    private:
#endif

        typedef typename ::boost::unordered::detail::rebind_wrap<
                allocator_type, value_type>::type
            value_allocator;
        typedef ::boost::unordered::detail::allocator_traits<value_allocator>
            allocator_traits;

        typedef ::boost::unordered::detail::multiset<H, P,
            value_allocator> types;
        typedef typename types::impl table;

        typedef typename types::node_ptr node_ptr;

    public:

        typedef typename allocator_traits::pointer pointer;
        typedef typename allocator_traits::const_pointer const_pointer;

        typedef value_type& reference;
        typedef value_type const& const_reference;

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef ::boost::unordered::iterator_detail::cl_iterator<
            value_allocator, false> const_local_iterator;
        typedef ::boost::unordered::iterator_detail::c_iterator<
            value_allocator, false> const_iterator;
        typedef const_local_iterator local_iterator;
        typedef const_iterator iterator;

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
    private:
#endif

        table table_;

    public:

        // construct/destroy/copy

        explicit unordered_multiset(
                size_type = ::boost::unordered::detail::default_bucket_count,
                const hasher& = hasher(),
                const key_equal& = key_equal(),
                const allocator_type& = allocator_type());

        explicit unordered_multiset(allocator_type const&);

        unordered_multiset(unordered_multiset const&, allocator_type const&);

        template <class InputIt>
        unordered_multiset(InputIt, InputIt);

        template <class InputIt>
        unordered_multiset(
                InputIt, InputIt,
                size_type,
                const hasher& = hasher(),
                const key_equal& = key_equal());

        template <class InputIt>
        unordered_multiset(
                InputIt, InputIt,
                size_type,
                const hasher&,
                const key_equal&,
                const allocator_type&);

        ~unordered_multiset();

        unordered_multiset& operator=(
                BOOST_COPY_ASSIGN_REF(unordered_multiset) x)
        {
            table_.assign(x.table_);
            return *this;
        }

        unordered_multiset(unordered_multiset const&);

        unordered_multiset& operator=(BOOST_RV_REF(unordered_multiset) x)
        {
            table_.move_assign(x.table_);
            return *this;
        }

        unordered_multiset(BOOST_RV_REF(unordered_multiset) other)
            : table_(other.table_, ::boost::unordered::detail::move_tag())
        {
        }

#if !defined(BOOST_NO_RVALUE_REFERENCES)
        unordered_multiset(unordered_multiset&&, allocator_type const&);
#endif

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
        unordered_multiset(
                std::initializer_list<value_type>,
                size_type = ::boost::unordered::detail::default_bucket_count,
                const hasher& = hasher(),
                const key_equal&l = key_equal(),
                const allocator_type& = allocator_type());

        unordered_multiset& operator=(std::initializer_list<value_type>);
#endif

        allocator_type get_allocator() const
        {
            return table_.node_alloc();
        }

        // size and capacity

        bool empty() const
        {
            return table_.size_ == 0;
        }

        size_type size() const
        {
            return table_.size_;
        }

        size_type max_size() const;

        // iterators

        iterator begin()
        {
            return iterator(table_.begin());
        }

        const_iterator begin() const
        {
            return const_iterator(table_.begin());
        }

        iterator end()
        {
            return iterator();
        }

        const_iterator end() const
        {
            return const_iterator();
        }

        const_iterator cbegin() const
        {
            return const_iterator(table_.begin());
        }

        const_iterator cend() const
        {
            return const_iterator();
        }

        // modifiers

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
        template <class... Args>
        iterator emplace(Args&&...);
        template <class... Args>
        iterator emplace_hint(const_iterator, Args&&...);
#else

        iterator emplace(
                boost::unordered::detail::empty_emplace
                    = boost::unordered::detail::empty_emplace(),
                value_type v = value_type()
            );
        iterator emplace_hint(const_iterator,
                boost::unordered::detail::empty_emplace
                    = boost::unordered::detail::empty_emplace(),
                value_type v = value_type()
            );

#define BOOST_UNORDERED_EMPLACE(z, n, _)                                       \
            template <                                                         \
                BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                            \
            >                                                                  \
            iterator emplace(                                                  \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n)                          \
            );                                                                 \
                                                                               \
            template <                                                         \
                BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                            \
            >                                                                  \
            iterator emplace_hint(const_iterator,                              \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n)                          \
            );

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_UNORDERED_EMPLACE_LIMIT,
            BOOST_UNORDERED_EMPLACE, _)

#undef BOOST_UNORDERED_EMPLACE

#endif

        iterator insert(value_type const&);
        iterator insert(BOOST_UNORDERED_RV_REF(value_type));
        iterator insert(const_iterator, value_type const&);
        iterator insert(const_iterator, BOOST_UNORDERED_RV_REF(value_type));

        template <class InputIt>
        void insert(InputIt, InputIt);

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
        void insert(std::initializer_list<value_type>);
#endif

        iterator erase(const_iterator);
        size_type erase(const key_type&);
        iterator erase(const_iterator, const_iterator);
        void quick_erase(const_iterator position) { erase(position); }
        void erase_return_void(const_iterator position) { erase(position); }

        void clear();
        void swap(unordered_multiset&);

        // observers

        hasher hash_function() const;
        key_equal key_eq() const;

        // lookup

        const_iterator find(const key_type&) const;

        template <class CompatibleKey, class CompatibleHash,
            class CompatiblePredicate>
        const_iterator find(
                CompatibleKey const&,
                CompatibleHash const&,
                CompatiblePredicate const&) const;

        size_type count(const key_type&) const;
        std::pair<const_iterator, const_iterator>
        equal_range(const key_type&) const;

        // bucket interface

        size_type bucket_count() const
        {
            return table_.bucket_count_;
        }

        size_type max_bucket_count() const
        {
            return table_.max_bucket_count();
        }

        size_type bucket_size(size_type) const;

        size_type bucket(const key_type& k) const
        {
            return table_.hash_function()(k) % table_.bucket_count_;
        }

        local_iterator begin(size_type n)
        {
            return local_iterator(
                table_.bucket_begin(n), n, table_.bucket_count_);
        }

        const_local_iterator begin(size_type n) const
        {
            return const_local_iterator(
                table_.bucket_begin(n), n, table_.bucket_count_);
        }

        local_iterator end(size_type)
        {
            return local_iterator();
        }

        const_local_iterator end(size_type) const
        {
            return const_local_iterator();
        }

        const_local_iterator cbegin(size_type n) const
        {
            return const_local_iterator(
                table_.bucket_begin(n), n, table_.bucket_count_);
        }

        const_local_iterator cend(size_type) const
        {
            return const_local_iterator();
        }

        // hash policy

        float max_load_factor() const
        {
            return table_.mlf_;
        }

        float load_factor() const;
        void max_load_factor(float);
        void rehash(size_type);

#if !BOOST_WORKAROUND(__BORLANDC__, < 0x0582)
        friend bool operator==<T,H,P,A>(
                unordered_multiset const&, unordered_multiset const&);
        friend bool operator!=<T,H,P,A>(
                unordered_multiset const&, unordered_multiset const&);
#endif
    }; // class template unordered_multiset

////////////////////////////////////////////////////////////////////////////////

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::unordered_set(
            size_type n, const hasher &hf, const key_equal &eql,
            const allocator_type &a)
      : table_(n, hf, eql, a)
    {
    }

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::unordered_set(allocator_type const& a)
      : table_(::boost::unordered::detail::default_bucket_count,
            hasher(), key_equal(), a)
    {
    }

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::unordered_set(
            unordered_set const& other, allocator_type const& a)
      : table_(other.table_, a)
    {
    }

    template <class T, class H, class P, class A>
    template <class InputIt>
    unordered_set<T,H,P,A>::unordered_set(InputIt f, InputIt l)
      : table_(::boost::unordered::detail::initial_size(f, l),
        hasher(), key_equal(), allocator_type())
    {
        table_.insert_range(f, l);
    }

    template <class T, class H, class P, class A>
    template <class InputIt>
    unordered_set<T,H,P,A>::unordered_set(
            InputIt f, InputIt l,
            size_type n,
            const hasher &hf,
            const key_equal &eql)
      : table_(::boost::unordered::detail::initial_size(f, l, n),
            hf, eql, allocator_type())
    {
        table_.insert_range(f, l);
    }
    
    template <class T, class H, class P, class A>
    template <class InputIt>
    unordered_set<T,H,P,A>::unordered_set(
            InputIt f, InputIt l,
            size_type n,
            const hasher &hf,
            const key_equal &eql,
            const allocator_type &a)
      : table_(::boost::unordered::detail::initial_size(f, l, n), hf, eql, a)
    {
        table_.insert_range(f, l);
    }
    
    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::~unordered_set() {}

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::unordered_set(unordered_set const& other)
      : table_(other.table_)
    {
    }

#if !defined(BOOST_NO_RVALUE_REFERENCES)

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::unordered_set(
            unordered_set&& other, allocator_type const& a)
      : table_(other.table_, a, ::boost::unordered::detail::move_tag())
    {
    }

#endif

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>::unordered_set(
            std::initializer_list<value_type> list, size_type n,
            const hasher &hf, const key_equal &eql, const allocator_type &a)
      : table_(
            ::boost::unordered::detail::initial_size(
                list.begin(), list.end(), n),
            hf, eql, a)
    {
        table_.insert_range(list.begin(), list.end());
    }

    template <class T, class H, class P, class A>
    unordered_set<T,H,P,A>& unordered_set<T,H,P,A>::operator=(
            std::initializer_list<value_type> list)
    {
        table_.clear();
        table_.insert_range(list.begin(), list.end());
        return *this;
    }

#endif

    // size and capacity

    template <class T, class H, class P, class A>
    std::size_t unordered_set<T,H,P,A>::max_size() const
    {
        return table_.max_size();
    }

    // modifiers

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)
    template <class T, class H, class P, class A>
    template <class... Args>
    std::pair<typename unordered_set<T,H,P,A>::iterator, bool>
        unordered_set<T,H,P,A>::emplace(Args&&... args)
    {
        return table_.emplace(std::forward<Args>(args)...);
    }

    template <class T, class H, class P, class A>
    template <class... Args>
    typename unordered_set<T,H,P,A>::iterator
        unordered_set<T,H,P,A>::emplace_hint(const_iterator, Args&&... args)
    {
        return iterator(table_.emplace(std::forward<Args>(args)...).first);
    }
#else

    template <class T, class H, class P, class A>
    std::pair<typename unordered_set<T,H,P,A>::iterator, bool>
        unordered_set<T,H,P,A>::emplace(
                boost::unordered::detail::empty_emplace,
                value_type v
            )
    {
        return table_.emplace(boost::move(v));
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::iterator
        unordered_set<T,H,P,A>::emplace_hint(const_iterator,
                boost::unordered::detail::empty_emplace,
                value_type v
            )
    {
        return iterator(table_.emplace(boost::move(v)).first);
    }

#define BOOST_UNORDERED_EMPLACE(z, n, _)                                    \
        template <class T, class H, class P, class A>                       \
        template <                                                          \
            BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                             \
        >                                                                   \
        std::pair<typename unordered_set<T,H,P,A>::iterator, bool>          \
            unordered_set<T,H,P,A>::emplace(                                \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n))                      \
        {                                                                   \
            return table_.emplace(BOOST_UNORDERED_CALL_PARAMS(z, n));       \
        }                                                                   \
                                                                            \
        template <class T, class H, class P, class A>                       \
        template <                                                          \
            BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                             \
        >                                                                   \
        typename unordered_set<T,H,P,A>::iterator                           \
            unordered_set<T,H,P,A>::emplace_hint(                           \
                const_iterator,                                             \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n)                       \
        )                                                                   \
        {                                                                   \
            return iterator(table_.emplace(                                 \
                BOOST_UNORDERED_CALL_PARAMS(z, n)).first);                  \
        }

    BOOST_PP_REPEAT_FROM_TO(1, BOOST_UNORDERED_EMPLACE_LIMIT,
        BOOST_UNORDERED_EMPLACE, _)

#undef BOOST_UNORDERED_EMPLACE

#endif

    template <class T, class H, class P, class A>
    std::pair<typename unordered_set<T,H,P,A>::iterator, bool>
        unordered_set<T,H,P,A>::insert(value_type const& obj)
    {
        return table_.emplace(obj);
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::iterator
        unordered_set<T,H,P,A>::insert(const_iterator,
                value_type const& obj)
    {
        return iterator(table_.emplace(obj).first);
    }

    template <class T, class H, class P, class A>
    std::pair<typename unordered_set<T,H,P,A>::iterator, bool>
        unordered_set<T,H,P,A>::insert(BOOST_UNORDERED_RV_REF(value_type) obj)
    {
        return table_.emplace(boost::move(obj));
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::iterator
        unordered_set<T,H,P,A>::insert(const_iterator,
                BOOST_UNORDERED_RV_REF(value_type) obj)
    {
        return iterator(table_.emplace(boost::move(obj)).first);
    }

    template <class T, class H, class P, class A>
    template <class InputIt>
    void unordered_set<T,H,P,A>::insert(InputIt first, InputIt last)
    {
        table_.insert_range(first, last);
    }

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
    template <class T, class H, class P, class A>
    void unordered_set<T,H,P,A>::insert(std::initializer_list<value_type> list)
    {
        table_.insert_range(list.begin(), list.end());
    }
#endif

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::iterator
        unordered_set<T,H,P,A>::erase(const_iterator position)
    {
        return iterator(table_.erase(position.node_));
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::size_type
        unordered_set<T,H,P,A>::erase(const key_type& k)
    {
        return table_.erase_key(k);
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::iterator
        unordered_set<T,H,P,A>::erase(const_iterator first, const_iterator last)
    {
        return iterator(table_.erase_range(first.node_, last.node_));
    }

    template <class T, class H, class P, class A>
    void unordered_set<T,H,P,A>::clear()
    {
        table_.clear();
    }

    template <class T, class H, class P, class A>
    void unordered_set<T,H,P,A>::swap(unordered_set& other)
    {
        table_.swap(other.table_);
    }

    // observers

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::hasher
        unordered_set<T,H,P,A>::hash_function() const
    {
        return table_.hash_function();
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::key_equal
        unordered_set<T,H,P,A>::key_eq() const
    {
        return table_.key_eq();
    }

    // lookup

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::const_iterator
        unordered_set<T,H,P,A>::find(const key_type& k) const
    {
        return const_iterator(table_.find_node(k));
    }

    template <class T, class H, class P, class A>
    template <class CompatibleKey, class CompatibleHash,
        class CompatiblePredicate>
    typename unordered_set<T,H,P,A>::const_iterator
        unordered_set<T,H,P,A>::find(
            CompatibleKey const& k,
            CompatibleHash const& hash,
            CompatiblePredicate const& eq) const
    {
        return const_iterator(table_.generic_find_node(k, hash, eq));
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::size_type
        unordered_set<T,H,P,A>::count(const key_type& k) const
    {
        return table_.count(k);
    }

    template <class T, class H, class P, class A>
    std::pair<
            typename unordered_set<T,H,P,A>::const_iterator,
            typename unordered_set<T,H,P,A>::const_iterator>
        unordered_set<T,H,P,A>::equal_range(const key_type& k) const
    {
        return table_.equal_range(k);
    }

    template <class T, class H, class P, class A>
    typename unordered_set<T,H,P,A>::size_type
        unordered_set<T,H,P,A>::bucket_size(size_type n) const
    {
        return table_.bucket_size(n);
    }

    // hash policy

    template <class T, class H, class P, class A>
    float unordered_set<T,H,P,A>::load_factor() const
    {
        return table_.load_factor();
    }

    template <class T, class H, class P, class A>
    void unordered_set<T,H,P,A>::max_load_factor(float m)
    {
        table_.max_load_factor(m);
    }

    template <class T, class H, class P, class A>
    void unordered_set<T,H,P,A>::rehash(size_type n)
    {
        table_.rehash(n);
    }

    template <class T, class H, class P, class A>
    inline bool operator==(
            unordered_set<T,H,P,A> const& m1,
            unordered_set<T,H,P,A> const& m2)
    {
#if BOOST_WORKAROUND(__CODEGEARC__, BOOST_TESTED_AT(0x0613))
        struct dummy { unordered_set<T,H,P,A> x; };
#endif
        return m1.table_.equals(m2.table_);
    }

    template <class T, class H, class P, class A>
    inline bool operator!=(
            unordered_set<T,H,P,A> const& m1,
            unordered_set<T,H,P,A> const& m2)
    {
#if BOOST_WORKAROUND(__CODEGEARC__, BOOST_TESTED_AT(0x0613))
        struct dummy { unordered_set<T,H,P,A> x; };
#endif
        return !m1.table_.equals(m2.table_);
    }

    template <class T, class H, class P, class A>
    inline void swap(
            unordered_set<T,H,P,A> &m1,
            unordered_set<T,H,P,A> &m2)
    {
#if BOOST_WORKAROUND(__CODEGEARC__, BOOST_TESTED_AT(0x0613))
        struct dummy { unordered_set<T,H,P,A> x; };
#endif
        m1.swap(m2);
    }

////////////////////////////////////////////////////////////////////////////////

    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            size_type n, const hasher &hf, const key_equal &eql,
            const allocator_type &a)
      : table_(n, hf, eql, a)
    {
    }

    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::unordered_multiset(allocator_type const& a)
      : table_(::boost::unordered::detail::default_bucket_count,
            hasher(), key_equal(), a)
    {
    }

    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            unordered_multiset const& other, allocator_type const& a)
      : table_(other.table_, a)
    {
    }

    template <class T, class H, class P, class A>
    template <class InputIt>
    unordered_multiset<T,H,P,A>::unordered_multiset(InputIt f, InputIt l)
      : table_(::boost::unordered::detail::initial_size(f, l),
        hasher(), key_equal(), allocator_type())
    {
        table_.insert_range(f, l);
    }

    template <class T, class H, class P, class A>
    template <class InputIt>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            InputIt f, InputIt l,
            size_type n,
            const hasher &hf,
            const key_equal &eql)
      : table_(::boost::unordered::detail::initial_size(f, l, n),
            hf, eql, allocator_type())
    {
        table_.insert_range(f, l);
    }
    
    template <class T, class H, class P, class A>
    template <class InputIt>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            InputIt f, InputIt l,
            size_type n,
            const hasher &hf,
            const key_equal &eql,
            const allocator_type &a)
      : table_(::boost::unordered::detail::initial_size(f, l, n), hf, eql, a)
    {
        table_.insert_range(f, l);
    }
    
    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::~unordered_multiset() {}

    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            unordered_multiset const& other)
      : table_(other.table_)
    {
    }

#if !defined(BOOST_NO_RVALUE_REFERENCES)

    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            unordered_multiset&& other, allocator_type const& a)
      : table_(other.table_, a, ::boost::unordered::detail::move_tag())
    {
    }

#endif

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>::unordered_multiset(
            std::initializer_list<value_type> list, size_type n,
            const hasher &hf, const key_equal &eql, const allocator_type &a)
      : table_(
            ::boost::unordered::detail::initial_size(
                list.begin(), list.end(), n),
            hf, eql, a)
    {
        table_.insert_range(list.begin(), list.end());
    }

    template <class T, class H, class P, class A>
    unordered_multiset<T,H,P,A>& unordered_multiset<T,H,P,A>::operator=(
            std::initializer_list<value_type> list)
    {
        table_.clear();
        table_.insert_range(list.begin(), list.end());
        return *this;
    }
#endif

    // size and capacity

    template <class T, class H, class P, class A>
    std::size_t unordered_multiset<T,H,P,A>::max_size() const
    {
        return table_.max_size();
    }

    // modifiers

#if defined(BOOST_UNORDERED_STD_FORWARD_MOVE)

    template <class T, class H, class P, class A>
    template <class... Args>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::emplace(Args&&... args)
    {
        return iterator(table_.emplace(std::forward<Args>(args)...));
    }

    template <class T, class H, class P, class A>
    template <class... Args>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::emplace_hint(
            const_iterator, Args&&... args)
    {
        return iterator(table_.emplace(std::forward<Args>(args)...));
    }

#else

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::emplace(
                boost::unordered::detail::empty_emplace,
                value_type v
            )
    {
        return iterator(table_.emplace(boost::move(v)));
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::emplace_hint(const_iterator,
                boost::unordered::detail::empty_emplace,
                value_type v
            )
    {
        return iterator(table_.emplace(boost::move(v)));
    }

#define BOOST_UNORDERED_EMPLACE(z, n, _)                                    \
        template <class T, class H, class P, class A>                       \
        template <                                                          \
            BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                             \
        >                                                                   \
        typename unordered_multiset<T,H,P,A>::iterator                      \
            unordered_multiset<T,H,P,A>::emplace(                           \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n))                      \
        {                                                                   \
            return iterator(table_.emplace(                                 \
                BOOST_UNORDERED_CALL_PARAMS(z, n)));                        \
        }                                                                   \
                                                                            \
        template <class T, class H, class P, class A>                       \
        template <                                                          \
            BOOST_UNORDERED_TEMPLATE_ARGS(z, n)                             \
        >                                                                   \
        typename unordered_multiset<T,H,P,A>::iterator                      \
            unordered_multiset<T,H,P,A>::emplace_hint(                      \
                const_iterator,                                             \
                BOOST_UNORDERED_FUNCTION_PARAMS(z, n))                      \
        {                                                                   \
            return iterator(table_.emplace(                                 \
                BOOST_UNORDERED_CALL_PARAMS(z, n)));                        \
        }

    BOOST_PP_REPEAT_FROM_TO(1, BOOST_UNORDERED_EMPLACE_LIMIT,
        BOOST_UNORDERED_EMPLACE, _)

#undef BOOST_UNORDERED_EMPLACE

#endif

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::insert(value_type const& obj)
    {
        return iterator(table_.emplace(obj));
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::insert(const_iterator,
                value_type const& obj)
    {
        return iterator(table_.emplace(obj));
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::insert(BOOST_UNORDERED_RV_REF(value_type) obj)
    {
        return iterator(table_.emplace(boost::move(obj)));
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::insert(const_iterator,
                BOOST_UNORDERED_RV_REF(value_type) obj)
    {
        return iterator(table_.emplace(boost::move(obj)));
    }

    template <class T, class H, class P, class A>
    template <class InputIt>
    void unordered_multiset<T,H,P,A>::insert(InputIt first, InputIt last)
    {
        table_.insert_range(first, last);
    }

#if !defined(BOOST_NO_0X_HDR_INITIALIZER_LIST)
    template <class T, class H, class P, class A>
    void unordered_multiset<T,H,P,A>::insert(std::initializer_list<value_type> list)
    {
        table_.insert_range(list.begin(), list.end());
    }
#endif

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::erase(const_iterator position)
    {
        return iterator(table_.erase(position.node_));
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::size_type
        unordered_multiset<T,H,P,A>::erase(const key_type& k)
    {
        return table_.erase_key(k);
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::iterator
        unordered_multiset<T,H,P,A>::erase(const_iterator first, const_iterator last)
    {
        return iterator(table_.erase_range(first.node_, last.node_));
    }

    template <class T, class H, class P, class A>
    void unordered_multiset<T,H,P,A>::clear()
    {
        table_.clear();
    }

    template <class T, class H, class P, class A>
    void unordered_multiset<T,H,P,A>::swap(unordered_multiset& other)
    {
        table_.swap(other.table_);
    }

    // observers

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::hasher
        unordered_multiset<T,H,P,A>::hash_function() const
    {
        return table_.hash_function();
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::key_equal
        unordered_multiset<T,H,P,A>::key_eq() const
    {
        return table_.key_eq();
    }

    // lookup

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::const_iterator
        unordered_multiset<T,H,P,A>::find(const key_type& k) const
    {
        return const_iterator(table_.find_node(k));
    }

    template <class T, class H, class P, class A>
    template <class CompatibleKey, class CompatibleHash,
        class CompatiblePredicate>
    typename unordered_multiset<T,H,P,A>::const_iterator
        unordered_multiset<T,H,P,A>::find(
            CompatibleKey const& k,
            CompatibleHash const& hash,
            CompatiblePredicate const& eq) const
    {
        return const_iterator(table_.generic_find_node(k, hash, eq));
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::size_type
        unordered_multiset<T,H,P,A>::count(const key_type& k) const
    {
        return table_.count(k);
    }

    template <class T, class H, class P, class A>
    std::pair<
            typename unordered_multiset<T,H,P,A>::const_iterator,
            typename unordered_multiset<T,H,P,A>::const_iterator>
        unordered_multiset<T,H,P,A>::equal_range(const key_type& k) const
    {
        return table_.equal_range(k);
    }

    template <class T, class H, class P, class A>
    typename unordered_multiset<T,H,P,A>::size_type
        unordered_multiset<T,H,P,A>::bucket_size(size_type n) const
    {
        return table_.bucket_size(n);
    }

    // hash policy

    template <class T, class H, class P, class A>
    float unordered_multiset<T,H,P,A>::load_factor() const
    {
        return table_.load_factor();
    }

    template <class T, class H, class P, class A>
    void unordered_multiset<T,H,P,A>::max_load_factor(float m)
    {
        table_.max_load_factor(m);
    }

    template <class T, class H, class P, class A>
    void unordered_multiset<T,H,P,A>::rehash(size_type n)
    {
        table_.rehash(n);
    }

    template <class T, class H, class P, class A>
    inline bool operator==(
            unordered_multiset<T,H,P,A> const& m1,
            unordered_multiset<T,H,P,A> const& m2)
    {
#if BOOST_WORKAROUND(__CODEGEARC__, BOOST_TESTED_AT(0x0613))
        struct dummy { unordered_multiset<T,H,P,A> x; };
#endif
        return m1.table_.equals(m2.table_);
    }

    template <class T, class H, class P, class A>
    inline bool operator!=(
            unordered_multiset<T,H,P,A> const& m1,
            unordered_multiset<T,H,P,A> const& m2)
    {
#if BOOST_WORKAROUND(__CODEGEARC__, BOOST_TESTED_AT(0x0613))
        struct dummy { unordered_multiset<T,H,P,A> x; };
#endif
        return !m1.table_.equals(m2.table_);
    }

    template <class T, class H, class P, class A>
    inline void swap(
            unordered_multiset<T,H,P,A> &m1,
            unordered_multiset<T,H,P,A> &m2)
    {
#if BOOST_WORKAROUND(__CODEGEARC__, BOOST_TESTED_AT(0x0613))
        struct dummy { unordered_multiset<T,H,P,A> x; };
#endif
        m1.swap(m2);
    }

} // namespace unordered
} // namespace boost

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // BOOST_UNORDERED_UNORDERED_SET_HPP_INCLUDED
