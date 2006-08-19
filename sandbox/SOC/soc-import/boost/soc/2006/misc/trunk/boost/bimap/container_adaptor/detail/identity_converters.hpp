// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/detail/identity_converters.hpp
/// \brief Value and iterators identity converters.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_IDENTITY_CONVERTERS_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_IDENTITY_CONVERTERS_HPP

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {


/// \brief Iterator identity converter used by default in container adaptors.
/**
If Iterator and ConstIterator are of the same type one of the convert function is not
included.
                                                                                    **/

template
<
    class BaseIterator              , class Iterator,
    class BaseConstIterator         , class ConstIterator
>
struct iterator_to_base_identity
{
    BaseIterator operator()(Iterator iter) const
    {
        return BaseIterator(iter);
    }

    BaseConstIterator operator()(ConstIterator iter) const
    {
        return BaseConstIterator(iter);
    }
};

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class BaseIterator, class Iterator >
struct iterator_to_base_identity<BaseIterator,Iterator,BaseIterator,Iterator>
{
    BaseIterator operator()(Iterator iter) const
    {
        return BaseIterator(iter);
    }
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief Iterator from base identity converter used by default in container adaptors.
/**
If Iterator and ConstIterator are of the same type one of the convert function is not
included.
                                                                                    **/

template
<
    class BaseIterator              , class Iterator,
    class BaseConstIterator         , class ConstIterator
>
struct iterator_from_base_identity
{
    Iterator operator()(BaseIterator iter) const
    {
        return Iterator(iter);
    }
    ConstIterator operator()(BaseConstIterator iter) const
    {
        return ConstIterator(iter);
    }
};

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class BaseIterator, class Iterator >
struct iterator_from_base_identity<BaseIterator,Iterator,BaseIterator,Iterator>
{
    Iterator operator()(BaseIterator iter) const
    {
        return Iterator(iter);
    }
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief Value to base identity converter used by default in container adaptors.

template< class BaseValue, class Value >
struct value_to_base_identity
{
    BaseValue operator()(const Value & val) const
    {
        return BaseValue(val);
    }
};

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class Value >
struct value_to_base_identity< Value, Value >
{
    const Value & operator()(const Value & val) const
    {
        return val;
    }
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief Value from base identity converter used by default in container adaptors.

template< class BaseValue, class Value >
struct value_from_base_identity
{
    Value operator()(const BaseValue & val) const
    {
        return Value(val);
    }
};

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class Value >
struct value_from_base_identity<Value,Value>
{
    Value & operator()(Value & val) const
    {
        return val;
    }

    const Value & operator()(const Value & val) const
    {
        return val;
    }
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief Key to base identity converter used by default in container adaptors.

template< class BaseKey, class Key >
struct key_to_base_identity
{
    BaseKey operator()(const Key & k) const
    {
        return BaseKey(k);
    }
};

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class Key >
struct key_to_base_identity< Key, Key >
{
    const Key & operator()(const Key & k) const
    {
        return k;
    }
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

} // namespace detail
} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_IDENTITY_CONVERTERS_HPP


