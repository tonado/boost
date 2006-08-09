//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_DETAIL_POSIX_UTILITY_HPP_02012006
#define BOOST_COROUTINE_DETAIL_POSIX_UTILITY_HPP_02012006
#include <boost/config.hpp>

#if defined(_POSIX_VERSION)
/**
 * Most of these utilities are really pure C++, but they are useful
 * only on posix systems.
 */
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <boost/type_traits.hpp>
/**
 * Stack allocation routines and trampolines for setcontext
 */
namespace boost { namespace coroutines { namespace detail { namespace posix {

  
  //this should be a fine default.
  static const std::size_t stack_alignment = sizeof(void*) > 16? sizeof(void*): 16;

  struct stack_aligner {
    boost::type_with_alignment<stack_alignment>::type dummy;
  };

  /**
   * Stack allocator and deleter functions.
   * Better implementations are possible using
   * mmap (might be required on some systems) and/or
   * using a pooling allocator.
   * NOTE: the SuSv3 documentation explictly allows
   * the use of malloc to allocate stacks for makectx.
   * We use free for guaranteed alignment.
   */
  inline
  void* alloc_stack(std::size_t size) {
    return new stack_aligner[size/sizeof(stack_aligner)];
  }

  inline
  void free_stack(void* stack, std::size_t size) {
    delete [] static_cast<stack_aligner*>(stack);
  }

  /**
   * The splitter is needed for 64 bit systems. 
   * @note The current implementation does NOT use
   * (for debug reasons).
   * Thus it is not 64 bit clean.
   * Use it for 64 bits systems.
   */
  template<typename T>
  union splitter {
    int int_[2];
    T* ptr;
    splitter(int first, int second) {
      int_[0] = first;
      int_[1] = second;
    }

    int first() {
      return int_[0];
    }

    int second() {
      return int_[1];
    }

    splitter(T* ptr) :ptr(ptr) {}

    void operator()() {
      (*ptr)();
    }
  };

  template<typename T>
  inline
  void
  trampoline_split(int first, int second) {
    splitter<T> split(first, second);
    split();
  }

  template<typename T>
  inline
  void
  trampoline(T * fun) {
    (*fun)();
  }
}
} } }

#if defined(_POSIX_MAPPED_FILES) && _POSIX_MAPPED_FILES > 0
#include <sys/mman.h>
namespace boost { namespace coroutines { namespace detail { namespace posix {
  inline 
  void * 
  alloc_stack_mmap(std::size_t size) {
    void * stack = ::mmap(NULL,
			  size,
			  PROT_EXEC|PROT_READ|PROT_WRITE,
			  MAP_PRIVATE|MAP_ANONYMOUS,
			  -1,
			  0
			  );
    if(stack == MAP_FAILED) {
      std::cerr <<strerror(errno)<<"\n";
      abort();
    }
    return stack;
  }

  inline
  void free_stack_mmap(void* stack, std::size_t size) {
    ::munmap(stack, size);
  }
} } } }
#endif
#else
#error This header can only be included when compiling for posix systems.
#endif

#endif
