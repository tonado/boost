
//  chrono_io
//
//  (C) Copyright Howard Hinnant
//  (C) Copyright 2010 Vicente J. Botet Escriba
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
// This code was adapted by Vicente from Howard Hinnant's experimental work
// on chrono i/o under lvm/libc++  to Boost

#ifndef BOOST_CHRONO_CHRONO_IO_HPP
#define BOOST_CHRONO_CHRONO_IO_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/thread_clock.hpp>
#include <boost/ratio/ratio_io.hpp>
#include <locale>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/mpl/if.hpp>
#include <boost/math/common_factor_rt.hpp>
#include <boost/chrono/detail/scan_keyword.hpp>
#include <boost/chrono/round.hpp>

namespace boost
{

namespace chrono
{

  struct timezone
  {
    enum type {
      utc, local
    };
  };

template <class CharT>
class duration_punct
    : public std::locale::facet
{
public:
    typedef std::basic_string<CharT> string_type;
    enum {use_long, use_short};

private:
    bool use_short_;
    string_type long_seconds_;
    string_type long_minutes_;
    string_type long_hours_;
    string_type short_seconds_;
    string_type short_minutes_;
    string_type short_hours_;

    template <class Period>
        string_type short_name(Period) const
            {return ::boost::ratio_string<Period, CharT>::short_name() + short_seconds_;}

    string_type short_name(ratio<1>) const    {return short_seconds_;}
    string_type short_name(ratio<60>) const   {return short_minutes_;}
    string_type short_name(ratio<3600>) const {return short_hours_;}

    template <class Period>
        string_type long_name(Period) const
            {return ::boost::ratio_string<Period, CharT>::long_name() + long_seconds_;}

    string_type long_name(ratio<1>) const    {return long_seconds_;}
    string_type long_name(ratio<60>) const   {return long_minutes_;}
    string_type long_name(ratio<3600>) const {return long_hours_;}

    void init_C();
public:
    static std::locale::id id;

    explicit duration_punct(int use = use_long, size_t refs = 0)
        : std::locale::facet(refs),  use_short_(use==use_short) {init_C();}

    duration_punct(int use,
        const string_type& long_seconds, const string_type& long_minutes,
        const string_type& long_hours, const string_type& short_seconds,
        const string_type& short_minutes, const string_type& short_hours,
        size_t refs = 0);

    duration_punct(int use, const duration_punct& d, size_t refs = 0);

    template <class Period>
        string_type short_name() const
            {return short_name(typename Period::type());}

    template <class Period>
        string_type long_name() const
            {return long_name(typename Period::type());}

    template <class Period>
        string_type name() const {
            if (use_short_) return short_name<Period>();
            else return long_name<Period>();
        }
    template <class Rep, class Period>
        string_type name(duration<Rep,Period> const& ) const {
            if (use_short_) return short_name<Period>();
            else return long_name<Period>();
        }

    bool is_short_name() const {return use_short_;}
    bool is_long_name() const {return !use_short_;}
};

template <class CharT>
std::locale::id
duration_punct<CharT>::id;

template <class CharT>
void
duration_punct<CharT>::init_C()
{
    short_seconds_ = CharT('s');
    short_minutes_ = CharT('m');
    short_hours_ = CharT('h');
    const CharT s[] = {'s', 'e', 'c', 'o', 'n', 'd', 's'};
    const CharT m[] = {'m', 'i', 'n', 'u', 't', 'e', 's'};
    const CharT h[] = {'h', 'o', 'u', 'r', 's'};
    long_seconds_.assign(s, s + sizeof(s)/sizeof(s[0]));
    long_minutes_.assign(m, m + sizeof(m)/sizeof(m[0]));
    long_hours_.assign(h, h + sizeof(h)/sizeof(h[0]));
}

template <class CharT>
duration_punct<CharT>::duration_punct(int use,
        const string_type& long_seconds, const string_type& long_minutes,
        const string_type& long_hours, const string_type& short_seconds,
        const string_type& short_minutes, const string_type& short_hours,
        size_t refs)
    : std::locale::facet(refs),
      use_short_(use==use_short),
      long_seconds_(long_seconds),
      long_minutes_(long_minutes),
      long_hours_(long_hours),
      short_seconds_(short_seconds),
      short_minutes_(short_minutes),
      short_hours_(short_hours)
{}

template <class CharT>
duration_punct<CharT>::duration_punct(int use, const duration_punct& d,
    size_t refs)
    : std::locale::facet(refs),
      use_short_(use==use_short),
      long_seconds_(d.long_seconds_),
      long_minutes_(d.long_minutes_),
      long_hours_(d.long_hours_),
      short_seconds_(d.short_seconds_),
      short_minutes_(d.short_minutes_),
      short_hours_(d.short_hours_)
{}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
duration_short(std::basic_ostream<CharT, Traits>& os)
{
    typedef duration_punct<CharT> Facet;
    std::locale loc = os.getloc();
    if (std::has_facet<Facet>(loc))
    {
        const Facet& f = std::use_facet<Facet>(loc);
        if (f.is_long_name())
            os.imbue(std::locale(loc, new Facet(Facet::use_short, f)));
    }
    else
        os.imbue(std::locale(loc, new Facet(Facet::use_short)));
    return os;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
duration_long(std::basic_ostream<CharT, Traits>& os)
{
    typedef duration_punct<CharT> Facet;
    std::locale loc = os.getloc();
    if (std::has_facet<Facet>(loc))
    {
        const Facet& f = std::use_facet<Facet>(loc);
        if (f.is_short_name())
            os.imbue(std::locale(loc, new Facet(Facet::use_long, f)));
    }
    return os;
}

template <class CharT, class Traits, class Rep, class Period>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const duration<Rep, Period>& d)
{
  typename std::basic_ostream<CharT, Traits>::sentry ok(os);
  if (ok)
  {
    typedef duration_punct<CharT> Facet;
    typedef std::basic_string<CharT, Traits> string_type;

    bool failed = false;
    try
    {
      std::locale loc = os.getloc();

      if (!std::has_facet<Facet>(loc))
        os.imbue(std::locale(loc, new Facet));
      const Facet& f = std::use_facet<Facet>(os.getloc());
      return os << d.count() << ' ' << f.template name<Rep,Period>(d);
    }
    catch (...)
    {
        failed = true;
    }
    if (failed)
        os.setstate(std::ios_base::failbit | std::ios_base::badbit);

  }
  return os;
}

namespace chrono_detail {
template <class Rep, bool = is_scalar<Rep>::value>
struct duration_io_intermediate
{
    typedef Rep type;
};

template <class Rep>
struct duration_io_intermediate<Rep, true>
{
    typedef typename mpl::if_c
    <
        is_floating_point<Rep>::value,
            long double,
            typename mpl::if_c
            <
                is_signed<Rep>::value,
                    long long,
                    unsigned long long
            >::type
    >::type type;
};

}

template <class CharT, class Traits, class Rep, class Period>
std::basic_istream<CharT, Traits>&
operator>>(std::basic_istream<CharT, Traits>& is, duration<Rep, Period>& d)
{
    typedef duration_punct<CharT> Facet;
    std::locale loc = is.getloc();
    if (!std::has_facet<Facet>(loc))
        is.imbue(std::locale(loc, new Facet));
    loc = is.getloc();
    const Facet& f = std::use_facet<Facet>(loc);
    typedef typename chrono_detail::duration_io_intermediate<Rep>::type intermediate_type;
    intermediate_type r;
    // read value into r
    is >> r;
    if (is.good())
    {
        // now determine unit
        typedef std::istreambuf_iterator<CharT, Traits> in_iterator;
        in_iterator i(is);
        in_iterator e;
        if (i != e && *i == ' ')  // mandatory ' ' after value
        {
            ++i;
            if (i != e)
            {
                // unit is num / den (yet to be determined)
                unsigned long long num = 0;
                unsigned long long den = 0;
                if (*i == '[')
                {
                    // parse [N/D]s or [N/D]seconds format
                    ++i;
                    CharT x;
                    is >> num >> x >> den;
                    if (!is.good() || (x != '/'))
                    {
                        is.setstate(is.failbit);
                        return is;
                    }
                    i = in_iterator(is);
                    if (*i != ']')
                    {
                        is.setstate(is.failbit);
                        return is;
                    }
                    ++i;
                    const std::basic_string<CharT> units[] =
                    {
                        f.template long_name<ratio<1> >(),
                        f.template short_name<ratio<1> >()
                    };
                    std::ios_base::iostate err = std::ios_base::goodbit;
                    const std::basic_string<CharT>* k = chrono_detail::scan_keyword(i, e,
                                  units, units + sizeof(units)/sizeof(units[0]),
                                  //~ std::use_facet<std::ctype<CharT> >(loc),
                                  err);
                    switch ((k - units) / 2)
                    {
                    case 0:
                        break;
                    default:
                        is.setstate(err);
                        return is;
                    }
                }
                else
                {
                    // parse SI name, short or long
                    const std::basic_string<CharT> units[] =
                    {
                        f.template long_name<atto>(),
                        f.template short_name<atto>(),
                        f.template long_name<femto>(),
                        f.template short_name<femto>(),
                        f.template long_name<pico>(),
                        f.template short_name<pico>(),
                        f.template long_name<nano>(),
                        f.template short_name<nano>(),
                        f.template long_name<micro>(),
                        f.template short_name<micro>(),
                        f.template long_name<milli>(),
                        f.template short_name<milli>(),
                        f.template long_name<centi>(),
                        f.template short_name<centi>(),
                        f.template long_name<deci>(),
                        f.template short_name<deci>(),
                        f.template long_name<deca>(),
                        f.template short_name<deca>(),
                        f.template long_name<hecto>(),
                        f.template short_name<hecto>(),
                        f.template long_name<kilo>(),
                        f.template short_name<kilo>(),
                        f.template long_name<mega>(),
                        f.template short_name<mega>(),
                        f.template long_name<giga>(),
                        f.template short_name<giga>(),
                        f.template long_name<tera>(),
                        f.template short_name<tera>(),
                        f.template long_name<peta>(),
                        f.template short_name<peta>(),
                        f.template long_name<exa>(),
                        f.template short_name<exa>(),
                        f.template long_name<ratio<1> >(),
                        f.template short_name<ratio<1> >(),
                        f.template long_name<ratio<60> >(),
                        f.template short_name<ratio<60> >(),
                        f.template long_name<ratio<3600> >(),
                        f.template short_name<ratio<3600> >()
                    };
                    std::ios_base::iostate err = std::ios_base::goodbit;
                    const std::basic_string<CharT>* k = chrono_detail::scan_keyword(i, e,
                                  units, units + sizeof(units)/sizeof(units[0]),
                                  //~ std::use_facet<std::ctype<CharT> >(loc),
                                  err);
                    switch ((k - units) / 2)
                    {
                    case 0:
                        num = 1ULL;
                        den = 1000000000000000000ULL;
                        break;
                    case 1:
                        num = 1ULL;
                        den = 1000000000000000ULL;
                        break;
                    case 2:
                        num = 1ULL;
                        den = 1000000000000ULL;
                        break;
                    case 3:
                        num = 1ULL;
                        den = 1000000000ULL;
                        break;
                    case 4:
                        num = 1ULL;
                        den = 1000000ULL;
                        break;
                    case 5:
                        num = 1ULL;
                        den = 1000ULL;
                        break;
                    case 6:
                        num = 1ULL;
                        den = 100ULL;
                        break;
                    case 7:
                        num = 1ULL;
                        den = 10ULL;
                        break;
                    case 8:
                        num = 10ULL;
                        den = 1ULL;
                        break;
                    case 9:
                        num = 100ULL;
                        den = 1ULL;
                        break;
                    case 10:
                        num = 1000ULL;
                        den = 1ULL;
                        break;
                    case 11:
                        num = 1000000ULL;
                        den = 1ULL;
                        break;
                    case 12:
                        num = 1000000000ULL;
                        den = 1ULL;
                        break;
                    case 13:
                        num = 1000000000000ULL;
                        den = 1ULL;
                        break;
                    case 14:
                        num = 1000000000000000ULL;
                        den = 1ULL;
                        break;
                    case 15:
                        num = 1000000000000000000ULL;
                        den = 1ULL;
                        break;
                    case 16:
                        num = 1;
                        den = 1;
                        break;
                    case 17:
                        num = 60;
                        den = 1;
                        break;
                    case 18:
                        num = 3600;
                        den = 1;
                        break;
                    default:
                        is.setstate(err);
                        return is;
                    }
                }
                // unit is num/den
                // r should be multiplied by (num/den) / Period
                // Reduce (num/den) / Period to lowest terms
                unsigned long long gcd_n1_n2 = math::gcd<unsigned long long>(num, Period::num);
                unsigned long long gcd_d1_d2 = math::gcd<unsigned long long>(den, Period::den);
                num /= gcd_n1_n2;
                den /= gcd_d1_d2;
                unsigned long long n2 = Period::num / gcd_n1_n2;
                unsigned long long d2 = Period::den / gcd_d1_d2;
                if (num > (std::numeric_limits<unsigned long long>::max)() / d2 ||
                    den > (std::numeric_limits<unsigned long long>::max)() / n2)
                {
                    // (num/den) / Period overflows
                    is.setstate(is.failbit);
                    return is;
                }
                num *= d2;
                den *= n2;
                // num / den is now factor to multiply by r
                typedef typename common_type<intermediate_type, unsigned long long>::type common_type_t;
                if (is_integral<intermediate_type>::value)
                {
                    // Reduce r * num / den
                    common_type_t t = math::gcd<common_type_t>(r, den);
                    r /= t;
                    den /= t;
                    if (den != 1)
                    {
                        // Conversion to Period is integral and not exact
                        is.setstate(is.failbit);
                        return is;
                    }
                }
                if (r > ((duration_values<common_type_t>::max)() / num))
                {
                    // Conversion to Period overflowed
                    is.setstate(is.failbit);
                    return is;
                }
                common_type_t t = r * num;
                t /= den;
                if ((duration_values<Rep>::max)() < t)
                {
                    // Conversion to Period overflowed
                    is.setstate(is.failbit);
                    return is;
                }
                // Success!  Store it.
                r = Rep(t);
                d = duration<Rep, Period>(r);
            }
            else
                is.setstate(is.failbit | is.eofbit);
        }
        else
        {
            if (i == e)
                is.setstate(is.eofbit);
            is.setstate(is.failbit);
        }
    }
    else
        is.setstate(is.failbit);
    return is;
}

template <class Clock, class CharT>
struct clock_string;

template <class CharT>
struct clock_string<system_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'s', 'y', 's', 't', 'e', 'm', '_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        static const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 'J', 'a',
                              'n', ' ', '1', ',', ' ', '1', '9', '7', '0'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};
#ifdef BOOST_CHRONO_HAS_CLOCK_STEADY

template <class CharT>
struct clock_string<steady_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'m', 'o', 'n', 'o', 't', 'o', 'n', 'i', 'c', '_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 'b', 'o', 'o', 't'};
        const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};


#endif

#if defined(BOOST_CHRONO_HAS_THREAD_CLOCK)

template <class CharT>
struct clock_string<thread_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'t', 'h', 'r', 'e', 'd', '_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 't', 'r', 'e', 'a', 'd', ' ', 's', 't', 'a', 'r', 't', '-', 'u', 'p'};
        const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};

#endif

template <class CharT>
struct clock_string<process_real_cpu_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'p', 'r', 'o', 'c', 'e', 's', 's', '_', 'r', 'e', 'a', 'l','_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 'p', 'r', 'o', 'c', 'e', 's', 's', ' ', 's', 't', 'a', 'r', 't', '-', 'u', 'p'};
        const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};

template <class CharT>
struct clock_string<process_user_cpu_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'p', 'r', 'o', 'c', 'e', 's', 's', '_', 'u', 's', 'e', 'r','_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 'p', 'r', 'o', 'c', 'e', 's', 's', ' ', 's', 't', 'a', 'r', 't', '-', 'u', 'p'};
        const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};

template <class CharT>
struct clock_string<process_system_cpu_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'p', 'r', 'o', 'c', 'e', 's', 's', '_', 's', 'y', 's', 't', 't', 'e', 'm', '_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 'p', 'r', 'o', 'c', 'e', 's', 's', ' ', 's', 't', 'a', 'r', 't', '-', 'u', 'p'};
        const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};

template <class CharT>
struct clock_string<process_cpu_clock, CharT>
{
    static std::basic_string<CharT> name()
    {
        static const CharT u[] = {'p', 'r', 'o', 'c', 'e', 's', 's', '_',
                                'c', 'l','o', 'c', 'k'};
        static const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
    static std::basic_string<CharT> since()
    {
        const CharT u[] = {' ', 's', 'i', 'n', 'c', 'e', ' ', 'p', 'r', 'o', 'c', 'e', 's', 's', ' ', 's', 't', 'a', 'r', 't', '-', 'u', 'p'};
        const std::basic_string<CharT> str(u, u + sizeof(u)/sizeof(u[0]));
        return str;
    }
};

template <class CharT>
class time_punct
    : public std::locale::facet
{
public:
    typedef std::basic_string<CharT> string_type;

private:
    string_type           fmt_;
    chrono::timezone::type tz_;

public:
    static std::locale::id id;

    explicit time_punct(size_t refs = 0)
        : std::locale::facet(refs), tz_(timezone::utc) {}

    time_punct(timezone::type tz, string_type fmt, size_t refs = 0)
    // todo use move semantic when available.
        : std::locale::facet(refs), fmt_(fmt), tz_(tz) {}

    const string_type& fmt() const BOOST_CHRONO_NOEXCEPT {return fmt_;}
    chrono::timezone::type timezone() const BOOST_CHRONO_NOEXCEPT {return tz_;}
};

    template <class CharT>
    std::locale::id
    time_punct<CharT>::id;

    namespace detail
    {
    template<class CharT>
    struct time_manip
    {
        std::basic_string<CharT> fmt_;
        timezone tz_;

        time_manip(timezone tz, std::basic_string<CharT> fmt)
        // todo move semantics
            : fmt_(fmt),
              tz_(tz) {}
    };

    // todo move semantics
    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>&
    operator <<(std::basic_ostream<CharT, Traits>& os, time_manip<CharT> m)
    {
      // todo move semantics
        os.imbue(std::locale(os.getloc(), new time_punct<CharT>(m.tz_, m.fmt_)));
        return os;
    }

    template<class CharT, class Traits>
    std::basic_istream<CharT, Traits>&
    operator >>(std::basic_istream<CharT, Traits>& is, time_manip<CharT> m)
    {
      // todo move semantics
        is.imbue(std::locale(is.getloc(), new time_punct<CharT>(m.tz_, m.fmt_)));
        return is;
    }

    class time_man
    {
        timezone::type form_;
    public:
        explicit time_man(timezone::type f) : form_(f) {}
        // explicit
            operator timezone::type() const {return form_;}
    };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>&
    operator <<(std::basic_ostream<CharT, Traits>& os, time_man m)
    {
        os.imbue(std::locale(os.getloc(), new time_punct<CharT>(static_cast<timezone::type>(m), std::basic_string<CharT>())));
        return os;
    }

    template<class CharT, class Traits>
    std::basic_istream<CharT, Traits>&
    operator >>(std::basic_istream<CharT, Traits>& is, time_man m)
    {
        is.imbue(std::locale(is.getloc(), new time_punct<CharT>(static_cast<timezone::type>(m), std::basic_string<CharT>())));
        return is;
    }

    }

    inline
    detail::time_man
    time_fmt(timezone::type f)
    {
        return detail::time_man(f);
    }

template <class CharT, class Traits, class Clock, class Duration>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const time_point<Clock, Duration>& tp)
{
    return os << tp.time_since_epoch() << clock_string<Clock, CharT>::since();
}

template <class CharT, class Traits, class Clock, class Duration>
std::basic_istream<CharT, Traits>&
operator>>(std::basic_istream<CharT, Traits>& is,
           time_point<Clock, Duration>& tp)
{
    Duration d;
    is >> d;
    if (is.good())
    {
        const std::basic_string<CharT> units=clock_string<Clock, CharT>::since();
        std::ios_base::iostate err = std::ios_base::goodbit;
        typedef std::istreambuf_iterator<CharT, Traits> in_iterator;
        in_iterator i(is);
        in_iterator e;
        std::ptrdiff_t k = chrono_detail::scan_keyword(i, e,
                      &units, &units + 1,
                      //~ std::use_facet<std::ctype<CharT> >(is.getloc()),
                      err) - &units;
        if (k == 1)
        {
            // failed to read epoch string
            is.setstate(err);
            return is;
        }
        tp = time_point<Clock, Duration>(d);
    }
    else
        is.setstate(is.failbit);
    return is;
}
#if 0
template <class _CharT, class _Traits, class _Duration>
std::basic_ostream<_CharT, _Traits>&
operator<<(std::basic_ostream<_CharT, _Traits>& os,
           const time_point<system_clock, _Duration>& tp)
{
    typename std::basic_ostream<_CharT, _Traits>::sentry ok(os);
    if (ok)
    {
        bool failed = false;
        try
        {
            const _CharT* pb = 0; //nullptr;
            const _CharT* pe = pb;
            timezone::type tz = timezone::utc;
            typedef time_punct<_CharT> F;
            std::locale loc = os.getloc();
            if (std::has_facet<F>(loc))
            {
                const F& f = std::use_facet<F>(loc);
                pb = f.fmt().data();
                pe = pb + f.fmt().size();
                tz = f.timezone();
            }
            time_t t = system_clock::to_time_t(tp);
            tm tm;
            if (tz == timezone::local)
            {
                if (localtime_r(&t, &tm) == 0)
                    failed = true;
            }
            else
            {
                if (gmtime_r(&t, &tm) == 0)
                    failed = true;
            }
            if (!failed)
            {
                const std::time_put<_CharT>& tpf = std::use_facet<std::time_put<_CharT> >(loc);
                if (pb == pe)
                {
                    _CharT pattern[] = {'%', 'F', ' ', '%', 'H', ':', '%', 'M', ':'};
                    pb = pattern;
                    pe = pb + sizeof(pattern) / sizeof(_CharT);
                    failed = tpf.put(os, os, os.fill(), &tm, pb, pe).failed();
                    if (!failed)
                    {
                        duration<double> d = tp - system_clock::from_time_t(t) +
                                  seconds(tm.tm_sec);
                        if (d.count() < 10)
                            os << _CharT('0');
                        std::ios::fmtflags flgs = os.flags();
                        os.setf(std::ios::fixed, std::ios::floatfield);
                        os << d.count();
                        os.flags(flgs);
                        if (tz == timezone::local)
                        {
                            _CharT sub_pattern[] = {' ', '%', 'z'};
                            pb = sub_pattern;
                            pe = pb + + sizeof(sub_pattern) / sizeof(_CharT);
                            failed = tpf.put(os, os, os.fill(), &tm, pb, pe).failed();
                        }
                        else
                        {
                            _CharT sub_pattern[] = {' ', '+', '0', '0', '0', '0', 0};
                            os << sub_pattern;
                        }
                    }
                }
                else
                    failed = tpf.put(os, os, os.fill(), &tm, pb, pe).failed();
            }
        }
        catch (...)
        {
            failed = true;
        }
        if (failed)
            os.setstate(std::ios_base::failbit | std::ios_base::badbit);
    }
    return os;
}
namespace detail {
template <class _CharT, class _InputIterator>
minutes
extract_z(_InputIterator& b, _InputIterator e,
    std::ios_base::iostate& err, const std::ctype<_CharT>& ct)
{
    int min = 0;
    if (b != e)
    {
        char cn = ct.narrow(*b, 0);
        if (cn != '+' && cn != '-')
        {
            err |= std::ios_base::failbit;
            return minutes(0);
        }
        int sn = cn == '-' ? -1 : 1;
        int hr = 0;
        for (int i = 0; i < 2; ++i)
        {
            if (++b == e)
            {
                err |= std::ios_base::eofbit | std::ios_base::failbit;
                return minutes(0);
            }
            cn = ct.narrow(*b, 0);
            if (!('0' <= cn && cn <= '9'))
            {
                err |= std::ios_base::failbit;
                return minutes(0);
            }
            hr = hr * 10 + cn - '0';
        }
        for (int i = 0; i < 2; ++i)
        {
            if (++b == e)
            {
                err |= std::ios_base::eofbit | std::ios_base::failbit;
                return minutes(0);
            }
            cn = ct.narrow(*b, 0);
            if (!('0' <= cn && cn <= '9'))
            {
                err |= std::ios_base::failbit;
                return minutes(0);
            }
            min = min * 10 + cn - '0';
        }
        if (++b == e)
            err |= std::ios_base::eofbit;
        min += hr * 60;
        min *= sn;
    }
    else
        err |= std::ios_base::eofbit | std::ios_base::failbit;
    return minutes(min);
}
}

template <class _CharT, class _Traits, class _Duration>
std::basic_istream<_CharT, _Traits>&
operator>>(std::basic_istream<_CharT, _Traits>& is,
           time_point<system_clock, _Duration>& tp)
{
    typename std::basic_istream<_CharT,_Traits>::sentry ok(is);
    if (ok)
    {
      std::ios_base::iostate err = std::ios_base::goodbit;
        try
        {
            const _CharT* pb = 0;//nullptr;
            const _CharT* pe = pb;
            typedef time_punct<_CharT> F;
            std::locale loc = is.getloc();
            timezone::type tz = timezone::utc;
            if (std::has_facet<F>(loc))
            {
                const F& f = std::use_facet<F>(loc);
                pb = f.fmt().data();
                pe = pb + f.fmt().size();
                tz = f.timezone::type();
            }
            const std::time_get<_CharT>& tg = std::use_facet<std::time_get<_CharT> >(loc);
            const std::ctype<_CharT>& ct = std::use_facet<std::ctype<_CharT> >(loc);
            tm tm = {0};
            typedef std::istreambuf_iterator<_CharT, _Traits> _I;
            if (pb == pe)
            {
                _CharT pattern[] = {'%', 'Y', '-', '%', 'm', '-', '%', 'd',
                                    ' ', '%', 'H', ':', '%', 'M', ':'};
                pb = pattern;
                pe = pb + sizeof(pattern) / sizeof(_CharT);
                tg.get(is, 0, is, err, &tm, pb, pe);
                if (err & std::ios_base::failbit)
                    goto exit;
                double sec;
                _CharT c = _CharT();
                is >> sec;
                if (is.fail())
                {
                    err |= std::ios_base::failbit;
                    goto exit;
                }
                _I i(is);
                _I eof;
                c = *i;
                if (++i == eof || c != ' ')
                {
                    err |= std::ios_base::failbit;
                    goto exit;
                }
                minutes min = detail::extract_z(i, eof, err, ct);
                if (err & std::ios_base::failbit)
                    goto exit;
                time_t t;
                t = timegm(&tm);
                tp = system_clock::from_time_t(t) - min
                                 + round<microseconds>(duration<double>(sec));
            }
            else
            {
                const _CharT z[2] = {'%', 'z'};
                const _CharT* fz = std::search(pb, pe, z, z+2);
                tg.get(is, 0, is, err, &tm, pb, fz);
                minutes min(0);
                if (fz != pe)
                {
                    if (err != std::ios_base::goodbit)
                    {
                        err |= std::ios_base::failbit;
                        goto exit;
                    }
                    _I i(is);
                    _I eof;
                    min = extract_z(i, eof, err, ct);
                    if (err & std::ios_base::failbit)
                        goto exit;
                    if (fz+2 != pe)
                    {
                        if (err != std::ios_base::goodbit)
                        {
                            err |= std::ios_base::failbit;
                            goto exit;
                        }
                        tg.get(is, 0, is, err, &tm, fz+2, pe);
                        if (err & std::ios_base::failbit)
                            goto exit;
                    }
                }
                tm.tm_isdst = -1;
                time_t t;
                if (tz == timezone::utc || fz != pe)
                    t = timegm(&tm);
                else
                    t = mktime(&tm);
                tp = system_clock::from_time_t(t) - min;
            }
        }
        catch (...)
        {
            err |= std::ios_base::badbit | std::ios_base::failbit;
        }
    exit:
        is.setstate(err);
    }
    return is;
}
#endif
}  // chrono

}
#include <boost/chrono/duration_style.hpp>

#endif  // BOOST_CHRONO_CHRONO_IO_HPP
