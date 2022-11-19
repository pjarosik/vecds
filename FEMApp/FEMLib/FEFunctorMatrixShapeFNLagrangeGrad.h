#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixShapeFNLagrangeGrad :
	public CFEFunctorMatrix
{
public:
	CFEFunctorMatrixShapeFNLagrangeGrad( CFEFunctorVector *x );
	CFEFunctorMatrixShapeFNLagrangeGrad( const CFEFunctorMatrixShapeFNLagrangeGrad &mf );
	virtual ~CFEFunctorMatrixShapeFNLagrangeGrad(void);
	CLONE_FN( CFEFunctorMatrixShapeFNLagrangeGrad )
	virtual void ManageDOFs(CFiniteElementInstance *fe1) { }

protected:
	
    virtual void Compute( CFiniteElementInstance *fe1 ) { fe1->GetShapeFunctions().jacobian( (*xi), fv ); fe1->GetGlobalSFGradient( (*xi), fv ); }
	CFEFunctorVector *xi;
};
