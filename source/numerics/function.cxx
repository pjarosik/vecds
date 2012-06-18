

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

#include <vecds/numerics/function.h>

namespace vecds
{

  namespace function
  {

    double heavyside (const double x)
    {
      return (x<0.) ? 0. : 1.;
    }

    double sigmoid (const double x)
    {
      return (1./(1. + exp (-x)));
    }

    int love (const gsl_vector *point, 
	      void             *parameters, 
	      gsl_vector       *result) 
    {
      const double radius_factor = 1.;

                                 // poissons ration
      const double nu       = 0.35;

      const double be       = ((struct params *) parameters)->be;
      const double bz       = ((struct params *) parameters)->bz;
      const double u0x      = ((struct params *) parameters)->u0x;
      const double u0y      = ((struct params *) parameters)->u0y;
      const double u0z      = ((struct params *) parameters)->u0z;
      
      const double x_distance    = gsl_vector_get (point, 0);
      const double y_distance    = gsl_vector_get (point, 1);
      const double radius_square = x_distance*x_distance + y_distance*y_distance;
      const double radius        = sqrt (radius_square);
      const double xx            = x_distance/radius;
      const double yy            = y_distance/radius;

                                 // radius of an inmobile ring
                                 // relative to which the atoms in the
                                 // core move up
      const double r02 = radius_factor*be*be; 

      const double ux = u0x - be / (2.*vecds::constant::pi) * (atan2 (yy,xx) + xx*yy/(2.*(1.-nu)));
      const double uy = u0y + be / (8.*vecds::constant::pi  * (1.-nu)) * ((1.-nu-nu)*log (radius_square/r02) + (xx+yy)*(xx-yy));
      const double uz = u0z - bz / (2.*vecds::constant::pi) * atan2 (yy, xx);
      
      gsl_vector_set (result, 0, ux);
      gsl_vector_set (result, 1, uy);
      gsl_vector_set (result, 2, uz);

      return GSL_SUCCESS;
    }


    int beta (const gsl_vector *x, 
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
	  const double a   = be / (4. * vecds::constant::pi * (1.-nu) * r2*r2);
	  const double bxx = 1. + a * yy * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);
	  const double byx = a * xx * ((1.-2.*nu)*x2 + (3.-2.*nu)*y2);     
	  const double bzx = bz / (2.*vecds::constant::pi) * yy/r2;          
	  const double bxy = -a * xx * ((3.-2.*nu)*x2 + (1.-2.*nu)*y2);    
	  const double byy = 1. - a * yy * ((1.+2.*nu)*x2 - (1.-2.*nu)*y2);
	  const double bzy = bz / (2.*vecds::constant::pi) * xx/r2;                         
	  
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
    
    int love_fdf (const gsl_vector *x, 
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
