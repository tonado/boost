//  tab_check header  --------------------------------------------------------//

//  Copyright Beman Dawes 2002.
//  See accompanying license for terms and conditions of use.

#ifndef BOOST_TAB_CHECK_HPP
#define BOOST_TAB_CHECK_HPP

#include "inspector.hpp"

namespace boost
{
  namespace inspect
  {
    class tab_check : public inspector
    {
      long m_files_with_errors;
    public:

      tab_check();
      virtual const char * name() const { return "tab-check"; }
      virtual const char * desc() const { return "tabs in file"; }

      virtual void inspect(
        const std::string & library_name,
        const path & full_path,
        const std::string & contents );

      virtual ~tab_check()
        { std::cout << "  " << m_files_with_errors << " files with tabs\n"; }
    };
  }
}

#endif // BOOST_TAB_CHECK_HPP
