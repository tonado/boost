// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNIT_SYSTEMS_IMPERIAL_POUND_HPP_INCLUDED
#define BOOST_UNIT_SYSTEMS_IMPERIAL_POUND_HPP_INCLUDED

#include <string>

#include <boost/units/config.hpp>
#include <boost/units/base_unit.hpp>
#include <boost/units/physical_dimensions/mass.hpp>
#include <boost/units/base_units/cgs/gram.hpp>
#include <boost/units/conversion.hpp>

// can't define in terms of kilogram because it is a scaled_base_unit
//BOOST_UNITS_DEFINE_BASE_UNIT_WITH_CONVERSIONS(imperial, pound, "pound", "lb", 0.45359237, si::kilogram_base_unit, -302);    // exact conversion
BOOST_UNITS_DEFINE_BASE_UNIT_WITH_CONVERSIONS(imperial, pound, "pound", "lb", 453.59237, cgs::gram_base_unit, -302);    // exact conversion

#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::units::imperial::pound_base_unit)

#endif

#endif // BOOST_UNIT_SYSTEMS_IMPERIAL_POUND_HPP_INCLUDED
