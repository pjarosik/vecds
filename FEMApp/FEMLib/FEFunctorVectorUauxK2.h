#pragma once

#include "FEFunctorVector.h"
#include "FEFunctorVectorLagrangeX.h"
#include "CrackInInterface.h"

class CFEFunctorVectorUauxK2 :
	public CFEFunctorVector
{
public:
	
	CFEFunctorVectorUauxK2( CCrackInInterface *c, CFEFunctorVectorLagrangeX *x  );
	virtual ~CFEFunctorVectorUauxK2(void);
	CFEFunctorVectorUauxK2( const CFEFunctorVectorUauxK2 &vf );
	CLONE_FN(CFEFunctorVectorUauxK2)
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:

	virtual void Compute(CFiniteElementInstance *fe1);

	CCrackInInterface *crack;
	CFEFunctorVectorLagrangeX *X ;
};
