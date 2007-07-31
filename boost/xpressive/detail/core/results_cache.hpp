///////////////////////////////////////////////////////////////////////////////
// results_cache.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_CORE_RESULTS_CACHE_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_CORE_RESULTS_CACHE_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <list>
#include <boost/assert.hpp>
#include <boost/xpressive/detail/detail_fwd.hpp>
#include <boost/xpressive/detail/core/access.hpp>
#include <boost/xpressive/match_results.hpp>

namespace boost { namespace xpressive { namespace detail
{

///////////////////////////////////////////////////////////////////////////////
// nested_results
//   BUGBUG by using std::list, it makes construction of of an empty nested_results
//   incur a dynamic allocation. As a result, construction an empty match_results is
//   likewise not free. FIXME.
#if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3206))
template<typename BidiIter>
struct nested_results
  : std::list<match_results<BidiIter> >
{
    friend struct results_cache<BidiIter>;
    friend struct match_results<BidiIter>;
};
#else
template<typename BidiIter>
struct nested_results
  : private std::list<match_results<BidiIter> >
{
    friend struct results_cache<BidiIter>;
    friend struct match_results<BidiIter>;
    typedef std::list<match_results<BidiIter> > base_type;

    using base_type::iterator;
    using base_type::const_iterator;
    #if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB , == 1)
    // old Dinkumware doesn't expose pointer typedefs
    typedef base_type::value_type *pointer;
    typedef base_type::value_type const *const_pointer;
    #else
    using base_type::pointer;
    using base_type::const_pointer;
    #endif
    using base_type::reference;
    using base_type::const_reference;
    using base_type::size_type;
    using base_type::begin;
    using base_type::end;
    using base_type::size;
    using base_type::empty;
    using base_type::front;
    using base_type::back;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// results_cache
//
//   cache storage for reclaimed match_results structs
template<typename BidiIter>
struct results_cache
{
    typedef core_access<BidiIter> access;

    match_results<BidiIter> &append_new(nested_results<BidiIter> &out)
    {
        if(this->cache_.empty())
        {
            out.push_back(match_results<BidiIter>());
        }
        else
        {
            BOOST_ASSERT(access::get_nested_results(this->cache_.back()).empty());
            out.splice(out.end(), this->cache_, --this->cache_.end());
        }
        return out.back();
    }

    // move the last match_results struct into the cache
    void reclaim_last(nested_results<BidiIter> &out)
    {
        BOOST_ASSERT(!out.empty());
        // first, reclaim any nested results
        nested_results<BidiIter> &nested = access::get_nested_results(out.back());
        if(!nested.empty())
        {
            this->reclaim_all(nested);
        }
        // then, reclaim the last match_results
        this->cache_.splice(this->cache_.end(), out, --out.end());
    }

    // move the last n match_results structs into the cache
    void reclaim_last_n(nested_results<BidiIter> &out, std::size_t count)
    {
        for(; 0 != count; --count)
        {
            this->reclaim_last(out);
        }
    }

    void reclaim_all(nested_results<BidiIter> &out)
    {
        typedef typename nested_results<BidiIter>::iterator iter_type;

        // first, recursively reclaim all the nested results
        for(iter_type begin = out.begin(); begin != out.end(); ++begin)
        {
            nested_results<BidiIter> &nested = access::get_nested_results(*begin);

            if(!nested.empty())
            {
                this->reclaim_all(nested);
            }
        }

        // next, reclaim the results themselves
        this->cache_.splice(this->cache_.end(), out);
    }

private:

    nested_results<BidiIter> cache_;
};

}}} // namespace boost::xpressive::detail

#endif
