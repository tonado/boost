// -*- mode:c++ -*-
//
// Header file container_suite.hpp
//
// Copyright (c) 2003 Raoul M. Gough
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this material for any purpose is hereby
// granted without fee, provided the above notices are retained on all
// copies.  Permission to modify the material and to distribute modified
// versions is granted, provided the above notices are retained, and a
// notice that the material was modified is included with the above
// copyright notice.
//
// History
// =======
// 2003/ 8/23	rmg	File creation
//
// $Id$
//

#ifndef container_suite_rmg_20030823_included
#define container_suite_rmg_20030823_included

#include "suite_utils.hpp"
#include "iterator_suite.hpp"
#include <boost/type_traits.hpp>

#include <set>
#include <map>
#include <list>
#include <deque>
#include <vector>

namespace indexing {
  //////////////////////////////////////////////////////////////////////////
  // Iterator pair container emulation
  //////////////////////////////////////////////////////////////////////////

  template<typename Iterator>
  class iterator_pair
  {
  private:
    typedef typename boost::call_traits<Iterator>::param_type iterator_param;
    typedef std::iterator_traits<Iterator> std_traits;

  public:
    typedef typename std_traits::reference       reference;
    typedef Iterator                             iterator;
    typedef typename std_traits::difference_type size_type;
    typedef typename std_traits::difference_type difference_type;
    typedef typename std_traits::value_type      value_type;
    typedef typename std_traits::pointer         pointer;

    // Can't provide: const_iterator, allocator_type, reverse_iterator
    // or const_reverse_iterator. Could probably provide (but don't)
    // const_reference and const_pointer. These would be the same
    // as reference and pointer if Iterator is itself a const_iterator.

  public:
    iterator_pair (iterator_param, iterator_param);

    iterator begin() const;
    iterator end() const;

  public:
    // Only sensible for random_access iterators
    size_type size () const;
    reference operator[] (size_type) const;

  private:
    iterator myBegin;
    iterator myEnd;
  };

  template<typename Iterator>
  iterator_pair<Iterator>::iterator_pair (iterator_param begin
					  , iterator_param end)
    : myBegin (begin)
    , myEnd (end)
  {
  }

  template<typename Iterator>
  typename iterator_pair<Iterator>::iterator
  iterator_pair<Iterator>::begin() const
  {
    return myBegin;
  }

  template<typename Iterator>
  typename iterator_pair<Iterator>::iterator
  iterator_pair<Iterator>::end() const
  {
    return myEnd;
  }

  template<typename Iterator>
  typename iterator_pair<Iterator>::size_type
  iterator_pair<Iterator>::size() const
  {
    return std::distance (begin(), end());
  }

  template<typename Iterator>
  typename iterator_pair<Iterator>::reference
  iterator_pair<Iterator>::operator[](size_type index) const
  {
    return *(begin() + index);
  }

  /////////////////////////////////////////////////////////////////////////
  // Container traits
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  // Traits for the iterator_pair container emulator
  /////////////////////////////////////////////////////////////////////////

  template<typename IteratorPair>
  struct iterator_pair_traits
    : public
    iterator_detail::traits_by_category<typename IteratorPair::iterator>::type
  {
    typedef IteratorPair container;
    typedef typename IteratorPair::size_type        size_type;
    typedef typename IteratorPair::size_type        index_type; // at()

    static bool const has_insert         = false;
    static bool const has_erase          = false;
    static bool const has_pop_back       = false;
    static bool const has_push_back      = false;

    // Default implementations of support functions
    typedef container_algorithms<iterator_pair_traits> algorithms;
  };

  /////////////////////////////////////////////////////////////////////////
  // Lowest common denominator (almost all "real" containers would
  // meet at least these requirements)
  /////////////////////////////////////////////////////////////////////////

  template<typename Container>
  struct default_container_traits
  {
  protected:
    static bool const is_mutable = ! boost::is_const<Container>::value;

  public:
    typedef Container container;

    typedef typename Container::value_type       value_type;
    typedef typename Container::reference        reference;
    typedef typename Container::difference_type  difference_type;
    typedef typename Container::size_type        size_type;
    typedef typename Container::size_type        index_type; // at()
    typedef value_type                           key_type; // find, count, ...

    // Should probably select iterator or const_iterator on the
    // basis of is_mutable
    typedef typename Container::iterator         iterator;
    typedef typename Container::const_iterator   const_iterator;

    static bool const has_copyable_iter  = true;

    static bool const has_random_access
      = ::indexing::iterator_traits<iterator>::has_random_access;

    static bool const has_mutable_ref
      = is_mutable && is_mutable_ref<reference>::value;

    // has_mutable_ref basically means that the container supports
    // in-place replacement of values (e.g. the associative containers
    // *don't*)

    static bool const is_reversible      = has_mutable_ref;

    static bool const has_insert         = is_mutable;
    static bool const has_erase          = is_mutable;
    static bool const has_pop_back       = false;
    static bool const has_push_back      = false;

    // Default implementations of support functions
    typedef container_algorithms<default_container_traits> algorithms;
  };

  /////////////////////////////////////////////////////////////////////////
  // Sequences (list, deque, vector)
  /////////////////////////////////////////////////////////////////////////

  template<typename Container>
  struct default_sequence_traits : public default_container_traits<Container>
  {
    static bool const has_pop_back       = is_mutable;
    static bool const has_push_back      = is_mutable;
  };

  template<typename Container>
  struct list_traits : public default_sequence_traits<Container>
  {
    // Some special algo's for list (using member functions)
    typedef list_algorithms<list_traits> algorithms;
  };

  /////////////////////////////////////////////////////////////////////////
  // Associative containers set and multiset
  /////////////////////////////////////////////////////////////////////////

  template<typename Container>
  struct set_traits : public default_container_traits<Container>
  {
    // Special algo's for set types (using member functions)
    typedef assoc_algorithms<set_traits> algorithms;
  };

  /////////////////////////////////////////////////////////////////////////
  // Associative containers map and multimap
  /////////////////////////////////////////////////////////////////////////

  template<typename Container>
  struct map_traits : public default_container_traits<Container>
  {
    typedef typename Container::key_type key_type; // find, count, ...

    // Special algo's for map types (using member functions)
    typedef assoc_algorithms<map_traits> algorithms;
  };

  /////////////////////////////////////////////////////////////////////////
  // Automated trait selection
  /////////////////////////////////////////////////////////////////////////

  namespace container_details {
    template<typename Container> struct traits_by_type;

    // traits_by_type instances should include two typedefs, one for
    // the non-const version of the container, and one for the
    // const version. This saves having to have two specializations
    // of traits_by_type for every kind of container.

    // std::set
    template <class Key, class Compare, class Allocator>
    class traits_by_type<std::set<Key, Compare, Allocator> >
    {
      typedef std::set<Key, Compare, Allocator> Container;

    public:
      typedef set_traits<Container>       mutable_type;
      typedef set_traits<Container const> const_type;
    };

    // std::multiset
    template <class Key, class Compare, class Allocator>
    class traits_by_type<std::multiset<Key, Compare, Allocator> >
    {
      typedef std::multiset<Key, Compare, Allocator> Container;

    public:
      typedef set_traits<Container>       mutable_type;
      typedef set_traits<Container const> const_type;
    };

    // std::map
    template <class Key, class T, class Compare, class Allocator>
    class traits_by_type<std::map<Key, T, Compare, Allocator> >
    {
      typedef std::map<Key, T, Compare, Allocator> Container;

    public:
      typedef map_traits<Container>       mutable_type;
      typedef map_traits<Container const> const_type;
    };

    // std::multimap
    template <class Key, class T, class Compare, class Allocator>
    class traits_by_type<std::multimap<Key, T, Compare, Allocator> >
    {
      typedef std::multimap<Key, T, Compare, Allocator> Container;

    public:
      typedef map_traits<Container>       mutable_type;
      typedef map_traits<Container const> const_type;
    };

    // std::vector
    template <class T, class Allocator>
    class traits_by_type<std::vector<T, Allocator> >
    {
      typedef std::vector<T, Allocator> Container;

    public:
      typedef default_sequence_traits<Container>       mutable_type;
      typedef default_sequence_traits<Container const> const_type;
    };

    // std::deque
    template <class T, class Allocator>
    class traits_by_type<std::deque<T, Allocator> >
    {
      typedef std::deque<T, Allocator> Container;

    public:
      typedef default_sequence_traits<Container>       mutable_type;
      typedef default_sequence_traits<Container const> const_type;
    };

    // std::list
    template <class T, class Allocator>
    class traits_by_type<std::list<T, Allocator> >
    {
      typedef std::list<T, Allocator> Container;

    public:
      typedef list_traits<Container>       mutable_type;
      typedef list_traits<Container const> const_type;
    };

    // Iterator ranges
    template <typename Iterator>
    class traits_by_type<indexing::iterator_pair<Iterator> >
    {
      typedef ::indexing::iterator_pair<Iterator> Container;

    public:
      typedef iterator_pair_traits<Container>       mutable_type;
      typedef iterator_pair_traits<Container const> const_type; // ?
    };
  }

  // Select the right traits for each supported kind of container

  // Generic version (mutable containers)
  template<class Container>
  struct container_traits
    : public container_details::traits_by_type<Container>::mutable_type
  {
  };

  // Partial specialization for const containers
  template<class Container>
  struct container_traits<Container const>
    : public container_details::traits_by_type<Container>::const_type
  {
  };
}

#endif // container_suite_rmg_20030823_included
