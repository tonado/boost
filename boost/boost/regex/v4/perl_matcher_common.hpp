/*
 *
 * Copyright (c) 2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         perl_matcher_common.cpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Definitions of perl_matcher member functions that are 
  *                common to both the recursive and non-recursive versions.
  */

#ifndef BOOST_REGEX_V4_PERL_MATCHER_COMMON_HPP
#define BOOST_REGEX_V4_PERL_MATCHER_COMMON_HPP

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

#ifdef __BORLANDC__
#  pragma option push -w-8008 -w-8066
#endif

namespace boost{
namespace re_detail{

template <class BidiIterator, class Allocator, class traits>
perl_matcher<BidiIterator, Allocator, traits>::perl_matcher(BidiIterator first, BidiIterator end, 
   match_results<BidiIterator, Allocator>& what, 
   const basic_regex<char_type, traits>& e,
   match_flag_type f,
   BidiIterator b)
   :  m_result(what), base(first), last(end), 
      position(first), backstop(b), re(e), traits_inst(e.get_traits()), 
      m_independent(false), next_count(&rep_obj), rep_obj(&next_count)
{
   construct_init(e, f);
}

template <class BidiIterator, class Allocator, class traits>
void perl_matcher<BidiIterator, Allocator, traits>::construct_init(const basic_regex<char_type, traits>& e, match_flag_type f)
{ 
   typedef typename regex_iterator_traits<BidiIterator>::iterator_category category;
   
   if(e.empty())
   {
      // precondition failure: e is not a valid regex.
      std::invalid_argument ex("Invalid regular expression object");
      boost::throw_exception(ex);
   }
   pstate = 0;
   m_match_flags = f;
   icase = re.flags() & regex_constants::icase;
   estimate_max_state_count(static_cast<category*>(0));
   if(!(m_match_flags & (match_perl|match_posix)))
   {
      if((re.flags() & (regbase::main_option_type|regbase::no_perl_ex)) == 0)
         m_match_flags |= match_perl;
      else if((re.flags() & (regbase::main_option_type|regbase::emacs_ex)) == (regbase::basic_syntax_group|regbase::emacs_ex))
         m_match_flags |= match_perl;
      else
         m_match_flags |= match_posix;
   }
   if(m_match_flags & match_posix)
   {
      m_temp_match.reset(new match_results<BidiIterator, Allocator>());
      m_presult = m_temp_match.get();
   }
   else
      m_presult = &m_result;
#ifdef BOOST_REGEX_NON_RECURSIVE
   m_stack_base = 0;
   m_backup_state = 0;
#endif
   // find the value to use for matching word boundaries:
   const char_type w = static_cast<char_type>('w');
   m_word_mask = traits_inst.lookup_classname(&w, &w+1);
   // find bitmask to use for matching '.':
   match_any_mask = static_cast<unsigned char>((f & match_not_dot_newline) ? re_detail::test_not_newline : re_detail::test_newline);
}

template <class BidiIterator, class Allocator, class traits>
void perl_matcher<BidiIterator, Allocator, traits>::estimate_max_state_count(std::random_access_iterator_tag*)
{
   static const difference_type k = 100000;
   difference_type dist = boost::re_detail::distance(base, last);
   traits_size_type states = static_cast<traits_size_type>(re.size());
   states *= states;
   difference_type lim = ((std::numeric_limits<difference_type>::max)() - k) / states;
   if(dist >= lim)
      max_state_count = (std::numeric_limits<difference_type>::max)();
   else
      max_state_count = k + states * dist;
}

template <class BidiIterator, class Allocator, class traits>
void perl_matcher<BidiIterator, Allocator, traits>::estimate_max_state_count(void*)
{
   // we don't know how long the sequence is:
   max_state_count = BOOST_REGEX_MAX_STATE_COUNT;
}

#ifdef BOOST_REGEX_HAS_MS_STACK_GUARD
template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::protected_call(
   protected_proc_type proc)
{
   /*
   __try{
      return (this->*proc)();
   }__except(EXCEPTION_STACK_OVERFLOW == GetExceptionCode())
   {
      reset_stack_guard_page();
   }
   // we only get here after a stack overflow:
   raise_error<traits>(traits_inst, regex_constants::error_size);
   // and we never really get here at all:
   return false;
   */
   ::boost::re_detail::concrete_protected_call
      <perl_matcher<BidiIterator, Allocator, traits> >
      obj(this, proc);
   return obj.execute();

}
#endif

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match()
{
#ifdef BOOST_REGEX_HAS_MS_STACK_GUARD
   return protected_call(&perl_matcher<BidiIterator, Allocator, traits>::match_imp);
#else
   return match_imp();
#endif
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_imp()
{
   // initialise our stack if we are non-recursive:
#ifdef BOOST_REGEX_NON_RECURSIVE
   save_state_init init(&m_stack_base, &m_backup_state);
   used_block_count = BOOST_REGEX_MAX_BLOCKS;
#if !defined(BOOST_NO_EXCEPTIONS)
   try{
#endif
#endif

   // reset our state machine:
   position = base;
   search_base = base;
   state_count = 0;
   m_match_flags |= regex_constants::match_all;
   m_presult->set_size((m_match_flags & match_nosubs) ? 1 : re.mark_count(), search_base, last);
   m_presult->set_base(base);
   if(m_match_flags & match_posix)
      m_result = *m_presult;
   verify_options(re.flags(), m_match_flags);
   if(0 == match_prefix())
      return false;
   return m_result[0].second == last;

#if defined(BOOST_REGEX_NON_RECURSIVE) && !defined(BOOST_NO_EXCEPTIONS)
   }
   catch(...)
   {
      // unwind all pushed states, apart from anything else this
      // ensures that all the states are correctly destructed
      // not just the memory freed.
      while(unwind(true)){}
      throw;
   }
#endif
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find()
{
#ifdef BOOST_REGEX_HAS_MS_STACK_GUARD
   return protected_call(&perl_matcher<BidiIterator, Allocator, traits>::find_imp);
#else
   return find_imp();
#endif
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find_imp()
{
   static matcher_proc_type const s_find_vtable[7] = 
   {
      &perl_matcher<BidiIterator, Allocator, traits>::find_restart_any,
      &perl_matcher<BidiIterator, Allocator, traits>::find_restart_word,
      &perl_matcher<BidiIterator, Allocator, traits>::find_restart_line,
      &perl_matcher<BidiIterator, Allocator, traits>::find_restart_buf,
      &perl_matcher<BidiIterator, Allocator, traits>::match_prefix,
      &perl_matcher<BidiIterator, Allocator, traits>::find_restart_lit,
      &perl_matcher<BidiIterator, Allocator, traits>::find_restart_lit,
   };

   // initialise our stack if we are non-recursive:
#ifdef BOOST_REGEX_NON_RECURSIVE
   save_state_init init(&m_stack_base, &m_backup_state);
   used_block_count = BOOST_REGEX_MAX_BLOCKS;
#if !defined(BOOST_NO_EXCEPTIONS)
   try{
#endif
#endif

   state_count = 0;
   if((m_match_flags & regex_constants::match_init) == 0)
   {
      // reset our state machine:
      search_base = position = base;
      pstate = re.get_first_state();
      m_presult->set_size((m_match_flags & match_nosubs) ? 1 : re.mark_count(), base, last);
      m_presult->set_base(base);
      m_match_flags |= regex_constants::match_init;
   }
   else
   {
      // start again:
      search_base = position = m_result[0].second;
      // If last match was null and match_not_null was not set then increment
      // our start position, otherwise we go into an infinite loop:
      if(((m_match_flags & match_not_null) == 0) && (m_result.length() == 0))
      {
         if(position == last)
            return false;
         else 
            ++position;
      }
      // reset $` start:
      m_presult->set_size((m_match_flags & match_nosubs) ? 1 : re.mark_count(), search_base, last);
      //if((base != search_base) && (base == backstop))
      //   m_match_flags |= match_prev_avail;
   }
   if(m_match_flags & match_posix)
   {
      m_result.set_size(re.mark_count(), base, last);
      m_result.set_base(base);
   }

   verify_options(re.flags(), m_match_flags);
   // find out what kind of expression we have:
   unsigned type = (m_match_flags & match_continuous) ? 
      static_cast<unsigned int>(regbase::restart_continue) 
         : static_cast<unsigned int>(re.get_restart_type());

   // call the appropriate search routine:
   matcher_proc_type proc = s_find_vtable[type];
   return (this->*proc)();

#if defined(BOOST_REGEX_NON_RECURSIVE) && !defined(BOOST_NO_EXCEPTIONS)
   }
   catch(...)
   {
      // unwind all pushed states, apart from anything else this
      // ensures that all the states are correctly destructed
      // not just the memory freed.
      while(unwind(true)){}
      throw;
   }
#endif
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_prefix()
{
   m_has_partial_match = false;
   m_has_found_match = false;
   pstate = re.get_first_state();
   m_presult->set_first(position);
   restart = position;
   match_all_states();
   if(!m_has_found_match && m_has_partial_match && (m_match_flags & match_partial))
   {
      m_has_found_match = true;
      m_presult->set_second(last, 0, false);
      position = last;
   }
#ifdef BOOST_REGEX_MATCH_EXTRA
   if(m_has_found_match && (match_extra & m_match_flags))
   {
      //
      // we have a match, reverse the capture information:
      //
      for(unsigned i = 0; i < m_presult->size(); ++i)
      {
         typename sub_match<BidiIterator>::capture_sequence_type & seq = ((*m_presult)[i]).get_captures();
         std::reverse(seq.begin(), seq.end());
      }
   }
#endif
   if(!m_has_found_match)
      position = restart; // reset search postion
   return m_has_found_match;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_endmark()
{
   int index = static_cast<const re_brace*>(pstate)->index;
   if(index > 0)
   {
      if((m_match_flags & match_nosubs) == 0)
         m_presult->set_second(position, index);
   }
   else if((index < 0) && (index != -4))
   {
      // matched forward lookahead:
      pstate = 0;
      return true;
   }
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_literal()
{
   unsigned int len = static_cast<const re_literal*>(pstate)->length;
   const char_type* what = reinterpret_cast<const char_type*>(static_cast<const re_literal*>(pstate) + 1);
   //
   // compare string with what we stored in
   // our records:
   for(unsigned int i = 0; i < len; ++i, ++position)
   {
      if((position == last) || (traits_inst.translate(*position, icase) != what[i]))
         return false;
   }
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_start_line()
{
   if(position == backstop)
   {
      if((m_match_flags & match_prev_avail) == 0)
      {
         if((m_match_flags & match_not_bol) == 0)
         {
            pstate = pstate->next.p;
            return true;
         }
         return false;
      }
   }
   else if(m_match_flags & match_single_line)
      return false;

   // check the previous value character:
   BidiIterator t(position);
   --t;
   if(position != last)
   {
      if(is_separator(*t) && !((*t == static_cast<char_type>('\r')) && (*position == static_cast<char_type>('\n'))) )
      {
         pstate = pstate->next.p;
         return true;
      }
   }
   else if(is_separator(*t))
   {
      pstate = pstate->next.p;
      return true;
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_end_line()
{
   if(position != last)
   {
      if(m_match_flags & match_single_line)
         return false;
      // we're not yet at the end so *first is always valid:
      if(is_separator(*position))
      {
         if((position != backstop) || (m_match_flags & match_prev_avail))
         {
            // check that we're not in the middle of \r\n sequence
            BidiIterator t(position);
            --t;
            if((*t == static_cast<char_type>('\r')) && (*position == static_cast<char_type>('\n')))
            {
               return false;
            }
         }
         pstate = pstate->next.p;
         return true;
      }
   }
   else if((m_match_flags & match_not_eol) == 0)
   {
      pstate = pstate->next.p;
      return true;
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_wild()
{
   if(position == last) 
      return false;
   if(is_separator(*position) && ((match_any_mask & static_cast<const re_dot*>(pstate)->mask) == 0))
      return false;
   if((*position == char_type(0)) && (m_match_flags & match_not_dot_null))
      return false;
   pstate = pstate->next.p;
   ++position;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_match()
{
   if((m_match_flags & match_not_null) && (position == (*m_presult)[0].first))
      return false;
   if((m_match_flags & match_all) && (position != last))
      return false;
   if((m_match_flags & regex_constants::match_not_initial_null) && (position == search_base))
      return false;
   m_presult->set_second(position);
   pstate = 0;
   m_has_found_match = true;
   if((m_match_flags & match_posix) == match_posix)
   {
      m_result.maybe_assign(*m_presult);
      if((m_match_flags & match_any) == 0)
         return false;
   }
#ifdef BOOST_REGEX_MATCH_EXTRA
   if(match_extra & m_match_flags)
   {
      for(unsigned i = 0; i < m_presult->size(); ++i)
         if((*m_presult)[i].matched)
            ((*m_presult)[i]).get_captures().push_back((*m_presult)[i]);
   }
#endif
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_word_boundary()
{
   bool b; // indcates whether next character is a word character
   if(position != last)
   {
      // prev and this character must be opposites:
   #if defined(BOOST_REGEX_USE_C_LOCALE) && defined(__GNUC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ < 95)
      b = traits::isctype(*position, m_word_mask);
   #else
      b = traits_inst.isctype(*position, m_word_mask);
   #endif
   }
   else
   {
      b = (m_match_flags & match_not_eow) ? true : false;
   }
   if((position == backstop) && ((m_match_flags & match_prev_avail) == 0))
   {
      if(m_match_flags & match_not_bow)
         b ^= true;
      else
         b ^= false;
   }
   else
   {
      --position;
      b ^= traits_inst.isctype(*position, m_word_mask);
      ++position;
   }
   if(b)
   {
      pstate = pstate->next.p;
      return true;
   }
   return false; // no match if we get to here...
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_within_word()
{
   if(position == last)
      return false;
   // both prev and this character must be m_word_mask:
   if(traits_inst.isctype(*position, m_word_mask))
   {
      bool b;
      if((position == backstop) && ((m_match_flags & match_prev_avail) == 0)) 
         return false;
      else
      {
         --position;
         b = traits_inst.isctype(*position, m_word_mask);
         ++position;
      }
      if(b)
      {
         pstate = pstate->next.p;
         return true;
      }
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_word_start()
{
   if(position == last)
      return false; // can't be starting a word if we're already at the end of input
   if(!traits_inst.isctype(*position, m_word_mask))
      return false; // next character isn't a word character
   if((position == backstop) && ((m_match_flags & match_prev_avail) == 0))
   {
      if(m_match_flags & match_not_bow)
         return false; // no previous input
   }
   else
   {
      // otherwise inside buffer:
      BidiIterator t(position);
      --t;
      if(traits_inst.isctype(*t, m_word_mask))
         return false; // previous character not non-word
   }
   // OK we have a match:
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_word_end()
{
   if((position == backstop) && ((m_match_flags & match_prev_avail) == 0))
      return false;  // start of buffer can't be end of word
   BidiIterator t(position);
   --t;
   if(traits_inst.isctype(*t, m_word_mask) == false)
      return false;  // previous character wasn't a word character

   if(position == last)
   {
      if(m_match_flags & match_not_eow)
         return false; // end of buffer but not end of word
   }
   else
   {
      // otherwise inside buffer:
      if(traits_inst.isctype(*position, m_word_mask))
         return false; // next character is a word character
   }
   pstate = pstate->next.p;
   return true;      // if we fall through to here then we've succeeded
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_buffer_start()
{
   if((position != backstop) || (m_match_flags & match_not_bob))
      return false;
   // OK match:
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_buffer_end()
{
   if((position != last) || (m_match_flags & match_not_eob))
      return false;
   // OK match:
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_backref()
{
   // compare with what we previously matched:
   BidiIterator i = (*m_presult)[static_cast<const re_brace*>(pstate)->index].first;
   BidiIterator j = (*m_presult)[static_cast<const re_brace*>(pstate)->index].second;
   while(i != j)
   {
      if((position == last) || (traits_inst.translate(*position, icase) != traits_inst.translate(*i, icase)))
         return false;
      ++i;
      ++position;
   }
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_long_set()
{
   typedef typename traits::char_class_type char_class_type;
   // let the traits class do the work:
   if(position == last)
      return false;
   BidiIterator t = re_is_set_member(position, last, static_cast<const re_set_long<char_class_type>*>(pstate), re.get_data(), icase);
   if(t != position)
   {
      pstate = pstate->next.p;
      position = t;
      return true;
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_set()
{
   if(position == last)
      return false;
   if(static_cast<const re_set*>(pstate)->_map[static_cast<unsigned char>(traits_inst.translate(*position, icase))])
   {
      pstate = pstate->next.p;
      ++position;
      return true;
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_jump()
{
   pstate = static_cast<const re_jump*>(pstate)->alt.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_combining()
{
   if(position == last)
      return false;
   if(is_combining(traits_inst.translate(*position, icase)))
      return false;
   ++position;
   while((position != last) && is_combining(traits_inst.translate(*position, icase)))
      ++position;
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_soft_buffer_end()
{
   if(m_match_flags & match_not_eob)
      return false;
   BidiIterator p(position);
   while((p != last) && is_separator(traits_inst.translate(*p, icase)))++p;
   if(p != last)
      return false;
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_restart_continue()
{
   if(position == search_base)
   {
      pstate = pstate->next.p;
      return true;
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_backstep()
{
   std::ptrdiff_t maxlen = ::boost::re_detail::distance(backstop, position);
   if(maxlen < static_cast<const re_brace*>(pstate)->index)
      return false;
   std::advance(position, -static_cast<const re_brace*>(pstate)->index);
   pstate = pstate->next.p;
   return true;
}

template <class BidiIterator, class Allocator, class traits>
inline bool perl_matcher<BidiIterator, Allocator, traits>::match_assert_backref()
{
   // return true if marked sub-expression N has been matched:
   bool result = (*m_presult)[static_cast<const re_brace*>(pstate)->index].matched;
   pstate = pstate->next.p;
   return result;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::match_toggle_case()
{
   // change our case sensitivity:
   this->icase = static_cast<const re_case*>(pstate)->icase;
   pstate = pstate->next.p;
   return true;
}


template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find_restart_any()
{
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4127)
#endif
   const unsigned char* _map = re.get_map();
   while(true)
   {
      // skip everything we can't match:
      while((position != last) && !can_start(*position, _map, (unsigned char)mask_any) )
         ++position;
      if(position == last)
      {
         // run out of characters, try a null match if possible:
         if(re.can_be_null())
            return match_prefix();
         break;
      }
      // now try and obtain a match:
      if(match_prefix())
         return true;
      if(position == last)
         return false;
      ++position;
   }
   return false;
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find_restart_word()
{
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4127)
#endif
   // do search optimised for word starts:
   const unsigned char* _map = re.get_map();
   if((m_match_flags & match_prev_avail) || (position != base))
      --position;
   else if(match_prefix())
      return true;
   do
   {
      while((position != last) && traits_inst.isctype(*position, m_word_mask))
         ++position;
      while((position != last) && !traits_inst.isctype(*position, m_word_mask))
         ++position;
      if(position == last)
         break;

      if(can_start(*position, _map, (unsigned char)mask_any) )
      {
         if(match_prefix())
            return true;
      }
      if(position == last)
         break;
   } while(true);
   return false;
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find_restart_line()
{
   // do search optimised for line starts:
   const unsigned char* _map = re.get_map();
   if(match_prefix())
      return true;
   while(position != last)
   {
      while((position != last) && !is_separator(*position))
         ++position;
      if(position == last)
         return false;
      ++position;
      if(position == last)
      {
         if(re.can_be_null() && match_prefix())
            return true;
         return false;
      }

      if( can_start(*position, _map, (unsigned char)mask_any) )
      {
         if(match_prefix())
            return true;
      }
      if(position == last)
         return false;
      //++position;
   }
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find_restart_buf()
{
   if((position == base) && ((m_match_flags & match_not_bob) == 0))
      return match_prefix();
   return false;
}

template <class BidiIterator, class Allocator, class traits>
bool perl_matcher<BidiIterator, Allocator, traits>::find_restart_lit()
{
#if 0
   if(position == last)
      return false; // can't possibly match if we're at the end already

   unsigned type = (m_match_flags & match_continuous) ? 
      static_cast<unsigned int>(regbase::restart_continue) 
         : static_cast<unsigned int>(re.get_restart_type());

   const kmp_info<char_type>* info = access::get_kmp(re);
   int len = info->len;
   const char_type* x = info->pstr;
   int j = 0; 
   while (position != last) 
   {
      while((j > -1) && (x[j] != traits_inst.translate(*position, icase))) 
         j = info->kmp_next[j];
      ++position;
      ++j;
      if(j >= len) 
      {
         if(type == regbase::restart_fixed_lit)
         {
            std::advance(position, -j);
            restart = position;
            std::advance(restart, len);
            m_result.set_first(position);
            m_result.set_second(restart);
            position = restart;
            return true;
         }
         else
         {
            restart = position;
            std::advance(position, -j);
            if(match_prefix())
               return true;
            else
            {
               for(int k = 0; (restart != position) && (k < j); ++k, --restart)
                     {} // dwa 10/20/2000 - warning suppression for MWCW
               if(restart != last)
                  ++restart;
               position = restart;
               j = 0;  //we could do better than this...
            }
         }
      }
   }
   if((m_match_flags & match_partial) && (position == last) && j)
   {
      // we need to check for a partial match:
      restart = position;
      std::advance(position, -j);
      return match_prefix();
   }
#endif
   return false;
}

} // namespace re_detail

} // namespace boost

#ifdef __BORLANDC__
#  pragma option pop
#endif
#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif

