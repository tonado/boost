// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/mutant_relation.hpp
/// \brief Defines the mutant_relation class

#ifndef BOOST_BIMAP_RELATION_MUTANT_RELATION_HPP
#define BOOST_BIMAP_RELATION_MUTANT_RELATION_HPP

#include <boost/mpl/list.hpp>
#include <boost/operators.hpp>
#include <boost/call_traits.hpp>

// Boost.Bimap
#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/tags/support/default_tagged.hpp>
#include <boost/bimap/tags/support/tag_of.hpp>
#include <boost/bimap/tags/support/value_type_of.hpp>

#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/detail/mutant.hpp>
#include <boost/bimap/relation/structured_pair.hpp>

namespace boost {
namespace bimap {
namespace relation {

/// \brief Abstraction of a related pair of values, that extends the std::pair class.
/**
The mutant_relation is a mutant class. A mutant class can mutate
with zero overhead in other classes that are called views.
Each view has to be StorageCompatible with the base class
of the mutant. Note that all the views have the following
storage structure:

\verbatim
                        __________
                       |          |
                       |    TA    |
                       |__________|
                       |          |
                       |    TB    |
                       |__________|

\endverbatim

See also select_relation, standard_relation.
\ingroup relation_group
                                                           **/

template< class TA, class TB >
class mutant_relation :
    public symmetrical_base<TA,TB>,
    public detail::can_mutate_in
    <
        mpl::list2
        <
            structured_pair< TA, TB, normal_layout >,
            structured_pair< TB, TA, mirror_layout >
        >
    >,
    totally_ordered<
        mutant_relation<TA,TB>,
    totally_ordered<
        structured_pair<TA,TB,normal_layout>,
    totally_ordered<
        structured_pair<TB,TA,mirror_layout>

    > > >
{
    public:

    //@{
        /// A signature compatible std::pair that is a view of the relation.

        typedef structured_pair< TA, TB, normal_layout > left_pair ;
        typedef structured_pair< TB, TA, mirror_layout > right_pair;

        typedef left_pair  & left_pair_reference ;
        typedef right_pair & right_pair_reference;

        typedef const left_pair  & const_left_pair_reference ;
        typedef const right_pair & const_right_pair_reference;
    //@}

    //@{

        /// data, exposed for easy manipulation
        typename mutant_relation::left_value_type  left;
        typename mutant_relation::right_value_type right;

    //@}

    mutant_relation() {}

    mutant_relation(typename boost::call_traits<
                        typename mutant_relation::left_value_type >::param_type l,
                    typename boost::call_traits<
                        typename mutant_relation::right_value_type>::param_type r) :
        left (l),
        right(r)
    {}

    mutant_relation(const mutant_relation & rel) :

        left (rel.left),
        right(rel.right)
    {}

    // Allow to create relations from views

    mutant_relation(const left_pair & lp) :

        left ( lp.first  ),
        right( lp.second )
    {}

    mutant_relation(const right_pair & rp) :

        left ( rp.second ),
        right( rp.first  )
    {}

    // Allow to create a relation from a std pair
    // This allows to better integration with the stl

    typedef std::pair
    <
        typename mutant_relation::left_value_type,
        typename mutant_relation::right_value_type

    > std_pair;

    mutant_relation(const std_pair & p) :

        left ( p.first  ),
        right( p.second )
    {}

    // Operators

    mutant_relation& operator=(const mutant_relation & rel)
    {
        left  = rel.left ;
        right = rel.right;
        return *this;
    }

    mutant_relation& operator=(const left_pair & p)
    {
        left  = p.first ;
        right = p.second;
        return *this;
    }

    mutant_relation& operator=(const right_pair & p)
    {
        left  = p.second;
        right = p.first ;
        return *this;
    }

    // Operators required by Boost.Operators

    bool operator==(const mutant_relation & rel) const
    {
        return ( ( left  == rel.left  ) &&
                 ( right == rel.right ) );
    }

    bool operator<(const mutant_relation & rel) const
    {
        return ( ( left  < rel.left  ) &&
                 ( right < rel.right ) );
    }

    bool operator==(const left_pair & p) const
    {
        return ( ( left  == p.first  ) &&
                 ( right == p.second ) );
    }

    bool operator<(const left_pair & p) const
    {
        return ( ( left   < p.first   ) &&
                 ( right  < p.second  ) );
    }

    bool operator==(const right_pair & p) const
    {
        return ( ( left  == p.second  ) &&
                 ( right == p.first   ) );
    }

    bool operator<(const right_pair & p) const
    {
        return ( ( left   < p.second  ) &&
                 ( right  < p.first   ) );
    }

    // The following functions are redundant if you only consider this class.
    // They are included to make easier the construction of the get and the
    // pair_by metafunction. Remember that not all compiler supports the mutant
    // idiom.

    typename mutant_relation::left_value_type & get_left()
    {
        return left;
    }

    const typename mutant_relation::left_value_type & get_left() const
    {
        return left;
    }

    typename mutant_relation::right_value_type & get_right()
    {
        return right;
    }

    const typename mutant_relation::right_value_type & get_right() const
    {
        return right;
    }

    left_pair_reference get_left_pair()
    {
        return detail::mutate<left_pair>(*this);
    }

    const_left_pair_reference get_left_pair() const
    {
        return detail::mutate<left_pair>(*this);
    }

    right_pair_reference get_right_pair()
    {
        return detail::mutate<right_pair>(*this);
    }

    const_right_pair_reference get_right_pair() const
    {
        return detail::mutate<right_pair>(*this);
    }

};

} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_MUTANT_RELATION_HPP



