#ifndef BOOST_GEOMETRY_PROJECTIONS_URMFPS_HPP
#define BOOST_GEOMETRY_PROJECTIONS_URMFPS_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2011 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels (Geodan, Amsterdam)

// Original copyright notice:

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/concept_check.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projection
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace urmfps{
            static const double C_x = 0.8773826753;
            static const double Cy = 1.139753528477;

            struct par_urmfps
            {
                double    n, C_y;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_urmfps_spheroid : public base_t_fi<base_urmfps_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_urmfps m_proj_parm;

                inline base_urmfps_spheroid(const Parameters& par)
                    : base_t_fi<base_urmfps_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    lp_lat = aasin(this->m_proj_parm.n * sin(lp_lat));
                    xy_x = C_x * lp_lon * cos(lp_lat);
                    xy_y = this->m_proj_parm.C_y * lp_lat;
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    xy_y /= this->m_proj_parm.C_y;
                    lp_lat = aasin(sin(xy_y) / this->m_proj_parm.n);
                    lp_lon = xy_x / (C_x * cos(xy_y));
                }
            };

            template <typename Parameters>
            void setup(Parameters& par, par_urmfps& proj_parm)
            {
                boost::ignore_unused_variable_warning(par);
                boost::ignore_unused_variable_warning(proj_parm);
                proj_parm.C_y = Cy / proj_parm.n;
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            }


            // Urmaev Flat-Polar Sinusoidal
            template <typename Parameters>
            void setup_urmfps(Parameters& par, par_urmfps& proj_parm)
            {
                if (pj_param(par.params, "tn").i) {
                    proj_parm.n = pj_param(par.params, "dn").f;
                    if (proj_parm.n <= 0. || proj_parm.n > 1.)
                        throw proj_exception(-40);
                } else
                    throw proj_exception(-40);
                setup(par, proj_parm);
            }

            // Wagner I (Kavraisky VI)
            template <typename Parameters>
            void setup_wag1(Parameters& par, par_urmfps& proj_parm)
            {
                proj_parm.n = 0.8660254037844386467637231707;
                setup(par, proj_parm);
            }

        }} // namespace detail::urmfps
    #endif // doxygen

    /*!
        \brief Urmaev Flat-Polar Sinusoidal projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
         - n=
        \par Example
        \image html ex_urmfps.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct urmfps_spheroid : public detail::urmfps::base_urmfps_spheroid<Geographic, Cartesian, Parameters>
    {
        inline urmfps_spheroid(const Parameters& par) : detail::urmfps::base_urmfps_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::urmfps::setup_urmfps(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Wagner I (Kavraisky VI) projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_wag1.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct wag1_spheroid : public detail::urmfps::base_urmfps_spheroid<Geographic, Cartesian, Parameters>
    {
        inline wag1_spheroid(const Parameters& par) : detail::urmfps::base_urmfps_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::urmfps::setup_wag1(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class urmfps_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<urmfps_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class wag1_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<wag1_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void urmfps_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("urmfps", new urmfps_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("wag1", new wag1_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projection

#endif // BOOST_GEOMETRY_PROJECTIONS_URMFPS_HPP

