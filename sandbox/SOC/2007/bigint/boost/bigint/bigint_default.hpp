/* Boost bigint_default.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BIGINT_BIGINT_DEFAULT_HPP
#define BOOST_BIGINT_BIGINT_DEFAULT_HPP

#include <limits>

#include <boost/scoped_array.hpp>

#include <boost/bigint/bigint_util.hpp>

namespace boost { namespace detail {
	// Default implementation
	template <template <class> class Storage> struct bigint_default_implementation
	{
		typedef unsigned int limb_t;
		typedef boost::uint64_t limb2_t;

		enum { limb_bit_number = sizeof(limb_t) * 8 };
	#define limb_max std::numeric_limits<limb_t>::max()

		Storage<limb_t> data;
		bool negative;

		bigint_default_implementation(): negative(false)
		{
		}

		void assign(int number)
		{
			assign(static_cast<int64_t>(number));
		}

		void assign(unsigned int number)
		{
			assign(static_cast<uint64_t>(number));
		}

		void assign(int64_t number)
		{
			// number is [-2^32, 2^32-1]
			// if number == -2^32, it's bit representation is 10...0, -number is 01...1+1 = 10...0 (the same)
			// converting to uint64_t yields still 10...0, it's exactly 2^32. In other cases we're safe.
			assign(static_cast<uint64_t>(number >= 0 ? number : -number));
			
			negative = (number < 0);
		}

		void assign(uint64_t number)
		{
			size_t size = 0;
			
			if (number != 0)
			{
				data.resize(1);
				data[0] = static_cast<limb_t>(number & limb_max);

				size = 1;
			}

			if (number > limb_max)
			{
				data.resize(64 / limb_bit_number); // we know that limb_bit_number is 2^n
				
				number >>= limb_bit_number;
				
				while (number > 0)
				{
					data[size++] = static_cast<limb_t>(number & limb_max);
					number >>= limb_bit_number;
				}
			}

			data.resize(size);
			negative = false;
		}
		
		// *this = *this * a + b
		void _mul_add(limb_t a, limb_t b)
		{
			limb_t carry = b;

			for (limb_t* i = data.begin(); i != data.end(); ++i)
			{
				limb2_t result = static_cast<limb2_t>(*i) * a + carry;

				*i = static_cast<limb_t>(result & limb_max);

				carry = static_cast<limb_t>(result >> limb_bit_number);
			}

			if (carry != 0)
			{
				data.resize(data.size() + 1);
				data[data.size()-1] = carry;
			}
		}

		template <typename Ch> void _assign_str(const Ch* str, int base)
		{
			assert(base >= 2 && base <= 36);
			
			// skip whitespace
			while (detail::bigint::isspace(*str)) ++str;

			negative = false;

			if (*str == Ch('-'))
			{
				negative = true;
				++str;
			}
			else if (*str == Ch('+'))
			{
				++str;
			}

			static const unsigned char digit_value_tab[] =
			{
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
				25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
				25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   0xff, 0xff, 0xff, 0xff, 0xff,
			};
			
			// skip zeros
			while (*str == Ch('0')) ++str;
			
			// is there anything left?
			if (!*str)
			{
				assign(0);
				return;
			}

			data.resize(0);
			
			for (; *str; ++str)
			{
				if (!detail::bigint::is_ascii(*str) || digit_value_tab[static_cast<unsigned int>(*str)] >= base)
				{
					break;
				}
				
				_mul_add(static_cast<limb_t>(base), digit_value_tab[static_cast<unsigned int>(*str)]);
			}
		}

		void assign(const char* str, int base)
		{
			_assign_str(str, base);
		}
		
		void assign(const wchar_t* str, int base)
		{
			_assign_str(str, base);
		}

		void _add_unsigned(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			limb_t carry = 0;
			
			size_t li_size = lhs.data.size();
			size_t ri_size = rhs.data.size();
			
			data.resize((std::max)(lhs.data.size(), rhs.data.size()) + 1);

			const limb_t* li = lhs.data.begin();
			const limb_t* li_end = li + li_size;
			const limb_t* ri = rhs.data.begin();
			const limb_t* ri_end = ri + ri_size;
			
			limb_t* i = data.begin();

			for (; li != li_end && ri != ri_end; ++li, ++ri)
			{
				limb2_t result = static_cast<limb2_t>(*li) + *ri + carry;

				*i++ = static_cast<limb_t>(result & limb_max);

				carry = static_cast<limb_t>(result >> limb_bit_number);
			}

			for (; li != li_end; ++li)
			{
				limb2_t result = static_cast<limb2_t>(*li) + carry;

				*i++ = static_cast<limb_t>(result & limb_max);

				carry = static_cast<limb_t>(result >> limb_bit_number);
			}

			for (; ri != ri_end; ++ri)
			{
				limb2_t result = static_cast<limb2_t>(*ri) + carry;

				*i++ = static_cast<limb_t>(result & limb_max);

				carry = static_cast<limb_t>(result >> limb_bit_number);
			}

			if (carry != 0)
			{
				*i = carry;
			}
			else
			{
				data.resize(data.size() - 1);
			}
		}

		void _normalize()
		{
			if (data.empty()) return;
			
			// strip zeroes
			const limb_t* i = data.end();

			do
			{
				--i;
			}
			while (i != data.begin() && *i == 0);

			if (i == data.begin() && *i == 0)
			{
				data.resize(0);
				negative = false;
			}
			else
			{
				data.resize((i - data.begin()) + 1);
			}
		}

		bool _sub_unsigned(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			limb_t borrow = 0;
			
			size_t li_size = lhs.data.size();
			size_t ri_size = rhs.data.size();
			
			data.resize((std::max)(lhs.data.size(), rhs.data.size()));

			const limb_t* li = lhs.data.begin();
			const limb_t* li_end = li + li_size;
			const limb_t* ri = rhs.data.begin();
			const limb_t* ri_end = ri + ri_size;

			limb_t* i = data.begin();

			for (; li != li_end && ri != ri_end; ++li, ++ri)
			{
				limb2_t result = static_cast<limb2_t>(*ri) + borrow;

				if (result > *li)
				{
					result = static_cast<limb2_t>(limb_max) + 1 + *li - result;
					borrow = 1;
				}
				else
				{
					result = *li - result;
					borrow = 0;
				}

				*i++ = static_cast<limb_t>(result & limb_max);
			}

			for (; li != li_end; ++li)
			{
				limb2_t result = borrow;

				if (result > *li)
				{
					result = static_cast<limb2_t>(limb_max) + 1 + *li - result;
					borrow = 1;
				}
				else
				{
					result = *li - result;
					borrow = 0;
				}

				*i++ = static_cast<limb_t>(result & limb_max);
			}

			for (; ri != ri_end; ++ri)
			{
				limb2_t result = static_cast<limb2_t>(*ri) + borrow;

				if (result > 0)
				{
					result = static_cast<limb2_t>(limb_max) + 1 - result;
					borrow = 1;
				}
				else
				{
					borrow = 0;
				}

				*i++ = static_cast<limb_t>(result & limb_max);
			}

			if (borrow != 0)
			{
				// we borrowed 2^number of bits in our number - we have to subtract it
				// for this we need to complement all limbs to 2, and add 1 to the last limb.
				for (limb_t* j = data.begin(); j != data.end(); ++j)
					*j = limb_max- *j;
			
				data[0]++;
			}
			
			_normalize();

			return borrow != 0;
		}

		void add(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			if (lhs.negative == rhs.negative) // positive + positive or negative + negative
			{
				negative = lhs.negative;
				_add_unsigned(lhs, rhs);
			}
			else if (lhs.negative) // negative + positive
			{
				negative = _sub_unsigned(rhs, lhs);
			}
			else // positive + negative
			{
				negative = _sub_unsigned(lhs, rhs);
			}
		}

		void sub(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			if (lhs.negative != rhs.negative) // positive - negative or negative - positive
			{
				negative = lhs.negative;
				_add_unsigned(lhs, rhs);
			}
			else if (lhs.negative) // negative - negative
			{
				negative = _sub_unsigned(rhs, lhs);
			}
			else // positive - positive
			{
				negative = _sub_unsigned(lhs, rhs);
			}
		}

		void mul(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			if (lhs.is_zero() || rhs.is_zero())
			{
				assign(0);
				return;
			}

			if (this == &lhs || this == &rhs)
			{
				bigint_default_implementation copy;
				copy.mul(lhs, rhs);
				*this = copy;
				return;
			}

			data.resize(lhs.data.size() + rhs.data.size());
			std::fill(data.begin(), data.end(), 0);

			limb_t* i = data.begin();
			
			for (const limb_t* li = lhs.data.begin(); li != lhs.data.end(); ++li, ++i)
			{
				limb_t carry = 0;

				limb_t* ci = i;

				for (const limb_t* ri = rhs.data.begin(); ri != rhs.data.end(); ++ri)
				{
					limb2_t result = static_cast<limb2_t>(*li) * *ri + *ci + carry;

					*ci++ = static_cast<limb_t>(result & limb_max);

					carry = static_cast<limb_t>(result >> limb_bit_number);
				}

				while (carry != 0)
				{
					limb2_t result = static_cast<limb2_t>(*ci) + carry;
					
					*ci++ = static_cast<limb_t>(result & limb_max);

					carry = static_cast<limb_t>(result >> limb_bit_number);
				}
			}

			_normalize();
			
			negative = lhs.negative ? !rhs.negative : rhs.negative;
		}

		void div(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
		}

		void mod(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
		}

		template <bool complement> limb_t _convert(limb_t limb)
		{
			return complement ? limb_max - limb : limb;
		}
		
		template <bool complement> limb_t _convert_first(limb_t limb)
		{
			return complement ? limb_max - limb + 1 : limb;
		}

		template <bool lhs_neg, bool rhs_neg> void _or_(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			const bool neg = lhs_neg || rhs_neg; // sign bit is or-ed
			negative = neg;

			size_t li_size = lhs.data.size();
			size_t ri_size = rhs.data.size();
			
			data.resize((std::max)(lhs.data.size(), rhs.data.size()));

			const limb_t* li = lhs.data.begin();
			const limb_t* li_end = li + li_size;
			const limb_t* ri = rhs.data.begin();
			const limb_t* ri_end = ri + ri_size;
			
			limb_t* i = data.begin();

			if (li != li_end && ri != ri_end)
			{
				*i++ = _convert_first<neg>(_convert_first<lhs_neg>(*li) | _convert_first<rhs_neg>(*ri));
				++li;
				++ri;
			}

			for (; li != li_end && ri != ri_end; ++li, ++ri)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(*li) | _convert<rhs_neg>(*ri));
			}

			for (; li != li_end; ++li)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(*li) | _convert<rhs_neg>(0)); // or with rhs sign bit
			}

			for (; ri != ri_end; ++ri)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(0) | _convert<rhs_neg>(*ri)); // or with lhs sign bit
			}

			_normalize();
		}

		void or_(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			if (lhs.negative)
				rhs.negative ? _or_<true, true>(lhs, rhs) : _or_<true, false>(lhs, rhs);
			else
				rhs.negative ? _or_<false, true>(lhs, rhs) : _or_<false, false>(lhs, rhs);
		}

		template <bool lhs_neg, bool rhs_neg> void _and_(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			const bool neg = lhs_neg && rhs_neg; // sign bit is and-ed
			negative = neg;

			size_t li_size = lhs.data.size();
			size_t ri_size = rhs.data.size();
			
			data.resize((std::max)(lhs.data.size(), rhs.data.size()));

			const limb_t* li = lhs.data.begin();
			const limb_t* li_end = li + li_size;
			const limb_t* ri = rhs.data.begin();
			const limb_t* ri_end = ri + ri_size;
			
			limb_t* i = data.begin();

			if (li != li_end && ri != ri_end)
			{
				*i++ = _convert_first<neg>(_convert_first<lhs_neg>(*li) & _convert_first<rhs_neg>(*ri));
				++li;
				++ri;
			}

			for (; li != li_end && ri != ri_end; ++li, ++ri)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(*li) & _convert<rhs_neg>(*ri));
			}

			for (; li != li_end; ++li)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(*li) & _convert<rhs_neg>(0)); // and with rhs sign bit
			}

			for (; ri != ri_end; ++ri)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(0) & _convert<rhs_neg>(*ri)); // and with lhs sign bit
			}

			_normalize();
		}

		void and_(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			if (lhs.negative)
				rhs.negative ? _and_<true, true>(lhs, rhs) : _and_<true, false>(lhs, rhs);
			else
				rhs.negative ? _and_<false, true>(lhs, rhs) : _and_<false, false>(lhs, rhs);
		}

		template <bool lhs_neg, bool rhs_neg> void _xor_(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			const bool neg = lhs_neg ? !rhs_neg : rhs_neg; // sign bit is xor-ed
			negative = neg;

			size_t li_size = lhs.data.size();
			size_t ri_size = rhs.data.size();
			
			data.resize((std::max)(lhs.data.size(), rhs.data.size()));

			const limb_t* li = lhs.data.begin();
			const limb_t* li_end = li + li_size;
			const limb_t* ri = rhs.data.begin();
			const limb_t* ri_end = ri + ri_size;
			
			limb_t* i = data.begin();

			if (li != li_end && ri != ri_end)
			{
				*i++ = _convert_first<neg>(_convert_first<lhs_neg>(*li) ^ _convert_first<rhs_neg>(*ri));
				++li;
				++ri;
			}

			for (; li != li_end && ri != ri_end; ++li, ++ri)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(*li) ^ _convert<rhs_neg>(*ri));
			}

			for (; li != li_end; ++li)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(*li) ^ _convert<rhs_neg>(0)); // xor with rhs sign bit
			}

			for (; ri != ri_end; ++ri)
			{
				*i++ = _convert<neg>(_convert<lhs_neg>(0) ^ _convert<rhs_neg>(*ri)); // xor with lhs sign bit
			}

			_normalize();
		}

		void xor_(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs)
		{
			if (lhs.negative)
				rhs.negative ? _xor_<true, true>(lhs, rhs) : _xor_<true, false>(lhs, rhs);
			else
				rhs.negative ? _xor_<false, true>(lhs, rhs) : _xor_<false, false>(lhs, rhs);
		}

		void not_(const bigint_default_implementation& lhs)
		{
			// ~value == -(value + 1) == -value-1
			negate(lhs);
			dec();
		}

		void negate(const bigint_default_implementation& lhs)
		{
			data = lhs.data;
			negative = !lhs.negative;
			if (data.empty()) negative = false;
		}

		void lshift(const bigint_default_implementation& lhs, boost::uint64_t rhs)
		{
		}

		void rshift(const bigint_default_implementation& lhs, boost::uint64_t rhs)
		{
		}

		void inc()
		{
			bigint_default_implementation one;
			one.assign(1);

			add(*this, one);
		}

		void dec()
		{
			bigint_default_implementation one;
			one.assign(1);

			sub(*this, one);
		}
		
		int compare(const bigint_default_implementation& rhs) const
		{
			if (negative != rhs.negative) return negative > rhs.negative ? -1 : 1; 
			
			int result = negative ? -1 : 1;

			if (data.size() != rhs.data.size()) return result * (data.size() < rhs.data.size() ? -1 : 1);
			if (data.empty()) return 0;

			const limb_t* li = data.end();
			const limb_t* ri = rhs.data.end();

			do
			{
				--li; --ri;

				if (*li < *ri)
				{
					return -result;
				}
				else if (*li > *ri)
				{
					return result;
				}
			}
			while (li != data.begin());

			return 0;
		}

		// *this = *this / a, return division remainder
		limb_t _div_rem(limb_t a)
		{
			if (data.empty()) return 0;

			limb_t remainder = 0;

			limb_t* i = data.end();
			
			do
			{
				--i;

				limb2_t result = (static_cast<limb2_t>(remainder) << limb_bit_number) + *i;

				*i = static_cast<limb_t>(result / a);

				remainder = static_cast<limb_t>(result % a);
			}
			while (i != data.begin());

			if (*(data.end() - 1) == 0) data.resize(data.size() - 1);

			return remainder;
		}

		template <typename Ch> std::basic_string<Ch> _to_str(int base) const
		{
			assert(base >= 2 && base <= 36);

			if (data.empty()) return std::basic_string<Ch>(1, Ch('0'));

			std::basic_string<Ch> result;

			bigint_default_implementation copy = *this;

			static const Ch digit_char_tab[] =
			{
				Ch('0'), Ch('1'), Ch('2'), Ch('3'), Ch('4'), Ch('5'), Ch('6'), Ch('7'), Ch('8'), Ch('9'), 
				Ch('a'), Ch('b'), Ch('c'), Ch('d'), Ch('e'), Ch('f'), Ch('g'), Ch('h'), Ch('i'), Ch('j'), 
				Ch('k'), Ch('l'), Ch('m'), Ch('n'), Ch('o'), Ch('p'), Ch('q'), Ch('r'), Ch('s'), Ch('t'), 
				Ch('u'), Ch('v'), Ch('w'), Ch('x'), Ch('y'), Ch('z')
			};

			while (!copy.data.empty())
			{
				result += digit_char_tab[copy._div_rem(static_cast<limb_t>(base))];
			}

			if (negative) result += '-';

			std::reverse(result.begin(), result.end());

			return result;
		}

		std::string str(int base) const
		{
			return _to_str<char>(base);
		}
		
		std::wstring wstr(int base) const
		{
			return _to_str<wchar_t>(base);
		}
		
		boost::uint64_t _to_uint64() const
		{
			boost::uint64_t value = 0;
			boost::uint64_t power = 1;

			for (const limb_t* i = data.begin(); i != data.end(); ++i)
			{
				value += *i * power;
				power <<= limb_bit_number;
			}
			
			return value;
		}

		template <typename T> bool can_convert_to() const
		{
			// Only integer types supported
			if (!std::numeric_limits<T>::is_integer) return false;
			
			boost::uint64_t max_value;
			
			size_t count = data.size();
			
			if (negative)
			{
				max_value = static_cast<boost::uint64_t>(-static_cast<boost::int64_t>((std::numeric_limits<T>::min)()));
			}
			else
			{
				max_value = (std::numeric_limits<T>::max)();
			}

			if (count * limb_bit_number > sizeof(boost::uint64_t) * 8) // we can't fit in uint64 => we won't fit in anything else
				return false;

			return max_value >= _to_uint64();
		}
		
		template <typename T> T to_number() const
		{
			if (!std::numeric_limits<T>::is_integer) return T();
			
			boost::uint64_t value = _to_uint64();
						
			return negative ? static_cast<T>(-static_cast<boost::int64_t>(value)) : static_cast<T>(value);
		}

		bool is_zero() const
		{
			return data.empty();
		}
		
		void abs(const bigint_default_implementation& rhs)
		{
			data = rhs.data;
			negative = false;
		}
		
		void pow(const bigint_default_implementation& lhs, boost::uint64_t rhs)
		{
		}
		
		void div(const bigint_default_implementation& lhs, const bigint_default_implementation& rhs, bigint_default_implementation& remainder)
		{
		}
		
		void sqrt(const bigint_default_implementation& lhs)
		{
		}
	};
} }  // namespace boost::detail

#endif // BOOST_BIGINT_BIGINT_DEFAULT_HPP
