#pragma once

#include "FEFunctorMatrix.h"
#include "FEFunctorVectorStrain.h"

class CFEFunctorVectorStress :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorStress( CFEFunctorMatrix *d, CFEFunctorVector *ee );
	CFEFunctorVectorStress( CFEFunctorVectorStress &vf );
	virtual ~CFEFunctorVectorStress(void);
	CLONE_FN( CFEFunctorVectorStress )

	virtual void ManageDOFs(CFiniteElementInstance *fe1 ) { D->ManageDOFs( fe1 ); e->ManageDOFs( fe1 ); }

protected:
	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorMatrix *D;
	CFEFunctorVector *e;
};
