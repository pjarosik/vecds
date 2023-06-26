#ifndef INCLUDE_GLOBALS_H
#define INCLUDE_GLOBALS_H

#include "points.h"
#include "lattice.h"
#include "atoms.h"
#include "internal.h"

extern std::unique_ptr<Internal> INT;
extern std::unique_ptr<Lattice> LATT;
extern std::unique_ptr<Atoms> AT;
extern std::unique_ptr<Points> POINTS;

#endif //INCLUDE_GLOBALS_H
