
// tupple_test.cpp

// Follows closely tuple_test_bench.cpp (as of boost 1.25.0).
// Few tests were added to demonstrate operators and dereferencing. (RR)

#include <boost/test/included/test_exec_monitor.hpp>

//#define TUPPLE_SKIP_NOT_EQUAL

#include <boost/tupple/tupple.hpp>
#include <boost/tupple/container_tupple.hpp>

using namespace boost::tupple;

#include <vector>
using std::vector;

#include <string>

#include <algorithm>
#include <numeric>
#include <functional>

// helpers

class A {};
class B {};
class C {};

// classes with different kinds of conversions
class AA {};
class BB : public AA {};
struct CC { CC() {} CC(const BB&) {} };
struct DD { operator CC() const { return CC(); }; };

// something to prevent warnings for unused variables
template<class T> void dummy(const T&) {}

// no public default constructor
class foo {
public:
  explicit foo(int v) : val(v) {}

  bool operator==(const foo& other) const  {
    return val == other.val;
  }

private:
  foo() {}
  int val;
};

// another class without a public default constructor
class no_def_constructor {
  no_def_constructor() {}
public:
  no_def_constructor(std::string) {}
};

// A non-copyable class
class no_copy {
  no_copy(const no_copy&) {}
public:
  no_copy() {};
};



void construction_test()
{
  tuple<null_type> t0;
  BOOST_TEST( t0.size() == 0 );

  tuple<int> t1;
  // BOOST_TEST( get0(t1) == int() );
  // Note: This test worked for gcc-2.95.3 (cygwin),
  //       but not for gcc-2.95.2

  tuple<float> t2(5.5f);
  BOOST_TEST( get0(t2) > 5.4f && get0(t2) < 5.6f );

  tuple<foo> t3(foo(12),t0);
  BOOST_TEST( get0(t3) == foo(12) );
  BOOST_TEST( t3.head() == foo(12) );
  BOOST_TEST( t3.tail() == t0 );

  tuple<double> t4(t2);
  BOOST_TEST( get0(t4) > 5.4 && get0(t4) < 5.6 );

  tuple<int, float> t5;
  // BOOST_TEST( get0(t5) == int() );
  // BOOST_TEST( get1(t5) == float() );

  tuple<int, float> t6(12, 5.5f);
  BOOST_TEST( get0(t6) == 12);
  BOOST_TEST( get1(t6) > 5.4f && get1(t6) < 5.6f );

  tuple<int, float> t7(t6);
  BOOST_TEST( get0(t7) == 12);
  BOOST_TEST( get1(t7) > 5.4f && get1(t7) < 5.6f );

  tuple<long, double> t8(t6);
  BOOST_TEST( get0(t8) == 12 );
  BOOST_TEST( get1(t8) > 5.4f && get1(t8) < 5.6f );

  tuple<char,long,double> t9('a',t8);
  BOOST_TEST( get0(t9) == 'a' );
  BOOST_TEST( get1(t9) == 12 );
  BOOST_TEST( get2(t9) > 5.4f && get2(t9) < 5.6f );
  BOOST_TEST( t9.head() == 'a' );
  BOOST_TEST( t9.tail().head() == 12 );
  BOOST_TEST( t9.tail().tail().head() > 5.4f && t9.tail().tail().head() < 5.6f );
  BOOST_TEST( t9.tail().tail().tail().size() == 0 );

  dummy(
    tuple<no_def_constructor, no_def_constructor, no_def_constructor>(
       std::string("Jaba"),   // ok, since the default
       std::string("Daba"),   // constructor is not used
       std::string("Doo")
    )
  );

// testing default values
  dummy(tuple<int, double>());
  //dummy(tuple<int, double>(1)); // Contrary to tuple, this isn't supported
  dummy(tuple<int, double>(1,3.14));


  //  dummy(tuple<double&>()); // should fail, not defaults for references
  //  dummy(tuple<const double&>()); // likewise
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  double dd = 5;
  dummy(tuple<double&>(dd)); // ok

  dummy(tuple<const double&>(dd+3.14)); // ok, but dangerous
#endif

  //  dummy(tuple<double&>(dd+3.14)); // should fail,
  //                                  // temporary to non-const reference
}


void element_access_test()
{
//#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  double d = 2.7;
  A a;
  tuple<int, double&, const A&> t(1, d, a);
  const tuple<int, double&, const A> ct = t;

  int i = get0(t);
  int j = get0(ct);
  BOOST_TEST(i == 1 && j == 1);

  get0(t) = 5;
  BOOST_TEST(t.get0() == 5);

  // get0(ct) = 5; // can't assign to const

  double e = get1(t);
  BOOST_TEST(e > 2.69 && e < 2.71);

  get1(t) = 3.14;
  BOOST_TEST( get1(t) > 3.13 && get1(t) < 3.15);

  get1(ct) = 2.78; // You think we can't assign to const?
                   // Well, but this compiles (with gcc-2.95.3)!
  BOOST_TEST( get1(ct) > 2.77 && get1(ct) < 2.79 );

  t.head() = 4;
  BOOST_TEST( get0(t) == 4 );

  ++t.head();
  BOOST_TEST( get0(t) == 5 );
  // get2(t) = A(); // can't assign to const
  //  dummy( get5(ct) ); // illegal index

  ++get0(t);
  BOOST_TEST( get0(t) == 6 );

  dummy(i); dummy(j); dummy(e); // avoid warns for unused variables
//#endif
}


void copy_test()
{
  tuple<int, char> t1(4, 'a');
  tuple<int, char> t2(5, 'b');
  t2 = t1;
  BOOST_TEST( get0(t1) == get0(t2));
  BOOST_TEST( get1(t1) == get1(t2));

  tuple<long, std::string> t3(2, "c");
  t3 = t1;
  BOOST_TEST( get0(t1) == get0(t3) );
  BOOST_TEST( get1(t1) == get1(t3)[0] );

// testing copy and assignment with implicit conversions between elements

  tuple<char, BB*, BB, DD> t;
  tuple<int, AA*, CC, CC> a(t);
  //BOOST_TEST( a == t ); // no operator==(BB,CC)
  BOOST_TEST( get0(a) == get0(t) );
  tuple<int, AA*, CC, CC> b; b = t;
  //BOOST_TEST( b == t );
}


void mutate_test()
{
  tuple<int, float, bool, foo> t1(5, 12.2f, true, foo(4));
  get0(t1) = 6;
  get1(t1) = 2.2f;
  get2(t1) = false;
  get3(t1) = foo(5);

  BOOST_TEST( get0(t1) == 6 );
  BOOST_TEST( get1(t1) > 2.1f && get1(t1) < 2.3f );
  BOOST_TEST( get2(t1) == false );
  BOOST_TEST( get3(t1) == foo(5) );
}


void make_tuple_test()
{
  tuple<int, char> t1 = make_tuple(5, 'a');
  BOOST_TEST( get0(t1) == 5);
  BOOST_TEST( get1(t1) == 'a');

  tuple<int, std::string> t2;
  t2 = make_tuple( (short int)2, std::string("Hi") );
  BOOST_TEST( get0(t2) == 2 );
  BOOST_TEST( get1(t2) == "Hi" );

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    A a; B b;
    const A ca = a;
    make_tuple(cref(a), b);
    make_tuple(ref(a), b);
    make_tuple(ref(a), cref(b));

    make_tuple(ref(ca));
#endif

// the result of make_tuple is assignable:
   BOOST_TEST( make_tuple(2, 4, 6) ==
       (make_tuple(1, 2, 3) = make_tuple(2, 4, 6)));

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    make_tuple("Donald", "Daisy"); // should work;
#endif
    //    std::make_pair("Doesn't","Work"); // fails

// You can store a reference to a function in a tuple
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    tuple<void(&)()> adf(make_tuple_test);

    dummy(adf); // avoid warning for unused variable
#endif

// But make_tuple doesn't work
// with function references, since it creates a const qualified function type

//   make_tuple(make_tuple_test);

// With function pointers, make_tuple works just fine

#if !defined(__BORLANDC__) || __BORLAND__ > 0x0551
   make_tuple(&make_tuple_test);
#endif

// NOTE:
//
// wrapping it the function reference with ref helps on gcc 2.95.2.
// on edg 2.43. it results in a catastrophic error?

// make_tuple(ref(foo3));

// It seems that edg can't use implicitly the ref's conversion operator, e.g.:
// typedef void (&func_t) (void);
// func_t fref = static_cast<func_t>(ref(make_tuple_test)); // works fine
// func_t fref = ref(make_tuple_test);                        // error

// This is probably not a very common situation, so currently
// I don't know how which compiler is right (JJ)
}


void tie_test()
{
  int a;
  char b;
  foo c(5);

  tie( a, b, c ) = make_tuple( 2, 'a', foo(3) );
  BOOST_TEST(a == 2);
  BOOST_TEST(b == 'a');
  BOOST_TEST(c == foo(3));

  tie(a, ignore, c) = make_tuple((short int)5, false, foo(5));
  BOOST_TEST(a == 5);
  BOOST_TEST(b == 'a');
  BOOST_TEST(c == foo(5));

// testing assignment from std::pair
/*   int i, j;
   tie(i, j) = std::make_pair(1, 2);
   BOOST_TEST(i == 1 && j == 2);

   tuple<int, int, float> ta;
#ifdef E11
   ta = std::make_pair(1, 2); // should fail, tuple is of length 3, not 2
#endif

   dummy(ta); */
}


void equality_test()
{
  tuple<int, char> t1(5, 'a');
  tuple<int, char> t2(5, 'a');
  BOOST_TEST(t1 == t2);

  tuple<int, char> t3(5, 'b');
  tuple<int, char> t4(2, 'a');
  BOOST_TEST(t1 != t3);
  BOOST_TEST(t1 != t4);
}


void ordering_test()
{
  tuple<int, float> t1(4, 3.3f);
  tuple<short, float> t2(5, 3.3f);
  tuple<long, double> t3(5, 4.4);

  BOOST_TEST(t1 < t2);
  BOOST_TEST(t1 <= t2);
  BOOST_TEST(t2 > t1);
  BOOST_TEST(t2 >= t1);
  BOOST_TEST(t2 < t3);
  BOOST_TEST(t2 <= t3);
  BOOST_TEST(t3 > t2);
  BOOST_TEST(t3 >= t2);
}


void n_fold_test()
{
  tuple<int,int,int> a(1,2,3);
  n_fold_tuple<int,3>::type b(1,2,3);

  BOOST_TEST( a == b );
}


void swap_test()
{
  tuple<char,int,std::string> a( '+', 3, "foo" ), aa( '+', 3, "foo" );
  tuple<char,int,std::string> b( '-', 7, "yo" ),  bb( '-', 7, "yo" );

  aa.swap( bb );

  BOOST_TEST( a == bb );
  BOOST_TEST( b == aa );

  std::swap( a,b );

  BOOST_TEST( a == aa );
  BOOST_TEST( b == bb );
}


double avg( int weight, int a, int b )
{ return( weight * a + ( 100 - weight ) * b ); }

void function_test()
{
  tuple<int,int,int> a(60,3,5);

  BOOST_TEST( ptr_fct( avg )( a ) == 60 * 3 + 40 * 5 );

  // More to come
}


// Test container_tupple.hpp

void min_max_test()
{
  tuple<int,int,int> t( 5, -2, 3 );

  //BOOST_TEST( minimal(t) == -2 );
  //BOOST_TEST( maximal(t) == 5 );
}


struct convolve
{
  typedef int result_type;

  int operator()( int a, int b, int c ) const
  {
    return( -a + b - c );
  }
};


void iterator_test()
{
  char str[10]   = "Yukon ho!";
  int  prime[10] = {2,3,5,7,11,13,17,19,23,29};

  iterator_tuple<char*,int*> t1( str, prime );
  typedef iterator_tuple<char*,int*>::difference_type diff_type;

  BOOST_TEST( get0(*t1) == 'Y' );
  BOOST_TEST( get1(*t1) == 2 );
  BOOST_TEST( *(get0(t1)) == 'Y' );
  BOOST_TEST( *(get1(t1)) == 2 );

  BOOST_TEST( get0(t1[diff_type(2,3)]) == 'k' );
  BOOST_TEST( get1(t1[diff_type(2,3)]) == 7 );

  ++t1;

  BOOST_TEST( get0(t1) == str+1 );
  BOOST_TEST( get1(t1) == prime+1 );

  BOOST_TEST( get0(*t1) == 'u' );
  BOOST_TEST( get1(*t1) == 3 );

  t1 += diff_type(2,3);

  BOOST_TEST( get0(t1) == str+3 );
  BOOST_TEST( get1(t1) == prime+4 );

  BOOST_TEST( get0(*t1) == 'o' );
  BOOST_TEST( get1(*t1) == 11 );


  t1[diff_type(0,0)] = tuple<char,int>('a',31);

  BOOST_TEST( str[3] == 'a' );
  BOOST_TEST( prime[4] == 31 );

  typedef n_fold_iterator<int*,3>::type triple_iter;
  typedef n_fold_tuple<int,3>::type triple_int;

//  triple_int  t  = add( 1, triple_int(0,2,3) );
  triple_int t(0,2,3); ++get0(t); ++get1(t); ++get2(t);
  triple_iter it( prime,prime,prime ); it += t;

  BOOST_TEST( get0(it) == prime + 1 );
  BOOST_TEST( get1(it) == prime + 3 );
  BOOST_TEST( get2(it) == prime + 4 );
}


void container_test()
{
  std::vector<int>    a;
  std::vector<char>   b(3,'-');
  std::vector<double> c(7,3.14);

  container_tuple< std::vector<int>, std::vector<char>, std::vector<double> > t( a,b,c );
  typedef container_tuple< std::vector<int>, std::vector<char>, std::vector<double> >::size_type index_type;

  BOOST_TEST( get0( t.empty() ) == a.empty() );
  BOOST_TEST( get1( t.empty() ) == b.empty() );
  BOOST_TEST( get2( t.empty() ) == c.empty() );

  BOOST_TEST( get0( t.size() ) == a.size() );
  BOOST_TEST( get1( t.size() ) == b.size() );
  BOOST_TEST( get2( t.size() ) == c.size() );

  // Does NOT result in a runtime error, although vector a is EMPTY!
  index_type one(1,1,1);
  BOOST_TEST( get1( t[one] ) == '-' );
  BOOST_TEST( get2( t[one] ) == 3.14 );

  std::vector< std::string > d( 5, "foo" );

  container_tuple< std::vector<char>, std::vector<double>, std::vector< std::string > > u( b,c,d );

  BOOST_TEST( get0( u[one] ) == '-' );
  BOOST_TEST( get1( u[one] ) == 3.14 );
  BOOST_TEST( get2( u[one] ) == "foo" );

  get0( u[one] ) = '+';
  get1( u[one] ) = 2.78;
  get2( u[one] ) = "yo";

  BOOST_TEST( get0( u[one] ) == '+' );
  BOOST_TEST( get1( u[one] ) == 2.78 );
  BOOST_TEST( get2( u[one] ) == "yo" );

  iterator_tuple< std::vector<char>::iterator,
                  std::vector<double>::iterator,
                  std::vector< std::string >::iterator > it = u.begin();

  BOOST_TEST( get0( *it ) == '-' );
  BOOST_TEST( get1( *it ) == 3.14 );
  BOOST_TEST( get2( *it ) == "foo" );

  ++it;

  BOOST_TEST( get0( *it ) == '+' );
  BOOST_TEST( get1( *it ) == 2.78 );
  BOOST_TEST( get2( *it ) == "yo" );

  *it = tuple< char, double, std::string >( '*', 1.33, "gasp" );

  BOOST_TEST( get0( u[one] ) == '*' );
  BOOST_TEST( get1( u[one] ) == 1.33 );
  BOOST_TEST( get2( u[one] ) == "gasp" );

}


#if 1

template<class TupleT, class OpT, class CondT>
TupleT do_if_first( TupleT t, OpT op, CondT cond )
{
  if( cond( t.head() ) )
    { return TupleT( op( t.head() ), t.tail() ); }
  else
    { return TupleT( t.head(), do_if_first( t.tail(), op, cond ) ); }
}


template<class OpT, class CondT>
tuple<null_type> do_if_first( tuple<null_type> t, OpT op, CondT cond )
{
  return tuple<null_type>();
}


void algo_test()
{
  tuple<int, char, double> t1(5, 'a', -3.14);
  
  tuple<int, char, double > t2 = 
    do_if_first( t1, bind2nd( std::multiplies<double>(), -1.5 ), bind2nd( std::less<double>(), 0) );
  
  
  BOOST_TEST( t2.get0() == 5 );
  BOOST_TEST( t2.get1() == 'a' );
  BOOST_TEST( t2.get2() == 3.14 * 1.5 );
}



#endif



#if 0
void zip_test()
{
  char strA[] = "Just a few words on views.";
  char strB[] = "Just a views word on few. ";

  vector<char> a( strA, strA + 27 );
  vector<char> b( strB, strB + 27 );

  typedef zip_view< vector<char>, vector<char>, std::equal_to<char> > compare_view;
  //typedef zip_view< vector<char>&, vector<char>&, std::equal_to<char> > compare_view;

  compare_view v( a, b );

  BOOST_TEST( v[0] == std::equal_to<char>()( a[0], b[0] ) );
  BOOST_TEST( v[1] == std::equal_to<char>()( a[1], b[1] ) );
  BOOST_TEST( v[7] == std::equal_to<char>()( a[7], b[7] ) );
  // etc.


  int m = 0;

  for( int i = 0; i < a.size(); ++i ) // How to do it in an STL-like way?
    if( a[i] != b[i] ) ++m;

  int n = std::count( v.begin(), v.end(), false );

  BOOST_TEST( m == n );
}
#endif


int test_main(int, char *[])
{
  construction_test();
  element_access_test();
  copy_test();
  mutate_test();
  make_tuple_test();
  tie_test();
  equality_test();
  ordering_test();
  n_fold_test();
  swap_test();
  function_test();

  //min_max_test();
  iterator_test();
  container_test();
  
  algo_test();

  return 0;
}

