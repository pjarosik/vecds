
#include "FEFunctorVectorLagrangeX.h"

CFEFunctorVectorLagrangeX::CFEFunctorVectorLagrangeX( CFEFunctorVector *x ):CFEFunctorVector("CFEFunctorVectorLagrangeX"),xi( x )
{
	AddNode( xi );
	mnames.push_back( "X" );
	mnames.push_back( "Y" );
	mnames.push_back( "Z" );
}

CFEFunctorVectorLagrangeX::CFEFunctorVectorLagrangeX( const CFEFunctorVectorLagrangeX &vf ):CFEFunctorVector(vf)
{
	CLONE_MEMBER( vf, xi )
}

CFEFunctorVectorLagrangeX::~CFEFunctorVectorLagrangeX(void)
{

}

