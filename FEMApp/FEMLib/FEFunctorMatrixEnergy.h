#ifndef CFEFUNCTORMATRIXENERGY_H
#define CFEFUNCTORMATRIXENERGY_H

#include "FEFunctorMatrixAAA.h"

class CFEFunctorMatrixEnergy : public CFEFunctorMatrix
{
public:
    CFEFunctorMatrixEnergy( CFEFunctorMatrixAAA *du );
    CFEFunctorMatrixEnergy( const CFEFunctorMatrixEnergy &mf );
    virtual ~CFEFunctorMatrixEnergy(void);
    CLONE_FN( CFEFunctorMatrixEnergy )
    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

    CFEFunctorMatrixAAA *AAA;
    virtual void Compute( CFiniteElementInstance *fe1 );
    double chem( const matrix &xch, int nch, matrix &mu_x, CFEFunctorArray3 &mu_xx );

};

#endif // CFEFUNCTORMATRIXENERGY_H
