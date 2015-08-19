#ifndef GENER_H
#define GENER_H

#include "internal.h"
#include "atoms.h"
#include "lattice.h"
 
class Gener
{ 
 public:
  static int genLattice(const int nx0, const int ny0, const int nz0, const int nx1, const int ny1, const int nz1);
  
};  
#endif // GENER_H