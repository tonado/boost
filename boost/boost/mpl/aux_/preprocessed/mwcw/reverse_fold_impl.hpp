namespace boost { namespace mpl { namespace aux {
// forward declaration

template<
      long N
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl;

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< 0,First,Last,State,BackwardOp,ForwardOp >
{
    typedef First iter0;
    typedef State fwd_state0;
    typedef fwd_state0 bkwd_state0;
    typedef bkwd_state0 state;
    typedef iter0 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< 1,First,Last,State,BackwardOp,ForwardOp >
{
    typedef First iter0;
    typedef State fwd_state0;
    typedef typename apply2< ForwardOp, fwd_state0, typename iter0::type >::type fwd_state1;
    typedef typename next<iter0>::type iter1;
    
    typedef fwd_state1 bkwd_state1;
    typedef typename apply2< BackwardOp, bkwd_state1, typename iter0::type >::type bkwd_state0;
    typedef bkwd_state0 state;
    typedef iter1 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< 2,First,Last,State,BackwardOp,ForwardOp >
{
    typedef First iter0;
    typedef State fwd_state0;
    typedef typename apply2< ForwardOp, fwd_state0, typename iter0::type >::type fwd_state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, fwd_state1, typename iter1::type >::type fwd_state2;
    typedef typename next<iter1>::type iter2;
    
    typedef fwd_state2 bkwd_state2;
    typedef typename apply2< BackwardOp, bkwd_state2, typename iter1::type >::type bkwd_state1;
    typedef typename apply2< BackwardOp, bkwd_state1, typename iter0::type >::type bkwd_state0;
    
    typedef bkwd_state0 state;
    typedef iter2 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< 3,First,Last,State,BackwardOp,ForwardOp >
{
    typedef First iter0;
    typedef State fwd_state0;
    typedef typename apply2< ForwardOp, fwd_state0, typename iter0::type >::type fwd_state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, fwd_state1, typename iter1::type >::type fwd_state2;
    typedef typename next<iter1>::type iter2;
    typedef typename apply2< ForwardOp, fwd_state2, typename iter2::type >::type fwd_state3;
    typedef typename next<iter2>::type iter3;
    
    typedef fwd_state3 bkwd_state3;
    typedef typename apply2< BackwardOp, bkwd_state3, typename iter2::type >::type bkwd_state2;
    typedef typename apply2< BackwardOp, bkwd_state2, typename iter1::type >::type bkwd_state1;
    typedef typename apply2< BackwardOp, bkwd_state1, typename iter0::type >::type bkwd_state0;
    
    typedef bkwd_state0 state;
    typedef iter3 iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< 4,First,Last,State,BackwardOp,ForwardOp >
{
    typedef First iter0;
    typedef State fwd_state0;
    typedef typename apply2< ForwardOp, fwd_state0, typename iter0::type >::type fwd_state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, fwd_state1, typename iter1::type >::type fwd_state2;
    typedef typename next<iter1>::type iter2;
    typedef typename apply2< ForwardOp, fwd_state2, typename iter2::type >::type fwd_state3;
    typedef typename next<iter2>::type iter3;
    typedef typename apply2< ForwardOp, fwd_state3, typename iter3::type >::type fwd_state4;
    typedef typename next<iter3>::type iter4;
    
    typedef fwd_state4 bkwd_state4;
    typedef typename apply2< BackwardOp, bkwd_state4, typename iter3::type >::type bkwd_state3;
    typedef typename apply2< BackwardOp, bkwd_state3, typename iter2::type >::type bkwd_state2;
    typedef typename apply2< BackwardOp, bkwd_state2, typename iter1::type >::type bkwd_state1;
    typedef typename apply2< BackwardOp, bkwd_state1, typename iter0::type >::type bkwd_state0;
    
    typedef bkwd_state0 state;
    typedef iter4 iterator;
};

template<
      long N
    , typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl
{
    typedef First iter0;
    typedef State fwd_state0;
    typedef typename apply2< ForwardOp, fwd_state0, typename iter0::type >::type fwd_state1;
    typedef typename next<iter0>::type iter1;
    typedef typename apply2< ForwardOp, fwd_state1, typename iter1::type >::type fwd_state2;
    typedef typename next<iter1>::type iter2;
    typedef typename apply2< ForwardOp, fwd_state2, typename iter2::type >::type fwd_state3;
    typedef typename next<iter2>::type iter3;
    typedef typename apply2< ForwardOp, fwd_state3, typename iter3::type >::type fwd_state4;
    typedef typename next<iter3>::type iter4;
    
    typedef reverse_fold_impl<
          ( (N - 4) < 0 ? 0 : N - 4 )
        , iter4
        , Last
        , fwd_state4
        , BackwardOp
        , ForwardOp
        > nested_chunk;
    typedef typename nested_chunk::state bkwd_state4;
    typedef typename apply2< BackwardOp, bkwd_state4, typename iter3::type >::type bkwd_state3;
    typedef typename apply2< BackwardOp, bkwd_state3, typename iter2::type >::type bkwd_state2;
    typedef typename apply2< BackwardOp, bkwd_state2, typename iter1::type >::type bkwd_state1;
    typedef typename apply2< BackwardOp, bkwd_state1, typename iter0::type >::type bkwd_state0;
    
    typedef bkwd_state0 state;
    typedef typename nested_chunk::iterator iterator;
};

template<
      typename First
    , typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< -1,First,Last,State,BackwardOp,ForwardOp >
{
    typedef reverse_fold_impl<
          -1
        , typename next<First>::type
        , Last
        , typename apply2<ForwardOp, State, typename First::type>::type
        , BackwardOp
        , ForwardOp
        > nested_step;
    typedef typename apply2<
          BackwardOp
        , typename nested_step::state
        , typename First::type
        >::type state;
    typedef typename nested_step::iterator iterator;
};

template<
      typename Last
    , typename State
    , typename BackwardOp
    , typename ForwardOp
    >
struct reverse_fold_impl< -1,Last,Last,State,BackwardOp,ForwardOp >
{
    typedef State state;
    typedef Last iterator;
};

}}}
