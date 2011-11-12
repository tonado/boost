//
//  (C) Copyright Howard Hinnant
//  (C) Copyright 2011 Vicente J. Botet Escriba
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//

#ifndef BOOST_CHRONO_IO_DURATION_GET_HPP
#define BOOST_CHRONO_IO_DURATION_GET_HPP

#include <boost/chrono/config.hpp>
#include <string>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/mpl/if.hpp>
#include <boost/math/common_factor_rt.hpp>
#include <boost/chrono/detail/scan_keyword.hpp>
#include <boost/chrono/detail/no_warning/signed_unsigned_cmp.hpp>

#include <boost/assert.hpp>
#include <locale>

/**
 * Duration formatting facet for input.
 */
namespace boost
{
  namespace chrono
  {

    namespace detail
    {
      template <class Rep, bool = is_scalar<Rep>::value>
      struct duration_io_intermediate
      {
        typedef Rep type;
      };

      template <class Rep>
      struct duration_io_intermediate<Rep, true>
      {
        typedef typename mpl::if_c<is_floating_point<Rep>::value, long double, typename mpl::if_c<
            is_signed<Rep>::value, long long, unsigned long long>::type>::type type;
      };

      template <typename intermediate_type>
      typename enable_if<is_integral<intermediate_type> , bool>::type reduce(intermediate_type& r,
          unsigned long long& den, std::ios_base::iostate& err)
      {
        typedef typename common_type<intermediate_type, unsigned long long>::type common_type_t;

        // Reduce r * num / den
        common_type_t t = math::gcd<common_type_t>(common_type_t(r), common_type_t(den));
        r /= t;
        den /= t;
        if (den != 1)
        {
          // Conversion to Period is integral and not exact
          err |= std::ios_base::failbit;
          return false;
        }
        return true;
      }
      template <typename intermediate_type>
      typename disable_if<is_integral<intermediate_type> , bool>::type reduce(intermediate_type&, unsigned long long&,
          std::ios_base::iostate&)
      {
        return true;
      }

    }

    /**
     * @c duration_get is used to parse a character sequence, extracting
     * components of a duration into a class duration.
     * Each get member parses a format as produced by a corresponding format specifier to time_put<>::put.
     * If the sequence being parsed matches the correct format, the
     * corresponding member of the class duration argument are set to the
     * value used to produce the sequence;
     * otherwise either an error is reported or unspecified values are assigned.
     * In other words, user confirmation is required for reliable parsing of
     * user-entered durations, but machine-generated formats can be parsed
     * reliably. This allows parsers to be aggressive about interpreting user
     * variations on standard formats.
     *
     * If the end iterator is reached during parsing of the get() member
     * function, the member sets std::ios_base::eofbit in err.
     */
    template <class CharT, class InputIterator = std::istreambuf_iterator<CharT> >
    class duration_get: public std::locale::facet
    {
    public:
      /**
       * Type of character the facet is instantiated on.
       */
      typedef CharT char_type;
      typedef std::basic_string<CharT> string_type;
      /**
       * Type of iterator used to scan the character buffer.
       */
      typedef InputIterator iter_type;

      /**
       * Construct a @c duration_get facet.
       * @param refs
       * @Effects Construct a @c duration_get facet.
       * If the @c refs argument is @c 0 then destruction of the object is
       * delegated to the @c locale, or locales, containing it. This allows
       * the user to ignore lifetime management issues. On the other had,
       * if @c refs is @c 1 then the object must be explicitly deleted;
       * the @c locale will not do so. In this case, the object can be
       * maintained across the lifetime of multiple locales.
       */

      explicit duration_get(size_t refs = 0) :
        std::locale::facet(refs)
      {
      }

      /**
       * @param s start input stream iterator
       * @param end end input stream iterator
       * @param ios a reference to a ios_base
       * @param err the ios_base state
       * @param d the duration
       * @param pattern begin of the formatting pattern
       * @param pat_end end of the formatting pattern
       *
       * Requires: [pattern,pat_end) shall be a valid range.
       *
       * Effects: The function starts by evaluating err = std::ios_base::goodbit.
       * It then enters a loop, reading zero or more characters from s at
       * each iteration. Unless otherwise specified below, the loop
       * terminates when the first of the following conditions holds:
       * - The expression pattern == pat_end evaluates to true.
       * - The expression err == std::ios_base::goodbit evaluates to false.
       * - The expression s == end evaluates to true, in which case the
       * function evaluates err = std::ios_base::eofbit | std::ios_base::failbit.
       * - The next element of pattern is equal to �%�, followed by a conversion
       * specifier character, format.
       * If the number of elements in the range [pattern,pat_end) is not
       * sufficient to unambiguously determine whether the conversion
       * specification is complete and valid, the function evaluates
       * err = std::ios_base::failbit. Otherwise, the function evaluates
       * s = do_get(s, end, ios, err, d). If err == std::ios_base::goodbit holds after
       * the evaluation of the expression, the function increments pattern to
       * point just past the end of the conversion specification and continues
       * looping.
       * - The expression isspace(*pattern, ios.getloc()) evaluates to true, in
       * which case the function first increments pattern until
       * pattern == pat_end || !isspace(*pattern, ios.getloc()) evaluates to true,
       * then advances s until s == end || !isspace(*s, ios.getloc()) is true,
       * and finally resumes looping.
       * - The next character read from s matches the element pointed to by
       * pattern in a case-insensitive comparison, in which case the function
       * evaluates ++pattern, ++s and continues looping. Otherwise, the function
       * evaluates err = std::ios_base::failbit.
       *
       * Returns: s
       */
      template <typename Rep, typename Period>
      iter_type get(iter_type s, iter_type end, std::ios_base& ios, std::ios_base::iostate& err,
          duration<Rep, Period> &d, const char_type *pattern, const char_type *pat_end) const
      {
        if (std::has_facet<duration_units<CharT> >(ios.getloc()))
        {
          duration_units<CharT> const&facet = std::use_facet<duration_units<CharT> >(ios.getloc());
          return get(facet, s, end, ios, err, d, pattern, pat_end);
        }
        else
        {
          duration_units_default<CharT> facet;
          return get(facet, s, end, ios, err, d, pattern, pat_end);
        }
      }

      template <typename Rep, typename Period>
      iter_type get(duration_units<CharT> const&facet, iter_type s, iter_type end, std::ios_base& ios,
          std::ios_base::iostate& err, duration<Rep, Period> &d, const char_type *pattern, const char_type *pat_end) const
      {

        typedef typename detail::duration_io_intermediate<Rep>::type intermediate_type;
        intermediate_type r;
        detail::rt_ratio rt;
        bool value_found = false, unit_found = false;
        //bool loc_found=false;

        const std::ctype<char_type>& ct = std::use_facet<std::ctype<char_type> >(ios.getloc());
        err = std::ios_base::goodbit;
        while (pattern != pat_end && err == std::ios_base::goodbit)
        {
          if (s == end)
          {
            err |= std::ios_base::eofbit;
            break;
          }
          if (ct.narrow(*pattern, 0) == '%')
          {
            if (++pattern == pat_end)
            {
              err |= std::ios_base::failbit;
              return s;
            }
            char cmd = ct.narrow(*pattern, 0);
            switch (cmd)
            {
            case 'v':
            {
              if (value_found)
              {
                err |= std::ios_base::failbit;
                return s;
              }
              if (value_found)
              {
                err |= std::ios_base::failbit;
                return s;
              }
              value_found = true;
              s = get_value(s, end, ios, err, r);
              if (err & (std::ios_base::badbit | std::ios_base::failbit))
              {
                return s;
              }
              break;
            }
            case 'u':
            {
              if (unit_found)
              {
                err |= std::ios_base::failbit;
                return s;
              }
              unit_found = true;
              s = get_unit(facet, s, end, ios, err, rt);
              if (err & (std::ios_base::badbit | std::ios_base::failbit))
              {
                return s;
              }
              break;
            }
              //                case 'x':
              //                {
              //                  if (unit_found || value_found || loc_found) {
              //                    err |= std::ios_base::failbit;
              //                    return s;
              //                  }
              //                  loc_found=true;
              //                  string_type pat = duration_units<CharT>::imbue_if_has_not(ios).get_pattern();
              //                  if (pattern+1 != pat_end)
              //                  pat.append(pattern+1, pat_end);
              //                  pattern = pat.data();
              //                  pat_end = pattern + pat.size();
              //                  break;
              //                }
            default:
              BOOST_ASSERT(false && "Boost::Chrono internal error.");
              break;
            }

            ++pattern;
          }
          else if (ct.is(std::ctype_base::space, *pattern))
          {
            for (++pattern; pattern != pat_end && ct.is(std::ctype_base::space, *pattern); ++pattern)
              ;
            for (; s != end && ct.is(std::ctype_base::space, *s); ++s)
              ;
          }
          else if (ct.toupper(*s) == ct.toupper(*pattern))
          {
            ++s;
            ++pattern;
          }
          else
          {
            err |= std::ios_base::failbit;
          }

        }

        unsigned long long num = rt.num;
        unsigned long long den = rt.den;

        // r should be multiplied by (num/den) / Period
        // Reduce (num/den) / Period to lowest terms
        unsigned long long gcd_n1_n2 = math::gcd<unsigned long long>(num, Period::num);
        unsigned long long gcd_d1_d2 = math::gcd<unsigned long long>(den, Period::den);
        num /= gcd_n1_n2;
        den /= gcd_d1_d2;
        unsigned long long n2 = Period::num / gcd_n1_n2;
        unsigned long long d2 = Period::den / gcd_d1_d2;
        if (num > (std::numeric_limits<unsigned long long>::max)() / d2 || den
            > (std::numeric_limits<unsigned long long>::max)() / n2)
        {
          // (num/den) / Period overflows
          err |= std::ios_base::failbit;
          return s;
        }
        num *= d2;
        den *= n2;

        typedef typename common_type<intermediate_type, unsigned long long>::type common_type_t;

        // num / den is now factor to multiply by r
        if (!detail::reduce(r, den, err)) return s;

        //if (r > ( (duration_values<common_type_t>::max)() / num))
        if (chrono::detail::gt(r, ( (duration_values<common_type_t>::max)() / num)))
        //if (common_type_t(r) > ( (duration_values<common_type_t>::max)() / num))
        {
          // Conversion to Period overflowed
          err |= std::ios_base::failbit;
          return s;
        }
        common_type_t t = r * num;
        t /= den;
        if (t > 0)
        {
          Rep pt = t;
          if ( (duration_values<Rep>::max)() < pt)
          {
            // Conversion to Period overflowed
            err |= std::ios_base::failbit;
            return s;
          }
        }
        // Success!  Store it.
        r = Rep(t);
        d = duration<Rep, Period> (r);

        return s;
      }

      /**
       *
       * @param s start input stream iterator
       * @param end end input stream iterator
       * @param ios a reference to a ios_base
       * @param err the ios_base state
       * @param d the duration
       * Stores the duration pattern from the @c duration_unit facet in let say @c str. Last as if
       * @code
       *   return get(s, end, ios, err, ios, d, str.data(), str.data() + str.size());
       * @codeend
       * @Returns An iterator pointing just beyond the last character that can be determined to be part of a valid name
       */
      template <typename Rep, typename Period>
      iter_type get(iter_type s, iter_type end, std::ios_base& ios, std::ios_base::iostate& err,
          duration<Rep, Period> & d) const
      {
        if (std::has_facet<duration_units<CharT> >(ios.getloc()))
        {
          duration_units<CharT> const&facet = std::use_facet<duration_units<CharT> >(ios.getloc());
          std::basic_string<CharT> str = facet.get_pattern();
          return get(facet, s, end, ios, err, d, str.data(), str.data() + str.size());
        }
        else
        {
          duration_units_default<CharT> facet;
          std::basic_string<CharT> str = facet.get_pattern();
          return get(facet, s, end, ios, err, d, str.data(), str.data() + str.size());
        }

      }

      /**
       *
       * @param s start input stream iterator
       * @param end end input stream iterator
       * @param ios a reference to a ios_base
       * @param err the ios_base state
       * @param r a reference to the duration representation
       * @Returns An iterator pointing just beyond the last character that can be determined to be part of a valid name
       */
      template <typename Rep>
      iter_type get_value(iter_type s, iter_type end, std::ios_base& ios, std::ios_base::iostate& err, Rep& r) const
      {
        return std::use_facet<std::num_get<CharT, iter_type> >(ios.getloc()).get(s, end, ios, err, r);
      }

      /**
       *
       * @param s start input stream iterator
       * @param e end input stream iterator
       * @param ios a reference to a ios_base
       * @param err the ios_base state
       * @param rt a reference to the duration run-time ratio.
       * @Returns An iterator pointing just beyond the last character that can be determined to be part of a valid name
       */
      iter_type get_unit(iter_type i, iter_type e, std::ios_base& is, std::ios_base::iostate& err, detail::rt_ratio &rt) const
      {
        if (std::has_facet<duration_units<CharT> >(is.getloc()))
        {
          return get_unit(std::use_facet<duration_units<CharT> >(is.getloc()), i, e, is, err, rt);
        }
        else
        {
          duration_units_default<CharT> facet;
          return get_unit(facet, i, e, is, err, rt);
        }
      }

      iter_type get_unit(duration_units<CharT> const &facet, iter_type i, iter_type e, std::ios_base& is,
          std::ios_base::iostate& err, detail::rt_ratio &rt) const
      {

        if (*i == '[')
        {
          // parse [N/D]s or [N/D]seconds format
          ++i;
          i = std::use_facet<std::num_get<CharT, iter_type> >(is.getloc()).get(i, e, is, err, rt.num);
          if ( (err & std::ios_base::failbit) != 0)
          {
            return i;
          }

          if (i == e)
          {
            err |= std::ios_base::failbit;
            return i;
          }
          CharT x = *i++;
          if (x != '/')
          {
            err |= std::ios_base::failbit;
            return i;
          }
          i = std::use_facet<std::num_get<CharT, iter_type> >(is.getloc()).get(i, e, is, err, rt.den);
          if ( (err & std::ios_base::failbit) != 0)
          {
            return i;
          }
          if (i == e)
          {
            err |= std::ios_base::failbit;
            return i;
          }
          if (*i != ']')
          {
            err |= std::ios_base::failbit;
            return i;
          }
          ++i;
          if (i == e)
          {
            err |= std::ios_base::failbit;
            return i;
          }
          const string_type units[] =
          { facet.template get_plural_form<ratio<1> > (duration_style::prefix, 1), facet.template get_plural_form<
              ratio<1> > (duration_style::prefix, 0), facet.template get_plural_form<ratio<1> > (
              duration_style::symbol, 0) };
          // FIXME is this necessary?????
          err = std::ios_base::goodbit;
          const string_type* k = chrono_detail::scan_keyword(i, e, units, units + sizeof (units) / sizeof (units[0]),
          //~ std::use_facet<std::ctype<CharT> >(loc),
              err);
          switch ( (k - units) / 3)
          {
          case 0:
            break;
          default:
            err |= std::ios_base::failbit;
            return i;
          }
        }
        else
        {
          return do_get_prefix_unit(facet, i, e, is, err, rt);
        }

        return i;
      }

      /**
       * Unique identifier for this type of facet.
       */
      static std::locale::id id;

      /**
       * @Effects Destroy the facet
       */
      ~duration_get()
      {
      }

    protected:

      virtual iter_type do_get_prefix_unit(iter_type i, iter_type e, std::ios_base& is, std::ios_base::iostate& err,
          detail::rt_ratio &rt) const
      {
        if (std::has_facet<duration_units<CharT> >(is.getloc()))
        {
          return do_get_prefix_unit(std::use_facet<duration_units<CharT> >(is.getloc()), i, e, is, err, rt);
        }
        else
        {
          duration_units_default<CharT> facet;
          return do_get_prefix_unit(facet, i, e, is, err, rt);
        }
      }

      /**
       * Extracts the run-time ratio associated to the duration when it is given in prefix form.
       *
       * This is an extension point of this facet so that we can take in account other periods that can have a useful
       * translation in other contexts, as e.g. days and weeks.
       * @param s start input stream iterator.
       * @param e end input stream iterator.
       * @param ios a reference to a ios_base.
       * @param err the ios_base state.
       * @param rt a reference to the duration run-time ratio.
       * @Returns An iterator pointing just beyond the last character that can be determined to be part of a valid name.
       */
      virtual iter_type do_get_prefix_unit(duration_units<CharT> const &facet, iter_type i, iter_type e,
          std::ios_base&, std::ios_base::iostate& err, detail::rt_ratio &rt) const
      {
        // parse SI name, short or long
        std::size_t pfs = facet.get_plural_forms() + 1;

        // scoped_ptr ???
        string_type* units = new string_type[19 * pfs]();
        string_type* it = units;
        it = facet.fill_units(it, atto());
        it = facet.fill_units(it, femto());
        it = facet.fill_units(it, pico());
        it = facet.fill_units(it, nano());
        it = facet.fill_units(it, micro());
        it = facet.fill_units(it, milli());
        it = facet.fill_units(it, centi());
        it = facet.fill_units(it, deci());
        it = facet.fill_units(it, deca());
        it = facet.fill_units(it, hecto());
        it = facet.fill_units(it, kilo());
        it = facet.fill_units(it, mega());
        it = facet.fill_units(it, giga());
        it = facet.fill_units(it, tera());
        it = facet.fill_units(it, peta());
        it = facet.fill_units(it, exa());
        it = facet.fill_units(it, ratio<1> ());
        it = facet.fill_units(it, ratio<60> ());
        it = facet.fill_units(it, ratio<3600> ());

        string_type* units_end = units + 19 * pfs;

        err = std::ios_base::goodbit;
        std::cerr << __FILE__ << ":" << __LINE__ << " " << std::endl;
        const string_type* k = chrono_detail::scan_keyword(i, e, units, units_end,
        //~ std::use_facet<std::ctype<CharT> >(loc),
            err);
        std::cerr << __FILE__ << ":" << __LINE__ << " err" << err << std::endl;

        std::size_t index = (k - units) / pfs;
        delete[] units;
        switch (index)
        {
        case 0:
          rt = detail::rt_ratio(atto());
          break;
        case 1:
          rt = detail::rt_ratio(femto());
          break;
        case 2:
          rt = detail::rt_ratio(pico());
          break;
        case 3:
          rt = detail::rt_ratio(nano());
          break;
        case 4:
          rt = detail::rt_ratio(micro());
          break;
        case 5:
          rt = detail::rt_ratio(milli());
          break;
        case 6:
          rt = detail::rt_ratio(centi());
          break;
        case 7:
          rt = detail::rt_ratio(deci());
          break;
        case 8:
          rt = detail::rt_ratio(deca());
          break;
        case 9:
          rt = detail::rt_ratio(hecto());
          break;
        case 10:
          rt = detail::rt_ratio(kilo());
          break;
        case 11:
          rt = detail::rt_ratio(mega());
          break;
        case 12:
          rt = detail::rt_ratio(giga());
          break;
        case 13:
          rt = detail::rt_ratio(tera());
          break;
        case 14:
          rt = detail::rt_ratio(peta());
          break;
        case 15:
          rt = detail::rt_ratio(exa());
          break;
        case 16:
          rt = detail::rt_ratio(ratio<1> ());
          break;
        case 17:
          rt = detail::rt_ratio(ratio<60> ());
          break;
        case 18:
          rt = detail::rt_ratio(ratio<3600> ());
          break;
        default:
          err = std::ios_base::failbit;
          std::cout << __FILE__ << ":" << __LINE__ << std::endl;
          return i;
        }
        std::cerr << __FILE__ << ":" << __LINE__ << " " << std::endl;

        return i;

      }
    };

    /**
     * Unique identifier for this type of facet.
     */
    template <class CharT, class InputIterator>
    std::locale::id duration_get<CharT, InputIterator>::id;

  } // chrono
}
// boost

#endif  // header
