#ifndef FEFUNCTORSCALARCOMPLIANCE_H
#define FEFUNCTORSCALARCOMPLIANCE_H

#include "FEFunctorScalar.h"
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "FiniteElementInstance.h"

class CFEFunctorScalarCompliance : public CFEFunctorScalar
{
public:
    CFEFunctorScalarCompliance( const matrix &ms, CFEFunctorMatrix *fs, CFEFunctorVector *d );
    CFEFunctorScalarCompliance( const CFEFunctorScalarCompliance &vf );
    CLONE_FN(CFEFunctorScalarCompliance)

    virtual void ManageDOFs(CFiniteElementInstance *fe1) { }
protected:

    matrix k;
    CFEFunctorMatrix *K;
    CFEFunctorVector *q;

    virtual void Compute( CFiniteElementInstance *param  );
};

#endif // FEFUNCTORSCALARCOMPLIANCE_H
