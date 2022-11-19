#pragma once
#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorVectorStraindU :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorStraindU( CFEFunctorMatrix *du );
	CFEFunctorVectorStraindU( const CFEFunctorVectorStraindU &vf );
	virtual ~CFEFunctorVectorStraindU(void);
	CLONE_FN( CFEFunctorVectorStraindU )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }
protected:

	virtual void Compute(CFiniteElementInstance *fe1);
		CFEFunctorMatrix *dU;
};