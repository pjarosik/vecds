#pragma once
#include "FEFunctorMatrix.h"

class CFEFunctorMatrixMaterialPlaneStressIzo :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixMaterialPlaneStressIzo( );
	virtual ~CFEFunctorMatrixMaterialPlaneStressIzo(void);
	CFEFunctorMatrixMaterialPlaneStressIzo( const CFEFunctorMatrixMaterialPlaneStressIzo &vf );
	CLONE_FN( CFEFunctorMatrixMaterialPlaneStressIzo )
	virtual void ManageDOFs( CFiniteElementInstance *fe1 ) { }

protected:

	virtual void Compute( CFiniteElementInstance *fe1 );
	
};