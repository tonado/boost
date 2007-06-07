/* Boost arithmetics.cpp test file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/bigint/bigint.hpp>

#include <sstream>

#include <iostream>

#pragma comment(lib, "libgmp-3.lib")

// This macro is not quite good, but - it's ok for our needs
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

template <typename I> void test()
{
	typedef boost::bigint_base<I> number;

	struct test_entry
	{
		const char* lhs;
		char op;
		const char* rhs;
		const char* result;
	};

	test_entry entries[] =
	{
		// addition

		// same sign, no carries
		{"3", '+', "1", "4"},
		{"1", '+', "3", "4"},
		// same sign, carries
		{"18446744073709551615", '+', "1", "18446744073709551616"},
		{"1", '+', "18446744073709551615", "18446744073709551616"},
		{"18446744073709551615", '+', "18446744073709551615", "36893488147419103230"},
		// different sign, positive result
		{"138493849343434", '+', "-128932839", "138493720410595"},
		{"-128932839", '+', "138493849343434", "138493720410595"},
		// different sign, negative result
		{"-2903920392323129", '+', "5293092323", "-2903915099230806"},
		{"5293092323", '+', "-2903920392323129", "-2903915099230806"},
		// negative numbers
		{"-18446744073709551615", '+', "-4", "-18446744073709551619"},
		{"-4", '+', "-18446744073709551615", "-18446744073709551619"},

		// subtraction

		// same sign, no borrows
		{"3", '-', "1", "2"},
		{"-39043434", '-', "-599509", "-38443925"},
		// same sign, borrows
		{"3", '-', "4", "-1"},
		{"-39043434304928394", '-', "-523243892384923499509", "523204848950618571115"},
		// different sign, positive result
		{"2390492304934", '-', "-2342930493043943434", "2342932883536248368"},
		{"123940", '-', "-1003", "124943"},
		// different sign, negative result
		{"-239402390434", '-', "2349034", "-239404739468"},
		{"239049304", '-', "39049230492304", "-39048991443000"},

		// multiplication

		// zero
		{"234902394023940234", '*', "0", "0"},
		{"0", '*', "234902394023940234", "0"},
		// positive * positive
		{"2389428394283434234234", '*', "895489472863784783", "2139707973242632227811083664960586861222"},
		{"895489472863784783", '*', "2389428394283434234234", "2139707973242632227811083664960586861222"},
		// different magnitudes
		{"2384", '*', "2389428349283942034234", "5696397184692917809613856"},
		{"2389428349283942034234", '*', "2384", "5696397184692917809613856"},
		// 10^n
		{"1000000000000000", '*', "100000000", "100000000000000000000000"},
		{"10000", '*', "10000000000000000000", "100000000000000000000000"},
		// different signs
		{"123940932409302930429304", '*', "-23940239409234", "-2967175594482401511466585794961793136"},
		{"-4895849540949", '*', "5906390354334334989", "-28916798504933355408364838964561"},
		// negative * negative
		{"-23489238492334893", '*', "-2930482394829348293489234", "68834799869735267747353413198446618041962"},
		{"-39403940", '*', "-90689586573473848347384834", "3573527027965969111849451155845960"},

		// division

		// zero
		{"0", '/', "23904293402394023940", "0"},
		// positive / larger positive
		{"239409034", '/', "23489023049230492304230493049034", "0"},
		{"-239409034", '/', "23489023049230492304230493049034", "0"},
		{"239409034", '/', "-23489023049230492304230493049034", "0"},
		// ~ equal magnitudes
		{"39049204923049203490234", '/', "39049204923049203490230", "1"},
		{"-39049204923049203490234", '/', "39049204923049203490230", "-1"},
		{"39049204923049203490234", '/', "-39049204923049203490230", "-1"},
		// some other numbers
		{"13940239402394034213904", '/', "49850", "279643719205497175"},
		{"-9049594859482398349", '/', "-3401", "2660862940159482"},
		// proper rounding for positive numbers
		{"3", '/', "2", "1"},
		{"9304", '/', "3", "3101"},
		{"394093", '/', "11", "35826"},
		// proper rounding for positive results (negative numbers)
		{"-3", '/', "-2", "1"},
		{"-9304", '/', "-3", "3101"},
		{"-394093", '/', "-11", "35826"},
		// proper rounding for positive / negative (towards zero)
		{"-3", '/', "2", "-1"},
		{"-9304", '/', "3", "-3101"},
		{"-394093", '/', "11", "-35826"},
		// proper rounding for negative / positive (towards zero)
		{"3", '/', "-2", "-1"},
		{"9304", '/', "-3", "-3101"},
		{"394093", '/', "-11", "-35826"},

		// modulo

		// zero
		{"0", '%', "134", "0"},
		// sign of the left hand value has to be equal to the sign of the remainder
		{"34", '%', "5", "4"},
		{"34", '%', "-5", "4"},
		{"-34", '%', "5", "-4"},
		{"-34", '%', "-5", "-4"},
		// some other numbers
		{"2394023940394034", '%', "100", "34"},
		{"2394023940394034", '%', "9", "5"},
		{"2394023940394034", '%', "8", "2"},
		{"2394023940394034", '%', "234890234034", "22675119506"},
	};

	for (size_t i = 0; i < ARRAY_SIZE(entries); ++i)
	{
		const test_entry& e = entries[i];

		number a(e.lhs), b(e.rhs), c(e.result);
		
#define GEN_OP(op, chop)	case chop: \
							{ \
								BOOST_CHECK_EQUAL(a op b, c); \
								\
								number d = a; \
								d op##= b; \
								\
								BOOST_CHECK_EQUAL(d, c); \
								\
								if (a.can_convert_to<boost::int64_t>()) \
								{ \
									BOOST_CHECK_EQUAL(a.to_number<boost::int64_t>() op b, c); \
								} \
								\
								if (b.can_convert_to<boost::int64_t>()) \
								{ \
									BOOST_CHECK_EQUAL(a op b.to_number<boost::int64_t>(), c); \
								} \
							} break

		switch (e.op)
		{
		GEN_OP(+, '+');
		GEN_OP(-, '-');
		GEN_OP(*, '*');
		GEN_OP(/, '/');
		GEN_OP(%, '%');
		}

#undef GEN_OP	
	}
}

int test_main(int argc, char* argv[])
{
  test<boost::detail::bigint_gmp_implementation>();

  return 0;
}
