#ifndef FEFUNCTORMATRIXDISPLACEMENTGRAD_H
#define FEFUNCTORMATRIXDISPLACEMENTGRAD_H
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixDisplacementGrad : public CFEFunctorMatrix
{
public:
    CFEFunctorMatrixDisplacementGrad( CFEFunctorMatrix *nx, CFEFunctorMatrix *j );
    CFEFunctorMatrixDisplacementGrad( const CFEFunctorMatrixDisplacementGrad &mf );
    virtual ~CFEFunctorMatrixDisplacementGrad(void);
    CLONE_FN( CFEFunctorMatrixDisplacementGrad )
    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

    virtual void Compute( CFiniteElementInstance *fe1 );
    CFEFunctorMatrix *Nx, *J;
};

#endif // FEFUNCTORMATRIXDISPLACEMENTGRAD_H
