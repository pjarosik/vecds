
#include "FEFunctorMatrixShapeNaturalGrad.h"

CFEFunctorMatrixShapeNaturalGrad::CFEFunctorMatrixShapeNaturalGrad(CFEFunctorVector *x):CFEFunctorMatrix("CFEFunctorMatrixShapeNaturalGrad"),xi(x)
{
	AddNode( xi );
}

CFEFunctorMatrixShapeNaturalGrad::CFEFunctorMatrixShapeNaturalGrad( const CFEFunctorMatrixShapeNaturalGrad &mf ):CFEFunctorMatrix(mf)
{
	CLONE_MEMBER( mf, xi )
}

CFEFunctorMatrixShapeNaturalGrad::~CFEFunctorMatrixShapeNaturalGrad(void)
{
}
