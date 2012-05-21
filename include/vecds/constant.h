
// -------------------------------------------------------------------
//
// Author: Toby D. Young (2010, 2012).
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#ifndef _CONSTANT_H
#define _CONSTANT_H

namespace vecds
{

                                 /* This is a namespace that declares
				    universal physical constants used
				    in a variety of places in this
				    application. Constants are not
				    restricted to being simply
				    scalars, for example \f$\pi\f$;
				    but may also be defined as vectors
				    or matrices, for example, the unit
				    matrix \f$I\f$). */
  namespace constant
  {
                                 /* Numerical value of
				    \f$\sqrt{2}\f$. */
    static const double sqrt2      = 1.4142135623730950488;

                                 /* Numerical value of
				    \f$\sqrt{3}\f$. */
    static const double sqrt3      =  1.7320508075688772935;

                                 /* Numerical value of \f$2\pi\f$. */
    static const double two_pi     =  6.28318530717958647688;

                                 /* Numerical value of \f$\pi\f$. */
    static const double pi         =  3.14159265358979323846;

                                 /* Numerical value of \f$\pi/2\f$. */
    static const double pi_2       =  1.57079632679489661922;

                                 /* Numerical value of \f$\pi/3\f$. */
    static const double pi_3       =  1.04719755119659774614;

                                 /* Numerical value of \f$1/\pi\f$. */
    static const double inv_pi     =  0.31830988618379067154;

                                 /* Numerical value of \f$\pi/180\f$. */
    static const double deg2rad    =  0.01745329251994329576;

                                 /* Numerical value of \f$180/\pi\f$. */
    static const double rad2deg    = 57.29577951308232087721;

                                 /* Numerical value of \f$e\f$. */
    static const double e          =  2.71828182845904523536;

                                 /* The unit matrix \f$I\f$. */
    static const Mat9d unit_matrix = Mat9d(1., 0., 0.,
					   0., 1., 0.,
					   0., 0., 1.);

  }                              // namespace constant

}                                // namespace vecds

#endif                           // _CONSTANT_H
