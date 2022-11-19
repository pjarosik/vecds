#pragma once

#include "FEFunctorVector.h"
#include "FEFunctorMatrixColDOF.h"
#include "EnrichmentFunctions.h"
#include "DOF_Bx.h"
#include "DOF_By.h"

class CFEFunctorMatrixStrainDerivPlaneXFEM_T :
	public CFEFunctorMatrixColDOF
{
public:
	
	CFEFunctorMatrixStrainDerivPlaneXFEM_T( CEnrichmentFunctions *e, CFEFunctorVector *vf1, CFEFunctorVector *vf2, CFEFunctorMatrix *mf1, CFEFunctorMatrix *mf2 );
	CFEFunctorMatrixStrainDerivPlaneXFEM_T( const CFEFunctorMatrixStrainDerivPlaneXFEM_T &mf );
	virtual ~CFEFunctorMatrixStrainDerivPlaneXFEM_T(void);

protected:

	CFEFunctorVector *N, *T;
	CFEFunctorMatrix *dN, *dT;
	CEnrichmentFunctions *ef;

	virtual void Compute( CFiniteElementInstance *fe1);
};
