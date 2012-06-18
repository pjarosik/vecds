
// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski 2012  
//    Toby D. Young 2010, 2012
//
// Copyright (C) 2010, 2012 The vecds authors
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


#include <cassert>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>

#include <vecds/base/constant.h>

#ifndef FUNCTION_H
#define FUNCTION_H


namespace vecds
{

                                 /**
				  * Parameters that define...?
				  */
  struct params 
  {
    double be;
    double bz;
    double u0x;
    double u0y;
    double u0z;
  };


                                 /**
				  * This is a namespace that declares
				  * functions used in a variety of
				  * places in this application. 
				  */
  namespace function
  {

                                 /**
				  * The heavyside step-function
				  * \f$H(x)\f$, also known as the unit
				  * step-function, is defined as:
				  * \f[H(x)=\left\{\begin{array}{cc}0&x<0\\1&x\geq0\end{array}\right.\quad.\f]
				  */
    double heavyside (const double x);

                                 /**
				  * A process that exhibits a
				  * progression from small-to-large
				  * and accelerates can sometimes be
				  * mapped using a Sigmoid
				  * function. This is particuarily
				  * useful when a detailed description
				  * of the process is lacking. The
				  * Sigmoid function \f$S(x)\f$ is
				  * defined as:
				  * \f[S(x)=\frac{1}{1+e^{-x}}\quad.\f]
				  */
    double sigmoid (const double x);

                                 /**
				  * The Love function.
				  */
    int love (const gsl_vector *point, 
	      void             *parameters, 
	      gsl_vector       *result);
    
                                 /**
				  * Distortion function...
				  */
    int beta (const gsl_vector *x, 
	      void             *par, 
	      gsl_matrix       *jac);
    
                                 /**
				  * This calls the Love functionand
				  * the....?
				  */    
    int love_fdf (const gsl_vector *x, 
		  void             *par, 
		  gsl_vector       *result_funct, 
		  gsl_matrix       *jac);
    
  }                              // namespace function

}                                // namespace vecds

#endif
