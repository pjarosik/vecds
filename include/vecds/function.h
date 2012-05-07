
// -------------------------------------------------------------------
//
// Copyright (C) 2010 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify  it under the  terms of  the GNU  General Public  License as
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


#include <math.h>

#ifndef FUNCTION_H
#define FUNCTION_H


/**
 * This is a namespace that declares functions used in a variety of
 * places in this application.
 *
 * @author Toby D. Young 2010.
 *
 * @ingroup function
 */
namespace function
{

  /**
   * The heavyside step-function \f$H(x)\f$, also known as the unit
   * stpe-function, is defined as:
   *
   * \f[
   * H(x)=\left\{\begin{array}{cc}0&x<0\\1&x\geq0\end{array}\right.\quad. \f]
   *
   */
  inline const double heavyside (const double x)
  {
    return (x<0.) ? 0. : 1.;
  }

  /**
   * A process that exhibits a progression from small-to-large and
   * accelerates can sometimes be mapped using a Sigmoid
   * function. This is particuarily useful when a detailed description
   * of the process is lacking. The Sigmoid function \f$S(x)\f$ is
   * defined as:
   *
   * \f[ S(x)=\frac{1}{1+e^{-x}}\quad. \f]
   *
   */
  inline const double sigmoid (const double x)
  {
    return (1./(1. + exp (-x)));
  }
}

#endif
