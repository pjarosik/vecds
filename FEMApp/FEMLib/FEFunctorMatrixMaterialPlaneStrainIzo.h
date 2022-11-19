#pragma once
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixMaterialPlaneStrainIzo :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixMaterialPlaneStrainIzo(  );
	virtual ~CFEFunctorMatrixMaterialPlaneStrainIzo(void);
	CFEFunctorMatrixMaterialPlaneStrainIzo( const CFEFunctorMatrixMaterialPlaneStrainIzo &vf );
	CLONE_FN( CFEFunctorMatrixMaterialPlaneStrainIzo )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	
};