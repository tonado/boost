namespace boost { namespace mpl {

template<
      typename T, long C0 = LONG_MAX, long C1 = LONG_MAX, long C2 = LONG_MAX
    , long C3 = LONG_MAX, long C4 = LONG_MAX, long C5 = LONG_MAX
    , long C6 = LONG_MAX, long C7 = LONG_MAX, long C8 = LONG_MAX
    , long C9 = LONG_MAX, long C10 = LONG_MAX, long C11 = LONG_MAX
    , long C12 = LONG_MAX, long C13 = LONG_MAX, long C14 = LONG_MAX
    , long C15 = LONG_MAX, long C16 = LONG_MAX, long C17 = LONG_MAX
    , long C18 = LONG_MAX, long C19 = LONG_MAX
    >
struct vector_c;

template<
      typename T
    >
struct vector_c<
          T, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector0_c<T>
{
    typedef typename vector0_c<T>::type type;
};

template<
      typename T, long C0
    >
struct vector_c<
          T, C0, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector1_c< T,C0 >
{
    typedef typename vector1_c< T,C0 >::type type;
};

template<
      typename T, long C0, long C1
    >
struct vector_c<
          T, C0, C1, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector2_c< T,C0,C1 >
{
    typedef typename vector2_c< T,C0,C1 >::type type;
};

template<
      typename T, long C0, long C1, long C2
    >
struct vector_c<
          T, C0, C1, C2, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector3_c< T,C0,C1,C2 >
{
    typedef typename vector3_c< T,C0,C1,C2 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3
    >
struct vector_c<
          T, C0, C1, C2, C3, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector4_c< T,C0,C1,C2,C3 >
{
    typedef typename vector4_c< T,C0,C1,C2,C3 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector5_c< T,C0,C1,C2,C3,C4 >
{
    typedef typename vector5_c< T,C0,C1,C2,C3,C4 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector6_c< T,C0,C1,C2,C3,C4,C5 >
{
    typedef typename vector6_c< T,C0,C1,C2,C3,C4,C5 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector7_c< T,C0,C1,C2,C3,C4,C5,C6 >
{
    typedef typename vector7_c< T,C0,C1,C2,C3,C4,C5,C6 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX
        >
    : vector8_c< T,C0,C1,C2,C3,C4,C5,C6,C7 >
{
    typedef typename vector8_c< T,C0,C1,C2,C3,C4,C5,C6,C7 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX
        >
    : vector9_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8 >
{
    typedef typename vector9_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        , LONG_MAX
        >
    : vector10_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9 >
{
    typedef typename vector10_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, LONG_MAX, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector11_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10 >
{
    typedef typename vector11_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector12_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11 >
{
    typedef typename vector12_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, LONG_MAX
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector13_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12 >
{
    typedef typename vector13_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector14_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13
        >
{
    typedef typename vector14_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13, long C14
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector15_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        >
{
    typedef typename vector15_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13, long C14, long C15
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, LONG_MAX, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector16_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15
        >
{
    typedef typename vector16_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13, long C14, long C15, long C16
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16, LONG_MAX, LONG_MAX, LONG_MAX
        >
    : vector17_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16
        >
{
    typedef typename vector17_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13, long C14, long C15, long C16, long C17
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16, C17, LONG_MAX, LONG_MAX
        >
    : vector18_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16, C17
        >
{
    typedef typename vector18_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17 >::type type;
};

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13, long C14, long C15, long C16, long C17, long C18
    >
struct vector_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16, C17, C18, LONG_MAX
        >
    : vector19_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16, C17, C18
        >
{
    typedef typename vector19_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17,C18 >::type type;
};

// primary template (not a specialization!)

template<
      typename T, long C0, long C1, long C2, long C3, long C4, long C5
    , long C6, long C7, long C8, long C9, long C10, long C11, long C12
    , long C13, long C14, long C15, long C16, long C17, long C18, long C19
    >
struct vector_c
    : vector20_c<
          T, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14
        , C15, C16, C17, C18, C19
        >
{
    typedef typename vector20_c< T,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16,C17,C18,C19 >::type type;
};

}}
