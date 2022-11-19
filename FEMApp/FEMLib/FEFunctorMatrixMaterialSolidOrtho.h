#pragma once
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixMaterialSolidOrtho :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixMaterialSolidOrtho( );
	virtual ~CFEFunctorMatrixMaterialSolidOrtho(void);
	CFEFunctorMatrixMaterialSolidOrtho( const CFEFunctorMatrixMaterialSolidOrtho &vf );
	CLONE_FN( CFEFunctorMatrixMaterialSolidOrtho )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	
};