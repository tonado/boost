namespace boost { namespace mpl {

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10
    >
struct set11
    : s_item<
          T10
        , set10< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11
    >
struct set12
    : s_item<
          T11
        , set11< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12
    >
struct set13
    : s_item<
          T12
        , set12< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13
    >
struct set14
    : s_item<
          T13
        , set13< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    >
struct set15
    : s_item<
          T14
        , set14< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    , typename T15
    >
struct set16
    : s_item<
          T15
        , set15< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    , typename T15, typename T16
    >
struct set17
    : s_item<
          T16
        , set16< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    , typename T15, typename T16, typename T17
    >
struct set18
    : s_item<
          T17
        , set17< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    , typename T15, typename T16, typename T17, typename T18
    >
struct set19
    : s_item<
          T18
        , set18< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17 >
        >
{
};

template<
      typename T0, typename T1, typename T2, typename T3, typename T4
    , typename T5, typename T6, typename T7, typename T8, typename T9
    , typename T10, typename T11, typename T12, typename T13, typename T14
    , typename T15, typename T16, typename T17, typename T18, typename T19
    >
struct set20
    : s_item<
          T19
        , set19< T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18 >
        >
{
};

}}
