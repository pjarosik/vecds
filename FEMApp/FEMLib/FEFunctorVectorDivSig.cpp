#include "FEFunctorVectorDivSig.h"

CFEFunctorVectorDivSIg::CFEFunctorVectorDivSIg( CFEFunctorMatrix *n, CFEFunctorVector *s ):CFEFunctorVector("DivSIg"),N(n),sig(s)
{
    AddNode( N );
    AddNode( sig );
}

CFEFunctorVectorDivSIg::CFEFunctorVectorDivSIg( const CFEFunctorVectorDivSIg &vf ):CFEFunctorVector( vf )
{
    CLONE_MEMBER( vf, N )
    CLONE_MEMBER( vf, sig )
}


void CFEFunctorVectorDivSIg::Compute( CFiniteElementInstance *fe1 )
{
    fv = (*N) * (*sig);
}
