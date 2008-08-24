///////////////////////////////////////////////////////////////////////////////
/// \file arg.hpp
/// Contains definition of the argN transforms.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRANSFORM_ARG_HPP_EAN_11_01_2007
#define BOOST_PROTO_TRANSFORM_ARG_HPP_EAN_11_01_2007

#include <boost/proto/detail/prefix.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/traits.hpp>
#include <boost/proto/transform/impl.hpp>
#include <boost/proto/detail/suffix.hpp>

namespace boost { namespace proto
{

    /// \brief A PrimitiveTransform that returns the current expression
    /// unmodified
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// proto::terminal<int>::type & j = proto::_expr()(i);
    /// assert( boost::addressof(i) == boost::addressof(j) );
    /// \endcode
    struct _expr : transform<_expr>
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            typedef Expr result_type;

            /// Returns the current expression.
            /// \param expr The current expression.
            /// \return \c expr
            /// \throw nothrow
            typename impl::expr_param operator()(
                typename impl::expr_param expr
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return expr;
            }
        };
    };

    /// \brief A PrimitiveTransform that returns the current state
    /// unmodified
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// char ch = proto::_state()(i, 'a');
    /// assert( ch == 'a' );
    /// \endcode
    struct _state : transform<_state>
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            typedef State result_type;

            /// Returns the current state.
            /// \param state The current state.
            /// \return \c state
            /// \throw nothrow
            typename impl::state_param operator ()(
                typename impl::expr_param
              , typename impl::state_param state
              , typename impl::data_param
            ) const
            {
                return state;
            }
        };
    };

    /// \brief A PrimitiveTransform that returns the current data
    /// unmodified
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// std::string str("hello");
    /// std::string & data = proto::_data()(i, 'a', str);
    /// assert( &str == &data );
    /// \endcode
    struct _data : transform<_data>
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            typedef Data result_type;

            /// Returns the current data.
            /// \param data The current data.
            /// \return \c data
            /// \throw nothrow
            typename impl::data_param operator ()(
                typename impl::expr_param
              , typename impl::state_param
              , typename impl::data_param data
            ) const
            {
                return data;
            }
        };
    };

    /// \brief A PrimitiveTransform that returns N-th child of the current
    /// expression.
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// proto::terminal<int>::type & j = proto::_child_c<0>()(-i);
    /// assert( boost::addressof(i) == boost::addressof(j) );
    /// \endcode
    template<int N>
    struct _child_c : transform<_child_c<N> >
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            typedef
                typename result_of::child_c<Expr, N>::type
            result_type;

            /// Returns the N-th child of \c expr
            /// \pre <tt>arity_of\<Expr\>::::value \> N</tt> 
            /// \param expr The current expression.
            /// \return <tt>proto::child_c\<N\>(expr)</tt>
            /// \throw nothrow
            typename result_of::child_c<typename impl::expr_param, N>::type
            operator ()(
                typename impl::expr_param expr
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return proto::child_c<N>(expr);
            }
        };
    };

    /// \brief A PrimitiveTransform that returns the value of the
    /// current terminal expression.
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// int j = proto::_value()(i);
    /// assert( 42 == j );
    /// \endcode
    struct _value : transform<_value>
    {
        template<typename Expr, typename State, typename Data>
        struct impl : transform_impl<Expr, State, Data>
        {
            typedef
                typename result_of::value<Expr>::type
            result_type;

            /// Returns the value of the specified terminal expression.
            /// \pre <tt>arity_of\<Expr\>::::value == 0</tt>.
            /// \param expr The current expression.
            /// \return <tt>proto::value(expr)</tt>
            /// \throw nothrow
            typename result_of::value<typename impl::expr_param>::type
            operator ()(
                typename impl::expr_param expr
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return proto::value(expr);
            }
        };
    };

    /// \brief A unary CallableTransform that wraps its argument
    /// in a \c boost::reference_wrapper\<\>.
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// boost::reference_wrapper<proto::terminal<int>::type> j
    ///     = proto::when<_, proto::_byref(_)>()(i);
    /// assert( boost::addressof(i) == boost::addressof(j.get()) );
    /// \endcode
    struct _byref : callable
    {
        template<typename Sig>
        struct result;

        template<typename This, typename T>
        struct result<This(T)>
        {
            typedef boost::reference_wrapper<T const> const type;
        };

        template<typename This, typename T>
        struct result<This(T &)>
        {
            typedef boost::reference_wrapper<T> const type;
        };

        /// Wrap the parameter \c t in a \c boost::reference_wrapper\<\>
        /// \param t The object to wrap
        /// \return <tt>boost::ref(t)</tt>
        /// \throw nothrow
        template<typename T>
        boost::reference_wrapper<T> const operator ()(T &t) const
        {
            return boost::reference_wrapper<T>(t);
        }

        /// \overload
        ///
        template<typename T>
        boost::reference_wrapper<T const> const operator ()(T const &t) const
        {
            return boost::reference_wrapper<T const>(t);
        }
    };

    /// \brief A unary CallableTransform that strips references
    /// and \c boost::reference_wrapper\<\> from its argument.
    ///
    /// Example:
    ///
    /// \code
    /// proto::terminal<int>::type i = {42};
    /// int j = 67;
    /// int k = proto::when<_, proto::_byval(proto::_state)>()(i, boost::ref(j));
    /// assert( 67 == k );
    /// \endcode
    struct _byval : callable
    {
        template<typename Sig>
        struct result;

        template<typename This, typename T>
        struct result<This(T)>
        {
            typedef T type;
        };

        template<typename This, typename T>
        struct result<This(T &)>
          : result<This(T)>
        {};

        template<typename This, typename T>
        struct result<This(boost::reference_wrapper<T>)>
          : result<This(T)>
        {};

        /// \param t The object to unref
        /// \return <tt>t</tt>
        /// \throw nothrow
        template<typename T>
        T const &operator ()(T const &t) const
        {
            return t;
        }

        /// \overload
        ///
        template<typename T>
        T &operator ()(boost::reference_wrapper<T> const &t) const
        {
            return t;
        }
    };

    /// INTERNAL ONLY
    ///
    template<>
    struct is_callable<_expr>
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<>
    struct is_callable<_state>
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<>
    struct is_callable<_data>
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<int N>
    struct is_callable<_child_c<N> >
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<>
    struct is_callable<_value>
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<>
    struct is_callable<_byref>
      : mpl::true_
    {};

    /// INTERNAL ONLY
    ///
    template<>
    struct is_callable<_byval>
      : mpl::true_
    {};

}}

#endif
