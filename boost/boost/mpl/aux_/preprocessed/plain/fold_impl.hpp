namespace boost { namespace mpl { namespace aux {
// forward declaration

template<
      int N
    , typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl;

template<
      typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< 0,First,Last,State,ForwardOp >
{
    typedef First iter0;
    typedef State state0;
    typedef state0 state;
    typedef iter0 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< 1,First,Last,State,ForwardOp >
{
    typedef First iter0;
    typedef State state0;
    typedef typename apply2< ForwardOp, state0, typename iter0::type >::type state1;
    typedef typename next<iter0>::type iter1;
    
    typedef state1 state;
    typedef iter1 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< 2,First,Last,State,ForwardOp >
{
    typedef First iter0;
    typedef State state0;
    typedef typename apply2< ForwardOp, state0, typename iter0::type >::type state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, state1, typename iter1::type >::type state2;
    typedef typename next<iter1>::type iter2;
    
    typedef state2 state;
    typedef iter2 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< 3,First,Last,State,ForwardOp >
{
    typedef First iter0;
    typedef State state0;
    typedef typename apply2< ForwardOp, state0, typename iter0::type >::type state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, state1, typename iter1::type >::type state2;
    typedef typename next<iter1>::type iter2;
    typedef typename apply2< ForwardOp, state2, typename iter2::type >::type state3;
    typedef typename next<iter2>::type iter3;
    
    typedef state3 state;
    typedef iter3 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< 4,First,Last,State,ForwardOp >
{
    typedef First iter0;
    typedef State state0;
    typedef typename apply2< ForwardOp, state0, typename iter0::type >::type state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, state1, typename iter1::type >::type state2;
    typedef typename next<iter1>::type iter2;
    typedef typename apply2< ForwardOp, state2, typename iter2::type >::type state3;
    typedef typename next<iter2>::type iter3;
    typedef typename apply2< ForwardOp, state3, typename iter3::type >::type state4;
    typedef typename next<iter3>::type iter4;
    
    typedef state4 state;
    typedef iter4 iterator;
};

template<
      int N
    , typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl
{
    typedef fold_impl<
          4
        , First
        , Last
        , State
        , ForwardOp
        > chunk_;
    typedef fold_impl<
          ( (N - 4) < 0 ? 0 : N - 4 )
        , typename chunk_::iterator
        , Last
        , typename chunk_::state
        , ForwardOp
        > res_;
    typedef typename res_::state state;
    typedef typename res_::iterator iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< -1,First,Last,State,ForwardOp >
    : fold_impl<
          -1
        , typename next<First>::type
        , Last
        , typename apply2<ForwardOp, State, typename First::type>::type
        , ForwardOp
        >
{
};

template<
      typename Last
    , typename State
    , typename ForwardOp
    >
struct fold_impl< -1,Last,Last,State,ForwardOp >
{
    typedef State state;
    typedef Last iterator;
};

}}}
