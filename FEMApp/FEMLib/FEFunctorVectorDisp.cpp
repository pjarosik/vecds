
#include "FEFunctorVectorDisp.h"

CFEFunctorVectorDisp::CFEFunctorVectorDisp( CFEFunctorMatrix *n, CFEFunctorVectorElemDOFs *qe ):CFEFunctorVector("DispVector"),N(n),q(qe)
{
	AddNode( N );
	AddNode( q );
	mnames = N->GetColsNames();
}

CFEFunctorVectorDisp::CFEFunctorVectorDisp( const CFEFunctorVectorDisp &vf ):CFEFunctorVector(vf)
{
	CLONE_MEMBER( vf, N )
	CLONE_MEMBER( vf, q )
}

CFEFunctorVectorDisp::~CFEFunctorVectorDisp( void )
{

}

void CFEFunctorVectorDisp::Compute( CFiniteElementInstance *fe1 )
{

	fv =  (*N) * (*q);

}
