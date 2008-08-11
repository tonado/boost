//[ FutureGroup
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This is an example of using Proto transforms to implement
// Howard Hinnant's future group proposal.

#include <boost/fusion/tuple.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/joint_view.hpp>
#include <boost/fusion/include/single_view.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
namespace mpl = boost::mpl;
namespace proto = boost::proto;
namespace fusion = boost::fusion;
using proto::_;

template<class L,class R>
struct pick_left
{
    BOOST_MPL_ASSERT((boost::is_same<L, R>));
    typedef L type;
};

// Define the grammar of future group expression, as well as a
// transform to turn them into a Fusion sequence of the correct
// type.
struct FutureGroup
  : proto::or_<
        // terminals become a single-element Fusion sequence
        proto::when<
            proto::terminal<_>
          , fusion::single_view<proto::_value>(proto::_value)
        >
        // (a && b) becomes a concatenation of the sequence
        // from 'a' and the one from 'b':
      , proto::when<
            proto::logical_and<FutureGroup, FutureGroup>
          , fusion::joint_view<
                boost::add_const<FutureGroup(proto::_left)>,
                boost::add_const<FutureGroup(proto::_right)>
            >(FutureGroup(proto::_left), FutureGroup(proto::_right))
        >
        // (a || b) becomes the sequence for 'a', so long
        // as it is the same as the sequence for 'b'.
      , proto::when<
            proto::logical_or<FutureGroup, FutureGroup>
          , pick_left<
                FutureGroup(proto::_left)
              , FutureGroup(proto::_right)
            >(FutureGroup(proto::_left))
        >
    >
{};

template<class E>
struct future_expr;

struct future_dom
  : proto::domain<proto::generator<future_expr>, FutureGroup>
{};

// Expressions in the future group domain have a .get()
// member function that (ostensibly) blocks for the futures
// to complete and returns the results in an appropriate
// tuple.
template<class E>
struct future_expr
  : proto::extends<E, future_expr<E>, future_dom>
{
    explicit future_expr(E const &e)
      : proto::extends<E, future_expr<E>, future_dom>(e)
    {}

    typename fusion::result_of::as_vector<
        typename boost::result_of<FutureGroup(E,int,int)>::type
    >::type
    get() const
    {
        int i = 0;
        return fusion::as_vector(FutureGroup()(*this, i, i));
    }
};

// The future<> type has an even simpler .get()
// member function.
template<class T>
struct future
  : future_expr<typename proto::terminal<T>::type>
{
    future(T const &t = T())
      : future_expr<typename proto::terminal<T>::type>(
            proto::terminal<T>::type::make(t)
        )
    {}

    T get() const
    {
        return proto::value(*this);
    }
};

// TEST CASES
struct A {};
struct B {};
struct C {};

int main()
{
    using fusion::tuple;
    future<A> a;
    future<B> b;
    future<C> c;
    future<tuple<A,B> > ab;

    // Verify that various future groups have the
    // correct return types.
    A                       t0 = a.get();
    tuple<A, B, C>          t1 = (a && b && c).get();
    tuple<A, C>             t2 = ((a || a) && c).get();
    tuple<A, B, C>          t3 = ((a && b || a && b) && c).get();
    tuple<tuple<A, B>, C>   t4 = ((ab || ab) && c).get();

    return 0;
}
//]
