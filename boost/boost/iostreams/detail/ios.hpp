// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef BOOST_IOSTREAMS_DETAIL_IOS_HPP_INCLUDED
#define BOOST_IOSTREAMS_DETAIL_IOS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              
                 
#include <boost/config.hpp> // BOOST_MSVC.
#include <boost/detail/workaround.hpp>
#include <boost/iostreams/detail/config/wide_streams.hpp>
#ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES
# if !BOOST_WORKAROUND(__MWERKS__, <= 0x3003)
#  include <ios>
# else
#  include <istream>
#  include <ostream>
# endif
#else 
# include <exception>
# include <iosfwd>
#endif 

namespace boost { namespace iostreams { namespace detail {

#ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //--------------------------------//
# if !BOOST_WORKAROUND(__MWERKS__, <= 0x3003) && \
     !BOOST_WORKAROUND(__BORLANDC__, < 0x600) && \
     !BOOST_WORKAROUND(BOOST_MSVC, < 1300) \
     /**/

#define BOOST_IOS                std::ios
#define BOOST_IOSTREAMS_FAILURE  std::ios::failure
typedef std::ios_base::failure   failure;

# else

#define BOOST_IOS                std::ios_base
#define BOOST_IOSTREAMS_FAILURE  std::ios_base::failure
typedef std::ios_base::failure   failure;

# endif
#else // #ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //-----------------------//

#define BOOST_IOS                std::ios
#define BOOST_IOSTREAMS_FAILURE  boost::detail::failure

class failure : std::exception {    
public:
    explicit failure(const std::string& what_arg) : what_(what_arg) { }
    const char* what() const { return what_.c_str(); }
private:
    std::string what_;
};

#endif // #ifndef BOOST_IOSTREAMS_NO_STREAM_TEMPLATES //----------------------//

} } } // End namespace failure, iostreams, boost.

#endif // #ifndef BOOST_IOSTREAMS_DETAIL_IOS_HPP_INCLUDED
