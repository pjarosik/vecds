
#include "FEFunctorVectorCrackTipFn.h"

CFEFunctorVectorCrackTipFn::CFEFunctorVectorCrackTipFn(CEnrichmentFunctions *e, CFEFunctorVector *x ):CFEFunctorVector("CFEFunctorVectorCrackTipFn"),ef(e),X(x)
{
	AddNode( X );
}

CFEFunctorVectorCrackTipFn::CFEFunctorVectorCrackTipFn( const CFEFunctorVectorCrackTipFn &vf ):CFEFunctorVector( vf )
{
	CLONE_MEMBER( vf, X );
}

CFEFunctorVectorCrackTipFn::~CFEFunctorVectorCrackTipFn(void)
{
}
