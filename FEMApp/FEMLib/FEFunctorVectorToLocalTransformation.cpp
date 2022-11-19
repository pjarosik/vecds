#include "FEFunctorVectorToLocalTransformation.h"


CFEFunctorVectorToLocalTransformation::CFEFunctorVectorToLocalTransformation( CFEFunctorMatrix *l, CFEFunctorVector *k ):CFEFunctorVector("CFEFunctorMatrixToGlobalTransformation"), L( l ), K( k )
{

    AddNode( K );
	AddNode( L );
	al = k->GetAllocator();
}

CFEFunctorVectorToLocalTransformation::CFEFunctorVectorToLocalTransformation( const CFEFunctorVectorToLocalTransformation &mf ):CFEFunctorVector(mf)
{
    CLONE_MEMBER( mf, K )
	CLONE_MEMBER( mf, L )
}


void CFEFunctorVectorToLocalTransformation::Compute( CFiniteElementInstance *fe1 )
{
	CFEFunctorMatrix &vL = (*L);
	fv = vL * (*K);

}

