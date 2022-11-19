#ifndef FEFUNCTORMATRIXDIVSIGEPS_H
#define FEFUNCTORMATRIXDIVSIGEPS_H
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixDivSigEps : public CFEFunctorMatrix
{
public:
    CFEFunctorMatrixDivSigEps( CFEFunctorMatrix *N, CFEFunctorMatrix *s, CFEFunctorMatrix *b);

    CFEFunctorMatrixDivSigEps( const CFEFunctorMatrixDivSigEps &mf );
    virtual ~CFEFunctorMatrixDivSigEps(void);
    CLONE_FN( CFEFunctorMatrixDivSigEps )
    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }


protected:
    CFEFunctorMatrix *N, *srf, *df;
    virtual void Compute( CFiniteElementInstance *fe1 );
};

#endif // FEFUNCTORMATRIXDIVSIGEPS_H
