// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_CONNECTION_OPERATORS_HPP
#define BOOST_DATAFLOW_CONNECTION_OPERATORS_HPP

#include <boost/dataflow/connection/operator/group.hpp>

namespace boost { namespace dataflow {

namespace operators {
    
    ///	Connects two components (typically as a part of a chain of components).
    /** This operator is identical to signals::operator| (it connects the
    left component to the right component, and returns a reference to the left component),
    except it is evaluated right to left.  This makes it semantics more suitable for
    connecting a chain of components.
    */
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, Input & >::type
    operator >>= (Input &input, Output &output)
    { connect(input, output); return input;}
    
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, Input & >::type
    operator >>= (Input &input, const Output &output)
    { connect(input, output); return input;}
    
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, const Input & >::type
    operator >>= (const Input &input, Output &output)
    { connect(input, output); return input;}
    
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, const Input & >::type
    operator >>= (const Input &input, const Output &output)
    { connect(input, output); return input;}
    
    /// Connects two components (typically as a part of branching from a single component).
    /** This operator is identical to signals::operator>>=, (it connects the
    left component to the right component, and returns a reference to the left component)
    except it is evaluated left to right.  This makes its semantics more suitable for
    branching connections.
    */
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, Input & >::type
    operator | (Input &input, Output &output)
    { connect(input, output); return input;}
    
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, Input & >::type
    operator | (Input &input, const Output &output)
    { connect(input, output); return input;}
    
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, const Input & >::type
    operator | (const Input &input, Output &output)
    { connect(input, output); return input;}
    
    template<typename Input, typename Output>
    typename boost::enable_if<dataflow::is_producer<Input>, const Input & >::type
    operator | (const Input &input, const Output &output)
    { connect(input, output); return input;}
    
} // namespace operators

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_CONNECTION_OPERATORS_HPP

