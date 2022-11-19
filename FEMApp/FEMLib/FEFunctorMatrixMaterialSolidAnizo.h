#pragma once
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixMaterialSolidAnizo :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixMaterialSolidAnizo(  );
	virtual ~CFEFunctorMatrixMaterialSolidAnizo(void);
	CFEFunctorMatrixMaterialSolidAnizo( const CFEFunctorMatrixMaterialSolidAnizo &vf );
	CLONE_FN( CFEFunctorMatrixMaterialSolidAnizo )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	
};