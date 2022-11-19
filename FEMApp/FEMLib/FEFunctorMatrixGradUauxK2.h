#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVectorLagrangeX.h"
#include "CrackInInterface.h"

class CFEFunctorMatrixGradUauxK2 :
	public CFEFunctorMatrix
{
public:
	
	CFEFunctorMatrixGradUauxK2( CCrackInInterface *c, CFEFunctorVectorLagrangeX *x  );
	virtual ~CFEFunctorMatrixGradUauxK2(void);
	CFEFunctorMatrixGradUauxK2( const CFEFunctorMatrixGradUauxK2 &vf );
	CLONE_FN(CFEFunctorMatrixGradUauxK2)
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);

	CCrackInInterface *crack;
	CFEFunctorVectorLagrangeX *X ;
};
