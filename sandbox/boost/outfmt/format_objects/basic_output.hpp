// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST_IOFM_FormatObjects_BasicOutput_HPP
#define BOOST_IOFM_FormatObjects_BasicOutput_HPP
   namespace boost { namespace io
   {
      class basic_output
      {
         public:
            typedef void                                             traits_type;
            typedef char *                                           format_type;
         public:
            template< typename T, class OutputStream >
            inline OutputStream & operator()( OutputStream & os, const T & value ) const
            {
               return( os << value );
            }
         public:
            template< typename T, class InputStream >
            inline bool                          read( InputStream & is, T & value ) const
            {
               is >> value;
               return( is.isgood());
            }
         public:
            inline           basic_output()
            {
            }
            inline           basic_output( const basic_output & )
            {
            }
      };

      inline basic_output basicfmt()
      {
         basic_output                  out;
         return( out );
      }
   }}
#endif
