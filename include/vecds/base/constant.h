
// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software  Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along      with      this      program.       If      not,      see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------

#ifndef _CONSTANT_H
#define _CONSTANT_H

/**
 * This is a namespace that declares universal physical constants used
 * in a variety of places in this application. Constants are not
 * restricted to being simply scalars, for example \f$\pi\f$; but may
 * also be defined as vectors or matrices, for example, the unit
 * matrix \f$I\f$).
 *
 * A typical usage in the application code could, for example, compute
 * the area of a circle given the input dimension
 * <code>radius</code>. In other words to compute the area of a circle
 * \f$A=\pi r^2\f$; a code fragment in the application may look like
 * this:
 *
 * @verbatim
    const double area_of_a_circle (const double radius)
    {
       return constant::pi * radius * radius;
    }
   @endverbatim
 *
 * Similarly for the diameter of a circle we could write:
 *
 * @verbatim
    const double diameter_of_a_circle (const double radius)
    {
       return constant::pi * 2 * radius;
    }
   @endverbatim
 *
 * @note This namespace only contains <i>universal</i> constants and
 * <i>not</i> material parameters. All constants that pertain to
 * specific materials should be defined by the user at run-time or in
 * the input files.
 *
 * @author Toby D. Young and Janek Cholewinski 2010.
 *
 * @ingroup constant
 */
namespace constant
{
  /**
   * Numerical value of \f$\sqrt{2}\f$.
   */
  static const double sqrt2      = 1.4142135623730950488;

  /**
   * Numerical value of \f$\sqrt{3}\f$.
   */
  static const double sqrt3      =  1.7320508075688772935;

  /**
   * Numerical value of \f$2\pi\f$.
   */
  static const double two_pi     =  6.28318530717958647688;

  /**
   * Numerical value of \f$\pi\f$.
   */
  static const double pi         =  3.14159265358979323846;

  /**
   * Numerical value of \f$\pi/2\f$.
   */
  static const double pi_2       =  1.57079632679489661922;

  /**
   * Numerical value of \f$\pi/3\f$.
   */
  static const double pi_3       =  1.04719755119659774614;

  /**
   * Numerical value of \f$1/\pi\f$.
   */
  static const double inv_pi     =  0.31830988618379067154;

  /**
   * Numerical value of \f$\pi/180\f$.
   */
  static const double deg2rad    =  0.01745329251994329576;

  /**
   * Numerical value of \f$180/\pi\f$.
   */
  static const double rad2deg    = 57.29577951308232087721;

  /**
   * Numerical value of \f$e\f$.
   */
  static const double e          =  2.71828182845904523536;

  /**
   * The unit matrix \f$I\f$.
   */
  static const Mat9d unit_matrix = Mat9d(1., 0., 0.,
                                       0., 1., 0.,
                                       0., 0., 1.);
}

#endif // _CONSTANT_H
