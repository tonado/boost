// (C) 2002, Fernando Luis Cacciola Carballal.
//
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
//
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
//
// You are welcome to contact the author at:
//  fernando_cacciola@hotmail.com
//
#ifndef BOOST_UTILITY_INPLACE_FACTORY_25AGO2003_HPP
#define BOOST_UTILITY_INPLACE_FACTORY_25AGO2003_HPP

#include <boost/detail/in_place_factory_prefix.hpp>

#include <boost/type.hpp>

namespace boost {

class InPlaceFactoryBase {} ;

#define BOOST_DEFINE_INPLACE_FACTORY_CLASS(z,n,_) \
template< BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n),class A) > \
class BOOST_PP_CAT(InPlaceFactory, BOOST_PP_INC(n) ) : public InPlaceFactoryBase \
{ \
public: \
\
  BOOST_PP_CAT(InPlaceFactory, BOOST_PP_INC(n) ) ( BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_INC(n),A,const& a) ) \
    : \
    BOOST_PP_ENUM( BOOST_PP_INC(n), BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_INIT, _ ) \
  {} \
\
  template<class T> \
  void apply ( void* address BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(T) ) const \
  { \
    new ( address ) T ( BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(n), m_a ) ) ; \
  } \
\
  BOOST_PP_REPEAT( BOOST_PP_INC(n), BOOST_DEFINE_INPLACE_FACTORY_CLASS_MEMBER_DECL, _) \
} ; \
\
template< BOOST_PP_ENUM_PARAMS(BOOST_PP_INC(n),class A) > \
BOOST_PP_CAT(InPlaceFactory, BOOST_PP_INC(n) ) < BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(n), A ) > \
in_place ( BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_INC(n),A, const& a) ) \
{ \
  return BOOST_PP_CAT(InPlaceFactory, BOOST_PP_INC(n) ) < BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(n), A ) > \
           ( BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(n), a ) ) ; \
} ; \

BOOST_PP_REPEAT( BOOST_MAX_INPLACE_FACTORY_ARITY, BOOST_DEFINE_INPLACE_FACTORY_CLASS, BOOST_PP_EMPTY() )

} // namespace boost

#endif

