/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>
#include <boost/utility/binary.hpp>
#include <algorithm>
#include <cstddef>

#ifdef BOOST_MSVC
#pragma warning(disable:4996) // warning C4996: 'std::equal': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#endif

/*
Note: This file tests every single valid bit-grouping on its own, and some
      random combinations of bit-groupings.
*/

std::size_t const num_random_test_values = 32;

// Note: These hex values should all correspond with the binary array below
unsigned int const random_unsigned_ints_hex[num_random_test_values]
  = { 0x0103u, 0x77ebu, 0x5f36u, 0x1f18u, 0xc530u, 0xa73au, 0xd6f8u, 0x0919u
    , 0xfbb0u, 0x3e7cu, 0xd0e9u, 0x22c8u, 0x724eu, 0x14fau, 0xd98eu, 0x40b5
    , 0xeba0u, 0xfe50u, 0x688au, 0x1b05u, 0x5f9cu, 0xe4fcu, 0xa7b8u, 0xd3acu
    , 0x1dddu, 0xbf04u, 0x8352u, 0xe89cu, 0x7506u, 0xe767u, 0xf489u, 0xe167
    };

unsigned int const random_unsigned_ints_binary[num_random_test_values]
  = { BOOST_BINARY( 0 00010000 0011 ), BOOST_BINARY( 0 11101 1111  101011 )
    , BOOST_BINARY( 010111 1100110 1 1 0 ), BOOST_BINARY( 000 1 11110 00 11000 )
    , BOOST_BINARY( 110 001010 0110 000 ), BOOST_BINARY( 1010 01110011 1010 )
    , BOOST_BINARY( 11 010 1 101111 1000 ), BOOST_BINARY( 0000 100100 0110 01 )
    , BOOST_BINARY( 1111 101110 11 0000 ), BOOST_BINARY( 00111110 01111100 )
    , BOOST_BINARY( 11  010 000111 01001 ), BOOST_BINARY( 00100 010110   01000 )
    , BOOST_BINARY( 01 11001001 001110 ), BOOST_BINARY( 0010 1001111 1010 )
    , BOOST_BINARY( 1101 1 00110 0 01110 ), BOOST_BINARY( 100 000 01011010 1 )
    , BOOST_BINARY( 11 1010 1110 1000 00 ), BOOST_BINARY( 11111 110010 10000 )
    , BOOST_BINARY( 01101 00010 001010 ), BOOST_BINARY( 000 11011 000001 01 )
    , BOOST_BINARY( 01 01111 1100111 00 ), BOOST_BINARY( 1 110010 0111111 00 )
    , BOOST_BINARY( 101 0011 11 01110 00 ), BOOST_BINARY( 110100 1 110101 100 )
    , BOOST_BINARY( 00 1110111 011 101 ), BOOST_BINARY( 1011 1111 00000 100 )
    , BOOST_BINARY( 1000 00110 101 0010 ), BOOST_BINARY( 1110  10001 001110 0 )
    , BOOST_BINARY( 011 1010100 000 110 ), BOOST_BINARY( 1110 0111 01100 111 )
    , BOOST_BINARY( 11110 10010 001001 ), BOOST_BINARY( 11 1000010 1100 111 )
    };

unsigned int const unsigned_ints_1_bit[2] =
{ BOOST_BINARY( 0 )
, BOOST_BINARY( 1 )
};

unsigned int const unsigned_ints_2_bits[4] =
{ BOOST_BINARY( 00 )
, BOOST_BINARY( 01 )
, BOOST_BINARY( 10 )
, BOOST_BINARY( 11 )
};

unsigned int const unsigned_ints_3_bits[8] =
{ BOOST_BINARY( 000 )
, BOOST_BINARY( 001 )
, BOOST_BINARY( 010 )
, BOOST_BINARY( 011 )
, BOOST_BINARY( 100 )
, BOOST_BINARY( 101 )
, BOOST_BINARY( 110 )
, BOOST_BINARY( 111 )
};

unsigned int const unsigned_ints_4_bits[16] =
{ BOOST_BINARY( 0000 )
, BOOST_BINARY( 0001 )
, BOOST_BINARY( 0010 )
, BOOST_BINARY( 0011 )
, BOOST_BINARY( 0100 )
, BOOST_BINARY( 0101 )
, BOOST_BINARY( 0110 )
, BOOST_BINARY( 0111 )
, BOOST_BINARY( 1000 )
, BOOST_BINARY( 1001 )
, BOOST_BINARY( 1010 )
, BOOST_BINARY( 1011 )
, BOOST_BINARY( 1100 )
, BOOST_BINARY( 1101 )
, BOOST_BINARY( 1110 )
, BOOST_BINARY( 1111 )
};

unsigned int const unsigned_ints_5_bits[32] =
{ BOOST_BINARY( 00000 )
, BOOST_BINARY( 00001 )
, BOOST_BINARY( 00010 )
, BOOST_BINARY( 00011 )
, BOOST_BINARY( 00100 )
, BOOST_BINARY( 00101 )
, BOOST_BINARY( 00110 )
, BOOST_BINARY( 00111 )
, BOOST_BINARY( 01000 )
, BOOST_BINARY( 01001 )
, BOOST_BINARY( 01010 )
, BOOST_BINARY( 01011 )
, BOOST_BINARY( 01100 )
, BOOST_BINARY( 01101 )
, BOOST_BINARY( 01110 )
, BOOST_BINARY( 01111 )
, BOOST_BINARY( 10000 )
, BOOST_BINARY( 10001 )
, BOOST_BINARY( 10010 )
, BOOST_BINARY( 10011 )
, BOOST_BINARY( 10100 )
, BOOST_BINARY( 10101 )
, BOOST_BINARY( 10110 )
, BOOST_BINARY( 10111 )
, BOOST_BINARY( 11000 )
, BOOST_BINARY( 11001 )
, BOOST_BINARY( 11010 )
, BOOST_BINARY( 11011 )
, BOOST_BINARY( 11100 )
, BOOST_BINARY( 11101 )
, BOOST_BINARY( 11110 )
, BOOST_BINARY( 11111 )
};

unsigned int const unsigned_ints_6_bits[64] =
{ BOOST_BINARY( 000000 )
, BOOST_BINARY( 000001 )
, BOOST_BINARY( 000010 )
, BOOST_BINARY( 000011 )
, BOOST_BINARY( 000100 )
, BOOST_BINARY( 000101 )
, BOOST_BINARY( 000110 )
, BOOST_BINARY( 000111 )
, BOOST_BINARY( 001000 )
, BOOST_BINARY( 001001 )
, BOOST_BINARY( 001010 )
, BOOST_BINARY( 001011 )
, BOOST_BINARY( 001100 )
, BOOST_BINARY( 001101 )
, BOOST_BINARY( 001110 )
, BOOST_BINARY( 001111 )
, BOOST_BINARY( 010000 )
, BOOST_BINARY( 010001 )
, BOOST_BINARY( 010010 )
, BOOST_BINARY( 010011 )
, BOOST_BINARY( 010100 )
, BOOST_BINARY( 010101 )
, BOOST_BINARY( 010110 )
, BOOST_BINARY( 010111 )
, BOOST_BINARY( 011000 )
, BOOST_BINARY( 011001 )
, BOOST_BINARY( 011010 )
, BOOST_BINARY( 011011 )
, BOOST_BINARY( 011100 )
, BOOST_BINARY( 011101 )
, BOOST_BINARY( 011110 )
, BOOST_BINARY( 011111 )
, BOOST_BINARY( 100000 )
, BOOST_BINARY( 100001 )
, BOOST_BINARY( 100010 )
, BOOST_BINARY( 100011 )
, BOOST_BINARY( 100100 )
, BOOST_BINARY( 100101 )
, BOOST_BINARY( 100110 )
, BOOST_BINARY( 100111 )
, BOOST_BINARY( 101000 )
, BOOST_BINARY( 101001 )
, BOOST_BINARY( 101010 )
, BOOST_BINARY( 101011 )
, BOOST_BINARY( 101100 )
, BOOST_BINARY( 101101 )
, BOOST_BINARY( 101110 )
, BOOST_BINARY( 101111 )
, BOOST_BINARY( 110000 )
, BOOST_BINARY( 110001 )
, BOOST_BINARY( 110010 )
, BOOST_BINARY( 110011 )
, BOOST_BINARY( 110100 )
, BOOST_BINARY( 110101 )
, BOOST_BINARY( 110110 )
, BOOST_BINARY( 110111 )
, BOOST_BINARY( 111000 )
, BOOST_BINARY( 111001 )
, BOOST_BINARY( 111010 )
, BOOST_BINARY( 111011 )
, BOOST_BINARY( 111100 )
, BOOST_BINARY( 111101 )
, BOOST_BINARY( 111110 )
, BOOST_BINARY( 111111 )
};

unsigned int const unsigned_ints_7_bits[128] =
{ BOOST_BINARY( 0000000 )
, BOOST_BINARY( 0000001 )
, BOOST_BINARY( 0000010 )
, BOOST_BINARY( 0000011 )
, BOOST_BINARY( 0000100 )
, BOOST_BINARY( 0000101 )
, BOOST_BINARY( 0000110 )
, BOOST_BINARY( 0000111 )
, BOOST_BINARY( 0001000 )
, BOOST_BINARY( 0001001 )
, BOOST_BINARY( 0001010 )
, BOOST_BINARY( 0001011 )
, BOOST_BINARY( 0001100 )
, BOOST_BINARY( 0001101 )
, BOOST_BINARY( 0001110 )
, BOOST_BINARY( 0001111 )
, BOOST_BINARY( 0010000 )
, BOOST_BINARY( 0010001 )
, BOOST_BINARY( 0010010 )
, BOOST_BINARY( 0010011 )
, BOOST_BINARY( 0010100 )
, BOOST_BINARY( 0010101 )
, BOOST_BINARY( 0010110 )
, BOOST_BINARY( 0010111 )
, BOOST_BINARY( 0011000 )
, BOOST_BINARY( 0011001 )
, BOOST_BINARY( 0011010 )
, BOOST_BINARY( 0011011 )
, BOOST_BINARY( 0011100 )
, BOOST_BINARY( 0011101 )
, BOOST_BINARY( 0011110 )
, BOOST_BINARY( 0011111 )
, BOOST_BINARY( 0100000 )
, BOOST_BINARY( 0100001 )
, BOOST_BINARY( 0100010 )
, BOOST_BINARY( 0100011 )
, BOOST_BINARY( 0100100 )
, BOOST_BINARY( 0100101 )
, BOOST_BINARY( 0100110 )
, BOOST_BINARY( 0100111 )
, BOOST_BINARY( 0101000 )
, BOOST_BINARY( 0101001 )
, BOOST_BINARY( 0101010 )
, BOOST_BINARY( 0101011 )
, BOOST_BINARY( 0101100 )
, BOOST_BINARY( 0101101 )
, BOOST_BINARY( 0101110 )
, BOOST_BINARY( 0101111 )
, BOOST_BINARY( 0110000 )
, BOOST_BINARY( 0110001 )
, BOOST_BINARY( 0110010 )
, BOOST_BINARY( 0110011 )
, BOOST_BINARY( 0110100 )
, BOOST_BINARY( 0110101 )
, BOOST_BINARY( 0110110 )
, BOOST_BINARY( 0110111 )
, BOOST_BINARY( 0111000 )
, BOOST_BINARY( 0111001 )
, BOOST_BINARY( 0111010 )
, BOOST_BINARY( 0111011 )
, BOOST_BINARY( 0111100 )
, BOOST_BINARY( 0111101 )
, BOOST_BINARY( 0111110 )
, BOOST_BINARY( 0111111 )
, BOOST_BINARY( 1000000 )
, BOOST_BINARY( 1000001 )
, BOOST_BINARY( 1000010 )
, BOOST_BINARY( 1000011 )
, BOOST_BINARY( 1000100 )
, BOOST_BINARY( 1000101 )
, BOOST_BINARY( 1000110 )
, BOOST_BINARY( 1000111 )
, BOOST_BINARY( 1001000 )
, BOOST_BINARY( 1001001 )
, BOOST_BINARY( 1001010 )
, BOOST_BINARY( 1001011 )
, BOOST_BINARY( 1001100 )
, BOOST_BINARY( 1001101 )
, BOOST_BINARY( 1001110 )
, BOOST_BINARY( 1001111 )
, BOOST_BINARY( 1010000 )
, BOOST_BINARY( 1010001 )
, BOOST_BINARY( 1010010 )
, BOOST_BINARY( 1010011 )
, BOOST_BINARY( 1010100 )
, BOOST_BINARY( 1010101 )
, BOOST_BINARY( 1010110 )
, BOOST_BINARY( 1010111 )
, BOOST_BINARY( 1011000 )
, BOOST_BINARY( 1011001 )
, BOOST_BINARY( 1011010 )
, BOOST_BINARY( 1011011 )
, BOOST_BINARY( 1011100 )
, BOOST_BINARY( 1011101 )
, BOOST_BINARY( 1011110 )
, BOOST_BINARY( 1011111 )
, BOOST_BINARY( 1100000 )
, BOOST_BINARY( 1100001 )
, BOOST_BINARY( 1100010 )
, BOOST_BINARY( 1100011 )
, BOOST_BINARY( 1100100 )
, BOOST_BINARY( 1100101 )
, BOOST_BINARY( 1100110 )
, BOOST_BINARY( 1100111 )
, BOOST_BINARY( 1101000 )
, BOOST_BINARY( 1101001 )
, BOOST_BINARY( 1101010 )
, BOOST_BINARY( 1101011 )
, BOOST_BINARY( 1101100 )
, BOOST_BINARY( 1101101 )
, BOOST_BINARY( 1101110 )
, BOOST_BINARY( 1101111 )
, BOOST_BINARY( 1110000 )
, BOOST_BINARY( 1110001 )
, BOOST_BINARY( 1110010 )
, BOOST_BINARY( 1110011 )
, BOOST_BINARY( 1110100 )
, BOOST_BINARY( 1110101 )
, BOOST_BINARY( 1110110 )
, BOOST_BINARY( 1110111 )
, BOOST_BINARY( 1111000 )
, BOOST_BINARY( 1111001 )
, BOOST_BINARY( 1111010 )
, BOOST_BINARY( 1111011 )
, BOOST_BINARY( 1111100 )
, BOOST_BINARY( 1111101 )
, BOOST_BINARY( 1111110 )
, BOOST_BINARY( 1111111 )
};
unsigned int const unsigned_ints_8_bits[256] =
{ BOOST_BINARY( 00000000 )
, BOOST_BINARY( 00000001 )
, BOOST_BINARY( 00000010 )
, BOOST_BINARY( 00000011 )
, BOOST_BINARY( 00000100 )
, BOOST_BINARY( 00000101 )
, BOOST_BINARY( 00000110 )
, BOOST_BINARY( 00000111 )
, BOOST_BINARY( 00001000 )
, BOOST_BINARY( 00001001 )
, BOOST_BINARY( 00001010 )
, BOOST_BINARY( 00001011 )
, BOOST_BINARY( 00001100 )
, BOOST_BINARY( 00001101 )
, BOOST_BINARY( 00001110 )
, BOOST_BINARY( 00001111 )
, BOOST_BINARY( 00010000 )
, BOOST_BINARY( 00010001 )
, BOOST_BINARY( 00010010 )
, BOOST_BINARY( 00010011 )
, BOOST_BINARY( 00010100 )
, BOOST_BINARY( 00010101 )
, BOOST_BINARY( 00010110 )
, BOOST_BINARY( 00010111 )
, BOOST_BINARY( 00011000 )
, BOOST_BINARY( 00011001 )
, BOOST_BINARY( 00011010 )
, BOOST_BINARY( 00011011 )
, BOOST_BINARY( 00011100 )
, BOOST_BINARY( 00011101 )
, BOOST_BINARY( 00011110 )
, BOOST_BINARY( 00011111 )
, BOOST_BINARY( 00100000 )
, BOOST_BINARY( 00100001 )
, BOOST_BINARY( 00100010 )
, BOOST_BINARY( 00100011 )
, BOOST_BINARY( 00100100 )
, BOOST_BINARY( 00100101 )
, BOOST_BINARY( 00100110 )
, BOOST_BINARY( 00100111 )
, BOOST_BINARY( 00101000 )
, BOOST_BINARY( 00101001 )
, BOOST_BINARY( 00101010 )
, BOOST_BINARY( 00101011 )
, BOOST_BINARY( 00101100 )
, BOOST_BINARY( 00101101 )
, BOOST_BINARY( 00101110 )
, BOOST_BINARY( 00101111 )
, BOOST_BINARY( 00110000 )
, BOOST_BINARY( 00110001 )
, BOOST_BINARY( 00110010 )
, BOOST_BINARY( 00110011 )
, BOOST_BINARY( 00110100 )
, BOOST_BINARY( 00110101 )
, BOOST_BINARY( 00110110 )
, BOOST_BINARY( 00110111 )
, BOOST_BINARY( 00111000 )
, BOOST_BINARY( 00111001 )
, BOOST_BINARY( 00111010 )
, BOOST_BINARY( 00111011 )
, BOOST_BINARY( 00111100 )
, BOOST_BINARY( 00111101 )
, BOOST_BINARY( 00111110 )
, BOOST_BINARY( 00111111 )
, BOOST_BINARY( 01000000 )
, BOOST_BINARY( 01000001 )
, BOOST_BINARY( 01000010 )
, BOOST_BINARY( 01000011 )
, BOOST_BINARY( 01000100 )
, BOOST_BINARY( 01000101 )
, BOOST_BINARY( 01000110 )
, BOOST_BINARY( 01000111 )
, BOOST_BINARY( 01001000 )
, BOOST_BINARY( 01001001 )
, BOOST_BINARY( 01001010 )
, BOOST_BINARY( 01001011 )
, BOOST_BINARY( 01001100 )
, BOOST_BINARY( 01001101 )
, BOOST_BINARY( 01001110 )
, BOOST_BINARY( 01001111 )
, BOOST_BINARY( 01010000 )
, BOOST_BINARY( 01010001 )
, BOOST_BINARY( 01010010 )
, BOOST_BINARY( 01010011 )
, BOOST_BINARY( 01010100 )
, BOOST_BINARY( 01010101 )
, BOOST_BINARY( 01010110 )
, BOOST_BINARY( 01010111 )
, BOOST_BINARY( 01011000 )
, BOOST_BINARY( 01011001 )
, BOOST_BINARY( 01011010 )
, BOOST_BINARY( 01011011 )
, BOOST_BINARY( 01011100 )
, BOOST_BINARY( 01011101 )
, BOOST_BINARY( 01011110 )
, BOOST_BINARY( 01011111 )
, BOOST_BINARY( 01100000 )
, BOOST_BINARY( 01100001 )
, BOOST_BINARY( 01100010 )
, BOOST_BINARY( 01100011 )
, BOOST_BINARY( 01100100 )
, BOOST_BINARY( 01100101 )
, BOOST_BINARY( 01100110 )
, BOOST_BINARY( 01100111 )
, BOOST_BINARY( 01101000 )
, BOOST_BINARY( 01101001 )
, BOOST_BINARY( 01101010 )
, BOOST_BINARY( 01101011 )
, BOOST_BINARY( 01101100 )
, BOOST_BINARY( 01101101 )
, BOOST_BINARY( 01101110 )
, BOOST_BINARY( 01101111 )
, BOOST_BINARY( 01110000 )
, BOOST_BINARY( 01110001 )
, BOOST_BINARY( 01110010 )
, BOOST_BINARY( 01110011 )
, BOOST_BINARY( 01110100 )
, BOOST_BINARY( 01110101 )
, BOOST_BINARY( 01110110 )
, BOOST_BINARY( 01110111 )
, BOOST_BINARY( 01111000 )
, BOOST_BINARY( 01111001 )
, BOOST_BINARY( 01111010 )
, BOOST_BINARY( 01111011 )
, BOOST_BINARY( 01111100 )
, BOOST_BINARY( 01111101 )
, BOOST_BINARY( 01111110 )
, BOOST_BINARY( 01111111 )
, BOOST_BINARY( 10000000 )
, BOOST_BINARY( 10000001 )
, BOOST_BINARY( 10000010 )
, BOOST_BINARY( 10000011 )
, BOOST_BINARY( 10000100 )
, BOOST_BINARY( 10000101 )
, BOOST_BINARY( 10000110 )
, BOOST_BINARY( 10000111 )
, BOOST_BINARY( 10001000 )
, BOOST_BINARY( 10001001 )
, BOOST_BINARY( 10001010 )
, BOOST_BINARY( 10001011 )
, BOOST_BINARY( 10001100 )
, BOOST_BINARY( 10001101 )
, BOOST_BINARY( 10001110 )
, BOOST_BINARY( 10001111 )
, BOOST_BINARY( 10010000 )
, BOOST_BINARY( 10010001 )
, BOOST_BINARY( 10010010 )
, BOOST_BINARY( 10010011 )
, BOOST_BINARY( 10010100 )
, BOOST_BINARY( 10010101 )
, BOOST_BINARY( 10010110 )
, BOOST_BINARY( 10010111 )
, BOOST_BINARY( 10011000 )
, BOOST_BINARY( 10011001 )
, BOOST_BINARY( 10011010 )
, BOOST_BINARY( 10011011 )
, BOOST_BINARY( 10011100 )
, BOOST_BINARY( 10011101 )
, BOOST_BINARY( 10011110 )
, BOOST_BINARY( 10011111 )
, BOOST_BINARY( 10100000 )
, BOOST_BINARY( 10100001 )
, BOOST_BINARY( 10100010 )
, BOOST_BINARY( 10100011 )
, BOOST_BINARY( 10100100 )
, BOOST_BINARY( 10100101 )
, BOOST_BINARY( 10100110 )
, BOOST_BINARY( 10100111 )
, BOOST_BINARY( 10101000 )
, BOOST_BINARY( 10101001 )
, BOOST_BINARY( 10101010 )
, BOOST_BINARY( 10101011 )
, BOOST_BINARY( 10101100 )
, BOOST_BINARY( 10101101 )
, BOOST_BINARY( 10101110 )
, BOOST_BINARY( 10101111 )
, BOOST_BINARY( 10110000 )
, BOOST_BINARY( 10110001 )
, BOOST_BINARY( 10110010 )
, BOOST_BINARY( 10110011 )
, BOOST_BINARY( 10110100 )
, BOOST_BINARY( 10110101 )
, BOOST_BINARY( 10110110 )
, BOOST_BINARY( 10110111 )
, BOOST_BINARY( 10111000 )
, BOOST_BINARY( 10111001 )
, BOOST_BINARY( 10111010 )
, BOOST_BINARY( 10111011 )
, BOOST_BINARY( 10111100 )
, BOOST_BINARY( 10111101 )
, BOOST_BINARY( 10111110 )
, BOOST_BINARY( 10111111 )
, BOOST_BINARY( 11000000 )
, BOOST_BINARY( 11000001 )
, BOOST_BINARY( 11000010 )
, BOOST_BINARY( 11000011 )
, BOOST_BINARY( 11000100 )
, BOOST_BINARY( 11000101 )
, BOOST_BINARY( 11000110 )
, BOOST_BINARY( 11000111 )
, BOOST_BINARY( 11001000 )
, BOOST_BINARY( 11001001 )
, BOOST_BINARY( 11001010 )
, BOOST_BINARY( 11001011 )
, BOOST_BINARY( 11001100 )
, BOOST_BINARY( 11001101 )
, BOOST_BINARY( 11001110 )
, BOOST_BINARY( 11001111 )
, BOOST_BINARY( 11010000 )
, BOOST_BINARY( 11010001 )
, BOOST_BINARY( 11010010 )
, BOOST_BINARY( 11010011 )
, BOOST_BINARY( 11010100 )
, BOOST_BINARY( 11010101 )
, BOOST_BINARY( 11010110 )
, BOOST_BINARY( 11010111 )
, BOOST_BINARY( 11011000 )
, BOOST_BINARY( 11011001 )
, BOOST_BINARY( 11011010 )
, BOOST_BINARY( 11011011 )
, BOOST_BINARY( 11011100 )
, BOOST_BINARY( 11011101 )
, BOOST_BINARY( 11011110 )
, BOOST_BINARY( 11011111 )
, BOOST_BINARY( 11100000 )
, BOOST_BINARY( 11100001 )
, BOOST_BINARY( 11100010 )
, BOOST_BINARY( 11100011 )
, BOOST_BINARY( 11100100 )
, BOOST_BINARY( 11100101 )
, BOOST_BINARY( 11100110 )
, BOOST_BINARY( 11100111 )
, BOOST_BINARY( 11101000 )
, BOOST_BINARY( 11101001 )
, BOOST_BINARY( 11101010 )
, BOOST_BINARY( 11101011 )
, BOOST_BINARY( 11101100 )
, BOOST_BINARY( 11101101 )
, BOOST_BINARY( 11101110 )
, BOOST_BINARY( 11101111 )
, BOOST_BINARY( 11110000 )
, BOOST_BINARY( 11110001 )
, BOOST_BINARY( 11110010 )
, BOOST_BINARY( 11110011 )
, BOOST_BINARY( 11110100 )
, BOOST_BINARY( 11110101 )
, BOOST_BINARY( 11110110 )
, BOOST_BINARY( 11110111 )
, BOOST_BINARY( 11111000 )
, BOOST_BINARY( 11111001 )
, BOOST_BINARY( 11111010 )
, BOOST_BINARY( 11111011 )
, BOOST_BINARY( 11111100 )
, BOOST_BINARY( 11111101 )
, BOOST_BINARY( 11111110 )
, BOOST_BINARY( 11111111 )
};

struct left_is_not_one_less_than_right
{
  bool operator ()( unsigned int left, unsigned int right ) const
  {
    return right != left + 1;
  }
};

template< std::size_t Size >
bool is_ascending_from_0_array( unsigned int const (&array)[Size] )
{
  unsigned int const* const curr = array,
                    * const end  = array + Size;

  return    ( *curr == 0 )
         && (    std::adjacent_find( curr, end
                                   , left_is_not_one_less_than_right()
                                   )
              == end
            );
}

std::size_t const unsigned_int_id = 1,
                  unsigned_long_int_id = 2;

typedef char (&unsigned_int_id_type)[unsigned_int_id];
typedef char (&unsigned_long_int_id_type)[unsigned_long_int_id];

// Note: Functions only used for type checking
unsigned_int_id_type      binary_type_checker( unsigned int );
unsigned_long_int_id_type binary_type_checker( unsigned long int );

int test_main( int, char *[] )
{
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_1_bit ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_2_bits ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_3_bits ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_4_bits ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_5_bits ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_6_bits ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_7_bits ) );
  BOOST_CHECK( is_ascending_from_0_array( unsigned_ints_8_bits ) );

  BOOST_CHECK( std::equal( &random_unsigned_ints_hex[0]
                         , random_unsigned_ints_hex + num_random_test_values
                         , &random_unsigned_ints_binary[0]
                         )
             );

  BOOST_CHECK(    sizeof( binary_type_checker( BOOST_BINARY_U( 110100 1010 ) ) )
               == unsigned_int_id
             );

  BOOST_CHECK(    sizeof( binary_type_checker( BOOST_BINARY_UL( 11110 ) ) )
               == unsigned_long_int_id
             );

  BOOST_CHECK(    sizeof( binary_type_checker( BOOST_BINARY_LU( 10 0001 ) ) )
               == unsigned_long_int_id
             );

  return 0;
}
