// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NAMED_PARAMS_031014_HPP
#define BOOST_NAMED_PARAMS_031014_HPP

#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/iterator/detail/config_def.hpp>
#include <boost/python/detail/is_xxx.hpp>

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/named_params/aux_/arg_list.hpp>
#include <boost/named_params/aux_/yesno.hpp>
#include <boost/named_params/aux_/void.hpp>
#include <boost/named_params/aux_/default.hpp>
#include <boost/named_params/aux_/unwrap_cv_reference.hpp>
#include <boost/named_params/aux_/tagged_argument.hpp>
#include <boost/named_params/keyword.hpp>

#ifndef BOOST_NAMED_PARAMS_MAX_ARITY
# define BOOST_NAMED_PARAMS_MAX_ARITY 5
#endif

#if defined(__GNUC__) && __GNUC__ < 3
#   define BOOST_NAMED_PARAMS_GCC2 1
#else
#   define BOOST_NAMED_PARAMS_GCC2 0
#endif

namespace boost {

template<class T> class reference_wrapper;

namespace named_params {

// These templates can be used to describe the treatment of particular
// named parameters for the purposes of overload elimination with
// SFINAE, by placing specializations in the parameters<...> list.  In
// order for a treated function to participate in overload resolution:
//
//   - all keyword tags wrapped in required<...> must have a matching
//     actual argument
//
//   - The actual argument type matched by every keyword tag
//     associated with a predicate must satisfy that predicate
//
// If a keyword k is specified without an optional<...> or
// required<...>, wrapper, it is treated as though optional<k> were
// specified.
//
template <class Tag, class Predicate = mpl::always<mpl::true_> >
struct required
{
    typedef Tag key_type;
    typedef Predicate predicate;
};

template <class Tag, class Predicate = mpl::always<mpl::true_> >
struct optional
{
    typedef Tag key_type;
    typedef Predicate predicate;
};

namespace aux
{
  // Defines metafunctions, is_required and is_optional, that
  // identify required<...> and optional<...> specializations.
  BOOST_PYTHON_IS_XXX_DEF(required, required, 2)
  BOOST_PYTHON_IS_XXX_DEF(optional, optional, 2)

  //
  // key_type, has_default, and predicate --
  //
  // These metafunctions accept a ParameterSpec and extract the
  // keyword tag, whether or not a default is supplied for the
  // parameter, and the predicate that the corresponding actual
  // argument type is required match.
  //
  // a ParameterSpec is a specialization of either keyword<...>,
  // required<...> or optional<...>.
  //
  
  // helper for key_type<...>, below.
  template <class T>
  struct get_key_type
  { typedef typename T::key_type type; };

  template <class T>
  struct key_type
    : mpl::eval_if<
          mpl::or_<
              is_optional<T>
            , is_required<T>
          >
        , get_key_type<T>
        , mpl::identity<T>
      >
  {
  };

  template <class T>
  struct has_default
    : mpl::not_<typename is_required<T>::type>
  {
  };

  // helper for predicate<...>, below
  template <class T>
  struct get_predicate
  {
      typedef typename T::predicate type;
  };

  template <class T>
  struct predicate
    : mpl::eval_if<
         mpl::or_<
              is_optional<T>
            , is_required<T>
          >
        , get_predicate<T>
        , mpl::identity<mpl::always<mpl::true_> >
      >
  {
  };


  // Converts a ParameterSpec into a specialization of
  // parameter_requirements.  We need to do this in order to get the
  // key_type into the type in a way that can be conveniently matched
  // by a satisfies(...) member function in arg_list.
  template <class ParameterSpec>
  struct as_parameter_requirements
  {
      typedef parameter_requirements<
          typename key_type<ParameterSpec>::type
        , typename predicate<ParameterSpec>::type
        , typename has_default<ParameterSpec>::type
      > type;
  };

  // Labels Arg with default keyword tag DefaultTag if it is not
  // already a tagged_argument
  template <class DefaultTag, class Arg>
  struct as_tagged_argument
  {
      typedef typename mpl::if_<
          is_tagged_argument<Arg>
        , Arg
        , tagged_argument<
              typename key_type<DefaultTag>::type
            , typename unwrap_cv_reference<Arg const>::type
          >
      >::type type;
  };

#if BOOST_WORKAROUND(BOOST_MSVC, == 1200)  // ETI workaround
  template <>
  struct as_tagged_argument<int,int>
  {
      typedef int type;
  };
#endif

  // Returns mpl::true_ iff the given ParameterRequirements are
  // satisfied by ArgList.
  template <class ArgList, class ParameterRequirements>
  struct satisfies
  {
      BOOST_STATIC_CONSTANT(
          bool, value = (
              sizeof(
                  aux::to_yesno(
                      ArgList::satisfies((ParameterRequirements*)0)
                  )
              ) == sizeof(yes_tag)
          )
      );

      typedef mpl::bool_<value> type;
  };

  // Returns mpl::true_ if the requirements of the given ParameterSpec
  // are satisfied by ArgList.
  template <class ArgList, class ParameterSpec>
  struct satisfies_requirements_of
    : satisfies<
          ArgList
        , typename as_parameter_requirements<ParameterSpec>::type
      >
  {};

  // Helper for make_partial_arg_list, below.  Produce an arg_list
  // node for the given ParameterSpec and ArgumentType, whose tail is
  // determined by invoking the nullary metafunction TailFn.
  template <class ParameterSpec, class ArgumentType, class TailFn>
  struct make_arg_list
  {
      typedef arg_list<
          typename as_tagged_argument<ParameterSpec,ArgumentType>::type
        , typename TailFn::type
      > type;
  };

  // Just like make_arg_list, except if ArgumentType is void_, the
  // result is empty_arg_list.  Used to build arg_lists whose length
  // depends on the number of non-default (void_) arguments passed to
  // a class template.
  template <
      class ParameterSpec
    , class ArgumentType
    , class TailFn
  >
  struct make_partial_arg_list
    : mpl::eval_if<
          is_same<ArgumentType,void_>
        , mpl::identity<empty_arg_list>
        , make_arg_list<ParameterSpec, ArgumentType, TailFn>
      >
  {};

  // Generates:
  //
  //   make<
  //       parameter_spec#0, argument_type#0
  //     , make<
  //           parameter_spec#1, argument_type#1
  //         , ... mpl::identity<aux::empty_arg_list>
  //    ...>
  //   >
#define BOOST_NAMED_PARAMS_make_arg_list(z, n, names)   \
      BOOST_PP_SEQ_ELEM(0,names)<                       \
          BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1,names), n),  \
          BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(2,names), n),
      
#define BOOST_NAMED_PARAMS_right_angle(z, n, text)    >
    
#define BOOST_NAMED_PARAMS_build_arg_list(n, make, parameter_spec, argument_type)   \
  BOOST_PP_REPEAT(                                                                  \
      n, BOOST_NAMED_PARAMS_make_arg_list, (make)(parameter_spec)(argument_type))   \
  mpl::identity<aux::empty_arg_list>                                                \
  BOOST_PP_REPEAT(n, BOOST_NAMED_PARAMS_right_angle, _)
  
} // namespace aux

#define BOOST_PARAMETERS_TEMPLATE_ARGS(z, n, text) class BOOST_PP_CAT(PS, n) = aux::void_

template<
     class PS0
   , BOOST_PP_ENUM_SHIFTED(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_PARAMETERS_TEMPLATE_ARGS, _)
>
struct parameters
{
    typedef parameters self;
#undef BOOST_PARAMETERS_TEMPLATE_ARGS

    // if the elements of NamedList match the criteria of overload
    // resolution, returns a type which can be constructed from
    // parameters.  Otherwise, this is not a valid metafunction (no nested
    // ::type).


#ifndef BOOST_NO_SFINAE
    // If NamedList satisfies the PS0, PS1, ..., this is a
    // metafunction returning parameters.  Otherwise it 
    // has no nested ::type.
    template <class NamedList>
    struct restrict_base
      : mpl::if_<
            // mpl::and_<
            //    aux::satisfies_requirements_of<NamedList,PS0>
            //  , mpl::and_<
            //       aux::satisfies_requirements_of<NamedList,PS1>...
            //           ..., mpl::true_
            // ...> >
            
# define BOOST_NAMED_PARAMS_satisfies(z, n, text)                                   \
            mpl::and_<                                                              \
                aux::satisfies_requirements_of<NamedList, BOOST_PP_CAT(PS, n)> ,
      
            BOOST_PP_REPEAT(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_NAMED_PARAMS_satisfies, _)
            mpl::true_
            BOOST_PP_REPEAT(BOOST_NAMED_PARAMS_MAX_ARITY, BOOST_NAMED_PARAMS_right_angle, _)

# undef BOOST_NAMED_PARAMS_satisfies
                                                                                    
          , mpl::identity<parameters>
          , aux::void_
        >
    {};
#endif
    
    
    // Specializations are to be used as an optional argument to
    // eliminate overloads via SFINAE
    template<
        BOOST_PP_ENUM_BINARY_PARAMS(
            BOOST_NAMED_PARAMS_MAX_ARITY, class A, = aux::void_ BOOST_PP_INTERCEPT
        )       
    >
    struct restrict
#ifndef BOOST_NO_SFINAE
      : restrict_base<
            typename BOOST_NAMED_PARAMS_build_arg_list(
                BOOST_NAMED_PARAMS_MAX_ARITY, aux::make_partial_arg_list, PS, A
            )::type
        >::type
    {};
#else
    { typedef parameters type; };
#endif

    //
    // The function call operator is used to build an arg_list that
    // labels the positional parameters and maintains whatever other
    // tags may have been specified by the caller.
    //
    aux::empty_arg_list operator()() const
    {
       return aux::empty_arg_list();
    }

    template<class A0>
    typename
      aux::make_arg_list<PS0,A0, mpl::identity<aux::empty_arg_list> >
    ::type
    operator()( A0 const& a0) const
    {
        typedef typename
          aux::make_arg_list<PS0, A0, mpl::identity<aux::empty_arg_list> >
        ::type result_type;

        return result_type(
            a0
            // , void_(), void_(), void_() ...
            BOOST_PP_ENUM_TRAILING_PARAMS(
                BOOST_PP_SUB(BOOST_NAMED_PARAMS_MAX_ARITY, 1)
              , aux::void_() BOOST_PP_INTERCEPT)
        );
    }

    template<class A0, class A1>
    typename
      aux::make_arg_list<
          PS0,A0
        , aux::make_arg_list<
              PS1,A1
            , mpl::identity<aux::empty_arg_list>
          >
      >
    ::type
    operator()(A0 const& a0, A1 const& a1) const
    {
        typedef typename
          aux::make_arg_list<
              PS0,A0
            , aux::make_arg_list<
                  PS1,A1
                , mpl::identity<aux::empty_arg_list>
              >
          >
        ::type result_type;


        return result_type(
            a0, a1
            // , void_(), void_() ...
            BOOST_PP_ENUM_TRAILING_PARAMS(
                BOOST_PP_SUB(BOOST_NAMED_PARAMS_MAX_ARITY, 2)
              , aux::void_() BOOST_PP_INTERCEPT)
        );
    }

    // Higher arities are handled by the preprocessor
#define BOOST_PP_ITERATION_PARAMS_1 (3,( \
        3,BOOST_NAMED_PARAMS_MAX_ARITY,<boost/named_params/aux_/overloads.hpp> \
    ))
#include BOOST_PP_ITERATE()
    
#undef BOOST_NAMED_PARAMS_build_arg_list
#undef BOOST_NAMED_PARAMS_make_arg_list
#undef BOOST_NAMED_PARAMS_right_angle

};

} // namespace named_params

} // namespace boost

#undef BOOST_NAMED_PARAMS_GCC2

#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/logical/bool.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>

#define BOOST_NAMED_PARAMS_FUN_TEMPLATE_HEAD1(n) \
    template<BOOST_PP_ENUM_PARAMS(n, class T)>

#define BOOST_NAMED_PARAMS_FUN_TEMPLATE_HEAD0(n)

#define BOOST_NAMED_PARAMS_FUN_DECL(z, n, params)                                       \
                                                                                        \
    BOOST_PP_CAT(BOOST_NAMED_PARAMS_FUN_TEMPLATE_HEAD, BOOST_PP_BOOL(n))(n)             \
                                                                                        \
    BOOST_PP_TUPLE_ELEM(3, 0, params)                                                   \
        BOOST_PP_TUPLE_ELEM(3, 1, params)(                                              \
            BOOST_PP_ENUM_BINARY_PARAMS(n, const T, &p)                                 \
            BOOST_PP_COMMA_IF(n)                                                        \
            BOOST_PP_EXPR_IF(n, typename) BOOST_PP_TUPLE_ELEM(3, 2, params)::restrict   \
            <                                                                           \
                BOOST_PP_ENUM_PARAMS(n, T)                                              \
            >::type kw = BOOST_PP_TUPLE_ELEM(3, 2, params)()                            \
        )                                                                               \
    {                                                                                   \
        return BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(3, 1, params), _with_named_params)(     \
            kw(BOOST_PP_ENUM_PARAMS(n, p))                                              \
        );                                                                              \
    }

#define BOOST_NAMED_PARAMS_FUN(ret, name, lo, hi, keywords)                         \
                                                                                    \
    template<class Params>                                                          \
    ret BOOST_PP_CAT(name, _with_named_params)(const Params&);                      \
                                                                                    \
    BOOST_PP_REPEAT_FROM_TO(                                                        \
        lo, BOOST_PP_INC(hi), BOOST_NAMED_PARAMS_FUN_DECL, (ret, name, keywords))   \
                                                                                    \
    template<class Params>                                                          \
    ret BOOST_PP_CAT(name, _with_named_params)(const Params& p)

#include <boost/iterator/detail/config_undef.hpp>
 
#endif // BOOST_NAMED_PARAMS_031014_HPP

