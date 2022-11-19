#ifndef FEFUNCTORMATRIXINPATTERN_H
#define FEFUNCTORMATRIXINPATTERN_H
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixInPattern :
        public CFEFunctorMatrix
{

public:

    CFEFunctorMatrixInPattern( CFEFunctorMatrix *p, CFEFunctorVector *a, CFEFunctorVector *f );
    CFEFunctorMatrixInPattern( const CFEFunctorMatrixInPattern &mf );
    virtual ~CFEFunctorMatrixInPattern(void);
    CLONE_FN(CFEFunctorMatrixInPattern)
    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

    virtual void Compute( CFiniteElementInstance *fe1 );
    CFEFunctorVector *xi, *fn_vector;
    CFEFunctorMatrix *pattern;

};
#endif
