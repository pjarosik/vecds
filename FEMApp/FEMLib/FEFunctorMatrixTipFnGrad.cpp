
#include "FEFunctorMatrixTipFnGrad.h"

CFEFunctorMatrixTipFnGrad::CFEFunctorMatrixTipFnGrad( CEnrichmentFunctions *e, CFEFunctorVector *x ):CFEFunctorMatrix("CFEFunctorMatrixTipFnGrad"),ef(e),X(x)
{
	AddNode( X );
}

CFEFunctorMatrixTipFnGrad::CFEFunctorMatrixTipFnGrad( const CFEFunctorMatrixTipFnGrad &mf ):CFEFunctorMatrix(mf)
{
	CLONE_MEMBER( mf, X )
}

CFEFunctorMatrixTipFnGrad::~CFEFunctorMatrixTipFnGrad(void)
{
}
