#ifndef BOOST_GEOMETRY_PROJECTIONS_OMERC_HPP
#define BOOST_GEOMETRY_PROJECTIONS_OMERC_HPP

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

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_tsfn.hpp>
#include <boost/geometry/extensions/gis/projections/impl/pj_phi2.hpp>

namespace boost { namespace geometry { namespace projection
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace omerc{
            static const double TOL = 1.e-7;
            static const double EPS = 1.e-10;

                inline double TSFN0(double x)
                    {return tan(.5 * (HALFPI - (x))); }


            struct par_omerc
            {
                double    A, B, E, AB, ArB, BrA, rB, singam, cosgam, sinrot, cosrot;
                double  v_pole_n, v_pole_s, u_0;
                int no_rot;
            };



            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_omerc_ellipsoid : public base_t_fi<base_omerc_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_omerc m_proj_parm;

                inline base_omerc_ellipsoid(const Parameters& par)
                    : base_t_fi<base_omerc_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double  Q, S, T, U, V, temp, u, v;

                    if (fabs(fabs(lp_lat) - HALFPI) > EPS) {
                        Q = this->m_proj_parm.E / pow(pj_tsfn(lp_lat, sin(lp_lat), this->m_par.e), this->m_proj_parm.B);
                        temp = 1. / Q;
                        S = .5 * (Q - temp);
                        T = .5 * (Q + temp);
                        V = sin(this->m_proj_parm.B * lp_lon);
                        U = (S * this->m_proj_parm.singam - V * this->m_proj_parm.cosgam) / T;
                        if (fabs(fabs(U) - 1.0) < EPS)
                            throw proj_exception();;
                        v = 0.5 * this->m_proj_parm.ArB * log((1. - U)/(1. + U));
                        temp = cos(this->m_proj_parm.B * lp_lon);
                        u = (fabs(temp) < TOL) ? this->m_proj_parm.AB * lp_lon :
                            this->m_proj_parm.ArB * atan2((S * this->m_proj_parm.cosgam + V * this->m_proj_parm.singam) , temp);
                    } else {
                        v = lp_lat > 0 ? this->m_proj_parm.v_pole_n : this->m_proj_parm.v_pole_s;
                        u = this->m_proj_parm.ArB * lp_lat;
                    }
                    if (this->m_proj_parm.no_rot) {
                        xy_x = u;
                        xy_y = v;
                    } else {
                        u -= this->m_proj_parm.u_0;
                        xy_x = v * this->m_proj_parm.cosrot + u * this->m_proj_parm.sinrot;
                        xy_y = u * this->m_proj_parm.cosrot - v * this->m_proj_parm.sinrot;
                    }
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double  u, v, Qp, Sp, Tp, Vp, Up;

                    if (this->m_proj_parm.no_rot) {
                        v = xy_y;
                        u = xy_x;
                    } else {
                        v = xy_x * this->m_proj_parm.cosrot - xy_y * this->m_proj_parm.sinrot;
                        u = xy_y * this->m_proj_parm.cosrot + xy_x * this->m_proj_parm.sinrot + this->m_proj_parm.u_0;
                    }
                    Qp = exp(- this->m_proj_parm.BrA * v);
                    Sp = .5 * (Qp - 1. / Qp);
                    Tp = .5 * (Qp + 1. / Qp);
                    Vp = sin(this->m_proj_parm.BrA * u);
                    Up = (Vp * this->m_proj_parm.cosgam + Sp * this->m_proj_parm.singam) / Tp;
                    if (fabs(fabs(Up) - 1.) < EPS) {
                        lp_lon = 0.;
                        lp_lat = Up < 0. ? -HALFPI : HALFPI;
                    } else {
                        lp_lat = this->m_proj_parm.E / sqrt((1. + Up) / (1. - Up));
                        if ((lp_lat = pj_phi2(pow(lp_lat, 1. / this->m_proj_parm.B), this->m_par.e)) == HUGE_VAL)
                            throw proj_exception();;
                        lp_lon = - this->m_proj_parm.rB * atan2((Sp * this->m_proj_parm.cosgam -
                            Vp * this->m_proj_parm.singam), cos(this->m_proj_parm.BrA * u));
                    }
                }
            };

            // Oblique Mercator
            template <typename Parameters>
            void setup_omerc(Parameters& par, par_omerc& proj_parm)
            {
                double con, com, cosph0, D, F, H, L, sinph0, p, J, gamma,
                    gamma0, lamc, lam1, lam2, phi1, phi2, alpha_c;
                int alp, gam, no_off = 0;
                proj_parm.no_rot = pj_param(par.params, "tno_rot").i;
                if ((alp = pj_param(par.params, "talpha").i))
                    alpha_c = pj_param(par.params, "ralpha").f;
                if ((gam = pj_param(par.params, "tgamma").i))
                    gamma = pj_param(par.params, "rgamma").f;
                if (alp || gam) {
                    lamc    = pj_param(par.params, "rlonc").f;
                    no_off = pj_param(par.params, "tno_off").i;
                } else {
                    lam1 = pj_param(par.params, "rlon_1").f;
                    phi1 = pj_param(par.params, "rlat_1").f;
                    lam2 = pj_param(par.params, "rlon_2").f;
                    phi2 = pj_param(par.params, "rlat_2").f;
                    if (fabs(phi1 - phi2) <= TOL ||
                        (con = fabs(phi1)) <= TOL ||
                        fabs(con - HALFPI) <= TOL ||
                        fabs(fabs(par.phi0) - HALFPI) <= TOL ||
                        fabs(fabs(phi2) - HALFPI) <= TOL) throw proj_exception(-33);
                }
                com = sqrt(par.one_es);
                if (fabs(par.phi0) > EPS) {
                    sinph0 = sin(par.phi0);
                    cosph0 = cos(par.phi0);
                    con = 1. - par.es * sinph0 * sinph0;
                    proj_parm.B = cosph0 * cosph0;
                    proj_parm.B = sqrt(1. + par.es * proj_parm.B * proj_parm.B / par.one_es);
                    proj_parm.A = proj_parm.B * par.k0 * com / con;
                    D = proj_parm.B * com / (cosph0 * sqrt(con));
                    if ((F = D * D - 1.) <= 0.)
                        F = 0.;
                    else {
                        F = sqrt(F);
                        if (par.phi0 < 0.)
                            F = -F;
                    }
                    proj_parm.E = F += D;
                    proj_parm.E *= pow(pj_tsfn(par.phi0, sinph0, par.e), proj_parm.B);
                } else {
                    proj_parm.B = 1. / com;
                    proj_parm.A = par.k0;
                    proj_parm.E = D = F = 1.;
                }
                if (alp || gam) {
                    if (alp) {
                        gamma0 = asin(sin(alpha_c) / D);
                        if (!gam)
                            gamma = alpha_c;
                    } else
                        alpha_c = asin(D*sin(gamma0 = gamma));
                    if ((con = fabs(alpha_c)) <= TOL ||
                        fabs(con - PI) <= TOL ||
                        fabs(fabs(par.phi0) - HALFPI) <= TOL)
                        throw proj_exception(-32);
                    par.lam0 = lamc - asin(.5 * (F - 1. / F) *
                       tan(gamma0)) / proj_parm.B;
                } else {
                    H = pow(pj_tsfn(phi1, sin(phi1), par.e), proj_parm.B);
                    L = pow(pj_tsfn(phi2, sin(phi2), par.e), proj_parm.B);
                    F = proj_parm.E / H;
                    p = (L - H) / (L + H);
                    J = proj_parm.E * proj_parm.E;
                    J = (J - L * H) / (J + L * H);
                    if ((con = lam1 - lam2) < -PI)
                        lam2 -= TWOPI;
                    else if (con > PI)
                        lam2 += TWOPI;
                    par.lam0 = adjlon(.5 * (lam1 + lam2) - atan(
                       J * tan(.5 * proj_parm.B * (lam1 - lam2)) / p) / proj_parm.B);
                    gamma0 = atan(2. * sin(proj_parm.B * adjlon(lam1 - par.lam0)) /
                       (F - 1. / F));
                    gamma = alpha_c = asin(D * sin(gamma0));
                }
                proj_parm.singam = sin(gamma0);
                proj_parm.cosgam = cos(gamma0);
                proj_parm.sinrot = sin(gamma);
                proj_parm.cosrot = cos(gamma);
                proj_parm.BrA = 1. / (proj_parm.ArB = proj_parm.A * (proj_parm.rB = 1. / proj_parm.B));
                proj_parm.AB = proj_parm.A * proj_parm.B;
                if (no_off)
                    proj_parm.u_0 = 0;
                else {
                    proj_parm.u_0 = fabs(proj_parm.ArB * atan2(sqrt(D * D - 1.), cos(alpha_c)));
                    if (par.phi0 < 0.)
                        proj_parm.u_0 = - proj_parm.u_0;
                }
                F = 0.5 * gamma0;
                proj_parm.v_pole_n = proj_parm.ArB * log(tan(FORTPI - F));
                proj_parm.v_pole_s = proj_parm.ArB * log(tan(FORTPI + F));
                // par.inv = e_inverse;
                // par.fwd = e_forward;
            }

        }} // namespace detail::omerc
    #endif // doxygen

    /*!
        \brief Oblique Mercator projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Cylindrical
         - Spheroid
         - Ell no_rot
         - alpha= [gamma=] [no_off] lonc= or
         - lon_1= lat_1= lon_2= lat_2=
        \par Example
        \image html ex_omerc.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct omerc_ellipsoid : public detail::omerc::base_omerc_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline omerc_ellipsoid(const Parameters& par) : detail::omerc::base_omerc_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::omerc::setup_omerc(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class omerc_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<omerc_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void omerc_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("omerc", new omerc_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail
    #endif // doxygen

}}} // namespace boost::geometry::projection

#endif // BOOST_GEOMETRY_PROJECTIONS_OMERC_HPP

