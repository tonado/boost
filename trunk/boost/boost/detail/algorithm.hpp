// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#ifndef BOOST_ALGORITHM_HPP
#define BOOST_ALGORITHM_HPP

// Algorithms on sequences

#include <algorithm>

namespace boost {

  template <typename Iter1, typename Iter2>
  Iter1 begin(const std::pair<Iter1, Iter2>& p) { return p.first; }

  template <typename Iter1, typename Iter2>
  Iter2 end(const std::pair<Iter1, Iter2>& p) { return p.second; }

  template <typename Iter1, typename Iter2>
  typename std::iterator_traits<Iter1>::difference_type
  size(const std::pair<Iter1, Iter2>& p) {
    return std::distance(p.first, p.second);
  }

#if 0
  // These seem to interfere with the std::pair overloads :(
  template <typename Container>
  typename Container::iterator
  begin(Container& c) { return c.begin(); }

  template <typename Container>
  typename Container::const_iterator
  begin(const Container& c) { return c.begin(); }

  template <typename Container>
  typename Container::iterator
  end(Container& c) { return c.end(); }

  template <typename Container>
  typename Container::const_iterator
  end(const Container& c) { return c.end(); }

  template <typename Container>
  typename Container::size_type
  size(const Container& c) { return c.size(); }
#else
  template <typename T>
  typename std::vector<T>::iterator
  begin(std::vector<T>& c) { return c.begin(); }

  template <typename T>
  typename std::vector<T>::const_iterator
  begin(const std::vector<T>& c) { return c.begin(); }

  template <typename T>
  typename std::vector<T>::iterator
  end(std::vector<T>& c) { return c.end(); }

  template <typename T>
  typename std::vector<T>::const_iterator
  end(const std::vector<T>& c) { return c.end(); }

  template <typename T>
  typename std::vector<T>::size_type
  size(const std::vector<T>& c) { return c.size(); }
#endif
  
  template <class ForwardIterator, class T>
  void iota(ForwardIterator first, ForwardIterator last, T value)
  {
    for (; first != last; ++first, ++value)
      *first = value;
  }
  template <typename Container, typename T>
  void iota(Container& c, const T& value)
  {
    iota(begin(c), end(c), value);
  }
 
  // Also do version with 2nd container?
  template <typename Container, typename OutIter>
  OutIter copy(const Container& c, OutIter result) {
    return std::copy(begin(c), end(c), result);
  }

  template <typename Container1, typename Container2>
  bool equal(const Container1& c1, const Container2& c2)
  {
    if (size(c1) != size(c2))
      return false;
    return std::equal(begin(c1), end(c1), begin(c2));
  }

  template <typename Container>
  void sort(Container& c) { std::sort(begin(c), end(c)); }

  template <typename Container, typename Predicate>
  void sort(Container& c, const Predicate& p) { 
    std::sort(begin(c), end(c), p);
  }

  template <typename Container>
  void stable_sort(Container& c) { std::stable_sort(begin(c), end(c)); }

  template <typename Container, typename Predicate>
  void stable_sort(Container& c, const Predicate& p) { 
    std::stable_sort(begin(c), end(c), p);
  }

  template <typename InputIterator, typename Predicate>
  bool any_if(InputIterator first, InputIterator last, Predicate p)
  {
    return std::find_if(first, last, p) != last;
  }
  template <typename Container, typename Predicate>
  bool any_if(const Container& c, Predicate p)
  {
    return any_if(begin(c), end(c), p);
  }

  template <typename InputIterator, typename T>
  bool contains(InputIterator first, InputIterator last, T value)
  {
    return std::find(first, last, value) != last;
  }
  template <typename Container, typename T>
  bool contains(const Container& c, const T& value)
  {
    return contains(begin(c), end(c), value);
  }

  template <typename InputIterator, typename Predicate>
  bool all(InputIterator first, InputIterator last, Predicate p)
  {
    for (; first != last; ++first)
      if (!p(*first))
        return false;
    return true;
  }
  template <typename Container, typename Predicate>
  bool all(const Container& c, Predicate p)
  {
    return all(begin(c), end(c), p);
  }

  template <typename InputIterator, typename Predicate>
  bool none(InputIterator first, InputIterator last, Predicate p)
  {
    return std::find_if(first, last, p) == last;
  }
  template <typename Container, typename Predicate>
  bool none(const Container& c, Predicate p)
  {
    return none(begin(c), end(c), p);
  }

  template <typename Container, typename T>
  std::size_t count(const Container& c, const T& value)
  {
    return std::count(begin(c), end(c), value);
  }

  template <typename Container, typename Predicate>
  std::size_t count_if(const Container& c, Predicate p)
  {
    return std::count_if(begin(c), end(c), p);
  }

} // namespace boost

#endif // BOOST_ALGORITHM_HPP
