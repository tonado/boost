namespace boost { namespace mpl {

namespace aux {
template< int N >
struct list_chooser;

}

namespace aux {
template<>
struct list_chooser<0>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list0<
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<1>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list1<
              T0
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<2>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list2<
              T0, T1
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<3>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list3<
              T0, T1, T2
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<4>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list4<
              T0, T1, T2, T3
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<5>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list5<
              T0, T1, T2, T3, T4
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<6>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list6<
              T0, T1, T2, T3, T4, T5
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<7>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list7<
              T0, T1, T2, T3, T4, T5, T6
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<8>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list8<
              T0, T1, T2, T3, T4, T5, T6, T7
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<9>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list9<
              T0, T1, T2, T3, T4, T5, T6, T7, T8
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<10>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list10<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<11>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list11<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<12>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list12<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<13>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list13<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<14>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list14<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<15>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list15<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<16>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list16<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<17>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list17<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<18>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list18<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<19>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list19<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18
            > type;
    };
};

} // namespace aux

namespace aux {
template<>
struct list_chooser<20>
{
    template<
          typename T0, typename T1, typename T2, typename T3, typename T4
        , typename T5, typename T6, typename T7, typename T8, typename T9
        , typename T10, typename T11, typename T12, typename T13, typename T14
        , typename T15, typename T16, typename T17, typename T18, typename T19
        >
    struct result_
    {
        typedef list20<
              T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19
            > type;
    };
};

} // namespace aux

namespace aux {
template< typename T >
struct is_list_arg
{
    static bool const value = true;
};

template<>
struct is_list_arg<na>
{
    static bool const value = false;
};

template<
      typename T1, typename T2, typename T3, typename T4, typename T5
    , typename T6, typename T7, typename T8, typename T9, typename T10
    , typename T11, typename T12, typename T13, typename T14, typename T15
    , typename T16, typename T17, typename T18, typename T19, typename T20
    >
struct list_count_args
{
    static int const value =
          is_list_arg<T1>::value + is_list_arg<T2>::value 
        + is_list_arg<T3>::value + is_list_arg<T4>::value 
        + is_list_arg<T5>::value + is_list_arg<T6>::value 
        + is_list_arg<T7>::value + is_list_arg<T8>::value 
        + is_list_arg<T9>::value + is_list_arg<T10>::value 
        + is_list_arg<T11>::value + is_list_arg<T12>::value 
        + is_list_arg<T13>::value + is_list_arg<T14>::value 
        + is_list_arg<T15>::value + is_list_arg<T16>::value 
        + is_list_arg<T17>::value + is_list_arg<T18>::value 
        + is_list_arg<T19>::value + is_list_arg<T20>::value
        ;
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    , typename T15, typename T16, typename T17, typename T18, typename T19
    >
struct list_impl
{
    typedef aux::list_count_args<
          T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19
        > arg_num_;
    typedef typename aux::list_chooser< arg_num_::value >
        ::template result_< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19 >::type type;
};

} // namespace aux

template<
      typename T0 = na, typename T1 = na, typename T2 = na, typename T3 = na
    , typename T4 = na, typename T5 = na, typename T6 = na, typename T7 = na
    , typename T8 = na, typename T9 = na, typename T10 = na, typename T11 = na
    , typename T12 = na, typename T13 = na, typename T14 = na
    , typename T15 = na, typename T16 = na, typename T17 = na
    , typename T18 = na, typename T19 = na
    >
struct list
    : aux::list_impl<
          T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19
        >::type
{
    typedef typename aux::list_impl<
          T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19
        >::type type;
};

}}
