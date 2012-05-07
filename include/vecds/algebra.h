
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
// Affiliation: Polish Academy of Sciences
//
// Copyright (C) 2010 The vecds authors
//
// This  program is  free  software: you  can  redistribute it  and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
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


#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "math.h"
#include <qglobal.h>


/**
 * The structure <code>Int4</code> is a four-integer; ie. a set of
 * four integers.
 */

struct Int4
{
  int i1, i2, i3, i4;
  
  /**
   * Constructor. Create a four-integer and initial all integers to be
   * zero.
   */
  Int4 () : 
  i1(0), i2(0), i3(0), i4(0) {}
  
  Int4 (int i_1, int i_2, int i_3, int i_4) :
  i1(i_1), i2(i_2), i3(i_3), i4(i_4) {}
  
  int &operator[](unsigned int index) 
  {
    Q_ASSERT (index<4);
    return (&i1)[index];
  }
  
  const int &operator[](unsigned int index) const 
  {
    Q_ASSERT(index<4);
    return (&i1)[index];
  }
};

/**
 * The structure <code>Int3</code> is a three-integer; ie. a set of
 * three integers.
 */
struct Int3
{
  int i1, i2, i3;
  
  /**
   * Constructor. Create a three-integer and initial all integers to be
   * zero.
   */
  Int3 () : 
  i1(0), i2(0), i3(0) {}
  
  Int3 (int x_, int y_, int z_) : 
  i1(x_), i2(y_), i3(z_) {}
  
  int &operator[] (unsigned int index) 
  {
    Q_ASSERT (index<3);
    return (&i1)[index];
  }
  
  const int &operator[] (unsigned int index) const 
  {
    Q_ASSERT (index<3);
    return (&i1)[index];
  }
};

/**
 * The structure <code>Int2</code> is a two-integer; ie. a set of
 * two integers.
 *
 * Note, this should be replaced inline with std::vector<int>
 * this(size)!!!
 */
struct Int2
{
  int i1, i2;
  
  /**
   * Constructor. Create a two-integer and initial all integers to be
   * zero.
   */
  Int2 () : 
  i1(0), i2(0) {}
  
  Int2 (int x_, int y_) : 
  i1(x_), i2(y_) {}
  
  int &operator[] (unsigned int index) 
  {
    Q_ASSERT (index<2);
    return (&i1)[index];
  }
  
  const int &operator[] (unsigned int index) const 
  {
    Q_ASSERT (index<2);
    return (&i1)[index];
  }
};

#endif
