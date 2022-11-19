#include "AtomAssignment.h"

CAtomAssignment::CAtomAssignment( CAtom *a, CFiniteElementInstance *f, const mvector &x ):atom(a),fe(f),xi(x)
{
    //ctor
}

CAtomAssignment::~CAtomAssignment()
{
    //dtor
}
