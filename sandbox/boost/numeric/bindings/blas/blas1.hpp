//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen and Kresimir Fresl

#ifndef BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_HPP
#define BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_HPP

#include <boost/numeric/bindings/blas/blas1_overloads.hpp>
#include <boost/numeric/bindings/traits/vector_traits.hpp>
#include <boost/type_traits/is_same.hpp>
#include <cassert> 

namespace boost { namespace numeric { namespace bindings { namespace blas {

  // x <- alpha * x
  template < typename value_type, typename vector_type >
  void scal(const value_type &alpha, vector_type &x )
  {
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
    BOOST_STATIC_ASSERT( ( boost::is_same< value_type, typename traits::vector_traits<vector_type>::value_type >::value ) ) ;
#else
    BOOST_STATIC_ASSERT( ( boost::is_same< value_type, typename vector_type::value_type >::value ) ) ;
#endif

    const int n =  traits::vector_size( x ) ;
    const int stride = traits::vector_stride( x ) ;
    value_type *x_ptr = traits::vector_storage( x ) ;

    detail::scal( n, alpha, x_ptr, stride ) ;
  }


  // y <- alpha * x + y
  template < typename value_type, typename vector_type_x, typename vector_type_y >
  void axpy(const value_type& alpha, const vector_type_x &x, vector_type_y &y )
  { 
#ifdef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
    BOOST_STATIC_ASSERT( ( is_same< value_type, typename vector_type_x::value_type >::value ) ) ;
    BOOST_STATIC_ASSERT( ( is_same< value_type, typename vector_type_y::value_type >::value ) ) ;
#else
    BOOST_STATIC_ASSERT( ( is_same< value_type, typename traits::vector_traits< vector_type_x >::value_type >::value ) ) ;
    BOOST_STATIC_ASSERT( ( is_same< value_type, typename traits::vector_traits< vector_type_y >::value_type >::value ) ) ;
#endif
    assert( traits::vector_size( x ) == traits::vector_size( y ) ) ;

    const int n = traits::vector_size( x ) ;
    const int stride_x = traits::vector_stride( x ) ;
    const int stride_y = traits::vector_stride( y ) ;
    const value_type *x_ptr = traits::vector_storage( x ) ;
    value_type *y_ptr = traits::vector_storage( y ) ;

    detail::axpy( n, alpha, x_ptr, stride_x, y_ptr, stride_y ) ; 
  }


  // dot <- x^T * y  (real vectors)
  template < typename vector_type_x, typename vector_type_y >
  typename traits::vector_traits< vector_type_x >::value_type 
  dot(const vector_type_x &x, const vector_type_y &y)
  {
#ifdef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
    BOOST_STATIC_ASSERT( ( is_same< typename vector_type_y::value_type, typename vector_type_x::value_type >::value ) ) ;
#else
    BOOST_STATIC_ASSERT( ( is_same< typename traits::vector_traits< vector_type_y >::value_type, typename traits::vector_traits< vector_type_x >::value_type >::value ) ) ;
#endif

    assert( traits::vector_size( x ) == traits::vector_size( y ) ) ;

    typedef typename vector_type_x::value_type value_type ;

    const int n = traits::vector_size( x ) ;
    const int stride_x = traits::vector_stride( x ) ;
    const int stride_y = traits::vector_stride( y ) ;
    const value_type *x_ptr = traits::vector_storage( x ) ;
    const value_type *y_ptr = traits::vector_storage( y ) ;

    return detail::dot( n, x_ptr, stride_x, y_ptr, stride_y ) ;
  }

  // dotu <- x^T * y  (complex vectors)
  template < typename vector_type_x, typename vector_type_y >
  typename traits::vector_traits< vector_type_x >::value_type 
  dotu(const vector_type_x &x, const vector_type_y &y)
  {
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
    BOOST_STATIC_ASSERT( ( is_same< typename traits::vector_traits< vector_type_y >::value_type, typename traits::vector_traits< vector_type_x >::value_type >::value ) ) ;
#else
    BOOST_STATIC_ASSERT( ( is_same< typename vector_type_y::value_type, typename vector_type_x::value_type >::value ) ) ;
#endif
    assert( traits::vector_size( x ) == traits::vector_size( y ) ) ;

    typedef typename vector_type_x::value_type value_type ;

    const int n = traits::vector_size( x ) ;
    const int stride_x = traits::vector_stride( x ) ;
    const int stride_y = traits::vector_stride( y ) ;
    const value_type *x_ptr = traits::vector_storage( x ) ;
    const value_type *y_ptr = traits::vector_storage( y ) ;
    
    value_type ret ;
    detail::dotu( ret, n, x_ptr, stride_x, y_ptr, stride_y ) ;
    return ret;
  }

  // dotc <- x^H * y  (complex vectors) 
  template < typename vector_type_x, typename vector_type_y >
  typename traits::vector_traits< vector_type_x >::value_type 
  dotc(const vector_type_x &x, const vector_type_y &y)
  {
#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS
    BOOST_STATIC_ASSERT( ( is_same< typename traits::vector_traits< vector_type_y >::value_type, typename traits::vector_traits< vector_type_x >::value_type >::value ) ) ;
#else
    BOOST_STATIC_ASSERT( ( is_same< typename vector_type_y::value_type, typename vector_type_x::value_type >::value ) ) ;
#endif
    assert( traits::vector_size( x ) == traits::vector_size( y ) ) ;

    typedef typename vector_type_x::value_type value_type ;

    const int n = traits::vector_size( x ) ;
    const int stride_x = traits::vector_stride( x ) ;
    const int stride_y = traits::vector_stride( y ) ;
    const value_type *x_ptr = traits::vector_storage( x ) ;
    const value_type *y_ptr = traits::vector_storage( y ) ;
    
    value_type ret ;
    detail::dotc( ret, n, x_ptr, stride_x, y_ptr, stride_y ) ;
    return ret;
  }


  /*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // nrm2 <- ||x||_2
  // .. for now works only with real vectors
  template < typename vector_type >
  typename traits::vector_traits< vector_type >::value_type 
  nrm2(const vector_type &x) 
  {
    const int n = vector_size( x ) ;
    const int stride_x = vector_stride( x ) ;
    const value_type *x_ptr = vector_storage( x ) ;

    return detail::nrm2( n, x_ptr, stride_x ) ;
  }


  // asum <- ||x||_1
  // .. for now works only with real vectors
  template < typename vector_type >
  typename traits::vector_traits< vector_type >::value_type 
  asum(const vector_type &x) 
  {
    typedef traits::vector_traits< const vector_type > vtraits ; 
    typedef typename vtraits::value_type  value_type ;
    typedef typename traits::value_traits< value_type >::value_type bind_type ;

    const int n = vtraits::size( x ) ;
    const int stride_x = vtraits::stride( x ) ;
    const value_type *x_ptr = vtraits::storage( x ) ;

    return 
      blas< value_type >::asum( &n, (const bind_type*)x_ptr, &stride_x ) ;
  }
  */

}}}}

#endif // BOOST_NUMERIC_BINDINGS_BLAS_BLAS1_HPP
