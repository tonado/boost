/*
 *
 * Copyright (c) 1998-2002
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
  *   FILE         states.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Declares internal state machine structures.
  */

#ifndef BOOST_REGEX_V4_STATES_HPP
#define BOOST_REGEX_V4_STATES_HPP

#ifdef __BORLANDC__
#  pragma option push -a8 -b -Vx -Ve -pc -w-8027
#endif

namespace boost{
namespace re_detail{

/*** mask_type *******************************************************
Whenever we have a choice of two alternatives, we use an array of bytes
to indicate which of the two alternatives it is possible to take for any
given input character.  If mask_take is set, then we can take the next 
state, and if mask_skip is set then we can take the alternative.
***********************************************************************/
enum mask_type
{
   mask_take = 1,
   mask_skip = 2,
   mask_any = mask_skip | mask_take,
   mask_all = mask_any
};

/*** helpers **********************************************************
These helpers let us use function overload resolution to detect whether
we have narrow or wide character strings:
***********************************************************************/
struct _narrow_type{};
struct _wide_type{};
template <class charT> struct is_byte;
template<>             struct is_byte<char>         { typedef _narrow_type width_type; };
template<>             struct is_byte<unsigned char>{ typedef _narrow_type width_type; };
template<>             struct is_byte<signed char>  { typedef _narrow_type width_type; };
template <class charT> struct is_byte               { typedef _wide_type width_type; };

/*** enum syntax_element_type ******************************************
Every record in the state machine falls into one of the following types:
***********************************************************************/
enum syntax_element_type
{
   // start of a marked sub-expression, or perl-style (?...) extension
   syntax_element_startmark = 0,
   // end of a marked sub-expression, or perl-style (?...) extension
   syntax_element_endmark = syntax_element_startmark + 1,
   // any sequence of literal characters
   syntax_element_literal = syntax_element_endmark + 1,
   // start of line assertion: ^
   syntax_element_start_line = syntax_element_literal + 1,
   // end of line assertion $
   syntax_element_end_line = syntax_element_start_line + 1,
   // match any character: .
   syntax_element_wild = syntax_element_end_line + 1,
   // end of expression: we have a match when we get here
   syntax_element_match = syntax_element_wild + 1,
   // perl style word boundary: \b
   syntax_element_word_boundary = syntax_element_match + 1,
   // perl style within word boundary: \B
   syntax_element_within_word = syntax_element_word_boundary + 1,
   // start of word assertion: \<
   syntax_element_word_start = syntax_element_within_word + 1,
   // end of word assertion: \>
   syntax_element_word_end = syntax_element_word_start + 1,
   // start of buffer assertion: \`
   syntax_element_buffer_start = syntax_element_word_end + 1,
   // end of buffer assertion: \'
   syntax_element_buffer_end = syntax_element_buffer_start + 1,
   // backreference to previously matched sub-expression
   syntax_element_backref = syntax_element_buffer_end + 1,
   // either a wide character set [..] or one with multicharacter collating elements:
   syntax_element_long_set = syntax_element_backref + 1,
   // narrow character set: [...]
   syntax_element_set = syntax_element_long_set + 1,
   // jump to a new state in the machine:
   syntax_element_jump = syntax_element_set + 1,
   // choose between two production states:
   syntax_element_alt = syntax_element_jump + 1,
   // a repeat
   syntax_element_rep = syntax_element_alt + 1,
   // match a combining character sequence
   syntax_element_combining = syntax_element_rep + 1,
   // perl style soft buffer end: \z
   syntax_element_soft_buffer_end = syntax_element_combining + 1,
   // perl style continuation: \G
   syntax_element_restart_continue = syntax_element_soft_buffer_end + 1,
   // single character repeats:
   syntax_element_dot_rep = syntax_element_restart_continue + 1,
   syntax_element_char_rep = syntax_element_dot_rep + 1,
   syntax_element_short_set_rep = syntax_element_char_rep + 1,
   syntax_element_long_set_rep = syntax_element_short_set_rep + 1, 
};

#ifdef BOOST_REGEX_DEBUG
// dwa 09/26/00 - This is needed to suppress warnings about an ambiguous conversion
std::ostream& operator<<(std::ostream&, syntax_element_type);
#endif

struct re_syntax_base;

/*** union offset_type ************************************************
Points to another state in the machine.  During machine construction
we use integral offsets, but these are converted to pointers before
execution of the machine.
***********************************************************************/
union offset_type
{
   re_syntax_base*   p;
   std::size_t       i;
};

/*** struct re_syntax_base ********************************************
Base class for all states in the machine.
***********************************************************************/
struct re_syntax_base
{
   syntax_element_type   type;         // what kind of state this is
   offset_type           next;         // next state in the machine
   unsigned int          can_be_null;  // true if we match a NULL string
};

/*** struct re_brace **************************************************
Base class for all states in the machine.
***********************************************************************/
struct re_brace : public re_syntax_base
{
   // The index to match, can be zero (don't mark the sub-expression)
   // or negative (for perl style (?...) extentions):
   int index;
};

/*** struct re_literal ************************************************
A string of literals, following this structure will be an 
array of characters: charT[length]
***********************************************************************/
struct re_literal : public re_syntax_base
{
   unsigned int length;
};

/*** struct re_set_long ***********************************************
A wide character set of characters, following this structure will be
an array of type charT:
First csingles null-terminated strings
Then 2 * cranges NULL terminated strings
Then cequivalents NULL terminated strings
***********************************************************************/
struct re_set_long : public re_syntax_base
{
   unsigned int            csingles, cranges, cequivalents;
   boost::uint_fast32_t    cclasses;
   bool                    isnot;
   bool                    singleton;
};

/*** struct re_set ****************************************************
A set of narrow-characters, matches any of _map which is none-zero
***********************************************************************/
struct re_set : public re_syntax_base
{
   unsigned char _map[256];
};

/*** struct re_jump ***************************************************
Jump to a new location in the machine (not next).
***********************************************************************/
struct re_jump : public re_syntax_base
{
   offset_type     alt;           // location to jump to
   unsigned char   _map[256];     // which characters can take the jump
};

/*** struct re_repeat *************************************************
Repeat a section of the machine
***********************************************************************/
struct re_repeat : public re_jump
{
   unsigned   min, max;  // min and max allowable repeats
   int        id;        // Unique identifier for this repeat
   bool       leading;   // True if this repeat is at the start of the machine (lets us optimize some searches)
   bool       greedy;    // True if this is a greedy repeat
};

/*** enum re_jump_size_type *******************************************
Provides compiled size of re_jump structure (allowing for trailing alignment).
We provide this so we know how manybytes to insert when constructing the machine
(The value of padding_mask is defined in regex_raw_buffer.hpp).
***********************************************************************/
enum re_jump_size_type
{
   re_jump_size = (sizeof(re_jump) + padding_mask) & ~(padding_mask),
   re_repeater_size = (sizeof(re_repeat) + padding_mask) & ~(padding_mask)
};

/*** proc re_is_set_member *********************************************
Forward declaration: we'll need this one later...
***********************************************************************/
template <class iterator, class charT, class traits_type, class Allocator>
iterator BOOST_REGEX_CALL re_is_set_member(iterator next, 
                          iterator last, 
                          const re_set_long* set_, 
                          const reg_expression<charT, traits_type, Allocator>& e);

} // namespace re_detail

} // namespace boost

#ifdef __BORLANDC__
#  pragma option pop
#endif

#endif
