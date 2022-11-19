#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixShapeNaturalGrad :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixShapeNaturalGrad( CFEFunctorVector *x );
	CFEFunctorMatrixShapeNaturalGrad( const CFEFunctorMatrixShapeNaturalGrad &mf );
	virtual ~CFEFunctorMatrixShapeNaturalGrad(void);
        CLONE_FN(CFEFunctorMatrixShapeNaturalGrad)
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:
	
    virtual void Compute( CFiniteElementInstance *fe1 ) { fe1->GetShapeFunctions().jacobian( *xi, *this ); }
	CFEFunctorVector *xi;
};
