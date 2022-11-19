#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixShapePlane :
	public CFEFunctorMatrix
{

	CFEFunctorMatrixShapePlane( CFEFunctorVector *n );
	virtual ~CFEFunctorMatrixShapePlane(void);
	CFEFunctorMatrixShapePlane( const CFEFunctorMatrixShapePlane &vf );

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorVector *N;
};
