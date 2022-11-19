#ifndef FEFUNCTORMATRIXAAA_H
#define FEFUNCTORMATRIXAAA_H

#include "FEFunctorMatrix.h"
#include "FEFunctorArray3.h"

class CFEFunctorMatrixAAA : public CFEFunctorMatrix
{
public:
    CFEFunctorMatrixAAA( CFEFunctorMatrix *du );
    CFEFunctorMatrixAAA( const CFEFunctorMatrixAAA &mf );
    virtual ~CFEFunctorMatrixAAA(void);
    CLONE_FN( CFEFunctorMatrixAAA )
    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

    mvector eps;
protected:
    CFEFunctorMatrix *Fe;
    CFEFunctorArray3 bb;
    virtual void Compute( CFiniteElementInstance *fe1 );

};

#endif // FEFUNCTORMATRIXAAA_H
