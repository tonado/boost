
//  Copyright Daniel James 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_UNORDERED_TEST_HELPERS_HEADER)
#define BOOST_UNORDERED_TEST_HELPERS_HEADER

namespace test
{
    template <class Container>
    struct get_key_impl
    {
        typedef typename Container::key_type key_type;

        static key_type get_key(key_type const& x)
        {
            return x;
        }

        template <class T>
        static key_type get_key(std::pair<key_type, T> const& x, char = 0)
        {
            return x.first;
        }

        template <class T>
        static key_type get_key(std::pair<key_type const, T> const& x, unsigned char = 0)
        {
            return x.first;
        }
    };
    
    template <class Container, class T>
    inline typename Container::key_type get_key(T const& x)
    {
        return get_key_impl<Container>::get_key(x);
    }
}

#endif
