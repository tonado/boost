// combinatorial.h header file - r-permutation and r-combination algorithms ---//

// Copyright � Philip F. Garofalo 2002. All rights reserved.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

// See http://www.boost.org for updates, documentation, and revision
// history.

// Jun 20 2002  Removed TABs and put in std:: when needed [Herve Bronnimann]
//              Also replaced all iter_swap(i,j) by swap(*i,*j)
//              
//              Heck, while I'm at it, I corrected is_sorted, which was
//              using --last (not part of ForwardIterator)
//              I hope I gave an even better implementation that Jeremy
//              and the SGI STL :)                        [Herve Bronnimann]

// Jun ?? 2002  Renamed file to combinatorial.hpp from rcombo.hpp. I also
//              renamed the function smallest_greater to min_element_greater than
//              and largest_less to max_element_less_than.

// Jun 27 2002  All functions now throw std::out_of_bounds if r (the iterator
//              that points into the middle of the sequence) is not within
//              (first, last]. The combination functions throw std::invalid_argument
//              if the selection [first, r) is not in sorted order. [Phil Garofalo]
 


#ifndef BOOST_COMBINATORIAL_HPP
#define BOOST_COMBINATORIAL_HPP

#include <algorithm>            // std::sort(), std::rotate(), etc.
#include <functional>           // greater<>(), not2()
#include <stdexcept>            // out_of_range, invalid_argument
#include <boost/utility.hpp>    // next()

namespace boost {
    
    // is_sorted -------------------------------------------------------//
    
    // Returns true if [first, last) is in std::sorted order, based on the
    // less than operator.
    
    template<class ForwardIterator>
        inline bool
        is_sorted(ForwardIterator first, ForwardIterator last)
    {
        if (first != last)
            for (ForwardIterator old = first; ++first != last; old = first)
                if (*first < *old)
                    return false;
                
                return true;
    }
    
    // is_sorted -------------------------------------------------------//
    
    // Returns true if [first, last) is in std::sorted order, based on
    // a user-supplied binary comparison operator.
    
    template<class ForwardIterator, class Compare>
        inline bool
        is_sorted(ForwardIterator first, ForwardIterator last, Compare comp)
    {
        if (first != last)
            for (ForwardIterator old = first; ++first != last; old = first)
                if (comp(*first, *old))
                    return false;
                
                return true;
    }
    
    // min_element_greater_than -------------------------------------------------//
    
    // Returns an iterator pointing to the smallest value in [first, last)
    // greater than x. last is returned if value is not found.
    
    // Time complexity is linear, O((last-first)/2) assignments in average
    // case.
    
    template<class ForwardIterator, class T>
        inline ForwardIterator
        min_element_greater_than(ForwardIterator first, ForwardIterator last,
            const T& x)
    {
        ForwardIterator hold = last;
        for (; first != last; first++)
            if (x < *first)
                if (hold == last)
                    hold = first;
                else
                    if (*first < *hold)
                        hold = first;
                    return hold;
    }    // min_element_greater_than
    
    // min_element_greater_than -------------------------------------------------//
    
    // Returns an iterator pointing to the smallest value in [first, last) 
    // greater than x, based on a user-supplied binary comparison operator.
    // last is returned if value is not found.
    
    // Time complexity is linear, O((last-first)/2) assignments in average
    // case.
    
    template<class ForwardIterator, class T, class Compare>
        inline ForwardIterator
        min_element_greater_than(ForwardIterator first, ForwardIterator last,
            const T& x, Compare comp)
    {
        ForwardIterator hold = last;
        for (; first != last; first++)
            if (comp(x, *first))
                if (hold == last)
                    hold = first;
                else
                    if (comp(*first, *hold))
                        hold = first;
                    return hold;
    }    // min_element_greater_than
    
    
    // max_element_less_than -----------------------------------------------------//
    
    // Returns an iterator pointing to the largest object in [first, last)
    // less than x. last is returned if no such value is present.
    
    template<class ForwardIterator, class T>
        inline ForwardIterator
        max_element_less_than(ForwardIterator first, ForwardIterator last, const T& x)
    {
        ForwardIterator hold = last;
        for(; first != last; first++)
            if (*first < x)
                if (hold == last)
                    hold = first;
                else
                    if (*hold < *first)
                        hold = first;
                    return hold;
    }    // max_element_less_than
    
    // max_element_less_than -------------------------------------------------------//
    
    // Returns an iterator pointing to the largest object in [first, last)
    // less than x, based on a user-supplied binary comparison operator.
    // last is returned if no such object is present.
    
    template<class ForwardIterator, class T, class Compare>
        inline ForwardIterator
        max_element_less_than(ForwardIterator first, ForwardIterator last,
            const T& x, Compare comp)
    {
        ForwardIterator hold = last;
        for(; first != last; first++)
            if (comp(*first, x))
                if (hold == last)
                    hold = first;
                else
                    if (comp(*hold, *first))
                        hold = first;
                    return hold;
    }    // max_element_less_than
    
    
    // next_r_permutation -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the next
    // r-permutation of elements in lexicographical order. When calling
    // the function for the first time, the elements in [first, last) must
    // be in ascending lexicographical order. Typically, when the function
    // is called it arranges the next r-permutation in [first, r) and returns
    // true. When the last permutation in lexicographical order is passed in,
    // the function std::sorts the entire range, [first, last) into ascending order,
    // restarting the sequence, and returns false.
    
    template<class RandomAccessIterator>
        bool
        next_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("next_r_permutation: r is not in (first, last]");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after index i.
            RandomAccessIterator k = min_element_greater_than(i + 1, last, *i);
            
            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::sort(first, last);    // O(n lg n)
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
                else
                {
                    std::swap(*i,* k);
                    std::partial_sort(i + 1, r, last);    // O(n lg n), heapsort
                    return true;
                }    // else
        }    // while
    }    // next_r_permutation
    
    // next_r_permutation -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied binary
    // comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        next_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("next_r_permutation: r is not in (first, last]");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after index i.
            RandomAccessIterator k = min_element_greater_than(i + 1, last, *i, comp);
            
            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::sort(first, last, comp);
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
                else
                {
                    std::swap(*i,* k);
                    std::partial_sort(i + 1, r, last, comp);    // O(n lg n), heapsort
                    return true;
                }    // else
        }    // while
    }    // next_r_permutation
    
    // prev_r_permutation -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the previous
    // r-permutation of elements in lexicographical order. When calling
    // the function for the first time, the elements in [first, last) must
    // be in descending lexicographical order. Typically, when the function
    // is called it arranges the previous r-permutation in [first, r)
    // and returns true. When the first permutation in lexicographical,
    // order is passed in the function std::sorts the entire range, [first, last)
    // into descending order, restarting the sequence at the end, and
    // returns false.
    
    template<class RandomAccessIterator>
        bool
        prev_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;

        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("prev_r_permutation: r is not in (first, last]");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after index i.
            RandomAccessIterator k = max_element_less_than(i + 1, last, *i);
            
            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::sort(first, last, greater<T>());
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
                else
                {
                    std::swap(*i,* k);
                    std::partial_sort(i+1, r, last, greater<T>());    // O(n lg n), heapsort
                    return true;
                }    // else
        }    // while
    }    // prev_r_permutation
    
    // prev_r_permutation -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied
    // binary comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        prev_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("prev_r_permutation: r is not in (first, last]");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after index i.
            RandomAccessIterator k = max_element_less_than(i + 1, last, *i, comp);
            
            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::sort(first, last, not2(comp));
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
                else
                {
                    std::swap(*i,* k);
                    std::partial_sort(i + 1, r, last, not2(comp));    // O(n lg n), heapsort 
                    return true;
                }    // else
        }    // while
    }    // prev_r_permutation
    
    
    // next_r_combination -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the next
    // r-combination of elements in lexicographical order. The elements
    // in [first, last) must be in ascending lexicographical order.
    // When the function is called and a next combination exists,
    // it arranges the next r-combination in [first, r) and returns true. 
    // If the next combination does not exist, the function std::sorts the entire 
    // range, [first, last) into ascending order, restarting the sequence,
    // and returns false.
    
    template<class RandomAccessIterator>
        bool
        next_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("next_r_combination: r is not in (first, last]");
        if (!is_sorted(first, r))
            throw std::invalid_argument("next_r_combination: [first, r) is not in order");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            RandomAccessIterator j = min_element_greater_than(r, last, *i);
            if (j == last)
                if (i == first)
                {
                    std::sort(first, last);
                    return false;
                }
                else
                    --i;
                else
                {
                    std::swap(*i,* j);
                    for(++i; i < r; i++)
                    {
                        j = min_element_greater_than(r, last, *(i - 1));
                        if (j != last)
                            std::swap(*i,* j);
                    }    // for
                    return true;
                }    // else
        }    // while
    }    // next_r_combination
    
    // next_r_combination -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied
    // binary comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        next_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("next_r_combination: r is not in (first, last]");
        if (!is_sorted(first, r, comp))
            throw std::invalid_argument("next_r_combination: [first, r) is not in order");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            RandomAccessIterator j = min_element_greater_than(r, last, *i, comp);
            if (j == last)
                if (i == first)
                {
                    std::sort(first, last, comp);
                    return false;
                }
                else
                    --i;
                else
                {
                    std::swap(*i, *j);
                    for(++i; i < r; ++i)
                    {
                        j = min_element_greater_than(r, last, *(i - 1), comp);
                        if (j != last)
                            std::swap(*i, *j);
                    }    // for
                    return true;
                }    // else
        }    // while
    }    // next_r_combination
    
    
    // prev_r_combination -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the
    // previous r-combination of elements in lexicographical order.
    // The elements in [first, last) must be in ascending lexicographical
    // order. When the function is called and a prior combination exists,
    // it arranges the previous r-combination in [first, r) and returns 
    // true. If the prior combination does not exist, the function 
    // arranges the sequence [first, last) into the last r-combination,
    // thus restarting the sequence at the end, and returns false.
    
    template<class RandomAccessIterator>
        bool
        prev_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("prev_r_combination: r is not in (first, last]");
        if (!is_sorted(first, r))
            throw std::invalid_argument("prev_r_combination: [first, r) is not in order");

        std::sort(r, last);
        for (RandomAccessIterator i = last - 1; i >= r; --i)
            for (RandomAccessIterator j = first; j < r; ++j)
                if (*i < *j)
                {
                    std::swap(*j, *i);
                    std::sort(++j, last);    // O(n lg n)
                    std::rotate(j, j + (last - r), last); // 2*[n/2]+[m/2]+[(n-m)/2] exchanges
                    return true;
                }    // if
        std::rotate(first, first + (last - r), last);
        return false;
    }    // prev_r_combination
    
    // prev_r_combination -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied
    // binary comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        prev_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        if (last - first <= 1)
            return false;
        if (!(first < r && r <= last))
            throw std::out_of_range("prev_r_combination: r is not in (first, last]");
        if (!is_sorted(first, r, comp))
            throw std::invalid_argument("prev_r_combination: [first, r) is not in order");

        std::sort(r, last, comp);
        for (RandomAccessIterator i = last - 1; i >= r; i--)
            for (RandomAccessIterator j = first; j < r; j++)
                if (comp(*i, *j))
                {
                    std::swap(*j,* i);
                    std::sort(++j, last, comp);    // O(n lg n)
                    std::rotate(j, last - (r - j), last); // 2*[n/2]+[m/2]+[(n-m)/2] exchanges
                    return true;
                }    // if
        std::rotate(first, first + (last - r), last);
        return false;
    }    // prev_r_combination
    
}    // namespace boost

#endif    // BOOST_COMBINATORIAL_HPP
