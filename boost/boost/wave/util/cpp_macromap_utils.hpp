/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Token sequence analysis and transformation helper functions
    
    http://www.boost.org/

    Copyright (c) 2001-2005 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(CPP_MACROMAP_UTIL_HPP_HK041119)
#define CPP_MACROMAP_UTIL_HPP_HK041119

#include <boost/assert.hpp>

#include <boost/wave/wave_config.hpp>
#include <boost/wave/token_ids.hpp>

///////////////////////////////////////////////////////////////////////////////
//
// This file contains the definition of several token sequence analyse 
// and transformation utility functions needed during macro handling.
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace util {

///////////////////////////////////////////////////////////////////////////////
namespace on_exit {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  On destruction pop the first element of the list given as the argument 
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContainerT>
    class pop_front {
    public:
        pop_front(ContainerT &list_) : list(list_) {}
        ~pop_front() { list.pop_front(); }
    
    private:
        ContainerT &list;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Append a given list to the list given as argument 
    //  On destruction pop the first element of the list given as argument 
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContainerT>
    class splice_pop_front {
    public:
        splice_pop_front(ContainerT &list_, ContainerT &queue) 
        :   list(list_) 
        {
            list.splice(list.end(), queue);
        }
        ~splice_pop_front() { list.pop_front(); }

    private:
        ContainerT &list;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  On destruction reset a referenced value to its initial state
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename TypeT>
    class reset {
    public:
        reset(TypeT &target_value_, TypeT new_value)
        :   target_value(target_value_), old_value(target_value_)
        {
            target_value_ = new_value;
        }
        ~reset() { target_value = old_value; }

    private:
        TypeT &target_value;
        TypeT old_value;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  On destruction assign the given iterator back
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT, typename UnputIteratorT>
    class assign {
    public:
        assign(IteratorT &it_, UnputIteratorT const &uit_) 
        :   it(it_), uit(uit_) {}
        ~assign() { it = uit.base(); }

    private:
        IteratorT &it;
        UnputIteratorT const &uit;
    };

    template <typename IteratorT>
    class assign<IteratorT, IteratorT> {
    public:
        assign(IteratorT &it_, IteratorT const &uit_) 
        :   it(it_), uit(uit_) {}
        ~assign() { it = uit; }

    private:
        IteratorT &it;
        IteratorT const &uit;
    };

///////////////////////////////////////////////////////////////////////////////
}   // namespace on_exit

///////////////////////////////////////////////////////////////////////////////
namespace impl {

///////////////////////////////////////////////////////////////////////////////
//
//  Test, whether a given identifier resolves to a predefined name
//
///////////////////////////////////////////////////////////////////////////////
template <typename StringT>
inline bool 
is_special_macroname (StringT const &name)
{
    if (name.size() < 7)
        return false;
        
    if ("defined" == name)
        return true;
        
    if ('_' == name[0] && '_' == name[1]) {
    StringT str = name.substr(2);
    
        if (str == "cplusplus"  || str == "STDC__" || 
            str == "TIME__"     || str == "DATE__" ||
            str == "LINE__"     || str == "FILE__" ||
            str == "INCLUDE_LEVEL__")
        {
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Test, whether two tokens are to be considered equal (different sequences
//  of whitespace are considered to be equal)
//
///////////////////////////////////////////////////////////////////////////////
template <typename TokenT>
inline bool 
token_equals(TokenT const &left, TokenT const &right)
{
    using namespace boost::wave;
    
#if BOOST_WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
    if (T_PARAMETERBASE == token_id(left) || 
        T_EXTPARAMETERBASE == token_id(left)) 
#else
    if (T_PARAMETERBASE == token_id(left))
#endif 
    {
    //  if the existing token is of type T_PARAMETERBASE, then the right token 
    //  must be of type T_IDENTIFIER or a keyword
    token_id id = token_id(right);
     
        return (T_IDENTIFIER == id || 
                IS_CATEGORY(id, KeywordTokenType) ||
                IS_EXTCATEGORY(id, OperatorTokenType|AltExtTokenType)) && 
            left.get_value() == right.get_value();
    }

    // if the left token has whitespace, the value is irrelevant
    return token_id(left) == token_id(right) && (
            IS_CATEGORY(left, WhiteSpaceTokenType) ||
            left.get_value() == right.get_value()
        );
}

///////////////////////////////////////////////////////////////////////////////
//
//  Tests, whether two macro definitions are equal
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline bool 
definition_equals(ContainerT const &definition, 
    ContainerT const &new_definition)
{
    typedef typename ContainerT::const_iterator const_iterator_type;
    
const_iterator_type first1 = definition.begin();
const_iterator_type last1 = definition.end();
const_iterator_type first2 = new_definition.begin();
const_iterator_type last2 = new_definition.end();
    
    while (first1 != last1 && token_equals(*first1, *first2)) {
    // skip whitespace, if both sequences have a whitespace next
    token_id id1 = next_token<const_iterator_type>::peek(first1, last1, false);
    token_id id2 = next_token<const_iterator_type>::peek(first2, last2, false);

        if (IS_CATEGORY(id1, WhiteSpaceTokenType) && 
            IS_CATEGORY(id2, WhiteSpaceTokenType)) 
        {
        // all consecutive whitespace tokens count as one whitespace
        // adjust first1 and first2 accordingly
            skip_whitespace(first1, last1);
            skip_whitespace(first2, last2);
        }
        else if (!IS_CATEGORY(id1, WhiteSpaceTokenType) && 
                 !IS_CATEGORY(id2, WhiteSpaceTokenType)) 
        {
            ++first1;
            ++first2;
        }
        else {
        // the sequences differ
            break;
        }
    }
    return (first1 == last1 && first2 == last2) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Tests, whether two given sets of macro parameters are equal
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline bool 
parameters_equal(ContainerT const &parameters, ContainerT const &new_parameters)
{
    if (parameters.size() != new_parameters.size())
        return false;   // different parameter count

    typedef typename ContainerT::const_iterator const_iterator_type;
    
const_iterator_type first1 = parameters.begin();
const_iterator_type last1 = parameters.end();
const_iterator_type first2 = new_parameters.begin();

    while (first1 != last1) {
    // parameters are different, if the corresponding tokens are different
        using namespace boost::wave;
        if (token_id(*first1) != token_id(*first2) ||
            (*first1).get_value() != (*first2).get_value())
        {
            break;
        }
        ++first1;
        ++first2;
    }
    return (first1 == last1) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Strip leading and trailing whitespace from the given token sequence
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline void
trim_replacement_list (ContainerT &replacement_list)
{
    using namespace boost::wave;

// strip leading whitespace
    if (replacement_list.size() > 0) {
    typename ContainerT::iterator end = replacement_list.end();
    typename ContainerT::iterator it = replacement_list.begin();
    
        while (it != end && IS_CATEGORY(*it, WhiteSpaceTokenType)) { 
            if (T_PLACEHOLDER != token_id(*it)) {
                typename ContainerT::iterator next = it;
                ++next;
                replacement_list.erase(it);
                it = next;
            }
            else {
                ++it;
            }
        }
    }
        
// strip trailing whitespace
    if (replacement_list.size() > 0) {
    typename ContainerT::reverse_iterator rend = replacement_list.rend();
    typename ContainerT::reverse_iterator rit = replacement_list.rbegin();
    
        while (rit != rend && IS_CATEGORY(*rit, WhiteSpaceTokenType)) 
            ++rit;

    typename ContainerT::iterator end = replacement_list.end();
    typename ContainerT::iterator it = rit.base();
    
        while (it != end && IS_CATEGORY(*it, WhiteSpaceTokenType)) { 
            if (T_PLACEHOLDER != token_id(*it)) {
                typename ContainerT::iterator next = it;
                ++next;
                replacement_list.erase(it);
                it = next;
            }
            else {
                ++it;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Remove all placeholder tokens from the given token sequence
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline void
remove_placeholders (ContainerT &replacement_list)
{
    using namespace boost::wave;

// strip leading whitespace
    if (replacement_list.size() > 0) {
    typename ContainerT::iterator end = replacement_list.end();
    typename ContainerT::iterator it = replacement_list.begin();
    
        while (it != end) {
            if (T_PLACEHOLDER == token_id(*it)) {
                typename ContainerT::iterator next = it;
                ++next;
                replacement_list.erase(it);
                it = next;
            }
            else {
                ++it;
            }
        }
        
    // remove all 'new' leading and trailing whitespace 
        trim_replacement_list(replacement_list);
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Remove all whitespace tokens on the left side of the given token sequence
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline void
trim_argument_left (ContainerT &argument)
{
    using namespace boost::wave;
    
// strip leading whitespace (should be only one token)
    if (argument.size() > 0 &&
        IS_CATEGORY(argument.front(), WhiteSpaceTokenType))
    {
        argument.pop_front();
    }
}
    
///////////////////////////////////////////////////////////////////////////////
//
//  Remove all whitespace tokens on the right side of the given token sequence
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline void
trim_argument_right (ContainerT &argument)
{
    using namespace boost::wave;
    
// strip trailing whitespace (should be only one token)
    if (argument.size() > 0 &&
        IS_CATEGORY(argument.back(), WhiteSpaceTokenType))
    {
        argument.pop_back();
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Remove all whitespace tokens on the keft and right sides of the given token 
//  sequence
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline void
trim_argument (ContainerT &argument)
{
    trim_argument_left(argument);
    trim_argument_right(argument);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Tests, whether the given token sequence consists out of whitespace only
//
///////////////////////////////////////////////////////////////////////////////
template <typename ContainerT>
inline bool
is_whitespace_only (ContainerT const &argument)
{
    using namespace cpplexer;
    
    typename ContainerT::const_iterator end = argument.end();
    for (typename ContainerT::const_iterator it = argument.begin();
          it != end; ++it)
    {
        if (!IS_CATEGORY(*it, WhiteSpaceTokenType))
            return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Skip forward to a given token
//
///////////////////////////////////////////////////////////////////////////////
template <typename IteratorT>
inline bool 
skip_to_token(IteratorT &it, IteratorT const &end, token_id id)
{
    using namespace boost::wave;
    if (token_id(*it) == id) 
        return true;
    if (++it == end) 
        return false;

    while (IS_CATEGORY(*it, WhiteSpaceTokenType) || 
            T_NEWLINE == token_id(*it)) 
    {
        if (++it == end)
            return false;
    }
    return token_id(*it) == id;
}

///////////////////////////////////////////////////////////////////////////////
//
//  Get the full name of a given macro name (concatenate the string 
//  representations of the single tokens).
//
///////////////////////////////////////////////////////////////////////////////
template <typename IteratorT>
inline std::string
get_full_name(IteratorT const &begin, IteratorT const &end)
{
    std::string full_name;
    for (IteratorT err_it = begin; err_it != end; ++err_it) 
        full_name += (*err_it).get_value().c_str();

    return full_name;
}

///////////////////////////////////////////////////////////////////////////////
//
//  The following predicate is used in conjunction with the remove_copy_if
//  algorithm to allow the detection of an eventually copied operator ##.
//  No removal is performed in any case.
//
///////////////////////////////////////////////////////////////////////////////
class find_concat_operator {
public:
    find_concat_operator(bool &found_) : found_concat(found_) {}
    
    template <typename TokenT>
    bool operator()(TokenT const &tok)
    {
        using namespace boost::wave;
        if (T_POUND_POUND == BASE_TOKEN(token_id(tok)))
            found_concat = true;
        return false;
    }

private:
    bool &found_concat;
};

///////////////////////////////////////////////////////////////////////////////
}   // namespace impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace util
}   // namespace wave
}   // namespace boost

#endif // !defined(CPP_MACROMAP_UTIL_HPP_HK041119)
