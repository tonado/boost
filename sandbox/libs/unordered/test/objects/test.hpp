
//  Copyright Daniel James 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_UNORDERED_TEST_OBJECTS_HEADER)
#define BOOST_UNORDERED_TEST_OBJECTS_HEADER

#include <cstddef>
#include "../helpers/fwd.hpp"
#include <ostream>

namespace test
{
    // Note that the default hash function will work for any equal_to (but not
    // very well).
    class object;
    class hash;
    class less;
    class equal_to;
    template <class T> class allocator;

    class object
    {
        friend class hash;
        friend class equal_to;
        friend class less;
        int tag1_, tag2_;
    public:
        explicit object(int t1 = 0, int t2 = 0) : tag1_(t1), tag2_(t2) {}

        friend bool operator==(object const& x1, object const& x2) {
            return x1.tag1_ == x2.tag1_ && x1.tag2_ == x2.tag2_;
        }

        friend bool operator!=(object const& x1, object const& x2) {
            return x1.tag1_ != x2.tag1_ || x1.tag2_ != x2.tag2_;
        }

        friend bool operator<(object const& x1, object const& x2) {
            return x1.tag1_ < x2.tag1_ ||
                (x1.tag1_ == x2.tag1_ && x1.tag2_ < x2.tag2_);
        }


        friend object generate(object const*) {
            int* x;
            return object(generate(x), generate(x));
        }

        friend std::ostream& operator<<(std::ostream& out, object const& o)
        {
            return out<<"("<<o.tag1_<<","<<o.tag2_<<")";
        }
    };

    class hash
    {
        int type_;
    public:
        explicit hash(int t = 0) : type_(t) {}

        std::size_t operator()(object const& x) const {
            switch(type_) {
            case 1:
                return x.tag1_;
            case 2:
                return x.tag2_;
            default:
                return x.tag1_ + x.tag2_; 
            }
        }

        std::size_t operator()(int x) const {
            return x;
        }

        friend bool operator==(hash const& x1, hash const& x2) {
            return x1.type_ == x2.type_;
        }

        friend bool operator!=(hash const& x1, hash const& x2) {
            return x1.type_ != x2.type_;
        }
    };

    class less
    {
        int type_;
    public:
        explicit less(int t = 0) : type_(t) {}

        bool operator()(object const& x1, object const& x2) const {
            switch(type_) {
            case 1:
                return x1.tag1_ < x2.tag1_;
            case 2:
                return x1.tag2_ < x2.tag2_;
            default:
                return x1 < x2;
            }
        }

        std::size_t operator()(int x1, int x2) const {
            return x1 < x2;
        }

        friend bool operator==(less const& x1, less const& x2) {
            return x1.type_ == x2.type_;
        }
    };

    class equal_to
    {
        int type_;
    public:
        explicit equal_to(int t = 0) : type_(t) {}

        bool operator()(object const& x1, object const& x2) const {
            switch(type_) {
            case 1:
                return x1.tag1_ == x2.tag1_;
            case 2:
                return x1.tag2_ == x2.tag2_;
            default:
                return x1 == x2;
            }
        }

        std::size_t operator()(int x1, int x2) const {
            return x1 == x2;
        }

        friend bool operator==(equal_to const& x1, equal_to const& x2) {
            return x1.type_ == x2.type_;
        }

        friend bool operator!=(equal_to const& x1, equal_to const& x2) {
            return x1.type_ != x2.type_;
        }

        friend less create_compare(equal_to x) {
            return less(x.type_);
        }
    };

    template <class T>
    class allocator
    {
# ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    public:
# else
        template <class> friend class allocator;
# endif
        int tag_;
    public:
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T const* const_pointer;
        typedef T& reference;
        typedef T const& const_reference;
        typedef T value_type;

        template <class U> struct rebind { typedef allocator<U> other; };

        explicit allocator(int t = 0) : tag_(t) {}
        template <class Y> allocator(allocator<Y> const& x) : tag_(x.tag_) {}
        allocator(allocator const& x) : tag_(x.tag_) {}
        ~allocator() {}

        pointer address(reference r) { return pointer(&r); }
        const_pointer address(const_reference r) { return const_pointer(&r); }

        pointer allocate(size_type n) {
            return pointer(static_cast<T*>(::operator new(n * sizeof(T))));
        }

        pointer allocate(size_type n, const_pointer u)
        {
            return pointer(static_cast<T*>(::operator new(n * sizeof(T))));
        }

        void deallocate(pointer p, size_type n)
        {
            ::operator delete((void*) p);
        }

        void construct(pointer p, T const& t) { new(p) T(t); }
        void destroy(pointer p) { p->~T(); }

        size_type max_size() const {
            return (std::numeric_limits<size_type>::max)();
        }

        friend bool operator==(allocator const& x, allocator const& y)
        {
            return x.tag_ == y.tag_;
        }

        friend bool operator!=(allocator const& x, allocator const& y)
        {
            return x.tag_ != y.tag_;
        }
    };
}

#endif
