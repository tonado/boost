#ifndef  BOOST_SERIALIZATION_HASH_SET_HPP
#define BOOST_SERIALIZATION_HASH_SET_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// hash_set.hpp: serialization for stl hash_set templates

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>
#ifdef BOOST_HAS_HASH
#include BOOST_HASH_SET_HEADER

#include <boost/serialization/hash_collections_save_imp.hpp>
#include <boost/serialization/hash_collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost { 
namespace serialization {

template<
    class Archive, 
    class Key, 
    class HashFcn, 
    class EqualKey,
    class Allocator
>
inline void save(
    Archive & ar,
    const BOOST_STD_EXTENSION_NAMESPACE::hash_set<
        Key, HashFcn, EqualKey, Allocator
    > &t,
    const unsigned int file_version
){
    boost::serialization::stl::save_hash_collection<
        Archive, 
        BOOST_STD_EXTENSION_NAMESPACE::hash_set<
            Key, HashFcn, EqualKey, Allocator
        > 
    >(ar, t);
}

template<
    class Archive, 
    class Key, 
    class HashFcn, 
    class EqualKey,
    class Allocator
>
inline void load(
    Archive & ar,
    BOOST_STD_EXTENSION_NAMESPACE::hash_set<
        Key, HashFcn, EqualKey, Allocator
    > &t,
    const unsigned int file_version
){
    boost::serialization::stl::load_hash_collection<
        Archive,
        BOOST_STD_EXTENSION_NAMESPACE::hash_set<
            Key, HashFcn, EqualKey, Allocator
        >,
        boost::serialization::stl::archive_input_set<
            Archive, 
            BOOST_STD_EXTENSION_NAMESPACE::hash_set<
                Key, HashFcn, EqualKey, Allocator
            >
        >
    >(ar, t);
}

// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<
    class Archive, 
    class Key, 
    class HashFcn, 
    class EqualKey,
    class Allocator
>
inline void serialize(
    Archive & ar,
    BOOST_STD_EXTENSION_NAMESPACE::hash_set<
        Key, HashFcn, EqualKey, Allocator
    > &t,
    const unsigned int file_version
){
    boost::serialization::split_free(ar, t, file_version);
}

// hash_multiset
template<
    class Archive, 
    class Key, 
    class HashFcn, 
    class EqualKey,
    class Allocator
>
inline void save(
    Archive & ar,
    const BOOST_STD_EXTENSION_NAMESPACE::hash_multiset<
        Key, HashFcn, EqualKey, Allocator
    > &t,
    const unsigned int file_version
){
    boost::serialization::stl::save_hash_collection<
        Archive, 
        BOOST_STD_EXTENSION_NAMESPACE::hash_multiset<
            Key, HashFcn, EqualKey, Allocator
        > 
    >(ar, t);
}

template<
    class Archive, 
    class Key, 
    class HashFcn, 
    class EqualKey,
    class Allocator
>
inline void load(
    Archive & ar,
    BOOST_STD_EXTENSION_NAMESPACE::hash_multiset<
        Key, HashFcn, EqualKey, Allocator
    > &t,
    const unsigned int file_version
){
    boost::serialization::stl::load_hash_collection<
        Archive,
        BOOST_STD_EXTENSION_NAMESPACE::hash_multiset<
            Key, HashFcn, EqualKey, Allocator
        >,
        boost::serialization::stl::archive_input_multiset<
            Archive,
            BOOST_STD_EXTENSION_NAMESPACE::hash_multiset<
                Key, HashFcn, EqualKey, Allocator
            > 
        >
    >(ar, t);
}

// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<
    class Archive, 
    class Key, 
    class HashFcn, 
    class EqualKey,
    class Allocator
>
inline void serialize(
    Archive & ar,
    BOOST_STD_EXTENSION_NAMESPACE::hash_multiset<
        Key, HashFcn, EqualKey, Allocator
    > & t,
    const unsigned int file_version
){
    boost::serialization::split_free(ar, t, file_version);
}

} // namespace serialization
} // namespace boost

#include <boost/serialization/collection_traits.hpp>

BOOST_SERIALIZATION_COLLECTION_TRAITS(BOOST_STD_EXTENSION_NAMESPACE::hash_set)
BOOST_SERIALIZATION_COLLECTION_TRAITS(BOOST_STD_EXTENSION_NAMESPACE::hash_multiset)

#endif // BOOST_HAS_HASH
#endif // BOOST_SERIALIZATION_HASH_SET_HPP
