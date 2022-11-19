
#include "FEFunctorVectorStrain.h"

CFEFunctorVectorStrain::CFEFunctorVectorStrain(CFEFunctorMatrixColDOF *b, CFEFunctorVector *qe):CFEFunctorVector("strain vector"), B( b ), q( qe )
{
	AddNode( B );
	AddNode( q );
	mnames = B->GetRowsNames( );
}

CFEFunctorVectorStrain::CFEFunctorVectorStrain( const CFEFunctorVectorStrain &vf ):CFEFunctorVector( vf )
{
	CLONE_MEMBER( vf, B )
	CLONE_MEMBER( vf, q )
}

CFEFunctorVectorStrain::~CFEFunctorVectorStrain(void)
{

}


void CFEFunctorVectorStrain::Compute(CFiniteElementInstance *fe1 )
{
	fv =  (*B) * (*q);
}