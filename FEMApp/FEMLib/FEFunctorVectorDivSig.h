#ifndef CFEFUNCTORVECTORDIVSIG_H
#define CFEFUNCTORVECTORDIVSIG_H
#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorVectorDivSIg : public CFEFunctorVector
{
public:
    CFEFunctorVectorDivSIg( CFEFunctorMatrix *n, CFEFunctorVector *s );
    CFEFunctorVectorDivSIg( const CFEFunctorVectorDivSIg &mf );
    virtual ~CFEFunctorVectorDivSIg(void);
    CLONE_FN( CFEFunctorVectorDivSIg )
    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

    CFEFunctorMatrix *N;
    CFEFunctorVector *sig;

    virtual void Compute( CFiniteElementInstance *fe1 );


};

#endif // CFEFUNCTORVECTORDIVSIG_H
