#pragma once
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixMaterialSolidIzo :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixMaterialSolidIzo(  );
	virtual ~CFEFunctorMatrixMaterialSolidIzo(void);
	CFEFunctorMatrixMaterialSolidIzo( const CFEFunctorMatrixMaterialSolidIzo &vf );
	CLONE_FN( CFEFunctorMatrixMaterialSolidIzo )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	
};