/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library

    Copyright (c) 2001-2004 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(TOKEN_CACHE_HPP_4D2320B7_1D56_4113_A114_397E70FA438C_INCLUDED)
#define TOKEN_CACHE_HPP_4D2320B7_1D56_4113_A114_397E70FA438C_INCLUDED

#include <boost/wave/token_ids.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace cpplexer {

template <typename StringT>
class token_cache 
{
public:
    token_cache()
    :   cache(T_LAST_TOKEN - T_FIRST_TOKEN)
    {
        typename std::vector<StringT>::iterator it = cache.begin();
        typename std::vector<StringT>::iterator end = cache.end();
        for (unsigned int i = T_FIRST_TOKEN; i < T_LAST_TOKEN;  ++i, ++it)
        {
            *it = StringT(boost::wave::get_token_value(token_id(i)));
        }
    }

    StringT const &get_token_value(token_id id)
    {
        return cache[BASEID_FROM_TOKEN(id) - T_FIRST_TOKEN];
    }
    
private:
    std::vector<StringT> cache;
};

///////////////////////////////////////////////////////////////////////////////
}   // namespace cpplexer 
}   // namespace wave
}   // namespace boost

#endif // !defined(TOKEN_CACHE_HPP_4D2320B7_1D56_4113_A114_397E70FA438C_INCLUDED)
