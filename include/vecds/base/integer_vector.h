
// -------------------------------------------------------------------
//
// Author: Jan Cholewinski and Pawel Dluzewski (2010)
//
// Copyright (C) 2010 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify  it under the  terms of  the GNU  General Public  License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//					 
// -------------------------------------------------------------------


#ifndef INTEGER_VECTOR_H
#define INTEGER_VECTOR_H

#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

namespace vecds 
{

  /**
   * The IntVector is a dedicated class for vectors of integers. We
   * want this, because we would like to have special operations that
   * may not apply to an ordinary vector of integers.
   *
   * @author Toby D. Young
   */
  template <unsigned int rank>
    class IntVector
    {
    public:

                                  /**
				   * Constructor. Set all objects in
				   * the vector to zero if zero=true.
				   */
      IntVector (const bool zero = true);

                                 /**
				  * Initialize a vector with list
				  * <code>v</code> using a direct
				  * memory copy. The length of
				  * <code>this</code> vector after
				  * initialization will be equal to
				  * the length of the list
				  * <code>v</code>;
				  *
				  * @note A typical usage of this
				  * could be:
				  * <code>vecds::IntVector<3> V =
				  * {1.0, 1.1, 1.25}</code>.
				  */
      IntVector (std::initializer_list<int> list);

                                 /**
				  * Destructor.
				  */
      ~IntVector ();

                                 /**
				  * Read-write access to the index of
				  * this vector.
				  */
      int& operator () (const unsigned int index);

                                 /**
				  * Read only access to the index of
				  * this vector.
				  */
      int& operator () (const unsigned int index, ...) const;

    protected:

                                 /**
				  * Internal reference to the number
				  * of components this vector
				  * has. For internal reference this
				  * is just the length of the
				  * underlying data array structure.
				  */
      unsigned int length;

    private:

                                 /**
				  * Internal object denoting this
				  * vector data.
				  */
      int *vector_data;

    public:

                                 /**
				  * Clear the contents of this tensor.
				  */
      void clear (); 

                                 /**
				  * Return the size of this vector.
				  */
      unsigned int size () const;

                                 /**
				  * Read-write access operator to the
				  * underlying C array structure
				  * associated with this vector.
				  */
      inline const int* operator* () const
      {
	return (*this).vector_data;
      }

                                 /**
				  * Read only access operator to the
				  * underlying C array structure
				  * associated with this vector.
				  */
      inline int* operator* ()
      {
	return (*this).vector_data;
      }


    };                             // class IntVector


                                 /* The structure <code>Int4</code> is
				    a four-integer; ie. a set of four
				    integers */
  struct Int4
  {
    int i1, i2, i3, i4;
  
                                 /* Constructor. Create a four-integer
				    and initial all integers to be
				    zero. */
    Int4 () 
    : 
    i1 (0), i2 (0), i3 (0), i4 (0) 
    {}
  
                                 /* Constructor. Create a four-integer
				    and initialise it with the four
				    input integers. */
    Int4 (int i_1, int i_2, int i_3, int i_4) 
    :
    i1(i_1), i2(i_2), i3(i_3), i4(i_4) 
    {}
  
                                 /* Read/write operator */
    int &operator[] (unsigned int index) 
    {
      assert (index<4);
      return (&i1)[index];
    }
  
                                 /* Read only operator */
    const int &operator[] (unsigned int index) const 
    {
      assert (index<4);
      return (&i1)[index];
    }

  };                             // struct Int4

                                 /* The structure <code>Int3</code> is
				    a three-integer; ie. a set of
				    three integers. */
  struct Int3
  {
    int i1, i2, i3;
  
                                 /* Constructor. Create a
				    three-integer and initial all
				    integers to be zero. */
    Int3 () 
    : 
    i1(0), i2(0), i3(0) 
    {}

                                 /* Constructor. Create a
				    three-integer and initialise it to
				    the input integers. */  
    Int3 (int x_, int y_, int z_) 
    : 
    i1(x_), i2(y_), i3(z_) 
    {}
    
                                 /* Read/write operator */
    int &operator[] (unsigned int index) 
    {
      assert (index<3);
      return (&i1)[index];
    }
    
                                 /* Read only operator */
    const int &operator[] (unsigned int index) const 
    {
      assert (index<3);
      return (&i1)[index];
    }
  };

                                 /* The structure <code>Int2</code> is
				    a two-integer; ie. a set of two
				    integers. */
  struct Int2
  {
    int i1, i2;
  
                                 /* Constructor. Create a two-integer
				    and initial all integers to be
				    zero. */
    Int2 () 
    : 
    i1(0), i2(0) 
    {}
    
                                 /* Constructor. Create a two-integer
				    and initialise it to the input
				    integers. */  
    Int2 (int x_, int y_) 
    : 
    i1(x_), i2(y_) 
    {}

                                 /* Read/write operator */  
    int &operator[] (unsigned int index) 
    {
      assert (index<2);
      return (&i1)[index];
    }

                                 /* Read only operator */  
    const int &operator[] (unsigned int index) const 
    {
      assert (index<2);
      return (&i1)[index];
    }

  };                             // struct Int2


  /*-------------- Inline and Other Functions -----------------------*/
  template <unsigned int rank>
    inline
    unsigned int
    IntVector<rank>::size () const
    {
      return length;
    }

} // namespace vecds

#endif
