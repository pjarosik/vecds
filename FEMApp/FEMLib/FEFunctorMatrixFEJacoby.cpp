
#include "FEFunctorMatrixFEJacoby.h"

CFEFunctorMatrixFEJacoby::CFEFunctorMatrixFEJacoby( CFEFunctorVector *x ):CFEFunctorMatrix("CFEFunctorMatrixFEJacoby"),xi(x)
{
	AddNode( xi );
}

CFEFunctorMatrixFEJacoby::CFEFunctorMatrixFEJacoby( const CFEFunctorMatrixFEJacoby &mf ):CFEFunctorMatrix(mf)
{
	CLONE_MEMBER( mf, xi )
}

CFEFunctorMatrixFEJacoby::~CFEFunctorMatrixFEJacoby(void)
{
}
