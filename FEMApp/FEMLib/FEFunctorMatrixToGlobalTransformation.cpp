#include "FEFunctorMatrixToGlobalTransformation.h"


CFEFunctorMatrixToGlobalTransformation::CFEFunctorMatrixToGlobalTransformation( CFEFunctorMatrix *l, CFEFunctorMatrix *k ):CFEFunctorMatrix("CFEFunctorMatrixToGlobalTransformation"),L(l),K(k)
{

    AddNode( K );
	AddNode( L );
	alr = k->GetRowAllocator();
	alc = k->GetColAllocator();

}

CFEFunctorMatrixToGlobalTransformation::CFEFunctorMatrixToGlobalTransformation( const CFEFunctorMatrixToGlobalTransformation &mf ):CFEFunctorMatrix(mf)
{
    CLONE_MEMBER( mf, K )
	CLONE_MEMBER( mf, L )
}


void CFEFunctorMatrixToGlobalTransformation::Compute( CFiniteElementInstance *fe1 )
{
	fv = ~(*L) * (*K) *  (*L);
}
