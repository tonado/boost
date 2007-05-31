// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_OTHER_UNITS_HPP_INCLUDED
#define BOOST_UNITS_OTHER_UNITS_HPP_INCLUDED

#include <string>

#include <boost/units/config.hpp>
#include <boost/units/base_unit.hpp>
//#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/absolute.hpp>

#include <boost/units/systems/base_units/ampere_base_unit.hpp>
#include <boost/units/systems/base_units/biot_base_unit.hpp>
#include <boost/units/systems/base_units/candela_base_unit.hpp>
#include <boost/units/systems/base_units/celsius_base_unit.hpp>
#include <boost/units/systems/base_units/centimeter_base_unit.hpp>
#include <boost/units/systems/base_units/degree_base_unit.hpp>
#include <boost/units/systems/base_units/fahrenheit_base_unit.hpp>
#include <boost/units/systems/base_units/foot_base_unit.hpp>
#include <boost/units/systems/base_units/gradian_base_unit.hpp>
#include <boost/units/systems/base_units/gram_base_unit.hpp>
#include <boost/units/systems/base_units/hour_base_unit.hpp>
#include <boost/units/systems/base_units/inch_base_unit.hpp>
#include <boost/units/systems/base_units/kelvin_base_unit.hpp>
#include <boost/units/systems/base_units/kilogram_base_unit.hpp>
#include <boost/units/systems/base_units/meter_base_unit.hpp>
#include <boost/units/systems/base_units/mile_base_unit.hpp>
#include <boost/units/systems/base_units/minute_base_unit.hpp>
#include <boost/units/systems/base_units/mole_base_unit.hpp>
#include <boost/units/systems/base_units/radian_base_unit.hpp>
#include <boost/units/systems/base_units/second_base_unit.hpp>
#include <boost/units/systems/base_units/steradian_base_unit.hpp>
#include <boost/units/systems/base_units/yard_base_unit.hpp>

BOOST_UNITS_DEFINE_CONVERSION(boost::units::meter_base_unit, boost::units::foot_base_unit::unit_type, double, 0.3048);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::meter_base_unit, boost::units::inch_base_unit::unit_type, double, 25.4e-3);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::meter_base_unit, boost::units::yard_base_unit::unit_type, double, 0.9144);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::meter_base_unit, boost::units::mile_base_unit::unit_type, double, 1609.344);

BOOST_UNITS_DEFINE_CONVERSION(boost::units::mile_base_unit, boost::units::yard_base_unit::unit_type, double, 1760.0);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::mile_base_unit, boost::units::foot_base_unit::unit_type, double, 5280.0);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::mile_base_unit, boost::units::inch_base_unit::unit_type, double, 63360.0);

BOOST_UNITS_DEFINE_CONVERSION(boost::units::yard_base_unit, boost::units::foot_base_unit::unit_type, double, 3.0);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::yard_base_unit, boost::units::inch_base_unit::unit_type, double, 36.0);

BOOST_UNITS_DEFINE_CONVERSION(boost::units::foot_base_unit, boost::units::inch_base_unit::unit_type, double, 12.0);

BOOST_UNITS_DEFINE_CONVERSION(boost::units::hour_base_unit, boost::units::minute_base_unit::unit_type, double, 60.0);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::hour_base_unit, boost::units::second_base_unit::unit_type, double, 3600.0);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::minute_base_unit, boost::units::second_base_unit::unit_type, double, 60.0);

BOOST_UNITS_DEFINE_CONVERSION(boost::units::kelvin_base_unit, boost::units::celsius_base_unit::unit_type, one, one());
BOOST_UNITS_DEFINE_CONVERSION(boost::units::kelvin_base_unit, boost::units::fahrenheit_base_unit::unit_type, double, 9.0/5.0);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::celsius_base_unit, boost::units::fahrenheit_base_unit::unit_type, double, 9.0/5.0);

BOOST_UNITS_DEFINE_AFFINE_CONVERSION(boost::units::kelvin_base_unit::unit_type, boost::units::celsius_base_unit::unit_type, double, -273.15);
BOOST_UNITS_DEFINE_AFFINE_CONVERSION(boost::units::kelvin_base_unit::unit_type, boost::units::fahrenheit_base_unit::unit_type, double, -273.15 * 9.0 / 5.0 + 32.0);
BOOST_UNITS_DEFINE_AFFINE_CONVERSION(boost::units::celsius_base_unit::unit_type, boost::units::fahrenheit_base_unit::unit_type, double, 32.0);

BOOST_UNITS_DEFINE_CONVERSION(boost::units::radian_base_unit, boost::units::degree_base_unit::unit_type, double, 180/3.14159265358979323846);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::radian_base_unit, boost::units::gradian_base_unit::unit_type, double, 200/3.14159265358979323846);
BOOST_UNITS_DEFINE_CONVERSION(boost::units::degree_base_unit, boost::units::gradian_base_unit::unit_type, double, 10/9.);

#endif // BOOST_UNITS_OTHER_UNITS_HPP_INCLUDED
