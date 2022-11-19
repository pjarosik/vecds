#pragma once
#include "FEFunctorVector.h"
#include "FEFunctorMatrixColDOF.h"


class CFEFunctorVectorStrain :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorStrain( CFEFunctorMatrixColDOF *b, CFEFunctorVector *qe );
	CFEFunctorVectorStrain( const CFEFunctorVectorStrain &vf );
	virtual ~CFEFunctorVectorStrain(void);

	CLONE_FN( CFEFunctorVectorStrain )

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { q->ManageDOFs(fe1); }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);
	CFEFunctorMatrixColDOF *B;
	CFEFunctorVector *q;
};
