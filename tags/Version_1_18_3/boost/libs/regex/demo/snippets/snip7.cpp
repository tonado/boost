/*
 *
 * Copyright (c) 1998-2000
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         snip7.cpp
  *   VERSION      3.00
  *   DESCRIPTION: regex_grep example 2: searches a cpp file for class definitions,
  *                using a C++ Builder closure as a callback.
  */

#include <string>
#include <map>
#include <boost/regex.hpp>
#include <functional>

// purpose:
// takes the contents of a file in the form of a string
// and searches for all the C++ class definitions, storing
// their locations in a map of strings/int's

typedef std::map<std::string, int, std::less<std::string> > map_type;

class class_index
{
   boost::regex expression;
   map_type index;
   std::string::const_iterator base;
   typedef boost::match_results<std::string::const_iterator, boost::regex::allocator_type> arg_type;

   bool grep_callback(const boost::match_results<std::string::const_iterator, boost::regex::allocator_type>& what);
public:
   map_type& get_map() { return index; }
   typedef bool (__closure* grep_callback_type)(const arg_type&);
   void IndexClasses(const std::string& file);
   class_index()
      : index(),
        expression("^(template[[:space:]]*<[^;:{]+>[[:space:]]*)?(class|struct)[[:space:]]*(\\<\\w+\\>([[:blank:]]*\\([^)]*\\))?[[:space:]]*)*(\\<\\w*\\>)[[:space:]]*(<[^;:{]+>)?[[:space:]]*(\\{|:[^;\\{()]*\\{)")
        {}
};

bool class_index::grep_callback(const boost::match_results<std::string::const_iterator, boost::regex::allocator_type>& what)
{
   // what[0] contains the whole string
   // what[5] contains the class name.
   // what[6] contains the template specialisation if any.
   // add class name and position to map:
   index[std::string(what[5].first, what[5].second) + std::string(what[6].first, what[6].second)] =
               what[5].first - base;
   return true;
}

void class_index::IndexClasses(const std::string& file)
{
   std::string::const_iterator start, end;
   start = file.begin();
   end = file.end();
   base = start;
   class_index::grep_callback_type cl = &(this->grep_callback);
   boost::regex_grep(cl,
            start,
            end,
            expression);
}


#include <fstream>
#include <iostream>

using namespace std;

void load_file(std::string& s, std::istream& is)
{
   s.erase();
   s.reserve(is.rdbuf()->in_avail());
   char c;
   while(is.get(c))
   {
      if(s.capacity() == s.size())
         s.reserve(s.capacity() * 3);
      s.append(1, c);
   }
}

int main(int argc, const char** argv)
{
   std::string text;
   for(int i = 1; i < argc; ++i)
   {
      cout << "Processing file " << argv[i] << endl;
      std::ifstream fs(argv[i]);
      load_file(text, fs);
      class_index i;
      i.IndexClasses(text);
      cout << i.get_map().size() << " matches found" << endl;
      map_type::iterator c, d;
      c = i.get_map().begin();
      d = i.get_map().end();
      while(c != d)
      {
         cout << "class \"" << (*c).first << "\" found at index: " << (*c).second << endl;
         ++c;
      }
   }
   return 0;
}



