// preprocessed version of 'boost/mpl/bind.hpp' header
// see the original for copyright information

namespace boost {
namespace mpl {

namespace aux {

template< bool >
struct resolve_arg_impl
{
    template<
          typename T, typename U1, typename U2, typename U3
        , typename U4 , typename U5
        >
    struct result_
    {
        typedef T type;
    };
};

template<> 
struct resolve_arg_impl<true>
{
    template<
          typename T, typename U1, typename U2, typename U3
        , typename U4 , typename U5
        >
    struct result_
    {
        typedef typename apply5< T,U1,U2,U3,U4,U5 >::type type;
    };
};

template< typename T > struct is_bind_template;

template< 
      typename T, typename U1, typename U2, typename U3, typename U4
    , typename U5
    >
struct resolve_bind_arg
    : resolve_arg_impl< is_bind_template<T >::value >
            ::template result_< T,U1,U2,U3,U4,U5 >
{
};

} // namespace aux

template<
      typename F, typename T1 = void_, typename T2 = void_
    , typename T3 = void_, typename T4 = void_, typename T5 = void_
    >
struct bind;

template< typename F, typename T > struct bind1st;
template< typename F, typename T > struct bind2nd;

namespace aux {

template< int > struct bind_impl_chooser;

aux::no_tag is_bind_helper(...);
template< typename T > aux::no_tag is_bind_helper(protect<T>*);

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
aux::yes_tag is_bind_helper(bind< F,T1,T2,T3,T4,T5 >*);

template< int N >
aux::yes_tag is_bind_helper(arg<N>*);

template< typename F, typename T > aux::yes_tag is_bind_helper(bind1st< F,T >*);
template< typename F, typename T > aux::yes_tag is_bind_helper(bind2nd< F,T >*);

template< typename T > struct is_bind_template
{
    enum { value =
         sizeof(aux::is_bind_helper(static_cast<T*>(0))) ==
         sizeof(aux::yes_tag)
        };
};

} // namespace aux

template<
      typename F
    >
struct bind0
{
    template<
          typename U1 = void_, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
    {
     private:
        typedef typename aux::resolve_bind_arg< F,U1,U2,U3,U4,U5 >::type f_;

     public:
        typedef typename apply0<f_>::type type;
    };
};

namespace aux {

template<
      typename F
    >
aux::yes_tag
is_bind_helper(bind0<F>*);

} // namespace aux

namespace aux {

template<>
struct bind_impl_chooser<0>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef bind0<F> type;
    };
};

} // namespace aux

template<
      typename F, typename T1
    >
struct bind1
{
    template<
          typename U1 = void_, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
    {
     private:
        typedef typename aux::resolve_bind_arg< F,U1,U2,U3,U4,U5 >::type f_;
        typedef typename aux::resolve_bind_arg< T1,U1,U2,U3,U4,U5 >::type t1;

     public:
        typedef typename apply1< f_,t1 >::type type;
    };
};

namespace aux {

template<
      typename F, typename T1
    >
aux::yes_tag
is_bind_helper(bind1< F,T1 >*);

} // namespace aux

namespace aux {

template<>
struct bind_impl_chooser<1>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef bind1< F,T1 > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2
    >
struct bind2
{
    template<
          typename U1 = void_, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
    {
     private:
        typedef typename aux::resolve_bind_arg< F,U1,U2,U3,U4,U5 >::type f_;
        typedef typename aux::resolve_bind_arg< T1,U1,U2,U3,U4,U5 >::type t1;
        typedef typename aux::resolve_bind_arg< T2,U1,U2,U3,U4,U5 >::type t2;

     public:
        typedef typename apply2< f_,t1,t2 >::type type;
    };
};

namespace aux {

template<
      typename F, typename T1, typename T2
    >
aux::yes_tag
is_bind_helper(bind2< F,T1,T2 >*);

} // namespace aux

namespace aux {

template<>
struct bind_impl_chooser<2>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef bind2< F,T1,T2 > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2, typename T3
    >
struct bind3
{
    template<
          typename U1 = void_, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
    {
     private:
        typedef typename aux::resolve_bind_arg< F,U1,U2,U3,U4,U5 >::type f_;
        typedef typename aux::resolve_bind_arg< T1,U1,U2,U3,U4,U5 >::type t1;
        typedef typename aux::resolve_bind_arg< T2,U1,U2,U3,U4,U5 >::type t2;
        typedef typename aux::resolve_bind_arg< T3,U1,U2,U3,U4,U5 >::type t3;

     public:
        typedef typename apply3< f_,t1,t2,t3 >::type type;
    };
};

namespace aux {

template<
      typename F, typename T1, typename T2, typename T3
    >
aux::yes_tag
is_bind_helper(bind3< F,T1,T2,T3 >*);

} // namespace aux

namespace aux {

template<>
struct bind_impl_chooser<3>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef bind3< F,T1,T2,T3 > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    >
struct bind4
{
    template<
          typename U1 = void_, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
    {
     private:
        typedef typename aux::resolve_bind_arg< F,U1,U2,U3,U4,U5 >::type f_;
        typedef typename aux::resolve_bind_arg< T1,U1,U2,U3,U4,U5 >::type t1;
        typedef typename aux::resolve_bind_arg< T2,U1,U2,U3,U4,U5 >::type t2;
        typedef typename aux::resolve_bind_arg< T3,U1,U2,U3,U4,U5 >::type t3;
        typedef typename aux::resolve_bind_arg< T4,U1,U2,U3,U4,U5 >::type t4;

     public:
        typedef typename apply4< f_,t1,t2,t3,t4 >::type type;
    };
};

namespace aux {

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    >
aux::yes_tag
is_bind_helper(bind4< F,T1,T2,T3,T4 >*);

} // namespace aux

namespace aux {

template<>
struct bind_impl_chooser<4>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef bind4< F,T1,T2,T3,T4 > type;
    };
};

} // namespace aux

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct bind5
{
    template<
          typename U1 = void_, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
    {
     private:
        typedef typename aux::resolve_bind_arg< F,U1,U2,U3,U4,U5 >::type f_;
        typedef typename aux::resolve_bind_arg< T1,U1,U2,U3,U4,U5 >::type t1;
        typedef typename aux::resolve_bind_arg< T2,U1,U2,U3,U4,U5 >::type t2;
        typedef typename aux::resolve_bind_arg< T3,U1,U2,U3,U4,U5 >::type t3;
        typedef typename aux::resolve_bind_arg< T4,U1,U2,U3,U4,U5 >::type t4;
        typedef typename aux::resolve_bind_arg< T5,U1,U2,U3,U4,U5 >::type t5;

     public:
        typedef typename apply5< f_,t1,t2,t3,t4,t5 >::type type;
    };
};

namespace aux {

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
aux::yes_tag
is_bind_helper(bind5< F,T1,T2,T3,T4,T5 >*);

} // namespace aux

namespace aux {

template<>
struct bind_impl_chooser<5>
{
    template<
          typename F, typename T1, typename T2, typename T3, typename T4
        , typename T5
        >
    struct result_
    {
        typedef bind5< F,T1,T2,T3,T4,T5 > type;
    };
};

} // namespace aux

namespace aux {

template< typename T >
struct is_bind_arg
{
    enum { value = true };
};

template<>
struct is_bind_arg<void_>
{
    enum { value = false };
};

template<
      typename T1, typename T2, typename T3, typename T4, typename T5
    >
struct bind_count_args
{
    enum { value =
          is_bind_arg<T1>::value + is_bind_arg<T2>::value 
        + is_bind_arg<T3>::value + is_bind_arg<T4>::value 
        + is_bind_arg<T5>::value
        };
};

}

template<
      typename F, typename T1, typename T2, typename T3, typename T4
    , typename T5
    >
struct bind
    : aux::bind_impl_chooser<
          aux::bind_count_args< T1,T2,T3,T4,T5 >::value
        >::template result_< F,T1,T2,T3,T4,T5 >::type
{
};

template< typename F, typename T >
struct bind1st
{
    template<
          typename U, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
        : apply2< F,T,U >
    {
    };
};

template< typename F, typename T >
struct bind2nd
{
    template<
          typename U, typename U2 = void_, typename U3 = void_
        , typename U4 = void_, typename U5 = void_
        >
    struct apply
        : apply2< F,U,T >
    {
    };
};

} // namespace mpl
} // namespace boost

