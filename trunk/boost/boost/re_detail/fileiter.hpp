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
  *   FILE         fileiter.hpp
  *   VERSION      3.02
  *   DESCRIPTION: Declares various platform independent file and
  *                directory iterators, plus binary file input in
  *                the form of class map_file.
  */

#ifndef BOOST_RE_FILEITER_HPP
#define BOOST_RE_FILEITER_HPP

#include <boost/re_detail/regex_config.hpp>

#if (defined(__CYGWIN__) || defined(__CYGWIN32__)) && !defined(BOOST_RE_NO_W32)
#error "Sorry, can't mix <windows.h> with STL code and gcc compiler: if you ran configure, try again with configure --disable-ms-windows"
#define FI_WIN32_MAP
#define FI_POSIX_DIR
#elif (defined(__WIN32__) || defined(_WIN32) || defined(WIN32)) && !defined(BOOST_RE_NO_W32)
#define FI_WIN32_MAP
#define FI_WIN32_DIR
#else
#define FI_POSIX_MAP
#define FI_POSIX_DIR
#endif

#if defined(FI_WIN32_MAP)||defined(FI_WIN32_DIR)
#include <windows.h>
#endif

#if defined(FI_WIN32_DIR)

namespace boost{
   namespace re_detail{

typedef WIN32_FIND_DATA _fi_find_data;
typedef HANDLE _fi_find_handle;

   } // namespace re_detail

} // namespace boost

#define _fi_invalid_handle INVALID_HANDLE_VALUE
#define _fi_dir FILE_ATTRIBUTE_DIRECTORY

#elif defined(FI_POSIX_DIR)

#include <cstdio>
#include <cctype>
#include <iterator>
#include <list>
#include <cassert>
#include <dirent.h>

#if defined(__SUNPRO_CC)
using std::list;
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

namespace boost{
   namespace re_detail{

#ifdef __BORLANDC__
   #if __BORLANDC__ == 0x530
    #pragma option push -a4 -b -Ve
   #elif __BORLANDC__ > 0x530
    #pragma option push -a8 -b -Ve
   #endif
#endif

struct _fi_find_data
{
   unsigned dwFileAttributes;
   char cFileName[MAX_PATH];
};

struct _fi_priv_data;

typedef _fi_priv_data* _fi_find_handle;
#define _fi_invalid_handle NULL
#define _fi_dir 1

_fi_find_handle _fi_FindFirstFile(const char* lpFileName, _fi_find_data* lpFindFileData);
bool _fi_FindNextFile(_fi_find_handle hFindFile,   _fi_find_data* lpFindFileData);
bool _fi_FindClose(_fi_find_handle hFindFile);

#ifdef __BORLANDC__
 #if __BORLANDC__ > 0x520
  #pragma option pop
 #endif
#endif

   } // namespace re_detail
} // namespace boost

#ifdef FindFirstFile
 #undef FindFirstFile
#endif
#ifdef FindNextFile
 #undef FindNextFile
#endif
#ifdef FindClose
 #undef FindClose
#endif

#define FindFirstFile _fi_FindFirstFile
#define FindNextFile _fi_FindNextFile
#define FindClose _fi_FindClose

#endif

namespace boost{
   namespace re_detail{

#ifdef __BORLANDC__
   #if __BORLANDC__ == 0x530
    #pragma option push -a4 -b
   #elif __BORLANDC__ > 0x530
    #pragma option push -a8 -b
   #endif
#endif

#ifdef FI_WIN32_MAP // win32 mapfile

class BOOST_RE_IX_DECL mapfile
{
   HANDLE hfile;
   HANDLE hmap;
   const char* _first;
   const char* _last;
public:

   typedef const char* iterator;

   mapfile(){ hfile = hmap = 0; _first = _last = 0; }
   mapfile(const char* file){ hfile = hmap = 0; _first = _last = 0; open(file); }
   ~mapfile(){ close(); }
   void open(const char* file);
   void close();
   const char* begin(){ return _first; }
   const char* end(){ return _last; }
   size_t size(){ return _last - _first; }
   bool valid(){ return (hfile != 0) && (hfile != INVALID_HANDLE_VALUE); }
};


#elif !defined(BOOST_RE_NO_STL)  // use C API to emulate the memory map:

class BOOST_RE_IX_DECL mapfile_iterator;

class BOOST_RE_IX_DECL mapfile
{
   typedef char* pointer;
   std::FILE* hfile;
   long int _size;
   pointer* _first;
   pointer* _last;
   mutable std::list<pointer*> condemed;
   enum sizes
   {
      buf_size = 4096
   };
   void lock(pointer* node)const;
   void unlock(pointer* node)const;
public:

   typedef mapfile_iterator iterator;

   mapfile(){ hfile = 0; _size = 0; _first = _last = 0; }
   mapfile(const char* file){ hfile = 0; _size = 0; _first = _last = 0; open(file); }
   ~mapfile(){ close(); }
   void open(const char* file);
   void close();
   iterator begin()const;
   iterator end()const;
   unsigned long size()const{ return _size; }
   bool valid()const{ return hfile != 0; }
   friend class mapfile_iterator;
};

class BOOST_RE_IX_DECL mapfile_iterator : public BOOST_RE_RA_ITERATOR(char, long)
{
   typedef mapfile::pointer internal_pointer;
   internal_pointer* node;
   const mapfile* file;
   unsigned long offset;
   long position()const
   {
      return file ? ((node - file->_first) * mapfile::buf_size + offset) : 0;
   }
   void position(long pos)
   {
      if(file)
      {
         node = file->_first + (pos / mapfile::buf_size);
         offset = pos % mapfile::buf_size;
      }
   }
public:
   typedef std::ptrdiff_t                  difference_type;
   typedef char                            value_type;
   typedef const char*                     pointer;
   typedef const char&                     reference;
   typedef std::random_access_iterator_tag iterator_category;

   mapfile_iterator() { node = 0; file = 0; offset = 0; }
   mapfile_iterator(const mapfile* f, long position)
   {
      file = f;
      node = f->_first + position / mapfile::buf_size;
      offset = position % mapfile::buf_size;
      if(file)
         file->lock(node);
   }
   mapfile_iterator(const mapfile_iterator& i)
   {
      file = i.file;
      node = i.node;
      offset = i.offset;
      if(file)
         file->lock(node);
   }
   ~mapfile_iterator()
   {
      if(file && node)
         file->unlock(node);
   }
   mapfile_iterator& operator = (const mapfile_iterator& i);
   char operator* ()const
   {
      assert(node >= file->_first);
      assert(node < file->_last);
      return file ? *(*node + sizeof(int) + offset) : char(0);
   }
   char operator[] (long off)const
   {
      mapfile_iterator tmp(*this);
      tmp += off;
      return *tmp;
   }
   mapfile_iterator& operator++ ();
   mapfile_iterator operator++ (int);
   mapfile_iterator& operator-- ();
   mapfile_iterator operator-- (int);

   mapfile_iterator& operator += (long off)
   {
      position(position() + off);
      return *this;
   }
   mapfile_iterator& operator -= (long off)
   {
      position(position() - off);
      return *this;
   }

   friend inline bool operator==(const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return (i.file == j.file) && (i.node == j.node) && (i.offset == j.offset);
   }
#ifndef BOOST_RE_NO_NOT_EQUAL
   friend inline bool operator!=(const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return !(i == j);
   }
#endif
   friend inline bool operator<(const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return i.position() < j.position();
   }
   friend inline bool operator>(const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return i.position() > j.position();
   }
   friend inline bool operator<=(const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return i.position() <= j.position();
   }
   friend inline bool operator>=(const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return i.position() >= j.position();
   }

   friend mapfile_iterator operator + (const mapfile_iterator& i, long off);
   friend mapfile_iterator operator + (long off, const mapfile_iterator& i)
   {
      mapfile_iterator tmp(i);
      return tmp += off;
   }
   friend mapfile_iterator operator - (const mapfile_iterator& i, long off);
   friend inline long operator - (const mapfile_iterator& i, const mapfile_iterator& j)
   {
      return i.position() - j.position();
   }
};

#endif

// _fi_sep determines the directory separator, either '\\' or '/'
BOOST_RE_IX_DECL extern const char* _fi_sep;

struct file_iterator_ref
{
   _fi_find_handle hf;
   _fi_find_data _data;
   long count;
};


class BOOST_RE_IX_DECL file_iterator : public BOOST_RE_INPUT_ITERATOR(const char*, std::ptrdiff_t)
{
   char* _root;
   char* _path;
   char* ptr;
   file_iterator_ref* ref;

public:
   file_iterator();
   file_iterator(const char* wild);
   ~file_iterator();
   file_iterator(const file_iterator&);
   file_iterator& operator=(const file_iterator&);
   const char* root()const { return _root; }
   const char* path()const { return _path; }
   const char* name()const { return ptr; }
   _fi_find_data* data() { return &(ref->_data); }
   void next();
   file_iterator& operator++() { next(); return *this; }
   file_iterator operator++(int);
   const char* operator*() { return path(); }

   friend inline bool operator == (const file_iterator& f1, const file_iterator& f2)
   {
      return ((f1.ref->hf == _fi_invalid_handle) && (f2.ref->hf == _fi_invalid_handle));
   }
#ifndef BOOST_RE_NO_NOT_EQUAL
   friend inline bool operator != (const file_iterator& f1, const file_iterator& f2)
   {
      return !(f1 == f2);
   }
#endif
};

// dwa 9/13/00 - suppress unused parameter warning
inline bool operator < (const file_iterator&, const file_iterator&)
{
   return false;
}


class BOOST_RE_IX_DECL directory_iterator : public BOOST_RE_INPUT_ITERATOR(const char*, std::ptrdiff_t)
{
   char* _root;
   char* _path;
   char* ptr;
   file_iterator_ref* ref;

public:
   directory_iterator();
   directory_iterator(const char* wild);
   ~directory_iterator();
   directory_iterator(const directory_iterator& other);
   directory_iterator& operator=(const directory_iterator& other);

   const char* root()const { return _root; }
   const char* path()const { return _path; }
   const char* name()const { return ptr; }
   _fi_find_data* data() { return &(ref->_data); }
   void next();
   directory_iterator& operator++() { next(); return *this; }
   directory_iterator operator++(int);
   const char* operator*() { return path(); }

   static const char* separator() { return _fi_sep; }

   friend inline bool operator == (const directory_iterator& f1, const directory_iterator& f2)
   {
      return ((f1.ref->hf == _fi_invalid_handle) && (f2.ref->hf == _fi_invalid_handle));
   }

#ifndef BOOST_RE_NO_NOT_EQUAL
   friend inline bool operator != (const directory_iterator& f1, const directory_iterator& f2)
   {
      return !(f1 == f2);
   }
#endif
};

inline bool operator < (const directory_iterator&, const directory_iterator&)
{
   return false;
}

#ifdef __BORLANDC__
 #if __BORLANDC__ > 0x520
  #pragma option pop
 #endif
#endif


} // namespace re_detail
using boost::re_detail::directory_iterator;
using boost::re_detail::file_iterator;
using boost::re_detail::mapfile;
} // namespace boost

#endif     // _FILEITER_H














