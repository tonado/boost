// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_LUMINOUS_FLUX_HPP
#define BOOST_UNITS_SI_LUMINOUS_FLUX_HPP

#include <boost/units/systems/si/base.hpp>

namespace boost {

namespace units { 

namespace SI {

/// luminous flux : I QS
typedef composite_dimension<intensity_tag,1,solid_angle_tag,1>::type                    luminous_flux_type;

typedef unit<SI::luminous_flux_type,SI::system>                                         luminous_flux;

BOOST_UNITS_STATIC_CONSTANT(lumen,luminous_flux);   

} // namespace SI

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_LUMINOUS_FLUX_HPP
