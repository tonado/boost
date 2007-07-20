//              -- cgi_service_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__

#include <string>
#include <map>
#include <boost/system/error_code.hpp>

//#include "../detail/extract_params.hpp"
//#include "../role_type.hpp"
//#include "../http/status_code.hpp"
//#include "../map.hpp"
//#include "../request_impl/cgi_request_impl.hpp"
#include "cgi_service_impl_base.hpp"
#include "../detail/extract_params.hpp"
#include "../request_impl/cgi_request_impl.hpp"

namespace cgi {

  //class cgi_request_impl;

  //template<typename RequestImplType>
  class cgi_service_impl
   : public cgi_service_impl_base<cgi_request_impl>
  {
    typedef ::cgi::map         map_type;
  public:
    typedef cgi_request_impl   impl_type;
    //typedef cgi_service_impl_base<cgi_request_impl> base;

    cgi_service_impl()
      : cgi_service_impl_base<cgi_request_impl>()
    {
    }

    template<typename T>
    cgi_service_impl(T&)
      : cgi_service_impl_base<cgi_request_impl>()
    {
    }
  };

} // namespace cgi

#endif // CGI_CGI_SERVICE_IMPL_HPP_INCLUDED__
