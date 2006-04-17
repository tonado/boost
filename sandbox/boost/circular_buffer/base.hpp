// Implementation of the base circular buffer.

// Copyright (c) 2003-2006 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_CIRCULAR_BUFFER_BASE_HPP)
#define BOOST_CIRCULAR_BUFFER_BASE_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif

#include <boost/call_traits.hpp>
#include <boost/concept_check.hpp>
#include <boost/limits.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits/is_stateless.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <algorithm>
#include <utility>
#include <deque>
#if !defined(BOOST_NO_EXCEPTIONS)
    #include <stdexcept>
#endif
#if BOOST_CB_ENABLE_DEBUG
    #include <cstring>
#endif

namespace boost {

/*!
    \class circular_buffer
    \brief Circular buffer - a STL compliant container.
    \param T The type of the elements stored in the circular buffer.
	       <code>T</code> has to be <a href="http://www.sgi.com/tech/stl/Assignable.html">
		   SGIAssignable</a> (SGI STL defined combination of <a href="../../utility/Assignable.html">
		   Assignable</a> and <a href="../../utility/CopyConstructible.html">CopyConstructible</a>.
		   Moreover <code>T</code> has to be <a href="http://www.sgi.com/tech/stl/DefaultConstructible.html">
		   DefaultConstructible</a> if supplied as a default parameter during invocation of some methods
		   e.g. insert(iterator pos, const T& item = T()).
    \param Alloc The allocator type used for all internal memory management.
	       The <code>Alloc</code> has to meet the allocator requirements imposed by STL.
           Default: std::allocator<T>
    \author Jan Gaspar
    \version 3.7
    \date 2006
    
    For detailed documentation of the circular_buffer visit:
    http://www.boost.org/libs/circular_buffer/doc/circular_buffer.html
*/
template <class T, class Alloc>
class circular_buffer : cb_details::iterator_registry {

// Requirements
    BOOST_CLASS_REQUIRE(T, boost, SGIAssignableConcept);

public:
// Basic types

    //! The type of the elements stored in the circular buffer.
    typedef typename Alloc::value_type value_type;

    //! Pointer to the element.
    typedef typename Alloc::pointer pointer;

    //! Const pointer to the element.
    typedef typename Alloc::const_pointer const_pointer;

    //! Reference to the element.
    typedef typename Alloc::reference reference;

    //! Const reference to the element.
    typedef typename Alloc::const_reference const_reference;

    //! Distance type.
    /*!
        A signed integral type used to represent the distance between two iterators.
    */
    typedef typename Alloc::difference_type difference_type;

    //! Size type.
    /*!
        An unsigned integral type that can represent any nonnegative value of the container's distance type.
    */
    typedef typename Alloc::size_type size_type;

    //! The type of the allocator used in the circular buffer.
    typedef Alloc allocator_type;

// Iterators

    //! Const (random access) iterator used to iterate through a circular buffer.
    typedef cb_details::iterator< circular_buffer<T, Alloc>, cb_details::const_traits<Alloc> > const_iterator;

    //! Iterator (random access) used to iterate through a circular buffer.
    typedef cb_details::iterator< circular_buffer<T, Alloc>, cb_details::nonconst_traits<Alloc> > iterator;

    //! Const iterator used to iterate backwards through a circular buffer.
    typedef reverse_iterator<const_iterator> const_reverse_iterator;

    //! Iterator used to iterate backwards through a circular buffer.
    typedef reverse_iterator<iterator> reverse_iterator;

// Container specific types

    //! An array range. TODO - better doc
    typedef std::pair<pointer, size_type> array_range;

	//! A range of a const array. TODO - better doc
    typedef std::pair<const_pointer, size_type> const_array_range;

	//! Capacity type (defined just for consistency with circular_buffer_space_optimized).
	typedef size_type capacity_control; 
    
// Helper types

    // Define a type that represents the "best" way to pass the value_type to a method.
    typedef typename call_traits<value_type>::param_type param_value_type;

    // Define a type that represents the "best" way to return the value_type from a const method.
    typedef typename call_traits<value_type>::param_type return_value_type;

private:
// Member variables

    //! The internal buffer used for storing elements in the circular buffer.
    pointer m_buff;

    //! The internal buffer's end (end of the storage space).
    pointer m_end;

    //! The virtual beginning of the circular buffer (the leftmost element).
    pointer m_first;

    //! The virtual end of the circular buffer (one behind the rightmost element).
    pointer m_last;

    //! The number of items currently stored in the circular buffer.
    size_type m_size;

    //! The allocator.
    allocator_type m_alloc;

// Friends
#if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
    friend iterator;
    friend const_iterator;
#else
    template <class Buff, class Traits> friend struct cb_details::iterator;
#endif

public:
// Allocator
    
    //! Return the allocator.
    allocator_type get_allocator() const { return m_alloc; }

    //! Return the allocator.
    /*!
        \note This method was added in order to optimize obtaining of the allocator with a state,
              although use of stateful allocators in STL is discouraged.
    */
    allocator_type& get_allocator() { return m_alloc; }
    
// Element access

    //! Return an iterator pointing to the beginning of the circular buffer.
    iterator begin() { return iterator(this, empty() ? 0 : m_first); }

    //! Return an iterator pointing to the end of the circular buffer.
    iterator end() { return iterator(this, 0); }

    //! Return a const iterator pointing to the beginning of the circular buffer.
    const_iterator begin() const { return const_iterator(this, empty() ? 0 : m_first); }

    //! Return a const iterator pointing to the end of the circular buffer.
    const_iterator end() const { return const_iterator(this, 0); }

    //! Return a reverse iterator pointing to the beginning of the reversed circular buffer.
    reverse_iterator rbegin() { return reverse_iterator(end()); }

    //! Return a reverse iterator pointing to the end of the reversed circular buffer.
    reverse_iterator rend() { return reverse_iterator(begin()); }

    //! Return a const reverse iterator pointing to the beginning of the reversed circular buffer.
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

    //! Return a const reverse iterator pointing to the end of the reversed circular buffer.
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    //! Return the element at the <code>index</code> position.
    /*!
        \pre <code>*(this).size() > index</code>
    */
    reference operator [] (size_type index) {
        BOOST_CB_ASSERT(index < size()); // check for invalid index
        return *add(m_first, index);
    }

    //! Return the element at the <code>index</code> position.
    /*!
        \pre <code>*(this).size() > index</code>
    */
    return_value_type operator [] (size_type index) const {
        BOOST_CB_ASSERT(index < size()); // check for invalid index
        return *add(m_first, index);
    }

    //! Return the element at the <code>index</code> position.
    /*!
        \throws std::out_of_range thrown when the <code>index</code> is invalid.
    */
    reference at(size_type index) {
        check_position(index);
        return (*this)[index];
    }

    //! Return the element at the <code>index</code> position.
    /*!
        \throws std::out_of_range thrown when the <code>index</code> is invalid.
    */
    return_value_type at(size_type index) const {
        check_position(index);
        return (*this)[index];
    }

    //! Return the first (leftmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    reference front() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (front element not available)
        return *m_first;
    }

    //! Return the last (rightmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    reference back() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (back element not available)
        return *((m_last == m_buff ? m_end : m_last) - 1);
    }

    //! Return the first (leftmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    return_value_type front() const {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (front element not available)
        return *m_first;
    }

    //! Return the last (rightmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
    */
    return_value_type back() const {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (back element not available)
        return *((m_last == m_buff ? m_end : m_last) - 1);
    }

    //! TODO doc
    array_range array_one() {
        return array_range(m_first, (m_last <= m_first && !empty() ? m_end : m_last) - m_first);
    }
    
    //! TODO doc
    array_range array_two() {
        return array_range(m_buff, m_last <= m_first && !empty() ? m_last - m_buff : 0);
    }
    
    //! TODO doc
    const_array_range array_one() const {
        return const_array_range(m_first, (m_last <= m_first && !empty() ? m_end : m_last) - m_first);
    }
    
    //! TODO doc
    const_array_range array_two() const {
        return const_array_range(m_buff, m_last <= m_first && !empty() ? m_last - m_buff : 0);
    }
    
    //! TODO doc - Return pointer to data stored in the circular buffer as a continuous array of values.
    /*!
        This method can be useful e.g. when passing the stored data into the legacy C API.
        \post <code>\&(*this)[0] \< \&(*this)[1] \< ... \< \&(*this).back()</code>
        \return 0 if empty.
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    pointer linearize() {
        if (empty())
            return 0;
        if (m_first < m_last || m_last == m_buff)
            return m_first;
        pointer src = m_first;
        pointer dest = m_buff;
        size_type moved = 0;
        size_type constructed = 0;
        BOOST_TRY {
			for (pointer first = m_first; dest < src; src = first) {
				for (size_type ii = 0; src < m_end; ++src, ++dest, ++moved, ++ii) {
					if (moved == size()) {
						first = dest;
						break;
					}
					if (dest == first) {
						first += ii;
						break;
					}
					if (is_uninitialized(dest)) {
						m_alloc.construct(dest, *src);
						++constructed;
					} else {
						value_type tmp = *src;
						replace(src, *dest);
						replace(dest, tmp);
					}
				}
			}
		} BOOST_CATCH(...) {
			m_last += constructed;
            m_size += constructed;
			BOOST_RETHROW
		}
		BOOST_CATCH_END
        for (src = m_end - constructed; src < m_end; ++src)
            destroy_item(src);
        m_first = m_buff;
        m_last = add(m_buff, size());
        return m_buff;
    }

// Size and capacity

    //! Return the number of elements currently stored in the circular buffer.
    size_type size() const { return m_size; }

    //! Return the largest possible size (or capacity) of the circular buffer.
    size_type max_size() const {
        return std::min<size_type>(m_alloc.max_size(), std::numeric_limits<difference_type>::max());
    }

    //! Is the circular buffer empty?
    /*!
        \return <code>true</code> if there are no elements stored in the circular buffer.
        \return <code>false</code> otherwise.
    */
    bool empty() const { return size() == 0; }

    //! Is the circular buffer full?
    /*!
        \return <code>true</code> if the number of elements stored in the circular buffer
                equals the capacity of the circular buffer.
        \return <code>false</code> otherwise.
    */
    bool full() const { return size() == capacity(); }

    //! Return the capacity of the circular buffer.
    size_type capacity() const { return m_end - m_buff; }

    //! Change the capacity of the circular buffer.
    /*!
        \param new_capacity The new capacity.
        \param remove_front This parameter plays its role only if the
               current number of elements stored in the circular buffer
               is greater than the desired new capacity. If set to
               <code>true</code> then the first (leftmost) elements
               will be removed. If set to <code>false</code> then the
               last (leftmost) elements will be removed.
        \post <code>(*this).capacity() == new_capacity</code><br>
              If the current number of elements stored in the circular
              buffer is greater than the desired new capacity then
              <code>((*this).size() - new_capacity)</code> elements
              will be removed according to the <code>remove_front</code>
              parameter.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void set_capacity(size_type new_capacity) {
        if (new_capacity == capacity())
            return;
        pointer buff = allocate(new_capacity);
        BOOST_TRY {
			reset(buff, cb_details::uninitialized_copy(end() - std::min(new_capacity, size()), end(), buff, m_alloc), new_capacity);
		} BOOST_CATCH(...) {
			deallocate(buff, new_capacity);
			BOOST_RETHROW
		}
		BOOST_CATCH_END
    }

    //! Change the size of the circular buffer.
    /*!
        \param new_size The new size.
        \param item See the postcondition.
        \param remove_front This parameter plays its role only if the
               current number of elements stored in the circular buffer
               is greater than the desired new capacity. If set to
               <code>true</code> then the first (leftmost) elements
               will be removed. If set to <code>false</code> then the
               last (rightmost) elements will be removed.
        \post <code>(*this).size() == new_size</code><br>
              If the new size is greater than the current size, the rest
              of the circular buffer is filled with copies of <code>item</code>.
              In case the resulting size exceeds the current capacity
              the capacity is set to <code>new_size</code>.
              If the new size is lower than the current size then
              <code>((*this).size() - new_size)</code> elements will be removed
              according to the <code>remove_front</code> parameter.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void resize(size_type new_size, param_value_type item = value_type()) {
        if (new_size > size())
            increase_size(new_size, item);
        else
            erase(begin(), end() - new_size);
    }

    //! TODO doc
    void rset_capacity(size_type new_capacity) {
        if (new_capacity == capacity())
            return;
        pointer buff = allocate(new_capacity);
        BOOST_TRY {
			reset(buff, cb_details::uninitialized_copy(begin(), begin() + std::min(new_capacity, size()), buff, m_alloc), new_capacity);
		} BOOST_CATCH(...) {
			deallocate(buff, new_capacity);
			BOOST_RETHROW
		}
		BOOST_CATCH_END
    }

    //! TODO doc
    void rresize(size_type new_size, param_value_type item = value_type()) {
        if (new_size > size())
            increase_size(new_size, item);
        else
            erase(begin() + new_size, end());
    }

// Construction/Destruction

	//! Create an empty circular buffer with a maximum capacity.
    /*!
        \post <code>capacity() == max_size() \&\& size() == 0</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
    */
    explicit circular_buffer(
        const allocator_type& alloc = allocator_type())
    : m_size(0), m_alloc(alloc) {
        initialize(max_size());
    }

    //! Create an empty circular buffer with a given capacity.
    /*!
        \post <code>(*this).capacity() == capacity \&\& (*this).size == 0</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
    */
    explicit circular_buffer(
        size_type capacity,
        const allocator_type& alloc = allocator_type())
    : m_size(0), m_alloc(alloc) {
        initialize(capacity);
    }

    //! Create a full circular buffer with a given capacity and filled with copies of <code>item</code>.
    /*!
        \post <code>capacity() == n \&\& size() == n \&\& (*this)[0] == (*this)[1] == ... == (*this)[n - 1] == item</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
    */
    circular_buffer(
        size_type n,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : m_size(n), m_alloc(alloc) {
        initialize(n, item);
    }

	//! TODO doc
    circular_buffer(
		size_type capacity,
        size_type n,
        param_value_type item,
        const allocator_type& alloc = allocator_type())
    : m_size(n), m_alloc(alloc) {
		BOOST_CB_ASSERT(capacity >= size()); // check for capacity lower than size
        initialize(capacity, item);
    }

    //! Copy constructor.
    /*!
        \post <code>*this == cb</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
    */
    circular_buffer(const circular_buffer<T, Alloc>& cb)
    : m_size(cb.size()), m_alloc(cb.get_allocator()) {
        m_first = m_last = m_buff = allocate(cb.capacity());
        BOOST_TRY {
			m_end = cb_details::uninitialized_copy(cb.begin(), cb.end(), m_buff, m_alloc);
		} BOOST_CATCH(...) {
			deallocate(m_buff, cb.capacity());
			BOOST_RETHROW
		}
		BOOST_CATCH_END
    }

#if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

	// TODO doc
	template <class InputIterator>
    circular_buffer(
        InputIterator first,
        InputIterator last)
    : m_alloc(allocator_type()) {
		initialize(first, last, is_integral<InputIterator>());
    }

	// TODO doc
    template <class InputIterator>
    circular_buffer(
        size_type capacity,
        InputIterator first,
        InputIterator last)
    : m_alloc(allocator_type()) {
        initialize(capacity, first, last, is_integral<InputIterator>());
    }

#else

	//! TODO doc
	template <class InputIterator>
    circular_buffer(
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : m_alloc(alloc) {
		initialize(first, last, is_integral<InputIterator>());
    }

    //! Create a circular buffer with a copy of a range.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \post <code>(*this).capacity() == capacity</code><br>
              If the number of items to copy from the range
              <code>[first, last)</code> is greater than the specified
              <code>capacity</code> then only elements from the range
              <code>[last - capacity, last)</code> will be copied.
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
    */
    template <class InputIterator>
    circular_buffer(
        size_type capacity,
        InputIterator first,
        InputIterator last,
        const allocator_type& alloc = allocator_type())
    : m_alloc(alloc) {
        initialize(capacity, first, last, is_integral<InputIterator>());
    }

#endif // #if BOOST_WORKAROUND(BOOST_MSVC, <= 1200)

    //! Destructor.
    ~circular_buffer() { destroy(); }

public:
// Assign methods

    //! Assignment operator.
    /*!
        \post <code>*this == cb</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    circular_buffer<T, Alloc>& operator = (const circular_buffer<T, Alloc>& cb) {
        if (this == &cb)
            return *this;
        pointer buff = allocate(cb.capacity());
        BOOST_TRY {
			reset(buff, cb_details::uninitialized_copy(cb.begin(), cb.end(), buff, m_alloc), cb.capacity());
		} BOOST_CATCH(...) {
			deallocate(buff, cb.capacity());
			BOOST_RETHROW
		}
		BOOST_CATCH_END
        return *this;
    }

    //! Assign <code>n</code> items into the circular buffer.
    /*!
        \post <code>(*this).capacity() == n \&\& (*this).size() == n \&\&
              (*this)[0] == (*this)[1] == ... == (*this).back() == item</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void assign(size_type n, param_value_type item) {
		assign_n(n, n, cb_details::assign_n<param_value_type, allocator_type>(n, item, m_alloc));
    }

	//! TODO doc
	void assign(size_type capacity, size_type n, param_value_type item) {
	   BOOST_CB_ASSERT(capacity >= n); // check for new capacity lower than n
	   assign_n(capacity, n, cb_details::assign_n<param_value_type, allocator_type>(n, item, m_alloc));
    }

    //! Assign a copy of range.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \post <code>(*this).capacity() == std::distance(first, last) \&\&
		      (*this).size() == std::distance(first, last)</code>
        \throws "An allocation error" if memory is exhausted (<code>std::bad_alloc</code> if the standard allocator is used).
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        assign(first, last, is_integral<InputIterator>());
    }

	//! TODO doc
	template <class InputIterator>
	void assign(size_type capacity, InputIterator first, InputIterator last) {
	   assign(capacity, first, last, is_integral<InputIterator>());
    }

    //! Swap the contents of two circular buffers.
    /*!
        \post <code>this</code> contains elements of <code>cb</code> and vice versa.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void swap(circular_buffer<T, Alloc>& cb) {
		swap_allocator(cb, is_stateless<allocator_type>());
        std::swap(m_buff, cb.m_buff);
        std::swap(m_end, cb.m_end);
        std::swap(m_first, cb.m_first);
        std::swap(m_last, cb.m_last);
        std::swap(m_size, cb.m_size);
#if BOOST_CB_ENABLE_DEBUG
        invalidate_all_iterators();
        cb.invalidate_all_iterators();
#endif
    }

// push and pop

    //! Insert a new element at the end.
    /*!
        \post <code>(*this).back() == item</code><br>
              If the circular buffer is full, the first (leftmost) element will be removed.
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void push_back(param_value_type item = value_type()) {
        if (full()) {
            if (empty())
                return;
            replace(m_last, item);
            increment(m_last);
            m_first = m_last;
        } else {
            m_alloc.construct(m_last, item);
            increment(m_last);
            ++m_size;
        }
    }

    //! Insert a new element at the start.
    /*!
        \post <code>(*this).front() == item</code><br>
              If the circular buffer is full, the last (rightmost) element will be removed.
        \throws Whatever T::T(const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void push_front(param_value_type item = value_type()) {
        BOOST_TRY {
			if (full()) {
				if (empty())
					return;
				decrement(m_first);
				replace(m_first, item);
				m_last = m_first;
			} else {
				decrement(m_first);
				m_alloc.construct(m_first, item);
				++m_size;
			}
		} BOOST_CATCH(...) {
			increment(m_first);
			BOOST_RETHROW
		}
		BOOST_CATCH_END
    }

    //! Remove the last (rightmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
        \pre <code>iterator it = ((*this).end() - 1)</code>
        \post <code>((*this).end() - 1) != it</code>
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void pop_back() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (back element not available)
        decrement(m_last);
        destroy_item(m_last);
        --m_size;
    }

    //! Remove the first (leftmost) element.
    /*!
        \pre <code>!*(this).empty()</code>
        \pre <code>iterator it = (*this).begin()</code>
        \post <code>(*this).begin() != it</code>
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void pop_front() {
        BOOST_CB_ASSERT(!empty()); // check for empty buffer (front element not available)
        destroy_item(m_first);
        increment(m_first);
        --m_size;
    }

public:
// Insert

    //! Insert the <code>item</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post The <code>item</code> will be inserted at the position <code>pos</code>.<br>
              If the circular buffer is full, the first (leftmost) element will be removed.
        \return iterator to the inserted element.
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator insert(iterator pos, param_value_type item = value_type()) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        if (full() && pos == begin())
            return begin();
		return insert_item(pos, item);
    }

    //! Insert <code>n</code> copies of the item before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the beginning (left) of the circular buffer will be removed
              or not all <code>n</code> elements will be inserted or both.<code><br>
              Example:<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, (size_t)5, 6);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|6|6|6|6|6|3|4|)<br>
                RESULTING circular buffer |6|6|6|6|3|4| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void insert(iterator pos, size_type n, param_value_type item) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        if (n == 0)
            return;
        size_type copy = capacity() - (end() - pos);
        if (copy == 0)
            return;
        if (n > copy)
            n = copy;
        insert_n(pos, n, cb_details::item_wrapper<const_pointer, param_value_type>(item));
    }

    //! Insert the range <code>[first, last)</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator and valid range <code>[first, last)</code>.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the beginning (left) of the circular buffer will be removed
              or not the whole range will be inserted or both.
              In case the whole range cannot be inserted it will be inserted just
              some elements from the end (right) of the range (see the example).<code><br>
              Example:<br>
                array to insert: int array[] = { 5, 6, 7, 8, 9 };<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, array, array + 5);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|5|6|7|8|9|3|4|)<br>
                RESULTING circular buffer |6|7|8|9|3|4| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        insert(pos, first, last, is_integral<InputIterator>());
    }

    //! Insert an <code>item</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post The <code>item</code> will be inserted before the position <code>pos</code>.<br>
              If the circular buffer is full, the last element (rightmost) will be removed.
        \return iterator to the inserted element.
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator rinsert(iterator pos, param_value_type item = value_type()) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        if (full() && pos.m_it == 0)
            return end();
        if (pos == begin()) {
            BOOST_TRY {
				decrement(m_first);
				construct_or_replace(!full(), m_first, item);
			} BOOST_CATCH(...) {
				increment(m_first);
				BOOST_RETHROW
			}
			BOOST_CATCH_END
			pos.m_it = m_first;
        } else {
            pointer src = m_first;
            pointer dest = m_first;
            decrement(dest);
            pos.m_it = map_pointer(pos.m_it);
            bool construct = !full();
            BOOST_TRY {
				while (src != pos.m_it) {
					construct_or_replace(construct, dest, *src);
					increment(src);
					increment(dest);
					construct = false;
				}
				decrement(pos.m_it);
				replace(pos.m_it, item);
			} BOOST_CATCH(...) {
                if (!construct && !full()) {
                    decrement(m_first);
                    ++m_size;
                }
				BOOST_RETHROW
			}
			BOOST_CATCH_END
            decrement(m_first);
        }
        if (full())
            m_last = m_first;
        else
            ++m_size;
        return iterator(this, pos.m_it);
    }

    //! Insert <code>n</code> copies of the item before the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the end (right) of the circular buffer will be removed
              or not all <code>n</code> elements will be inserted or both.<code><br>
              Example:<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, (size_t)5, 6);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|6|6|6|6|6|3|4|)<br>
                RESULTING circular buffer |1|2|6|6|6|6| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void rinsert(iterator pos, size_type n, param_value_type item) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        rinsert_n(pos, n, cb_details::item_wrapper<const_pointer, param_value_type>(item));
    }

    //! Insert the range <code>[first, last)</code> before the given position.
    /*!
        \pre Valid <code>pos</code> iterator and valid range <code>[first, last)</code>.
        \post This operation preserves the capacity of the circular buffer.
              If the insertion would result in exceeding the capacity
              of the circular buffer then the necessary number of elements
              from the end (right) of the circular buffer will be removed
              or not the whole range will be inserted or both.
              In case the whole range cannot be inserted it will be inserted just
              some elements from the beginning (left) of the range (see the example).<code><br>
              Example:<br>
                array to insert: int array[] = { 5, 6, 7, 8, 9 };<br>
                original circular buffer |1|2|3|4| | | - capacity: 6, size: 4<br>
                position ---------------------^<br>
                insert(position, array, array + 5);<br>
                (If the operation won't preserve capacity, the buffer
                would look like this |1|2|5|6|7|8|9|3|4|)<br>
                RESULTING circular buffer |1|2|5|6|7|8| - capacity: 6, size: 6</code>
        \throws Whatever T::T(const T&) throws.
        \throws Whatever T::operator = (const T&) throws.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        rinsert(pos, first, last, is_integral<InputIterator>());
    }

// Erase

    //! Erase the element at the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - 1</code><br>
              Removes an element at the position <code>pos</code>.
        \return iterator to the first element remaining beyond the removed
                element or <code>(*this).end()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator erase(iterator pos) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(pos.m_it != 0);  // check for iterator pointing to end()
        pointer next = pos.m_it;
        increment(next);
        for (pointer p = pos.m_it; next != m_last; p = next, increment(next))
            replace(p, *next);
        decrement(m_last);
        destroy_item(m_last);
        --m_size;
#if BOOST_CB_ENABLE_DEBUG
        return m_last == pos.m_it ? end() : iterator(this, pos.m_it);
#else
        return m_last == pos.m_it ? end() : pos;
#endif
    }

    //! Erase the range <code>[first, last)</code>.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - std::distance(first, last)</code><br>
              Removes the elements from the range <code>[first, last)</code>.
        \return iterator to the first element remaining beyond the removed
                element or <code>(*this).end()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator erase(iterator first, iterator last) {
        BOOST_CB_ASSERT(first.is_valid());            // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(last.is_valid());             // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(first.m_buff == last.m_buff); // check for iterators of different containers
        BOOST_CB_ASSERT(first <= last);               // check for wrong range
        if (first == last)
            return first;
        pointer p = first.m_it;
        while (last.m_it != 0)
            replace((first++).m_it, *last++);
        do {
            decrement(m_last);
            destroy_item(m_last);
            --m_size;
        } while(m_last != first.m_it);
        return m_last == p ? end() : iterator(this, p);
    }

    //! Erase the element at the given position.
    /*!
        \pre Valid <code>pos</code> iterator.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - 1</code><br>
              Removes an element at the position <code>pos</code>.
        \return iterator to the first element remaining in front of the removed
                element or <code>(*this).begin()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator rerase(iterator pos) {
        BOOST_CB_ASSERT(pos.is_valid()); // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(pos.m_it != 0);  // check for iterator pointing to end()
        pointer prev = pos.m_it;
        pointer p = prev;
        for (decrement(prev); p != m_first; p = prev, decrement(prev))
            replace(p, *prev);
        destroy_item(m_first);
        increment(m_first);
        --m_size;
#if BOOST_CB_ENABLE_DEBUG
        return p == pos.m_it ? begin() : iterator(this, pos.m_it);
#else
        return p == pos.m_it ? begin() : pos;
#endif
    }

    //! Erase the range <code>[first, last)</code>.
    /*!
        \pre Valid range <code>[first, last)</code>.
        \pre <code>size_type old_size = (*this).size()</code>
        \post <code>(*this).size() == old_size - std::distance(first, last)</code><br>
              Removes the elements from the range <code>[first, last)</code>.
        \return iterator to the first element remaining in front of the removed
                element or <code>(*this).begin()</code> if no such element exists.
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    iterator rerase(iterator first, iterator last) {
        BOOST_CB_ASSERT(first.is_valid());            // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(last.is_valid());             // check for uninitialized or invalidated iterator
        BOOST_CB_ASSERT(first.m_buff == last.m_buff); // check for iterators of different containers
        BOOST_CB_ASSERT(first <= last);               // check for wrong range
        if (first == last)
            return first;
        pointer p = map_pointer(last.m_it);
        last.m_it = p;
        while (first.m_it != m_first) {
            decrement(first.m_it);
            decrement(p);
            replace(p, *first.m_it);
        }
        do {
            destroy_item(m_first);
            increment(m_first);
            --m_size;
        } while(m_first != p);
        if (m_first == last.m_it)
            return begin();
        decrement(last.m_it);
        return iterator(this, last.m_it);
    }

    //! Erase all stored elements.
    /*!
        \post (*this).size() == 0
        \note For iterator invalidation see the <a href="../circular_buffer.html#invalidation">documentation</a>.
    */
    void clear() {
        destroy_content();
        m_size = 0;
    }

private:
// Helper methods

    //! Check if the <code>index</code> is valid.
    void check_position(size_type index) const {
        if (index >= size())
            throw_exception(std::out_of_range("circular_buffer"));
    }

    //! Increment the pointer.
    template <class Pointer>
    void increment(Pointer& p) const {
        if (++p == m_end)
            p = m_buff;
    }

    //! Decrement the pointer.
    template <class Pointer>
    void decrement(Pointer& p) const {
        if (p == m_buff)
            p = m_end;
        --p;
    }

    //! Add <code>n</code> to the pointer.
    template <class Pointer>
    Pointer add(Pointer p, difference_type n) const {
        return p + (n < (m_end - p) ? n : n - capacity());
    }

    //! Subtract <code>n</code> from the pointer.
    template <class Pointer>
    Pointer sub(Pointer p, difference_type n) const {
        return p - (n > (p - m_buff) ? n - capacity() : n);
    }

    //! Map the null pointer to virtual end of circular buffer.
    pointer map_pointer(pointer p) const { return p == 0 ? m_last : p; }

    //! Allocate memory.
    pointer allocate(size_type n) {
        if (n > max_size())
            throw_exception(std::length_error("circular_buffer"));
#if BOOST_CB_ENABLE_DEBUG
        pointer p = (n == 0) ? 0 : m_alloc.allocate(n, 0);
        ::memset(p, cb_details::UNINITIALIZED, sizeof(value_type) * n);
        return p;
#else
        return (n == 0) ? 0 : m_alloc.allocate(n, 0);
#endif
    }

    //! Deallocate memory.
    void deallocate(pointer p, size_type n) {
        if (p != 0)
            m_alloc.deallocate(p, n);
    }

    //! Does the pointer point to the uninitialized memory?
    bool is_uninitialized(const_pointer p) const {
        return p >= m_last && (m_first < m_last || p < m_first);
    }

    //! Replace an element.
    void replace(pointer pos, param_value_type item) {
        *pos = item;
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators(iterator(this, pos));
#endif
    }

    //! Construct or replace an element.
    /*!
        <code>construct</code> has to be set to <code>true</code> if and only if
        <code>pos</code> points to an uninitialized memory.
    */
    void construct_or_replace(bool construct, pointer pos, param_value_type item) {
        if (construct)
            m_alloc.construct(pos, item);
        else
            replace(pos, item);
    }

    //! Destroy an item.
    void destroy_item(pointer p) {
        m_alloc.destroy(p);
#if BOOST_CB_ENABLE_DEBUG
        invalidate_iterators(iterator(this, p));
        ::memset(p, cb_details::UNINITIALIZED, sizeof(value_type));
#endif
    }

    //! Destroy an item only if it has been constructed.
    void destroy_if_constructed(pointer pos) {
        if (is_uninitialized(pos))
            destroy_item(pos);
    }

    //! Destroy the whole content of the circular buffer.
    void destroy_content() {
        for (size_type ii = 0; ii < size(); ++ii, increment(m_first))
            destroy_item(m_first);
    }

    //! Destroy content and free allocated memory.
    void destroy() {
        destroy_content();
        deallocate(m_buff, capacity());
#if BOOST_CB_ENABLE_DEBUG
        m_buff = 0;
        m_first = 0;
        m_last = 0;
        m_end = 0;
#endif
    }

	//! Initialize the circular buffer.
	void initialize(size_type capacity) {
		m_first = m_last = m_buff = allocate(capacity);
        m_end = m_buff + capacity;
	}

	//! Initialize the circular buffer.
	void initialize(size_type capacity, param_value_type item) {
		initialize(capacity);
		BOOST_TRY {
			cb_details::uninitialized_fill_n(m_buff, size(), item, m_alloc);
		} BOOST_CATCH(...) {
			deallocate(m_buff, size());
			BOOST_RETHROW
		}
		BOOST_CATCH_END
	}

	//! Specialized initialize method.
    template <class IntegralType>
    void initialize(IntegralType n, IntegralType item, const true_type&) {
		m_size = static_cast<size_type>(n);
        initialize(size(), item);
    }

    //! Specialized initialize method.
    template <class Iterator>
    void initialize(Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        initialize(first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }

	//! Specialized initialize method.
    template <class InputIterator>
    void initialize(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
		BOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS // check if the STL provides templated iterator constructors for containers
		std::deque<value_type, allocator_type> tmp(first, last, m_alloc);
		size_type distance = tmp.size();
		initialize(distance, tmp.begin(), tmp.end(), distance);
	}

	//! Specialized initialize method.
    template <class ForwardIterator>
    void initialize(ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
		size_type distance = std::distance(first, last);
        initialize(distance, first, last, distance);
    }

    //! Specialized initialize method.
    template <class IntegralType>
    void initialize(size_type capacity, IntegralType n, IntegralType item, const true_type&) {
        BOOST_CB_ASSERT(capacity >= static_cast<size_type>(n)); // check for capacity lower than n
		m_size = static_cast<size_type>(n);
        initialize(capacity, item);
    }

    //! Specialized initialize method.
    template <class Iterator>
    void initialize(size_type capacity, Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        initialize(capacity, first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }
    
    //! Specialized initialize method.
    template <class InputIterator>
    void initialize(size_type capacity,
        InputIterator first,
        InputIterator last,
        const std::input_iterator_tag&) {
        initialize(capacity);
        m_size = 0;
        if (capacity == 0)
            return;
        while (first != last && !full()) {
            m_alloc.construct(m_last, *first++);
            increment(m_last);
            ++m_size;
        }
        while (first != last) {
            replace(m_last, *first++);
            increment(m_last);
            m_first = m_last;
        }
    }

    //! Specialized initialize method.
    template <class ForwardIterator>
    void initialize(size_type capacity,
        ForwardIterator first,
        ForwardIterator last,
        const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        initialize(capacity, first, last, std::distance(first, last));
    }

    //! Helper initialize method.
    template <class ForwardIterator>
    void initialize(size_type capacity,
        ForwardIterator first,
        ForwardIterator last,
        size_type distance) {
		initialize(capacity);
        if (distance > capacity) {
            std::advance(first, distance - capacity);
            m_size = capacity;
        } else {
            m_size = distance;
            if (distance != capacity)
                m_last = m_buff + size();
        }
        BOOST_TRY {
			cb_details::uninitialized_copy(first, last, m_buff, m_alloc);
		} BOOST_CATCH(...) {
			deallocate(m_buff, capacity);
			BOOST_RETHROW
		}
		BOOST_CATCH_END
    }
    
	//! Increase the size of the circular buffer.
	void increase_size(size_type new_size, param_value_type item) {
		if (new_size > capacity())
            set_capacity(new_size);
        insert(end(), new_size - size(), item);
	}

	//! Reset the circular buffer.
	void reset(pointer buff, pointer last, size_type new_capacity) {
		destroy();
        m_size = last - buff;
        m_first = m_buff = buff;
        m_end = m_buff + new_capacity;
        m_last = last == m_end ? m_buff : last;
	}

	//! Specialized method for swapping the allocator.
	void swap_allocator(circular_buffer<T, Alloc>& cb, const true_type&) {
		// Swap is not needed because allocators have no state.
	}

	//! Specialized method for swapping the allocator.
	void swap_allocator(circular_buffer<T, Alloc>& cb, const false_type&) {
		std::swap(m_alloc, cb.m_alloc);
	}

    //! Specialized assign method.
    template <class IntegralType>
    void assign(IntegralType n, IntegralType item, const true_type&) {
		assign(static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized assign method.
    template <class Iterator>
    void assign(Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        assign(first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }
    
    //! Specialized assign method.
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        BOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS // check if the STL provides templated iterator constructors for containers
        std::deque<value_type, allocator_type> tmp(first, last, m_alloc);
        size_type distance = tmp.size();
        assign_n(distance, distance, cb_details::assign_range<BOOST_DEDUCED_TYPENAME std::deque<value_type, allocator_type>::iterator, allocator_type>(tmp.begin(), tmp.end(), m_alloc));
    }
    
    //! Specialized assign method.
    template <class ForwardIterator>
    void assign(ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        size_type distance = std::distance(first, last);
        assign_n(distance, distance, cb_details::assign_range<ForwardIterator, allocator_type>(first, last, m_alloc));
    }
    
    //! Specialized assign method.
    template <class IntegralType>
    void assign(size_type new_capacity, IntegralType n, IntegralType item, const true_type&) {
        assign(new_capacity, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized assign method.
    template <class Iterator>
    void assign(size_type new_capacity, Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        assign(new_capacity, first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }
    
    //! Specialized assign method.
    template <class InputIterator>
    void assign(size_type new_capacity, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
		if (new_capacity == capacity()) {
			clear();
			insert(begin(), first, last);
		} else {
			circular_buffer<value_type, allocator_type> tmp(new_capacity, first, last, m_alloc);
			tmp.swap(*this);
		}
    }
    
    //! Specialized assign method.
    template <class ForwardIterator>
    void assign(size_type new_capacity, ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        size_type distance = std::distance(first, last);
        if (distance > new_capacity) {
            std::advance(first, distance - capacity);
            distance = new_capacity;
        }
        assign_n(new_capacity, distance, cb_details::assign_range<ForwardIterator, allocator_type>(first, last, m_alloc));
    }

    //! Helper assign method.
    template <class Functor>
    void assign_n(size_type new_capacity, size_type n, const Functor& fnc) {
        if (new_capacity == capacity()) {
            destroy_content();
            BOOST_TRY {
				fnc(m_buff);
			} BOOST_CATCH(...) {
				m_size = 0;
				BOOST_RETHROW
			}
			BOOST_CATCH_END
        } else {
			pointer buff = allocate(new_capacity);
            BOOST_TRY {
				fnc(buff);
			} BOOST_CATCH(...) {
				deallocate(buff, new_capacity);
				BOOST_RETHROW
			}
			BOOST_CATCH_END
            destroy();
            m_buff = buff;
            m_end = m_buff + new_capacity;
        }
        m_size = n;
        m_first = m_buff;
        m_last = add(m_buff, size());
    }

	//! Helper insert method.
	iterator insert_item(const iterator& pos, param_value_type item) {
		pointer p = pos.m_it;
        if (p == 0) {
            construct_or_replace(!full(), m_last, item);
            p = m_last;
        } else {
            pointer src = m_last;
            pointer dest = m_last;
            bool construct = !full();
            BOOST_TRY {
				while (src != p) {
					decrement(src);
					construct_or_replace(construct, dest, *src);
					decrement(dest);
					construct = false;
				}
				replace(p, item);
			} BOOST_CATCH(...) {
				if (!construct && !full()) {
                    increment(m_last);
                    ++m_size;
                }
				BOOST_RETHROW
			}
			BOOST_CATCH_END
        }
        increment(m_last);
        if (full())
            m_first = m_last;
        else
            ++m_size;
        return iterator(this, p);
    }

    //! Specialized insert method.
    template <class IntegralType>
    void insert(const iterator& pos, IntegralType n, IntegralType item, const true_type&) {
        insert(pos, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized insert method.
    template <class Iterator>
    void insert(const iterator& pos, Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        insert(pos, first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }
    
    //! Specialized insert method.
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        if (!full() || pos != begin()) {
            for (;first != last; ++pos)
                pos = insert_item(pos, *first++);
        }
    }

    //! Specialized insert method.
    template <class ForwardIterator>
    void insert(const iterator& pos, ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        difference_type n = std::distance(first, last);
        if (n == 0)
            return;
        difference_type copy = capacity() - (end() - pos);
        if (copy == 0)
            return;
        if (n > copy) {
            std::advance(first, n - copy);
            n = copy;
        }
        insert_n(pos, n, cb_details::iterator_wrapper<ForwardIterator>(first));
    }
    
    //! Helper insert method.
    template <class Wrapper>
    void insert_n(const iterator& pos, size_type n, const Wrapper& wrapper) {
        size_type construct = capacity() - size();
        if (construct > n)
            construct = n;
        if (pos.m_it == 0) {
            size_type ii = 0;
            pointer p = m_last;
            BOOST_TRY {
				for (; ii < construct; ++ii, increment(p))
					m_alloc.construct(p, *wrapper());
				for (;ii < n; ++ii, increment(p))
					replace(p, *wrapper());
			} BOOST_CATCH(...) {
				size_type constructed = std::min(ii, construct);
                m_last = add(m_last, constructed);
                m_size += constructed;
				BOOST_RETHROW
			}
			BOOST_CATCH_END
        } else {
            pointer src = m_last;
            pointer dest = add(m_last, n - 1);
            pointer p = pos.m_it;
            size_type ii = 0;
            BOOST_TRY {
				while (src != pos.m_it) {
					decrement(src);
					construct_or_replace(is_uninitialized(dest), dest, *src);
					decrement(dest);
				}
				for (; ii < n; ++ii, increment(p))
					construct_or_replace(is_uninitialized(p), p, *wrapper());
			} BOOST_CATCH(...) {
				for (p = add(m_last, n - 1); p != dest; decrement(p))
                    destroy_if_constructed(p);
                for (n = 0, p = pos.m_it; n < ii; ++n, increment(p))
                    destroy_if_constructed(p);
				BOOST_RETHROW
			}
			BOOST_CATCH_END
        }
        m_last = add(m_last, n);
        m_first = add(m_first, n - construct);
        m_size += construct;
    }

    //! Specialized rinsert method.
    template <class IntegralType>
    void rinsert(const iterator& pos, IntegralType n, IntegralType item, const true_type&) {
        rinsert(pos, static_cast<size_type>(n), static_cast<value_type>(item));
    }

    //! Specialized rinsert method.
    template <class Iterator>
    void rinsert(const iterator& pos, Iterator first, Iterator last, const false_type&) {
        BOOST_CB_IS_CONVERTIBLE(Iterator, value_type); // check for invalid iterator type
        rinsert(pos, first, last, BOOST_DEDUCED_TYPENAME BOOST_ITERATOR_CATEGORY<Iterator>::type());
    }
    
    //! Specialized insert method.
    template <class InputIterator>
    void rinsert(iterator pos, InputIterator first, InputIterator last, const std::input_iterator_tag&) {
        if (!full() || pos.m_it != 0) {
            for (;first != last; ++pos) {
                pos = rinsert(pos, *first++);
				if (pos.m_it == 0)
					break;
			}
        }
    }

    //! Specialized rinsert method.
    template <class ForwardIterator>
    void rinsert(const iterator& pos, ForwardIterator first, ForwardIterator last, const std::forward_iterator_tag&) {
        BOOST_CB_ASSERT(std::distance(first, last) >= 0); // check for wrong range
        rinsert_n(pos, std::distance(first, last), cb_details::iterator_wrapper<ForwardIterator>(first));
    }

    //! Helper rinsert method.
    template <class Wrapper>
    void rinsert_n(const iterator& pos, size_type n, const Wrapper& wrapper) {
        if (n == 0)
            return;
        size_type copy = capacity() - (pos - begin());
        if (copy == 0)
            return;
        if (n > copy)
            n = copy;
        size_type construct = capacity() - size();
        if (construct > n)
            construct = n;
        if (pos == begin()) {
            pointer p = sub(m_first, n);
            size_type ii = n;
            BOOST_TRY {
				for (;ii > construct; --ii, increment(p))
					replace(p, *wrapper());
				for (; ii > 0; --ii, increment(p))
					m_alloc.construct(p, *wrapper());
			} BOOST_CATCH(...) {
				size_type constructed = ii < construct ? construct - ii : 0;
                m_last = add(m_last, constructed);
                m_size += constructed;
				BOOST_RETHROW
			}
			BOOST_CATCH_END
        } else {
            pointer src = m_first;
            pointer dest = sub(m_first, n);
            pointer p = map_pointer(pos.m_it);
            BOOST_TRY {
				while (src != p) {
					construct_or_replace(is_uninitialized(dest), dest, *src);
					increment(src);
					increment(dest);
				}
				for (size_type ii = 0; ii < n; ++ii, increment(dest))
					construct_or_replace(is_uninitialized(dest), dest, *wrapper());
			} BOOST_CATCH(...) {
				for (src = sub(m_first, n); src != dest; increment(src))
                    destroy_if_constructed(src);
				BOOST_RETHROW
			}
			BOOST_CATCH_END
        }
        m_first = sub(m_first, n);
        m_last = sub(m_last, n - construct);
        m_size += construct;
    }
};

// Non-member functions

//! Test two circular buffers for equality.
template <class T, class Alloc>
inline bool operator == (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return lhs.size() == rhs.size() &&
        std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator < (const circular_buffer<T, Alloc>& lhs,
                        const circular_buffer<T, Alloc>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || defined(BOOST_MSVC)

//! Test two circular buffers for non-equality.
template <class T, class Alloc>
inline bool operator != (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator > (const circular_buffer<T, Alloc>& lhs,
                        const circular_buffer<T, Alloc>& rhs) {
    return rhs < lhs;
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator <= (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

//! Lexicographical comparison.
template <class T, class Alloc>
inline bool operator >= (const circular_buffer<T, Alloc>& lhs,
                         const circular_buffer<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

//! Swap the contents of two circular buffers.
template <class T, class Alloc>
inline void swap(circular_buffer<T, Alloc>& lhs, circular_buffer<T, Alloc>& rhs) {
    lhs.swap(rhs);
}

#endif // #if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING) || defined(BOOST_MSVC)

} // namespace boost

#endif // #if !defined(BOOST_CIRCULAR_BUFFER_BASE_HPP)
