// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//


#ifndef BOOST_ASSIGN_LIST_OF_HPP
#define BOOST_ASSIGN_LIST_OF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/assignment_exception.hpp>
#include <boost/config.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/mpl/if.hpp>
#include <deque>
#include <cstddef>
#include <utility>

#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
// BCB requires full type definition for is_array<> to work correctly.
#include <boost/array.hpp>
#endif

namespace boost
{

// this here is necessary to avoid compiler error in <boost/array.hpp>
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    template< class T, std::size_t sz >
    class array;
#endif    
    
namespace assign_detail
{
    /////////////////////////////////////////////////////////////////////////
    // Part 0: common conversion code
    /////////////////////////////////////////////////////////////////////////

    template< class T, std::size_t sz >
    type_traits::yes_type is_array( const array<T,sz>* );
    type_traits::no_type is_array( ... );
    template< class T, class U >
    type_traits::yes_type is_pair( const std::pair<T,U>* );
    type_traits::no_type is_pair( ... ); 


    
    struct array_type_tag
    {
    #if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    private:
      char dummy_;  // BCB would by default use 8 bytes
    #endif
    };
    struct adapter_type_tag
    {
    #if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    private:
      char dummy_;  // BCB would by default use 8 bytes
    #endif
    };
    struct pair_type_tag
    {
    #if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    private:
      char dummy_;  // BCB would by default use 8 bytes
    #endif
    };
    struct default_type_tag
    {
    #if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    private:
      char dummy_;  // BCB would by default use 8 bytes
    #endif
    };


    template< class T >
    class generic_list;
    
    template< class Derived >
    class converter
    {
    public:

        template< class Container >
        operator Container() const
        {
            static Container* c = 0;
            BOOST_STATIC_CONSTANT( bool, is_array_flag = sizeof( assign_detail::is_array( c ) ) 
                                   == sizeof( type_traits::yes_type ) );

            typedef BOOST_DEDUCED_TYPENAME mpl::if_c< is_array_flag,
                                                      array_type_tag,
                                             default_type_tag >::type tag_type;

            return convert( c, tag_type() );
        }

        template< class Container >
        Container convert( const Container*, default_type_tag ) const
        {

#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
// old Dinkumware doesn't support iterator type as template
            Container result;
            BOOST_DEDUCED_TYPENAME Derived::iterator 
                it  = static_cast<const Derived*>(this)->begin(); 
                end = static_cast<const Derived*>(this)->end();
            while( it != end )
            {
                result.insert( result.end(), *it );
                ++it;
            }
            return result;
#else
            return Container( static_cast<const Derived*>(this)->begin(),
                              static_cast<const Derived*>(this)->end() );
#endif
        }

        template< class Array >
        Array convert( const Array*, array_type_tag ) const
        {
            typedef BOOST_DEDUCED_TYPENAME Array::value_type value_type;
            Array array;
            const std::size_t sz = array.size();
            if( sz < static_cast<const Derived*>(this)->size() )
                throw assign::assignment_exception( "array initialized with too many elements" );
            std::size_t n = 0; 
            BOOST_DEDUCED_TYPENAME Derived::iterator 
                i   = static_cast<const Derived*>(this)->begin(), 
                end = static_cast<const Derived*>(this)->end();
            for( ; i != end; ++i, ++n )
                array[n] = *i;
            for( ; n < sz; ++n )
                array[n] = value_type();
            return array; 
        }

        template< class Adapter >
        Adapter convert_to_adapter( const Adapter* = 0 ) const
        {
            Adapter a;
            BOOST_DEDUCED_TYPENAME Derived::iterator 
                i = static_cast<const Derived*>(this)->begin(), 
                end = static_cast<const Derived*>(this)->end();
            for( ; i != end; ++i )
                a.push( *i );
            return a;
        }

    private:
        struct adapter_converter;
        friend struct adapter_converter;

        struct adapter_converter
        {
            const converter& gl;
            adapter_converter( const converter& this_ ) : gl( this_ )
            {}

            adapter_converter( const adapter_converter& r ) 
            : gl( r.gl )
            { }

            template< class Adapter >
            operator Adapter() const
            {
                return gl.convert_to_adapter<Adapter>();
            }
        };

    public: 
        template< class Container >
        Container to_container( Container& c ) const
        {
            return convert( &c, default_type_tag() ); 
        }

        adapter_converter to_adapter() const
        {
            return adapter_converter( *this );
        }

        template< class Adapter >
        Adapter to_adapter( Adapter& a ) const
        {
            return this->convert_to_adapter( &a ); 
        }

        template< class Array >
        Array to_array( Array& a ) const
        {
            return convert( &a, array_type_tag() );
        }
    };
    
    /////////////////////////////////////////////////////////////////////////
    // Part 1: flexible, but inefficient interface
    /////////////////////////////////////////////////////////////////////////    

    template< class T > 
    class generic_list : public converter< generic_list<T> >
    {
        typedef std::deque<T>  impl_type;
        mutable impl_type      values_;
        
    public:
        typedef BOOST_DEDUCED_TYPENAME impl_type::iterator         iterator;
        typedef BOOST_DEDUCED_TYPENAME impl_type::const_iterator   const_iterator;
        typedef BOOST_DEDUCED_TYPENAME impl_type::value_type       value_type;
        typedef BOOST_DEDUCED_TYPENAME impl_type::size_type        size_type;
        typedef BOOST_DEDUCED_TYPENAME impl_type::difference_type  difference_type;
        
    public:
        iterator begin() const       { return values_.begin(); }
        iterator end() const         { return values_.end(); }
        bool empty() const           { return values_.empty(); }
        size_type size() const       { return values_.size(); }
        
    private:
        void push_back( value_type r ) { values_.push_back( r ); }
        
    public:
        generic_list& operator()()
        {
            this->push_back( T() );
            return *this;
        }
        
        template< class U >
        generic_list& operator()( U u )
        {
            this->push_back( u );
            return *this;
        }
        
       
#ifndef BOOST_ASSIGN_MAX_PARAMS // use user's value
#define BOOST_ASSIGN_MAX_PARAMS 5
#endif        
#define BOOST_ASSIGN_MAX_PARAMETERS (BOOST_ASSIGN_MAX_PARAMS - 1) 
#define BOOST_ASSIGN_PARAMS1(n) BOOST_PP_ENUM_PARAMS(n, class U)
#define BOOST_ASSIGN_PARAMS2(n) BOOST_PP_ENUM_BINARY_PARAMS(n, U, u)
#define BOOST_ASSIGN_PARAMS3(n) BOOST_PP_ENUM_PARAMS(n, u)

#define BOOST_PP_LOCAL_LIMITS (1, BOOST_ASSIGN_MAX_PARAMETERS)
#define BOOST_PP_LOCAL_MACRO(n) \
    template< class U, BOOST_ASSIGN_PARAMS1(n) > \
    generic_list& operator()(U u, BOOST_ASSIGN_PARAMS2(n) ) \
    { \
        this->push_back(T(u, BOOST_ASSIGN_PARAMS3(n))); \
        return *this; \
    } \
    /**/
        
#include BOOST_PP_LOCAL_ITERATE()

        
        template< class U >
        generic_list& repeat( std::size_t sz, U u )
        {
            std::size_t i = 0;
            while( i++ != sz )
                this->push_back( u );
            return *this;
        }
        
        template< class Nullary_function >
        generic_list& repeat_fun( std::size_t sz, Nullary_function fun )
        {
            std::size_t i = 0;
            while( i++ != sz )
                this->push_back( fun() );
            return *this;
        }
    };

    /////////////////////////////////////////////////////////////////////////
    // Part 2: efficient, but inconvenient interface
    /////////////////////////////////////////////////////////////////////////

    template< class T >
    struct assign_reference
    {
        assign_reference()
        { }

        assign_reference( T& r ) : ref_(&r)
        { }

        void operator=( const assign_reference r )
        {
            ref_ = r.ref_;
        }

        operator T&() const
        {
            return *ref_;
        }

        void swap( assign_reference& r )
        {
            std::swap( *ref_, *r.ref_ );
        }

        friend inline bool operator<( const assign_reference& l, 
                                      const assign_reference& r )
        {
            return *l.ref_ < *r.ref_;
        }
        
        friend inline bool operator>( const assign_reference& l,
                                      const assign_reference& r )
        {
            return *l.ref_ > *r.ref_;
        }

        friend inline void swap( assign_reference& l, 
                                 assign_reference& r )
        {
            l.swap( r );
        }
        
    private:
        T* ref_;
    };

    template< class T, int N >
    struct static_generic_list : public converter< static_generic_list<T,N> >
    {
        typedef BOOST_DEDUCED_TYPENAME 
            remove_reference<T>::type                 internal_value_type; 
        typedef assign_reference<internal_value_type> value_type;
        typedef value_type*                           iterator;
        typedef const value_type*                     const_iterator;
        typedef std::size_t                           size_type;
        typedef std::ptrdiff_t                        difference_type;

    
        static_generic_list( T r ) :
            current_(1)
        {
            refs_[0] = r;
        }

        static_generic_list& operator()( T r )
        {
            refs_[current_] = r;
            ++current_;
            return *this;
        }

        iterator begin() const 
        {
            return &refs_[0];
        }

        iterator end() const
        {
            return &refs_[current_];
        }

        size_type size() const
        {
            return static_cast<size_type>( current_ ); 
        }

        bool empty() const
        {
            return false;
        }
        
    private:
        static_generic_list();
        
        mutable assign_reference<internal_value_type> refs_[N];
        int current_;
    };

} // namespace 'assign_detail'

namespace assign
{
    template< class T >
    inline assign_detail::generic_list<T>
    list_of()
    {
        return assign_detail::generic_list<T>();
    }
    
    template< class T >
    inline assign_detail::generic_list<T> 
    list_of( T t )
    {
        return assign_detail::generic_list<T>()( t );
    }

    template< int N, class T >
    inline assign_detail::static_generic_list<T,N>
    list_of( T t )
    {
        // This version is on for reference arguments
        //BOOST_STATIC_ASSERT(( boost::is_reference<T>::value && "only reference values allowed" ));
        return assign_detail::static_generic_list<T,N>( t );
    }
    
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_ASSIGN_MAX_PARAMETERS)
#define BOOST_PP_LOCAL_MACRO(n) \
    template< class T, class U, BOOST_ASSIGN_PARAMS1(n) > \
    inline assign_detail::generic_list<T> \
    list_of(U u, BOOST_ASSIGN_PARAMS2(n) ) \
    { \
        return assign_detail::generic_list<T>()(u, BOOST_ASSIGN_PARAMS3(n)); \
    } \
    /**/
    
#include BOOST_PP_LOCAL_ITERATE()


    template< class Key, class T >
    inline assign_detail::generic_list< std::pair<Key,T> >
    map_list_of( Key k, T t )
    {
        return assign_detail::generic_list< std::pair<Key,T> >()( k, t );
    }

    template< class F, class S >
    inline assign_detail::generic_list< std::pair<F,S> >
    pair_list_of( F f, S s )
    {
        return assign_detail::generic_list< std::pair<F,S> >()( f, s );
    }


} // namespace 'assign'
} // namespace 'boost'


#undef BOOST_ASSIGN_MAX_PARAMS         
#undef BOOST_ASSIGN_MAX_PARAMETERS         
#undef BOOST_ASSIGN_PARAMS1
#undef BOOST_ASSIGN_PARAMS2
#undef BOOST_ASSIGN_PARAMS3


#endif
