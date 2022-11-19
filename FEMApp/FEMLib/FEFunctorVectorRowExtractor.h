#pragma once
#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorVectorRowExtractor :
	public CFEFunctorVector
{
public:
	
	CFEFunctorVectorRowExtractor( CFEFunctorMatrix *m, unsigned r, const string &nm );
	virtual ~CFEFunctorVectorRowExtractor(void);
	CFEFunctorVectorRowExtractor( const CFEFunctorVectorRowExtractor &vf );

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorMatrix *M;
	unsigned row;
};

