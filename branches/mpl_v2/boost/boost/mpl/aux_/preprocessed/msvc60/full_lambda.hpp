// preprocessed version of 'boost/mpl/aux_/full_lambda.hpp' header
// see the original for copyright information

namespace boost {
namespace mpl {

template< typename T >
struct lambda
{
    typedef false_c is_le;
    typedef T type;
};

namespace aux {

template<
      bool C1 = false, bool C2 = false, bool C3 = false, bool C4 = false
    , bool C5 = false
    >
struct lambda_or
    : true_c
{
};

template<>
struct lambda_or< false,false,false,false,false >
    : false_c
{
};

} // namespace aux

template< int N >
struct lambda< arg<N> >
{
    typedef true_c is_le;
    typedef arg<N> type;
};

template<
      typename F
    >
struct lambda<
      bind0<F>
    >
{
    typedef false_c is_le;
    typedef bind0<
          F
        > type;
};

namespace aux {

template<
      bool
    , template< typename P1 > class F
    , typename L1
    >
struct le_result1
{
    typedef F<
          typename L1::type
        > type;
};

template<
      template< typename P1 > class F
    , typename L1
    >
struct le_result1< true,F,L1 >
{
    typedef bind1<
          meta_fun1<F>
        , typename L1::type
        > type;
};

} // namespace aux

template<
      template< typename P1 > class F
    , typename T1
    >

struct lambda< F<T1> >

{
    typedef lambda<T1> l1;
    typedef aux::lambda_or<
          l1::is_le::value
        > is_le;

    typedef typename aux::le_result1<
          is_le::value
        , F
        , l1
        >::type type;
};

template<
      typename F, typename T1
    >
struct lambda<
      bind1< F,T1 >
    >
{
    typedef false_c is_le;
    typedef bind1<
          F
        , T1
        > type;
};

namespace aux {

template<
      bool
    , template< typename P1, typename P2 > class F
    , typename L1, typename L2
    >
struct le_result2
{
    typedef F<
          typename L1::type, typename L2::type
        > type;
};

template<
      template< typename P1, typename P2 > class F
    , typename L1, typename L2
    >
struct le_result2< true,F,L1,L2 >
{
    typedef bind2<
          meta_fun2<F>
        , typename L1::type, typename L2::type
        > type;
};

} // namespace aux

template<
      template< typename P1, typename P2 > class F
    , typename T1, typename T2
    >

struct lambda< F<T1,T2> >

{
    typedef lambda<T1> l1;
    typedef lambda<T2> l2;
        typedef aux::lambda_or<
          l1::is_le::value, l2::is_le::value
        > is_le;

    typedef typename aux::le_result2<
          is_le::value
        , F
        , l1, l2
        >::type type;
};

template<
      typename F, typename T1, typename T2
    >
struct lambda<
      bind2< F,T1,T2 >
    >
{
    typedef false_c is_le;
    typedef bind2<
          F
        , T1, T2
        > type;
};

namespace aux {

template<
      bool
    , template< typename P1, typename P2, typename P3 > class F
    , typename L1, typename L2, typename L3
    >
struct le_result3
{
    typedef F<
          typename L1::type, typename L2::type, typename L3::type
        > type;
};

template<
      template< typename P1, typename P2, typename P3 > class F
    , typename L1, typename L2, typename L3
    >
struct le_result3< true,F,L1,L2,L3 >
{
    typedef bind3<
          meta_fun3<F>
        , typename L1::type, typename L2::type, typename L3::type
        > type;
};

} // namespace aux

template<
      template< typename P1, typename P2, typename P3 > class F
    , typename T1, typename T2, typename T3
    >

struct lambda< F<T1,T2,T3> >

{
    typedef lambda<T1> l1;
    typedef lambda<T2> l2;
    typedef lambda<T3> l3;
        typedef aux::lambda_or<
          l1::is_le::value, l2::is_le::value, l3::is_le::value
        > is_le;

    typedef typename aux::le_result3<
          is_le::value
        , F
        , l1, l2, l3
        >::type type;
};

template<
      typename F, typename T1, typename T2, typename T3
    >
struct lambda<
      bind3< F,T1,T2,T3 >
    >
{
    typedef false_c is_le;
    typedef bind3<
          F
        , T1, T2, T3
        > type;
};

namespace aux {

template<
      bool
    , template< typename P1, typename P2, typename P3, typename P4 > class F
    , typename L1, typename L2, typename L3, typename L4
    >
struct le_result4
{
    typedef F<
          typename L1::type, typename L2::type, typename L3::type
        , typename L4::type
        > type;
};

template<
      template< typename P1, typename P2, typename P3, typename P4 > class F
    , typename L1, typename L2, typename L3, typename L4
    >
struct le_result4< true,F,L1,L2,L3,L4 >
{
    typedef bind4<
          meta_fun4<F>
        , typename L1::type, typename L2::type, typename L3::type
        , typename L4::type
        > type;
};

} // namespace aux

template<
      template< typename P1, typename P2, typename P3, typename P4 > class F
    , typename T1, typename T2, typename T3, typename T4
    >

struct lambda< F<T1,T2,T3,T4> >

{
    typedef lambda<T1> l1;
    typedef lambda<T2> l2;
    typedef lambda<T3> l3;
    typedef lambda<T4> l4;
        typedef aux::lambda_or<
          l1::is_le::value, l2::is_le::value, l3::is_le::value
        , l4::is_le::value
        > is_le;

    typedef typename aux::le_result4<
          is_le::value
        , F
        , l1, l2, l3, l4
        >::type type;
};

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    >
struct lambda<
      bind4< F,T1,T2,T3,T4 >
    >
{
    typedef false_c is_le;
    typedef bind4<
          F
        , T1, T2, T3, T4
        > type;
};

namespace aux {

template<
      bool
    , template< typename P1, typename P2, typename P3, typename P4, typename P5 > class F
    , typename L1, typename L2, typename L3, typename L4, typename L5
    >
struct le_result5
{
    typedef F<
          typename L1::type, typename L2::type, typename L3::type
        , typename L4::type, typename L5::type
        > type;
};

template<
      template<
          typename P1, typename P2, typename P3, typename P4
        , typename P5
        >
      class F
    , typename L1, typename L2, typename L3, typename L4, typename L5
    >
struct le_result5< true,F,L1,L2,L3,L4,L5 >
{
    typedef bind5<
          meta_fun5<F>
        , typename L1::type, typename L2::type, typename L3::type
        , typename L4::type, typename L5::type
        > type;
};

} // namespace aux

template<
      template<
          typename P1, typename P2, typename P3, typename P4
        , typename P5
        >
      class F
    , typename T1, typename T2, typename T3, typename T4, typename T5
    >

struct lambda< F<T1,T2,T3,T4,T5> >

{
    typedef lambda<T1> l1;
    typedef lambda<T2> l2;
    typedef lambda<T3> l3;
    typedef lambda<T4> l4;
    typedef lambda<T5> l5;
        typedef aux::lambda_or<
          l1::is_le::value, l2::is_le::value, l3::is_le::value
        , l4::is_le::value, l5::is_le::value
        > is_le;

    typedef typename aux::le_result5<
          is_le::value
        , F
        , l1, l2, l3, l4, l5
        >::type type;
};

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct lambda<
      bind5< F,T1,T2,T3,T4,T5 >
    >
{
    typedef false_c is_le;
    typedef bind5<
          F
        , T1, T2, T3, T4, T5
        > type;
};

// special case for 'protect'
template< typename T >
struct lambda< protect<T> >
{
    typedef false_c is_le;
    typedef protect<T> type;
};

// specializations for main 'bind', 'bind1st' and 'bind2nd' forms
template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct lambda< bind<F,T1,T2,T3,T4,T5> >
{
    typedef false_c is_le;
    typedef bind< F,T1,T2,T3,T4,T5 > type;
};

template<
      typename F
    , typename T
    >
struct lambda< bind1st<F,T> >
{
    typedef false_c is_le;
    typedef bind1st< F,T > type;
};

template<
      typename F
    , typename T
    >
struct lambda< bind2nd<F,T> >
{
    typedef false_c is_le;
    typedef bind2nd< F,T > type;
};

} // namespace mpl
} // namespace boost

