/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_ASSIGN_FIXED_SIZE_ASSIGNER_HPP
#define BOOST_ASSIGN_FIXED_SIZE_ASSIGNER_HPP

#include "assign_traits.hpp"
#include "exception.hpp"

namespace boost
{

namespace assign
{
    //
    // @note: An iterator-based approach seems appropriate
    //        when we need to initialize a fixed sized container
    //        because the storage underneath will often be an array.
    //

    typedef int dummy_overload;
    template< typename I, typename V >
    inline void insert( I& i, const V& v, dummy_overload )
    {
	*i = v;
    }
    

    
    namespace detail
    {
	
	template< typename V, typename I >
	class fixed_size_assigner
	{
	public:
	    typedef V  value_type;
	    typedef I  iterator;

	    fixed_size_assigner( const iterator& begin, const iterator& end,
				 const value_type& v ) 
		: iter_( begin ), end_( end ), copied_once_( false ) 
	    {
		insert_( v );
	    }
	    


	    fixed_size_assigner( const fixed_size_assigner& r ) 
		: iter_( r.iter_ ), end_( r.end_ ), copied_once_( true )
	    {
	    }



	    ~fixed_size_assigner()
	    {
		if ( iter_ != end_ )
		    if ( copied_once_ && !std::uncaught_exception() )
			throw init_exception( " Exception: container"
					      " initialized with too"
					      " few elements! " );
	    }


	    
	    fixed_size_assigner& operator,( const value_type& v )
	    {
		insert_( v );
		return *this;
	    }

	private:
	    
	    fixed_size_assigner& operator=( const fixed_size_assigner& );
	    
	    void insert_( const value_type& v )
	    {
		if ( iter_ == end_ )
		    throw init_exception( " Exception: trying to write"
					  " past the end of the container! " );
		init::insert( iter_, v, dummy_overload() );
		++iter_;		
	    }

	    iterator       iter_;
	    const iterator end_;
	    bool           copied_once_;
	};

    } // namespace 'detail'

} // namespace 'assign'

} // namespace 'boost'

#endif
