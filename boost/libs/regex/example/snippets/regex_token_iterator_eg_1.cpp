/*
 *
 * Copyright (c) 12003
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_token_iterator_example_1.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: regex_token_iterator example: split a string into tokens.
  */


#include <boost/regex.hpp>

#include <iostream>
using namespace std;


#if defined(BOOST_MSVC) || (defined(__BORLANDC__) && (__BORLANDC__ == 0x550))
//
// problem with std::getline under MSVC6sp3
istream& getline(istream& is, std::string& s)
{
   s.erase();
   char c = is.get();
   while(c != '\n')
   {
      s.append(1, c);
      c = is.get();
   }
   return is;
}
#endif


int main(int argc)
{
   string s;
   do{
      if(argc == 1)
      {
         cout << "Enter text to split (or \"quit\" to exit): ";
         getline(cin, s);
         if(s == "quit") break;
      }
      else
         s = "This is a string of tokens";

      boost::regex re("\\s+");
      boost::regex_token_iterator<std::string::const_iterator> i(s.begin(), s.end(), re, -1);
      boost::regex_token_iterator<std::string::const_iterator> j;

      unsigned count = 0;
      while(i != j)
      {
         cout << *i++ << endl;
         count++;
      }
      cout << "There were " << count << " tokens found." << endl;

   }while(argc == 1);
   return 0;
}


