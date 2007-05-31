// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_DOSE_EQUIVALENT_HPP
#define BOOST_UNITS_SI_DOSE_EQUIVALENT_HPP

#include <boost/units/systems/si/base.hpp>
#include <boost/units/systems/derived_dimensions/dose_equivalent_derived_dimension.hpp>

namespace boost {

namespace units { 

namespace SI {

typedef unit<dose_equivalent_dimension,SI::system>     dose_equivalent;
    
BOOST_UNITS_STATIC_CONSTANT(sievert,dose_equivalent);  
BOOST_UNITS_STATIC_CONSTANT(sieverts,dose_equivalent); 

} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_DOSE_EQUIVALENT_HPP
