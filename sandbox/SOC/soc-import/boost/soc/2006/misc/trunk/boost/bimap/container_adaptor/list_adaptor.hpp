// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/list_adaptor.hpp
/// \brief Container adaptor to easily build a std::list signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/sequence_container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/comparison_adaptor.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/call_traits.hpp>
#include <functional>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::list signature compatible container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,
    class ReverseIterator,
    class ConstReverseIterator,

    class IteratorToBaseConverter          = ::boost::mpl::na,
    class IteratorFromBaseConverter        = ::boost::mpl::na,
    class ReverseIteratorFromBaseConverter = ::boost::mpl::na,
    class ValueToBaseConverter             = ::boost::mpl::na,
    class ValueFromBaseConverter           = ::boost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::vector<>
>
class list_adaptor :

    public ::boost::bimap::container_adaptor::sequence_container_adaptor
    <
        Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses
    >
{
    typedef ::boost::bimap::container_adaptor::sequence_container_adaptor
    <
        Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // Access -----------------------------------------------------------------

    public:

    explicit list_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef list_adaptor list_adaptor_;

    // Interface -------------------------------------------------------------

    public:

    void splice(Iterator position, list_adaptor & x)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base()
        );
    }

    void splice(Iterator position, list_adaptor & x, Iterator i)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base(),
            this->template functor<typename base_::iterator_to_base>()(i)
        );
    }

    void splice(Iterator position, list_adaptor & x, Iterator first, Iterator last)
    {
        this->base().splice(
            this->template functor<typename base_::iterator_to_base>()(position),
            x.base(),
            this->template functor<typename base_::iterator_to_base>()(first),
            this->template functor<typename base_::iterator_to_base>()(last)
        );
    }

    void remove(typename ::boost::call_traits< typename base_::value_type >::param_type value)
    {
        this->base().remove(
            this->template functor<typename base_::value_to_base>()(value)
        );
    }

    template<typename Predicate>
    void remove_if(Predicate pred)
    {
        this->base().remove_if(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Predicate,
                typename Base::value_type,
                typename base_::value_from_base

            >( pred, this->template functor<typename base_::value_from_base>() )
        );
    }

    void unique()
    {
        this->base().unique(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::equal_to<typename base_::value_type>,
                typename Base::value_type,
                typename base_::value_from_base

            >(
                std::equal_to<typename base_::value_type>(),
                this->template functor<typename base_::value_from_base>()
            )
        );
    }

    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred)
    {
        this->base().unique(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                BinaryPredicate,
                typename Base::value_type,
                typename base_::value_from_base

            >( binary_pred, this->template functor<typename base_::value_from_base>() )
        );
    }

    void merge(list_adaptor & x)
    {
        this->base().merge(x.base(),
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::less<typename base_::value_type>,
                typename Base::value_type,
                typename base_::value_from_base

            >(
                std::less<typename base_::value_type>(),
                this->template functor<typename base_::value_from_base>()
            )
        );
    }

    template <typename Compare>
    void merge(list_adaptor & x, Compare comp)
    {
        this->base().merge(x.base(),
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Compare,
                typename Base::value_type,
                typename base_::value_from_base

            >( comp, this->template functor<typename base_::value_from_base>() )
        );
    }

    void sort()
    {
        this->base().sort(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                std::less<typename base_::value_type>,
                typename Base::value_type,
                typename base_::value_from_base

            >(
                std::less<typename base_::value_type>(),
                this->template functor<typename base_::value_from_base>()
            )
        );
    }

    template <typename Compare>
    void sort(Compare comp)
    {
        this->base().sort(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Compare,
                typename Base::value_type,
                typename base_::value_from_base

            >( comp, this->template functor<typename base_::value_from_base>() )
        );
    }

    void reverse()
    {
        this->base().reverse();
    }

};


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP


