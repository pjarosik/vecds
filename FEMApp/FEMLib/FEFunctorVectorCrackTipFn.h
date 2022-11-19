#pragma once
#include "FEFunctorVector.h"
#include "EnrichmentFunctions.h"

class CFEFunctorVectorCrackTipFn :
	public CFEFunctorVector
{
public:
	CFEFunctorVectorCrackTipFn( CEnrichmentFunctions *e, CFEFunctorVector *X );
	CFEFunctorVectorCrackTipFn( const CFEFunctorVectorCrackTipFn &vf );
	virtual ~CFEFunctorVectorCrackTipFn( void );

	virtual void ManageDOFs( CFiniteElementInstance *fe1 )  {  }

protected:
	
	virtual void Compute( CFiniteElementInstance *fe1) { ef->GetValues( (*X), fv ); }
	CEnrichmentFunctions *ef;
	CFEFunctorVector *X;
};
