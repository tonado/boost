//  Boost big_whole_test.cpp test file  --------------------------------------//

//  Copyright 2004 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/math/> for the library's home page.

//  Revision History
//   06 Feb 2004  Initial version (Daryle Walker)

#include <boost/math/big_whole.hpp>  // for boost::math::big_whole, etc.
#include <boost/test/unit_test.hpp>  // for main, BOOST_CHECK_EQUAL, etc.

#include <algorithm>  // for std::copy
#include <cstddef>    // for std::size_t
#include <limits>     // for std::numeric_limits
#include <set>        // for std::set
#include <valarray>   // for std::valarray


// Use internal knowledge of big_whole (i.e. cheat) to force situations
// where multiple-word representations have to be used
typedef unsigned int                       word_type;
typedef std::numeric_limits<word_type>  wlimits_type;


// Helper function to compare valarrays
template < typename T >
bool
equal_valarrays
(
    std::valarray<T> const &  lhs,
    std::valarray<T> const &  rhs
)
{
    using std::valarray;

    if ( lhs.size() == rhs.size() )
    {
        valarray<std::size_t>  s( lhs.size() );

        s[ lhs == rhs ] = 1;
        return s.sum() == s.size();
    }

    return false;
}

// Helper function to insert values into sets
template < class SetType, typename ValueType >
void
insert_value_range
(
    SetType &  s,
    ValueType  start,
    ValueType  finish
)
{
    for ( ValueType i = start ; i <= finish ; ++i )
    {
        s.insert( i );
    }
}

// Helper function to remove values from sets
template < class SetType, typename ValueType >
void
erase_value_range
(
    SetType &  s,
    ValueType  start,
    ValueType  finish
)
{
    for ( ValueType i = start ; i <= finish ; ++i )
    {
        s.erase( i );
    }
}

// Helper function to convert sets to valarrays
template < typename T >
std::valarray<T>
set_to_valarray
(
    std::set<T> const &  s
)
{
    std::valarray<T>  temp( s.size() );

    std::copy( s.begin(), s.end(), &temp[0] );
    return temp;
}


// Unit test for the basics
void
basic_bigwhole_unit_test
(
)
{
    using boost::math::big_whole;
    using std::valarray;
    using std::size_t;

    typedef valarray<bool>    va_bool_t;
    typedef valarray<size_t>  va_size_t;

    // Default construction
    big_whole  x1;
    BOOST_CHECK_EQUAL( 0u, x1.to_uintmax() );

    // Converting assignment
    x1.assign( 5u );
    BOOST_CHECK_EQUAL( 5u, x1.to_uintmax() );

    // Converting construction
    big_whole  x2 = 17;
    BOOST_CHECK_EQUAL( 17u, x2.to_uintmax() );

    // Copy construction
    big_whole  x3( x1 );
    BOOST_CHECK_EQUAL( 5u, x3.to_uintmax() );

    // Assignment operator
    x1 = x2;
    BOOST_CHECK_EQUAL( 17u, x1.to_uintmax() );

    // Swapping
    swap( x1, x3 );
    BOOST_CHECK_EQUAL( 5u, x1.to_uintmax() );
    BOOST_CHECK_EQUAL( 17u, x3.to_uintmax() );

    // Copying assignment
    x2.assign( big_whole() );
    BOOST_CHECK_EQUAL( 0u, x2.to_uintmax() );

    // Bit-vector conversion
    va_bool_t const  x1_b = x1.to_bit_vector();
    bool const       x1_b_check[] = { true, false, true };
    size_t const     x1_b_size = sizeof( x1_b_check ) / sizeof( x1_b_check[0] );
    BOOST_CHECK( equal_valarrays(va_bool_t( x1_b_check, x1_b_size ), x1_b) );

    BOOST_CHECK_EQUAL( 0u, x2.to_bit_vector().size() );

    va_bool_t const  x3_b = x3.to_bit_vector();
    bool const       x3_b_check[] = { true, false, false, false, true };
    size_t const     x3_b_size = sizeof( x3_b_check ) / sizeof( x3_b_check[0] );
    BOOST_CHECK( equal_valarrays(va_bool_t( x3_b_check, x3_b_size ), x3_b) );

    // Bit-index conversion
    va_size_t const  x1_i = x1.to_bit_indices();
    size_t const     x1_i_check[] = { 0, 2 };
    size_t const     x1_i_size = sizeof( x1_i_check ) / sizeof( x1_i_check[0] );
    BOOST_CHECK( equal_valarrays(va_size_t( x1_i_check, x1_i_size ), x1_i) );
    BOOST_CHECK_EQUAL( x1_b_size, 1u + x1_i.max() );

    BOOST_CHECK_EQUAL( 0u, x2.to_bit_indices().size() );

    va_size_t const  x3_i = x3.to_bit_indices();
    size_t const     x3_i_check[] = { 0, 4 };
    size_t const     x3_i_size = sizeof( x3_i_check ) / sizeof( x3_i_check[0] );
    BOOST_CHECK( equal_valarrays(va_size_t( x3_i_check, x3_i_size ), x3_i) );
    BOOST_CHECK_EQUAL( x3_b_size, 1u + x3_i.max() );

    // Bit-vector construction and assignment
    big_whole  x4( x1_b );
    BOOST_CHECK_EQUAL( 5u, x4.to_uintmax() );

    x4.reconfigure( x3_b );
    BOOST_CHECK_EQUAL( 17u, x4.to_uintmax() );

    x4.reconfigure( va_bool_t() );
    BOOST_CHECK_EQUAL( 0u, x4.to_uintmax() );

    // Bit-index construction and assignment
    big_whole  x5( x3_i );
    BOOST_CHECK_EQUAL( 17u, x5.to_uintmax() );

    x5.reconfigure( x1_i );
    BOOST_CHECK_EQUAL( 5u, x5.to_uintmax() );

    x5.reconfigure( va_size_t() );
    BOOST_CHECK_EQUAL( 0u, x5.to_uintmax() );

    // Minimum-required bit length
    BOOST_CHECK_EQUAL( x1_b_size, x1.length() );
    BOOST_CHECK_EQUAL( 0u, x2.length() );
    BOOST_CHECK_EQUAL( x3_b_size, x3.length() );

    // Bit count
    BOOST_CHECK_EQUAL( x1_i_size, x1.count() );
    BOOST_CHECK_EQUAL( 0u, x2.count() );
    BOOST_CHECK_EQUAL( x3_i_size, x3.count() );

    BOOST_CHECK( x1.any() );
    BOOST_CHECK( !x2.any() );
    BOOST_CHECK( x3.any() );

    BOOST_CHECK( !x1.none() );
    BOOST_CHECK( x2.none() );
    BOOST_CHECK( !x3.none() );

    // Bit testing
    BOOST_CHECK( x1.test(0) && !x1.test(1) && x1.test(2) && !x1.test(3)
     && !x1.test(4) && !x1.test(5) && !x1.test(wlimits_type::digits) );
    BOOST_CHECK( !x2.test(0) && !x2.test(1) && !x2.test(2) && !x2.test(3)
     && !x2.test(4) && !x2.test(5) && !x2.test(wlimits_type::digits) );
    BOOST_CHECK( x3.test(0) && !x3.test(1) && !x3.test(2) && !x3.test(3)
     && x3.test(4) && !x3.test(5) && !x3.test(wlimits_type::digits) );

    // Boolean test
    BOOST_CHECK( x1 );
    BOOST_CHECK( !x2 );
    BOOST_CHECK( x3 );
}

// Unit test for "tests"
void
bigwhole_multi_bit_check_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    size_t const  size_max = std::numeric_limits<size_t>::max();

    // Non-zero tests
    big_whole const  x1( 74u );
    size_t const     l1 = x1.length();

    BOOST_CHECK_EQUAL( 7u, l1 );

    BOOST_CHECK_EQUAL( 74u, x1.tests(0, size_max).to_uintmax() );
    BOOST_CHECK_EQUAL( 74u, x1.tests(0, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 74u, x1.tests(0, l1 - 1).to_uintmax() );
    BOOST_CHECK_EQUAL( 10u, x1.tests(0, 5).to_uintmax() );
    BOOST_CHECK_EQUAL( 10u, x1.tests(0, 4).to_uintmax() );
    BOOST_CHECK_EQUAL( 10u, x1.tests(0, 3).to_uintmax() );
    BOOST_CHECK_EQUAL( 2u, x1.tests(0, 2).to_uintmax() );
    BOOST_CHECK_EQUAL( 2u, x1.tests(0, 1).to_uintmax() );
    BOOST_CHECK_EQUAL( 0u, x1.tests(0, 0).to_uintmax() );

    BOOST_CHECK_EQUAL( 37u, x1.tests(1, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 18u, x1.tests(2, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 9u, x1.tests(3, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 4u, x1.tests(4, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 2u, x1.tests(5, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 1u, x1.tests(l1 - 1, l1).to_uintmax() );
    BOOST_CHECK_EQUAL( 0u, x1.tests(l1, l1).to_uintmax() );

    BOOST_CHECK( x1.tests(1, 1) );
    BOOST_CHECK( !x1.tests(2, 2) );
    BOOST_CHECK( x1.tests(3, 3) );
    BOOST_CHECK( !x1.tests(4, 4) );
    BOOST_CHECK( !x1.tests(5, 5) );
    BOOST_CHECK( x1.tests(l1 - 1, l1 - 1) );

    // Zero tests
    big_whole const  x2;

    BOOST_CHECK( !x2.tests(0, 4) );
    BOOST_CHECK( !x2.tests(2, size_max) );
    BOOST_CHECK( !x2.tests(0, size_max) );
    BOOST_CHECK( !x2.tests(3, 3) );
}

// Unit test for reversing
void
bigwhole_reverse_unit_test
(
)
{
    using boost::math::big_whole;

    // Non-zero tests
    big_whole const  x1( 1 );

    BOOST_CHECK_EQUAL( 1u, x1.reverse().to_uintmax() );
    BOOST_CHECK_EQUAL( 1u, x1.length() );

    BOOST_CHECK_EQUAL( 1u, x1.reverse(0).to_uintmax() );
    BOOST_CHECK_EQUAL( 2u, x1.reverse(1).to_uintmax() );
    BOOST_CHECK_EQUAL( 4u, x1.reverse(2).to_uintmax() );
    BOOST_CHECK_EQUAL( 128u, x1.reverse(7).to_uintmax() );

    big_whole const  x2( 5 );

    BOOST_CHECK_EQUAL( 5u, x2.reverse().to_uintmax() );
    BOOST_CHECK_EQUAL( 3u, x2.length() );

    BOOST_CHECK_EQUAL( 1u, x2.reverse(0).to_uintmax() );
    BOOST_CHECK_EQUAL( 2u, x2.reverse(1).to_uintmax() );
    BOOST_CHECK_EQUAL( 5u, x2.reverse(2).to_uintmax() );
    BOOST_CHECK_EQUAL( 10u, x2.reverse(3).to_uintmax() );
    BOOST_CHECK_EQUAL( 20u, x2.reverse(4).to_uintmax() );
    BOOST_CHECK_EQUAL( 160u, x2.reverse(7).to_uintmax() );

    big_whole const  x3( 74 );

    BOOST_CHECK_EQUAL( 41u, x3.reverse().to_uintmax() );
    BOOST_CHECK_EQUAL( 7u, x3.length() );

    BOOST_CHECK_EQUAL( 0u, x3.reverse(0).to_uintmax() );
    BOOST_CHECK_EQUAL( 1u, x3.reverse(1).to_uintmax() );
    BOOST_CHECK_EQUAL( 2u, x3.reverse(2).to_uintmax() );
    BOOST_CHECK_EQUAL( 5u, x3.reverse(3).to_uintmax() );
    BOOST_CHECK_EQUAL( 10u, x3.reverse(4).to_uintmax() );
    BOOST_CHECK_EQUAL( 20u, x3.reverse(5).to_uintmax() );
    BOOST_CHECK_EQUAL( 41u, x3.reverse(6).to_uintmax() );
    BOOST_CHECK_EQUAL( 82u, x3.reverse(7).to_uintmax() );
    BOOST_CHECK_EQUAL( 164u, x3.reverse(8).to_uintmax() );
    BOOST_CHECK_EQUAL( 656u, x3.reverse(10).to_uintmax() );

    // Zero tests
    big_whole const  x4;

    BOOST_CHECK( !x4.length() );
    BOOST_CHECK( !x4.reverse() );
    BOOST_CHECK( !x4.reverse(0) );
    BOOST_CHECK( !x4.reverse(2 * wlimits_type::digits) );
}

// Unit test for resetting every bit
void
bigwhole_all_bit_reset_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK( !a1 );

    a1.reset();
    BOOST_CHECK( !a1 );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK( a2 );

    a2.reset();
    BOOST_CHECK( !a2 );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK( a3 );

    a3.reset();
    BOOST_CHECK( !a3 );

    // two words
    size_t const  a4_i[] = { 0, wlimits_type::digits + 1 };
    size_t const  a4_s = sizeof( a4_i ) / sizeof( a4_i[0] );
    big_whole     a4( va_size_t(a4_i, a4_s) );

    BOOST_CHECK( a4 );

    a4.reset();
    BOOST_CHECK( !a4 );

    // more-than-two words
    size_t const  a5_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const  a5_s = sizeof( a5_i ) / sizeof( a5_i[0] );
    big_whole     a5( va_size_t(a5_i, a5_s) );

    BOOST_CHECK( a5 );

    a5.reset();
    BOOST_CHECK( !a5 );
}

// Unit test for resetting single bits
void
bigwhole_single_bit_reset_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    a1.reset( 3 );
    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.reset( 2 );
    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.reset( 3 );
    BOOST_CHECK_EQUAL( 0u, a2.to_uintmax() );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.reset( 0 );
    BOOST_CHECK_EQUAL( 24u, a3.to_uintmax() );

    a3.reset( 1 );
    BOOST_CHECK_EQUAL( 24u, a3.to_uintmax() );

    a3.reset( 4 );
    BOOST_CHECK_EQUAL( 8u, a3.to_uintmax() );

    // two words
    size_t const     a4_old_i[] = { 0, wlimits_type::digits + 1 };
    size_t const     a4_old_s = sizeof( a4_old_i ) / sizeof( a4_old_i[0] );
    va_size_t const  a4_old( a4_old_i, a4_old_s );
    big_whole        a4( a4_old );

    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    a4.reset( 5 );
    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    a4.reset( 0 );
    BOOST_CHECK( equal_valarrays(va_size_t( a4_old_i + 1, a4_old_s - 1 ), a4.to_bit_indices()) );

    // more-than-two words
    size_t const     a5_old_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_old_s = sizeof( a5_old_i ) / sizeof( a5_old_i[0] );
    va_size_t const  a5_old( a5_old_i, a5_old_s );
    big_whole        a5( a5_old );

    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    a5.reset( 4 );
    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    a5.reset( 2 * wlimits_type::digits + 5 );
    BOOST_CHECK( equal_valarrays(va_size_t( a5_old_i, a5_old_s - 1 ), a5.to_bit_indices()) );
}

// Unit test for resetting a group of bits
void
bigwhole_group_bit_reset_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    a1.reset( 3, 7 );
    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.reset( 6, 9 );
    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.reset( 2, 3 );
    BOOST_CHECK_EQUAL( 0u, a2.to_uintmax() );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.reset( 3, 3 );
    BOOST_CHECK_EQUAL( 17u, a3.to_uintmax() );

    a3.reset( 1, 2 );
    BOOST_CHECK_EQUAL( 17u, a3.to_uintmax() );

    a3.reset( 2, 6 );
    BOOST_CHECK_EQUAL( 1u, a3.to_uintmax() );

    // two words
    size_t const     a4_old_i[] = { 0, wlimits_type::digits + 1 };
    size_t const     a4_old_s = sizeof( a4_old_i ) / sizeof( a4_old_i[0] );
    va_size_t const  a4_old( a4_old_i, a4_old_s );
    big_whole        a4( a4_old );

    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    a4.reset( 5, 12 );
    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    a4.reset( 9, 2 * wlimits_type::digits );
    BOOST_CHECK( equal_valarrays(va_size_t( a4_old_i, a4_old_s - 1 ), a4.to_bit_indices()) );

    // more-than-two words
    size_t const     a5_old_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_old_s = sizeof( a5_old_i ) / sizeof( a5_old_i[0] );
    va_size_t const  a5_old( a5_old_i, a5_old_s );
    big_whole        a5( a5_old );

    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    a5.reset( 3, 12 );
    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    size_t const  a5_new_i[] = { 1, 2 * wlimits_type::digits + 5 };
    size_t const  a5_new_s = sizeof( a5_new_i ) / sizeof( a5_new_i[0] );

    a5.reset( wlimits_type::digits - 1, 2 * wlimits_type::digits + 1 );
    BOOST_CHECK( equal_valarrays(va_size_t( a5_new_i, a5_new_s ), a5.to_bit_indices()) );

    a5.reset( 2 * wlimits_type::digits + 1, 3 * wlimits_type::digits );
    BOOST_CHECK( equal_valarrays(va_size_t( 1u, 1 ), a5.to_bit_indices()) );

    a5.reset( wlimits_type::digits + 1, 3 * wlimits_type::digits );
    BOOST_CHECK( equal_valarrays(va_size_t( 1u, 1 ), a5.to_bit_indices()) );
}

// Unit test for setting single bits
void
bigwhole_single_bit_set_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    a1.set( 3 );
    BOOST_CHECK_EQUAL( 8u, a1.to_uintmax() );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.set( 2 );
    BOOST_CHECK_EQUAL( 12u, a2.to_uintmax() );

    a2.set( 3 );
    BOOST_CHECK_EQUAL( 12u, a2.to_uintmax() );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.set( 0 );
    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.set( 1 );
    BOOST_CHECK_EQUAL( 27u, a3.to_uintmax() );

    a3.set( 4 );
    BOOST_CHECK_EQUAL( 27u, a3.to_uintmax() );

    // two words
    size_t const     a4_old_i[] = { 0, wlimits_type::digits + 1 };
    size_t const     a4_old_s = sizeof( a4_old_i ) / sizeof( a4_old_i[0] );
    va_size_t const  a4_old( a4_old_i, a4_old_s );
    big_whole        a4( a4_old );

    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    size_t const     a4_new_i[] = { 0, 5, wlimits_type::digits + 1 };
    size_t const     a4_new_s = sizeof( a4_new_i ) / sizeof( a4_new_i[0] );
    va_size_t const  a4_new( a4_new_i, a4_new_s );

    a4.set( 5 );
    BOOST_CHECK( equal_valarrays(a4_new, a4.to_bit_indices()) );

    a4.set( 0 );
    BOOST_CHECK( equal_valarrays(a4_new, a4.to_bit_indices()) );

    // more-than-two words
    size_t const     a5_old_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_old_s = sizeof( a5_old_i ) / sizeof( a5_old_i[0] );
    va_size_t const  a5_old( a5_old_i, a5_old_s );
    big_whole        a5( a5_old );

    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    size_t const     a5_new_i[] = { 1, 4, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_new_s = sizeof( a5_new_i ) / sizeof( a5_new_i[0] );
    va_size_t const  a5_new( a5_new_i, a5_new_s );

    a5.set( 4 );
    BOOST_CHECK( equal_valarrays(a5_new, a5.to_bit_indices()) );

    a5.set( 2 * wlimits_type::digits + 5 );
    BOOST_CHECK( equal_valarrays(a5_new, a5.to_bit_indices()) );
}

// Unit test for setting a group of bits
void
bigwhole_group_bit_set_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;
    typedef std::set<size_t>       st_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    a1.set( 3, 7 );
    BOOST_CHECK_EQUAL( 248u, a1.to_uintmax() );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.set( 6, 9 );
    BOOST_CHECK_EQUAL( 968u, a2.to_uintmax() );

    a2.set( 2, 3 );
    BOOST_CHECK_EQUAL( 972u, a2.to_uintmax() );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.set( 3, 3 );
    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.set( 1, 2 );
    BOOST_CHECK_EQUAL( 31u, a3.to_uintmax() );

    a3.set( 2, 6 );
    BOOST_CHECK_EQUAL( 127u, a3.to_uintmax() );

    // two words
    size_t const     a4_old_i[] = { 0, wlimits_type::digits + 1 };
    size_t const     a4_old_s = sizeof( a4_old_i ) / sizeof( a4_old_i[0] );
    va_size_t const  a4_old( a4_old_i, a4_old_s );
    big_whole        a4( a4_old );
    st_size_t        a4_new( a4_old_i, a4_old_i + a4_old_s );

    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    a4.set( 5, 12 );
    insert_value_range( a4_new, 5, 12 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a4_new ), a4.to_bit_indices()) );

    a4.set( 9, 2 * wlimits_type::digits );
    insert_value_range( a4_new, 9, 2 * wlimits_type::digits );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a4_new ), a4.to_bit_indices()) );

    // more-than-two words
    size_t const     a5_old_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_old_s = sizeof( a5_old_i ) / sizeof( a5_old_i[0] );
    va_size_t const  a5_old( a5_old_i, a5_old_s );
    big_whole        a5( a5_old );
    st_size_t        a5_new( a5_old_i, a5_old_i + a5_old_s );

    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    a5.set( 3, 12 );
    insert_value_range( a5_new, 3, 12 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );

    a5.set( wlimits_type::digits - 1, 2 * wlimits_type::digits + 1 );
    insert_value_range( a5_new, wlimits_type::digits - 1, 2 * wlimits_type::digits + 1 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );

    a5.set( 2 * wlimits_type::digits + 1, 3 * wlimits_type::digits );
    insert_value_range( a5_new, 2 * wlimits_type::digits + 1, 3 * wlimits_type::digits );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );

    a5.set( wlimits_type::digits + 1, 3 * wlimits_type::digits );
    insert_value_range( a5_new, wlimits_type::digits + 1, 3 * wlimits_type::digits );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );
}

// Unit test for flipping single bits
void
bigwhole_single_bit_flip_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    a1.flip( 3 );
    BOOST_CHECK_EQUAL( 8u, a1.to_uintmax() );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.flip( 2 );
    BOOST_CHECK_EQUAL( 12u, a2.to_uintmax() );

    a2.flip( 3 );
    BOOST_CHECK_EQUAL( 4u, a2.to_uintmax() );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.flip( 0 );
    BOOST_CHECK_EQUAL( 24u, a3.to_uintmax() );

    a3.flip( 1 );
    BOOST_CHECK_EQUAL( 26u, a3.to_uintmax() );

    a3.flip( 4 );
    BOOST_CHECK_EQUAL( 10u, a3.to_uintmax() );

    // two words
    size_t const     a4_old_i[] = { 0, wlimits_type::digits + 1 };
    size_t const     a4_old_s = sizeof( a4_old_i ) / sizeof( a4_old_i[0] );
    va_size_t const  a4_old( a4_old_i, a4_old_s );
    big_whole        a4( a4_old );

    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    size_t const     a4_new_i[] = { 0, 5, wlimits_type::digits + 1 };
    size_t const     a4_new_s = sizeof( a4_new_i ) / sizeof( a4_new_i[0] );
    va_size_t const  a4_new1( a4_new_i, a4_new_s );
    va_size_t const  a4_new2( a4_new_i + 1, a4_new_s - 1 );

    a4.flip( 5 );
    BOOST_CHECK( equal_valarrays(a4_new1, a4.to_bit_indices()) );

    a4.flip( 0 );
    BOOST_CHECK( equal_valarrays(a4_new2, a4.to_bit_indices()) );

    // more-than-two words
    size_t const     a5_old_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_old_s = sizeof( a5_old_i ) / sizeof( a5_old_i[0] );
    va_size_t const  a5_old( a5_old_i, a5_old_s );
    big_whole        a5( a5_old );

    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    size_t const     a5_new_i[] = { 1, 4, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_new_s = sizeof( a5_new_i ) / sizeof( a5_new_i[0] );
    va_size_t const  a5_new1( a5_new_i, a5_new_s );
    va_size_t const  a5_new2( a5_new_i, a5_new_s - 1 );

    a5.flip( 4 );
    BOOST_CHECK( equal_valarrays(a5_new1, a5.to_bit_indices()) );

    a5.flip( 2 * wlimits_type::digits + 5 );
    BOOST_CHECK( equal_valarrays(a5_new2, a5.to_bit_indices()) );
}

// Unit test for flipping a group of bits
void
bigwhole_group_bit_flip_unit_test
(
)
{
    using boost::math::big_whole;
    using std::size_t;

    typedef std::valarray<size_t>  va_size_t;
    typedef std::set<size_t>       st_size_t;

    // zero
    big_whole  a1;

    BOOST_CHECK_EQUAL( 0u, a1.to_uintmax() );

    a1.flip( 3, 7 );
    BOOST_CHECK_EQUAL( 248u, a1.to_uintmax() );

    a1.flip( 2, 4 );
    BOOST_CHECK_EQUAL( 228u, a1.to_uintmax() );

    // one bit set
    big_whole  a2( 8 );

    BOOST_CHECK_EQUAL( 8u, a2.to_uintmax() );

    a2.flip( 6, 9 );
    BOOST_CHECK_EQUAL( 968u, a2.to_uintmax() );

    a2.flip( 2, 3 );
    BOOST_CHECK_EQUAL( 964u, a2.to_uintmax() );

    // multiple bits set
    big_whole  a3( 25 );

    BOOST_CHECK_EQUAL( 25u, a3.to_uintmax() );

    a3.flip( 3, 3 );
    BOOST_CHECK_EQUAL( 17u, a3.to_uintmax() );

    a3.flip( 1, 2 );
    BOOST_CHECK_EQUAL( 23u, a3.to_uintmax() );

    a3.flip( 2, 6 );
    BOOST_CHECK_EQUAL( 107u, a3.to_uintmax() );

    // two words
    size_t const     a4_old_i[] = { 0, wlimits_type::digits + 1 };
    size_t const     a4_old_s = sizeof( a4_old_i ) / sizeof( a4_old_i[0] );
    va_size_t const  a4_old( a4_old_i, a4_old_s );
    big_whole        a4( a4_old );
    st_size_t        a4_new( a4_old_i, a4_old_i + a4_old_s );

    BOOST_CHECK( equal_valarrays(a4_old, a4.to_bit_indices()) );

    a4.flip( 5, 12 );
    insert_value_range( a4_new, 5, 12 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a4_new ), a4.to_bit_indices()) );

    a4.flip( 9, 2 * wlimits_type::digits );
    insert_value_range( a4_new, 9, 2 * wlimits_type::digits );
    erase_value_range( a4_new, 9, 12 );
    a4_new.erase( wlimits_type::digits + 1 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a4_new ), a4.to_bit_indices()) );

    // more-than-two words
    size_t const     a5_old_i[] = { 1, wlimits_type::digits + 3, wlimits_type::digits + 4, 2 * wlimits_type::digits + 5 };
    size_t const     a5_old_s = sizeof( a5_old_i ) / sizeof( a5_old_i[0] );
    va_size_t const  a5_old( a5_old_i, a5_old_s );
    big_whole        a5( a5_old );
    st_size_t        a5_new( a5_old_i, a5_old_i + a5_old_s );

    BOOST_CHECK( equal_valarrays(a5_old, a5.to_bit_indices()) );

    a5.flip( 3, 12 );
    insert_value_range( a5_new, 3, 12 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );

    a5.flip( wlimits_type::digits - 1, 2 * wlimits_type::digits + 1 );
    insert_value_range( a5_new, wlimits_type::digits - 1, 2 * wlimits_type::digits + 1 );
    a5_new.erase( wlimits_type::digits + 3 );
    a5_new.erase( wlimits_type::digits + 4 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );

    a5.flip( 2 * wlimits_type::digits + 1, 3 * wlimits_type::digits );
    insert_value_range( a5_new, 2 * wlimits_type::digits + 1, 3 * wlimits_type::digits );
    a5_new.erase( 2 * wlimits_type::digits + 1 );
    a5_new.erase( 2 * wlimits_type::digits + 5 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );

    a5.flip( wlimits_type::digits + 1, 3 * wlimits_type::digits );
    erase_value_range( a5_new, wlimits_type::digits + 1, 3 * wlimits_type::digits );
    a5_new.insert( wlimits_type::digits + 3 );
    a5_new.insert( wlimits_type::digits + 4 );
    a5_new.insert( 2 * wlimits_type::digits + 1 );
    a5_new.insert( 2 * wlimits_type::digits + 5 );
    BOOST_CHECK( equal_valarrays(set_to_valarray( a5_new ), a5.to_bit_indices()) );
}

// Unit test for assigning a group of bits to an arbitrary value
void
bigwhole_group_bit_assign_unit_test
(
)
{
    using boost::math::big_whole;

    // no segment is zero
    big_whole  x( 255 );

    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    x.bits_assign( 3, 5, big_whole(2) );
    BOOST_CHECK_EQUAL( 215u, x.to_uintmax() );

    // old middle part is zero
    x.assign( 199 );
    BOOST_CHECK_EQUAL( 199u, x.to_uintmax() );

    x.bits_assign( 3, 5, big_whole(7) );
    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    // new-value bits above the range are ignored
    x.assign( 199 );
    BOOST_CHECK_EQUAL( 199u, x.to_uintmax() );

    x.bits_assign( 3, 4, big_whole(14) );
    BOOST_CHECK_EQUAL( 215u, x.to_uintmax() );

    // new middle part is zero
    x.assign( 255 );
    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    x.bits_assign( 3, 5, big_whole() );
    BOOST_CHECK_EQUAL( 199u, x.to_uintmax() );

    // change the lowest bits
    x.assign( 255 );
    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    x.bits_assign( 0, 2, big_whole(33) );
    BOOST_CHECK_EQUAL( 249u, x.to_uintmax() );

    // change the lowest bits to zero
    x.assign( 255 );
    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    x.bits_assign( 0, 3, big_whole() );
    BOOST_CHECK_EQUAL( 240u, x.to_uintmax() );

    // change the highest bits
    x.assign( 255 );
    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    x.bits_assign( 5, 7, big_whole(5) );
    BOOST_CHECK_EQUAL( 191u, x.to_uintmax() );

    // change the highest bits to zero
    x.assign( 255 );
    BOOST_CHECK_EQUAL( 255u, x.to_uintmax() );

    x.bits_assign( 4, 7, big_whole() );
    BOOST_CHECK_EQUAL( 15u, x.to_uintmax() );

    // turn zero into a nonzero
    x.reset();
    BOOST_CHECK_EQUAL( 0u, x.to_uintmax() );

    x.bits_assign( 8, 9, big_whole(35) );
    BOOST_CHECK_EQUAL( 768u, x.to_uintmax() );

    // keep zero a zero
    x.reset();
    BOOST_CHECK_EQUAL( 0u, x.to_uintmax() );

    x.bits_assign( 11, 45, big_whole() );
    BOOST_CHECK_EQUAL( 0u, x.to_uintmax() );
}


// Unit test program
boost::unit_test_framework::test_suite *
init_unit_test_suite
(
    int         ,   // "argc" is unused
    char *      []  // "argv" is unused
)
{
    boost::unit_test_framework::test_suite *  test
     = BOOST_TEST_SUITE( "big_whole test" );

    test->add( BOOST_TEST_CASE(basic_bigwhole_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_multi_bit_check_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_reverse_unit_test) );

    test->add( BOOST_TEST_CASE(bigwhole_all_bit_reset_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_single_bit_reset_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_group_bit_reset_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_single_bit_set_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_group_bit_set_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_single_bit_flip_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_group_bit_flip_unit_test) );
    test->add( BOOST_TEST_CASE(bigwhole_group_bit_assign_unit_test) );

    return test;
}
