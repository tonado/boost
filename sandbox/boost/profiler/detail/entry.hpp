#ifndef BOOST_PROFILER_DETAIL_ENTRY_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_ENTRY_HPP_INCLUDED

#include <boost/cstdint.hpp>

namespace boost { namespace profiler { namespace detail
{

    class point;
    
    struct entry
    {
        const point *p;
        tick_t start;
        tick_t total;
        tick_t start_exclusive;
        tick_t total_exclusive;
        boost::uint64_t count;
        entry(): 
            total(0), total_exclusive(0), count(0), p(0)
        {
        }
    };
    
} } }

#endif
