#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVectorLagrangeX.h"
#include "CrackInInterface.h"

class CFEFunctorMatrixGradUauxK1 :
	public CFEFunctorMatrix
{
public:
	
	CFEFunctorMatrixGradUauxK1( CCrackInInterface *c, CFEFunctorVectorLagrangeX *x  );
	virtual ~CFEFunctorMatrixGradUauxK1(void);
	CFEFunctorMatrixGradUauxK1( const CFEFunctorMatrixGradUauxK1 &vf );
	CLONE_FN( CFEFunctorMatrixGradUauxK1 )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);

	CCrackInInterface *crack;
	CFEFunctorVectorLagrangeX *X ;
};