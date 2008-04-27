/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2001-2008 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_REAL_IMPL_APR_18_2006_0901AM)
#define SPIRIT_REAL_IMPL_APR_18_2006_0901AM

#include <cmath>
#include <boost/type_traits/is_same.hpp>
#include <boost/spirit/home/support/unused.hpp>

namespace boost { namespace spirit { namespace qi  { namespace detail
{
    namespace
    {
        template <typename T>
        inline void
        scale_number(T const& exp, T& n)
        {
            using namespace std;    // allow for ADL to find the correct overload
            n *= pow(T(10), exp);
        }

        inline void
        scale_number(unused_type /*exp*/, unused_type /*n*/)
        {
            // no-op for unused_type
        }

        template <typename T>
        inline void
        scale_number(T const& exp, int frac, T& n)
        {
            scale_number(exp - T(frac), n);
        }

        inline void
        scale_number(unused_type /*exp*/, int /*frac*/, unused_type /*n*/)
        {
            // no-op for unused_type
        }

        template <typename T>
        inline T
        negate_number(bool neg, T const& n)
        {
            return neg ? -n : n;
        }

        inline unused_type
        negate_number(bool /*neg*/, unused_type n)
        {
            // no-op for unused_type
            return n;
        }
        
        template <typename T>
        inline bool
        number_equal_to_one(T const& value)
        {
            return value == 1.0;
        }

        inline bool
        number_equal_to_one(unused_type)
        {
            // no-op for unused_type
            return false;
        }
    }

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)  
# pragma warning(push)  
# pragma warning(disable: 4100)   // 'p': unreferenced formal parameter  
# pragma warning(disable: 4127)   // conditional expression is constant
#endif 

    template <typename T, typename RealPolicies>
    struct real_impl
    {
        template <typename Iterator, typename Attribute>
        static bool
        parse(Iterator& first, Iterator const& last, Attribute& attr, 
            RealPolicies const& p)
        {
            if (first == last)
                return false;
            Iterator save = first;

            // Start by parsing the sign. neg will be true if
            // we got a "-" sign, false otherwise.
            bool neg = p.parse_sign(first, last);

            // Now attempt to parse an integer
            Attribute n = 0;
            bool got_a_number = p.parse_n(first, last, n);

            // If we did not get a number it might be a NaN, Inf or a leading 
            // dot.
            if (!got_a_number)
            {
                // Check whether the number to parse is a NaN or Inf
                if (p.parse_nan(first, last, attr) ||
                    p.parse_inf(first, last, attr))
                {
                    // If we got a negative sign, negate the number
                    attr = negate_number(neg, attr);
                    return true;    // got a NaN or Inf, return early
                }
                
                // If we did not get a number and our policies do not
                // allow a leading dot, fail and return early (no-match)
                if (!p.allow_leading_dot)
                {
                    first = save;
                    return false;
                }
            }
            
            bool e_hit = false;
            int frac_digits = 0;

            // Try to parse the dot ('.' decimal point)
            if (p.parse_dot(first, last))
            {
                // We got the decimal point. Now we will try to parse
                // the fraction if it is there. If not, it defaults
                // to zero (0) only if we already got a number.
                Iterator savef = first;
                if (p.parse_frac_n(first, last, n))
                {
                    // Optimization note: don't compute frac_digits if T is
                    // an unused_type. This should be optimized away by the compiler.
                    if (!is_same<T, unused_type>::value)
                        frac_digits = 
                            static_cast<int>(std::distance(savef, first));
                }
                else if (!got_a_number || !p.allow_trailing_dot)
                {
                    // We did not get a fraction. If we still haven't got a
                    // number and our policies do not allow a trailing dot,
                    // return no-match.
                    first = save;
                    return false;
                }

                // Now, let's see if we can parse the exponent prefix
                e_hit = p.parse_exp(first, last);
            }
            else
            {
                // No dot and no number! Return no-match.
                if (!got_a_number)
                {
                    first = save;
                    return false;
                }

                // If we must expect a dot and we didn't see an exponent
                // prefix, return no-match.
                e_hit = p.parse_exp(first, last);
                if (p.expect_dot && !e_hit)
                {
                    first = save;
                    return false;
                }
            }

            if (e_hit)
            {
                // We got the exponent prefix. Now we will try to parse the
                // actual exponent. It is an error if it is not there.
                Attribute exp = 0;
                if (p.parse_exp_n(first, last, exp))
                {
                    // Got the exponent value. Scale the number by
                    // exp-frac_digits.
                    scale_number(exp, frac_digits, n);
                }
                else
                {
                    // Oops, no exponent, return no-match.
                    first = save;
                    return false;
                }
            }
            else if (frac_digits)
            {
                // No exponent found. Scale the number by -frac_digits.
                scale_number(Attribute(-frac_digits), n);
            }
            else if (number_equal_to_one(n))
            {
                // There is a chance of having to parse one of the 1.0#... 
                // styles some implementations use for representing NaN or Inf.

                // Check whether the number to parse is a NaN or Inf
                if (p.parse_nan(first, last, attr) ||
                    p.parse_inf(first, last, attr))
                {
                    // If we got a negative sign, negate the number
                    attr = negate_number(neg, attr);
                    return true;    // got a NaN or Inf, return immediately
                }
            }
            
            // If we got a negative sign, negate the number
            attr = negate_number(neg, n);

            // Success!!!
            return true;
        }
    };

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)  
# pragma warning(pop)  
#endif 

}}}}

#endif
