/**
 * Pointer Container Library
 * (C) Thorsten Ottosen 2003-2005
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */

#ifndef BOOST_PTR_CONTAINER_SCOPED_DELETER_HPP
#define BOOST_PTR_CONTAINER_SCOPED_DELETER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstddef>
#include <boost/scoped_array.hpp>

namespace boost
{
    namespace ptr_container
    {
        namespace ptr_container_detail
        {
            template< class T, class CloneAllocator >
            class scoped_deleter
            {
                scoped_array<T*>  ptrs_;
                size_t            stored_; 
                bool              released_;
                
            public:
                scoped_deleter( size_t size ) : 
                    ptrs_( new T*[size] ), stored_( 0 ), 
                    released_( false )
                {
                    BOOST_ASSERT( size > 0 );
                }


                
                scoped_deleter( size_t n, const T& x ) // strong
                    : ptrs_( new T*[n] ), stored_(0),
                      released_( false )
                {
                    for( size_t i = 0; i != n; i++ )
                        add( CloneAllocator::allocate_clone( &x ) );
                    BOOST_ASSERT( stored_ > 0 );
                }


                
                template< class InputIterator >
                scoped_deleter ( InputIterator first, InputIterator last  ) // strong
                    : ptrs_( new T*[ std::distance(first,last) ] ),
                      stored_(0),
                      released_( false )
                {
                    for( ; first != last; ++first )
                        add( CloneAllocator::allocate_clone( &*first ) );
                    BOOST_ASSERT( stored_ > 0 );
                }

                
                
                ~scoped_deleter()
                {
                    if ( !released_ )
                    {
                        for( size_t i = 0u; i != stored_; ++i )
                            CloneAllocator::deallocate_clone( ptrs_[i] ); 
                    }
                }
                
                
                
                void add( T* t )
                {
                    BOOST_ASSERT( ptrs_.get() != 0 );
                    ptrs_[stored_] = t;
                    ++stored_;
                }
                
                
                
                void release()
                {
                    released_ = true;
                }
                
                
                
                T** begin()
                {
                    BOOST_ASSERT( ptrs_.get() != 0 );
                    return &ptrs_[0];
                }
                
                
                
                T** end()
                {
                    BOOST_ASSERT( ptrs_.get() != 0 );
                    return &ptrs_[stored_];
                }
                
            }; // class 'scoped_deleter'
        }
    }
}

#endif
