#pragma once

#include "FEFunctorMatrix.h"
#include "FEFunctorVectorElemDOFs.h"

class CFEFunctorVectorDisp :
	public CFEFunctorVector
{
public:
	
	CFEFunctorVectorDisp( CFEFunctorMatrix *n, CFEFunctorVectorElemDOFs *qe );
	virtual ~CFEFunctorVectorDisp(void);
	CFEFunctorVectorDisp( const CFEFunctorVectorDisp &vf );

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	
	CFEFunctorMatrix *N;
	CFEFunctorVectorElemDOFs *q;
};
