// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DETAIL_InputHelper_HPP
#define BOOST_IOFM_DETAIL_InputHelper_HPP
#  include <boost/noncopyable.hpp>

   namespace boost { namespace io { namespace detail
   {
      // helper class for handling input-related tasks

      template< class InputStream >
      class input_helper: public boost::noncopyable
      {
         public:
            typedef typename InputStream::char_type                  char_type;
            typedef typename InputStream::traits_type                traits_type;
         private:
            InputStream              & is;
         public:
            template< typename T >
            inline input_helper & operator>>( T & t )
            {
               is >> t;
               return( *this );
            }
         public:
            inline bool                          isgood() const
            {
               return( !!is );
            }
            inline bool                          getch( char_type & ch )
            {
               is.get( ch );
               return( isgood());
            }
            inline bool                          isspace( char_type ch ) const
            {
               return( std::isspace( ch, is.getloc()));
            }
         public:
            inline void                          skipws()
            {
               char_type               ch;
               while( getch( ch ) && isspace( ch ))
                  ;
               is.putback( ch );
            }
         public: // string and character matching
            inline bool                          match( const char_type * s )
            {
               return( match( s, s + traits_type::length( s )));
            }
            bool                                 match( char_type c )
            {
               char_type               ch;
               if( getch( ch ) && traits_type::eq( c, ch ))
                  return( true );

               is.putback( ch );
               is.setstate( std::ios_base::failbit );
               return( false );
            }
         private:
            bool                                 match( const char_type * first, const char_type * last )
            {
               skipws();

               // remove leading whitespace
               while(( first != last ) && isspace( *first ))
                  ++first;

               // remove trailing whitespace
               while(( first != last ) && isspace( *( last - 1 )))
                  --last;

               // compare
               while(( first != last ) && match( *first ))
                  ++first;

               return( first == last ); // did we get a match?
            }
         public: // constructors
            inline           input_helper( InputStream & in ): is( in )
            {
            }
      };
   }}}
#endif
