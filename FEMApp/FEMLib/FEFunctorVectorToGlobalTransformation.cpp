#include "FEFunctorVectorToGlobalTransformation.h"


CFEFunctorVectorToGlobalTransformation::CFEFunctorVectorToGlobalTransformation( CFEFunctorMatrix *l, CFEFunctorVector *k ):CFEFunctorVector("CFEFunctorMatrixToGlobalTransformation"),L(l),K(k)
{

    AddNode( K );
	AddNode( L );
	al = k->GetAllocator();
}

CFEFunctorVectorToGlobalTransformation::CFEFunctorVectorToGlobalTransformation( const CFEFunctorVectorToGlobalTransformation &mf ):CFEFunctorVector( mf )
{
    CLONE_MEMBER( mf, K )
	CLONE_MEMBER( mf, L )
}


void CFEFunctorVectorToGlobalTransformation::Compute( CFiniteElementInstance *fe1 )
{
	fv = ~(*L) * (*K);
}

