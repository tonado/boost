// (C) Copyright David Abrahams 2002.
// (C) Copyright Jeremy Siek    2002.
// (C) Copyright Thomas Witt    2002.
// Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BOOST_FILTER_ITERATOR_23022003THW_HPP
#define BOOST_FILTER_ITERATOR_23022003THW_HPP

#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>

namespace boost
{
  
  namespace detail {

    template <class Iterator>
    struct filter_iterator_traits
      : iterator_traits<Iterator>
    {
      typedef iterator_tag<
        typename return_category<Iterator>::type
        , minimum_category< forward_traversal_tag
                            ,typename traversal_category<Iterator>::type >::type 
        > iterator_category;
    };

  } // namespace detail

  template <class Predicate, class Iterator>
  class filter_iterator
      : public iterator_adaptor<
           filter_iterator<Predicate, Iterator>, Iterator
           , detail::filter_iterator_traits<Iterator>
        >
  {
      typedef iterator_adaptor<
           filter_iterator<Predicate, Iterator>, Iterator
          , detail::filter_iterator_traits<Iterator>
      > super_t;

      friend class iterator_core_access;

   public:
      filter_iterator() { }

      filter_iterator(Predicate f, Iterator x, Iterator end = Iterator())
          : super_t(x), m_predicate(f), m_end(end)
      {
          satisfy_predicate();
      }

      filter_iterator(Iterator x, Iterator end = Iterator())
          : super_t(x), m_predicate(), m_end(end)
      {
          satisfy_predicate();
      }

      template<class OtherIterator>
      filter_iterator(
          filter_iterator<Predicate, OtherIterator> const& t
          , typename enable_if_convertible<OtherIterator, Iterator>::type* = 0
          )
          : super_t(t.base()), m_predicate(t.predicate()), m_end(t.end()) {}

      Predicate predicate() const { return m_predicate; }

      Iterator end() const { return m_end; }

   private:
      void increment()
      {
          super_t::increment();
          satisfy_predicate();
      }

      void satisfy_predicate()
      {
          while (this->base() != this->m_end && !this->m_predicate(*this->base()))
              super_t::increment();
      }

      // Probably should be the initial base class so it can be
      // optimized away via EBO if it is an empty class.
      Predicate m_predicate;
      Iterator m_end;
  };

  template <class Predicate, class Iterator>
  filter_iterator<Predicate,Iterator>
  make_filter_iterator(Predicate f, Iterator x, Iterator end = Iterator())
  {
      return filter_iterator<Predicate,Iterator>(f,x,end);
  }

} // namespace boost

#endif // BOOST_FILTER_ITERATOR_23022003THW_HPP
