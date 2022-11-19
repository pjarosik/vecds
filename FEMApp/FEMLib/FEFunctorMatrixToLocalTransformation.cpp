#include "FEFunctorMatrixToLocalTransformation.h"


CFEFunctorMatrixToLocalTransformation::CFEFunctorMatrixToLocalTransformation( CFEFunctorMatrix *l, CFEFunctorMatrix *k ):CFEFunctorMatrix("CFEFunctorMatrixToGlobalTransformation"),L(l),K(k)
{

    AddNode( K );
	AddNode( L );
	alr = k->GetRowAllocator();
	alc = k->GetColAllocator();

}

CFEFunctorMatrixToLocalTransformation::CFEFunctorMatrixToLocalTransformation( const CFEFunctorMatrixToLocalTransformation &mf ):CFEFunctorMatrix(mf)
{
    CLONE_MEMBER( mf, K )
	CLONE_MEMBER( mf, L )
}


void CFEFunctorMatrixToLocalTransformation::Compute( CFiniteElementInstance *fe1 )
{
	fv = (*L) * (*K) *  ~(*L);
}
