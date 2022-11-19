#pragma once

#include "FEFunctorVector.h"
#include "FEFunctorMatrix.h"

class CFEFunctorVectorColExtractor :
	public CFEFunctorVector
{
public:
	
	CFEFunctorVectorColExtractor( CFEFunctorMatrix *m, unsigned c, const string &nm );
	virtual ~CFEFunctorVectorColExtractor(void);
	CFEFunctorVectorColExtractor( const CFEFunctorVectorColExtractor &vf );

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorMatrix *M;
	unsigned col;
};
