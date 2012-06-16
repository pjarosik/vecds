
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
				  * Read-write access to the
				  * <code>i</code>th index of this
				  * vector.
				  */
      int& operator () (const unsigned int i);

                                 /**
				  * Read only access to the
				  * <code>i</code>th index of this
				  * vector.
				  */
      const int& operator () (const unsigned int i) const;

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

  /*-------------- Inline and Other Functions -----------------------*/
  template <unsigned int rank>
    inline
    unsigned int
    IntVector<rank>::size () const
    {
      return this->length;
    }

  template <unsigned int rank>
    inline 
    const int& 
    IntVector<rank>::operator () (const unsigned int i) const
    {
      assert (i < length);
      return vector_data[i];
    }
  
  template <unsigned int rank>
    inline 
    int& 
    IntVector<rank>::operator () (const unsigned int i) 
    {
      assert (i < length);
      return vector_data[i];
    }


} // namespace vecds

#endif
