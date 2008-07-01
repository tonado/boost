// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ARCHIVE_DETAIL_REGISTER_ARCHIVE_DWA2006521_HPP
# define BOOST_ARCHIVE_DETAIL_REGISTER_ARCHIVE_DWA2006521_HPP

namespace boost { namespace archive { namespace detail { 

// No instantiate_ptr_serialization overloads generated by
// BOOST_SERIALIZATION_REGISTER_ARCHIVE that lexically follow the call
// will be seen *unless* they are in an associated namespace of one of
// the arguments, so we pass one of these along to make sure this
// namespace is considered.  See temp.dep.candidate (14.6.4.2) in the
// standard.
struct adl_tag {};

template <class Archive, class Serializable>
struct ptr_serialization_support;

// We could've just used ptr_serialization_support, above, but using
// it with only a forward declaration causes vc6/7 to complain about a
// missing instantiate member, even if it has one.  This is just a
// friendly layer of indirection.
template <class Archive, class Serializable>
struct _ptr_serialization_support
  : ptr_serialization_support<Archive,Serializable>
{
    typedef int type;
};

// This function gets called, but its only purpose is to participate
// in overload resolution with the functions declared by
// BOOST_SERIALIZATION_REGISTER_ARCHIVE, below.
template <class Serializable>
void instantiate_ptr_serialization(Serializable*, int, adl_tag ) {}

// The function declaration generated by this macro never actually
// gets called, but its return type gets instantiated, and that's
// enough to cause registration of serialization functions between
// Archive and any exported Serializable type.  See also:
// boost/serialization/export.hpp
# define BOOST_SERIALIZATION_REGISTER_ARCHIVE(Archive)                  \
namespace boost { namespace archive { namespace detail {                \
                                                                        \
template <class Serializable>                                           \
BOOST_DEDUCED_TYPENAME _ptr_serialization_support<Archive, Serializable>::type  \
instantiate_ptr_serialization( Serializable*, Archive*, adl_tag );              \
                                                                        \
}}}

}}} // namespace boost::archive::detail

#endif // BOOST_ARCHIVE_DETAIL_INSTANTIATE_SERIALIZE_DWA2006521_HPP
