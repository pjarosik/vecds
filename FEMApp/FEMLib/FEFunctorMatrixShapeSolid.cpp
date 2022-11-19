
#include "FEFunctorMatrixShapeSolid.h"

CFEFunctorMatrixShapeSolid::CFEFunctorMatrixShapeSolid( CFEFunctorVectorShapeFn *n ):CFEFunctorMatrix("Solid shape matrix"),N( n )
{
	AddNode( N );
}

CFEFunctorMatrixShapeSolid::CFEFunctorMatrixShapeSolid( const CFEFunctorMatrixShapeSolid &vf ):CFEFunctorMatrix(vf)
{
	CLONE_MEMBER( vf, N )
}

CFEFunctorMatrixShapeSolid::~CFEFunctorMatrixShapeSolid( void )
{

}

void CFEFunctorMatrixShapeSolid::Compute( CFiniteElementInstance *fe1 )
{
	fv.SetDim( 3, 3 * N->GetDim() );
	unsigned k;
	matrix &sN=*this;
	for (k=0; k< N->GetDim(); k++)
	{
		sN( 0, 3 * k )		= (*N)[ k ];
		sN( 1, 3 * k + 1)	= (*N)[ k ];
		sN( 2, 3 * k + 2)	= (*N)[ k ];
	}
}