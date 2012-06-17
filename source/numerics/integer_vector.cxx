
// -------------------------------------------------------------------
//
// Author: 
//    Jan Cholewinski 2010.
//    Pawel Dluzewski 2010. 
//    Toby D. Young 2010, 2012. 
//
// Copyright (C) 2010, 2012 The vecds authors
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
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


                                 // vecds includes
#include <vecds/numerics/integer_vector.h>

namespace vecds
{


  template <unsigned int rank>
  IntVector<rank>::IntVector (const bool zero)
    :
    length (rank),
    vector_data (new int[length])
  {
    if (zero) 
      clear ();
  }

  template <unsigned int rank>
  IntVector<rank>::IntVector (std::initializer_list<int> list) 
    :
    length (rank),
    vector_data (new int[length])
  {
    assert (length==list.size ());

    if (length != 0)
      std::copy (list.begin(), list.end(), this->vector_data);
  }

  template <unsigned int rank>
  IntVector<rank>::~IntVector ()
  {
                                 // Contents don't matter - blindly
                                 // free up the memory used by this
                                 // tensor.
    if (vector_data)
      delete[] vector_data;
    
    // vector_data = 0;
  }

  template <unsigned int rank>
  void
  IntVector<rank>::clear () 
  {
                                 // Zero out vector memory.
    std::memset (vector_data, int (0), sizeof (int)*length);
  }


}                                // namespace vecds

/*-------------- Explicit Instantiations -------------------------------*/
template class vecds::IntVector<2>;
template class vecds::IntVector<3>;
