// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST_IOFM_FormatObjects_StaticNaryOutput_HPP
#define BOOST_IOFM_FormatObjects_StaticNaryOutput_HPP
#  include <boost/outfmt/formatter.hpp>
#  include <boost/outfmt/getval.hpp>

   namespace boost { namespace io
   {
      template< typename FormatType, class Outputter >
      class static_nary_output: public formatter_t
                                <
                                   FormatType,
                                   static_nary_output< FormatType, Outputter >,
                                   boost::io::detail::pair_traits< FormatType >
                                >
      {
         public:
            typedef static_nary_output< FormatType, Outputter >      this_type;
            typedef typename formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >::traits_type
                                                                     traits_type;
         public:
            Outputter                  out;
         public: // nary-valued types
#        if !defined(BOOST_IOFM_NO_LIB_QUATERNION)
            template< typename T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const boost::math::quaternion< T > & h
                                  ) const
            {
               os << open();
            /*
               out( os, getval< 1 >( h )) << separator();
               out( os, getval< 2 >( h )) << separator();
               out( os, getval< 3 >( h )) << separator();
               return( out( os, getval< 4 >( h )) << close());
            */
               return( os << close());
            }
#        endif
#        if !defined(BOOST_IOFM_NO_LIB_OCTONION)
            template< typename T, class OutputStream >
            inline OutputStream & operator()
                                  (
                                     OutputStream & os,
                                     const boost::math::octonion< T > & o
                                  ) const 
            {
               os << open();
               out( os, o.R_component_1()) << separator();
               out( os, o.R_component_2()) << separator();
               out( os, o.R_component_3()) << separator();
               out( os, o.R_component_4()) << separator();
               out( os, o.R_component_5()) << separator();
               out( os, o.R_component_6()) << separator();
               out( os, o.R_component_7()) << separator();
               return( out( os, o.R_component_8()) << close());
            }
#        endif
         public:
            inline           static_nary_output()
            {
            }
            inline           static_nary_output( const static_nary_output & o ):
               formatter_t< FormatType, this_type, boost::io::detail::pair_traits< FormatType > >( o ),
               out( o.out )
            {
            }
            inline           static_nary_output( const Outputter & o ):
               out( o )
            {
            }
      };

      template< class FormatType >
      inline static_nary_output< FormatType >    naryfmtex()
      {
         static_nary_output< FormatType > out;
         return( out );
      }

      inline static_nary_output< char * >        naryfmt()
      {
         return( naryfmtex< char * >());
      }

      template< class Outputter >
      inline static_nary_output< typename Outputter::format_type, Outputter >
                                                 naryfmt
                                                 (
                                                    const Outputter & out
                                                 )
      {
         return( static_nary_output< typename Outputter::format_type, Outputter >( out ));
      }
   }}
#endif
