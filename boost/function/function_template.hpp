// Boost.Function library

//  Copyright Douglas Gregor 2001-2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

// Note: this header is a header template and must NOT have multiple-inclusion
// protection.
#include <boost/function/detail/prologue.hpp>

#if defined(BOOST_MSVC)
#   pragma warning( push )
#   pragma warning( disable : 4127 ) // "conditional expression is constant"
#endif       

#define BOOST_FUNCTION_TEMPLATE_PARMS BOOST_PP_ENUM_PARAMS(BOOST_FUNCTION_NUM_ARGS, typename T)

#define BOOST_FUNCTION_TEMPLATE_ARGS BOOST_PP_ENUM_PARAMS(BOOST_FUNCTION_NUM_ARGS, T)

#define BOOST_FUNCTION_PARM(J,I,D) BOOST_PP_CAT(T,I) BOOST_PP_CAT(a,I)

#define BOOST_FUNCTION_PARMS BOOST_PP_ENUM(BOOST_FUNCTION_NUM_ARGS,BOOST_FUNCTION_PARM,BOOST_PP_EMPTY)

#define BOOST_FUNCTION_ARGS BOOST_PP_ENUM_PARAMS(BOOST_FUNCTION_NUM_ARGS, a)

#define BOOST_FUNCTION_ARG_TYPE(J,I,D) \
  typedef BOOST_PP_CAT(T,I) BOOST_PP_CAT(BOOST_PP_CAT(arg, BOOST_PP_INC(I)),_type);

#define BOOST_FUNCTION_ARG_TYPES BOOST_PP_REPEAT(BOOST_FUNCTION_NUM_ARGS,BOOST_FUNCTION_ARG_TYPE,BOOST_PP_EMPTY)

// Type of the default allocator
#ifndef BOOST_NO_STD_ALLOCATOR
#  define BOOST_FUNCTION_DEFAULT_ALLOCATOR std::allocator<function_base>
#else
#  define BOOST_FUNCTION_DEFAULT_ALLOCATOR int
#endif // BOOST_NO_STD_ALLOCATOR

// Comma if nonzero number of arguments
#if BOOST_FUNCTION_NUM_ARGS == 0
#  define BOOST_FUNCTION_COMMA
#else
#  define BOOST_FUNCTION_COMMA ,
#endif // BOOST_FUNCTION_NUM_ARGS > 0

// Class names used in this version of the code
#define BOOST_FUNCTION_FUNCTION BOOST_JOIN(function,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_FUNCTION_INVOKER \
  BOOST_JOIN(function_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_VOID_FUNCTION_INVOKER \
  BOOST_JOIN(void_function_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_FUNCTION_OBJ_INVOKER \
  BOOST_JOIN(function_obj_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER \
  BOOST_JOIN(void_function_obj_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_FUNCTION_REF_INVOKER \
  BOOST_JOIN(function_ref_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER \
  BOOST_JOIN(void_function_ref_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_MEMBER_INVOKER \
  BOOST_JOIN(member_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_VOID_MEMBER_INVOKER \
  BOOST_JOIN(void_member_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_GET_FUNCTION_INVOKER \
  BOOST_JOIN(get_function_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER \
  BOOST_JOIN(get_function_obj_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER \
  BOOST_JOIN(get_function_ref_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_GET_MEMBER_INVOKER \
  BOOST_JOIN(get_member_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_GET_INVOKER \
  BOOST_JOIN(get_invoker,BOOST_FUNCTION_NUM_ARGS)
#define BOOST_FUNCTION_VTABLE BOOST_JOIN(basic_vtable,BOOST_FUNCTION_NUM_ARGS)

#ifndef BOOST_NO_VOID_RETURNS
#  define BOOST_FUNCTION_VOID_RETURN_TYPE void
#  define BOOST_FUNCTION_RETURN(X) X
#else
#  define BOOST_FUNCTION_VOID_RETURN_TYPE boost::detail::function::unusable
#  define BOOST_FUNCTION_RETURN(X) X; return BOOST_FUNCTION_VOID_RETURN_TYPE ()
#endif

namespace boost {
  namespace detail {
    namespace function {
      template<
        typename FunctionPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_FUNCTION_INVOKER
      {
        static R invoke(function_buffer& function_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)
        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
          return f(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename FunctionPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
        >
      struct BOOST_FUNCTION_VOID_FUNCTION_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionPtr f = reinterpret_cast<FunctionPtr>(function_ptr.func_ptr);
          BOOST_FUNCTION_RETURN(f(BOOST_FUNCTION_ARGS));
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
      {
        static R invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f;
          if (function_allows_small_object_optimization<FunctionObj>::value)
            f = reinterpret_cast<FunctionObj*>(&function_obj_ptr.data);
          else
            f = reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          return (*f)(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f;
          if (function_allows_small_object_optimization<FunctionObj>::value)
            f = reinterpret_cast<FunctionObj*>(&function_obj_ptr.data);
          else
            f = reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          BOOST_FUNCTION_RETURN((*f)(BOOST_FUNCTION_ARGS));
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_FUNCTION_REF_INVOKER
      {
        static R invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f = 
            reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          return (*f)(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          FunctionObj* f = 
            reinterpret_cast<FunctionObj*>(function_obj_ptr.obj_ptr);
          BOOST_FUNCTION_RETURN((*f)(BOOST_FUNCTION_ARGS));
        }
      };

#if BOOST_FUNCTION_NUM_ARGS > 0
      /* Handle invocation of member pointers. */
      template<
        typename MemberPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_MEMBER_INVOKER
      {
        static R invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
                        BOOST_FUNCTION_PARMS)

        {
          MemberPtr* f = 
            reinterpret_cast<MemberPtr*>(&function_obj_ptr.data);
          return boost::mem_fn(*f)(BOOST_FUNCTION_ARGS);
        }
      };

      template<
        typename MemberPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_VOID_MEMBER_INVOKER
      {
        static BOOST_FUNCTION_VOID_RETURN_TYPE
        invoke(function_buffer& function_obj_ptr BOOST_FUNCTION_COMMA
               BOOST_FUNCTION_PARMS)

        {
          MemberPtr* f = 
            reinterpret_cast<MemberPtr*>(&function_obj_ptr.data);
          BOOST_FUNCTION_RETURN(boost::mem_fn(*f)(BOOST_FUNCTION_ARGS));
        }
      };
#endif

      template<
        typename FunctionPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
      >
      struct BOOST_FUNCTION_GET_FUNCTION_INVOKER
      {
        typedef typename mpl::if_c<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_INVOKER<
                            FunctionPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_INVOKER<
                            FunctionPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER
      {
        typedef typename mpl::if_c<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_OBJ_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

      template<
        typename FunctionObj,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER
      {
        typedef typename mpl::if_c<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_FUNCTION_REF_INVOKER<
                            FunctionObj,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };

#if BOOST_FUNCTION_NUM_ARGS > 0
      /* Retrieve the appropriate invoker for a member pointer.  */
      template<
        typename MemberPtr,
        typename R BOOST_FUNCTION_COMMA
        BOOST_FUNCTION_TEMPLATE_PARMS
       >
      struct BOOST_FUNCTION_GET_MEMBER_INVOKER
      {
        typedef typename mpl::if_c<(is_void<R>::value),
                            BOOST_FUNCTION_VOID_MEMBER_INVOKER<
                            MemberPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >,
                          BOOST_FUNCTION_MEMBER_INVOKER<
                            MemberPtr,
                            R BOOST_FUNCTION_COMMA
                            BOOST_FUNCTION_TEMPLATE_ARGS
                          >
                       >::type type;
      };
#endif

      /* Given the tag returned by get_function_tag, retrieve the
         actual invoker that will be used for the given function
         object. 

         Each specialization contains an "apply" nested class template
         that accepts the function object, return type, function
         argument types, and allocator. The resulting "apply" class
         contains two typedefs, "invoker_type" and "manager_type",
         which correspond to the invoker and manager types. */
      template<typename Tag>
      struct BOOST_FUNCTION_GET_INVOKER { };

      /* Retrieve the invoker for a function pointer. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<function_ptr_tag>
      {
        template<typename FunctionPtr,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS,
                 typename Allocator>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_INVOKER<
                             FunctionPtr,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<FunctionPtr, Allocator> manager_type;
        };
      };

#if BOOST_FUNCTION_NUM_ARGS > 0
      /* Retrieve the invoker for a member pointer. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<member_ptr_tag>
      {
        template<typename MemberPtr,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS,
                 typename Allocator>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_MEMBER_INVOKER<
                             MemberPtr,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<MemberPtr, Allocator> manager_type;
        };
      };
#endif

      /* Retrieve the invoker for a function object. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<function_obj_tag>
      {
        template<typename FunctionObj,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS,
                 typename Allocator>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER<
                             FunctionObj,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef functor_manager<FunctionObj, Allocator> manager_type;
        };
      };

      /* Retrieve the invoker for a reference to a function object. */
      template<>
      struct BOOST_FUNCTION_GET_INVOKER<function_obj_ref_tag>
      {
        template<typename RefWrapper,
                 typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS,
                 typename Allocator>
        struct apply
        {
          typedef typename BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER<
                             typename RefWrapper::type,
                             R BOOST_FUNCTION_COMMA
                             BOOST_FUNCTION_TEMPLATE_ARGS
                           >::type
            invoker_type;

          typedef reference_manager<typename RefWrapper::type> manager_type;
        };
      };

      /**
       * vtable for a specific boost::function instance.
       */
      template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS,
               typename Allocator>
      struct BOOST_FUNCTION_VTABLE
      {
#ifndef BOOST_NO_VOID_RETURNS
        typedef R         result_type;
#else
        typedef typename function_return_type<R>::type result_type;
#endif // BOOST_NO_VOID_RETURNS

        typedef result_type (*invoker_type)(function_buffer&
                                            BOOST_FUNCTION_COMMA
                                            BOOST_FUNCTION_TEMPLATE_ARGS);

        template<typename F>
        bool assign_to(const F& f, function_buffer& functor) const
        {
          typedef typename get_function_tag<F>::type tag;
          return assign_to(f, functor, tag());
        }

        void clear(function_buffer& functor) const
        {
          if (base.manager)
            base.manager(functor, functor, destroy_functor_tag);
        }
#ifndef BOOST_NO_PRIVATE_IN_AGGREGATE
      private:
#endif
        // Function pointers
        template<typename FunctionPtr>
        bool 
        assign_to(FunctionPtr f, function_buffer& functor, 
                  function_ptr_tag) const
        {
          this->clear(functor);
          if (f) {
            // should be a reinterpret cast, but some compilers insist
            // on giving cv-qualifiers to free functions
            functor.func_ptr = (void (*)())(f);
            return true;
          } else {
            return false;
          }
        }

        // Member pointers
#if BOOST_FUNCTION_NUM_ARGS > 0
        template<typename MemberPtr>
        bool 
        assign_to(MemberPtr f, function_buffer& functor, member_ptr_tag) const
        {
          if (f) {
            // Always use the small-object optimization for member
            // pointers.
            assign_functor(f, functor, mpl::true_());
            return true;
          } else {
            return false;
          }
        }
#endif // BOOST_FUNCTION_NUM_ARGS > 0

        // Function objects
        // Assign to a function object using the small object optimization
        template<typename FunctionObj>
        void 
        assign_functor(const FunctionObj& f, function_buffer& functor, 
                       mpl::true_) const
        {
          new ((void*)&functor.data) FunctionObj(f);
        }

        // Assign to a function object allocated on the heap.
        template<typename FunctionObj>
        void 
        assign_functor(const FunctionObj& f, function_buffer& functor, 
                       mpl::false_) const
        {
#ifndef BOOST_NO_STD_ALLOCATOR
          typedef typename Allocator::template rebind<FunctionObj>::other
            allocator_type;
          typedef typename allocator_type::pointer pointer_type;

          allocator_type allocator;
          pointer_type copy = allocator.allocate(1);
          allocator.construct(copy, f);

          // Get back to the original pointer type
          functor.obj_ptr = static_cast<FunctionObj*>(copy);
#  else
          functor.obj_ptr = new FunctionObj(f);
#  endif // BOOST_NO_STD_ALLOCATOR
        }

        template<typename FunctionObj>
        bool 
        assign_to(const FunctionObj& f, function_buffer& functor, 
                  function_obj_tag) const
        {
          if (!boost::detail::function::has_empty_target(boost::addressof(f))) {
            assign_functor(f, functor, 
                           mpl::bool_<(function_allows_small_object_optimization<FunctionObj>::value)>());
            return true;
          } else {
            return false;
          }
        }

        // Reference to a function object
        template<typename FunctionObj>
        bool 
        assign_to(const reference_wrapper<FunctionObj>& f, 
                  function_buffer& functor, function_obj_ref_tag) const
        {
          if (!boost::detail::function::has_empty_target(f.get_pointer())) {
            // DPG TBD: We might need to detect constness of
            // FunctionObj to assign into obj_ptr or const_obj_ptr to
            // be truly legit, but no platform in existence makes
            // const void* different from void*.
            functor.const_obj_ptr = f.get_pointer();
            return true;
          } else {
            return false;
          }
        }

      public:
        vtable_base base;
        invoker_type invoker;
      };
    } // end namespace function
  } // end namespace detail

  template<
    typename R BOOST_FUNCTION_COMMA
    BOOST_FUNCTION_TEMPLATE_PARMS,
    typename Allocator = BOOST_FUNCTION_DEFAULT_ALLOCATOR
  >
  class BOOST_FUNCTION_FUNCTION : public function_base

#if BOOST_FUNCTION_NUM_ARGS == 1

    , public std::unary_function<T0,R>

#elif BOOST_FUNCTION_NUM_ARGS == 2

    , public std::binary_function<T0,T1,R>

#endif

  {
  public:
#ifndef BOOST_NO_VOID_RETURNS
    typedef R         result_type;
#else
    typedef  typename boost::detail::function::function_return_type<R>::type
      result_type;
#endif // BOOST_NO_VOID_RETURNS

  private:
    typedef boost::detail::function::BOOST_FUNCTION_VTABLE<
              R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_ARGS, Allocator>
      vtable_type;

    struct clear_type {};

  public:
    BOOST_STATIC_CONSTANT(int, args = BOOST_FUNCTION_NUM_ARGS);

    // add signature for boost::lambda
    template<typename Args>
    struct sig
    {
      typedef result_type type;
    };

#if BOOST_FUNCTION_NUM_ARGS == 1
    typedef T0 argument_type;
#elif BOOST_FUNCTION_NUM_ARGS == 2
    typedef T0 first_argument_type;
    typedef T1 second_argument_type;
#endif

    BOOST_STATIC_CONSTANT(int, arity = BOOST_FUNCTION_NUM_ARGS);
    BOOST_FUNCTION_ARG_TYPES

    typedef Allocator allocator_type;
    typedef BOOST_FUNCTION_FUNCTION self_type;

    BOOST_FUNCTION_FUNCTION() : function_base() { }

    // MSVC chokes if the following two constructors are collapsed into
    // one with a default parameter.
    template<typename Functor>
    BOOST_FUNCTION_FUNCTION(Functor BOOST_FUNCTION_TARGET_FIX(const &) f
#ifndef BOOST_NO_SFINAE
                            ,typename enable_if_c<
                            (boost::type_traits::ice_not<
                             (is_integral<Functor>::value)>::value),
                                        int>::type = 0
#endif // BOOST_NO_SFINAE
                            ) :
      function_base()
    {
      this->assign_to(f);
    }

#ifndef BOOST_NO_SFINAE
    BOOST_FUNCTION_FUNCTION(clear_type*) : function_base() { }
#else
    BOOST_FUNCTION_FUNCTION(int zero) : function_base()
    {
      BOOST_ASSERT(zero == 0);
    }
#endif

    BOOST_FUNCTION_FUNCTION(const BOOST_FUNCTION_FUNCTION& f) : function_base()
    {
      this->assign_to_own(f);
    }

    ~BOOST_FUNCTION_FUNCTION() { clear(); }

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
    // MSVC 6.0 and prior require all definitions to be inline, but
    // these definitions can become very costly.
    result_type operator()(BOOST_FUNCTION_PARMS) const
    {
      if (this->empty())
        boost::throw_exception(bad_function_call());

      return reinterpret_cast<const vtable_type*>(vtable)->invoker
               (this->functor BOOST_FUNCTION_COMMA BOOST_FUNCTION_ARGS);
    }
#else
    result_type operator()(BOOST_FUNCTION_PARMS) const;
#endif

    // The distinction between when to use BOOST_FUNCTION_FUNCTION and
    // when to use self_type is obnoxious. MSVC cannot handle self_type as
    // the return type of these assignment operators, but Borland C++ cannot
    // handle BOOST_FUNCTION_FUNCTION as the type of the temporary to
    // construct.
    template<typename Functor>
#ifndef BOOST_NO_SFINAE
    typename enable_if_c<
               (boost::type_traits::ice_not<
                 (is_integral<Functor>::value)>::value),
               BOOST_FUNCTION_FUNCTION&>::type
#else
    BOOST_FUNCTION_FUNCTION&
#endif
    operator=(Functor BOOST_FUNCTION_TARGET_FIX(const &) f)
    {
      this->clear();
#ifndef BOOST_NO_EXCEPTIONS
      try {
        this->assign_to(f);
      } catch (...) {
        vtable = 0;
        throw;
      }
#else
      this->assign_to(f);
#endif
      return *this;
    }

#ifndef BOOST_NO_SFINAE
    BOOST_FUNCTION_FUNCTION& operator=(clear_type*)
    {
      this->clear();
      return *this;
    }
#else
    BOOST_FUNCTION_FUNCTION& operator=(int zero)
    {
      BOOST_ASSERT(zero == 0);
      this->clear();
      return *this;
    }
#endif

    // Assignment from another BOOST_FUNCTION_FUNCTION
    BOOST_FUNCTION_FUNCTION& operator=(const BOOST_FUNCTION_FUNCTION& f)
    {
      if (&f == this)
        return *this;

      this->clear();
#ifndef BOOST_NO_EXCEPTIONS
      try {
        this->assign_to_own(f);
      } catch (...) {
        vtable = 0;
        throw;
      }
#else
      this->assign_to_own(f);
#endif
      return *this;
    }

    void swap(BOOST_FUNCTION_FUNCTION& other)
    {
      if (&other == this)
        return;

      BOOST_FUNCTION_FUNCTION tmp = *this;
      *this = other;
      other = tmp;
    }

    // Clear out a target, if there is one
    void clear()
    {
      if (vtable) {
        reinterpret_cast<const vtable_type*>(vtable)->clear(this->functor);
        vtable = 0;
      }
    }

#if (defined __SUNPRO_CC) && (__SUNPRO_CC <= 0x530) && !(defined BOOST_NO_COMPILER_CONFIG)
    // Sun C++ 5.3 can't handle the safe_bool idiom, so don't use it
    operator bool () const { return !this->empty(); }
#else
  private:
    struct dummy {
      void nonnull() {};
    };

    typedef void (dummy::*safe_bool)();

  public:
    operator safe_bool () const
      { return (this->empty())? 0 : &dummy::nonnull; }

    bool operator!() const
      { return this->empty(); }
#endif

  private:
    void assign_to_own(const BOOST_FUNCTION_FUNCTION& f)
    {
      if (!f.empty()) {
        this->vtable = f.vtable;
        f.vtable->manager(f.functor, this->functor,
                          boost::detail::function::clone_functor_tag);
      }
    }

    template<typename Functor>
    void assign_to(const Functor& f)
    {
      using detail::function::vtable_base;

      typedef typename detail::function::get_function_tag<Functor>::type tag;
      typedef detail::function::BOOST_FUNCTION_GET_INVOKER<tag> get_invoker;
      typedef typename get_invoker::
                         template apply<Functor, R BOOST_FUNCTION_COMMA 
                        BOOST_FUNCTION_TEMPLATE_ARGS, Allocator>
        handler_type;
      
      typedef typename handler_type::invoker_type invoker_type;
      typedef typename handler_type::manager_type manager_type;
      
      static const vtable_type stored_vtable = 
        { { &manager_type::manage }, &invoker_type::invoke };

      if (stored_vtable.assign_to(f, functor)) vtable = &stored_vtable.base;
      else vtable = 0;
    }
  };

  template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS ,
           typename Allocator>
  inline void swap(BOOST_FUNCTION_FUNCTION<
                     R BOOST_FUNCTION_COMMA
                     BOOST_FUNCTION_TEMPLATE_ARGS ,
                     Allocator
                   >& f1,
                   BOOST_FUNCTION_FUNCTION<
                     R BOOST_FUNCTION_COMMA
                     BOOST_FUNCTION_TEMPLATE_ARGS,
                     Allocator
                   >& f2)
  {
    f1.swap(f2);
  }

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1300)
  template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS,
           typename Allocator>
  typename BOOST_FUNCTION_FUNCTION<
      R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_ARGS,
      Allocator>::result_type
   BOOST_FUNCTION_FUNCTION<R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_ARGS,

                           Allocator>
  ::operator()(BOOST_FUNCTION_PARMS) const
  {
    if (this->empty())
      boost::throw_exception(bad_function_call());

    return reinterpret_cast<const vtable_type*>(vtable)->invoker
             (this->functor BOOST_FUNCTION_COMMA BOOST_FUNCTION_ARGS);
  }
#endif

// Poison comparisons between boost::function objects of the same type.
template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS ,
         typename Allocator>
  void operator==(const BOOST_FUNCTION_FUNCTION<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS ,
                          Allocator>&,
                  const BOOST_FUNCTION_FUNCTION<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS ,
                  Allocator>&);
template<typename R BOOST_FUNCTION_COMMA BOOST_FUNCTION_TEMPLATE_PARMS ,
         typename Allocator>
  void operator!=(const BOOST_FUNCTION_FUNCTION<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS ,
                          Allocator>&,
                  const BOOST_FUNCTION_FUNCTION<
                          R BOOST_FUNCTION_COMMA
                          BOOST_FUNCTION_TEMPLATE_ARGS ,
                  Allocator>&);

#if !defined(BOOST_FUNCTION_NO_FUNCTION_TYPE_SYNTAX)

#if BOOST_FUNCTION_NUM_ARGS == 0
#define BOOST_FUNCTION_PARTIAL_SPEC R (void)
#else
#define BOOST_FUNCTION_PARTIAL_SPEC R (BOOST_PP_ENUM_PARAMS(BOOST_FUNCTION_NUM_ARGS,T))
#endif

template<typename R BOOST_FUNCTION_COMMA
         BOOST_FUNCTION_TEMPLATE_PARMS,
         typename Allocator>
class function<BOOST_FUNCTION_PARTIAL_SPEC, Allocator>
  : public BOOST_FUNCTION_FUNCTION<R, BOOST_FUNCTION_TEMPLATE_ARGS
                                   BOOST_FUNCTION_COMMA Allocator>
{
  typedef BOOST_FUNCTION_FUNCTION<R, BOOST_FUNCTION_TEMPLATE_ARGS
                                  BOOST_FUNCTION_COMMA Allocator> base_type;
  typedef function self_type;

  struct clear_type {};

public:
  typedef typename base_type::allocator_type allocator_type;

  function() : base_type() {}

  template<typename Functor>
  function(Functor f
#ifndef BOOST_NO_SFINAE
           ,typename enable_if_c<
                            (boost::type_traits::ice_not<
                          (is_integral<Functor>::value)>::value),
                       int>::type = 0
#endif
           ) :
    base_type(f)
  {
  }

#ifndef BOOST_NO_SFINAE
  function(clear_type*) : base_type() {}
#endif

  function(const self_type& f) : base_type(static_cast<const base_type&>(f)){}

  function(const base_type& f) : base_type(static_cast<const base_type&>(f)){}

  self_type& operator=(const self_type& f)
  {
    self_type(f).swap(*this);
    return *this;
  }

  template<typename Functor>
#ifndef BOOST_NO_SFINAE
  typename enable_if_c<
                            (boost::type_traits::ice_not<
                         (is_integral<Functor>::value)>::value),
                      self_type&>::type
#else
  self_type&
#endif
  operator=(Functor f)
  {
    self_type(f).swap(*this);
    return *this;
  }

#ifndef BOOST_NO_SFINAE
  self_type& operator=(clear_type*)
  {
    this->clear();
    return *this;
  }
#endif

  self_type& operator=(const base_type& f)
  {
    self_type(f).swap(*this);
    return *this;
  }
};

#undef BOOST_FUNCTION_PARTIAL_SPEC
#endif // have partial specialization

} // end namespace boost

// Cleanup after ourselves...
#undef BOOST_FUNCTION_VTABLE
#undef BOOST_FUNCTION_GET_INVOKER
#undef BOOST_FUNCTION_DEFAULT_ALLOCATOR
#undef BOOST_FUNCTION_COMMA
#undef BOOST_FUNCTION_FUNCTION
#undef BOOST_FUNCTION_FUNCTION_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_INVOKER
#undef BOOST_FUNCTION_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_FUNCTION_REF_INVOKER
#undef BOOST_FUNCTION_VOID_FUNCTION_REF_INVOKER
#undef BOOST_FUNCTION_MEMBER_INVOKER
#undef BOOST_FUNCTION_VOID_MEMBER_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_OBJ_INVOKER
#undef BOOST_FUNCTION_GET_FUNCTION_REF_INVOKER
#undef BOOST_FUNCTION_GET_MEMBER_INVOKER
#undef BOOST_FUNCTION_TEMPLATE_PARMS
#undef BOOST_FUNCTION_TEMPLATE_ARGS
#undef BOOST_FUNCTION_PARMS
#undef BOOST_FUNCTION_PARM
#undef BOOST_FUNCTION_ARGS
#undef BOOST_FUNCTION_ARG_TYPE
#undef BOOST_FUNCTION_ARG_TYPES
#undef BOOST_FUNCTION_VOID_RETURN_TYPE
#undef BOOST_FUNCTION_RETURN

#if defined(BOOST_MSVC)
#   pragma warning( pop )
#endif       
