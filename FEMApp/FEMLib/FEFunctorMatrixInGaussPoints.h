#pragma once
#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"
#include "FEFunctorVector.h"
#include "Integrator.h"
#include "IntegrationManager.h"

class CFEFunctorMatrixInGaussPoints :
	public CFEFunctorMatrix
{
    CFEFunctorVector *fn_vector,*xi;
public:
    CFEFunctorMatrixInGaussPoints( const CIntegrationManager &im , CFEFunctorVector *xi, CFEFunctorVector *i );
	CFEFunctorMatrixInGaussPoints( const CFEFunctorMatrixInGaussPoints &mf );
	virtual ~CFEFunctorMatrixInGaussPoints( void );
	CLONE_FN( CFEFunctorMatrixInGaussPoints )

	virtual void ManageDOFs( CFiniteElementInstance *fe1 );

protected:
	const CIntegrationManager &IM;
	virtual void Compute( CFiniteElementInstance *fe1 );
};
