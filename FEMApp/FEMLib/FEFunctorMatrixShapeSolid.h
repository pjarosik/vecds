#pragma once
#include "FEFunctorVectorShapeFn.h"
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixShapeSolid :
	public CFEFunctorMatrix
{

	CFEFunctorMatrixShapeSolid( CFEFunctorVectorShapeFn *n );
	virtual ~CFEFunctorMatrixShapeSolid(void);
	CFEFunctorMatrixShapeSolid( const CFEFunctorMatrixShapeSolid &vf );
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }
	CLONE_FN(CFEFunctorMatrixShapeSolid)

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorVectorShapeFn *N;
};
