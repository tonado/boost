//
//  Copyright (c) 2002-2003
//  Toon Knapen, Kresimir Fresl, Joerg Walter
//
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.
//  It is provided "as is" without express or implied warranty.
//
//

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_SYMM_HERM_RAW_HPP
#define BOOST_NUMERIC_BINDINGS_TRAITS_SYMM_HERM_RAW_HPP

#include <boost/numeric/bindings/traits/matrix_raw.hpp> 
#include <boost/numeric/ublas/symmetric.hpp> 
#include <boost/numeric/ublas/hermitian.hpp> 

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace ublas = boost::numeric::ublas; 

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename M, typename F>
  BOOST_UBLAS_INLINE
  typename M::const_pointer 
  matrix_storage (const ublas::symmetric_adaptor<M, F> &m) {
    return matrix_storage (m.data()); 
  }
#endif
  // We need data_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename M, typename F>
  BOOST_UBLAS_INLINE
  typename M::const_pointer 
  matrix_storage_const (const ublas::symmetric_adaptor<M, F> &m) {
    return matrix_storage_const (m.data()); 
  }
  template <typename M, typename F>
  BOOST_UBLAS_INLINE
  typename M::pointer matrix_storage (ublas::symmetric_adaptor<M, F> &m) {
    return matrix_storage (m.data()); 
  }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  template <typename M, typename F>
  BOOST_UBLAS_INLINE
  typename M::const_pointer 
  matrix_storage (const ublas::hermitian_adaptor<M, F> &m) {
    return matrix_storage (m.data()); 
  }
#endif
  // We need data_const() mostly due to MSVC 6.0.
  // But how shall we write portable code otherwise?
  template <typename M, typename F>
  BOOST_UBLAS_INLINE
  typename M::const_pointer 
  matrix_storage_const (const ublas::hermitian_adaptor<M, F> &m) {
    return matrix_storage_const (m.data()); 
  }
  template <typename M, typename F>
  BOOST_UBLAS_INLINE
  typename M::pointer matrix_storage (ublas::hermitian_adaptor<M, F> &m) {
    return matrix_storage (m.data()); 
  }

  namespace detail {

    inline char m_uplo_tag (ublas::upper_tag const&) { return 'U'; } 
    inline char m_uplo_tag (ublas::lower_tag const&) { return 'L'; } 

  }

  template <typename SymmM> 
  inline 
  char matrix_uplo_tag (SymmM&) {
      typedef typename SymmM::packed_category uplo_t; 
      return detail::m_uplo_tag (uplo_t());
  }
  

}}}}

#endif
