//  Boost string_algo library sequence_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_SEQUENCE_TRAITS_HPP
#define BOOST_STRING_SEQUENCE_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/string_algo/yes_no_type.hpp>

/*! \file
	Defines sequence_traits class. This class it used to determine semantic properties
	of containers. Algoritms can take advantage of this information.
*/

namespace boost {

    namespace string_algo {

//  sequence traits  -----------------------------------------------//

        //! native replace tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a container with this property.

            \return yes_type if the container has basic_string like native replace
            method.
        */
        no_type sequence_has_native_replace(...);

        //! stable iterators tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a container with this property.

			\return yes_type if the container's insert/replace/erase methods do not invalidate
            existing iterators.
        */
        no_type sequence_has_stable_iterators(...);                     

        //! const time insert tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a container with this property.

			\return yes_type if the container's insert method is working in constant time
        */
        no_type sequence_has_const_time_insert(...);                        

        //! const time erase tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a container with this property.

			\return yes_type if the container's erase method is working in constant time
        */
        no_type sequence_has_const_time_erase(...);                     
	

        //! sequence_traits
        /*!
            Traits from this class template are used by various algorithms to achieve
            better performance for specific containers.
            This class template provides a default selection of sequence properties. 
            New containers can supported eihter by specializing the whole class,
            or by providing a specific overrides for tester functions.

			Due to language restriction, it is not currently possible to specialize
			stl containers without including the stl header. To decrease overhead,
			needed by this inclusion, user can selectively include specialization 
			header for a specific container. They are located in boost/string_algo/stl
			directory. Alternatively she can include boost/string_algo/std_container_traits.hpp
			header which container specialization for all stl containers.
        */
        template< typename SequenceT >
        struct sequence_traits
        {
        private:
            static SequenceT* t;
            BOOST_STATIC_CONSTANT(bool, native_replace_=( 
                sizeof(sequence_has_native_replace(t))==sizeof(yes_type) ) );
            BOOST_STATIC_CONSTANT(bool, stable_iterators_=( 
                sizeof(sequence_has_stable_iterators(t))==sizeof(yes_type) ) );
            BOOST_STATIC_CONSTANT(bool, const_time_insert_=( 
                sizeof(sequence_has_const_time_insert(t))==sizeof(yes_type) ) );
            BOOST_STATIC_CONSTANT(bool, const_time_erase_=( 
                sizeof(sequence_has_const_time_erase(t))==sizeof(yes_type) ) );

        public:
            // Container features
            
			//! Does container have native replace member function?
			typedef ::boost::mpl::bool_<native_replace_> native_replace;
			//! Does container have stable iterators?
            typedef ::boost::mpl::bool_<stable_iterators_> stable_iterators;
			//! Does container have constant time insert member function?
            typedef ::boost::mpl::bool_<const_time_insert_> const_time_insert;
			//! Does container have constant time erase member function?
            typedef ::boost::mpl::bool_<const_time_erase_> const_time_erase;                
        };

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_SEQUENCE_TRAITS_HPP
