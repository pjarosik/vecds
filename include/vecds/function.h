
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Toby D. Young.
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

#ifndef FUNCTION_H
#define FUNCTION_H


namespace vecds
{

                                 /* This is a namespace that declares
				    functions used in a variety of
				    places in this application. */
  namespace function
  {

                                 /* The heavyside step-function
				    \f$H(x)\f$, also known as the unit
				    step-function, is defined as:
				    \f[H(x)=\left\{\begin{array}{cc}0&x<0\\1&x\geq0\end{array}\right.\quad.\f] */
    inline double heavyside (const double x)
    {
      return (x<0.) ? 0. : 1.;
    }

                                 /* A process that exhibits a
				    progression from small-to-large
				    and accelerates can sometimes be
				    mapped using a Sigmoid
				    function. This is particuarily
				    useful when a detailed description
				    of the process is lacking. The
				    Sigmoid function \f$S(x)\f$ is
				    defined as: \f[
				    S(x)=\frac{1}{1+e^{-x}}\quad.\f] */
    inline double sigmoid (const double x)
    {
      return (1./(1. + exp (-x)));
    }

                                 /* The love function */
    inline int love (const gsl_vector *x, 
		     void             *par, 
		     gsl_vector       *result) 
    {
      const double rad_fact = 1.;
      const double nu       = 0.35;
      const double be       = ((struct params *) par)->be;
      const double bz       = ((struct params *) par)->bz;
      const double u0x      = ((struct params *) par)->u0x;
      const double u0y      = ((struct params *) par)->u0y;
      const double u0z      = ((struct params *) par)->u0z;
      
      const double x_distance = gsl_vector_get (x, 0);
      const double y_distance = gsl_vector_get (x, 1);
      const double r2         = x_distance*x_distance + y_distance*y_distance;
      const double r          = sqrt (r2);
      const double xx         = x_distance/r;
      const double yy         = y_distance/r;

                                 /* radius of an inmobile ring
				    relative to which the atoms in the
				    core move up */
      const double r02 = rad_fact*be*be; 

      const double ux = u0x - be /(2.*vecds::constant::pi)*(atan2(yy,xx)+xx*yy/(2.*(1.-nu)));
      const double uy = u0y + be /(8.*vecds::constant::pi*(1.-nu)) * ((1.-nu-nu)*log(r2/r02) + (xx+yy)*(xx-yy));
      const double uz = u0z - bz /(2.*vecds::constant::pi)*atan2(yy, xx);
      
      gsl_vector_set (result, 0, ux);
      gsl_vector_set (result, 1, uy);
      gsl_vector_set (result, 2, uz);

      return GSL_SUCCESS;
    }

                                 /* distortion function */
    inline int beta (const gsl_vector *x, 
		     void             *par, 
		     gsl_matrix       *jac) 
    {
      double nu = 0.35;
      double be = ((struct params *) par)->be;
      double bz = ((struct params *) par)->bz;
      
      const double xx = gsl_vector_get(x, 0);
      const double yy = gsl_vector_get(x, 1);
      
      const double y2 = yy*yy;
      const double x2 = xx*xx;
      const double r2 = x2+y2;
      
      if (r2<1.e-15) 
	{
	  std::cout << " Atom in the center of dislocation core" << endl;

	  gsl_matrix_set (jac, 0, 0, 1.);
	  gsl_matrix_set (jac, 0, 1, 0.);
	  gsl_matrix_set (jac, 0, 2, 0.);
	  gsl_matrix_set (jac, 1, 0, 0.);
	  gsl_matrix_set (jac, 1, 1, 1.);
	  gsl_matrix_set (jac, 1, 2, 0.);
	  gsl_matrix_set (jac, 2, 0, 0.);
	  gsl_matrix_set (jac, 2, 1, 0.);
	  gsl_matrix_set (jac, 2, 2, 1.);
	} 
      else 
	{
	  const double a   = be/(4. * vecds::constant::pi * (1.-nu) * r2*r2);
	  const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);
	  const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);     
	  const double bzx = bz/(2.*vecds::constant::pi) * yy/r2;          
	  const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);    
	  const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2);
	  const double bzy = bz/(2.*vecds::constant::pi) * xx/r2;                         
	  
	  gsl_matrix_set (jac, 0, 0, bxx);
	  gsl_matrix_set (jac, 0, 1, bxy);
	  gsl_matrix_set (jac, 0, 2, 0.);
	  gsl_matrix_set (jac, 1, 0, byx);
	  gsl_matrix_set (jac, 1, 1, byy);
	  gsl_matrix_set (jac, 1, 2, 0.);
	  gsl_matrix_set (jac, 2, 0, bzx);
	  gsl_matrix_set (jac, 2, 1, bzy);
	  gsl_matrix_set (jac, 2, 2, 1.); 
	}
      
      return GSL_SUCCESS;
    }
    
    inline int love_fdf (const gsl_vector *x, 
			 void             *par, 
			 gsl_vector       *result_funct, 
			 gsl_matrix       *jac) 
    {
      unsigned int ierr = 0;

      ierr = love (x, par, result_funct);
      assert (ierr==1);

      ierr = beta (x, par, jac);
      assert (ierr==1);

      return GSL_SUCCESS;
    }


  }                              // namespace function

}                                // namespace vecds

#endif
