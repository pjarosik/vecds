
#include "FEFunctorMatrixShapePlane.h"

CFEFunctorMatrixShapePlane::CFEFunctorMatrixShapePlane( CFEFunctorVector *n ):CFEFunctorMatrix("Plane shape matrix"),N( n )
{
	AddNode( N );
}

CFEFunctorMatrixShapePlane::CFEFunctorMatrixShapePlane( const CFEFunctorMatrixShapePlane &vf ):CFEFunctorMatrix(vf)
{
	CLONE_MEMBER( vf, N )
}

CFEFunctorMatrixShapePlane::~CFEFunctorMatrixShapePlane( void )
{

}

void CFEFunctorMatrixShapePlane::Compute( CFiniteElementInstance *fe1 )
{
	fv.SetDim( 2, 2 * N->GetDim() );
	unsigned k;
	matrix &sN=fv;
	for (k=0; k< N->GetDim(); k++)
	{
		sN( 0, 2 * k )		= (*N)[ k ];
		sN( 1, 2 * k + 1)	= (*N)[ k ];
	}
}