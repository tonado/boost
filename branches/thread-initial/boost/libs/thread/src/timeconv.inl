/*
 * Copyright (C) 2001
 * William E. Kempf
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  William E. Kempf makes no representations
 * about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * Revision History (excluding minor changes for specific compilers)
 *    1 Jun 01  Initial creation.
 */
 
namespace {
	const unsigned MILLISECONDS_PER_SECOND = 1000;
	const unsigned NANOSECONDS_PER_SECOND = 1000000000;
	const unsigned NANOSECONDS_PER_MILLISECOND = 1000000;

	inline void to_time(unsigned milliseconds, boost::xtime& xt)
	{
        int res = boost::xtime_get(&xt, boost::TIME_UTC);
        assert(res == boost::TIME_UTC);

        xt.sec += (milliseconds / MILLISECONDS_PER_SECOND);
        xt.nsec += ((milliseconds % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND);

        if (xt.nsec > NANOSECONDS_PER_SECOND)
        {
            ++xt.sec;
            xt.nsec -= NANOSECONDS_PER_SECOND;
        }
	}

#if defined(BOOST_HAS_PTHREADS)
    inline void to_timespec(const boost::xtime& xt, timespec& ts)
    {
        ts.tv_sec = static_cast<int>(xt.sec);
        ts.tv_nsec = static_cast<int>(xt.nsec);
    }

    inline void to_time(unsigned milliseconds, timespec& ts)
    {
        boost::xtime xt;
        to_time(milliseconds, xt);
        to_timespec(xt, ts);
    }
#endif

	inline void to_duration(const boost::xtime& xt, unsigned& milliseconds)
	{
        boost::xtime cur;
        int res = boost::xtime_get(&cur, boost::TIME_UTC);
        assert(res == boost::TIME_UTC);

        if (xt.sec < cur.sec || (xt.sec == cur.sec && xt.nsec < cur.nsec))
            milliseconds = 0;
		else
		{
			milliseconds = ((xt.sec - cur.sec) * MILLISECONDS_PER_SECOND) +
				(((xt.nsec - cur.nsec) + (NANOSECONDS_PER_MILLISECOND/2))
				/ NANOSECONDS_PER_MILLISECOND);
		}
	}
}