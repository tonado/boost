// Boost tokenizer.hpp  ------------------------------------------------------//

// � Copyright Jeremy Siek and John R. Bandela 2001. 

// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all
// copies. This software is provided "as is" without express or
// implied warranty, and with no claim as to its suitability for any
// purpose.

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_TOKENIZER_JRB051801_HPP_
#define BOOST_TOKENIZER_JRB051801_HPP_

#include <boost/token_iterator.hpp>
#include <cassert>
namespace boost {

  
  //===========================================================================
  // A container-view of a tokenized "sequence"
  template <
	class TokenizerFunc = char_delimiters_separator<char>, 
	class Iterator = std::string::const_iterator,
	class Type = std::string
  >
  class tokenizer {
  private:
	typedef detail::tokenizer_policy<Type, TokenizerFunc> Pol;
	typedef detail::token_iterator_base<Iterator> TBase;
	typedef token_iterator_generator<TokenizerFunc,Iterator,Type> TGen;
	
	// It seems that MSVC does not like the unqualified use of iterator,
	// Thus we use iter internally when it is used unqualified and
	// the users of this class will always qualify iterator.	 
	typedef typename TGen::type iter;
	
  public:

	typedef iter iterator;
	typedef iter const_iterator;
    typedef Type value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const pointer const_pointer;
    typedef void size_type;
    typedef void difference_type;

    tokenizer(Iterator first, Iterator last,
		const TokenizerFunc& f = TokenizerFunc()) 
      :first_(first), last_(last), f_(f) { }
	
	template<class Container>
	tokenizer(const Container& c,const TokenizerFunc& f = TokenizerFunc())
	: first_(c.begin()), last_(c.end()), f_(f) { }

	void assign(Iterator first, Iterator last){
		first_ = first;
		last_ = last;
	}

	void assign(Iterator first, Iterator last, const TokenizerFunc& f){
		assign(first,last);
		f_ = f;
	}

	template<class Container>
	void assign(const Container& c){
		assign(c.begin(),c.end());
	}


	template<class Container>
	void assign(const Container& c, const TokenizerFunc& f){
		assign(c.begin(),c.end(),f);
	}
		

    iter begin() const { return iter(TBase(first_,last_),Pol(f_)); }
    iter end() const { return iter(TBase(last_,last_),Pol(f_)); }
	

  private:
    Iterator first_;
    Iterator last_;
    TokenizerFunc f_;
  };



} // namespace boost

#endif
