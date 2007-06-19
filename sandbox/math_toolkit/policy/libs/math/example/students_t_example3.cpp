// students_t_example3.cpp
// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Example 3 of using Student's t.

// A general guide to Student's t is at
// http://en.wikipedia.org/wiki/Student's_t-test
// (and many other elementary and advanced statistics texts).
// It says:
// The t statistic was invented by William Sealy Gosset
// for cheaply monitoring the quality of beer brews.
// "Student" was his pen name.
// Gosset was statistician for Guinness brewery in Dublin, Ireland,
// hired due to Claude Guinness's innovative policy of recruiting the
// best graduates from Oxford and Cambridge for applying biochemistry
// and statistics to Guinness's industrial processes.
// Gosset published the t test in Biometrika in 1908,
// but was forced to use a pen name by his employer who regarded the fact
// that they were using statistics as a trade secret.
// In fact, Gosset's identity was unknown not only to fellow statisticians
// but to his employer - the company insisted on the pseudonym
// so that it could turn a blind eye to the breach of its rules.

#ifdef _MSC_VER
#  pragma warning(disable: 4702) // unreachable code
#endif
// 4996 4512 

// The Students't distribution function is described at
// http://en.wikipedia.org/wiki/Student%27s_t_distribution

#include <boost/math/distributions/students_t.hpp>
	using boost::math::students_t;  // Probability of students_t(df, t).

#include <iostream>
	using std::cout;
	using std::endl;
#include <iomanip>
	using std::setprecision;
	using std::setw;
#include <cmath>
	using std::sqrt;

// This example of a two-sided test is from:
// B. M. Smith & M. B. Griffiths, Analyst, 1982, 107, 253,
// from Statistics for Analytical Chemistry, 3rd ed. (1994), pp 58-59
// J. C. Miller and J. N. Miller, Ellis Horwood ISBN 0 13 0309907

// Concentrations of lead (ug/l) determined by two different methods
// for each of four test portions,
// the concentration of each portion is significantly different,
// the values may NOT be pooled.

// Portion  Wet oxidation Direct Extraction
//   1           71            76
//   2           61            68
//   3           50            48
//   4           60            57

const int portions = 4;
const int methods = 2;
float data [portions][methods] = {{71, 76}, {61,68}, {50, 48}, {60, 57}};
float diffs[portions];

int main()
{
	cout << "Example3 using Student's t function. ";
#if defined(__FILE__) && defined(__TIMESTAMP__)
	cout << "  " << __FILE__ << ' ' << __TIMESTAMP__ << ' '<< _MSC_FULL_VER;
#endif
	cout << endl;
	float mean_diff = 0.f;
	cout << "\n""Portion  wet_oxidation  Direct_extraction  difference" << endl;
	for (int portion = 0; portion < portions; portion++)
	{ // Echo data and differences.
		diffs[portion] = data[portion][0] - data[portion][1];
		mean_diff += diffs[portion];
		cout << setw(4) << portion << ' ' << setw(14) << data[portion][0] << ' ' << setw(18)<< data[portion][1] << ' ' << setw(9) << diffs[portion] << endl;
	}
	mean_diff /= portions;
	cout << "Mean difference = " << mean_diff << endl; // -1.75

	float sd_diffs = 0.f;
	for (int portion = 0; portion < portions; portion++)
	{ // Calculate standard deviation of differences.
		sd_diffs +=(diffs[portion] - mean_diff) * (diffs[portion] - mean_diff);
	}
	int degrees_of_freedom = portions-1; // Use the n-1 formula.
	sd_diffs /= degrees_of_freedom;
   sd_diffs = sqrt(sd_diffs);
	cout << "Standard deviation of differences = " << sd_diffs << endl; // 4.99166

	double t = mean_diff * sqrt(static_cast<double>(portions))/ sd_diffs; // -0.70117
	cout << "Student's t = " << t << ", if " << degrees_of_freedom << " degrees of freedom." << endl; //

	cout << "Probability of the means being different is " << 2.F * cdf(students_t(degrees_of_freedom), t) << "."<< endl; // 0.266846 * 2 =
	// Double the probability because using a 'two-sided test' because
	// mean for 'Wet Oxidation' could be either greater OR LESS than for 'Direct extraction'.

	return 0;
}  // int main()

/*

Check using http://faculty.vassar.edu/lowry/VassarStats.html
Output from avoid data using
Independent samples is
n = 4,
sum 242 and 249 total 491
sumsq 14862 15953  30815
SS 221  452.75  679.875
means 60.5  62.25  61.375

Mean diff = -1.75, t = -0.23, df 6,
independent samples: p 1-tailed 0.4128665, 2-tailed 0.825733

correlated samples: p 1-tailed 0.261635, 2-tailed 0.534327
(Called a paired test in Miller and Miller because each portion analysed has a different concentration.)

Output is:

------ Build started: Project: students_t_example3, Configuration: Debug Win32 ------
Compiling...
students_t_example3.cpp
Linking...
Atuorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\students_t_example3.exe"
Example3 using Student's t function.   ..\..\..\..\..\..\boost-sandbox\libs\math_functions\example\students_t_example3.cpp Sat Aug 12 16:57:50 2006 140050727
Portion  wet_oxidation  Direct_extraction  difference
   0             71                 76        -5
   1             61                 68        -7
   2             50                 48         2
   3             60                 57         3
Mean difference = -1.75
Standard deviation of differences = 4.99166
Student's t = -0.70117, if 3 degrees of freedom.
Probability of the means being different is 0.533692.
Build Time 0:03
Build log was saved at "file://i:\boost-06-05-03-1300\libs\math\test\Math_test\students_t_example3\Debug\BuildLog.htm"
students_t_example3 - 0 error(s), 0 warning(s)
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

------ Build started: Project: students_t_example3, Configuration: Release Win32 ------
Compiling...
students_t_example3.cpp
Linking...
Generating code
Finished generating code
Autorun "i:\boost-06-05-03-1300\libs\math\test\math_test\release\students_t_example3.exe"
Example3 using Student's t function.   ..\..\..\..\..\..\boost-sandbox\libs\math_functions\example\students_t_example3.cpp Sat Aug 12 17:01:52 2006 140050727
Portion  wet_oxidation  Direct_extraction  difference
   0             71                 76        -5
   1             61                 68        -7
   2             50                 48         2
   3             60                 57         3
Mean difference = -1.75
Standard deviation of differences = 4.99166
Student's t = -0.70117, if 3 degrees of freedom.
Probability of the means being different is 0.533692.
Build Time 0:04
Build log was saved at "file://i:\boost-06-05-03-1300\libs\math\test\Math_test\students_t_example3\Release\BuildLog.htm"
students_t_example3 - 0 error(s), 0 warning(s)
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========


*/




