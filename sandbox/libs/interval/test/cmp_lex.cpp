#include <boost/interval/interval.hpp>
#include <boost/interval/checking.hpp>
#include <boost/interval/compare.hpp>
#include <boost/interval/policies.hpp>
#include <boost/test/test_tools.hpp>

struct empty_class {};

typedef boost::interval_lib::policies
          <empty_class, boost::interval_lib::checking_base<int> >
  policies;

typedef boost::interval<int, policies> I;

using namespace boost::interval_lib::compare::lexicographic;

#define BOOST_C_EXN(e) \
  BOOST_CHECK_THROW(e, boost::interval_lib::comparison_error)

// comparisons between [1,2] and [3,4]

static void test_12_34() {
  const I a(1,2), b(3,4);

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(b > a);
  BOOST_CHECK(b >= a);
  BOOST_CHECK(!(b < a));
  BOOST_CHECK(!(b <= a));

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

// comparisons between [1,3] and [2,4]

static void test_13_24() {
  const I a(1,3), b(2,4);

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(!(b < a));
  BOOST_CHECK(!(b <= a));
  BOOST_CHECK(b > a);
  BOOST_CHECK(b >= a);

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

// comparisons between [1,2] and [2,3]

static void test_12_23() {
  const I a(1,2), b(2,3);

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(!(b < a));
  BOOST_CHECK(!(b <= a));
  BOOST_CHECK(b > a);
  BOOST_CHECK(b >= a);

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

// comparisons between [1,2] and 0

static void test_12_0() {
  const I a(1,2);
  const int b = 0;

  BOOST_CHECK(!(a < b));
  BOOST_CHECK(!(a <= b));
  BOOST_CHECK(a > b);
  BOOST_CHECK(a >= b);

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

// comparisons between [1,2] and 1

static void test_12_1() {
  const I a(1,2);
  const int b = 1;

  BOOST_CHECK(!(a < b));
  BOOST_CHECK(!(a <= b));
  BOOST_CHECK(a > b);
  BOOST_CHECK(a >= b);

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

// comparisons between [1,2] and 2

static void test_12_2() {
  const I a(1,2);
  const int b = 2;

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

// comparisons between [1,2] and 3

static void test_12_3() {
  const I a(1,2);
  const int b = 3;

  BOOST_CHECK(a < b);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(!(a > b));
  BOOST_CHECK(!(a >= b));

  BOOST_CHECK(!(a == b));
  BOOST_CHECK(a != b);
}

static void test_12_12() {
  const I a(1,2), b(1,2);

  BOOST_CHECK(a == b);
  BOOST_CHECK(!(a != b));
}

static void test_11_11() {
  const I a(1,1), b(1,1);

  BOOST_CHECK(a == b);
  BOOST_CHECK(!(a != b));
}

static void test_11_1() {
  const I a(1,1);
  const int b = 1;

  BOOST_CHECK(a == b);
  BOOST_CHECK(!(a != b));
}

int test_main(int, char *[]) {
  test_12_34();
  test_13_24();
  test_12_23();
  test_12_0();
  test_12_1();
  test_12_2();
  test_12_3();
  test_12_12();
  test_11_11();
  test_11_1();

  return 0;
}
