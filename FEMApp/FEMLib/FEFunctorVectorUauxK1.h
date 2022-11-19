#pragma once
#include "FEFunctorVector.h"
#include "FEFunctorVectorLagrangeX.h"
#include "CrackInInterface.h"

class CFEFunctorVectorUauxK1 :
	public CFEFunctorVector
{
public:
	
	CFEFunctorVectorUauxK1( CCrackInInterface *c, CFEFunctorVectorLagrangeX *x  );
	virtual ~CFEFunctorVectorUauxK1(void);
	CFEFunctorVectorUauxK1( const CFEFunctorVectorUauxK1 &vf );
	CLONE_FN(CFEFunctorVectorUauxK1)

	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);

	CCrackInInterface *crack;
	CFEFunctorVectorLagrangeX *X ;
};

