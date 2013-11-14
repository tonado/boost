/*=============================================================================
    Copyright (c) 2013 Daniel James

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/utility/string_ref.hpp>

namespace quickbook
{
    bool glob(boost::string_ref const& pattern,
            boost::string_ref const& filename);
}
