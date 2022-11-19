
#include "FEFunctorMatrixShapeFNLagrangeGrad.h"

CFEFunctorMatrixShapeFNLagrangeGrad::CFEFunctorMatrixShapeFNLagrangeGrad( CFEFunctorVector *x ):CFEFunctorMatrix("CFEFunctorMatrixShapeFNLagrangeGrad"),xi(x)
{
	AddNode( xi );
}

CFEFunctorMatrixShapeFNLagrangeGrad::CFEFunctorMatrixShapeFNLagrangeGrad( const CFEFunctorMatrixShapeFNLagrangeGrad &mf ):CFEFunctorMatrix(mf)
{
	CLONE_MEMBER( mf, xi )
}

CFEFunctorMatrixShapeFNLagrangeGrad::~CFEFunctorMatrixShapeFNLagrangeGrad(void)
{
}
