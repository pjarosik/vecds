

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



  }                              // namespace function

}                                // namespace vecds
