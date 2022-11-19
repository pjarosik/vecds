#pragma once

#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "FEFunctorVectorStress.h"
#include "FEFunctorVectorStraindU.h"

class CFEFunctorMatrixdJint :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixdJint( CFEFunctorVectorStress *sa, CFEFunctorVectorStraindU *ea, CFEFunctorMatrix *dua, CFEFunctorVectorStress *se, CFEFunctorMatrix *du, CFEFunctorVector *dq );
	virtual ~CFEFunctorMatrixdJint(void);
	CFEFunctorMatrixdJint( const CFEFunctorMatrixdJint &vf );
	CLONE_FN(CFEFunctorMatrixdJint)
	
	virtual void ManageDOFs( CFiniteElementInstance *fe1  ) { }

protected:

	virtual void Compute( CFiniteElementInstance *fe1  );

	CFEFunctorVectorStress *s,*saux;
	CFEFunctorMatrix *dUaux, *dU;
	CFEFunctorVectorStraindU *eaux;
	CFEFunctorVector *dqn;

	mvector delta, dq;
};
