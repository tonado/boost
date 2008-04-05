#ifndef PORTABLE_BINARY_ARCHIVE_HPP
#define PORTABLE_BINARY_ARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/pfto.hpp>
#include <boost/static_assert.hpp>

#include <climits>
#if CHAR_BIT != 8
#error This code assumes an eight-bit byte.
#endif

#include <boost/archive/basic_archive.hpp>
#include <boost/detail/endian.hpp>

enum portable_binary_archive_flags {
    endian_big        = 0x4000,
    endian_little     = 0x8000
};

//#if ( endian_big <= boost::archive::flags_last )
//#error archive flags conflict
//#endif

inline void
reverse_bytes(char size, char *address){
    char * first = address;
    char * last = first + size - 1;
    for(;first < last;++first, --last){
        char x = *last;
        *last = *first;
        *first = x;
    }
}

#endif // PORTABLE_BINARY_ARCHIVE_HPP
