#ifndef _DATE_TIME_DATE_PARSING_HPP___
#define _DATE_TIME_DATE_PARSING_HPP___

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */

#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/date_time/compiler_config.hpp"
#include "boost/date_time/parse_format_base.hpp"
#include <string>
#include <iterator>
#include <algorithm>

#if defined(BOOST_NO_STD_LOCALE)
#include <cctype> // ::tolower(int)
#else
#include <locale> // std::tolower(char, locale)
#endif

namespace boost {

namespace date_time {

  //! A function to replace the std::transform( , , ,tolower) construct
  /*! This function simply takes a string, and changes all the characters
   * in that string to lowercase (according to the default system locale).
   * In the event that a compiler does not support locales, the old 
   * C style tolower() is used.
   */
  inline
  std::string convert_to_lower(const std::string& inp)
  {
    std::string tmp;
    unsigned i = 0;
#if defined(BOOST_NO_STD_LOCALE)
    while(i < inp.length())
    {
      tmp += static_cast<char>(tolower(inp.at(i++)));
#else
    std::locale loc("");
    while(i < inp.length())
    {
      // tolower and others were brought in to std for borland >= v564
      // in compiler_config.hpp
      std::string::value_type c(inp.at(i++));
      tmp += std::tolower(c, loc);
#endif
    }
    return tmp;
  }

  //! Helper function for parse_date.
  /* Used by-value parameter because we change the string and may 
   * want to preserve the original argument */
  template<class month_type> 
  unsigned short month_str_to_ushort(std::string s)
  {
    if((s.at(0) >= '0') && (s.at(0) <= '9'))
    {
      return boost::lexical_cast<unsigned short>(s);
    }
    else
    {
      s = convert_to_lower(s);
      typename month_type::month_map_ptr_type ptr = month_type::get_month_map_ptr();
      typename month_type::month_map_type::iterator iter = ptr->find(s);
      if(iter != ptr->end()) // required for STLport
      {
	return iter->second;
      }
    }
    return 13; // intentionally out of range - name not found
  }

  //! Generic function to parse a delimited date (eg: 2002-02-10)
  /*! Accepted formats are: "2003-02-10" or " 2003-Feb-10" or
   * "2003-Feburary-10" 
   * The order in which the Month, Day, & Year appear in the argument 
   * string can be accomodated by passing in the appropriate ymd_order_spec */
  template<class date_type>
  date_type
  parse_date(const std::string& s, int order_spec = ymd_order_iso)
  {
    std::string spec_str("");
    if(order_spec == ymd_order_iso)
    {
      spec_str = "ymd";
    }
    else if(order_spec == ymd_order_dmy)
    {
      spec_str = "dmy";
    }
    else{ // (order_spec == ymd_order_us)
      spec_str = "mdy";
    }
    
    typedef typename date_type::year_type year_type;
    typedef typename date_type::month_type month_type;
    unsigned pos = 0;
    typename date_type::ymd_type ymd(year_type::min(),1,1);
    boost::tokenizer<boost::char_delimiters_separator<char> > tok(s);
    for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end(), pos < spec_str.size(); ++beg, ++pos)
    {
      unsigned short i =0; 
      switch(spec_str.at(pos)) {
        case 'y':
	{
          i = boost::lexical_cast<unsigned short>(*beg);
	  ymd.year = i; 
	  break;
	}
        case 'm':
	{
	  ymd.month = month_str_to_ushort<month_type>(*beg); 
	  break;
	}
        case 'd':
        {
          i = boost::lexical_cast<unsigned short>(*beg);
          ymd.day = i; 
          break;
        }
      }; //switch
    }
    return date_type(ymd);
  }

  //! Generic function to parse undelimited date (eg: 20020201)
  template<class date_type>
  date_type
  parse_undelimited_date(const std::string& s)
  {
    int offsets[] = {4,2,2};
    int pos = 0;
    typedef typename date_type::year_type year_type;
    typename date_type::ymd_type ymd(year_type::min(),1,1);
    boost::offset_separator osf(offsets, offsets+3); 
    boost::tokenizer<boost::offset_separator> tok(s, osf);
    for(boost::tokenizer<boost::offset_separator>::iterator ti=tok.begin(); ti!=tok.end();++ti) {
      unsigned short i = boost::lexical_cast<unsigned short>(*ti);
      //      std::cout << i << std::endl;
      switch(pos) {
      case 0: ymd.year = i; break;
      case 1: ymd.month = i; break;
      case 2: ymd.day = i; break;
      };
      pos++;
    } 
    return date_type(ymd);
  }
  
  
  //! Helper function for 'date gregorian::from_stream()'
  /*! Creates a string from the iterators that reference the 
   * begining & end of a char[] or string. All elements are 
   * used in output string */
  template<class iterator_type>
  inline std::string from_stream_type(iterator_type& beg, iterator_type& end,
      char)
  {
    std::stringstream ss("");
    while(beg != end)
    {
      ss << *beg++;
    }
    return ss.str();
  }
  //! Helper function for 'date gregorian::from_stream()'
  /*! Returns the first string found in the stream referenced by the
   * begining & end iterators */
  template<class iterator_type>
  inline std::string from_stream_type(iterator_type& beg, iterator_type& end,
      std::string)
  {
    return *beg;
  }

  /* I believe the wchar stuff would be best elsewhere, perhaps in 
   * parse_date<>()? In the mean time this gets us started... */
  //! Helper function for 'date gregorian::from_stream()'
  /*! Creates a string from the iterators that reference the 
   * begining & end of a wstring. All elements are 
   * used in output string */
  template<class iterator_type>
  inline std::string from_stream_type(iterator_type& beg, iterator_type& end,
      wchar_t)
  {
    std::stringstream ss("");
    while(beg != end)
    {
      ss << ss.narrow(*beg++, 'X'); // 'X' will cause exception to be thrown
    }
    return ss.str();
  }
#ifndef BOOST_NO_STD_WSTRING
  //! Helper function for 'date gregorian::from_stream()'
  /*! Creates a string from the first wstring found in the stream 
   * referenced by the begining & end iterators */
  template<class iterator_type>
  inline std::string from_stream_type(iterator_type& beg, iterator_type& end,
      std::wstring)
  {
    std::wstring ws = *beg;
    std::stringstream ss("");
    std::wstring::iterator wsb = ws.begin(), wse = ws.end();
    while(wsb != wse)
    {
      ss << ss.narrow(*wsb++, 'X'); // 'X' will cause exception to be thrown
    }
    return ss.str();
  }
#endif // BOOST_NO_STD_WSTRING

} } //namespace date_time




#endif

