///////////////////////////////////////////////////////////////////////////////
//
// has_unique.hpp
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef HAS_UNIQUE_EN_14_12_2004
#define HAS_UNIQUE_EN_14_12_2004

#include "./is_std_list.hpp"

namespace boost
{
    // Specialize this for user-defined types
    template<typename T>
    struct has_unique
        : is_std_list<T>
    {
    };
}

#endif
