// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_WAVENUMBER_DERIVED_DIMENSION_HPP
#define BOOST_UNITS_WAVENUMBER_DERIVED_DIMENSION_HPP

#include <boost/units/derived_dimension.hpp>
#include <boost/units/systems/base_dimensions/length_base_dimension.hpp>

namespace boost {

namespace units {

/// derived dimension for wavenumber : L^-1
typedef derived_dimension<length_base_dimension,-1>::type   wavenumber_dim;                 

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_WAVENUMBER_DERIVED_DIMENSION_HPP
