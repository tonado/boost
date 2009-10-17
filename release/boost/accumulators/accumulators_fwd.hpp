///////////////////////////////////////////////////////////////////////////////
// accumulators_fwd.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_ACCUMULATORS_FWD_HPP_EAN_28_10_2005
#define BOOST_ACCUMULATORS_ACCUMULATORS_FWD_HPP_EAN_28_10_2005

#include <boost/config.hpp>
#include <boost/mpl/apply_fwd.hpp> // for mpl::na
#include <boost/mpl/limits/vector.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/accumulators/numeric/functional_fwd.hpp>

#ifndef BOOST_ACCUMULATORS_MAX_FEATURES
  /// The maximum number of accumulators that may be put in an accumulator_set.
  /// Defaults to BOOST_MPL_LIMIT_VECTOR_SIZE (which defaults to 20).
# define BOOST_ACCUMULATORS_MAX_FEATURES BOOST_MPL_LIMIT_VECTOR_SIZE
#endif

#if BOOST_ACCUMULATORS_MAX_FEATURES > BOOST_MPL_LIMIT_VECTOR_SIZE
# error BOOST_ACCUMULATORS_MAX_FEATURES cannot be larger than BOOST_MPL_LIMIT_VECTOR_SIZE
#endif

#ifndef BOOST_ACCUMULATORS_MAX_ARGS
  /// The maximum number of arguments that may be specified to an accumulator_set's
  /// accumulation function. Defaults to 15.
# define BOOST_ACCUMULATORS_MAX_ARGS 15
#endif

#if BOOST_WORKAROUND(__GNUC__, == 3) \
 || BOOST_WORKAROUND(__EDG_VERSION__, BOOST_TESTED_AT(306))
# define BOOST_ACCUMULATORS_BROKEN_CONST_OVERLOADS
#endif

#ifdef BOOST_ACCUMULATORS_BROKEN_CONST_OVERLOADS
# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/is_const.hpp>
# define BOOST_ACCUMULATORS_PROTO_DISABLE_IF_IS_CONST(T)\
    , typename boost::disable_if<boost::is_const<T> >::type * = 0
#else
# define BOOST_ACCUMULATORS_PROTO_DISABLE_IF_IS_CONST(T)
#endif

namespace boost { namespace accumulators
{

///////////////////////////////////////////////////////////////////////////////
// Named parameters tags
//
namespace tag
{
    struct sample;
    struct weight;
    struct accumulator;
    struct weights;
}

///////////////////////////////////////////////////////////////////////////////
// User-level features
//
namespace tag
{
    template<typename ValueType, typename Tag>
    struct value;

    template<typename Tag>
    struct value_tag;

    template<typename Referent, typename Tag>
    struct reference;

    template<typename Tag>
    struct reference_tag;

    template<typename Type, typename Tag = void, typename AccumulatorSet = void>
    struct external;

    template<typename Feature>
    struct droppable;
}

template<typename Accumulator>
struct droppable_accumulator_base;

template<typename Accumulator>
struct droppable_accumulator;

template<typename Accumulator>
struct with_cached_result;

template<typename Sample, typename Features, typename Weight = void>
struct accumulator_set;

template<typename Feature>
struct extractor;

template<typename Feature>
struct feature_of;

template<typename Feature>
struct as_feature;

template<typename Feature>
struct as_weighted_feature;

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_ACCUMULATORS_MAX_FEATURES, typename Feature, mpl::na)>
struct depends_on;

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_ACCUMULATORS_MAX_FEATURES, typename Feature, mpl::na)>
struct features;

template<typename Feature, typename AccumulatorSet>
typename mpl::apply<AccumulatorSet, Feature>::type const &
find_accumulator(AccumulatorSet const &acc);

template<typename Feature, typename AccumulatorSet>
typename mpl::apply<AccumulatorSet, Feature>::type::result_type
extract_result(AccumulatorSet const &acc);

template<typename Feature, typename AccumulatorSet, typename A1>
typename mpl::apply<AccumulatorSet, Feature>::type::result_type
extract_result(AccumulatorSet const &acc, A1 const &a1);

// ... other overloads generated by Boost.Preprocessor:

/// INTERNAL ONLY
///
#define BOOST_ACCUMULATORS_EXTRACT_RESULT_FWD(z, n, _)                      \
    template<                                                               \
        typename Feature                                                    \
      , typename AccumulatorSet                                             \
        BOOST_PP_ENUM_TRAILING_PARAMS_Z(z, n, typename A)                   \
    >                                                                       \
    typename mpl::apply<AccumulatorSet, Feature>::type::result_type         \
    extract_result(                                                         \
        AccumulatorSet const &acc                                           \
        BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(z, n, A, const &a)           \
    );

/// INTERNAL ONLY
///
BOOST_PP_REPEAT_FROM_TO(
    2
  , BOOST_PP_INC(BOOST_ACCUMULATORS_MAX_ARGS)
  , BOOST_ACCUMULATORS_EXTRACT_RESULT_FWD
  , _
)

#ifdef BOOST_ACCUMULATORS_DOXYGEN_INVOKED
template<typename Feature, typename AccumulatorSet, typename A1, typename A2 ...>
typename mpl::apply<AccumulatorSet, Feature>::type::result_type
extract_result(AccumulatorSet const &acc, A1 const &a1, A2 const &a2 ...);
#endif

namespace impl
{
    using namespace numeric::operators;

    template<typename Accumulator, typename Tag>
    struct external_impl;
}

namespace detail
{
    template<typename Accumulator>
    struct feature_tag;

    template<typename Feature, typename Sample, typename Weight>
    struct to_accumulator;

    struct accumulator_set_base;

    template<typename T>
    struct is_accumulator_set;

    inline void ignore_variable(void const *) {}

  #define BOOST_ACCUMULATORS_IGNORE_GLOBAL(X)\
    namespace detail { inline void BOOST_PP_CAT(ignore_, X)() { boost::accumulators::detail::ignore_variable(&X); } }
}

}} // namespace boost::accumulators

// For defining boost::parameter keywords that can be inherited from to
// get a nested, class-scoped keyword with the requested alias
#define BOOST_PARAMETER_NESTED_KEYWORD(tag_namespace, name, alias)                                  \
    namespace tag_namespace                                                                         \
    {                                                                                               \
        template<int Dummy = 0>                                                                     \
        struct name ## _                                                                            \
        {                                                                                           \
            static char const* keyword_name()                                                       \
            {                                                                                       \
                return #name;                                                                       \
            }                                                                                       \
            static ::boost::parameter::keyword<name ## _<Dummy> > &alias;                           \
        };                                                                                          \
        template<int Dummy>                                                                         \
        ::boost::parameter::keyword<name ## _<Dummy> > &name ## _<Dummy>::alias =                   \
        ::boost::parameter::keyword<name ## _<Dummy> >::get();                                      \
        typedef name ## _ <> name;                                                                  \
    }                                                                                               \
    namespace                                                                                       \
    {                                                                                               \
        ::boost::parameter::keyword<tag_namespace::name> &name =                                    \
        ::boost::parameter::keyword<tag_namespace::name>::get();                                    \
    }

#endif
