#pragma once

#include "FEFunctorMatrix.h"
#include "FEFunctorVector.h"

class CFEFunctorMatrixTipFnGrad :
	public CFEFunctorMatrix
{

public:

	CFEFunctorMatrixTipFnGrad( CEnrichmentFunctions *e, CFEFunctorVector *X );
	CFEFunctorMatrixTipFnGrad( const CFEFunctorMatrixTipFnGrad &mf );
	virtual ~CFEFunctorMatrixTipFnGrad( void );
	virtual void ManageDOFs( CFiniteElementInstance *fe1 ) { }

protected:
	
	virtual void Compute( CFiniteElementInstance *fe1 ) { ef->GetGradients( (*X), *this ); }
	
	CEnrichmentFunctions *ef;
	CFEFunctorVector *X;

};
