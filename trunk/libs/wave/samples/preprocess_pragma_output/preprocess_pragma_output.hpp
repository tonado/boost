/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    Example demonstrating how to preprocess the token stream generated by a
    #pragma directive
    
    http://www.boost.org/

    Copyright (c) 2001-2009 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_WAVE_SAMPLE_PREPROCESS_PRAGMA_OUTPUT_APR_03_2008_0813AM)
#define BOOST_WAVE_SAMPLE_PREPROCESS_PRAGMA_OUTPUT_APR_03_2008_0813AM

template <typename String, typename Iterator>
inline String
as_unescaped_string(Iterator it, Iterator const& end)
{
    using namespace boost::wave;
    
    String result;
    for (/**/; it != end; ++it) 
    {
        switch (token_id(*it)) {
        case T_STRINGLIT:
            {
                string val (util::impl::unescape_lit((*it).get_value()).c_str());
                val.erase(val.size()-1);
                val.erase(0, 1);
                result += val;
            }
            break;

        default:    // just skip everything else (hey it's a sample)
            break;
        }
    }
    return result;
}

// return the string representation of a token sequence
template <typename String, typename Container>
inline String
as_unescaped_string(Container const &token_sequence)
{
    return as_unescaped_string<String>(token_sequence.begin(), 
        token_sequence.end());
}

///////////////////////////////////////////////////////////////////////////////
//  
//  The preprocess_pragma_output_hooks policy class is used implement a special
//  #pragma wave pp("some C++ code") directive allowing to insert preprocessed
//  code into the output sequence generated by the tool.
//
//  This policy type is used as a template parameter to the boost::wave::context<>
//  object.
//
///////////////////////////////////////////////////////////////////////////////
class preprocess_pragma_output_hooks
:   public boost::wave::context_policies::default_preprocessing_hooks
{
public:
    preprocess_pragma_output_hooks() {}
    
    template <typename Context>
    struct reset_language_support
    {
        reset_language_support(Context& ctx)
          : ctx_(ctx), lang_(ctx.get_language())
        {
            ctx.set_language(boost::wave::enable_single_line(lang_), false);
        }
        ~reset_language_support()
        {
            ctx_.set_language(lang_, false);
        }
        
        Context& ctx_;
        boost::wave::language_support lang_;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    //  
    //  The function 'interpret_pragma' is called, whenever a #pragma command 
    //  directive is found which isn't known to the core Wave library, where
    //  command is the value defined as the BOOST_WAVE_PRAGMA_KEYWORD constant
    //  which defaults to "wave".
    //
    //  The parameter 'ctx' is a reference to the context object used for 
    //  instantiating the preprocessing iterators by the user.
    //
    //  The parameter 'pending' may be used to push tokens back into the input 
    //  stream, which are to be used as the replacement text for the whole 
    //  #pragma directive.
    //
    //  The parameter 'option' contains the name of the interpreted pragma.
    //
    //  The parameter 'values' holds the values of the parameter provided to 
    //  the pragma operator.
    //
    //  The parameter 'act_token' contains the actual #pragma token, which may 
    //  be used for error output.
    //
    //  If the return value is 'false', the whole #pragma directive is 
    //  interpreted as unknown and a corresponding error message is issued. A
    //  return value of 'true' signs a successful interpretation of the given 
    //  #pragma.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename Context, typename Container>
    bool 
    interpret_pragma(Context& ctx, Container &pending, 
        typename Context::token_type const& option, 
        Container const& values, typename Context::token_type const& act_token)
    {
        typedef typename Context::token_type token_type;
        typedef typename Context::iterator_type iterator_type;

        if (option.get_value() == "pp")  {
        // Concatenate the string(s) passed as the options to this pragma, 
        // preprocess the result using the current context and insert the 
        // generated token sequence in place of the pragma directive into the 
        // output stream.
         
            try {
            //  We're explicitly using a std::string here since the type of the
            //  iterators passed to the ctx.begin() below must match the types
            //  of the iterator the original context instance has been created 
            //  with.
                std::string s (as_unescaped_string<std::string>(values)); 
                reset_language_support<Context> lang(ctx);
                
                using namespace boost::wave;

            // The expanded token sequence is stored in the 'pragma' container
            // to ensure consistency in the output in the case of an error 
            // while preprocessing the pragma option strings.
                Container pragma;
                iterator_type end = ctx.end();
                for (iterator_type it = ctx.begin(s.begin(), s.end()); 
                     it != end && token_id(*it) != T_EOF; ++it) 
                {
                    pragma.push_back(*it);
                    it++;
                }

            // prepend the newly generated token sequence to the 'pending' 
            // container
                pending.splice(pending.begin(), pragma);
            }
            catch (boost::wave::preprocess_exception const& /*e*/) {
            // the library will report an 'ill_formed_pragma_option' for us
                return false;
            }
            return true;
        }

        // we don't know anything about this #pragma wave directive
        return false;   
    }
};


#endif

