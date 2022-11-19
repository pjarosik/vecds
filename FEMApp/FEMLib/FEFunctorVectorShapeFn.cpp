
#include "FEFunctorVectorShapeFn.h"

CFEFunctorVectorShapeFn::CFEFunctorVectorShapeFn(CFEFunctorVector *x):CFEFunctorVector("CFEFunctorVectorShapeFn"),xi(x)
{
	AddNode( xi );
}

CFEFunctorVectorShapeFn::CFEFunctorVectorShapeFn( const CFEFunctorVectorShapeFn &vf ):CFEFunctorVector( vf )
{
	CLONE_MEMBER( vf, xi )
}

CFEFunctorVectorShapeFn::~CFEFunctorVectorShapeFn(void)
{

}
