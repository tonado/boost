/* boost random/detail/seed.hpp header file
 *
 * Copyright Steven Watanabe 2009
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 */

#ifndef BOOST_RANDOM_DETAIL_SEED_IMPL_HPP
#define BOOST_RANDOM_DETAIL_SEED_IMPL_HPP

#include <stdexcept>
#include <boost/cstdint.hpp>
#include <boost/config/no_tr1/cmath.hpp>
#include <boost/integer/integer_mask.hpp>
#include <boost/integer/static_log2.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/random/detail/const_mod.hpp>
#include <boost/random/detail/integer_log2.hpp>
#include <boost/random/detail/signed_unsigned_tools.hpp>

namespace boost {
namespace random {
namespace detail {

template<class Engine, class Iter>
void generate_from_real(Engine& eng, Iter begin, Iter end)
{
    using std::ldexp;
    typedef typename Engine::result_type RealType;
    const int Bits = Engine::precision();
    int remaining_bits = 0;
    boost::uint_least32_t saved_bits = 0;
    RealType multiplier = ldexp(RealType(1), Bits);
    RealType mult32 = 4294967296.0; // 2^32
    while(true) {
        RealType val = eng() * multiplier;
        int available_bits = Bits;
        if(available_bits < 32 - remaining_bits) {
            saved_bits |= boost::uint_least32_t(val) << remaining_bits;
            remaining_bits += Bits;
        } else {
            if(remaining_bits != 0) {
                boost::uint_least32_t extra_bits = boost::uint_least32_t(val) & ((boost::uint_least32_t(1) << (32 - remaining_bits)) - 1);
                val = ldexp(val, -(32 - remaining_bits));
                *begin++ = saved_bits | (extra_bits << remaining_bits);
                if(begin == end) return;
                available_bits -= 32 - remaining_bits;
                remaining_bits = 0;
            }
            for(; available_bits >= 32; available_bits -= 32) {
                boost::uint_least32_t word = boost::uint_least32_t(val);
                val /= mult32;
                *begin++ = word;
                if(begin == end) return;
            }
            remaining_bits = available_bits;
            saved_bits = static_cast<boost::uint_least32_t>(val);
        }
    }
}

template<class Engine, class Iter>
void generate_from_int(Engine& eng, Iter begin, Iter end)
{
    typedef typename Engine::result_type IntType;
    typedef typename boost::make_unsigned<IntType>::type unsigned_type;
    int remaining_bits = 0;
    boost::uint_least32_t saved_bits = 0;
    unsigned_type range = boost::random::detail::subtract<IntType>()((eng.max)(), (eng.min)());

    int bits =
        (range == (std::numeric_limits<unsigned_type>::max)()) ?
            std::numeric_limits<unsigned_type>::digits :
            detail::integer_log2(range + 1);

    {
        int discarded_bits = detail::integer_log2(static_cast<unsigned_type>(bits));
        unsigned_type excess = (range + 1) >> (bits - discarded_bits);
        if(excess != 0) {
            int extra_bits = detail::integer_log2((excess - 1) ^ excess);
            bits = bits - discarded_bits + extra_bits;
        }
    }

    unsigned_type mask = (static_cast<unsigned_type>(2) << (bits - 1)) - 1;
    unsigned_type limit = ((range + 1) & ~mask) - 1;

    while(true) {
        unsigned_type val;
        do {
            val = boost::random::detail::subtract<IntType>()(eng(), (eng.min)());
        } while(val > limit);
        val &= mask;
        int available_bits = bits;
        if(available_bits < 32 - remaining_bits) {
            saved_bits |= boost::uint_least32_t(val) << remaining_bits;
            remaining_bits += bits;
        } else {
            if(remaining_bits != 0) {
                boost::uint_least32_t extra_bits = boost::uint_least32_t(val) & ((boost::uint_least32_t(1) << (32 - remaining_bits)) - 1);
                val >>= 32 - remaining_bits;
                *begin++ = saved_bits | (extra_bits << remaining_bits);
                if(begin == end) return;
                available_bits -= 32 - remaining_bits;
                remaining_bits = 0;
            }
            for(; available_bits >= 32; available_bits -= 32) {
                boost::uint_least32_t word = boost::uint_least32_t(val);
                val >>= 32;
                *begin++ = word;
                if(begin == end) return;
            }
            remaining_bits = available_bits;
            saved_bits = static_cast<boost::uint_least32_t>(val);
        }
    }
}

template<class Engine, class Iter>
void generate_impl(Engine& eng, Iter first, Iter last, boost::mpl::true_)
{
    return detail::generate_from_int(eng, first, last);
}

template<class Engine, class Iter>
void generate_impl(Engine& eng, Iter first, Iter last, boost::mpl::false_)
{
    return detail::generate_from_real(eng, first, last);
}

template<class Engine, class Iter>
void generate(Engine& eng, Iter first, Iter last)
{
    return detail::generate_impl(eng, first, last, boost::is_integral<typename Engine::result_type>());
}



template<class IntType, IntType m, class SeedSeq>
IntType seed_one_int(SeedSeq& seq)
{
    static const int log = ::boost::mpl::if_c<(m == 0),
        ::boost::mpl::int_<(::std::numeric_limits<IntType>::digits)>,
        ::boost::static_log2<m> >::type::value;
    static const int k =
        (log + ((~(static_cast<IntType>(2) << (log - 1)) & m)? 32 : 31)) / 32;
    ::boost::uint_least32_t array[log / 32 + 4];
    seq.generate(&array[0], &array[0] + k + 3);
    IntType s = 0;
    for(int j = 0; j < k; ++j) {
        IntType digit = const_mod<IntType, m>::apply(IntType(array[j+3]));
        IntType mult = IntType(1) << 32*j;
        s = const_mod<IntType, m>::mult_add(mult, digit, s);
    }
    return s;
}

template<class IntType, IntType m, class Iter>
IntType get_one_int(Iter& first, Iter last)
{
    static const int log = ::boost::mpl::if_c<(m == 0),
        ::boost::mpl::int_<(::std::numeric_limits<IntType>::digits)>,
        ::boost::static_log2<m> >::type::value;
    static const int k =
        (log + ((~(static_cast<IntType>(2) << (log - 1)) & m)? 32 : 31)) / 32;
    IntType s = 0;
    for(int j = 0; j < k; ++j) {
        if(first == last) {
            throw ::std::invalid_argument("Not enough elements in call to seed.");
        }
        IntType digit = const_mod<IntType, m>::apply(IntType(*first++));
        IntType mult = IntType(1) << 32*j;
        s = const_mod<IntType, m>::mult_add(mult, digit, s);
    }
    return s;
}

// TODO: work in-place whenever possible
template<int w, std::size_t n, class SeedSeq, class UIntType>
void seed_array_int_impl(SeedSeq& seq, UIntType (&x)[n])
{
    boost::uint_least32_t storage[((w+31)/32) * n];
    seq.generate(&storage[0], &storage[0] + ((w+31)/32) * n);
    for(std::size_t j = 0; j < n; j++) {
        UIntType val = 0;
        for(std::size_t k = 0; k < (w+31)/32; ++k) {
            val += static_cast<UIntType>(storage[(w+31)/32*j + k]) << 32*k;
        }
        x[j] = val & ::boost::low_bits_mask_t<w>::sig_bits;
    }
}

template<int w, std::size_t n, class SeedSeq, class IntType>
inline void seed_array_int_impl(SeedSeq& seq, IntType (&x)[n], boost::mpl::true_)
{
    typedef typename boost::make_unsigned<IntType>::type unsigned_array[n];
    seed_array_int_impl<w>(seq, reinterpret_cast<unsigned_array&>(x));
}

template<int w, std::size_t n, class SeedSeq, class IntType>
inline void seed_array_int_impl(SeedSeq& seq, IntType (&x)[n], boost::mpl::false_)
{
    seed_array_int_impl<w>(seq, x);
}

template<int w, std::size_t n, class SeedSeq, class IntType>
inline void seed_array_int(SeedSeq& seq, IntType (&x)[n])
{
    seed_array_int_impl<w>(seq, x, boost::is_signed<IntType>());
}

template<int w, std::size_t n, class Iter, class UIntType>
void fill_array_int_impl(Iter& first, Iter last, UIntType (&x)[n])
{
    for(std::size_t j = 0; j < n; j++) {
        UIntType val = 0;
        for(std::size_t k = 0; k < (w+31)/32; ++k) {
            if(first == last) {
                throw std::invalid_argument("Not enough elements in call to seed.");
            }
            val += static_cast<UIntType>(*first++) << 32*k;
        }
        x[j] = val & ::boost::low_bits_mask_t<w>::sig_bits;
    }
}

template<int w, std::size_t n, class Iter, class IntType>
inline void fill_array_int_impl(Iter& first, Iter last, IntType (&x)[n], boost::mpl::true_)
{
    typedef typename boost::make_unsigned<IntType>::type unsigned_array[n];
    fill_array_int_impl<w>(first, last, reinterpret_cast<unsigned_array&>(x));
}

template<int w, std::size_t n, class Iter, class IntType>
inline void fill_array_int_impl(Iter& first, Iter last, IntType (&x)[n], boost::mpl::false_)
{
    fill_array_int_impl<w>(first, last, x);
}

template<int w, std::size_t n, class Iter, class IntType>
inline void fill_array_int(Iter& first, Iter last, IntType (&x)[n])
{
    fill_array_int_impl<w>(first, last, x, boost::is_signed<IntType>());
}

template<int w, std::size_t n, class RealType>
void seed_array_real_impl(const boost::uint_least32_t* storage, RealType (&x)[n])
{
    using std::pow;
    boost::uint_least32_t mask = ~((~boost::uint_least32_t(0)) << (w%32));
    RealType two32 = 4294967296.0;
    const RealType divisor = pow(RealType(2), -w);
    unsigned int j;
    for(j = 0; j < n; ++j) {
        RealType val = RealType(0);
        RealType mult = divisor;
        for(int k = 0; k < w/32; ++k) {
            val += *storage++ * mult;
            mult *= two32;
        }
        if(mask != 0) {
            val += (*storage++ & mask) * mult;
        }
        BOOST_ASSERT(val >= 0);
        BOOST_ASSERT(val < 1);
        x[j] = val;
    }
}

template<int w, std::size_t n, class SeedSeq, class RealType>
void seed_array_real(SeedSeq& seq, RealType (&x)[n])
{
    using std::pow;
    boost::uint_least32_t storage[((w+31)/32) * n];
    seq.generate(&storage[0], &storage[0] + ((w+31)/32) * n);
    seed_array_real_impl<w>(storage, x);
}

template<int w, std::size_t n, class Iter, class RealType>
void fill_array_real(Iter& first, Iter last, RealType (&x)[n])
{
    using std::pow;
    boost::uint_least32_t mask = ~((~boost::uint_least32_t(0)) << (w%32));
    RealType two32 = 4294967296.0;
    const RealType divisor = pow(RealType(2), -w);
    unsigned int j;
    for(j = 0; j < n; ++j) {
        RealType val = RealType(0);
        RealType mult = divisor;
        for(int k = 0; k < w/32; ++k, ++first) {
            if(first == last) throw std::invalid_argument("Not enough elements in call to seed.");
            val += *first * mult;
            mult *= two32;
        }
        if(mask != 0) {
            if(first == last) throw std::invalid_argument("Not enough elements in call to seed.");
            val += (*first & mask) * mult;
            ++first;
        }
        BOOST_ASSERT(val >= 0);
        BOOST_ASSERT(val < 1);
        x[j] = val;
    }
}

}
}
}

#endif
