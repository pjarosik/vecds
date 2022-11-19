#ifndef CATOMASSIGNMENT_H
#define CATOMASSIGNMENT_H

#include "../MolecularLib/Atom.h"
#include "../FEMLib/FiniteElementInstance.h"
#include "../MathLib/matrix.h"

class CAtomAssignment
{
    public:
        CAtomAssignment( CAtom *a, CFiniteElementInstance *f, const mvector &xi );
        ~CAtomAssignment( );

    CAtom *atom;
    CFiniteElementInstance *fe;
    mvector xi;


};

#endif // CATOMASSIGNMENT_H
